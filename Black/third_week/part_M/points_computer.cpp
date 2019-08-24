#include "points_computer.h"
#include "map_database.h"


namespace Map {

    void PointsComputer::ComputeStopPoints() {
        std::unique_ptr<StopsBuilder> stopsBuilder =
                std::make_unique<StopsBuilder>(stops_base_, buses_base_);

        std::unique_ptr<std::vector<StopContainer>> sorting_vec =
                std::move(stopsBuilder->BuildStopsVec());

        updateX(sorting_vec);
        updateY(sorting_vec);
        for (const auto &container : *sorting_vec) {
            (*stop_name_to_point_)[container.stop_name_] = Svg::Point{
                    container.x_,
                    container.y_
            };
        }
    }

    void PointsComputer::updateX(
            const std::unique_ptr<std::vector<StopContainer>> &sorting_vec
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
            const auto max_index = updateSortingVec(sorting_vec);
            const double x_step = max_index == 0 ? 0 :
                                  (master_->width_ - 2 * master_->padding_) / max_index;
            for (auto &container : *sorting_vec) {
                container.x_ = container.index_ * x_step + master_->padding_;
            }
        }
    }

    void PointsComputer::updateY(
            const std::unique_ptr<std::vector<StopContainer>> &sorting_vec
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
            const auto max_index = updateSortingVec(sorting_vec);
            const double y_step = max_index == 0 ? 0 :
                                  (master_->height_ - 2 * master_->padding_) / max_index;
            for (auto &container : *sorting_vec) {
                container.y_ = master_->height_ - master_->padding_ - container.index_ * y_step;
            }
        }
    }

    size_t PointsComputer::updateSortingVec(
            const std::unique_ptr<std::vector<StopContainer>> &sorting_vec
    ) {
        size_t index = 0;
        for (auto &container : *sorting_vec) {
            container.index_ = index;
            ++index;
        }

        size_t max_index = 0;
        for (size_t i = 1; i < sorting_vec->size(); ++i) {
            const auto remain_response = needToRemainIndex(sorting_vec, i);
            if (!remain_response.need_to_remain_index) {
                const auto new_index = *remain_response.new_index;
                sorting_vec->at(i).index_ = new_index;
                if (new_index > max_index) {
                    max_index = new_index;
                }
            } else {
                sorting_vec->at(i).index_ = 0;
            }
        }

        return max_index;
    }

    PointsComputer::RemainIndexResponse PointsComputer::needToRemainIndex(
            const std::unique_ptr<std::vector<StopContainer>> &stops_vec,
            size_t target_index
    ) {
        bool has_neighbors_among_minors = false;
        size_t new_index = 0;
        for (size_t i = 0; i < target_index; ++i) {
            if (check_stops_to_neighborhood(
                    stops_vec->at(i).stop_name_,
                    stops_vec->at(target_index).stop_name_
            )) {
                has_neighbors_among_minors = true;
                if (stops_vec->at(i).index_ > new_index)
                    new_index = stops_vec->at(i).index_;
            }
        }

        if (has_neighbors_among_minors) {
            return RemainIndexResponse {
                    false,
                    new_index + 1
            };
        }

        return RemainIndexResponse{
                true,
                std::nullopt
        };
    }

    bool PointsComputer::check_stops_to_neighborhood(
            const std::string &first,
            const std::string &second
    ) {
        const auto &first_buses = stops_base_->GetBuses(first);
        const auto &second_buses = stops_base_->GetBuses(second);

        for (const auto &bus : first_buses) {
            const auto finder = second_buses.find(bus);
            if (finder != second_buses.end()) {
                if (buses_base_->GetRouteInfo(bus)->AreStopsNeighbors(first, second)) {
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