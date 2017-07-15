.SUFFIXES:
.PHONY: all clean tidy
.PRECIOUS: %.s
default: all

EXE := raytracer
CXX :=  clang++-5.0
LD := $(CXX)
MAKEFLAGS := -j5
INCLUDES := -I./MobileRT/src/main/cpp
INCLUDES += -I./Components/src/main/cpp
INCLUDES += $(shell pkg-config --cflags gtk+-2.0)

CXXFLAGS := -std=c++14 -Wpedantic -Wall -Wextra -MMD -pedantic -isystem /usr/include/c++/v1 ${SANITIZER} -Werror -ferror-limit=1
#SANITIZER := -fsanitize=memory -fsanitize-memory-track-origins=2 -fsanitize-memory-use-after-dtor
#CXXFLAGS += -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-reserved-id-macro -Wno-documentation -Wno-padded -Wno-deprecated-dynamic-exception-spec -Wno-documentation-unknown-command -Wno-old-style-cast -Wno-zero-as-null-pointer-constant
#CXXFLAGS += -march=native -ftree-vectorize -Ofast
CXXFLAGS += -O0 -g3 -fno-omit-frame-pointer
CXXFLAGS += -DNO_ANDROID $(INCLUDES)

LDFLAGS := $(shell pkg-config --libs gtk+-2.0) -fopenmp -lc++
LDFLAGS += $(SANITIZER)

###############################################################################

SOURCES := $(shell find . ! -name "DrawView.cpp" -regextype gnu-awk -iname "*.cpp" -print | sort -nr)
OBJS := $(addsuffix .o,$(basename $(SOURCES)))
DEPS := $(addsuffix .d,$(basename $(SOURCES)))
ASSM := $(addsuffix .s,$(basename $(SOURCES)))
GARB := $(shell find . -regextype gnu-awk -iname "*.fuse*" -print)

%.o: %.s
	$(CXX) $(CXXFLAG) -c $< -o $@
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

INCLUDES += -I/mnt/D/Linux/Android_SDK/ndk-bundle/sysroot/usr/include
tidy:
	clang-tidy-5.0 \
	-checks='*,-*google-runtime-references*,-*llvm-header-guard*' \
	-analyze-temporary-dtors \
	-header-filter=*.*p \
	-system-headers \
	-p \
	*.*p \
	./MobileRT/src/main/cpp/*/*.*p \
	./MobileRT/src/main/cpp/*/*/*.*p \
	./Components/src/main/cpp/*/*/*.*p \
	../app/src/main/cpp/*.*p \
	-- -std=c++14 $(INCLUDES) -isystem /usr/include/c++/v1 > tidy.out