#include <algorithm>
#include <unordered_set>
#include <sstream>
#include "map_database.h"
#include "route_map.h"


namespace Map {

    MapDataBase::MapDataBase(
            const std::shared_ptr<BusStopsDataBase> &sdb,
            const std::shared_ptr<RoutesDataBase> &bdb
    ) : stops_base_(sdb), buses_base_(bdb) {
        render_settings_ = std::make_unique<RenderSettings>();
        svg = std::make_unique<Svg::Document>();

        layers_object_ = std::make_unique<Layers>();

        circles_map_ = std::make_unique<std::map<std::string, Svg::Circle>>();
        bus_to_end_stops_ = std::make_unique<std::map<std::string, std::vector<std::pair<Svg::Point, size_t>>>>();
    }

    void MapDataBase::AddRenderSettings(const std::map<std::string, Json::Node> &settings_map) {
        render_settings_->width_ = settings_map.at("width").AsDouble();
        render_settings_->height_ = settings_map.at("height").AsDouble();
        render_settings_->padding_ = settings_map.at("padding").AsDouble();
        render_settings_->stop_radius_ = settings_map.at("stop_radius").AsDouble();
        render_settings_->line_width_ = settings_map.at("line_width").AsDouble();
        render_settings_->stop_label_font_size_ = settings_map.at("stop_label_font_size").AsInt();

        const auto offset = settings_map.at("stop_label_offset").AsArray();
        render_settings_->stop_label_offset_ = Svg::Point{offset[0].AsDouble(), offset[1].AsDouble()};

        render_settings_->underlayer_width_ = settings_map.at("underlayer_width").AsDouble();

        render_settings_->underlayer_color_ = get_color_from_json(
                settings_map.at("underlayer_color"));

        for (const auto &json : settings_map.at("color_palette").AsArray())
            render_settings_->color_palette_->push_back(get_color_from_json(json));

        render_settings_->bus_label_font_size_ = settings_map.at("bus_label_font_size").AsInt();
        const auto bus_offset = settings_map.at("bus_label_offset").AsArray();
        render_settings_->bus_label_offset_ = Svg::Point{bus_offset[0].AsDouble(), bus_offset[1].AsDouble()};

        for (const auto &layer : settings_map.at("layers").AsArray()) {
            render_settings_->layers_list_->push_back(layer.AsString());
        }
        render_settings_->outer_margin_ = settings_map.at("outer_margin").AsDouble();
    }

    Svg::Color MapDataBase::get_color_from_json(const Json::Node &json) {
        try {
            return Svg::Color(json.AsString());
        }
        catch (std::bad_variant_access &) {
            const auto &color_settings = json.AsArray();
            const size_t size = color_settings.size();

            if (size == 3) {
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

    void MapDataBase::BuildMap() {
        pointsComputer = std::make_unique<PointsComputer>(this, stops_base_, buses_base_);
        pointsComputer->ComputeStopPoints();

        build_layers();
        build_svg();
    }

    std::string MapDataBase::GetRouteMapResponse(
            const NavigationResponse &route,
            const std::shared_ptr<NavigationDataBase> &ndb
    ) const {
        std::unique_ptr<RouteMap> route_map =
                std::make_unique<RouteMap>(this, ndb, buses_base_);
        return route_map->GetResponse(route);
    }

    void MapDataBase::build_layers() {
        size_t palette_index = 0;
        for (const auto &bus_name : buses_base_->GetRouteNames()) {
            build_line(bus_name, palette_index);
            ++palette_index;
        }

        recompute_circles_map();

        build_end_stops();
        build_circles();
        build_texts();
    }

    void MapDataBase::build_svg() {
        for (const auto &layer_name : *render_settings_->layers_list_) {
            layers_object_->BuildLayer(layer_name, svg);
        }
    }

    void MapDataBase::build_line(
            const std::string &bus_name,
            size_t index
    ) {
        index %= render_settings_->color_palette_->size();

        Svg::Polyline polyline;
        polyline.SetStrokeColor(render_settings_->color_palette_->at(index))
                .SetStrokeWidth(render_settings_->line_width_)
                .SetStrokeLineCap("round")
                .SetStrokeLineJoin("round");

        for (const auto &stop : buses_base_->GetRouteInfo(bus_name)->GetStops()) {
            const Svg::Point stop_point = pointsComputer->GetPoint(stop);

            polyline.AddPoint(stop_point);

            update_circles_map(stop, stop_point);
        }
        update_end_stops_map(bus_name, index);

        layers_object_->AddBusLine(polyline);
    }

    void MapDataBase::update_circles_map(
            const std::string &stop_name,
            const Svg::Point &stop_point) {
        const auto finder = circles_map_->find(stop_name);

        if (finder == circles_map_->end()) {
            (*circles_map_)[stop_name] = Svg::Circle()
                    .SetCenter(stop_point)
                    .SetRadius(render_settings_->stop_radius_)
                    .SetFillColor("white");
        }
    }

    void MapDataBase::update_end_stops_map(
            const std::string &bus_name,
            size_t palette_index
    ) {
        const auto end_stops = buses_base_->GetRouteInfo(bus_name)->GetEndStops();

        for (const auto &stop : end_stops) {
            (*bus_to_end_stops_)[bus_name].push_back(
                    std::make_pair(
                            pointsComputer->GetPoint(stop),
                            palette_index
                    )
            );
        }
    }

    void MapDataBase::recompute_circles_map() {
        for (const auto&[stop_name, _] : stops_base_->GetStopsMap()) {
            const Svg::Point stop_point = pointsComputer->GetPoint(stop_name);

            update_circles_map(stop_name, stop_point);
        }
    }

    void MapDataBase::build_end_stops() {
        for (const auto &bus_pair : *bus_to_end_stops_) {
            for (const auto &end_stop : bus_pair.second) {
                Svg::Text text;
                text.SetPoint(end_stop.first)
                        .SetOffset(render_settings_->bus_label_offset_)
                        .SetFontSize(render_settings_->bus_label_font_size_)
                        .SetFontFamily("Verdana")
                        .SetFontWeight("bold")
                        .SetData(bus_pair.first);

                Svg::Text substrate_text = text;
                substrate_text.SetFillColor(render_settings_->underlayer_color_)
                        .SetStrokeColor(render_settings_->underlayer_color_)
                        .SetStrokeWidth(render_settings_->underlayer_width_)
                        .SetStrokeLineCap("round")
                        .SetStrokeLineJoin("round");

                text.SetFillColor((*render_settings_->color_palette_)[end_stop.second]);

                layers_object_->AddBusLabel(substrate_text);
                layers_object_->AddBusLabel(text);
            }
        }
    }

    void MapDataBase::build_circles() {
        for (const auto &circle_pair : *circles_map_) {
            layers_object_->AddStopPoint(circle_pair.second);
        }
    }

    void MapDataBase::build_texts() {
        for (const auto &circle_pair : *circles_map_) {
            Svg::Text text;
            text.SetPoint(circle_pair.second.GetCenter())
                    .SetOffset(render_settings_->stop_label_offset_)
                    .SetFontSize(render_settings_->stop_label_font_size_)
                    .SetFontFamily("Verdana")
                    .SetData(circle_pair.first);

            Svg::Text substrate_text = text;
            substrate_text.SetFillColor(render_settings_->underlayer_color_)
                    .SetStrokeColor(render_settings_->underlayer_color_)
                    .SetStrokeWidth(render_settings_->underlayer_width_)
                    .SetStrokeLineCap("round")
                    .SetStrokeLineJoin("round");

            text.SetFillColor("black");

            layers_object_->AddStopLabel(substrate_text);
            layers_object_->AddStopLabel(text);
        }
    }

}