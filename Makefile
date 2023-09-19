CC := g++
AR := ar
TARGET := lib/netlib.a
RM := rm

SRCDIR := src
BUILDDIR := build
SOURCES := $(notdir $(shell find $(SRCDIR) -name '*.cpp'))
OBJECTS := $(SOURCES:%.cpp=$(BUILDDIR)/%.o)
CFLAGS := -std=c++17 -Wall -Wextra

$(TARGET): $(OBJECTS)
	mkdir -p $(dir $@)
	@echo "Linking static library..."
	$(AR) rcs $(TARGET) $(OBJECTS)
	@echo "Library can be now found in lib/directory"

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp
	mkdir -p $(BUILDDIR)
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	@echo "Cleaning..."
	$(RM) -r $(BUILDDIR) $(TARGET)
