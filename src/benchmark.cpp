#include "Image.h"
#include "SegmentTree.h"
#include "VectorImage.h"
#include <chrono>
#include <functional>
#include <iostream>
#include <random>
#include <string>
#include <vector>

// Helper to run and time a function
double time_operation(const std::function<void()> &f)
{
	auto start = std::chrono::high_resolution_clock::now();
	f();
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> ms = end - start;
	return ms.count();
}

// Main benchmark runner
void run_benchmark(const std::string &op_name, int width, int height,
                   int region_size, int iters)
{
	Image initial_image(width, height);
	initial_image.generate_random();

	std::mt19937 gen(1337); // Fixed seed for reproducibility
	std::uniform_int_distribution<> r_dist(0, height - region_size);
	std::uniform_int_distribution<> c_dist(0, width - region_size);

	std::vector<std::pair<int, int>> regions;
	for (int i = 0; i < iters; ++i)
	{
		regions.emplace_back(r_dist(gen), c_dist(gen));
	}

	VectorImage vi(width, height);
	SegmentTree st(initial_image);

	double time_vi = 0.0;
	double time_st = 0.0;

	if (op_name == "Fill Region")
	{
		auto op_vi = [&](int r, int c) {
			vi.fill_region(r, c, r + region_size - 1, c + region_size - 1,
			               {0, 255, 0});
		};
		auto op_st = [&](int r, int c) {
			st.fill_region(r, c, r + region_size - 1, c + region_size - 1,
			               {0, 255, 0});
		};
		time_vi = time_operation([&]() {
			for (const auto &reg : regions)
				op_vi(reg.first, reg.second);
		});
		time_st = time_operation([&]() {
			for (const auto &reg : regions)
				op_st(reg.first, reg.second);
		});
	}
	else if (op_name == "Adjust Brightness")
	{
		auto op_vi = [&](int r, int c) {
			vi.adjust_brightness(r, c, r + region_size - 1, c + region_size - 1,
			                     20);
		};
		auto op_st = [&](int r, int c) {
			st.adjust_brightness(r, c, r + region_size - 1, c + region_size - 1,
			                     20);
		};
		time_vi = time_operation([&]() {
			for (const auto &reg : regions)
				op_vi(reg.first, reg.second);
		});
		time_st = time_operation([&]() {
			for (const auto &reg : regions)
				op_st(reg.first, reg.second);
		});
	}

	// Print as CSV for easy parsing
	std::cout << op_name << ","
	          << std::to_string(region_size) + "x" + std::to_string(region_size)
	          << "," << iters << "," << time_vi << "," << time_st << std::endl;
}

int main()
{
	const int IMAGE_SIZE = 4096;
	const std::vector<std::string> OPERATIONS = {"Fill Region",
	                                             "Adjust Brightness"};
	const std::vector<int> REGION_SIZES = {4096, 1080, 64};
	const std::vector<int> ITERATIONS = {1, 1000, 100000};

	// CSV Header
	std::cout << "Operation,RegionSize,Iterations,VectorTime,TreeTime"
	          << std::endl;

	for (const auto &op : OPERATIONS)
	{
		for (int region : REGION_SIZES)
		{
			for (int iter : ITERATIONS)
			{
				// Don't run huge iterations on huge regions
				if (region > 1000 && iter > 1000)
					continue;
				if (region > 64 && iter > 10000)
					continue;

				run_benchmark(op, IMAGE_SIZE, IMAGE_SIZE, region, iter);
			}
		}
	}

	return 0;
}
