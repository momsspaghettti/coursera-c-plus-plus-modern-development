#pragma once
#include "json.h"
#include "UnitTestsFramework.h"
#include "layers.h"
#include "routes_database.h"
#include "points_computer.h"


namespace Map {

    class MapDataBase
    {
    public:
        MapDataBase();

        void AddRenderSettings(const std::map<std::string, Json::Node>&);

        void BuildMap(const std::shared_ptr<BusStopsDataBase>&, const std::shared_ptr<RoutesDataBase>&);

        [[nodiscard]] std::string GetMapResponse() const;

    private:
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

        std::unique_ptr<Svg::Document> svg;
        void build_svg();

        std::unique_ptr<Layers> layers_object_;
        void build_layers(const std::shared_ptr<BusStopsDataBase>&,
                          const std::shared_ptr<RoutesDataBase>&);

        static Svg::Color get_color_from_json(const Json::Node&);

        std::unique_ptr<PointsComputer> pointsComputer;
        friend class PointsComputer;

        void build_line(
                const std::string&,
                const std::shared_ptr<BusStopsDataBase>&,
                const std::shared_ptr<RoutesDataBase>&,
                size_t index
        );

        void build_end_stops();

        void build_circles();

        void build_texts();

        void update_circles_map(const std::string&, const Svg::Point&);
        void recompute_circles_map(const std::shared_ptr<BusStopsDataBase>&);

        std::unique_ptr<std::map<std::string, Svg::Circle>> circles_map_;

        std::unique_ptr<std::map<std::string, std::vector<std::pair<Svg::Point, size_t>>>> bus_to_end_stops_;

        void update_end_stops_map(
                const std::string&,
                const std::shared_ptr<BusStopsDataBase>&,
                const std::shared_ptr<RoutesDataBase>&,
                size_t
        );

        friend void TestRenderSettings();
    };

    void TestMapDataBase(TestRunner& tr);
}