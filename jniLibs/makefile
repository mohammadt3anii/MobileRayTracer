.SUFFIXES:
.PHONY: all clean tidy
.PRECIOUS: %.s
default: all

EXE := raytracer
CXX := clang++-5.0
LD := $(CXX)
MAKEFLAGS := -j5
INCLUDES := -I./MobileRT/src/main/cpp
INCLUDES += -I./Components/src/main/cpp
INCLUDES += $(shell pkg-config --cflags gtk+-2.0)

CXXFLAGS := -std=c++14 -Wpedantic -Wall -Wextra -MMD -pedantic -isystem /usr/include/c++/v1 ${SANITIZER} -Werror -ferror-limit=1
#SANITIZER := -fsanitize=memory -fsanitize-memory-track-origins=2 -fsanitize-memory-use-after-dtor
CXXFLAGS += -Weverything -Wno-c++98-compat -Wno-c++98-compat-pedantic -Wno-reserved-id-macro -Wno-documentation -Wno-padded -Wno-deprecated-dynamic-exception-spec -Wno-documentation-unknown-command -Wno-old-style-cast -Wno-zero-as-null-pointer-constant -Wno-weak-vtables -Wno-shift-sign-overflow -Wno-unused-variable
CXXFLAGS += -march=native -ftree-vectorize -Ofast
#CXXFLAGS += -O0 -g3 -fno-omit-frame-pointer
CXXFLAGS += -DNO_ANDROID $(INCLUDES)

LDFLAGS := $(shell pkg-config --libs gtk+-2.0) -fopenmp -lc++
LDFLAGS += $(SANITIZER)
LDFLAGS += -Wl,-O3 -Wl,-discard-all -Wl,-no-undefined -Wl,-gc-sections -Wl,-z,norelro -Wl,-hash-style=sysv -Wl,-build-id=none -Wl,-s

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
	-checks=\
	'*,-*google-runtime-references*,-*llvm-header-guard*' \
	-analyze-temporary-dtors \
	-header-filter=*.*pp* \
	-system-headers \
	-p \
	*.*pp* \
	./MobileRT/src/main/cpp/*/*.*pp* \
	./MobileRT/src/main/cpp/*/*/*.*pp* \
	./Components/src/main/cpp/*/*/*.*pp* \
	../app/src/main/cpp/*.*p* \
	-- -std=c++14 -isystem /usr/include/c++/v1 \
	-I./MobileRT/src/main/cpp \
	-I./Components/src/main/cpp \
	-pthread -I/usr/include/gtk-2.0 -I/usr/lib/x86_64-linux-gnu/gtk-2.0/include -I/usr/include/gio-unix-2.0/ -I/usr/include/cairo -I/usr/include/pango-1.0 -I/usr/include/atk-1.0 -I/usr/include/cairo -I/usr/include/pixman-1 -I/usr/include/libpng12 -I/usr/include/gdk-pixbuf-2.0 -I/usr/include/libpng12 -I/usr/include/pango-1.0 -I/usr/include/harfbuzz -I/usr/include/pango-1.0 -I/usr/include/glib-2.0 -I/usr/lib/x86_64-linux-gnu/glib-2.0/include -I/usr/include/freetype2 \
	-I/mnt/D/Linux/Android_SDK/ndk-bundle/sysroot/usr/include \
	2>&1 | tee tidy.out