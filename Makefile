# Thanks to Job Vranish (https://spin.atomicobject.com/2016/08/26/makefile-c-projects/)
CWD := $(shell pwd)
PROJECT_NAME := $(shell basename $(CWD))
TARGET_EXEC := $(PROJECT_NAME)

DEBUG := 0

BUILD_DIR := ./build
SRC_DIRS := ./src

# Set compiler
CC := $(shell which gcc)
CXX := $(shell which g++)

# Find all the C and C++ files we want to compile
# Note the single quotes around the * expressions. The shell will incorrectly expand these otherwise, but we want to send the * directly to the find command.
SRCS := $(shell find $(SRC_DIRS) -name '*.cpp' -or -name '*.c' -or -name '*.s')

# Prepends BUILD_DIR and appends .o to every src file
# As an example, ./your_dir/hello.cpp turns into ./build/./your_dir/hello.cpp.o
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

# String substitution (suffix version without %).
# As an example, ./build/hello.cpp.o turns into ./build/hello.cpp.d
DEPS := $(OBJS:.o=.d)

# Every folder in ./src will need to be passed to GCC so that it can find header files
INC_DIRS := $(shell find $(SRC_DIRS) -type d)
# Add a prefix to INC_DIRS. So moduleA would become -ImoduleA. GCC understands this -I flag
INC_FLAGS := $(addprefix -I,$(INC_DIRS))

# The -MMD and -MP flags together generate Makefiles for us!
# These files will have .d instead of .o as the output.
CPPFLAGS := $(INC_FLAGS) -MMD -MP -Wall -Wextra

ifeq ($(DEBUG), 1)
	CPPFLAGS += -g
endif

# Enable compile_commands.json when not clean
ifneq ($(MAKECMDGOALS),clean)
	ifeq (,$(shell which bear))
		BEAR_FOUND := $(warning Warning: bear is not installed, therefore compile_commands.json will not be generated.)
	else
		export BUILD_CDB := "1"
	endif
endif

# The final build step.
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $@ $(LDFLAGS)

# Build step for C source
$(BUILD_DIR)/%.c.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
ifeq ($(BUILD_CDB), "1")
	@bear --append -- $(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@
endif

# Build step for C++ source
$(BUILD_DIR)/%.cpp.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@
ifeq ($(BUILD_CDB), "1")
	@bear --append -- $(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@
endif


.PHONY: clean
clean:
	-@rm -r $(BUILD_DIR)
	-@rm compile_commands.json

# Include the .d makefiles. The - at the front suppresses the errors of missing
# Makefiles. Initially, all the .d files will be missing, and we don't want those
# errors to show up.
-include $(DEPS)
