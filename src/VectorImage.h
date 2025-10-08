#ifndef VECTOR_IMAGE_H
#define VECTOR_IMAGE_H

#include "Image.h"
#include "types.h"
#include <vector>

class VectorImage {
public:
    VectorImage(int width, int height);

    void generate_random();
    Image get_image() const;

    void adjust_brightness(int r1, int c1, int r2, int c2, int value);
    void adjust_contrast(int r1, int c1, int r2, int c2, double multiplier);
    void fill_region(int r1, int c1, int r2, int c2, const RGB_uc& color);

private:
    std::vector<RGB_uc> image_data;
    int width, height;
};

#endif // VECTOR_IMAGE_H