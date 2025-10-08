#include "Image.h"
#include "ImageProcessor.h"
#include "SegmentTree.h"
#include "VectorImage.h"
#include "types.h"
#include <chrono>
#include <iostream>
#include <limits>
#include <random>
#include <string>
#include <vector>

// --- Image Display ---
void print_image_terminal(const Image &img)
{
	if (img.get_width() <= 0 || img.get_height() <= 0)
	{
		std::cout << "[Image is empty]" << std::endl;
		return;
	}

	for (int r = 0; r < img.get_height(); ++r)
	{
		for (int c = 0; c < img.get_width(); ++c)
		{
			RGB_uc pixel = img.get_pixel(r, c);
			std::cout << "\033[48;2;" << (int)pixel.r << ";" << (int)pixel.g
			          << ";" << (int)pixel.b << "m ";
		}
		std::cout << "\033[0m\n";
	}
	std::cout << "\033[0m";
}

// --- UI and App Logic ---
void print_menu()
{
	std::cout << R"(
┌──────────────────────────────────────────────────┐
│                       MENU                       │
├──────────────────────────────────────────────────┤
│  1. Generate New Random Image   6. Delete Row/Column  │
│  2. Adjust Brightness           7. Blur Image        │
│  3. Adjust Contrast             8. Reset to Original │
│  4. Fill Region with Color      9. Benchmark (Single)│
│  5. Query Average Color        10. Benchmark (Many)  │
│ 11. Histogram                   0. Exit            │
└──────────────────────────────────────────────────┘
)";
	std::cout << "Enter your choice: ";
}

bool get_rect(int max_rows, int max_cols, int &r1, int &c1, int &r2, int &c2)
{
	std::cout << "Enter Top-Left Corner (row col): ";
	std::cin >> r1 >> c1;
	std::cout << "Enter Bottom-Right Corner (row col): ";
	std::cin >> r2 >> c2;

	if (std::cin.fail() || r1 < 0 || c1 < 0 || r2 >= max_rows ||
	    c2 >= max_cols || r1 > r2 || c1 > c2)
	{
		std::cout << "\x1b[31mError: Invalid coordinates.\x1b[0m\n";
		std::cin.clear();
		std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
		return false;
	}
	return true;
}

// --- Main Loop ---
int main()
{
	ImageProcessor processor(64, 32);
	Image original_image = processor.get_image();
	SegmentTree st(original_image);

	std::cout << "Generated initial 64x32 random image." << std::endl;
	print_image_terminal(original_image);

	int choice = -1;
	do
	{
		print_menu();
		std::cin >> choice;

		if (std::cin.eof())
		{
			choice = 0;
			continue;
		}
		if (std::cin.fail())
		{
			std::cin.clear();
			std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
			choice = -1;
		}

		std::cout << std::endl;

		switch (choice)
		{
		case 1: {
			processor.generate_random();
			original_image = processor.get_image();
			st = SegmentTree(original_image);
			std::cout << "Generated new random image." << std::endl;
			print_image_terminal(original_image);
			break;
		}
		case 2: { // Brightness
			int r1, c1, r2, c2, value;
			if (!get_rect(original_image.get_height(),
			              original_image.get_width(), r1, c1, r2, c2))
				break;
			std::cout << "Enter brightness adjustment (-255 to 255): ";
			std::cin >> value;

			st.adjust_brightness(r1, c1, r2, c2, value);
			std::cout << "\nAfter:\n";
			print_image_terminal(st.get_image());
			break;
		}
		case 3: { // Contrast
			int r1, c1, r2, c2;
			double multiplier;
			if (!get_rect(original_image.get_height(),
			              original_image.get_width(), r1, c1, r2, c2))
				break;
			std::cout << "Enter contrast multiplier (> 0.0): ";
			std::cin >> multiplier;

			st.adjust_contrast(r1, c1, r2, c2, multiplier);
			std::cout << "\nAfter:\n";
			print_image_terminal(st.get_image());
			break;
		}
		case 4: { // Fill Region
			int r1, c1, r2, c2, r, g, b;
			if (!get_rect(original_image.get_height(),
			              original_image.get_width(), r1, c1, r2, c2))
				break;
			std::cout << "Enter color (R G B): ";
			std::cin >> r >> g >> b;

			st.fill_region(
			    r1, c1, r2, c2,
			    {(unsigned char)r, (unsigned char)g, (unsigned char)b});
			std::cout << "\nAfter:\n";
			print_image_terminal(st.get_image());
			break;
		}

		case 5: { // Query Average Color
			int r1, c1, r2, c2;
			if (!get_rect(original_image.get_height(),
			              original_image.get_width(), r1, c1, r2, c2))
				break;

			RGB_d avg = st.query_average_color(r1, c1, r2, c2);
			std::cout << "Average color in region: (R=" << avg.r
			          << ", G=" << avg.g << ", B=" << avg.b << ")" << std::endl;
			break;
		}

		case 6: { // Delete Row/Column
			Image before_img = st.get_image();
			std::cout << "Delete row or col? ";
			std::string choice;
			std::cin >> choice;

			int num_to_delete;
			int old_height = before_img.get_height();
			int old_width = before_img.get_width();

			if (choice == "row")
			{
				std::cout << "Enter row number to delete: ";
				std::cin >> num_to_delete;
				if (std::cin.fail() || num_to_delete < 0 ||
				    num_to_delete >= old_height)
				{
					std::cout << "\x1b[31mError: Invalid row number.\x1b[0m\n";
					break;
				}

				Image new_image(old_width, old_height - 1);
				for (int r = 0, new_r = 0; r < old_height; ++r)
				{
					if (r == num_to_delete)
						continue;
					for (int c = 0; c < old_width; ++c)
					{
						new_image.set_pixel(new_r, c,
						                    before_img.get_pixel(r, c));
					}
					new_r++;
				}
				original_image = new_image;
				st = SegmentTree(new_image);
			}
			else if (choice == "col")
			{
				std::cout << "Enter column number to delete: ";
				std::cin >> num_to_delete;
				if (std::cin.fail() || num_to_delete < 0 ||
				    num_to_delete >= old_width)
				{
					std::cout
					    << "\x1b[31mError: Invalid column number.\x1b[0m\n";
					break;
				}

				Image new_image(old_width - 1, old_height);
				for (int r = 0; r < old_height; ++r)
				{
					for (int c = 0, new_c = 0; c < old_width; ++c)
					{
						if (c == num_to_delete)
							continue;
						new_image.set_pixel(r, new_c,
						                    before_img.get_pixel(r, c));
						new_c++;
					}
				}
				original_image = new_image;
				st = SegmentTree(new_image);
			}
			else
			{
				std::cout << "\x1b[31mError: Invalid choice.\x1b[0m\n";
				break;
			}

			std::cout << "\nBefore:\n";
			print_image_terminal(before_img);
			std::cout << "\nAfter:\n";
			print_image_terminal(st.get_image());
			break;
		}

		case 7: { // Blur
			int r1, c1, r2, c2;
			if (!get_rect(original_image.get_height(),
			              original_image.get_width(), r1, c1, r2, c2))
				break;

			Image before_img = st.get_image();
			Image blurred_img = st.blur(r1, c1, r2, c2);

			std::cout << "\nBefore:\n";
			print_image_terminal(before_img);
			std::cout << "\nAfter:\n";
			print_image_terminal(blurred_img);
			break;
		}

		case 8: { // Reset
			st = SegmentTree(original_image);
			std::cout << "Image reset to original." << std::endl;
			print_image_terminal(original_image);
			break;
		}

		case 9: { // Benchmark (Single)
			int width, height;
			std::cout
			    << "Enter image resolution for benchmark (width height): ";
			std::cin >> width >> height;

			if (std::cin.fail() || width <= 0 || height <= 0)
			{
				std::cout << "\x1b[31mError: Invalid resolution.\x1b[0m\n";
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
				                '\n');
				break;
			}

			std::cout << "Select operation to benchmark:\n";
			std::cout << "1. Adjust Brightness\n";
			std::cout << "2. Adjust Contrast\n";
			std::cout << "3. Fill Region\n";
			std::cout << "Enter your choice: ";
			int bench_choice;
			std::cin >> bench_choice;

			Image bench_image(width, height);
			bench_image.generate_random();

			if (bench_choice == 1)
			{
				int r1, c1, r2, c2, value;
				if (!get_rect(height, width, r1, c1, r2, c2))
					break;
				std::cout << "Enter brightness adjustment (-255 to 255): ";
				std::cin >> value;

				VectorImage vi(width, height);
				SegmentTree st_bench(bench_image);

				auto start_vi = std::chrono::high_resolution_clock::now();
				vi.adjust_brightness(r1, c1, r2, c2, value);
				auto end_vi = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> vi_duration = end_vi - start_vi;

				auto start_st = std::chrono::high_resolution_clock::now();
				st_bench.adjust_brightness(r1, c1, r2, c2, value);
				auto end_st = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> st_duration = end_st - start_st;

				std::cout << "\n--- Benchmark Results ---" << std::endl;
				std::cout << "VectorImage (std::vector): "
				          << vi_duration.count() * 1e6 << " us" << std::endl;
				std::cout << "SegmentTree: " << st_duration.count() * 1e6
				          << " us" << std::endl;
			}
			else if (bench_choice == 2)
			{
				int r1, c1, r2, c2;
				double multiplier;
				if (!get_rect(height, width, r1, c1, r2, c2))
					break;
				std::cout << "Enter contrast multiplier (> 0.0): ";
				std::cin >> multiplier;

				VectorImage vi(width, height);
				SegmentTree st_bench(bench_image);

				auto start_vi = std::chrono::high_resolution_clock::now();
				vi.adjust_contrast(r1, c1, r2, c2, multiplier);
				auto end_vi = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> vi_duration = end_vi - start_vi;

				auto start_st = std::chrono::high_resolution_clock::now();
				st_bench.adjust_contrast(r1, c1, r2, c2, multiplier);
				auto end_st = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> st_duration = end_st - start_st;

				std::cout << "\n--- Benchmark Results ---" << std::endl;
				std::cout << "VectorImage (std::vector): "
				          << vi_duration.count() * 1e6 << " us" << std::endl;
				std::cout << "SegmentTree: " << st_duration.count() * 1e6
				          << " us" << std::endl;
			}
			else if (bench_choice == 3)
			{
				int r1, c1, r2, c2, r, g, b;
				if (!get_rect(height, width, r1, c1, r2, c2))
					break;
				std::cout << "Enter color (R G B): ";
				std::cin >> r >> g >> b;

				VectorImage vi(width, height);
				SegmentTree st_bench(bench_image);

				auto start_vi = std::chrono::high_resolution_clock::now();
				vi.fill_region(
				    r1, c1, r2, c2,
				    {(unsigned char)r, (unsigned char)g, (unsigned char)b});
				auto end_vi = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> vi_duration = end_vi - start_vi;

				auto start_st = std::chrono::high_resolution_clock::now();
				st_bench.fill_region(
				    r1, c1, r2, c2,
				    {(unsigned char)r, (unsigned char)g, (unsigned char)b});
				auto end_st = std::chrono::high_resolution_clock::now();
				std::chrono::duration<double> st_duration = end_st - start_st;

				std::cout << "\n--- Benchmark Results ---" << std::endl;
				std::cout << "VectorImage (std::vector): "
				          << vi_duration.count() * 1e6 << " us" << std::endl;
				std::cout << "SegmentTree: " << st_duration.count() * 1e6
				          << " us" << std::endl;
			}
			break;
		}

		case 10: { // Benchmark (Many)
			int width, height, num_updates;
			std::cout
			    << "Enter image resolution for benchmark (width height): ";
			std::cin >> width >> height;
			std::cout << "Enter number of updates: ";
			std::cin >> num_updates;

			if (std::cin.fail() || width <= 0 || height <= 0 ||
			    num_updates <= 0)
			{
				std::cout << "\x1b[31mError: Invalid parameters.\x1b[0m\n";
				std::cin.clear();
				std::cin.ignore(std::numeric_limits<std::streamsize>::max(),
				                '\n');
				break;
			}

			Image bench_image(width, height);
			bench_image.generate_random();

			VectorImage vi(width, height);
			SegmentTree st_bench(bench_image);

			std::mt19937 gen(1337); // Fixed seed for reproducibility
			std::uniform_int_distribution<> op_dist(0, 2);
			std::uniform_int_distribution<> val_dist(-255, 255);
			std::uniform_real_distribution<> mul_dist(0.5, 1.5);
			std::uniform_int_distribution<> r_dist(0, height - 1);
			std::uniform_int_distribution<> c_dist(0, width - 1);

			std::vector<std::tuple<int, int, int, int, int, int, int, double>>
			    updates;
			for (int i = 0; i < num_updates; ++i)
			{
				int r1 = r_dist(gen);
				int r2 = r_dist(gen);
				int c1 = c_dist(gen);
				int c2 = c_dist(gen);
				if (r1 > r2)
					std::swap(r1, r2);
				if (c1 > c2)
					std::swap(c1, c2);
				updates.emplace_back(op_dist(gen), r1, c1, r2, c2,
				                     val_dist(gen), val_dist(gen),
				                     mul_dist(gen));
			}

			auto start_vi = std::chrono::high_resolution_clock::now();
			for (const auto &up : updates)
			{
				auto [op, r1, c1, r2, c2, val, color, mul] = up;
				if (op == 0)
					vi.adjust_brightness(r1, c1, r2, c2, val);
				else if (op == 1)
					vi.fill_region(r1, c1, r2, c2,
					               {(unsigned char)color, (unsigned char)color,
					                (unsigned char)color});
				else
					vi.adjust_contrast(r1, c1, r2, c2, mul);
			}
			auto end_vi = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> vi_duration = end_vi - start_vi;

			auto start_st = std::chrono::high_resolution_clock::now();
			for (const auto &up : updates)
			{
				auto [op, r1, c1, r2, c2, val, color, mul] = up;
				if (op == 0)
					st_bench.adjust_brightness(r1, c1, r2, c2, val);
				else if (op == 1)
					st_bench.fill_region(r1, c1, r2, c2,
					                     {(unsigned char)color,
					                      (unsigned char)color,
					                      (unsigned char)color});
				else
					st_bench.adjust_contrast(r1, c1, r2, c2, mul);
			}
			auto end_st = std::chrono::high_resolution_clock::now();
			std::chrono::duration<double> st_duration = end_st - start_st;

			std::cout << "\n--- Benchmark Results (" << num_updates
			          << " updates) ---" << std::endl;
			std::cout << "VectorImage (std::vector): "
			          << vi_duration.count() * 1e3 << " ms" << std::endl;
			std::cout << "SegmentTree: " << st_duration.count() * 1e3 << " ms"
			          << std::endl;
			break;
		}

		// Histogram
		case 11: {
			Image current_image = st.get_image();
			std::vector<std::vector<int>> hist = current_image.histogram();

			std::cout << "\n--- Image Histogram ---" << std::endl;
			std::cout << "Value\tR\tG\tB" << std::endl;
			std::cout << "-------------------------" << std::endl;
			for (int i = 0; i < 256; ++i)
			{
				if (hist[0][i] > 0 || hist[1][i] > 0 || hist[2][i] > 0)
				{
					std::cout << i << "\t" << hist[0][i] << "\t" << hist[1][i]
					          << "\t" << hist[2][i] << std::endl;
				}
			}
			break;
		}
		case 0:
			std::cout << "Exiting.\n";
			break;
		default:
			std::cout << "\x1b[31mInvalid choice. Please try again.\x1b[0m\n";
			break;
		}
	}
	while (choice != 0);

	return 0;
}
