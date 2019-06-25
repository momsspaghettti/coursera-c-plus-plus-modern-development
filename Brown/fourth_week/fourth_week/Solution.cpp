#include "Common.h"
#include "string"


// Этот файл сдаётся на проверку
// Здесь напишите реализацию необходимых классов-потомков `IShape`
class Figure : public IShape
{
public:
	void SetPosition(Point point) override
	{
		position_ = point;
	}

	Point GetPosition() const override
	{
		return position_;
	}

	void SetSize(Size size) override
	{
		size_ = size;
	}

	Size GetSize() const override
	{
		return size_;
	}

	void SetTexture(std::shared_ptr<ITexture> texture) override
	{
		texture_ = texture;
	}

	ITexture* GetTexture() const override
	{
		return texture_.get();
	}

	void Draw(Image& image) const override
	{
		if (image.empty())
			return;

		const Image texture_image = texture_ ? texture_->GetImage() :
			Image(size_.height, std::string(size_.width, '.'));

		const int n = image.size();
		const int m = image[0].size();

		for (int i = position_.y; i < position_.y + size_.height; ++i)
		{
			for (int j = position_.x; j < position_.x + size_.width; ++j)
			{
				if (i >= 0 && i < n && j >= 0 && j < m)
				{
					const Point point = Point{ j - position_.x, i - position_.y };
					const int i_index = i - position_.y;
					const int j_index = j - position_.x;

                    if (CheckPointInShape(point))
                    {
                        if (CheckPointInTexture(point, texture_image))
                        {
							image[i][j] = texture_image[point.y][point.x];
                        }
                        else
                        {
							image[i][j] = '.';
                        }
                    }
				}
			}
		}
	}

private:
	Size size_;
	Point position_;
	std::shared_ptr<ITexture> texture_;

	bool CheckPointInTexture(Point point, const Image& texture_image) const
	{
		return point.y < texture_image.size() && point.x < texture_image[point.y].size();
	}

	virtual bool CheckPointInShape(Point) const = 0;
};


class Rectangle : public Figure
{
public:
	std::unique_ptr<IShape> Clone() const override
	{		
		return std::make_unique<Rectangle>(*this);
	}

private:
    bool CheckPointInShape(Point point) const override
    {
		return true;
    }
};


class Ellipse : public Figure
{
public:
	std::unique_ptr<IShape> Clone() const override
	{
		return std::make_unique<Ellipse>(*this);
	}

private:
	bool CheckPointInShape(Point point) const override
	{
		return IsPointInEllipse(point, GetSize());
	}
};


// Напишите реализацию функции
std::unique_ptr<IShape> MakeShape(ShapeType shape_type)
{
	if (shape_type == ShapeType::Rectangle)
		return std::make_unique<Rectangle>();

	return std::make_unique<Ellipse>();
}