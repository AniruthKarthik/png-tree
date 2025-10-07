#ifndef IMAGE_PROCESSOR_H
#define IMAGE_PROCESSOR_H

#include "Image.h"

class ImageProcessor {
public:
    ImageProcessor(int width, int height);

    void generate_random();
    Image get_image() const;
    void set_image(const Image& img);

private:
    Image image;
};

#endif // IMAGE_PROCESSOR_H