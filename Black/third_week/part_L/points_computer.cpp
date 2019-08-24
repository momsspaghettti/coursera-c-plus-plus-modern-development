#include "points_computer.h"
#include "map_database.h"


namespace Map {

    void PointsComputer::ComputeStopPoints(
            const std::shared_ptr<BusStopsDataBase> &stops_base,
            const std::shared_ptr<RoutesDataBase> &buses_base
    ) {
        std::unique_ptr<StopsBuilder> stopsBuilder =
                std::make_unique<StopsBuilder>(stops_base, buses_base);

        std::unique_ptr<std::vector<StopContainer>> sorting_vec =
                std::move(stopsBuilder->BuildStopsVec());

        updateX(sorting_vec, stops_base, buses_base);
        updateY(sorting_vec, stops_base, buses_base);
        for (const auto &container : *sorting_vec) {
            (*stop_name_to_point_)[container.stop_name_] = Svg::Point{
                    container.x_,
                    container.y_
            };
        }
    }

    void PointsComputer::updateX(
            const std::unique_ptr<std::vector<StopContainer>> &sorting_vec,
            const std::shared_ptr<BusStopsDataBase> &stops_base,
            const std::shared_ptr<RoutesDataBase> &buses_base
    ) {
        std::sort(sorting_vec->begin(), sorting_vec->end(), [](
                const StopContainer &lhs,
                const StopContainer &rhs
        ) {
            return lhs.x_ < rhs.x_;
        });

        if (sorting_vec->size() == 1) {
            sorting_vec->front().x_ = master_->padding_;
        } else {
            updateSortingVec(sorting_vec, stops_base, buses_base);
            const double x_step = sorting_vec->back().index_ == 0 ? 0 :
                                  (master_->width_ - 2 * master_->padding_) / sorting_vec->back().index_;
            for (auto &container : *sorting_vec) {
                container.x_ = container.index_ * x_step + master_->padding_;
            }
        }
    }

    void PointsComputer::updateY(
            const std::unique_ptr<std::vector<StopContainer>> &sorting_vec,
            const std::shared_ptr<BusStopsDataBase> &stops_base,
            const std::shared_ptr<RoutesDataBase> &buses_base
    ) {
        std::sort(sorting_vec->begin(), sorting_vec->end(), [](
                const StopContainer &lhs,
                const StopContainer &rhs
        ) {
            return lhs.y_ < rhs.y_;
        });

        if (sorting_vec->size() == 1) {
            sorting_vec->front().y_ = master_->height_ - master_->padding_;
        } else {
            updateSortingVec(sorting_vec, stops_base, buses_base);
            const double y_step = sorting_vec->back().index_ == 0 ? 0 :
                                  (master_->height_ - 2 * master_->padding_) / sorting_vec->back().index_;
            for (auto &container : *sorting_vec) {
                container.y_ = master_->height_ - master_->padding_ - container.index_ * y_step;
            }
        }
    }

    void PointsComputer::updateSortingVec(
            const std::unique_ptr<std::vector<StopContainer>> &sorting_vec,
            const std::shared_ptr<BusStopsDataBase> &stops_base,
            const std::shared_ptr<RoutesDataBase> &buses_base
    ) {
        size_t index = 0;
        for (auto &container : *sorting_vec) {
            container.index_ = index;
            ++index;
        }

        std::unique_ptr<std::unordered_map<size_t, std::unordered_set<std::string>>> index_to_stops_set =
                std::make_unique<std::unordered_map<size_t, std::unordered_set<std::string>>>();
        index_to_stops_set->reserve(sorting_vec->size());

        (*index_to_stops_set)[0].insert(sorting_vec->front().stop_name_);
        size_t current_index = 0;
        for (size_t i = 1; i < sorting_vec->size(); ++i) {
            bool remain_current_index = true;
            for (const auto &previous_stop_name : index_to_stops_set->at(current_index)) {
                if (check_stops_to_neighborhood(
                        previous_stop_name,
                        sorting_vec->at(i).stop_name_,
                        stops_base,
                        buses_base
                )) {
                    remain_current_index = false;
                    break;
                }
            }
            if (!remain_current_index) {
                ++current_index;
            }
            (*index_to_stops_set)[current_index].insert(sorting_vec->at(i).stop_name_);
            sorting_vec->at(i).index_ = current_index;
        }
    }

    bool PointsComputer::check_stops_to_neighborhood(
            const std::string &first,
            const std::string &second,
            const std::shared_ptr<BusStopsDataBase> &stops_base,
            const std::shared_ptr<RoutesDataBase> &buses_base
    ) {
        const auto &first_buses = stops_base->GetBuses(first);
        const auto &second_buses = stops_base->GetBuses(second);

        for (const auto &bus : first_buses) {
            const auto finder = second_buses.find(bus);
            if (finder != second_buses.end()) {
                if (buses_base->GetRouteInfo(bus)->AreStopsNeighbors(first, second)) {
                    return true;
                }
            }
        }
        return false;
    }


    Svg::Point PointsComputer::GetPoint(const std::string &stop_name) {
        return stop_name_to_point_->at(stop_name);
    }
}