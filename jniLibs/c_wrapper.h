#ifndef C_WRAPPER_HPP
#define C_WRAPPER_HPP

#ifndef __cplusplus
  #include <stdbool.h>
#endif

void work_thread (unsigned* bitmap, int width, int height, int /*stride*/, int threads, int shader, int scene, int samplesPixel, int samplesLight, int repeats, int accelerator, bool printStdOut);

#ifdef __cplusplus
  extern "C"
#endif
void RayTrace (unsigned* bitmap, int width, int height, int stride, int threads, int shader, int scene, int samplesPixel, int samplesLight, int repeats, int accelerator, bool printStdOut, bool async);

#endif // C_WRAPPER_HPP
