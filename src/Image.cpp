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
