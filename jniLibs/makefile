.SUFFIXES:
.PHONY: all clean tidy gtest
.PRECIOUS: %.s
default: all

EXE := raytracer
CXX := clang++
LD := $(CXX)
MAKEFLAGS := -j5
INCLUDES := -I./MobileRT/src/main/cpp
INCLUDES += -I./Components/src/main/cpp
INCLUDES += $(shell pkg-config --cflags gtk+-2.0)

CXXFLAGS := -std=c++14 -Wall -Wpedantic -Wextra -MMD -pedantic -Werror -Wno-c++98-compat-pedantic -fno-rtti -fno-exceptions -DNO_ANDROID $(INCLUDES)
CXXFLAGS += -Wno-shift-sign-overflow -Wno-old-style-cast -Wno-zero-as-null-pointer-constant -Wno-documentation-unknown-command -Wno-deprecated-dynamic-exception-spec -Wno-documentation -Wno-reserved-id-macro
CXXFLAGS +=  -Wno-padded

LDFLAGS := $(shell pkg-config --libs gtk+-2.0) -fopenmp

ifeq ($(S),)
	CXXFLAGS += -march=native -ftree-vectorize -Ofast
	LDFLAGS += -Wl,-O3 -Wl,-discard-all -Wl,-no-undefined -Wl,-gc-sections -Wl,-z,norelro -Wl,-hash-style=sysv -Wl,-build-id=none -Wl,-s
endif

ifeq ($(S),1)
	CXXFLAGS += -O0 -g3 -fno-omit-frame-pointer -fno-optimize-sibling-calls
	LDFLAGS += -O0 -g3 -fno-omit-frame-pointer -fno-optimize-sibling-calls
endif

ifeq ($(S),2)
	CXXFLAGS += -O0 -g3 -fno-omit-frame-pointer -fno-optimize-sibling-calls
	LDFLAGS += -O0 -g3 -fno-omit-frame-pointer -fno-optimize-sibling-calls
	SANITIZER := -fsanitize=memory -fsanitize-memory-track-origins=2 -fsanitize-memory-use-after-dtor
	#SANITIZER := -fsanitize=address
	SANITIZER += -fsanitize-recover=address
	SANITIZER_LD := -fsanitize-address-use-after-scope
	ASAN_OPTIONS := strict_string_checks=1:detect_stack_use_after_return=1:check_initialization_order=1:strict_init_order=1:halt_on_error=0
endif

ifeq ($(CXX),clang++)
  CXXFLAGS += -Weverything -ferror-limit=1 -isystem /usr/include/c++/v1 $(SANITIZER)
  LDFLAGS += -lc++ $(SANITIZER) $(SANITIZER_LD)
endif

###############################################################################

SOURCES := $(shell find \
./Components/src \
./MobileRT/src \
! -name "DrawView.cpp" -regextype gnu-awk -iname "*.cpp" -print | sort -nr)
OBJS := $(addsuffix .o,$(basename $(SOURCES)))
DEPS := $(addsuffix .d,$(basename $(SOURCES)))
ASSM := $(addsuffix .s,$(basename $(SOURCES)))
GARB := $(shell find \
./Components/src \
./MobileRT/src \
./RayTracerMain.* \
./GoogleUnitTest \
-regextype gnu-awk -iname "*fuse*" -print \
-o -regextype gnu-awk -iname "*.d" -print \
-o -regextype gnu-awk -iname "*.s" -print \
-o -regextype gnu-awk -iname "*.o" -print \
-o -regextype gnu-awk -iname "*.out" -print \
-o -regextype gnu-awk -iname "*.gto" -print)

###############################################################################

%.o: %.s
	$(CXX) $(CXXFLAG) -c $< -o $@
	objdump --source $@ > $<

%.s: %.cpp
	$(CXX) $(CXXFLAGS) -S $< -o $@

$(EXE): $(OBJS) RayTracerMain.o
	$(LD) $(OBJS) RayTracerMain.o -o $@ $(LDFLAGS)
	@rm -f $(OBJS) $(DEPS) $(GARB) RayTracerMain.o RayTracerMain.s RayTracerMain.d
	@echo "Compilation done"

clean:
	@rm -f $(OBJS) $(DEPS) $(GARB) $(ASSM) $(EXE) gtest RayTracerMain.o RayTracerMain.d RayTracerMain.s
	@echo "Clean done"

all: $(EXE)

INCLUDES += -I/mnt/D/Linux/Android_SDK/ndk-bundle/sysroot/usr/include
tidy:
	clang-tidy-5.0 \
	-analyze-temporary-dtors \
	-checks='*,-*llvm-header-guard*' \
	-header-filter=*.*pp \
	-system-headers \
	-p \
	./*.*pp \
	./MobileRT/src/main/cpp/*/*.*pp \
	./MobileRT/src/main/cpp/*/*/*.*pp \
	./Components/src/main/cpp/*/*/*.*pp \
	../app/src/main/cpp/*.*pp \
	-- -std=c++14 -isystem /usr/include/c++/v1 \
	-I./MobileRT/src/main/cpp \
	-I./Components/src/main/cpp \
	-pthread -I/usr/include/gtk-2.0 -I/usr/lib/x86_64-linux-gnu/gtk-2.0/include -I/usr/include/gio-unix-2.0/ -I/usr/include/cairo -I/usr/include/pango-1.0 -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/pixman-1 -I/usr/include/libpng12 -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/libpng12 -I/usr/include/pango-1.0 -I/usr/include/harfbuzz -I/usr/include/pango-1.0 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/freetype2 \
	-I/mnt/D/Linux/Android_SDK/ndk-bundle/sysroot/usr/include \
	2>&1 | tee tidy.out

GTESTSRCS := $(shell find ./GoogleUnitTest -regextype gnu-awk -iname "*.cpp" -print)
GTESTOBJS := $(addsuffix .gto,$(basename $(GTESTSRCS)))
%.gto: %.cpp
	g++ -std=c++14 -c $< -o $@

gtest: $(OBJS) $(GTESTOBJS)
	g++ $(GTESTOBJS) $(OBJS) -o $@ $(LDFLAGS) -lgtest -lgtest_main
	@echo "Compilation Test Units done"