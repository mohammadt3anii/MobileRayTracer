.SUFFIXES:
.PHONY: all

EXE := raytracer
CXX := c++
LD := $(CXX)
MAKEFLAGS := -j5

CXXFLAGS := -Wpedantic -Wall -Wextra -Winline -Wfatal-errors -MMD -MP -std=c++11
CXXFLAGS += -march=native -ftree-vectorize -O2
CXXFLAGS += -g3
CXXFLAGS += -DNO_ANDROID
CXXFLAGS += -I./Components/src/main/cpp
CXXFLAGS += -I./MobileRT/src/main/cpp
CXXFLAGS += $(shell pkg-config --cflags gtk+-2.0)

LDFLAGS := $(shell pkg-config --libs gtk+-2.0) -pthread -fopenmp

################################################################################

SOURCES := $(shell find . ! -name "DrawView.cpp" -regextype gnu-awk -iname "*.cpp" -print)
OBJS := $(addsuffix .o,$(basename $(SOURCES)))
DEPS := $(addsuffix .d,$(basename $(SOURCES)))

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(EXE): $(OBJS)
	$(LD) $(OBJS) -o $@ $(LDFLAGS)
	@rm -f $(OBJS) $(DEPS)
	@echo "Compilation done"

clean:
	@rm -f $(OBJS) $(DEPS) $(EXE)
	@echo "Clean done"

all: $(EXE)
