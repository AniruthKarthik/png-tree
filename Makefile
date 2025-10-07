CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -pedantic -O2
LDFLAGS = 

BUILD_DIR = build
SRC_DIR = src

APP_NAME = image_app
EXECUTABLE = $(BUILD_DIR)/$(APP_NAME)

SRCS = $(SRC_DIR)/main.cpp $(SRC_DIR)/ImageProcessor.cpp $(SRC_DIR)/VectorImage.cpp $(SRC_DIR)/SegmentTree.cpp $(SRC_DIR)/Image.cpp
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))

.PHONY: all cli clean

all: cli # Make 'cli' the default target

cli: $(EXECUTABLE)
	@echo "Running CLI..."
	./$(EXECUTABLE)

$(EXECUTABLE): $(OBJS)
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(@D)
	$(CXX) $(CXXFLAGS) -c -o $@ $<

clean:
	@echo "Cleaning up..."
	rm -rf $(BUILD_DIR)
