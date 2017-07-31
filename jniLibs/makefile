.SUFFIXES:
.PHONY: all clean tidy gtest
.PRECIOUS: %.s %.llvmIR
default: all

EXE := raytracer
CXX := clang++
LD := $(CXX)
MAKEFLAGS := -j5
INCLUDES := -I./MobileRT/src/main/cpp
INCLUDES += -I./Components/src/main/cpp
INCLUDES += -I../third-party
INCLUDES += $(shell pkg-config --cflags gtk+-2.0)

CXXFLAGS := -std=c++14 -Wall -Wpedantic -Wextra -MMD -pedantic -Werror -fno-rtti -fno-exceptions -DNO_ANDROID -fPIE $(INCLUDES)
CXXFLAGS += -Wno-old-style-cast -Wno-zero-as-null-pointer-constant -Wno-unused-variable -Wno-unused-function
CXXFLAGS +=  -Wno-padded -Wno-float-equal -flto=full -gsplit-dwarf -fno-math-errno -funsafe-math-optimizations -ffinite-math-only -fno-signed-zeros -fno-trapping-math -ffunction-sections -fdata-sections

INCLUDES += -L../third-party/tinyobjloader

LDFLAGS := $(shell pkg-config --libs gtk+-2.0) -pie -fopenmp -flto=full -gsplit-dwarf -fno-math-errno -funsafe-math-optimizations -ffinite-math-only -fno-signed-zeros -fno-trapping-math -fsingle-precision-constant -funswitch-loops -ffunction-sections -fdata-sections -fprofile-correction

ifeq ($(S),)
	CXXFLAGS += -march=native -ftree-vectorize -Ofast
	LDFLAGS += -Wl,-O3 -Wl,-discard-all -Wl,-no-undefined -Wl,-gc-sections -Wl,-z,norelro -Wl,-hash-style=sysv -Wl,-build-id=none -Wl,-s -fvisibility=hidden -fvisibility-inlines-hidden
endif

ifeq ($(S),1)
	CXXFLAGS += -O0 -g3 -fno-omit-frame-pointer -fno-optimize-sibling-calls -fno-stack-check
	LDFLAGS += -O0 -g3 -fno-omit-frame-pointer -fno-optimize-sibling-calls -fno-stack-check
endif

ifeq ($(S),2)
	#CXX := g++
	CXXFLAGS += -O0 -g3 -fno-omit-frame-pointer -fno-optimize-sibling-calls
	LDFLAGS += -O0 -g3 -fno-omit-frame-pointer -fno-optimize-sibling-calls
	SANITIZER := -fsanitize=memory -fsanitize-memory-track-origins=2 -fsanitize-memory-use-after-dtor #MemorySanitizer, a detector of uninitialized reads. Requires instrumentation of all program code.
	#SANITIZER := -fsanitize=address #AddressSanitizer, a memory error detector.
	#SANITIZER := -fsanitize=thread #ThreadSanitizer, a data race detector.
	#SANITIZER := -fsanitize=undefined #UndefinedBehaviorSanitizer, a fast and compatible undefined behavior checker.
	#SANITIZER := -fsanitize=dataflow #DataFlowSanitizer, a general data flow analysis.
	#SANITIZER := -fsanitize=cfi #control flow integrity checks. Requires -flto.
	#SANITIZER := -fsanitize=safe-stack #safe stack protection against stack-based memory corruption errors.
	SANITIZER += -fsanitize-recover=address
	#SANITIZER_CXX := -static-libasan
	SANITIZER_LD := -fsanitize-address-use-after-scope# -ltsan -lasan
	ASAN_OPTIONS=verbosity=1:strict_string_checks=1:detect_stack_use_after_return=1:check_initialization_order=1:strict_init_order=1:halt_on_error=0
	CXXFLAGS += $(SANITIZER) $(SANITIZER_CXX)
	LDFLAGS += $(SANITIZER) $(SANITIZER_LD)
endif

ifeq ($(CXX),clang++)
  CXXFLAGS += -Weverything -ferror-limit=1 -isystem /usr/include/c++/v1
	CXXFLAGS += -Wno-deprecated-dynamic-exception-spec -Wno-documentation -Wno-reserved-id-macro -Wno-documentation-unknown-command -Wno-shift-sign-overflow -Wno-c++98-compat-pedantic
  LDFLAGS += -lc++
endif

###############################################################################

SOURCES := $(shell find \
./Components/src \
./MobileRT/src \
../third-party/tinyobjloader/tiny_obj_loader.cc \
! -name "DrawView.cpp" \
-regextype gnu-awk -iname "*.cpp" -print | sort -nr)
OBJS := $(addsuffix .o,$(basename $(SOURCES)))
DEPS := $(addsuffix .d,$(basename $(SOURCES)))
ASSM := $(addsuffix .s,$(basename $(SOURCES)))
LLVM := $(addsuffix .llvmIR,$(basename $(SOURCES)))
GARB := $(shell find \
./Components/src \
./MobileRT/src \
./RayTracerMain.* \
./GoogleUnitTest \
-regextype gnu-awk -iname "*.d" -print \
-o -regextype gnu-awk -iname "*.s" -print \
-o -regextype gnu-awk -iname "*.o" -print \
-o -regextype gnu-awk -iname "*.out" -print \
-o -regextype gnu-awk -iname "*.gto" -print \
-regextype gnu-awk -iname "*fuse*" -print \
)

###############################################################################

%.o: %.s
	$(CXX) $(CXXFLAGS) -c $(basename $<).cpp -o $@
#	objdump --source $@ > $<

%.s: %.llvmIR
	$(CXX) $(CXXFLAGS) -S $(basename $<).cpp -o $@

%.llvmIR: %.cpp
	$(CXX) $(CXXFLAGS) -S -emit-llvm $< -o $@

$(EXE): $(OBJS) RayTracerMain.o third_party
	$(LD) $(OBJS) RayTracerMain.o -o $@ $(LDFLAGS) third-party/tiny_obj_loader.o
	@rm -f $(OBJS) $(DEPS) $(GARB) RayTracerMain.o RayTracerMain.d
	@echo "Compilation done"

clean:
	@rm -f $(OBJS) $(DEPS) $(GARB) $(ASSM) $(LLVM) $(EXE) gtest RayTracerMain.o RayTracerMain.d RayTracerMain.s RayTracerMain.llvmIR
	@echo "Clean done"

all: $(EXE)

third_party:
	$(CXX) $(CXXFLAGS) -c ../third-party/tinyobjloader/tiny_obj_loader.cc -o third-party/tiny_obj_loader.o

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

#valgrind --tool=memcheck --leak-check=full --show-leak-kinds=all --track-origins=yes --show-reachable=yes --suppressions=valgrind.suppr --num-callers=500 --trace-children=yes --error-limit=no ./raytracer 5 3 3 0 1 1

#valgrind --tool=cachegrind --suppressions=valgrind.suppr --num-callers=500 --trace-children=yes --error-limit=no ./raytracer 5 3 3 0 1 1

#valgrind --tool=callgrind --suppressions=valgrind.suppr --num-callers=500 --trace-children=yes --error-limit=no ./raytracer 5 3 3 0 1 1

#valgrind --tool=helgrind  --suppressions=valgrind.suppr --num-callers=500 --trace-children=yes --error-limit=no ./raytracer 5 3 3 0 1 1

#valgrind --tool=drd --suppressions=valgrind.suppr --num-callers=500 --trace-children=yes --error-limit=no ./raytracer 5 3 3 0 1 1

#valgrind --tool=massif --suppressions=valgrind.suppr --num-callers=500 --trace-children=yes --error-limit=no ./raytracer 5 3 3 0 1 1

#valgrind --tool=exp-dhat --suppressions=valgrind.suppr --num-callers=500 --trace-children=yes --error-limit=no ./raytracer 5 3 3 0 1 1

#valgrind --tool=exp-sgcheck --suppressions=valgrind.suppr --num-callers=500 --trace-children=yes --error-limit=no ./raytracer 5 3 3 0 1 1

#valgrind --tool=exp-bbv --suppressions=valgrind.suppr --num-callers=500 --trace-children=yes --error-limit=no ./raytracer 5 3 3 0 1 1

#valgrind --tool=lackey --suppressions=valgrind.suppr --num-callers=500 --trace-children=yes --error-limit=no ./raytracer 5 3 3 0 1 1

#llvm-profdata-5.0 merge -o testcov.profdata pgo/default_4412358617431096720_0.profraw

#llvm-cov-5.0 show ./testcov -instr-profile=testcov.profdata testcov.c