CXX = clang++

CXXFLAGS = -std=c++23 -Wall -Wextra -Wpedantic -Wconversion -Wsign-conversion -fsanitize=address,undefined -g -MMD -MP -O3 -ffast-math -march=native -mtune=native -flto

LDFLAGS = -fsanitize=address,undefined -g -flto

TARGET = cscpt.outBUILD_DIR = build

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
	@echo "\n$(BOLD)$(GREEN)Compilation finished ✓$(RESET)"
	@echo "\n$(BOLD)$(CYAN)Linking ⧗$(RESET)"
	@if ! $(CXX) $(LDFLAGS) -o $@ $^ > /dev/null ; then \
		echo "\n$(BOLD)$(RED)Linking failed ✘$(RESET)\n"; \
		exit 1;\
	fi

	@echo "$(BOLD)$(GREEN)Linking finished ✓$(RESET)"
	@echo "\n$(BOLD)$(CYAN)Executable dumped:$(RESET) $(BOLD)$(YELLOW)$(TARGET) ✓$(RESET)\n\n"

$(BUILD_DIR)/%.o: ./%.cpp
	@echo "\n$(BOLD)$(CYAN)Compiling $< ⧗$(RESET)"
	@
	@mkdir -p $(dir $@)

	@if ! $(CXX) $(CXXFLAGS) -c $< -o $@ > /dev/null; then \
		echo "\n$(BOLD)$(RED)Compilation failed ✘$(RESET)\n\n"; \
		exit 1;\
	fi


clean:
	@echo "Cleaning build directory..."
	rm -rf $(BUILD_DIR)

clear:
	@echo "Clearing project directory..."
	rm -rf $(BUILD_DIR) $(TARGET)

-include $(DEPS)
