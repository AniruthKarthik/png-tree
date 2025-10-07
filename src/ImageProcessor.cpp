#include "ImageProcessor.h"

ImageProcessor::ImageProcessor(int width, int height) : image(width, height) {
    generate_random();
}

void ImageProcessor::generate_random() {
    image.generate_random();
}

Image ImageProcessor::get_image() const {
    return image;
}

void ImageProcessor::set_image(const Image& img) {
    image = img;
}