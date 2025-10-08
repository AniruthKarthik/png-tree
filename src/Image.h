#ifndef IMAGE_H
#define IMAGE_H

#include "types.h"
#include <vector>

class Image {
public:
    Image(int width, int height);

    int get_width() const { return width; }
    int get_height() const { return height; }

    RGB_uc get_pixel(int r, int c) const;
    void set_pixel(int r, int c, const RGB_uc& color);

    void generate_random();

private:
    int width, height;
    std::vector<RGB_uc> data;
};

#endif // IMAGE_H
