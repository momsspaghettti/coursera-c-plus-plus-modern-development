#include <sstream>
#include "route_map.h"
#include "map_database.h"


namespace Map {

    std::string RouteMap::GetResponse(
            const NavigationResponse &route) {
        build_map(route);

        std::ostringstream out;
        out.precision(16);
        svg_->Render(out);
        return out.str();
    }

    void RouteMap::build_map(const NavigationResponse &route) const {
        set_rect();
        size_t index = 0;
        for (const auto &route_part : route.items) {
            if (route_part->GetType() == "Bus") {
                build_line(
                        route_part->GetName().second,
                        navigation_base_->GetStopsFromEdgeId(route_part->GetEdgeId())
                );
                if (index + 1 == route.items.size()) {
                    build_stop_label(
                            *navigation_base_->GetStopsFromEdgeId(route_part->GetEdgeId()).crbegin()
                    );
                }
            }
            if (route_part->GetType() == "Wait") {
                build_stop_label(route_part->GetName().second);
            }
            ++index;
        }
        for (const auto &layer : *master_->render_settings_->layers_list_) {
            layers_object_->BuildLayer(layer, svg_);
        }
    }

    void RouteMap::build_line(
            const std::string &bus,
            const std::vector<std::string> &stops
    ) const {
        const auto palette_index = master_->bus_to_end_stops_->at(bus)[0].second;
        Svg::Polyline polyline;
        polyline.SetStrokeColor(master_->render_settings_->color_palette_->at(palette_index))
                .SetStrokeWidth(master_->render_settings_->line_width_)
                .SetStrokeLineCap("round")
                .SetStrokeLineJoin("round");

        for (const auto &stop : stops) {
            polyline.AddPoint(master_->pointsComputer->GetPoint(stop));
            build_stop_point(stop);
        }
        layers_object_->AddBusLine(polyline);
        build_bus_labels(bus, stops, palette_index);
    }

    void RouteMap::build_stop_point(const std::string &stop) const {
        layers_object_->AddStopPoint(master_->circles_map_->at(stop));
    }

    void RouteMap::build_bus_labels(
            const std::string &bus,
            const std::vector<std::string> &stops,
            size_t palette_index
    ) const {
        std::vector<std::string> end_stops;
        end_stops.reserve(2);
        if (buses_base_->GetRouteInfo(bus)->IsEndStop(*stops.cbegin())) {
            end_stops.push_back(*stops.cbegin());
        }
        if (buses_base_->GetRouteInfo(bus)->IsEndStop(*stops.crbegin())) {
            end_stops.push_back(*stops.crbegin());
        }
        for (const auto &end_stop : end_stops) {
            Svg::Text text;
            text.SetPoint(master_->pointsComputer->GetPoint(end_stop))
                    .SetOffset(master_->render_settings_->bus_label_offset_)
                    .SetFontSize(master_->render_settings_->bus_label_font_size_)
                    .SetFontFamily("Verdana")
                    .SetFontWeight("bold")
                    .SetData(bus);

            Svg::Text substrate_text = text;
            substrate_text.SetFillColor(master_->render_settings_->underlayer_color_)
                    .SetStrokeColor(master_->render_settings_->underlayer_color_)
                    .SetStrokeWidth(master_->render_settings_->underlayer_width_)
                    .SetStrokeLineCap("round")
                    .SetStrokeLineJoin("round");

            text.SetFillColor(master_->render_settings_->color_palette_->at(palette_index));
            layers_object_->AddBusLabel(substrate_text);
            layers_object_->AddBusLabel(text);
        }
    }

    void RouteMap::build_stop_label(const std::string &stop) const {
        Svg::Text text;
        text.SetPoint(master_->pointsComputer->GetPoint(stop))
                .SetOffset(master_->render_settings_->stop_label_offset_)
                .SetFontSize(master_->render_settings_->stop_label_font_size_)
                .SetFontFamily("Verdana")
                .SetData(stop);

        Svg::Text substrate_text = text;
        substrate_text.SetFillColor(master_->render_settings_->underlayer_color_)
                .SetStrokeColor(master_->render_settings_->underlayer_color_)
                .SetStrokeWidth(master_->render_settings_->underlayer_width_)
                .SetStrokeLineCap("round")
                .SetStrokeLineJoin("round");

        text.SetFillColor("black");

        layers_object_->AddStopLabel(substrate_text);
        layers_object_->AddStopLabel(text);
    }

    void RouteMap::set_rect() const {
        Svg::Rect rect;
        rect.SetCorner(Svg::Point{
                        -master_->render_settings_->outer_margin_,
                        -master_->render_settings_->outer_margin_})
                .SetWidth(
                        master_->render_settings_->width_ + 2 * master_->render_settings_->outer_margin_)
                .SetHeight(master_->render_settings_->height_ + 2 * master_->render_settings_->outer_margin_)
                .SetFillColor(master_->render_settings_->underlayer_color_);
        svg_->Add(rect);
    }

}