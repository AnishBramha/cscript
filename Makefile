CXX = clang++

CXXFLAGS = -std=c++23 -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -fsanitize=address,undefined -g -MMD -MP -O3 -ffast-math -march=native -mtune=native -flto

LDFLAGS = -fsanitize=address,undefined -g -flto

TARGET = cscpt.out
BUILD_DIR = build

SRCS = $(shell find . -name "*.cpp")
OBJS = $(patsubst ./%.cpp,$(BUILD_DIR)/%.o,$(SRCS))
DEPS = $(patsubst ./%.cpp,$(BUILD_DIR)/%.d,$(SRCS))

define COLOR_CODES
RED   := \033[31m
GREEN := \033[32m
YELLOW := \033[33m
MAGENTA := \033[35m
CYAN  := \033[36m
RESET    := \033[0m
BOLD  := \033[1m
endef

$(eval $(COLOR_CODES))

.PHONY: clean all clear

all: $(TARGET)

$(TARGET): $(OBJS)
	@echo "\n\n$(BOLD)$(GREEN)Compilation finished...$(RESET)"
	@echo "\n\n$(BOLD)$(CYAN)Linking...$(RESET)\n"
	$(CXX) $(LDFLAGS) -o $@ $^
	@echo "\n\n$(BOLD)$(GREEN)Linking finished...$(RESET)"
	@echo "\n\n$(BOLD)$(CYAN)Executable dumped:$(RESET) $(BOLD)$(YELLOW)$(TARGET)$(RESET)\n\n"

$(BUILD_DIR)/%.o: ./%.cpp
	@echo "\n\n$(BOLD)$(CYAN)Compiling $<...$(RESET)\n\n"
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












