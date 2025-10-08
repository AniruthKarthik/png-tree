#include "Image.h"
#include <iostream>
#include <random>

Image::Image(int width, int height) : width(width), height(height)
{
	data.resize(width * height);
}

RGB_uc Image::get_pixel(int r, int c) const { return data[r * width + c]; }

void Image::set_pixel(int r, int c, const RGB_uc &color)
{
	data[r * width + c] = color;
}

void Image::generate_random()
{
	std::random_device rd;
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 255);

	for (auto &pixel : data)
	{
		pixel.r = dis(gen);
		pixel.g = dis(gen);
		pixel.b = dis(gen);
	}
}

Image Image::blur(int r1, int c1, int r2, int c2) const
{
	Image blurred_image(width, height);
	for (int r = 0; r < height; ++r)
	{
		for (int c = 0; c < width; ++c)
		{
			if (r >= r1 && r <= r2 && c >= c1 && c <= c2)
			{
				int red = 0, green = 0, blue = 0;
				int count = 0;
				for (int i = -1; i <= 1; ++i)
				{
					for (int j = -1; j <= 1; ++j)
					{
						int nr = r + i;
						int nc = c + j;
						if (nr >= 0 && nr < height && nc >= 0 && nc < width)
						{
							RGB_uc pixel = get_pixel(nr, nc);
							red += pixel.r;
							green += pixel.g;
							blue += pixel.b;
							count++;
						}
					}
				}
				blurred_image.set_pixel(r, c,
				                        {(unsigned char)(red / count),
				                         (unsigned char)(green / count),
				                         (unsigned char)(blue / count)});
			}
			else
			{
				blurred_image.set_pixel(r, c, get_pixel(r, c));
			}
		}
	}
	return blurred_image;
}
