#include "stops_builder.h"
#include "UnitTestsFramework.h"


namespace Map {

    bool ReferenceChecker::IsReferenceStop(const std::string &stop, const std::string &bus) {
        const auto finder = cache_->find(stop);
        if (finder != cache_->end()) {
            return finder->second;
        } else {
            const auto end_stops = buses_base_->GetRouteInfo(bus)->GetEndStops();
            for (const auto &end_stop : end_stops) {
                if (stop == end_stop) {
                    return updateAndReturn(stop, true);
                }
            }

            const size_t count_buses_through_stop = stops_base_->GetBuses(stop).size();
            if (count_buses_through_stop > 1) {
                return updateAndReturn(stop, true);
            }

            const size_t pass_count_through_stop = buses_base_->GetRouteInfo(bus)->GetPassCountThroughStop(stop);
            return updateAndReturn(stop, pass_count_through_stop > 2);
        }
    }


    std::unique_ptr<std::vector<StopContainer>> StopsBuilder::BuildStopsVec() {
        std::unique_ptr<ReferenceChecker> referenceChecker =
                std::make_unique<ReferenceChecker>(stops_base_, buses_base_);

        for (const auto &single_stop : stops_base_->GetSingleStops()) {
            const auto &coordinate = stops_base_->GetStopInfo(single_stop)->GetCoordinate();
            insertNewStop(
                    *stops_vec_,
                    single_stop,
                    coordinate.GetLongitude(),
                    coordinate.GetLatitude()
            );
        }

        for (const auto&[bus_name, bus_info_ptr] : buses_base_->GetRoutes()) {
            std::vector<std::pair<std::string, bool>> this_bus_stops;
            this_bus_stops.reserve(bus_info_ptr->GetRouteStats().stops_on_route);

            for (const auto &stop : bus_info_ptr->GetStops()) {
                this_bus_stops.emplace_back(
                        std::make_pair(
                                stop,
                                referenceChecker->IsReferenceStop(stop, bus_name))
                );
            }
            Assert(this_bus_stops.front().second, "First stop is always reference!");
            Assert(this_bus_stops.back().second, "Last stop is always reference!");

            for (const auto &[stop_name, stop_container] : buildStopsForBus(this_bus_stops)) {
                insertNewStop(
                        *stops_vec_,
                        stop_name,
                        stop_container.x_,
                        stop_container.y_
                );
            }
        }

        return std::move(stops_vec_);
    }

    std::unordered_map<std::string, StopContainer> StopsBuilder::buildStopsForBus(
            const std::vector<std::pair<std::string, bool>> &bus_stops
    ) {
        std::unordered_map<std::string, StopContainer> result;
        result.reserve(bus_stops.size());

        size_t first_ref = 0;
        size_t second_ref = 0;

        do {
            const auto &first_ref_name = bus_stops[first_ref].first;
            result[first_ref_name] = StopContainer{
                    first_ref_name,
                    stops_base_->GetStopInfo(first_ref_name)->GetCoordinate().GetLongitude(),
                    stops_base_->GetStopInfo(first_ref_name)->GetCoordinate().GetLatitude(),
                    0
            };

            for (size_t i = second_ref + 1; i < bus_stops.size(); ++i) {
                if (bus_stops[i].second) {
                    second_ref = i;
                    break;
                }
            }
            if (first_ref != second_ref) {
                const auto &firts_coordinate = stops_base_->GetStopInfo(bus_stops[first_ref].first)->GetCoordinate();
                const auto &second_coordinate = stops_base_->GetStopInfo(bus_stops[second_ref].first)->GetCoordinate();
                const double lon_step = (second_coordinate.GetLongitude() - firts_coordinate.GetLongitude()) /
                                        static_cast<double>(second_ref - first_ref);
                const double lat_step = (second_coordinate.GetLatitude() - firts_coordinate.GetLatitude()) /
                                        static_cast<double>(second_ref - first_ref);
                for (size_t k = first_ref + 1; k < second_ref; ++k) {
                    const auto &item = bus_stops[k];
                    Assert(!item.second, "This stop should be not a reference!");
                    result[item.first] = StopContainer{
                            item.first,
                            firts_coordinate.GetLongitude() + lon_step * static_cast<double>(k - first_ref),
                            firts_coordinate.GetLatitude() + lat_step * static_cast<double>(k - first_ref),
                            0
                    };
                }
                const auto &second_ref_name = bus_stops[second_ref].first;
                result[second_ref_name] = StopContainer{
                        second_ref_name,
                        stops_base_->GetStopInfo(second_ref_name)->GetCoordinate().GetLongitude(),
                        stops_base_->GetStopInfo(second_ref_name)->GetCoordinate().GetLatitude(),
                        0
                };
            }

            first_ref = second_ref;
        } while (second_ref + 1 != bus_stops.size());

        return result;
    }

    void StopsBuilder::insertNewStop(
            std::vector<StopContainer> &target,
            const std::string &name,
            double x,
            double y
    ) {
        const auto finder = built_stops_->find(name);
        if (finder == built_stops_->end()) {
            target.emplace_back(StopContainer{name, x, y, 0});
            built_stops_->insert(name);
        }
    }

}