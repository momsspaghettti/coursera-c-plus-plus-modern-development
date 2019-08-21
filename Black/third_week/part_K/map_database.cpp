#include "map_database.h"
#include <sstream>
#include <algorithm>
#include <unordered_set>


MapDataBase::MapDataBase() {
    color_palette_ = std::make_unique<std::vector<Svg::Color>>();
    color_palette_->reserve(100);

    svg = std::make_unique<Svg::Document>();

    layers_list_ = std::make_unique<std::vector<std::string>>();
    layers_list_->reserve(10);

    layers_object_ = std::make_unique<Layers>();

    pointsComputer = std::make_unique<PointsComputer>(this);

    circles_map_ = std::make_unique<std::map<std::string, Svg::Circle>>();
    bus_to_end_stops_ = std::make_unique<std::map<std::string, std::vector<std::pair<Svg::Point, size_t>>>>();
}


void MapDataBase::AddRenderSettings(const std::map<std::string, Json::Node> &settings_map) {
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

    for (const auto &json : settings_map.at("color_palette").AsArray())
        color_palette_->push_back(get_color_from_json(json));

    bus_label_font_size_ = settings_map.at("bus_label_font_size").AsInt();
    const auto bus_offset = settings_map.at("bus_label_offset").AsArray();
    bus_label_offset_ = Svg::Point{bus_offset[0].AsDouble(), bus_offset[1].AsDouble()};

    for (const auto &layer : settings_map.at("layers").AsArray()) {
        layers_list_->push_back(layer.AsString());
    }
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


void MapDataBase::BuildMap(
        const std::shared_ptr<BusStopsDataBase> &stops_base,
        const std::shared_ptr<RoutesDataBase> &buses_base) {
    pointsComputer->ComputeStopPoints(stops_base, buses_base);

    build_layers(stops_base, buses_base);
    build_svg();
}


void PointsComputer::ComputeStopPoints(
        const std::shared_ptr<BusStopsDataBase> &stops_base,
        const std::shared_ptr<RoutesDataBase> &buses_base
) {
    std::unique_ptr<std::vector<StopContainer>> sorting_vec =
            std::make_unique<std::vector<StopContainer>>();
    sorting_vec->reserve(stops_base->GetBusStopsCount());
    for (const auto&[stop_name, stop_stats_ptr] : stops_base->GetStopsMap()) {
        sorting_vec->emplace_back(
                StopContainer{
                        stop_name,
                        stop_stats_ptr->GetCoordinate().GetLongitude() * 10000,
                        stop_stats_ptr->GetCoordinate().GetLatitude() * 10000,
                        0
                }
        );
    }

    updateX(sorting_vec, stops_base, buses_base);
    updateY(sorting_vec, stops_base, buses_base);
    for (const auto& container : *sorting_vec) {
        (*stop_name_to_point_)[container.stop_name_] = Svg::Point{
            container.x_,
            container.y_
        };
    }
}

void PointsComputer::updateX(
        const std::unique_ptr<std::vector<StopContainer>> &sorting_vec,
        const std::shared_ptr<BusStopsDataBase> &stops_base,
        const std::shared_ptr<RoutesDataBase> &buses_base
) {
    std::sort(sorting_vec->begin(), sorting_vec->end(), [](
            const StopContainer &lhs,
            const StopContainer &rhs
    ) {
        return lhs.x_ < rhs.x_;
    });

    if (sorting_vec->size() == 1) {
        sorting_vec->front().x_ = master_->padding_;
    } else {
        updateSortingVec(sorting_vec, stops_base, buses_base);
        const double x_step = sorting_vec->back().index_ == 0 ? 0 :
                (master_->width_ - 2 * master_->padding_) / sorting_vec->back().index_;
        for (auto& container : *sorting_vec) {
            container.x_ = container.index_ * x_step + master_->padding_;
        }
    }
}

void PointsComputer::updateY(
        const std::unique_ptr<std::vector<StopContainer>> &sorting_vec,
        const std::shared_ptr<BusStopsDataBase> &stops_base,
        const std::shared_ptr<RoutesDataBase> &buses_base
        ) {
    std::sort(sorting_vec->begin(), sorting_vec->end(), [](
            const StopContainer &lhs,
            const StopContainer &rhs
    ) {
        return lhs.y_ < rhs.y_;
    });

    if (sorting_vec->size() == 1) {
        sorting_vec->front().y_ = master_->height_ - master_->padding_;
    } else {
        updateSortingVec(sorting_vec, stops_base, buses_base);
        const double y_step = sorting_vec->back().index_ == 0 ? 0 :
                (master_->height_ - 2 * master_->padding_) / sorting_vec->back().index_;
        for (auto& container : *sorting_vec) {
            container.y_ = master_->height_ - master_->padding_ - container.index_ * y_step;
        }
    }
}

void PointsComputer::updateSortingVec(
        const std::unique_ptr<std::vector<StopContainer>> &sorting_vec,
        const std::shared_ptr<BusStopsDataBase> &stops_base,
        const std::shared_ptr<RoutesDataBase> &buses_base
        ) {
    size_t index = 0;
    for (auto &container : *sorting_vec) {
        container.index_ = index;
        ++index;
    }

    std::unique_ptr<std::unordered_map<size_t, std::unordered_set<std::string>>> index_to_stops_set =
            std::make_unique<std::unordered_map<size_t, std::unordered_set<std::string>>>();
    index_to_stops_set->reserve(sorting_vec->size());

    (*index_to_stops_set)[0].insert(sorting_vec->front().stop_name_);
    size_t current_index = 0;
    for (size_t i = 1; i < sorting_vec->size(); ++i) {
        bool remain_current_index = true;
        for (const auto &previous_stop_name : index_to_stops_set->at(current_index)) {
            if (check_stops_to_neighborhood(
                    previous_stop_name,
                    sorting_vec->at(i).stop_name_,
                    stops_base,
                    buses_base
            )) {
                remain_current_index = false;
                break;
            }
        }
        if (!remain_current_index) {
            ++current_index;
        }
        (*index_to_stops_set)[current_index].insert(sorting_vec->at(i).stop_name_);
        sorting_vec->at(i).index_ = current_index;
    }
}

bool PointsComputer::check_stops_to_neighborhood(
        const std::string &first,
        const std::string &second,
        const std::shared_ptr<BusStopsDataBase> &stops_base,
        const std::shared_ptr<RoutesDataBase> &buses_base
        ) {
    const auto& first_buses = stops_base->GetBuses(first);
    const auto& second_buses = stops_base->GetBuses(second);

    for (const auto& bus : first_buses) {
        const auto finder = second_buses.find(bus);
        if (finder != second_buses.end()) {
            if (buses_base->GetRouteInfo(bus)->AreStopsNeighbors(first, second)) {
                return true;
            }
        }
    }
    return false;
}


Svg::Point PointsComputer::GetPoint(const std::string &stop_name) {
    return stop_name_to_point_->at(stop_name);
}


void MapDataBase::build_layers(
        const std::shared_ptr<BusStopsDataBase> &stops_base,
        const std::shared_ptr<RoutesDataBase> &buses_base
) {
    size_t palette_index = 0;
    for (const auto &bus_name : buses_base->GetRouteNames()) {
        build_line(bus_name, stops_base, buses_base, palette_index);
        ++palette_index;
    }

    recompute_circles_map(stops_base);

    build_end_stops();
    build_circles();
    build_texts();
}


void MapDataBase::build_svg() {
    for (const auto &layer_name : *layers_list_) {
        layers_object_->BuildLayer(layer_name, svg);
    }
}


void MapDataBase::build_line(
        const std::string &bus_name,
        const std::shared_ptr<BusStopsDataBase> &stops_base,
        const std::shared_ptr<RoutesDataBase> &buses_base,
        size_t index
) {
    index %= color_palette_->size();

    Svg::Polyline polyline;
    polyline.SetStrokeColor(color_palette_->at(index))
            .SetStrokeWidth(line_width_)
            .SetStrokeLineCap("round")
            .SetStrokeLineJoin("round");

    for (const auto &stop : buses_base->GetRouteInfo(bus_name)->GetStops()) {
        const Svg::Point stop_point = pointsComputer->GetPoint(stop);

        polyline.AddPoint(stop_point);

        update_circles_map(stop, stop_point);
    }
    update_end_stops_map(bus_name, stops_base, buses_base, index);

    layers_object_->AddBusLine(polyline);
}


void MapDataBase::update_circles_map(
        const std::string &stop_name,
        const Svg::Point &stop_point) {
    const auto finder = circles_map_->find(stop_name);

    if (finder == circles_map_->end()) {
        (*circles_map_)[stop_name] = Svg::Circle()
                .SetCenter(stop_point)
                .SetRadius(stop_radius_)
                .SetFillColor("white");
    }
}


void MapDataBase::update_end_stops_map(
        const std::string &bus_name,
        const std::shared_ptr<BusStopsDataBase> &stops_base,
        const std::shared_ptr<RoutesDataBase> &buses_base,
        size_t palette_index
) {
    const auto end_stops = buses_base->GetRouteInfo(bus_name)->GetEndStops();

    for (const auto &stop : end_stops) {
        (*bus_to_end_stops_)[bus_name].push_back(
                std::make_pair(
                        pointsComputer->GetPoint(stop),
                        palette_index
                )
        );
    }
}


void MapDataBase::recompute_circles_map(
        const std::shared_ptr<BusStopsDataBase> &stops_base
) {
    for (const auto&[stop_name, _] : stops_base->GetStopsMap()) {
        const Svg::Point stop_point = pointsComputer->GetPoint(stop_name);

        update_circles_map(stop_name, stop_point);
    }
}


void MapDataBase::build_end_stops() {
    for (const auto &bus_pair : *bus_to_end_stops_) {
        for (const auto &end_stop : bus_pair.second) {
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
    for (const auto &circle_pair : *circles_map_) {
        layers_object_->AddStopPoint(circle_pair.second);
    }
}


void MapDataBase::build_texts() {
    for (const auto &circle_pair : *circles_map_) {
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