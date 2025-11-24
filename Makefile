# ============================
# ELO321-MASP Memory Simulator
# ============================

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude
OBJDIR = build
SRCDIR = src

TARGET = sim

# Source files
SRC = $(SRCDIR)/main.c \
      $(SRCDIR)/first-fit.c \
      $(SRCDIR)/best-fit.c \
      $(SRCDIR)/best-fit-tdas.c

# Object files
OBJ = $(SRC:$(SRCDIR)/%.c=$(OBJDIR)/%.o)

# Default rule
all: $(TARGET)

# Build target
$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) $(OBJ) -o $(TARGET)

# Compile each source file into build/
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Create build directory if missing
$(OBJDIR):
	mkdir -p $(OBJDIR)

# Cleanup
clean:
	rm -rf $(OBJDIR) $(TARGET)

# Rebuild everything
rebuild: clean all
