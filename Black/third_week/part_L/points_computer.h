#pragma once
#include <utility>
#include "stops_database.h"
#include "routes_database.h"
#include "svg.h"
#include "stops_builder.h"


namespace Map {
    class MapDataBase;

    class PointsComputer {
    public:
        explicit PointsComputer(MapDataBase* m) : master_(m) {
            stop_name_to_point_ = std::make_unique<std::unordered_map<std::string, Svg::Point>>();
            stop_name_to_point_->reserve(100);
        }

        void ComputeStopPoints(
                const std::shared_ptr<BusStopsDataBase>&,
                const std::shared_ptr<RoutesDataBase>&);

        [[nodiscard]] Svg::Point GetPoint(const std::string& stop_name);

    private:
        MapDataBase* master_;

        std::unique_ptr<std::unordered_map<std::string, Svg::Point>> stop_name_to_point_;

        void updateX(
                const std::unique_ptr<std::vector<StopContainer>>&,
                const std::shared_ptr<BusStopsDataBase>&,
                const std::shared_ptr<RoutesDataBase>&
        );

        void updateY(
                const std::unique_ptr<std::vector<StopContainer>>&,
                const std::shared_ptr<BusStopsDataBase>&,
                const std::shared_ptr<RoutesDataBase>&
        );

        static void updateSortingVec(
                const std::unique_ptr<std::vector<StopContainer>>&,
                const std::shared_ptr<BusStopsDataBase>&,
                const std::shared_ptr<RoutesDataBase>&
        );

        static bool check_stops_to_neighborhood(
                const std::string& first,
                const std::string& second,
                const std::shared_ptr<BusStopsDataBase>& stops_base,
                const std::shared_ptr<RoutesDataBase>& buses_base
        );
    };
}