#include "Textures.h"
#include <cassert>
#include <string>


class Texture : public ITexture 
{
public:
	Texture(Image image) : image_(std::move(image)) 
    {
		for (const auto& line : image_) 
		{
			assert(line.size() == image_[0].size());
		}
	}


	Size GetSize() const override 
    {
		const auto width = static_cast<int>(image_.empty() ? 0 : image_[0].size());
		const auto height = static_cast<int>(image_.size());
		return { width, height };
	}


	const Image& GetImage() const override 
    {
		return image_;
	}

private:
	Image image_;
};


std::unique_ptr<ITexture> MakeTextureSolid(Size size, char pixel) 
{
	Image image(size.height, std::string(size.width, pixel));
	
    return std::make_unique<Texture>(std::move(image));
}


std::unique_ptr<ITexture> MakeTextureCheckers(Size size, char pixel1,
	char pixel2) 
{
	Image image(size.height, std::string(size.width, pixel1));

	for (int i = 0; i < size.height; ++i) 
	{
		for (int j = 0; j < size.width; ++j) 
		{
			if ((i + j) % 2 != 0) 
			{
				image[i][j] = pixel2;
			}
		}
	}

	return std::make_unique<Texture>(std::move(image));
}


std::unique_ptr<ITexture> MakeTextureCow() 
{
	Image image = { R"(^__^            )",
				   R"((oo)\_______    )",
				   R"((__)\       )\/\)",
				   R"(    ||----w |   )",
				   R"(    ||     ||   )" };
	return std::make_unique<Texture>(std::move(image));
}