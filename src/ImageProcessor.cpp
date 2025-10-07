#include "ImageProcessor.h"

ImageProcessor::ImageProcessor(int width, int height) {
    image = cv::Mat(height, width, CV_8UC3);
    generate_random();
}

void ImageProcessor::generate_random() {
    cv::randu(image, cv::Scalar(0, 0, 0), cv::Scalar(255, 255, 255));
}

cv::Mat ImageProcessor::get_image() const {
    return image.clone();
}

void ImageProcessor::set_image(const cv::Mat& img) {
    image = img.clone();
}

void ImageProcessor::adjust_brightness(int r1, int c1, int r2, int c2, int value) {
    for (int r = r1; r <= r2; ++r) {
        for (int c = c1; c <= c2; ++c) {
            cv::Vec3b& pixel = image.at<cv::Vec3b>(r, c);
            for (int i = 0; i < 3; ++i) {
                pixel[i] = cv::saturate_cast<uchar>(pixel[i] + value);
            }
        }
    }
}

void ImageProcessor::adjust_contrast(int r1, int c1, int r2, int c2, double multiplier) {
    for (int r = r1; r <= r2; ++r) {
        for (int c = c1; c <= c2; ++c) {
            cv::Vec3b& pixel = image.at<cv::Vec3b>(r, c);
            for (int i = 0; i < 3; ++i) {
                pixel[i] = cv::saturate_cast<uchar>(pixel[i] * multiplier);
            }
        }
    }
}

void ImageProcessor::fill_region(int r1, int c1, int r2, int c2, const cv::Vec3b& color) {
    cv::rectangle(image, cv::Point(c1, r1), cv::Point(c2, r2), color, -1);
}

void ImageProcessor::flip(int direction) {
    cv::flip(image, image, direction);
}

void ImageProcessor::rotate() {
    cv::rotate(image, image, cv::ROTATE_90_CLOCKWISE);
}
