# Image Processor Case Study

This project is a C++ application designed to explore and compare the performance of two different in-memory image representations for common image processing tasks. It serves as a practical case study in data structure and algorithm efficiency in the context of image manipulation.

## Core Concepts: Image Representations

The central theme of this project is the comparison between a library-provided, optimized data structure and a standard, more generic one.

1.  **`ImageProcessor` (using `cv::Mat`)**
    *   This class utilizes `cv::Mat`, the core data structure for image representation in the popular OpenCV library.
    *   `cv::Mat` is a highly optimized n-dimensional array designed for speed and efficiency in image processing tasks. It manages its own memory and often uses continuous memory blocks for faster row access.

2.  **`VectorImage` (using `std::vector`)**
    *   This class uses a `std::vector<std::vector<cv::Vec3b>>` to represent the image.
    *   This is a more "naive" or standard C++ approach, where each row of the image is a `std::vector`, and the image itself is a `std::vector` of these rows. While flexible, this can lead to performance overhead due to non-contiguous memory and pointer chasing.

## Features

The application provides a console-based menu to perform the following operations on an image:

*   **Generate New Random Image**: Creates a new image with random pixel values.
*   **Adjust Brightness**: Modifies the brightness of a selected rectangular region.
*   **Adjust Contrast**: Modifies the contrast of a selected rectangular region.
*   **Fill Region with Color**: Fills a selected rectangular region with a specified color.
*   **Flip Image**: Flips the image vertically or horizontally.
*   **Rotate Image**: Rotates the image 90 degrees clockwise.
*   **Save Image**: Saves the current image to the `output/` directory.
*   **Reset to Original**: Reverts all changes to the last generated or loaded image.
*   **Benchmark**: Provides a tool to compare the performance of `ImageProcessor` and `VectorImage` for key operations.

## Building and Running

The project uses a `Makefile` for easy compilation.

### Prerequisites

*   A C++ compiler (like g++)
*   OpenCV library installed

### Compilation

To compile the application, run the `make` command in the project root directory:

```bash
make
```

This will create an executable file named `image_app` in the `build/` directory.

### Execution

To run the application, execute the following command:

```bash
./build/image_app
```

This will launch the interactive console menu.

## Benchmarking

The application includes a built-in benchmarking tool to compare the `cv::Mat` and `std::vector` approaches. The tool measures the execution time for the following operations across user-defined image sizes:

*   **Adjust Brightness**
*   **Adjust Contrast**
*   **Fill Region**

The results of these benchmarks are intended to demonstrate the real-world performance impact of choosing an appropriate data structure for a specific task. The findings are documented in `experiment.md`.
