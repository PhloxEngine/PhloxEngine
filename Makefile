# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -I./dependencies -I./dependencies/discord-rpc/include
LDFLAGS = -L./dependencies/discord-rpc/builds/linux-dynamic/lib -Wl,-rpath,'$$ORIGIN' -lGL -lGLU -lglut -ldiscord-rpc

# Directories
SRC_DIR = source/Engine
OBJ_DIR = obj
BIN_DIR = bin

# Source files
SRCS = $(wildcard $(SRC_DIR)/*.cpp)
OBJS = $(SRCS:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

# Output executable
TARGET = $(BIN_DIR)/PhloxEngine

# Default target
all: directories discord-rpc $(TARGET)

# Create necessary directories
directories:
	@mkdir -p $(OBJ_DIR)
	@mkdir -p $(BIN_DIR)

# Link the executable
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) -o $(TARGET) $(LDFLAGS)

# Compile source files
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Install dependencies
deps:
	@echo "Installing dependencies..."
	sudo pacman -S --needed base-devel cmake freeglut

# Clean build files
clean:
	rm -rf $(OBJ_DIR) $(BIN_DIR)

# Build Discord RPC
discord-rpc:
	@echo "Building Discord RPC..."
	cd dependencies/discord-rpc && \
	mkdir -p builds && \
	cd builds && \
	cmake .. -DBUILD_SHARED_LIBS=ON -DCLANG_FORMAT_SUFFIX="-dummy" && \
	cmake --build . --config Release && \
	mkdir -p linux-dynamic/lib && \
	cp src/libdiscord-rpc.so linux-dynamic/lib/ && \
	cp src/libdiscord-rpc.so ../../../bin/

run: all
	@export LD_LIBRARY_PATH=./bin:$$LD_LIBRARY_PATH && \
	./$(TARGET)

.PHONY: all directories clean deps discord-rpc run