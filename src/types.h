#ifndef TYPES_H
#define TYPES_H

struct RGB_uc {
    unsigned char r, g, b;
};

struct RGB_d {
    double r, g, b;

    RGB_d& operator+=(const RGB_d& other) {
        r += other.r;
        g += other.g;
        b += other.b;
        return *this;
    }

    RGB_d operator*(double val) const {
        return {r * val, g * val, b * val};
    }
};

inline unsigned char saturate_cast_uchar(double val) {
    if (val > 255.0) return 255;
    if (val < 0.0) return 0;
    return static_cast<unsigned char>(val);
}

#endif // TYPES_H
