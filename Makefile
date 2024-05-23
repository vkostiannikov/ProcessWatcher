# Directories
SRC_DIR = ./src
INC_DIR = ./include
BUILD_DIR = build
# Compiler settings
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -pthread -I$(INC_DIR)
#-fsanitize=address


# Files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(patsubst $(SRC_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
TARGET = processWatcher

# Targets
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) $^ -o $@

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.cpp | $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR):
	mkdir -p $@

install:
	cp $(TARGET) /usr/local/bin

clean:
	rm -rf $(BUILD_DIR) $(TARGET)
