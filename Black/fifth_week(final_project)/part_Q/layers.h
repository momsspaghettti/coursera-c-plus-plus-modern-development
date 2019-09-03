#pragma once

#include "svg.h"
#include <vector>
#include <unordered_map>


namespace Map {

    class Layers {
    public:
        Layers() {
            stop_points_ = std::make_unique<std::vector<Svg::Circle>>();
            stop_points_->reserve(100);

            bus_lines_ = std::make_unique<std::vector<Svg::Polyline>>();
            bus_lines_->reserve(100);

            bus_labels_ = std::make_unique<std::vector<Svg::Text>>();
            bus_labels_->reserve(400);

            stop_labels_ = std::make_unique<std::vector<Svg::Text>>();
            stop_labels_->reserve(200);
        }

        void AddStopPoint(const Svg::Circle &p) {
            stop_points_->push_back(p);
        }

        void AddBusLine(const Svg::Polyline &l) {
            bus_lines_->push_back(l);
        }

        void AddBusLabel(const Svg::Text &l) {
            bus_labels_->push_back(l);
        }

        void AddStopLabel(const Svg::Text &l) {
            stop_labels_->push_back(l);
        }

    private:
        void BuildStopPoints(const std::unique_ptr<Svg::Document> &svg) const {
            for (const auto &point : *stop_points_)
                svg->Add(point);
        }

        void BuildBusLines(const std::unique_ptr<Svg::Document> &svg) const {
            for (const auto &line : *bus_lines_)
                svg->Add(line);
        }

        void BuildBusLabels(const std::unique_ptr<Svg::Document> &svg) const {
            for (const auto &label : *bus_labels_)
                svg->Add(label);
        }

        void BuildStopLabels(const std::unique_ptr<Svg::Document> &svg) const {
            for (const auto &label : *stop_labels_)
                svg->Add(label);
        }

    public:
        void BuildLayer(const std::string &layer, const std::unique_ptr<Svg::Document> &svg) const {
            (this->*building_map_.at(layer))(svg);
        }

    private:
        std::unique_ptr<std::vector<Svg::Circle>> stop_points_;
        std::unique_ptr<std::vector<Svg::Polyline>> bus_lines_;
        std::unique_ptr<std::vector<Svg::Text>> bus_labels_;
        std::unique_ptr<std::vector<Svg::Text>> stop_labels_;

        const std::unordered_map<std::string, void (Layers::*)(const std::unique_ptr<Svg::Document> &) const>
                building_map_ = {
                {"bus_lines",   &Layers::BuildBusLines},
                {"bus_labels",  &Layers::BuildBusLabels},
                {"stop_points", &Layers::BuildStopPoints},
                {"stop_labels", &Layers::BuildStopLabels},
        };
    };

}