# CONFIGURATION - Directories
SRC_DIR := src
TEST_DIR := test
INCLUDE_DIR := include
BIN_DIR := bin
DEBUG_DIR := $(BIN_DIR)/debug
RELEASE_DIR := $(BIN_DIR)/release
UTILS_SRC := ../utils/src
UTILS_INCLUDE := ../utils/include

# Shared library paths
LIB_DEBUG := $(DEBUG_DIR)/libds_debug.so
LIB_RELEASE := $(RELEASE_DIR)/libds_release.so

# CONFIGURATION - Compiler and Flags
CC := gcc
CFLAGS := -ansi -Wall -Wextra -pedantic-errors
CPPFLAGS := -I$(INCLUDE_DIR) -I$(UTILS_INCLUDE)
LDFLAGS := -shared -fPIC
LDLIBS := -lm

# FILE DISCOVERY
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
TEST_FILES := $(wildcard $(TEST_DIR)/*_test.c)
TEST_BASENAMES := $(basename $(notdir $(TEST_FILES)))

# DERIVED VARIABLES
MODULES := $(patsubst %_test,%,$(TEST_BASENAMES))
DEBUG_OBJS := $(SRC_FILES:$(SRC_DIR)/%.c=$(DEBUG_DIR)/%.o)
RELEASE_OBJS := $(SRC_FILES:$(SRC_DIR)/%.c=$(RELEASE_DIR)/%.o)
TEST_EXECUTABLES := $(TEST_BASENAMES:%=$(DEBUG_DIR)/%_debug)
# Dependency files
DEBUG_DEPS := $(DEBUG_OBJS:.o=.d)
RELEASE_DEPS := $(RELEASE_OBJS:.o=.d)
TEST_DEPS := $(TEST_EXECUTABLES:%=%.d)
# Test utils object
TEST_UTILS_DEBUG := $(DEBUG_DIR)/test_utils.o
TEST_UTILS_DEBUG_DEP := $(TEST_UTILS_DEBUG:.o=.d)

# TARGET-SPECIFIC VARIABLES
debug: CFLAGS += -g
debug: CPPFLAGS += -DDEBUG
release: CFLAGS += -O3
release: CPPFLAGS += -DNDEBUG
test: CFLAGS += -g
test: CPPFLAGS += -DDEBUG

# TARGETS 
.PHONY: all debug release test clean clean-all clean-objs $(MODULES)
# Default target
all: release debug
# Build debug shared object
debug: $(LIB_DEBUG)
# Build release shared object
release: $(LIB_RELEASE)
# Build all test executables
test: debug $(TEST_UTILS_DEBUG) $(TEST_EXECUTABLES)
# Individual module targets
$(MODULES): %: $(DEBUG_DIR)/%_test_debug
# Run individual test 
run-%: $(DEBUG_DIR)/%_test_debug
	@$<
# Run valgrind on individual test
val-%: $(DEBUG_DIR)/%_test_debug
	@valgrind --leak-check=yes --track-origins=yes $<
# BUILD RULES 
# Shared Libraries
$(LIB_DEBUG): $(DEBUG_OBJS) | $(DEBUG_DIR)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)
$(LIB_RELEASE): $(RELEASE_OBJS) | $(RELEASE_DIR)
	$(CC) $(LDFLAGS) -o $@ $^ $(LDLIBS)
# Object Files
$(DEBUG_DIR)/%.o: $(SRC_DIR)/%.c | $(DEBUG_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -fPIC -MMD -MT $@ -MF $(@:.o=.d) -c $< -o $@
$(RELEASE_DIR)/%.o: $(SRC_DIR)/%.c | $(RELEASE_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -fPIC -MMD -MT $@ -MF $(@:.o=.d) -c $< -o $@
$(TEST_UTILS_DEBUG): $(UTILS_SRC)/test_utils.c | $(DEBUG_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MT $@ -MF $(@:.o=.d) -c $< -o $@
# Test Executables
$(DEBUG_DIR)/%_test_debug: $(TEST_DIR)/%_test.c $(LIB_DEBUG) \
                           $(TEST_UTILS_DEBUG) | $(DEBUG_DIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MT $@ -MF $@.d -o \
	$@ $< $(TEST_UTILS_DEBUG) -L$(DEBUG_DIR) -lds_debug \
	-Wl,-rpath,$(DEBUG_DIR) $(LDLIBS)
# Directory Creation
$(DEBUG_DIR):
	mkdir -p $(DEBUG_DIR)
$(RELEASE_DIR):
	mkdir -p $(RELEASE_DIR)
clean:
	rm -f $(DEBUG_DIR)/*.o $(DEBUG_DIR)/*.d $(DEBUG_DIR)/*_test_debug
	rm -f $(RELEASE_DIR)/*.o $(RELEASE_DIR)/*.d
	rm -f $(LIB_DEBUG) $(LIB_RELEASE)

# Include dependency files if they exist
-include $(DEBUG_DEPS)
-include $(RELEASE_DEPS)
-include $(TEST_DEPS)
-include $(TEST_UTILS_DEBUG_DEP)
