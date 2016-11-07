.SUFFIXES:
.PHONY: all

EXE := raytracer
CC := c++
LD := $(CC)
MAKEFLAGS := -j5

CCFLAGS := -MMD -MP -Wall -Wextra -std=c++11
CCFLAGS += -Ofast -march=native -ftree-vectorize -fopenmp
#CCFLAGS += -g3
CCFLAGS += -I./
CCFLAGS += $(shell pkg-config --cflags gtk+-2.0)

LDFLAGS := $(shell pkg-config --libs gtk+-2.0)
LDFLAGS += -fopenmp

################################################################################

SOURCES := $(shell find . ! -name "DrawViewImpl.cpp" -regextype gnu-awk -iname "*.cpp" -print)
OBJS := $(addsuffix .o,$(basename $(SOURCES)))
DEPS := $(addsuffix .d,$(basename $(SOURCES)))

%.o: %.cpp
	$(CC) $(CCFLAGS) -c $< -o $@

$(EXE): $(OBJS)
	$(LD) $(OBJS) -o $@ $(LDFLAGS)

clean:
	@rm -f $(OBJS) $(DEPS) $(EXE)

all: $(EXE)