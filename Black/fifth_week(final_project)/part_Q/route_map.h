#pragma once

#include "stops_database.h"
#include "routes_database.h"
#include "svg.h"
#include "map_database.h"
#include "layers.h"


namespace Map {
    class MapDataBase;

    class RouteMap {
    public:
        RouteMap(
                const MapDataBase *m,
                const std::shared_ptr<NavigationDataBase> &ndb,
                const std::shared_ptr<RoutesDataBase> &bdb
        ) : master_(m), navigation_base_(ndb), buses_base_(bdb) {
            svg_ = std::make_unique<Svg::Document>(*master_->svg);
            layers_object_ = std::make_unique<Layers>();
        };

        std::string GetResponse(const NavigationResponse &);

    private:
        const MapDataBase *master_;
        const std::shared_ptr<NavigationDataBase> &navigation_base_;
        const std::shared_ptr<RoutesDataBase> &buses_base_;

        std::unique_ptr<Svg::Document> svg_;
        std::unique_ptr<Layers> layers_object_;

        void build_map(const NavigationResponse &) const;

        void set_rect() const;

        void build_line(const std::string &, const std::vector<std::string> &) const;

        void build_bus_labels(
                const std::string &,
                const std::vector<std::string> &,
                size_t
        ) const;

        void build_stop_point(const std::string &stop) const;

        void build_stop_label(const std::string &stop) const;
    };
}