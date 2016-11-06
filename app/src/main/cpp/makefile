.SUFFIXES:
.PHONY: all

EXE := raytracer
CC := c++
LD := $(CC)
MAKEFLAGS := -j5

CCFLAGS := -MMD -MP -Wall -Wextra -std=c++11
CCFLAGS += -Ofast -march=native -ftree-vectorize #-fopenmp
CCFLAGS += -I./



#LDFLAGS := -fopenmp
CCFLAGS += $(shell pkg-config --cflags gtk+-2.0)
LDFLAGS += $(shell pkg-config --libs gtk+-2.0)

#CCACHE_EXISTS := $(shell ccache -V)
#ifdef CCACHE_EXISTS
#	CC := ccache $(CC)
#	LD := ccache $(LD)
#endif

################################################################################

SOURCES := $(shell find . ! -name "DrawViewImpl.cpp" -regextype gnu-awk -iname "*.cpp" -print)
#SOURCES := $(filter-out DrawViewImpl.cpp, $(SOURCES))
OBJS := $(addsuffix .o,$(basename $(SOURCES)))
#OBJS := $(addprefix ./objs/,$(OBJS))
DEPS := $(addsuffix .d,$(basename $(SOURCES)))


%.o: %.cpp
	$(CC) $(CCFLAGS) -c $< -o $@

$(EXE): $(OBJS)
	$(LD) $(OBJS) -o $@ $(LDFLAGS)

clean:
	@rm -f $(OBJS) $(DEPS) $(EXE)

all: $(EXE)
