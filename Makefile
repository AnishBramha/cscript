CXX = clang++

CXXFLAGS = -std=c++23 -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -fsanitize=address,undefined -g -MMD -MP

LDFLAGS = -fsanitize=address,undefined -g

TARGET = cscpt.out
BUILD_DIR = build

SRCS = $(shell find . -name "*.cpp")
OBJS = $(patsubst ./%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
DEPS = $(patsubst ./%.cpp,$(BUILD_DIR)/%.d,$(SRCS))

.PHONY: clean all clear

all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "Linking..."
	$(CXX) $(LDFLAGS) -o $@ $^
	@echo "Executable dumped: $(TARGET)"

$(BUILD_DIR)/%.o: ./%.cpp
	@echo "Compiling $<..."
	@
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@echo "Cleaning build directory..."
	rm -rf $(BUILD_DIR)

clear:
	@echo "Clearing project directory..."
	rm -rf $(BUILD_DIR) $(TARGET)

-include $(DEPS)












