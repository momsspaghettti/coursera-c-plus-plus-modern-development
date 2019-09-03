#pragma once

#include "json.h"
#include "layers.h"
#include "routes_database.h"
#include "points_computer.h"
#include "navigation_database.h"


namespace Map {

    struct RenderSettings {
        RenderSettings() {
            color_palette_ = std::make_unique<std::vector<Svg::Color>>();
            color_palette_->reserve(100);
            layers_list_ = std::make_unique<std::vector<std::string>>();
            layers_list_->reserve(10);
        }

        double width_{}, height_{},
                padding_{}, stop_radius_{}, line_width_{};

        int stop_label_font_size_{};
        Svg::Point stop_label_offset_{};
        Svg::Color underlayer_color_;

        double underlayer_width_{};
        std::unique_ptr<std::vector<Svg::Color>> color_palette_;

        int bus_label_font_size_{};
        Svg::Point bus_label_offset_{};

        std::unique_ptr<std::vector<std::string>> layers_list_;
        double outer_margin_{};
    };

    class MapDataBase {
    public:
        MapDataBase(
                const std::shared_ptr<BusStopsDataBase> &,
                const std::shared_ptr<RoutesDataBase> &
        );

        void AddRenderSettings(const std::map<std::string, Json::Node> &);

        void BuildMap();

        [[nodiscard]] std::string GetMapResponse() const;

        [[nodiscard]] std::string GetRouteMapResponse(
                const NavigationResponse &,
                const std::shared_ptr<NavigationDataBase> &
        ) const;

    private:
        const std::shared_ptr<BusStopsDataBase> &stops_base_;
        const std::shared_ptr<RoutesDataBase> &buses_base_;

        std::unique_ptr<RenderSettings> render_settings_;

        std::unique_ptr<Svg::Document> svg;

        void build_svg();

        std::unique_ptr<Layers> layers_object_;

        void build_layers();

        static Svg::Color get_color_from_json(const Json::Node &);

        std::unique_ptr<PointsComputer> pointsComputer;

        friend class PointsComputer;

        friend class RouteMap;

        void build_line(
                const std::string &,
                size_t index
        );

        void build_end_stops();

        void build_circles();

        void build_texts();

        void update_circles_map(const std::string &, const Svg::Point &);

        void recompute_circles_map();

        std::unique_ptr<std::map<std::string, Svg::Circle>> circles_map_;

        std::unique_ptr<std::map<std::string, std::vector<std::pair<Svg::Point, size_t>>>> bus_to_end_stops_;

        void update_end_stops_map(
                const std::string &,
                size_t
        );
    };

}