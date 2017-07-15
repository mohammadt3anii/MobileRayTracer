//
// Created by Tiago on 14-11-2016.
//

#ifndef MOBILERAYTRACER_UTILS
#define MOBILERAYTRACER_UTILS

#include <cstring>
#include <sstream>
#include <string>

#ifndef NO_ANDROID

#include <android/log.h>

#endif

template <typename T>
void MagicLog(std::ostream& o, T t)
{
    o << t;
}

template<typename T, typename... Args>
void MagicLog(std::ostream& o, T t, Args... args)
{
    MagicLog(o, t);
    MagicLog(o, args...);
}

template<typename... Args>
void log(Args... args)
{
    std::ostringstream oss;
    MagicLog(oss, args...);
	#ifdef NO_ANDROID
		printf("%s\n", oss.str().c_str());
	#else
		__android_log_print(ANDROID_LOG_INFO, "LOG", "%s\n", oss.str().c_str());
	#endif
}

#define FILENAME (std::strrchr(__FILE__, '/') != nullptr?\
std::strrchr(__FILE__, '/') : __FILE__)

#define LOG(msg,...) \
{std::string filepath;\
getFileName(filepath, __FILE__);\
log(filepath,"::",__LINE__,"::",msg,__VA_ARGS__);}


inline void getFileName (std::string& str, const char *filepath) {
	std::stringstream ss;
	ss << filepath;
	std::string filePath(ss.str());
	std::string::size_type filePos = filePath.rfind('/');
	if (filePos != std::string::npos) {
		++filePos;
	}
	else {
		filePos = 0;
	}
	str = filePath.substr(filePos);
}


//#define LOG(__FILE__, __LINE__, ...)
//     __android_log_print(ANDROID_LOG_INFO, "LOG", "%s::line:%d: " msg "\n",\
//         FILENAMEL, __LINE__, __VA_ARGS__);

// void LOG2() {
// 	#ifdef NO_ANDROID
// 	printf("\n");
// 	#else
// 	__android_log_print(ANDROID_LOG_INFO, "LOG", "\n");
// 	#endif
// }

// template<typename ... T>
// void LOG2(const int& t1, const T& ... t) {
// 	#ifdef NO_ANDROID
// 	printf("%d", t1);
// 	#else
// 	__android_log_print(ANDROID_LOG_INFO, "LOG", "%d", t1);
// 	#endif
// 	LOG2(t...);
// }

// template<typename ... T>
// void LOG2(const unsigned int& t1, const T& ... t) {
// 	#ifdef NO_ANDROID
// 	printf("%u", t1);
// 	#else
// 	__android_log_print(ANDROID_LOG_INFO, "LOG", "%u", t1);
// 	#endif
// 	LOG2(t...);
// }

// template<typename ... T>
// void LOG2(const unsigned long& t1, const T& ... t) {
// 	#ifdef NO_ANDROID
// 	printf("%lu", t1);
// 	#else
// 	__android_log_print(ANDROID_LOG_INFO, "LOG", "%lu", t1);
// 	#endif
// 	LOG2(t...);
// }

// template<typename ... T>
// void LOG2(const float& t1, const T& ... t) {
// 	#ifdef NO_ANDROID
// 	printf("%f", t1);
// 	#else
// 	__android_log_print(ANDROID_LOG_INFO, "LOG", "%f", t1);
// 	#endif
// 	LOG2(t...);
// }

// template<typename T1, typename ... T>
// void LOG2(const T1& t1, const T& ... t) {
// 	#ifdef NO_ANDROID
// 	printf("%s", t1);
// 	#else
// 	__android_log_print(ANDROID_LOG_INFO, "LOG", "%s", t1);
// 	#endif
// 	LOG2(t...);
// }



// #define FILENAMEL (std::strrchr(__FILE__, '/') != nullptr?\
// std::strrchr(__FILE__, '/') : __FILE__)

// template<typename T1, typename ... T>
// void LOG3(const T1& t1, const T& ... t) {
// 	#ifdef NO_ANDROID
// 	printf("%s::line:%d:%s", FILENAMEL, __LINE__, t1);
// 	#else
// 	__android_log_print(ANDROID_LOG_INFO, "LOG", "%s", t1);
// 	#endif
// 	LOG2(t...);
// }

// #ifndef NO_ANDROID

// #include <android/log.h>

// #define FILENAMEL (std::strrchr(__FILE__, '/') != nullptr?\
// std::strrchr(__FILE__, '/') : __FILE__)

// #define LOG(msg, ...)\
//     __android_log_print(ANDROID_LOG_INFO, "LOG", "%s::line:%d: " msg "\n",\
//         FILENAMEL, __LINE__, __VA_ARGS__);

// #else

// #define FILENAMEW (std::strrchr(__FILE__, '\\') != nullptr?\
// std::strrchr(__FILE__, '\\') + 1 : __FILE__)

// #define LOG(msg, ...)\
//     printf("%s::line:%d: " msg "\n",\
//         FILENAMEW, __LINE__, __VA_ARGS__);

// /*#define LOG(msg, ...)\
// 	//template<typename... P>
//     printf("%s::line:%d: " msg "\n",\
//         FILENAMEL, __LINE__, __VA_ARGS__);*/

// #endif

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
