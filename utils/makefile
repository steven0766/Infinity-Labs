# Configuration
SRC_DIR := src
INCLUDE_DIR := include
BIN_DIR := bin

CC := gcc
CFLAGS := -ansi -Wall -Wextra -pedantic-errors -fPIC
CPPFLAGS := -I$(INCLUDE_DIR)

# Build mode selection (defult is debug)
ifeq ($(filter release, $(MAKECMDGOALS)), release)
  OBJDIR := $(BIN_DIR)/release
  CFLAGS += -O3
  CPPFLAGS += -DNDEBUG
else
  OBJDIR := $(BIN_DIR)/debug
  CFLAGS += -g
endif

LIB := $(OBJDIR)/libutils.so

# File discovery
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(SRC_FILES:$(SRC_DIR)/%.c=$(OBJDIR)/%.o)
DEPS := $(OBJS:.o=.d)

vpath %.c $(SRC_DIR)

# Targets
.PHONY: all debug release clean

# Defualt target
all:
	@$(MAKE) debug
	@$(MAKE) release

# Build modes
debug: $(LIB)

release: $(LIB)

# Shared library
$(LIB): $(OBJS) | $(OBJDIR)
	$(CC) -shared -o $@ $^

# Object Files
$(OBJDIR)/%.o: %.c | $(OBJDIR)
	$(CC) $(CPPFLAGS) $(CFLAGS) -MMD -MT $@ -MF $(@:.o=.d) -c $< -o $@

# Directory Creation
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Clean - Remove content, not directories
clean:
	rm -f $(BIN_DIR)/debug/*.o $(BIN_DIR)/debug/*.d $(BIN_DIR)/debug/*.so
	rm -f $(BIN_DIR)/release/*.o $(BIN_DIR)/release/*.d $(BIN_DIR)/release/*.so

# Include dependency files if they exist
-include $(DEPS)

