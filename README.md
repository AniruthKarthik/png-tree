# Case Study: High-Performance 2D Image Manipulation in C++

This project provides a detailed performance comparison between two in-memory data structures for common 2D image processing tasks. It analyzes a naive `std::vector` implementation against a sophisticated quadtree-based `SegmentTree` to determine the optimal choice for different workloads.

## The Contenders: Data Structures

The core of this case study is the trade-off between simplicity and algorithmic efficiency.

### 1. `VectorImage`
- **Implementation:** A simple, row-major `std::vector<RGB_uc>` that stores pixel data contiguously.
- **How it Works:** Operations like adjusting brightness or filling a region are performed by iterating through every pixel within the target rectangle.
- **Pros:**
    - Simple to implement and understand.
    - Low memory overhead per pixel.
    - Potentially very fast for small regions, as modern compilers can heavily optimize simple loops (e.g., into `memset`).
- **Cons:**
    - Inefficient for large region updates. The complexity for updating a region of `N` pixels is `O(N)`.

### 2. `SegmentTree`
- **Implementation:** A quadtree-based segment tree. The image is recursively divided into four quadrants, with each node in the tree representing a rectangular region of the image.
- **How it Works:** It uses **lazy propagation** for updates. Instead of changing every pixel, an operation is "tagged" on a high-level node and only propagated down to its children when necessary. This allows for massive regions to be updated by modifying only a few nodes in the tree.
- **Pros:**
    - Extremely fast for large region updates, with a time complexity of `O(log N)`.
    - Efficient for complex updates (e.g., multiplication) across regions of any size.
- **Cons:**
    - Significantly more complex to implement.
    - Higher memory footprint due to the tree structure.
    - Traversal overhead can make it slower than `VectorImage` for simple updates on very small regions.

## The Experiment: Methodology

To produce a clear winner, the two data structures were benchmarked on a **4096x4096** image. The benchmark measured the time taken to perform two key operations across a matrix of region sizes and iteration counts.

- **Operations Tested:**
    1.  **Fill Region**: Setting all pixels in a region to a solid color.
    2.  **Adjust Brightness**: Adding a value to all pixels in a region.
- **Test Scenarios:**
    - **Region Sizes:** `4096x4096`, `1080x1080`, `64x64`
    - **Iteration Counts:** `1`, `1000`, `100000`

## The Results: Analysis

The benchmark data provides a clear picture of the strengths and weaknesses of each approach.

| Operation         | Region Size | Iterations | `VectorImage` Time (ms) | `SegmentTree` Time (ms) | Efficiency | Winner      |
| :---------------- | :---------- | :--------- | :---------------------- | :---------------------- | :--------- | :---------- |
| Fill Region       | 4096x4096   | 1000       | 11503.30                | 0.0287                  | `400811x`  | SegmentTree |
| Adjust Brightness | 4096x4096   | 1000       | 28811.00                | 0.0283                  | `1018056x` | SegmentTree |
| Fill Region       | 64x64       | 1000       | 3.9043                  | 36.1645                 | `0.11x`    | VectorImage |
| Adjust Brightness | 64x64       | 1000       | 44.2902                 | 34.3142                 | `1.3x`     | SegmentTree |

**Key Findings:**

1.  **`SegmentTree` Dominates Large Regions:** The `SegmentTree` is exponentially faster when performing updates on large regions. For a full-image brightness adjustment, it was over **1,000,000x faster** than the naive vector implementation. This is a direct result of its `O(log N)` complexity.

2.  **`VectorImage` Wins on Small, Simple Updates:** For `Fill Region` operations on a small 64x64 area, the `VectorImage` was the clear winner. The overhead of traversing the segment tree was more costly than the simple, highly optimizable loop used by the vector.

3.  **`SegmentTree` Wins on Complex Updates:** For `Adjust Brightness`, the `SegmentTree` was faster across **all region sizes**. Because this operation involves both multiplication and addition (not just a simple `memset`), the logarithmic complexity of the tree outweighed the simplicity of the vector loop even for small regions.

## Conclusion: The Right Tool for the Job

The choice of data structure is critically dependent on the expected workload.

-   **Use a `SegmentTree`** (or a similar hierarchical structure) for applications that involve frequent updates on medium-to-large sized regions, or for any application performing complex mathematical adjustments. The implementation complexity is paid back in massive performance gains.

-   **Use a `VectorImage`** (a simple vector) only when the primary workload consists of frequent, simple `set` operations on very small regions of the image, or when read-only access is the primary concern.

## How to Build and Run

### Prerequisites
- A C++ compiler (e.g., g++)
- `make`

### Compilation
Run the `make` command in the project root to compile the application.
```bash
make
```
This will create an executable at `build/image_app`.

### Running the CLI
To run the interactive command-line interface:
```bash
./build/image_app
```

## CLI Usage

The application will present a menu of options:

- **1. Generate New Random Image**: Creates a new random image.
- **2. Adjust Brightness**: Modifies brightness for a specified region.
- **3. Adjust Contrast**: Modifies contrast for a specified region.
- **4. Fill Region with Color**: Fills a region with a solid color.
- **5. Query Average Color**: Calculates the average RGB value for a region.
- **6. Delete Row/Column**: Removes a row or column to resize the image.
- **7. Blur Image**: Applies a 3x3 box blur to a specified region.
- **8. Reset to Original**: Reverts all changes.
- **9. Benchmark (Single)**: Run a single, user-defined benchmark.
- **10. Benchmark (Many)**: Run a randomized stress test.
- **0. Exit**: Exits the program.