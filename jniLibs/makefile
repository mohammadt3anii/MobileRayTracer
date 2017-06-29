.SUFFIXES:
.PRECIOUS: %.o %.s
.PHONY: all

EXE := raytracer
CXX := c++
LD := $(CXX)
MAKEFLAGS := -j5

CXXFLAGS := -Wpedantic -Wall -Wextra -Winline -Wfatal-errors -MMD -MP -std=c++11 -g3
#CXXFLAGS += -march=native -ftree-vectorize -Ofast
#CXXFLAGS += -masm=intel -fverbose-asm
CXXFLAGS += -DNO_ANDROID
CXXFLAGS += -I./MobileRT/src/main/cpp
CXXFLAGS += -I./Components/src/main/cpp
CXXFLAGS += $(shell pkg-config --cflags gtk+-2.0)

LDFLAGS := $(shell pkg-config --libs gtk+-2.0) -pthread -fopenmp

################################################################################

SOURCES := $(shell find . ! -name "DrawView.cpp" -regextype gnu-awk -iname "*.cpp" -print | sort -nr)
OBJS := $(addsuffix .o,$(basename $(SOURCES)))
DEPS := $(addsuffix .d,$(basename $(SOURCES)))
ASM := $(addsuffix .s,$(basename $(SOURCES)))
GARBAGE := $(shell find . -regextype gnu-awk -iname "*.fuse*" -print)

%.o: %.s
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.s: %.cpp
	$(CXX) $(CXXFLAGS) -S -Wa,-ahl=$@ $< -o $@

$(EXE): $(OBJS)
	$(LD) $(OBJS) -o $@ $(LDFLAGS)
	@rm -f $(DEPS) $(GARBAGE)
	@echo "Compilation done"

clean:
	@rm -f $(OBJS) $(DEPS) $(ASM) $(EXE) $(GARBAGE)
	@echo "Clean done"

all: $(EXE)
