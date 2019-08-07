#include "svg.h"


namespace Svg
{
    void Document::Render(std::ostream& out) const
    {
		out << R"(<?xml version=\"1.0\" encoding=\"UTF-8\" ?>)";
		out << R"(<svg xmlns=\"http://www.w3.org/2000/svg\" version=\"1.1\">)";

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
		
        out << R"(cx=\")" << center_.x << R"(\" cy=\")" << center_.y << R"(\" )";
		out << R"(r=\")" << radius_ << R"(\" )";

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

            case 2:
                std::get<Rgba>(*color_).Render(out);
                break;

            default:
                throw std::bad_variant_access();
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

		out << R"(points=\")";
        for (const auto point : points_)
        {
			point.Render(out);
			out << " ";
        }

		out << R"(\" />)";
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

    Text& Text::SetFontWeight(const std::string& weight) {
        font_weight_ = weight;
        return *this;
    }


    void Text::Render(std::ostream& out) const
    {
		out << "<text ";

		SvgBase<Text>::Render(out);

		out << R"(x=\")" << coords_.x << R"(\" y=\")" << coords_.y << R"(\" )";

		out << R"(dx=\")" << offset_.x << R"(\" dy=\")" << offset_.y << R"(\" )";

		out << R"(font-size=\")" << font_size_ << R"(\" )";

		if (font_family_)
			out << R"(font-family=\")" << *font_family_ << R"(\" )";

		if (font_weight_)
            out << R"(font-weight=\")" << *font_weight_ << R"(\" )";

		out << ">";

		out << text_ << "</text>";
    }
}