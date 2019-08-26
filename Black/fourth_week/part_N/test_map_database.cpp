#include "map_database.h"


namespace Map {
    void TestRenderSettings() {
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
        "outer_margin": 150,
        "layers": [
            "bus_lines",
            "stop_points",
            "bus_labels"
        ]
    } })");

        const auto json_input = Json::Load(input).GetRoot();

        std::shared_ptr<BusStopsDataBase> stops_database_ = std::make_shared<BusStopsDataBase>();
        std::shared_ptr<RoutesDataBase> routes_database_ = std::make_shared<RoutesDataBase>();
        MapDataBase map_database(stops_database_, routes_database_);

        map_database.AddRenderSettings(
                json_input.AsMap().at("render_settings").AsMap());

        ASSERT(std::abs(map_database.render_settings_->width_ - 1200) < 0.0001)
        ASSERT(std::abs(map_database.render_settings_->height_ - 1200) < 0.0001)
        ASSERT(std::abs(map_database.render_settings_->padding_ - 50) < 0.0001)
        ASSERT(std::abs(map_database.render_settings_->stop_radius_ - 5) < 0.0001)
        ASSERT(std::abs(map_database.render_settings_->height_ - 1200) < 0.0001)
        ASSERT(std::abs(map_database.render_settings_->line_width_ - 14) < 0.0001)
        ASSERT_EQUAL(map_database.render_settings_->bus_label_font_size_, 20)
        ASSERT(std::abs(map_database.render_settings_->bus_label_offset_.x - 7) < 0.0001)
        ASSERT(std::abs(map_database.render_settings_->bus_label_offset_.y - 15) < 0.0001)
        ASSERT_EQUAL(map_database.render_settings_->stop_label_font_size_, 20)
        ASSERT_EQUAL(map_database.render_settings_->underlayer_width_, 3)
        ASSERT(std::abs(map_database.render_settings_->stop_label_offset_.x - 7) < 0.0001)
        ASSERT(std::abs(map_database.render_settings_->stop_label_offset_.y + 3) < 0.0001)

        ASSERT_EQUAL(std::get<Svg::Rgba>(*map_database.render_settings_->underlayer_color_.GetColor()).red, 255)
        ASSERT_EQUAL(std::get<Svg::Rgba>(*map_database.render_settings_->underlayer_color_.GetColor()).green, 255)
        ASSERT_EQUAL(std::get<Svg::Rgba>(*map_database.render_settings_->underlayer_color_.GetColor()).blue, 255)
        ASSERT(std::abs(
                std::get<Svg::Rgba>(*map_database.render_settings_->underlayer_color_.GetColor()).alpha - 0.85) <
               0.0001)

        ASSERT_EQUAL(std::get<std::string>(*map_database.render_settings_->color_palette_->at(0).GetColor()), "green")
        ASSERT_EQUAL(std::get<std::string>(*map_database.render_settings_->color_palette_->at(2).GetColor()), "red")

        ASSERT_EQUAL(std::get<Svg::Rgb>(*map_database.render_settings_->color_palette_->at(1).GetColor()).red, 255)
        ASSERT_EQUAL(std::get<Svg::Rgb>(*map_database.render_settings_->color_palette_->at(1).GetColor()).green, 160)
        ASSERT_EQUAL(std::get<Svg::Rgb>(*map_database.render_settings_->color_palette_->at(1).GetColor()).blue, 0)

        ASSERT_EQUAL(std::get<Svg::Rgba>(*map_database.render_settings_->color_palette_->at(3).GetColor()).red, 255)
        ASSERT_EQUAL(std::get<Svg::Rgba>(*map_database.render_settings_->color_palette_->at(3).GetColor()).green, 255)
        ASSERT_EQUAL(std::get<Svg::Rgba>(*map_database.render_settings_->color_palette_->at(3).GetColor()).blue, 255)
        ASSERT(std::abs(
                std::get<Svg::Rgba>(*map_database.render_settings_->color_palette_->at(3).GetColor()).alpha - 0.85) <
               0.0001)

        ASSERT_EQUAL(map_database.render_settings_->layers_list_->at(0), "bus_lines")
        ASSERT_EQUAL(map_database.render_settings_->layers_list_->at(1), "stop_points")
        ASSERT_EQUAL(map_database.render_settings_->layers_list_->at(2), "bus_labels")
    }


    void TestMapDataBase(TestRunner &tr) {
        RUN_TEST(tr, TestRenderSettings);
    }
}