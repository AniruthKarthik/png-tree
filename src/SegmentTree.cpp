#include "SegmentTree.h"
#include "types.h"
#include <stack>

SegmentTree::SegmentTree(const Image& image) {
    rows = image.get_height();
    cols = image.get_width();
    tree.resize(4 * rows * cols);
    build(1, 0, 0, rows - 1, cols - 1, image);
}

void SegmentTree::build(int node_idx, int start_r, int start_c, int end_r, int end_c, const Image& image) {
    if (start_r > end_r || start_c > end_c) return;

    if (start_r == end_r && start_c == end_c) {
        RGB_uc p = image.get_pixel(start_r, start_c);
        tree[node_idx].sum = {(double)p.r, (double)p.g, (double)p.b};
        return;
    }

    int mid_r = start_r + (end_r - start_r) / 2;
    int mid_c = start_c + (end_c - start_c) / 2;

    int child_base = node_idx * 4;
    build(child_base - 2, start_r, start_c, mid_r, mid_c, image);
    build(child_base - 1, start_r, mid_c + 1, mid_r, end_c, image);
    build(child_base + 0, mid_r + 1, start_c, end_r, mid_c, image);
    build(child_base + 1, mid_r + 1, mid_c + 1, end_r, end_c, image);

    tree[node_idx].sum = {0,0,0};
    tree[node_idx].sum += tree[child_base - 2].sum;
    tree[node_idx].sum += tree[child_base - 1].sum;
    tree[node_idx].sum += tree[child_base + 0].sum;
    tree[node_idx].sum += tree[child_base + 1].sum;
}

void SegmentTree::push(int node_idx, int start_r, int start_c, int end_r, int end_c) {
    Node& node = tree[node_idx];
    if (start_r == end_r && start_c == end_c) { // Leaf node
        if (node.is_lazy_set) {
            node.sum = {(double)node.lazy_set.r, (double)node.lazy_set.g, (double)node.lazy_set.b};
        }
        node.sum.r = node.sum.r * node.lazy_mul.r + node.lazy_add.r;
        node.sum.g = node.sum.g * node.lazy_mul.g + node.lazy_add.g;
        node.sum.b = node.sum.b * node.lazy_mul.b + node.lazy_add.b;
        node.lazy_mul = {1, 1, 1};
        node.lazy_add = {0, 0, 0};
        node.is_lazy_set = false;
        return;
    }

    int mid_r = start_r + (end_r - start_r) / 2;
    int mid_c = start_c + (end_c - start_c) / 2;

    int child_base = node_idx * 4;
    int children[] = {child_base - 2, child_base - 1, child_base + 0, child_base + 1};
    int r_ranges[] = {start_r, mid_r, start_r, mid_r, mid_r + 1, end_r, mid_r + 1, end_r};
    int c_ranges[] = {start_c, mid_c, mid_c + 1, end_c, start_c, mid_c, mid_c + 1, end_c};

    for (int i = 0; i < 4; ++i) {
        int child_idx = children[i];
        int r1 = r_ranges[i*2], r2 = r_ranges[i*2+1];
        int c1 = c_ranges[i*2], c2 = c_ranges[i*2+1];
        if (r1 > r2 || c1 > c2) continue;

        Node& child = tree[child_idx];
        long long num_pixels = (long long)(r2 - r1 + 1) * (c2 - c1 + 1);

        if (node.is_lazy_set) {
            child.is_lazy_set = true;
            child.lazy_set = node.lazy_set;
            child.sum = {(double)node.lazy_set.r * num_pixels, (double)node.lazy_set.g * num_pixels, (double)node.lazy_set.b * num_pixels};
            child.lazy_add = {0, 0, 0};
            child.lazy_mul = {1, 1, 1};
        }

        child.sum.r = child.sum.r * node.lazy_mul.r;
        child.sum.g = child.sum.g * node.lazy_mul.g;
        child.sum.b = child.sum.b * node.lazy_mul.b;
        child.lazy_mul.r *= node.lazy_mul.r;
        child.lazy_mul.g *= node.lazy_mul.g;
        child.lazy_mul.b *= node.lazy_mul.b;
        child.lazy_add.r *= node.lazy_mul.r;
        child.lazy_add.g *= node.lazy_mul.g;
        child.lazy_add.b *= node.lazy_mul.b;

        child.sum.r += num_pixels * node.lazy_add.r;
        child.sum.g += num_pixels * node.lazy_add.g;
        child.sum.b += num_pixels * node.lazy_add.b;
        child.lazy_add += node.lazy_add;
    }

    node.is_lazy_set = false;
    node.lazy_add = {0, 0, 0};
    node.lazy_mul = {1, 1, 1};
}

void SegmentTree::update(int node_idx, int start_r, int start_c, int end_r, int end_c, int r1, int c1, int r2, int c2, const RGB_d& mul_val, const RGB_d& add_val, const RGB_uc* set_val) {
    if (start_r > r2 || end_r < r1 || start_c > c2 || end_c < c1 || start_r > end_r || start_c > end_c) {
        return;
    }

    Node& node = tree[node_idx];
    if (r1 <= start_r && end_r <= r2 && c1 <= start_c && end_c <= c2) {
        long long num_pixels = (long long)(end_r - start_r + 1) * (end_c - start_c + 1);
        if (set_val) {
            node.is_lazy_set = true;
            node.lazy_set = *set_val;
            node.sum = {(double)set_val->r * num_pixels, (double)set_val->g * num_pixels, (double)set_val->b * num_pixels};
            node.lazy_add = {0, 0, 0};
            node.lazy_mul = {1, 1, 1};
        }
        node.sum.r = node.sum.r * mul_val.r;
        node.sum.g = node.sum.g * mul_val.g;
        node.sum.b = node.sum.b * mul_val.b;
        node.lazy_mul.r *= mul_val.r;
        node.lazy_mul.g *= mul_val.g;
        node.lazy_mul.b *= mul_val.b;
        node.lazy_add.r *= mul_val.r;
        node.lazy_add.g *= mul_val.g;
        node.lazy_add.b *= mul_val.b;

        node.sum.r += num_pixels * add_val.r;
        node.sum.g += num_pixels * add_val.g;
        node.sum.b += num_pixels * add_val.b;
        node.lazy_add += add_val;
        return;
    }

    push(node_idx, start_r, start_c, end_r, end_c);

    int mid_r = start_r + (end_r - start_r) / 2;
    int mid_c = start_c + (end_c - start_c) / 2;

    int child_base = node_idx * 4;
    update(child_base - 2, start_r, start_c, mid_r, mid_c, r1, c1, r2, c2, mul_val, add_val, set_val);
    update(child_base - 1, start_r, mid_c + 1, mid_r, end_c, r1, c1, r2, c2, mul_val, add_val, set_val);
    update(child_base + 0, mid_r + 1, start_c, end_r, mid_c, r1, c1, r2, c2, mul_val, add_val, set_val);
    update(child_base + 1, mid_r + 1, mid_c + 1, end_r, end_c, r1, c1, r2, c2, mul_val, add_val, set_val);

    node.sum = {0,0,0};
    node.sum += tree[child_base - 2].sum;
    node.sum += tree[child_base - 1].sum;
    node.sum += tree[child_base + 0].sum;
    node.sum += tree[child_base + 1].sum;
}

void SegmentTree::adjust_brightness(int r1, int c1, int r2, int c2, int value) {
    update(1, 0, 0, rows - 1, cols - 1, r1, c1, r2, c2, {1, 1, 1}, {(double)value, (double)value, (double)value}, nullptr);
}

void SegmentTree::adjust_contrast(int r1, int c1, int r2, int c2, double multiplier) {
    RGB_d add_val = {(1.0 - multiplier) * 128.0, (1.0 - multiplier) * 128.0, (1.0 - multiplier) * 128.0};
    update(1, 0, 0, rows - 1, cols - 1, r1, c1, r2, c2, {multiplier, multiplier, multiplier}, add_val, nullptr);
}

void SegmentTree::fill_region(int r1, int c1, int r2, int c2, const RGB_uc& color) {
    update(1, 0, 0, rows - 1, cols - 1, r1, c1, r2, c2, {1, 1, 1}, {0, 0, 0}, &color);
}

Image SegmentTree::get_image() {
    Image final_image(cols, rows);
    reconstruct_image_iterative(final_image);
    return final_image;
}

void SegmentTree::reconstruct_image_iterative(Image& image) {
    std::stack<std::tuple<int, int, int, int, int>> s;
    s.push({1, 0, 0, rows - 1, cols - 1});

    while (!s.empty()) {
        auto [node_idx, start_r, start_c, end_r, end_c] = s.top();
        s.pop();

        if (start_r > end_r || start_c > end_c) continue;

        Node& node = tree[node_idx];
        if (start_r == end_r && start_c == end_c) {
            push(node_idx, start_r, start_c, end_r, end_c);
            RGB_d final_color = node.sum;
            image.set_pixel(start_r, start_c, {saturate_cast_uchar(final_color.r), saturate_cast_uchar(final_color.g), saturate_cast_uchar(final_color.b)});
            continue;
        }

        push(node_idx, start_r, start_c, end_r, end_c);

        int mid_r = start_r + (end_r - start_r) / 2;
        int mid_c = start_c + (end_c - start_c) / 2;

        int child_base = node_idx * 4;
        s.push({child_base + 1, mid_r + 1, mid_c + 1, end_r, end_c});
        s.push({child_base + 0, mid_r + 1, start_c, end_r, mid_c});
        s.push({child_base - 1, start_r, mid_c + 1, mid_r, end_c});
        s.push({child_base - 2, start_r, start_c, mid_r, mid_c});
    }
}

RGB_d SegmentTree::query_average_color(int r1, int c1, int r2, int c2) {
    RGB_d total_sum = query_tree(1, 0, 0, rows - 1, cols - 1, r1, c1, r2, c2);
    long long num_pixels = (long long)(r2 - r1 + 1) * (c2 - c1 + 1);
    if (num_pixels == 0) return {0, 0, 0};
    return {total_sum.r / num_pixels, total_sum.g / num_pixels, total_sum.b / num_pixels};
}

RGB_d SegmentTree::query_tree(int node_idx, int start_r, int start_c, int end_r, int end_c, int r1, int c1, int r2, int c2) {
    if (start_r > r2 || end_r < r1 || start_c > c2 || end_c < c1 || start_r > end_r || start_c > end_c) {
        return {0, 0, 0};
    }

    if (r1 <= start_r && end_r <= r2 && c1 <= start_c && end_c <= c2) {
        return tree[node_idx].sum;
    }

    push(node_idx, start_r, start_c, end_r, end_c);

    int mid_r = start_r + (end_r - start_r) / 2;
    int mid_c = start_c + (end_c - start_c) / 2;

    RGB_d result = {0, 0, 0};
    int child_base = node_idx * 4;
    result += query_tree(child_base - 2, start_r, start_c, mid_r, mid_c, r1, c1, r2, c2);
    result += query_tree(child_base - 1, start_r, mid_c + 1, mid_r, end_c, r1, c1, r2, c2);
    result += query_tree(child_base + 0, mid_r + 1, start_c, end_r, mid_c, r1, c1, r2, c2);
    result += query_tree(child_base + 1, mid_r + 1, mid_c + 1, end_r, end_c, r1, c1, r2, c2);

    return result;
}