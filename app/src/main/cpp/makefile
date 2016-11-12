.SUFFIXES:
.PHONY: all

EXE := raytracer
CXX := c++
LD := $(CXX)
MAKEFLAGS := -j5

CXXFLAGS := -MMD -MP -Wall -Wextra -std=c++11 -Wfatal-errors -m64 -fopenmp
CXXFLAGS += -Ofast -march=native -ftree-vectorize
CXXFLAGS += -g3
CXXFLAGS += -I./
CXXFLAGS += $(shell pkg-config --cflags gtk+-2.0)

LDFLAGS := $(shell pkg-config --libs gtk+-2.0)
LDFLAGS += -fopenmp

################################################################################

SOURCES := $(shell find . ! -name "DrawView.cpp" -regextype gnu-awk -iname "*.cpp" -print)
OBJS := $(addsuffix .o,$(basename $(SOURCES)))
DEPS := $(addsuffix .d,$(basename $(SOURCES)))

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(EXE): $(OBJS)
	$(LD) $(OBJS) -o $@ $(LDFLAGS)

clean:
	@rm -f $(OBJS) $(DEPS) $(EXE)
	@echo "Clean done"

all: $(EXE)
	@rm -f $(OBJS) $(DEPS)
	@echo "Compilation done"