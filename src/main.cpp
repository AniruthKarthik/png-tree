#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include "ImageProcessor.h"
#include "VectorImage.h"

// --- Image Display ---
void print_image_terminal(const cv::Mat& img) {
    if (img.empty() || img.cols <= 0 || img.rows <= 0) {
        std::cout << "[Image is empty]" << std::endl;
        return;
    }

    for (int r = 0; r < img.rows; ++r) {
        for (int c = 0; c < img.cols; ++c) {
            cv::Vec3b pixel = img.at<cv::Vec3b>(r, c);
            std::cout << "[48;2;" << (int)pixel[2] << ";" << (int)pixel[1] << ";" << (int)pixel[0] << "m ";
        }
        std::cout << "[0m\n";
    }
    std::cout << "[0m";
}

// --- UI and App Logic ---
void print_menu() {
    std::cout << "\n\x1b[1;34m──────────────────────────────────────────────\x1b[0m\n";
    std::cout << "\x1b[1;36mMENU\x1b[0m\n";
    std::cout << "\x1b[1;34m──────────────────────────────────────────────\x1b[0m\n";
    std::cout << "1. Generate New Random Image      6. Flip Image\n";
    std::cout << "2. Adjust Brightness              7. Rotate Image\n";
    std::cout << "3. Adjust Contrast                8. Reset to Original\n";
    std::cout << "4. Fill Region with Color         9. Benchmark\n";
    std::cout << "5. Save Image to 'output'         0. Exit\n";
    std::cout << "\x1b[1;34m──────────────────────────────────────────────\x1b[0m\n";
    std::cout << "Enter your choice: ";
}


bool get_rect(int max_rows, int max_cols, int& r1, int& c1, int& r2, int& c2) {
    std::cout << "Enter Top-Left Corner (row col): ";
    std::cin >> r1 >> c1;
    std::cout << "Enter Bottom-Right Corner (row col): ";
    std::cin >> r2 >> c2;

    if (std::cin.fail() || r1 < 0 || c1 < 0 || r2 >= max_rows || c2 >= max_cols || r1 > r2 || c1 > c2) {
        std::cout << "\x1b[31mError: Invalid coordinates.\x1b[0m\n";
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        return false;
    }
    return true;
}

// --- Main Loop ---
int main() {
    ImageProcessor processor(128, 128);
    cv::Mat original_image = processor.get_image();
    int output_counter = 0;

    std::cout << "Generated initial 128x128 random image." << std::endl;
    print_image_terminal(original_image);

    int choice = -1;
    do {
        print_menu();
        std::cin >> choice;

        if (std::cin.eof()) { choice = 0; continue; }
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            choice = -1;
        }

        std::cout << std::endl;

        cv::Mat before_img = processor.get_image();

        switch (choice) {
            case 1: {
                processor.generate_random();
                original_image = processor.get_image();
                std::cout << "Generated new random image." << std::endl;
                print_image_terminal(processor.get_image());
                break;
            }
            case 2: { // Brightness
                int r1, c1, r2, c2, value;
                if (!get_rect(128, 128, r1, c1, r2, c2)) break;
                std::cout << "Enter brightness adjustment (-255 to 255): ";
                std::cin >> value;
                
                std::cout << "\nBefore:\n";
                print_image_terminal(before_img);
                processor.adjust_brightness(r1, c1, r2, c2, value);
                std::cout << "\nAfter:\n";
                print_image_terminal(processor.get_image());
                break;
            }
            case 3: { // Contrast
                int r1, c1, r2, c2;
                double multiplier;
                if (!get_rect(128, 128, r1, c1, r2, c2)) break;
                std::cout << "Enter contrast multiplier (> 0.0): ";
                std::cin >> multiplier;

                std::cout << "\nBefore:\n";
                print_image_terminal(before_img);
                processor.adjust_contrast(r1, c1, r2, c2, multiplier);
                std::cout << "\nAfter:\n";
                print_image_terminal(processor.get_image());
                break;
            }
            case 4: { // Fill Region
                int r1, c1, r2, c2, r, g, b;
                if (!get_rect(128, 128, r1, c1, r2, c2)) break;
                std::cout << "Enter color (R G B): ";
                std::cin >> r >> g >> b;

                std::cout << "\nBefore:\n";
                print_image_terminal(before_img);
                processor.fill_region(r1, c1, r2, c2, cv::Vec3b(b, g, r));
                std::cout << "\nAfter:\n";
                print_image_terminal(processor.get_image());
                break;
            }
            case 5: { // Save
                std::string out_path = "output/" + std::to_string(output_counter++) + ".png";
                cv::imwrite(out_path, processor.get_image());
                std::cout << "Saved image to " << out_path << std::endl;
                break;
            }
            case 6: { // Flip
                int direction;
                std::cout << "Enter flip direction (0: Vertical, 1: Horizontal): ";
                std::cin >> direction;

                std::cout << "\nBefore:\n";
                print_image_terminal(before_img);
                processor.flip(direction);
                std::cout << "\nAfter:\n";
                print_image_terminal(processor.get_image());
                break;
            }
            case 7: { // Rotate
                std::cout << "\nBefore:\n";
                print_image_terminal(before_img);
                processor.rotate();
                std::cout << "\nAfter:\n";
                print_image_terminal(processor.get_image());
                break;
            }
            case 8: { // Reset
                processor.set_image(original_image);
                std::cout << "Image reset to original." << std::endl;
                print_image_terminal(processor.get_image());
                break;
            }
            case 9: { // Benchmark
                std::cout << "Select operation to benchmark:\n";
                std::cout << "1. Adjust Brightness\n";
                std::cout << "2. Adjust Contrast\n";
                std::cout << "3. Fill Region\n";
                std::cout << "Enter your choice: ";
                int bench_choice;
                std::cin >> bench_choice;

                if (bench_choice == 1) {
                    int r1, c1, r2, c2, value;
                    if (!get_rect(128, 128, r1, c1, r2, c2)) break;
                    std::cout << "Enter brightness adjustment (-255 to 255): ";
                    std::cin >> value;

                    ImageProcessor ip(128, 128);
                    VectorImage vi(128, 128);

                    auto start_ip = std::chrono::high_resolution_clock::now();
                    ip.adjust_brightness(r1, c1, r2, c2, value);
                    auto end_ip = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> ip_duration = end_ip - start_ip;

                    auto start_vi = std::chrono::high_resolution_clock::now();
                    vi.adjust_brightness(r1, c1, r2, c2, value);
                    auto end_vi = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> vi_duration = end_vi - start_vi;

                    std::cout << "\n--- Benchmark Results ---" << std::endl;
                    std::cout << "ImageProcessor (cv::Mat): " << ip_duration.count() << " seconds" << std::endl;
                    std::cout << "VectorImage (std::vector): " << vi_duration.count() << " seconds" << std::endl;
                } else if (bench_choice == 2) {
                    int r1, c1, r2, c2;
                    double multiplier;
                    if (!get_rect(128, 128, r1, c1, r2, c2)) break;
                    std::cout << "Enter contrast multiplier (> 0.0): ";
                    std::cin >> multiplier;

                    ImageProcessor ip(128, 128);
                    VectorImage vi(128, 128);

                    auto start_ip = std::chrono::high_resolution_clock::now();
                    ip.adjust_contrast(r1, c1, r2, c2, multiplier);
                    auto end_ip = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> ip_duration = end_ip - start_ip;

                    auto start_vi = std::chrono::high_resolution_clock::now();
                    vi.adjust_contrast(r1, c1, r2, c2, multiplier);
                    auto end_vi = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> vi_duration = end_vi - start_vi;

                    std::cout << "\n--- Benchmark Results ---" << std::endl;
                    std::cout << "ImageProcessor (cv::Mat): " << ip_duration.count() << " seconds" << std::endl;
                    std::cout << "VectorImage (std::vector): " << vi_duration.count() << " seconds" << std::endl;
                } else if (bench_choice == 3) {
                    int r1, c1, r2, c2, r, g, b;
                    if (!get_rect(128, 128, r1, c1, r2, c2)) break;
                    std::cout << "Enter color (R G B): ";
                    std::cin >> r >> g >> b;

                    ImageProcessor ip(128, 128);
                    VectorImage vi(128, 128);

                    auto start_ip = std::chrono::high_resolution_clock::now();
                    ip.fill_region(r1, c1, r2, c2, cv::Vec3b(b, g, r));
                    auto end_ip = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> ip_duration = end_ip - start_ip;

                    auto start_vi = std::chrono::high_resolution_clock::now();
                    vi.fill_region(r1, c1, r2, c2, cv::Vec3b(b, g, r));
                    auto end_vi = std::chrono::high_resolution_clock::now();
                    std::chrono::duration<double> vi_duration = end_vi - start_vi;

                    std::cout << "\n--- Benchmark Results ---" << std::endl;
                    std::cout << "ImageProcessor (cv::Mat): " << ip_duration.count() << " seconds" << std::endl;
                    std::cout << "VectorImage (std::vector): " << vi_duration.count() << " seconds" << std::endl;
                }
                break;
            }
            case 0: std::cout << "Exiting.\n"; break;
            default: std::cout << "\x1b[31mInvalid choice. Please try again.\x1b[0m\n"; break;
        }
    } while (choice != 0);

    return 0;
}
