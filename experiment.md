# Benchmark Experiment: `cv::Mat` vs. `std::vector`

This document presents the results of a benchmark comparison between two image representation methods:

1.  **`ImageProcessor`**: Utilizes `cv::Mat` from the OpenCV library.
2.  **`VectorImage`**: Utilizes a `std::vector<std::vector<cv::Vec3b>>`.

The goal is to measure the performance of these two approaches across different image sizes and common operations.

## Methodology

The benchmarks were executed on a Linux system for three image size categories:

*   **Small**: 100x100 pixels
*   **Medium**: 1000x1000 pixels
*   **Large**: 5000x5000 pixels

For each size, the following operations were benchmarked:

*   **Adjust Brightness**: Modifying the brightness of the entire image.
*   **Adjust Contrast**: Modifying the contrast of the entire image.
*   **Fill Region**: Filling the entire image with a solid color.

The runtime of each operation was measured in milliseconds (ms).

## Benchmark Results

| Image Size | Operation | `ImageProcessor` (`cv::Mat`) | `VectorImage` (`std::vector`) | Winner | Performance Difference |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **Small** | Adjust Brightness | 0.370 ms | 0.374 ms | `cv::Mat` | 1.08% |
| (100x100) | Adjust Contrast | 0.962 ms | 0.893 ms | `std::vector` | 7.17% |
| | Fill Region | 6.185 ms | 0.038 ms | `std::vector` | 99.39% |
| **Medium** | Adjust Brightness | 42.740 ms | 39.699 ms | `std::vector` | 7.11% |
| (1000x1000) | Adjust Contrast | 91.082 ms | 90.801 ms | `std::vector` | 0.31% |
| | Fill Region | 0.492 ms | 3.792 ms | `cv::Mat` | 87.02% |
| **Large** | Adjust Brightness | 1022.7 ms | 976.191 ms | `std::vector` | 4.55% |
| (5000x5000) | Adjust Contrast | 2308.82 ms | 2288.62 ms | `std::vector` | 0.87% |
| | Fill Region | 11.100 ms | 90.984 ms | `cv::Mat` | 87.80% |

## Analysis of Results

### Adjust Brightness & Adjust Contrast

For both `Adjust Brightness` and `Adjust Contrast`, the performance of `cv::Mat` and `std::vector` is very similar, with `std::vector` being slightly faster in most cases. This is because both operations are pixel-wise and iterate through the entire image data. The overhead of `std::vector`'s non-contiguous memory is minimal here compared to the actual pixel calculations.

### Fill Region

This is where the difference is most dramatic. `cv::Mat` is significantly faster for medium and large images, while `std::vector` is faster for the small image.

*   For **medium and large images**, `cv::Mat`'s `roi` (region of interest) and `setTo` methods are highly optimized, likely using low-level memory operations like `memset` to fill the continuous memory block of the image. This is much faster than iterating through each pixel in a nested `std::vector`.

*   For the **small image**, the `std::vector` implementation was surprisingly faster. This could be due to the overhead of creating the `cv::Mat` ROI object for such a small image, which might be greater than the simple loop for the vector.

## Complexity Analysis

Let `R` be the number of rows and `C` be the number of columns in the image region being processed.

### `ImageProcessor` (`cv::Mat`)

| Operation | Time Complexity | Space Complexity | Notes |
| :--- | :--- | :--- | :--- |
| Adjust Brightness | O(R * C) | O(1) | Iterates through each pixel in the region. |
| Adjust Contrast | O(R * C) | O(1) | Iterates through each pixel in the region. |
| Fill Region | O(1) or O(R*C) | O(1) | `cv::Mat::setTo` is highly optimized and can be considered near-constant time for practical purposes, though it technically still touches every pixel. |

### `VectorImage` (`std::vector`)

| Operation | Time Complexity | Space Complexity | Notes |
| :--- | :--- | :--- | :--- |
| Adjust Brightness | O(R * C) | O(1) | Iterates through each pixel in the region. |
| Adjust Contrast | O(R * C) | O(1) | Iterates through each pixel in the region. |
| Fill Region | O(R * C) | O(1) | Iterates through each pixel in the region. |

## Conclusion

*   For **pixel-wise operations** like brightness and contrast adjustments, the difference between `cv::Mat` and a standard `std::vector` is not significant, especially for smaller images.

*   For **region-based operations** that can be optimized with memory-level instructions, `cv::Mat` is the clear winner, especially as the image size increases. Its ability to work with continuous blocks of memory provides a significant performance advantage.

This experiment demonstrates the importance of choosing the right data structure for the task. While a `std::vector` is a versatile tool, for performance-critical image processing, a specialized and optimized library like OpenCV provides invaluable advantages.
