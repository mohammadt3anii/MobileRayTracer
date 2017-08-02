//
// Created by Tiago on 14-11-2016.
//

#ifndef MOBILERT_UTILS_HPP
#define MOBILERT_UTILS_HPP

#include <iostream>
#include <sstream>

#ifndef NO_ANDROID

#include <android/log.h>

#endif

#define LOG(...) {log(getFileName(__FILE__),":",__LINE__,": ",__VA_ARGS__);}

#ifndef LOG
    #define LOG(...)
#endif

template <typename T>
void MagicLog(std::ostringstream& o, T t)
{
    o << t;
}

template<typename T, typename... Args>
void MagicLog(std::ostringstream& o, T t, Args... args)
{
	o << t;
	MagicLog(o, args...);
}

template<typename... Args>
void log(Args... args)
{
    std::ostringstream oss("");
    MagicLog(oss, args...);
		oss << std::endl;
	#ifdef NO_ANDROID
		std::cout << oss.str();
	#else
		__android_log_print(ANDROID_LOG_INFO, "LOG", "%s", oss.str().c_str());
	#endif
}

inline std::string getFileName (const char *filepath) {
	std::string filePath(filepath);
	std::string::size_type filePos (filePath.rfind('/'));
	if (filePos != std::string::npos) {
		++filePos;
	}
	else {
		filePos = 0;
	}
	return std::string(filePath.substr(filePos));
}

#define EPSILON         1.0e-05f
#define RAY_LENGTH_MAX  1.0e+30f
#define RAY_DEPTH_MIN   4u
#define RAY_DEPTH_MAX   8u
#define PI              3.14159265358979323846f
#define PI_4            0.78539816339744830962f

int roundDownToMultipleOf(int value,
                                   int multiple) noexcept;

#endif //MOBILERT_UTILS_HPP
