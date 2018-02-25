#ifndef C_WRAPPER_HPP
#define C_WRAPPER_HPP

#include <cstdint>

#ifndef __cplusplus
#include <stdbool.h>
#endif

#ifdef __cplusplus
extern "C"
#endif
void RayTrace(uint32_t *bitmap, int32_t width, int32_t height, int32_t threads, int32_t shader, int32_t scene,
              int32_t samplesPixel, int32_t samplesLight, int32_t repeats, int32_t accelerator, bool printStdOut,
              bool async, const char *pathObj, const char *pathMtl);

#endif // C_WRAPPER_HPP
