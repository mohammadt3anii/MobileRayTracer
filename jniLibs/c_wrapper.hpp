#ifndef C_WRAPPER_HPP
#define C_WRAPPER_HPP

extern "C" void RayTrace (unsigned* bitmap, int width, int height, int stride, int threads, int shader, int scene, int samplesPixel, int samplesLight, int repeats, int accelerator, bool printStdOut);

#endif // C_WRAPPER_HPP
