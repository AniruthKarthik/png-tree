#include "VectorImage.h"
#include "types.h"
#include <random>
#include <algorithm>

VectorImage::VectorImage(int width, int height) : width(width), height(height) {
    image_data.resize(width * height);
    generate_random();
}

void VectorImage::generate_random() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);

    for (auto& pixel : image_data) {
        pixel.r = dis(gen);
        pixel.g = dis(gen);
        pixel.b = dis(gen);
    }
}

Image VectorImage::get_image() const {
    Image image(width, height);
    for (int r = 0; r < height; ++r) {
        for (int c = 0; c < width; ++c) {
            image.set_pixel(r, c, image_data[r * width + c]);
        }
    }
    return image;
}

void VectorImage::adjust_brightness(int r1, int c1, int r2, int c2, int value) {
    for (int r = r1; r <= r2; ++r) {
        for (int c = c1; c <= c2; ++c) {
            RGB_uc& pixel = image_data[r * width + c];
            pixel.r = saturate_cast_uchar(pixel.r + value);
            pixel.g = saturate_cast_uchar(pixel.g + value);
            pixel.b = saturate_cast_uchar(pixel.b + value);
        }
    }
}

void VectorImage::adjust_contrast(int r1, int c1, int r2, int c2, double multiplier) {
    for (int r = r1; r <= r2; ++r) {
        for (int c = c1; c <= c2; ++c) {
            RGB_uc& pixel = image_data[r * width + c];
            pixel.r = saturate_cast_uchar(pixel.r * multiplier);
            pixel.g = saturate_cast_uchar(pixel.g * multiplier);
            pixel.b = saturate_cast_uchar(pixel.b * multiplier);
        }
    }
}

void VectorImage::fill_region(int r1, int c1, int r2, int c2, const RGB_uc& color) {
    for (int r = r1; r <= r2; ++r) {
        for (int c = c1; c <= c2; ++c) {
            image_data[r * width + c] = color;
        }
    }
}