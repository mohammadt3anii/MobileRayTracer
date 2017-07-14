//
// Created by Tiago on 14-11-2016.
//

#ifndef MOBILERAYTRACER_UTILS
#define MOBILERAYTRACER_UTILS

#include <cstdio>
#include <cstring>

#define FILENAMEL (std::strrchr(__FILE__, '/') != nullptr?\
std::strrchr(__FILE__, '/') + 1 : __FILE__)

#define FILENAMEW (std::strrchr(__FILE__, '\\') != nullptr?\
std::strrchr(__FILE__, '\\') + 1 : __FILE__)

#ifndef NO_ANDROID

#include <android/log.h>

#define LOG(msg, ...)\
    __android_log_print(ANDROID_LOG_INFO, "LOG", "%s::line:%d: " msg "\n",\
        FILENAMEW, __LINE__, __VA_ARGS__);

#else

#define LOG(msg, ...)\
    printf("%s::line:%d: " msg "\n",\
        FILENAMEL, __LINE__, __VA_ARGS__);

/*#define LOG(msg, ...)\
	//template<typename... P>
    printf("%s::line:%d: " msg "\n",\
        FILENAMEL, __LINE__, __VA_ARGS__);*/

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

#define RAY_LENGTH_MIN  1.0e-3f
#define RAY_LENGTH_MAX  1.0e+5f
#define RAY_DEPTH_MIN   2u
#define RAY_DEPTH_MAX   4u
#define PI              3.14159265358979323846f
#define PI_4            0.78539816339744830962f
#define VECT_PROJ_MIN   1.0e-3f // kEpsilon

float fastArcTan(float value) noexcept;

unsigned int roundDownToMultipleOf(unsigned int value,
                                   unsigned int multiple) noexcept;

#endif //MOBILERAYTRACER_UTILS
