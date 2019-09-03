#pragma once

#include <utility>
#include <optional>
#include "stops_database.h"
#include "routes_database.h"
#include "svg.h"
#include "stops_builder.h"


namespace Map {
    class MapDataBase;

    class PointsComputer {
    public:
        PointsComputer(
                const MapDataBase *m,
                const std::shared_ptr<BusStopsDataBase> &sdb,
                const std::shared_ptr<RoutesDataBase> &bdb
        ) : master_(m), stops_base_(sdb), buses_base_(bdb) {
            stop_name_to_point_ = std::make_unique<std::unordered_map<std::string, Svg::Point>>();
            stop_name_to_point_->reserve(100);
        }

        void ComputeStopPoints();

        [[nodiscard]] Svg::Point GetPoint(const std::string &stop_name);

    private:
        const MapDataBase *master_;

        const std::shared_ptr<BusStopsDataBase> &stops_base_;
        const std::shared_ptr<RoutesDataBase> &buses_base_;

        std::unique_ptr<std::unordered_map<std::string, Svg::Point>> stop_name_to_point_;

        void updateX(
                const std::unique_ptr<std::vector<StopContainer>> &
        );

        void updateY(
                const std::unique_ptr<std::vector<StopContainer>> &
        );

        struct RemainIndexResponse {
            bool need_to_remain_index = true;
            std::optional<size_t> new_index;
        };

        [[nodiscard]] size_t updateSortingVec(
                const std::unique_ptr<std::vector<StopContainer>> &
        );

        RemainIndexResponse needToRemainIndex(
                const std::unique_ptr<std::vector<StopContainer>> &,
                size_t target_index
        );

        bool check_stops_to_neighborhood(
                const std::string &first,
                const std::string &second
        );
    };
}