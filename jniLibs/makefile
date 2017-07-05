.SUFFIXES:
.PHONY: all clean
.PRECIOUS: %.s
default: all

EXE := raytracer
CXX := c++
LD := $(CXX)
MAKEFLAGS := -j5

CXXFLAGS := -Wpedantic -Wall -Wextra -Werror -Wfatal-errors -MMD -std=c++14 -g
CXXFLAGS += -march=native -ftree-vectorize -Ofast
CXXFLAGS += -DNO_ANDROID
CXXFLAGS += -I./MobileRT/src/main/cpp
CXXFLAGS += -I./Components/src/main/cpp
CXXFLAGS += $(shell pkg-config --cflags gtk+-2.0)

LDFLAGS := $(shell pkg-config --libs gtk+-2.0) -fopenmp

###############################################################################

SOURCES := $(shell find . ! -name "DrawView.cpp" -regextype gnu-awk -iname "*.cpp" -print | sort -nr)
OBJS := $(addsuffix .o,$(basename $(SOURCES)))
DEPS := $(addsuffix .d,$(basename $(SOURCES)))
ASSM := $(addsuffix .s,$(basename $(SOURCES)))
GARB := $(shell find . -regextype gnu-awk -iname "*.fuse*" -print)

%.o: %.s
	$(CXX) $(CXXFLAGS) -c $< -o $@
	objdump --source $@ > $<

%.s: %.cpp
	$(CXX) $(CXXFLAGS) -S $< -o $@

$(EXE): $(OBJS)
	$(LD) $(OBJS) -o $@ $(LDFLAGS)
	@rm -f $(OBJS) $(DEPS) $(GARB)
	@echo "Compilation done"

clean:
	@rm -f $(OBJS) $(DEPS) $(GARB) $(ASSM) $(EXE)
	@echo "Clean done"

all: $(EXE)
