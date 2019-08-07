#pragma once
#include "svg.h"
#include <vector>
#include "json.h"
#include "stops_database.h"
#include "routes_database.h"


class Layers
{
public:
    Layers() {
        stop_points_.reserve(100);
        bus_lines_.reserve(100);
        bus_labels_.reserve(400);
        stop_labels_.reserve(200);
    }

    void AddStopPoint(const Svg::Circle& p) {
        stop_points_.push_back(p);
    }

    void AddBusLine(const Svg::Polyline& l) {
        bus_lines_.push_back(l);
    }

    void AddBusLabel(const Svg::Text& l) {
        bus_labels_.push_back(l);
    }

    void AddStopLabel(const Svg::Text& l) {
        stop_labels_.push_back(l);
    }

    void BuildStopPoints(const std::unique_ptr<Svg::Document>& svg) const {
        for (const auto& point : stop_points_)
            svg->Add(point);
    }

    void BuildBusLines(const std::unique_ptr<Svg::Document>& svg) const {
        for (const auto& line : bus_lines_)
            svg->Add(line);
    }

    void BuildBusLabels(const std::unique_ptr<Svg::Document>& svg) const {
        for (const auto& label : bus_labels_)
            svg->Add(label);
    }

    void BuildStopLabels(const std::unique_ptr<Svg::Document>& svg) const {
        for (const auto& label : stop_labels_)
            svg->Add(label);
    }

private:
    std::vector<Svg::Circle> stop_points_;
    std::vector<Svg::Polyline> bus_lines_;
    std::vector<Svg::Text> bus_labels_;
    std::vector<Svg::Text> stop_labels_;
};


class MapDataBase
{
public:
    MapDataBase()
    {
        color_palette_.reserve(100);
        svg = std::make_unique<Svg::Document>();
        layers_list_.reserve(10);
        layers_object_ = std::make_unique<Layers>();
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

    std::vector<std::string> layers_list_;

    GroundPoint min_ground_point_, max_ground_point_;

    std::unique_ptr<Svg::Document> svg;
    void build_svg();

    std::unique_ptr<Layers> layers_object_;
    void build_layers(const std::shared_ptr<BusStopsDataBase>&,
                      const std::shared_ptr<RoutesDataBase>&);

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