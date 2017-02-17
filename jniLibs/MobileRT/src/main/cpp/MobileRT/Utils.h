//
// Created by Tiago on 14-11-2016.
//

#ifndef MOBILERAYTRACER_UTILS
#define MOBILERAYTRACER_UTILS

#include <cmath>
#include <android/log.h>

#define LOG(msg, ...)\
__android_log_print(ANDROID_LOG_DEBUG, "LOG_TAG", "line:%d: " msg, __LINE__, __VA_ARGS__);

#define RAY_LENGTH_MIN  1.0e-5f
#define RAY_LENGTH_MAX  1.0e+10f
#define PI              3.14159265358979323846f
#define PI_4            0.78539816339744830962f
#define VECT_PROJ_MIN   1.0e-8f // kEpsilon

float fastArcTan(const float value);

float haltonSequence(const unsigned int index, const unsigned int base);
unsigned int roundToPower2(unsigned int v);

unsigned int roundToEvenNumber(unsigned int number);

#endif //MOBILERAYTRACER_UTILS
