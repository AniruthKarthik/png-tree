#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

#include <opencv2/opencv.hpp>

class ImageProcessor {
public:
    ImageProcessor(int width, int height);

    void generate_random();
    cv::Mat get_image() const;
    void set_image(const cv::Mat& img);

    void adjust_brightness(int r1, int c1, int r2, int c2, int value);
    void adjust_contrast(int r1, int c1, int r2, int c2, double multiplier);
    void fill_region(int r1, int c1, int r2, int c2, const cv::Vec3b& color);
    void flip(int direction);
    void rotate();

private:
    cv::Mat image;
};

#endif // IMAGE_PROCESSOR_H
