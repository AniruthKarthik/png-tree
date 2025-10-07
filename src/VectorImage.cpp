#include "VectorImage.h"
#include <random>

VectorImage::VectorImage(int width, int height) : width(width), height(height) {
    image_data.resize(width * height);
    generate_random();
}

void VectorImage::generate_random() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);

    for (auto& pixel : image_data) {
        pixel[0] = dis(gen);
        pixel[1] = dis(gen);
        pixel[2] = dis(gen);
    }
}

cv::Mat VectorImage::get_image() const {
    cv::Mat image(height, width, CV_8UC3);
    for (int r = 0; r < height; ++r) {
        for (int c = 0; c < width; ++c) {
            image.at<cv::Vec3b>(r, c) = image_data[r * width + c];
        }
    }
    return image;
}

void VectorImage::adjust_brightness(int r1, int c1, int r2, int c2, int value) {
    for (int r = r1; r <= r2; ++r) {
        for (int c = c1; c <= c2; ++c) {
            cv::Vec3b& pixel = image_data[r * width + c];
            for (int i = 0; i < 3; ++i) {
                pixel[i] = cv::saturate_cast<uchar>(pixel[i] + value);
            }
        }
    }
}

void VectorImage::adjust_contrast(int r1, int c1, int r2, int c2, double multiplier) {
    for (int r = r1; r <= r2; ++r) {
        for (int c = c1; c <= c2; ++c) {
            cv::Vec3b& pixel = image_data[r * width + c];
            for (int i = 0; i < 3; ++i) {
                pixel[i] = cv::saturate_cast<uchar>(pixel[i] * multiplier);
            }
        }
    }
}

void VectorImage::fill_region(int r1, int c1, int r2, int c2, const cv::Vec3b& color) {
    for (int r = r1; r <= r2; ++r) {
        for (int c = c1; c <= c2; ++c) {
            image_data[r * width + c] = color;
        }
    }
}
