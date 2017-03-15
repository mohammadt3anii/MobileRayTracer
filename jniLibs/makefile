.SUFFIXES:
.PHONY: all

EXE := raytracer
CXX := c++
LD := $(CXX)
MAKEFLAGS := -j5

CXXFLAGS := -Wpedantic -Wall -Wextra -Winline -Wfatal-errors -MMD -MP -std=c++11 -ggdb3
CXXFLAGS += -march=native -ftree-vectorize
CXXFLAGS += -DNO_ANDROID
CXXFLAGS += -I./MobileRT/src/main/cpp
CXXFLAGS += -I./Components/src/main/cpp
CXXFLAGS += $(shell pkg-config --cflags gtk+-2.0)

LDFLAGS := $(shell pkg-config --libs gtk+-2.0) -pthread -fopenmp

################################################################################

SOURCES := $(shell find . ! -name "DrawView.cpp" -regextype gnu-awk -iname "*.cpp" -print | sort -nr)
OBJS := $(addsuffix .o,$(basename $(SOURCES)))
DEPS := $(addsuffix .d,$(basename $(SOURCES)))
GARBAGE := $(shell find . -regextype gnu-awk -iname "*.fuse*" -print)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(EXE): $(OBJS)
	$(LD) $(OBJS) -o $@ $(LDFLAGS)
	@rm -f $(OBJS) $(DEPS) $(GARBAGE)
	@echo "Compilation done"

clean:
	@rm -f $(OBJS) $(DEPS) $(EXE) $(GARBAGE)
	@echo "Clean done"

all: $(EXE)
