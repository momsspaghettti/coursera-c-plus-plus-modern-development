#pragma once
#include "svg.h"
#include <vector>
#include "json.h"
#include "stops_database.h"
#include "routes_database.h"


class MapDataBase
{
public:
    MapDataBase()
    {
        color_palette_.reserve(100);
        svg = std::make_unique<Svg::Document>();
    }

    void AddRenderSettings(const std::map<std::string, Json::Node>&);

    void AddMinMaxCoords(std::pair<GroundPoint, GroundPoint> ground_pair)
    {
        min_ground_point_ = ground_pair.first;
        max_ground_point_ = ground_pair.second;
    }

    void BuildMap(const std::shared_ptr<BusStopsDataBase>&, const std::shared_ptr<RoutesDataBase>&);

    [[nodiscard]] std::string GetMapResponse() const;

private:
    double width_{}, height_{},
    padding_{}, stop_radius_{}, line_width_{};

    int stop_label_font_size_{};
    Svg::Point stop_label_offset_{};
    Svg::Color underlayer_color_;

    double underlayer_width_{};
    std::vector<Svg::Color> color_palette_;

    int bus_label_font_size_{};
    Svg::Point bus_label_offset_{};

    GroundPoint min_ground_point_, max_ground_point_;

    std::unique_ptr<Svg::Document> svg;

    static Svg::Color get_color_from_json(const Json::Node&);

    [[nodiscard]] Svg::Point compute_point_from_coord(const GroundPoint&) const;

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

    std::map<std::string, Svg::Circle> circles_map_;

    std::map<std::string, std::vector<std::pair<Svg::Point, size_t>>> bus_to_end_stops_;

    void update_end_stops_map(
            const std::string&,
            const std::shared_ptr<BusStopsDataBase>&,
            const std::shared_ptr<RoutesDataBase>&,
            size_t
                    );

    friend void TestMapDataBase();
};


void TestMapDataBase();