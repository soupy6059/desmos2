# Simple Makefile for C++ projects
# Project structure:
# project/
# ├── src/        (source files)
# ├── inc/        (header files)
# ├── build/      (object files, created automatically)
# └── Makefile

CXX = g++
CXXFLAGS = -std=c++20 -Wall -Wextra -g -O3 -Wno-missing-field-initializers
TARGET = main

RAYLIB_PATH=/nix/store/y8v2pmxvc181ihb3aj67plyg22m7b6w5-raylib-5.0#$(shell nix-build '<nixpkgs>' -A raylib --no-out-link)
GL_PATH=/nix/store/diadxlh8mdkdh1p3r2z224jfqgynbwx3-libglvnd-1.7.0#$(shell nix-build '<nixpkgs>' -A libGL --no-out-link)
X11_PATH=/nix/store/3890sar0ihl1gyx5mn7fd79q57vcmisi-libX11-1.8.10#$(shell nix-build '<nixpkgs>' -A xorg.libX11 --no-out-link)

# Compiler and flags
INCLUDES = -Iinc -I$(RAYLIB_PATH)/include

LDFLAGS = -L$(RAYLIB_PATH)/lib -L$(GL_PATH)/lib -L$(X11_PATH)/lib 
LDLIBS = -lm -lpthread -ldl -lrt -lraylib -lGL -lX11

# Directories
SRCDIR = src
INCDIR = inc
BUILDDIR = build

# Find all .cc files in src directory
SOURCES = $(wildcard $(SRCDIR)/*.cc)
OBJECTS = $(SOURCES:$(SRCDIR)/%.cc=$(BUILDDIR)/%.o)


# Default target
all: $(TARGET)

# Build the final executable
$(TARGET): $(OBJECTS)
	$(CXX) $(OBJECTS) ${LDFLAGS} ${LDLIBS} -o $(TARGET)
	@echo "Build complete: $(TARGET)"

# Compile source files to object files
$(BUILDDIR)/%.o: $(SRCDIR)/%.cc
	@mkdir -p $(BUILDDIR)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Clean build files
clean:
	rm -rf $(BUILDDIR) $(TARGET)
	@echo "Clean complete"

# Rebuild everything
rebuild: clean all

# Run the program
run: $(TARGET)
	./$(TARGET)

# Debug build
debug: CXXFLAGS += -DDEBUG -g3 -O0
debug: $(TARGET)

zip:
	rm cc3k.zip
	zip -r cc3k.zip *

# Declare phony targets
.PHONY: all clean rebuild install run debug release help
