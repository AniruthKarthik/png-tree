#ifndef SEGMENT_TREE_H
#define SEGMENT_TREE_H

#include <vector>
#include <opencv2/opencv.hpp>

class SegmentTree {
public:
    SegmentTree(const cv::Mat& image);
    void update(int r1, int c1, int r2, int c2, const cv::Vec3b& value);
    cv::Vec3b query(int r, int c);
    void adjust_brightness(int r1, int c1, int r2, int c2, int value);

private:
    std::vector<cv::Vec3i> tree;
    int rows, cols;
    void build(const cv::Mat& image);
    void update_tree(int node, int start_r, int start_c, int end_r, int end_c, int r1, int c1, int r2, int c2, const cv::Vec3b& value);
    cv::Vec3b query_tree(int node, int start_r, int start_c, int end_r, int end_c, int r, int c);
    void adjust_brightness_tree(int node, int start_r, int start_c, int end_r, int end_c, int r1, int c1, int r2, int c2, int value);
};

#endif // SEGMENT_TREE_H
