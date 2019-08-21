#include "map_database.h"
#include <sstream>
#include <algorithm>
#include "UnitTestsFramework.h"


MapDataBase::MapDataBase() {
    color_palette_ = std::make_unique<std::vector<Svg::Color>>();
    color_palette_->reserve(100);

    svg = std::make_unique<Svg::Document>();

    layers_list_ = std::make_unique<std::vector<std::string>>();
    layers_list_->reserve(10);

    layers_object_ = std::make_unique<Layers>();

    stop_name_to_point_ = std::make_unique<std::unordered_map<std::string, Svg::Point>>();
    stop_name_to_point_->reserve(100);

    circles_map_ = std::make_unique<std::map<std::string, Svg::Circle>>();
    bus_to_end_stops_ = std::make_unique<std::map<std::string, std::vector<std::pair<Svg::Point, size_t>>>>();
}


void MapDataBase::AddRenderSettings(const std::map<std::string, Json::Node>& settings_map) {
    width_ = settings_map.at("width").AsDouble();
    height_ = settings_map.at("height").AsDouble();
    padding_ = settings_map.at("padding").AsDouble();
    stop_radius_ = settings_map.at("stop_radius").AsDouble();
    line_width_ = settings_map.at("line_width").AsDouble();
    stop_label_font_size_ = settings_map.at("stop_label_font_size").AsInt();

    const auto offset = settings_map.at("stop_label_offset").AsArray();
    stop_label_offset_ = Svg::Point{offset[0].AsDouble(), offset[1].AsDouble()};

    underlayer_width_ = settings_map.at("underlayer_width").AsDouble();

    underlayer_color_ = get_color_from_json(
            settings_map.at("underlayer_color"));

    for (const auto& json : settings_map.at("color_palette").AsArray())
        color_palette_->push_back(get_color_from_json(json));

    bus_label_font_size_ = settings_map.at("bus_label_font_size").AsInt();
    const auto bus_offset = settings_map.at("bus_label_offset").AsArray();
    bus_label_offset_ = Svg::Point{bus_offset[0].AsDouble(), bus_offset[1].AsDouble()};

    for (const auto& layer : settings_map.at("layers").AsArray()) {
        layers_list_->push_back(layer.AsString());
    }
}


Svg::Color MapDataBase::get_color_from_json(const Json::Node& json) {
    try {
        return Svg::Color(json.AsString());
    }
    catch (std::bad_variant_access&) {
        const auto& color_settings = json.AsArray();
        const size_t size = color_settings.size();

        if (size == 3)
        {
            return Svg::Color(Svg::Rgb{
                static_cast<int>(color_settings[0].AsInt()),
                static_cast<int>(color_settings[1].AsInt()),
                static_cast<int>(color_settings[2].AsInt())
            });
        }

        return Svg::Color(Svg::Rgba{
                static_cast<int>(color_settings[0].AsInt()),
                static_cast<int>(color_settings[1].AsInt()),
                static_cast<int>(color_settings[2].AsInt()),
                color_settings[3].AsDouble()
        });
    }
}


std::string MapDataBase::GetMapResponse() const {
    std::ostringstream out;
    out.precision(16);
    svg->Render(out);

    return out.str();
}


void MapDataBase::BuildMap(
        const std::shared_ptr<BusStopsDataBase>& stops_base,
        const std::shared_ptr<RoutesDataBase>& buses_base) {
    computeStopPoints(stops_base);

    build_layers(stops_base, buses_base);
    build_svg();
}


void MapDataBase::computeStopPoints(
        const std::shared_ptr<BusStopsDataBase>& stops_base
        ) {
    std::unique_ptr<std::vector<TemporaryStopContainer>> sorting_vec =
            std::make_unique<std::vector<TemporaryStopContainer>>();
    sorting_vec->reserve(100);

    for (const auto& [stop_name, stop_stats] : stops_base->GetStopsMap()) {
        sorting_vec->emplace_back(
                TemporaryStopContainer{
                        stop_name,
                        stop_stats->GetCoordinate().GetLongitude(),
                        stop_stats->GetCoordinate().GetLatitude()
                }
                );
    }

    recomputeStopPoints(sorting_vec);
}

void MapDataBase::recomputeStopPoints(
        const std::unique_ptr<std::vector<TemporaryStopContainer>>& sorting_vec
        ) {
    if (sorting_vec->size() == 1) {
        sorting_vec->front().x_ = padding_;
        sorting_vec->front().y_ = height_ - padding_;
    } else {
        std::sort(sorting_vec->begin(), sorting_vec->end(), [](
                const TemporaryStopContainer& lhs,
                const TemporaryStopContainer& rhs) {
            return lhs.x_ < rhs.x_;
        });

        const double x_step = (width_ - 2 * padding_) / static_cast<double>(sorting_vec->size() - 1);
        for (size_t i = 0; i < sorting_vec->size(); ++i) {
            sorting_vec->at(i).x_ = i * x_step + padding_;
        }

        std::sort(sorting_vec->begin(), sorting_vec->end(), [](
                const TemporaryStopContainer& lhs,
                const TemporaryStopContainer& rhs) {
            return lhs.y_ < rhs.y_;
        });
        const double y_step = (height_ - 2 * padding_) / static_cast<double>(sorting_vec->size() - 1);
        for (size_t i = 0; i < sorting_vec->size(); ++i) {
            sorting_vec->at(i).y_ = height_ - padding_ - i * y_step;
        }
    }

    for (const auto& [name, x, y] : *sorting_vec) {
        (*stop_name_to_point_)[name] = Svg::Point{x, y};
    }
}


void MapDataBase::build_layers(
        const std::shared_ptr<BusStopsDataBase>& stops_base,
        const std::shared_ptr<RoutesDataBase>& buses_base
        ) {
    size_t palette_index = 0;
    for (const auto& bus_name : buses_base->GetRouteNames()) {
        build_line(bus_name, stops_base, buses_base, palette_index);
        ++palette_index;
    }

    recompute_circles_map(stops_base);

    build_end_stops();
    build_circles();
    build_texts();
}


void MapDataBase::build_svg() {
    for (const auto& layer_name : *layers_list_) {
        layers_object_->BuildLayer(layer_name, svg);
    }
}


void MapDataBase::build_line(
        const std::string& bus_name,
        const std::shared_ptr<BusStopsDataBase>& stops_base,
        const std::shared_ptr<RoutesDataBase>& buses_base,
        size_t index
        ) {
    index %= color_palette_->size();

    Svg::Polyline polyline;
    polyline.SetStrokeColor(color_palette_->at(index))
    .SetStrokeWidth(line_width_)
    .SetStrokeLineCap("round")
    .SetStrokeLineJoin("round");

    for (const auto& stop : buses_base->GetRouteInfo(bus_name)->GetStops()) {
        const Svg::Point stop_point = getPoint(stop);

        polyline.AddPoint(stop_point);

        update_circles_map(stop, stop_point);
    }
    update_end_stops_map(bus_name, stops_base, buses_base, index);

    layers_object_->AddBusLine(polyline);
}


Svg::Point MapDataBase::getPoint(const std::string &stop_name) {
    return stop_name_to_point_->at(stop_name);
}


void MapDataBase::update_circles_map(
        const std::string& stop_name,
        const Svg::Point& stop_point) {
    const auto finder = circles_map_->find(stop_name);

    if (finder == circles_map_->end()) {
        (*circles_map_)[stop_name] = Svg::Circle()
                .SetCenter(stop_point)
                .SetRadius(stop_radius_)
                .SetFillColor("white");
    }
}


void MapDataBase::update_end_stops_map(
        const std::string& bus_name,
        const std::shared_ptr<BusStopsDataBase>& stops_base,
        const std::shared_ptr<RoutesDataBase>& buses_base,
        size_t palette_index
                ) {
    const auto end_stops = buses_base->GetRouteInfo(bus_name)->GetEndStops();

    for (const auto& stop : end_stops) {
        (*bus_to_end_stops_)[bus_name].push_back(
                std::make_pair(
                        getPoint(stop),
                        palette_index
                )
        );
    }
}


void MapDataBase::recompute_circles_map(
        const std::shared_ptr<BusStopsDataBase>& stops_base
        ) {
    for (const auto& [stop_name, _] : stops_base->GetStopsMap()) {
        const Svg::Point stop_point = getPoint(stop_name);

        update_circles_map(stop_name, stop_point);
    }
}


void MapDataBase::build_end_stops() {
    for (const auto& bus_pair : *bus_to_end_stops_) {
        for (const auto& end_stop : bus_pair.second) {
            Svg::Text text;
            text.SetPoint(end_stop.first)
            .SetOffset(bus_label_offset_)
            .SetFontSize(bus_label_font_size_)
            .SetFontFamily("Verdana")
            .SetFontWeight("bold")
            .SetData(bus_pair.first);

            Svg::Text substrate_text = text;
            substrate_text.SetFillColor(underlayer_color_)
            .SetStrokeColor(underlayer_color_)
            .SetStrokeWidth(underlayer_width_)
            .SetStrokeLineCap("round")
            .SetStrokeLineJoin("round");

            text.SetFillColor((*color_palette_)[end_stop.second]);

            layers_object_->AddBusLabel(substrate_text);
            layers_object_->AddBusLabel(text);
        }
    }
}


void MapDataBase::build_circles() {
    for (const auto& circle_pair : *circles_map_) {
        layers_object_->AddStopPoint(circle_pair.second);
    }
}


void MapDataBase::build_texts() {
    for (const auto& circle_pair : *circles_map_) {
        Svg::Text text;
        text.SetPoint(circle_pair.second.GetCenter())
        .SetOffset(stop_label_offset_)
        .SetFontSize(stop_label_font_size_)
        .SetFontFamily("Verdana")
        .SetData(circle_pair.first);

        Svg::Text substrate_text = text;
        substrate_text.SetFillColor(underlayer_color_)
        .SetStrokeColor(underlayer_color_)
        .SetStrokeWidth(underlayer_width_)
        .SetStrokeLineCap("round")
        .SetStrokeLineJoin("round");

        text.SetFillColor("black");

        layers_object_->AddStopLabel(substrate_text);
        layers_object_->AddStopLabel(text);
    }
}


void TestMapDataBase()
{
    std::istringstream input(R"({ "render_settings": {
        "width": 1200,
        "height": 1200,
        "padding": 50,
        "stop_radius": 5,
        "line_width": 14,
        "bus_label_font_size": 20,
        "bus_label_offset": [
            7,
            15
        ],
        "stop_label_font_size": 20,
        "stop_label_offset": [
            7,
            -3
        ],
        "underlayer_color": [
            255,
            255,
            255,
            0.85
        ],
        "underlayer_width": 3,
        "color_palette": [
            "green",
            [
                255,
                160,
                0
            ],
            "red",
            [
                255,
                255,
                255,
                0.85
            ]
        ],
        "layers": [
            "bus_lines",
            "stop_points",
            "bus_labels"
        ]
    } })");

    const auto json_input = Json::Load(input).GetRoot();

    MapDataBase map_database;

    map_database.AddRenderSettings(
            json_input.AsMap().at("render_settings").AsMap());

    ASSERT(std::abs(map_database.width_ - 1200) < 0.0001)
    ASSERT(std::abs(map_database.height_ - 1200) < 0.0001)
    ASSERT(std::abs(map_database.padding_ - 50) < 0.0001)
    ASSERT(std::abs(map_database.stop_radius_ - 5) < 0.0001)
    ASSERT(std::abs(map_database.height_ - 1200) < 0.0001)
    ASSERT(std::abs(map_database.line_width_ - 14) < 0.0001)
    ASSERT_EQUAL(map_database.bus_label_font_size_, 20)
    ASSERT(std::abs(map_database.bus_label_offset_.x - 7) < 0.0001)
    ASSERT(std::abs(map_database.bus_label_offset_.y - 15) < 0.0001)
    ASSERT_EQUAL(map_database.stop_label_font_size_, 20)
    ASSERT_EQUAL(map_database.underlayer_width_, 3)
    ASSERT(std::abs(map_database.stop_label_offset_.x - 7) < 0.0001)
    ASSERT(std::abs(map_database.stop_label_offset_.y + 3) < 0.0001)

    ASSERT_EQUAL(std::get<Svg::Rgba>(*map_database.underlayer_color_.GetColor()).red, 255)
    ASSERT_EQUAL(std::get<Svg::Rgba>(*map_database.underlayer_color_.GetColor()).green, 255)
    ASSERT_EQUAL(std::get<Svg::Rgba>(*map_database.underlayer_color_.GetColor()).blue, 255)
    ASSERT(std::abs(std::get<Svg::Rgba>(*map_database.underlayer_color_.GetColor()).alpha - 0.85) < 0.0001)

    ASSERT_EQUAL(std::get<std::string>(*map_database.color_palette_->at(0).GetColor()), "green")
    ASSERT_EQUAL(std::get<std::string>(*map_database.color_palette_->at(2).GetColor()), "red")

    ASSERT_EQUAL(std::get<Svg::Rgb>(*map_database.color_palette_->at(1).GetColor()).red, 255)
    ASSERT_EQUAL(std::get<Svg::Rgb>(*map_database.color_palette_->at(1).GetColor()).green, 160)
    ASSERT_EQUAL(std::get<Svg::Rgb>(*map_database.color_palette_->at(1).GetColor()).blue, 0)

    ASSERT_EQUAL(std::get<Svg::Rgba>(*map_database.color_palette_->at(3).GetColor()).red, 255)
    ASSERT_EQUAL(std::get<Svg::Rgba>(*map_database.color_palette_->at(3).GetColor()).green, 255)
    ASSERT_EQUAL(std::get<Svg::Rgba>(*map_database.color_palette_->at(3).GetColor()).blue, 255)
    ASSERT(std::abs(std::get<Svg::Rgba>(*map_database.color_palette_->at(3).GetColor()).alpha - 0.85) < 0.0001)

    ASSERT_EQUAL(map_database.layers_list_->at(0), "bus_lines")
    ASSERT_EQUAL(map_database.layers_list_->at(1), "stop_points")
    ASSERT_EQUAL(map_database.layers_list_->at(2), "bus_labels")
}