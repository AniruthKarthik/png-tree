CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -O2
LDFLAGS = 

BUILD_DIR = build
SRC_DIR = src

APP_NAME = image_app
EXECUTABLE = $(BUILD_DIR)/$(APP_NAME)

SRCS = $(SRC_DIR)/main.cpp $(SRC_DIR)/ImageProcessor.cpp $(SRC_DIR)/VectorImage.cpp $(SRC_DIR)/SegmentTree.cpp $(SRC_DIR)/Image.cpp
BENCH_SRC = $(SRC_DIR)/benchmark.cpp
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
BENCH_OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(BENCH_SRC))
IMG_OBJS = $(BUILD_DIR)/Image.o $(BUILD_DIR)/ImageProcessor.o $(BUILD_DIR)/VectorImage.o $(BUILD_DIR)/SegmentTree.o

.PHONY: all cli clean benchmark

all: cli # Make 'cli' the default target

cli: $(EXECUTABLE)
	@echo "Running CLI..."
	./$(EXECUTABLE)

benchmark: $(BUILD_DIR)/benchmark
	@echo "Running Benchmark..."
	./$(BUILD_DIR)/benchmark

$(EXECUTABLE): $(OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/benchmark: $(BENCH_OBJS) $(IMG_OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	@echo "Cleaning up..."
	rm -rf $(BUILD_DIR)
