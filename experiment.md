# Performance Analysis: VectorImage vs. SegmentTree

## 1. Introduction

This document presents a detailed performance comparison between two data structures for 2D image manipulation:

*   **`VectorImage`**: A simple, row-major `std::vector` where operations are performed by iterating through pixels.
*   **`SegmentTree`**: A quadtree-based segment tree that uses lazy propagation for efficient range updates.

The goal is to identify which data structure is more performant across a matrix of different workloads, varying the operation type, region size, and number of iterations.

## 2. Methodology

The benchmark was conducted on a **4096x4096** image.

Two primary operations were tested:
1.  **Fill Region**: Setting a rectangular region to a solid color.
2.  **Adjust Brightness**: Adding a value to all pixels in a rectangular region.

The time measured reflects **only the duration of the update operation(s)** and does not include image reconstruction time. This provides a pure measure of the algorithm's time complexity.

Each operation was tested against every combination of the following scenarios:
*   **Region Sizes:** `4096x4096`, `1080x1080`, `64x64`
*   **Iteration Counts:** `1`, `1000`, `100000`

## 3. Benchmark Results

The following table shows the total time taken in milliseconds (ms) and the performance efficiency of the `SegmentTree` relative to the `VectorImage`.

| Operation         | Region Size | Iterations | Vector Time (ms) | Tree Time (ms) | Efficiency | Winner      |
| :---------------- | :---------- | :--------- | :--------------- | :------------- | :--------- | :---------- |
| Fill Region       | 4096x4096   | 1          | 11.8502          | 0.0016         | `7217x`    | SegmentTree |
| Fill Region       | 4096x4096   | 1000       | 11503.3000       | 0.0287         | `400811x`  | SegmentTree |
| Fill Region       | 1080x1080   | 1          | 0.8004           | 0.5313         | `1.5x`     | SegmentTree |
| Fill Region       | 1080x1080   | 1000       | 829.5790         | 431.2250       | `1.9x`     | SegmentTree |
| Fill Region       | 64x64       | 1          | 0.0063           | 0.0127         | `0.50x`    | VectorImage |
| Fill Region       | 64x64       | 1000       | 3.9043           | 36.1645        | `0.11x`    | VectorImage |
| Fill Region       | 64x64       | 100000     | 436.4340         | 2669.8600      | `0.16x`    | VectorImage |
| Adjust Brightness | 4096x4096   | 1          | 99.3899          | 0.0016         | `61162x`   | SegmentTree |
| Adjust Brightness | 4096x4096   | 1000       | 28811.0000       | 0.0283         | `1018056x` | SegmentTree |
| Adjust Brightness | 1080x1080   | 1          | 7.6897           | 0.6845         | `11.2x`    | SegmentTree |
| Adjust Brightness | 1080x1080   | 1000       | 4303.7900        | 583.2260       | `7.4x`     | SegmentTree |
| Adjust Brightness | 64x64       | 1          | 0.0432           | 0.0106         | `4.1x`     | SegmentTree |
| Adjust Brightness | 64x64       | 1000       | 44.2902          | 34.3142        | `1.3x`     | SegmentTree |
| Adjust Brightness | 64x64       | 100000     | 4976.4800        | 3497.8500      | `1.4x`     | SegmentTree |

## 4. Analysis and Conclusion

The results provide a clear and detailed picture of the strengths and weaknesses of each data structure.

### `SegmentTree` Dominance

The `SegmentTree` is overwhelmingly superior when performing updates on **large regions**. For a single update on the full 4096x4096 image, it is thousands of times faster. This is its core strength: the time to update a large region is `O(log N)` because it only modifies a few nodes in the tree, whereas the `VectorImage` must touch every pixel (`O(N)`).

Interestingly, for the `Adjust Brightness` operation, the `SegmentTree` remained the winner across **all tested scenarios**. Its logarithmic complexity was even more efficient than the `VectorImage`'s simple loop for 64x64 regions.

### `VectorImage` Dominance

The `VectorImage`'s strength lies in its simplicity and low overhead. It is the decisive winner when performing **`Fill Region` operations on small regions** (e.g., 64x64). For this specific operation, modern compilers can heavily optimize the simple loop into something resembling `memset`, which is extremely fast for small amounts of data. The `SegmentTree`'s traversal overhead, while logarithmic, is more costly than the brute-force memory set for these small regions.

### Final Verdict

The choice of data structure is critically dependent on the expected workload.

*   **`SegmentTree` is the ideal choice** for applications involving frequent updates on medium-to-large sized regions, or for complex updates (like addition) across most region sizes.

*   **`VectorImage` is the better choice** only when the primary workload consists of frequent, simple `set` operations on very small regions of the image.
