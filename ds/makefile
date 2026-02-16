MAKEFLAGS += -r
.SUFFIXES:

# Configuration
SRC_DIR := src
TEST_DIR := test
INCLUDE_DIR := include
BIN_DIR := bin
UTILS_DIR := ../utils
UTILS_INCLUDE := $(UTILS_DIR)/include

CC := gcc
CFLAGS := -ansi -Wall -Wextra -pedantic-errors
CPPFLAGS := -I$(INCLUDE_DIR) -I$(UTILS_INCLUDE)
LDLIBS := -lm

# Build mode selection (defaults to debug)
ifeq ($(filter release, $(MAKECMDGOALS)), release)
  OBJDIR := $(BIN_DIR)/release
  CFLAGS += -O3
  CPPFLAGS += -DNDEBUG
else
  OBJDIR := $(BIN_DIR)/debug
  CFLAGS += -g
endif

LIB := $(OBJDIR)/libds.so
UTILS_LIB := $(UTILS_DIR)/$(OBJDIR)/libutils.so

# File discovery
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
# remove old pqueue implemenation
SRC_FILES := $(filter-out src/pqueue.c, $(SRC_FILES))
TEST_FILES := $(wildcard $(TEST_DIR)/*_test.c)
TEST_BASENAMES := $(basename $(notdir $(TEST_FILES)))

MODULES := $(patsubst %_test,%,$(TEST_BASENAMES))
OBJS := $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJDIR)/%.o)
TEST_EXECUTABLES := $(TEST_BASENAMES:%=$(OBJDIR)/%)

# Dependency files
DEPS := $(OBJS:.o=.d)
TEST_DEPS := $(TEST_EXECUTABLES:%=%.d)

vpath %.c $(SRC_DIR)
vpath %_test.c $(TEST_DIR)

# Targets
.PHONY: all debug release test clean run-all $(MODULES)

# Default target
all:
	@$(MAKE) debug
	@$(MAKE) release

# Build modes
debug: $(LIB)

release: $(LIB)

# Build all test executables (defaults to debug)
test: $(LIB) $(UTILS_LIB) $(TEST_EXECUTABLES)

# Individual module targets
$(MODULES): %: $(OBJDIR)/%_test

run-%: $(OBJDIR)/%_test
	@$<
	
vlg-%: $(OBJDIR)/%_test
	@valgrind --leak-check=yes --track-origins=yes $<

gdb-%: $(OBJDIR)/%_test
	@cgdb $<

# Build rules
$(UTILS_LIB):
	@$(MAKE) -C $(UTILS_DIR) $(filter debug release, $(MAKECMDGOALS))

# Shared library
$(LIB): $(OBJS) | $(OBJDIR)
	$(CC) -shared -fPIC -o $@ $^ $(LDLIBS)

# Object Files
$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -fPIC -MMD -MT $@ -MF $(@:.o=.d) -c $< -o $@

# Test executables
$(OBJDIR)/%_test: %_test.c $(LIB) $(UTILS_LIB) | $(OBJDIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MT $@ -MF $@.d -o $@ $< \
	-L$(OBJDIR) -L$(dir $(UTILS_LIB)) -lds -lutils \
	-Wl,-rpath,'$$ORIGIN:$(abspath $(dir $(UTILS_LIB)))' \
	$(LDLIBS)

$(OBJDIR):
	mkdir -p $(OBJDIR)
clean:
	rm -f $(BIN_DIR)/debug/*.o $(BIN_DIR)/debug/*.d \
	$(BIN_DIR)/debug/*_test $(BIN_DIR)/debug/*.so
	rm -f $(BIN_DIR)/release/*.o $(BIN_DIR)/release/*.d \
	$(BIN_DIR)/release/*_test $(BIN_DIR)/release/*.so
	@$(MAKE) -C $(UTILS_DIR) clean

-include $(DEPS)
-include $(TEST_DEPS)

