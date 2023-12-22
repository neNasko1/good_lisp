# This Makefile provides a basic structure for an extensible build system

# Compiler options
CC := gcc-13
CFLAGS := -Wall -Wextra -Werror

LINK_FLAGS := -L/opt/homebrew/Cellar/libgccjit/13.2.0/lib
INCL_FLAGS := -I/opt/homebrew/Cellar/libgccjit/13.2.0/include/

# Directories
SRCDIR := src
INCDIR := include
OBJDIR := obj
BINDIR := bin

# Source files
SRCS := $(wildcard $(SRCDIR)/*.c)
OBJS := $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SRCS))

# JSON generation tool
BEAR := bear

# Default target
all: $(BINDIR)/good_lisp

# Rule to build the executable
$(BINDIR)/good_lisp: $(OBJS)
	@mkdir -p $(BINDIR)
	$(CC) $(CFLAGS) $(LINK_FLAGS) $^ -o $@

# Rule to build object files
$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(OBJDIR)
	$(BEAR) -- $(CC) $(CFLAGS) -I$(INCDIR) $(INCL_FLAGS) -c $< -o $@

# Clean target
clean:
	@rm -rf $(OBJDIR) $(BINDIR)

# PHONY targets (targets that don't correspond to files)
.PHONY: all clean
