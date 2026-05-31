CXX      = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Iinclude
SRCDIR   = src
OBJDIR   = build
TARGET   = railway_system

SOURCES  = $(wildcard $(SRCDIR)/*.cpp)
OBJECTS  = $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

all: $(OBJDIR) $(TARGET)

$(OBJDIR):
	mkdir -p $(OBJDIR)

$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^
	@echo ""
	@echo "  ✓ Build successful! Run with: ./$(TARGET)"
	@echo ""

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJDIR) $(TARGET)
	@echo "  ✓ Cleaned build files."

run: all
	./$(TARGET)

.PHONY: all clean run
