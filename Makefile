# ─── Compiler & Flags ────────────────────────────────────────────────────────
CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2 -fopenmp

# ─── Directories ─────────────────────────────────────────────────────────────
SRC_DIR    := src
OBJ_DIR    := build/obj
BIN_DIR    := build

RAYLIB_DIR := external/raylib/src

# ─── Sources & Objects ───────────────────────────────────────────────────────
SRCS := $(shell find $(SRC_DIR) -name '*.cpp')
OBJS := $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRCS))

# ─── Output Binary ───────────────────────────────────────────────────────────
TARGET := $(BIN_DIR)/worldgen

# ─── Include Paths ───────────────────────────────────────────────────────────
INCLUDES := -I$(SRC_DIR) \
            -I$(RAYLIB_DIR)

# ─── Raylib Static Lib ───────────────────────────────────────────────────────
RAYLIB_LIB := $(RAYLIB_DIR)/libraylib.a

# ─── Platform Linker Flags ───────────────────────────────────────────────────
UNAME := $(shell uname)

ifeq ($(UNAME), Linux)
    LDFLAGS := -L$(RAYLIB_DIR) -lraylib -lGL -lm -lpthread -ldl -lrt -lX11 -fopenmp
endif
ifeq ($(UNAME), Darwin)
    LDFLAGS := -L$(RAYLIB_DIR) -lraylib -framework OpenGL -framework Cocoa \
               -framework IOKit -framework CoreAudio -framework CoreVideo -fopenmp
endif
# Windows (under MinGW):
ifeq ($(OS), Windows_NT)
    LDFLAGS := -L$(RAYLIB_DIR) -lraylib -lopengl32 -lgdi32 -lwinmm -fopenmp
endif

# ─── Dependency Tracking ─────────────────────────────────────────────────────
# Auto-generates .d files so changed headers trigger recompilation
DEPS := $(OBJS:.o=.d)
CXXFLAGS += -MMD -MP

# ─── Targets ─────────────────────────────────────────────────────────────────
.PHONY: all clean raylib

all: raylib $(TARGET)

# Build raylib static library if it doesn't exist yet
raylib: $(RAYLIB_LIB)

$(RAYLIB_LIB):
	$(MAKE) -C $(RAYLIB_DIR) PLATFORM=PLATFORM_DESKTOP

# Link final binary
$(TARGET): $(OBJS) | $(BIN_DIR)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# Compile each .cpp → .o, preserving subdirectory structure
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

# Create output dirs if missing
$(BIN_DIR) $(OBJ_DIR):
	mkdir -p $@

# Pull in generated dependency files
-include $(DEPS)

clean:
	rm -rf build

# Wipe raylib build too (rarely needed)
clean-all: clean
	$(MAKE) -C $(RAYLIB_DIR) clean