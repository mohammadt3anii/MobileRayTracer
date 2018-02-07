//
// Created by Tiago on 14-11-2016.
//

#include "Utils.hpp"

int ::MobileRT::roundDownToMultipleOf(const int value, const int multiple) noexcept {
  const int rest {value % multiple};
  return rest > 1 ? value - rest : value;
}

//https://en.wikipedia.org/wiki/Halton_sequence
float ::MobileRT::haltonSequence (uint32_t index, const unsigned base) noexcept {
  float f {1.0f};
  float result {0.0f};
  while (index > 0) {
    f /= base;
    result += f * (index % base);
    index = static_cast<uint32_t> (::std::floor (index / base));
  }
  return result;
}

void ::MobileRT::escape (void *pointer) noexcept {
  //inline assembly ("asm code : outputs : inputs : clobbers")
  //outputs -> what comes out of inline assembly back to C++ code
  //inputs -> what comes in from C++ code to assembly
  //clobbers -> what parts of program are modified while this runs
  //clobers -> eg: it changed all memory
  //volatile -> this assembly code has a noable side effect that the compiler isn't aware of
  //volatile -> turn off compiler
  //volatile -> useful eg: emit 8 bytes of instructions
  asm volatile ("" : : "g" (pointer) : "memory");
}

void ::MobileRT::clobber () noexcept {
  asm volatile ("" : : : "memory");
}
