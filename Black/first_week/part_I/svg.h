#pragma once
#include <optional>
#include <variant>
#include <string>
#include <utility>
#include <ostream>
#include <vector>
#include <cstdint>
#include <memory>


namespace Svg
{
    struct Point
	{
		double x;
		double y;

		void Render(std::ostream& out) const
		{
			out << x << "," << y;
		}
	};


	struct Rgb
	{
		int red, green, blue;

		void Render(std::ostream& out) const
		{
			out << "rgb(" << red << "," << green << "," << blue << ")";
		}
	};


	struct Rgba
    {
        int red, green, blue;
        double alpha;

        void Render(std::ostream& out) const
        {
            out << "rgba(" << red << "," << green << ","
            << blue << "," << alpha << ")";
        }
    };


	class Color
	{
	public:
		Color()
		{
			color_ = std::nullopt;
		}

		explicit Color(std::string color)
		{
			color_ = std::move(color);
		}

		explicit Color(Rgb color)
		{
			color_ = color;
		}

        explicit Color(Rgba color)
        {
            color_ = color;
        }

        Color(const Color& other)
		{
			*this = other;
		}

		[[nodiscard]] const std::optional<std::variant<std::string, Rgb, Rgba>>&
		GetColor() const
        {
            return color_;
        }

		void Render(std::ostream&) const;

	private:
		std::optional<std::variant<std::string, Rgb, Rgba>> color_;
	};


	const Color NoneColor = Color();


    class SvgObject
    {
    public:
		virtual void Render(std::ostream&) const = 0;

		virtual ~SvgObject() = default;
    };


    template <class Child>
    class SvgBase : public SvgObject
    {
    public:
        Child& GetChild()
        {
			return static_cast<Child&>(*this);
        }

        template<typename ColorInit>
		Child& SetFillColor(const ColorInit& init)
        {
			fill_ = Color(init);
			return GetChild();
        }

		template<typename ColorInit>
		Child& SetStrokeColor(const ColorInit& init)
        {
			stroke_ = Color(init);
			return GetChild();
        }

		Child& SetStrokeWidth(double width)
        {
			stroke_width_ = width;
			return GetChild();
        }

		Child& SetStrokeLineCap(const std::string& line)
        {
			stroke_linecap_ = line;
			return GetChild();
        }

		Child& SetStrokeLineJoin(const std::string& line)
        {
			stroke_linejoin_ = line;
			return GetChild();
        }

		void Render(std::ostream& out) const override
        {
			out << R"(stroke-width=\")" << stroke_width_ << R"(\" )";

			out << R"(fill=\")";
			fill_.Render(out);
			out << R"(\" )";

			out << R"(stroke=\")";
			stroke_.Render(out);
			out << R"(\" )";

			if (stroke_linecap_)
				out << R"(stroke-linecap=\")" << *stroke_linecap_ << R"(\" )";

			if (stroke_linejoin_)
				out << R"(stroke-linejoin=\")" << *stroke_linejoin_ << R"(\" )";
        }

    private:
		Color fill_ = NoneColor;
		Color stroke_ = NoneColor;
		double stroke_width_ = 1.0;

		std::optional<std::string> stroke_linecap_ = std::nullopt;
		std::optional<std::string> stroke_linejoin_ = std::nullopt;
    };


    class Circle : public SvgBase<Circle>
    {
    public:
        Circle() : center_(Point()), radius_(1.0) {}

		Circle& SetCenter(Point);

		Circle& SetRadius(double);

        void Render(std::ostream&) const override;

        [[nodiscard]] const Point& GetCenter() const {
            return center_;
        }

        [[nodiscard]] double GetRadius() const {
            return radius_;
        }

    private:
		Point center_;
		double radius_;
    };


    class Polyline : public SvgBase<Polyline>
    {
    public:
		Polyline& AddPoint(Point);

		void Render(std::ostream&) const override;

    private:
		std::vector<Point> points_;
    };


    class Text : public SvgBase<Text>
    {
    public:
		Text& SetPoint(Point);

		Text& SetOffset(Point);

		Text& SetFontSize(uint32_t);

		Text& SetFontFamily(const std::string&);

		Text& SetData(const std::string&);

		Text& SetFontWeight(const std::string&);

		void Render(std::ostream&) const override;

    private:
		Point coords_ = Point();
		Point offset_ = Point();
		uint32_t font_size_ = 1;
		
        std::optional<std::string> font_family_ = std::nullopt;
		
        std::string text_;

        std::optional<std::string> font_weight_ = std::nullopt;
    };


	class Document : public SvgObject
	{
	public:
		Document() = default;

        template<typename ObjectType>
		void Add(ObjectType object)
        {
			objects_.push_back(std::make_unique<ObjectType>(std::move(object)));
        }

		void Render(std::ostream&) const override;

	private:
		std::vector<std::unique_ptr<SvgObject>> objects_;
	};
}