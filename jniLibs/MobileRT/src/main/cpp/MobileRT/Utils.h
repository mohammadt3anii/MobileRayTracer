//
// Created by Tiago on 14-11-2016.
//

#ifndef MOBILERAYTRACER_UTILS
#define MOBILERAYTRACER_UTILS

#include <cmath>
#include <cstring>
#include <iostream>
#include <iomanip>

#define FILENAME (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)

#ifndef NO_ANDROID

#include <android/log.h>

#define LOG(msg, ...)\
    __android_log_print(ANDROID_LOG_INFO, "LOG", "%s::line:%d: " msg "\n",\
        FILENAME, __LINE__, __VA_ARGS__);

#else

#define LOG(msg, ...)\
	printf("%s::line:%d: " msg "\n",\
		FILENAME, __LINE__, __VA_ARGS__);

#endif

/*#define ASSERT(left, operator, right)\
{\
    if(!(left operator right))\
    {\
        std::cerr << FILENAME << "::line:" << __LINE__ << ": " << "ASSERT FAILED: "\
        << std::fixed << std::setprecision(7) << #left << #operator << #right << " @ "\
        << #left << "=" << left << "; " << #right << "=" << right << std::endl;\
        raise(SIGTRAP);\
    }\
}*/

#define RAY_LENGTH_MIN  1.0e-5f
#define RAY_LENGTH_MAX  1.0e+10f
#define RAY_DEPTH_MIN   2u
#define RAY_DEPTH_MAX   4u
#define PI              3.14159265358979323846f
#define PI_4            0.78539816339744830962f
#define VECT_PROJ_MIN   1.0e-8f // kEpsilon

float fastArcTan(const float value);

unsigned int roundDownToMultipleOf(const unsigned int value,
								   const unsigned int multiple);

#endif //MOBILERAYTRACER_UTILS
