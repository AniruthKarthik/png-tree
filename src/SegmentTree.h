#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H

#include "Image.h"
#include "types.h"
#include <vector>

class SegmentTree {
public:
    SegmentTree(const Image& image);
    void adjust_brightness(int r1, int c1, int r2, int c2, int value);
    void adjust_contrast(int r1, int c1, int r2, int c2, double multiplier);
    void fill_region(int r1, int c1, int r2, int c2, const RGB_uc& color);
    Image get_image();
    RGB_d query_average_color(int r1, int c1, int r2, int c2);

private:
    struct Node {
        RGB_d sum;
        RGB_d lazy_add = {0, 0, 0};
        RGB_d lazy_mul = {1, 1, 1};
        RGB_uc lazy_set;
        bool is_lazy_set = false;
    };

    int rows, cols;
    std::vector<Node> tree;

    void build(int node_idx, int start_r, int start_c, int end_r, int end_c, const Image& image);
    void push(int node_idx, int start_r, int start_c, int end_r, int end_c);
    void update(int node_idx, int start_r, int start_c, int end_r, int end_c, int r1, int c1, int r2, int c2, const RGB_d& mul_val, const RGB_d& add_val, const RGB_uc* set_val);
    void reconstruct_image_iterative(Image& image);
    RGB_d query_tree(int node_idx, int start_r, int start_c, int end_r, int end_c, int r1, int c1, int r2, int c2);
};

#endif // SEGMENT_TREE_H
