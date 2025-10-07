#ifndef VECTOR_IMAGE_H
#define VECTOR_IMAGE_H

#include <vector>
#include <opencv2/opencv.hpp>

class VectorImage {
public:
    VectorImage(int width, int height);

    void generate_random();
    cv::Mat get_image() const;

    void adjust_brightness(int r1, int c1, int r2, int c2, int value);
    void adjust_contrast(int r1, int c1, int r2, int c2, double multiplier);
    void fill_region(int r1, int c1, int r2, int c2, const cv::Vec3b& color);

private:
    std::vector<cv::Vec3b> image_data;
    int width, height;
};

#endif // VECTOR_IMAGE_H
