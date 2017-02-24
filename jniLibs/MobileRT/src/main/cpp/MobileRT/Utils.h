//
// Created by Tiago on 14-11-2016.
//

#ifndef MOBILERAYTRACER_UTILS
#define MOBILERAYTRACER_UTILS

#include <cmath>
#include <android/log.h>

#define FILENAME (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#define LOG(msg, ...)\
__android_log_print(ANDROID_LOG_DEBUG, "LOG", "file:%s::line:%d: " msg,\
FILENAME, __LINE__, __VA_ARGS__);

#define RAY_LENGTH_MIN  1.0e-5f
#define RAY_LENGTH_MAX  1.0e+10f
#define PI              3.14159265358979323846f
#define PI_4            0.78539816339744830962f
#define VECT_PROJ_MIN   1.0e-8f // kEpsilon

float fastArcTan(const float value);

unsigned int roundToPower2(unsigned int value);

unsigned int roundToEvenNumber(const unsigned int value);

unsigned int roundToMultipleOf(unsigned int numToRound, unsigned int multipleOf);

#endif //MOBILERAYTRACER_UTILS
