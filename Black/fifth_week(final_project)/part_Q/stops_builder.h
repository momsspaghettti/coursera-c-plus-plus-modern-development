#pragma once

#include <string>
#include <memory>
#include <utility>
#include "stops_database.h"
#include "routes_database.h"


namespace Map {

    struct StopContainer {
        std::string stop_name_;
        double x_;
        double y_;
        size_t index_;
    };

    class ReferenceChecker {
    public:
        ReferenceChecker(
                const std::shared_ptr<BusStopsDataBase> &sdb,
                const std::shared_ptr<RoutesDataBase> &bdb) :
                stops_base_(sdb), buses_base_(bdb) {
            cache_ = std::make_unique<std::unordered_map<std::string, bool>>();
            cache_->reserve(stops_base_->GetBusStopsCount());
        }

        [[nodiscard]] bool IsReferenceStop(const std::string &, const std::string &);

    private:
        const std::shared_ptr<BusStopsDataBase> &stops_base_;
        const std::shared_ptr<RoutesDataBase> &buses_base_;
        std::unique_ptr<std::unordered_map<std::string, bool>> cache_;

        [[nodiscard]] bool updateAndReturn(const std::string &stop, bool result) {
            (*cache_)[stop] = result;
            return result;
        }
    };


    class StopsBuilder {
    public:
        StopsBuilder(const std::shared_ptr<BusStopsDataBase> &sdb,
                     const std::shared_ptr<RoutesDataBase> &bdb) :
                stops_base_(sdb), buses_base_(bdb) {
            stops_vec_ = std::make_unique<std::vector<StopContainer>>();
            stops_vec_->reserve(stops_base_->GetBusStopsCount());

            built_stops_ = std::make_unique<std::unordered_set<std::string>>();
            built_stops_->reserve(stops_vec_->size());
        }

        std::unique_ptr<std::vector<StopContainer>> BuildStopsVec();

    private:
        const std::shared_ptr<BusStopsDataBase> &stops_base_;
        const std::shared_ptr<RoutesDataBase> &buses_base_;

        std::unique_ptr<std::vector<StopContainer>> stops_vec_;
        std::unique_ptr<std::unordered_set<std::string>> built_stops_;

        std::unordered_map<std::string, StopContainer> buildStopsForBus(
                const std::vector<std::pair<std::string, bool>> &
        );

        void insertNewStop(
                std::vector<StopContainer> &,
                const std::string &,
                double,
                double
        );
    };
}