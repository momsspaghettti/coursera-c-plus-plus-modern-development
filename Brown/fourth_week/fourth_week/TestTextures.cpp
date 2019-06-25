#include "Common.h"
#include "Textures.h"
#include "UnitTestsFramework.h"
#include <map>
#include <stdexcept>
#include <sstream>


class Canvas 
{
public:
	using ShapeId = size_t;

	void SetSize(Size size) 
    {
		size_ = size;
	}

	ShapeId AddShape(ShapeType shape_type, Point position, Size size, 
		std::unique_ptr<ITexture> texture) 
    {
		auto shape = MakeShape(shape_type);
		shape->SetPosition(position);
		shape->SetSize(size);
		shape->SetTexture(move(texture));
		return InsertShape(move(shape));
	}


	ShapeId DuplicateShape(ShapeId source_id, Point target_position) 
    {
		auto shape = GetShapeNodeById(source_id)->second->Clone();
		shape->SetPosition(target_position);
		return InsertShape(std::move(shape));
	}


	void RemoveShape(ShapeId id) 
    {
		shapes_.erase(GetShapeNodeById(id));
	}


	void MoveShape(ShapeId id, Point position) 
    {
		GetShapeNodeById(id)->second->SetPosition(position);
	}


	void ResizeShape(ShapeId id, Size size) 
    {
		GetShapeNodeById(id)->second->SetSize(size);
	}


	int GetShapesCount() const 
    {
		return static_cast<int>(shapes_.size());
	}


	void Print(std::ostream& output) const 
    {
		Image image(size_.height, std::string(size_.width, ' '));

		for (const auto& [id, shape] : shapes_) 
		{
			shape->Draw(image);
		}

		output << '#' << std::string(size_.width, '#') << "#\n";
		for (const auto& line : image) 
		{
			output << '#' << line << "#\n";
		}

		output << '#' << std::string(size_.width, '#') << "#\n";
	}

private:
	using Shapes = std::map<ShapeId, std::unique_ptr<IShape>>;

	
    Shapes::iterator GetShapeNodeById(ShapeId id)
    {
		auto it = shapes_.find(id);
		if (it == shapes_.end()) 
		{
			throw std::out_of_range("No shape with given ID");
		}
		return it;
	}


    ShapeId InsertShape(std::unique_ptr<IShape> shape)
    {
		shapes_[current_id_] = std::move(shape);
		return current_id_++;
	}

	Size size_ = {};
	ShapeId current_id_ = 0;
	Shapes shapes_;
};


void TestSimple() 
{
	Canvas canvas;
	canvas.SetSize({ 5, 3 });

	canvas.AddShape(ShapeType::Rectangle, { 1, 0 }, { 3, 3 }, nullptr);

	std::stringstream output;
	canvas.Print(output);

	const auto answer =
		"#######\n"
		"# ... #\n"
		"# ... #\n"
		"# ... #\n"
		"#######\n";

	ASSERT_EQUAL(answer, output.str());
}


void TestSmallTexture() 
{
	Canvas canvas;
	canvas.SetSize({ 6, 4 });

	canvas.AddShape(ShapeType::Rectangle, { 1, 1 }, { 4, 2 },
		MakeTextureSolid({ 3, 1 }, '*'));

	std::stringstream output;
	canvas.Print(output);

	const auto answer =
		"########\n"
		"#      #\n"
		"# ***. #\n"
		"# .... #\n"
		"#      #\n"
		"########\n";

	ASSERT_EQUAL(answer, output.str());
}


void TestCow() 
{
	Canvas canvas;
	canvas.SetSize({ 18, 5 });

	canvas.AddShape(ShapeType::Rectangle, { 1, 0 }, { 16, 5 }, MakeTextureCow());

	std::stringstream output;
	canvas.Print(output);

	// Здесь уместно использовать сырые литералы, т.к. в текстуре есть символы '\'
	const auto answer =
		R"(####################)""\n"
		R"(# ^__^             #)""\n"
		R"(# (oo)\_______     #)""\n"
		R"(# (__)\       )\/\ #)""\n"
		R"(#     ||----w |    #)""\n"
		R"(#     ||     ||    #)""\n"
		R"(####################)""\n";

	ASSERT_EQUAL(answer, output.str());
}


void TestCpp() 
{
	Canvas canvas;
	canvas.SetSize({ 77, 17 });

	// Буква "C" как разность двух эллипсов, один из которых нарисован цветом фона
	canvas.AddShape(ShapeType::Ellipse, { 2, 1 }, { 30, 15 },
		MakeTextureCheckers({ 100, 100 }, 'c', 'C'));
	canvas.AddShape(ShapeType::Ellipse, { 8, 4 }, { 30, 9 },
		MakeTextureSolid({ 100, 100 }, ' '));

	// Горизонтальные чёрточки плюсов
	auto h1 = canvas.AddShape(ShapeType::Rectangle, { 54, 7 }, { 22, 3 },
		MakeTextureSolid({ 100, 100 }, '+'));
	
    auto h2 = canvas.DuplicateShape(h1, { 30, 7 });

	// Вертикальные чёрточки плюсов
	auto v1 = canvas.DuplicateShape(h1, { 62, 3 });
	canvas.ResizeShape(v1, { 6, 11 });
	auto v2 = canvas.DuplicateShape(v1, { 38, 3 });

	std::stringstream output;
	canvas.Print(output);

	const auto answer =
		"###############################################################################\n"
		"#                                                                             #\n"
		"#            cCcCcCcCcC                                                       #\n"
		"#        CcCcCcCcCcCcCcCcCc                                                   #\n"
		"#      cCcCcCcCcCcCcCcCcCcCcC          ++++++                  ++++++         #\n"
		"#    CcCcCcCcCcCc                      ++++++                  ++++++         #\n"
		"#   CcCcCcCcC                          ++++++                  ++++++         #\n"
		"#   cCcCcCc                            ++++++                  ++++++         #\n"
		"#  cCcCcC                      ++++++++++++++++++++++  ++++++++++++++++++++++ #\n"
		"#  CcCcCc                      ++++++++++++++++++++++  ++++++++++++++++++++++ #\n"
		"#  cCcCcC                      ++++++++++++++++++++++  ++++++++++++++++++++++ #\n"
		"#   cCcCcCc                            ++++++                  ++++++         #\n"
		"#   CcCcCcCcC                          ++++++                  ++++++         #\n"
		"#    CcCcCcCcCcCc                      ++++++                  ++++++         #\n"
		"#      cCcCcCcCcCcCcCcCcCcCcC          ++++++                  ++++++         #\n"
		"#        CcCcCcCcCcCcCcCcCc                                                   #\n"
		"#            cCcCcCcCcC                                                       #\n"
		"#                                                                             #\n"
		"###############################################################################\n";

	ASSERT_EQUAL(answer, output.str());
}


void TestTextures() 
{
	TestRunner tr;
	RUN_TEST(tr, TestSimple);
	RUN_TEST(tr, TestSmallTexture);
	RUN_TEST(tr, TestCow);
	RUN_TEST(tr, TestCpp);
}