#include "svg.h"


namespace Svg
{
    void Document::Render(std::ostream& out) const
    {
		out << R"(<?xml version="1.0" encoding="UTF-8" ?>)";
		out << R"(<svg xmlns="http://www.w3.org/2000/svg" version="1.1">)";

        for (const auto& object : objects_)
        {
			object->Render(out);
        }

		out << "</svg>";
    }


    Circle& Circle::SetCenter(Point c)
    {
		center_ = c;
		return *this;
    }


    Circle& Circle::SetRadius(double r)
    {
		radius_ = r;
		return *this;
    }


    void Circle::Render(std::ostream& out) const
    {
		out << "<circle ";

		SvgBase<Circle>::Render(out);
		
        out << "cx=\"" << center_.x << "\" cy=\"" << center_.y << "\" ";
		out << "r=\"" << radius_ << "\" ";

		out << "/>";
    }


    void Color::Render(std::ostream& out) const
    {
        if (color_)
        {
			const size_t index = color_->index();

            switch (index)
            {
			case 0:
				out << std::get<std::string>(*color_);
                break;

			case 1:
				std::get<Rgb>(*color_).Render(out);
                break;
            }
        }
        else
        {
			out << "none";
        }
    }


    Polyline& Polyline::AddPoint(Point p)
    {
		points_.emplace_back(p);
		return *this;
    }


    void Polyline::Render(std::ostream& out) const
    {
		out << "<polyline ";

		SvgBase<Polyline>::Render(out);

		out << "points=\"";
        for (const auto point : points_)
        {
			point.Render(out);
			out << " ";
        }

		out << "\" />";
    }


    Text& Text::SetPoint(Point p)
    {
		coords_ = p;
		return *this;
    }


    Text& Text::SetOffset(Point p)
    {
		offset_ = p;
		return *this;
    }


    Text& Text::SetFontSize(uint32_t size)
    {
		font_size_ = size;
		return *this;
    }


    Text& Text::SetFontFamily(const std::string& font)
    {
		font_family_ = font;
		return *this;
    }


    Text& Text::SetData(const std::string& t)
    {
		text_ = t;
		return *this;
    }


    void Text::Render(std::ostream& out) const
    {
		out << "<text ";

		SvgBase<Text>::Render(out);

		out << "x=\"" << coords_.x << "\" y=\"" << coords_.y << "\" ";

		out << "dx=\"" << offset_.x << "\" dy=\"" << offset_.y << "\" ";

		out << "font-size=\"" << font_size_ << "\" ";

		if (font_family_)
			out << "font-family=\"" << *font_family_ << "\" ";

		out << ">";

		out << text_ << "</text>";
    }
}