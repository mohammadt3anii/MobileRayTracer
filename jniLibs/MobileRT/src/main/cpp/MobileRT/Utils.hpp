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

#define LOG(...) {log(getFileName(__FILE__), ":", __LINE__, ": ", __VA_ARGS__);}

#ifndef LOG
  #define LOG(...)
#endif

inline void MagicLog (std::ostringstream & /*o*/) {}

template<typename T, typename... Args>
void MagicLog (std::ostringstream &o, T &&t, Args &&... args)
{

	o << t;
  MagicLog (o, std::forward<Args> (args)...);
}

template<typename... Args>
void log (Args &&... args)
{

  std::ostringstream oss {""};
  MagicLog<Args ...> (oss, std::forward<Args> (args)...);
  oss << std::endl;
	#ifdef NO_ANDROID
		std::cout << oss.str();
	#else
		__android_log_print(ANDROID_LOG_INFO, "LOG", "%s", oss.str().c_str());
	#endif
}

inline std::string getFileName (const char *filepath) {

  const std::string filePath {filepath};
  std::string::size_type filePos {filePath . rfind ('/')};
	if (filePos != std::string::npos) {
		++filePos;
	} else {
		filePos = 0;
	}
  return std::string {filePath . substr (filePos)};
}

const float Epsilon {1.0e-05f};
const float RayLengthMax {1.0e+30f};
const float Pi {3.14159265358979323846f};
const float Pi_4 {0.78539816339744830962f};
const int RayDepthMin {4};
const int RayDepthMax {8};

int roundDownToMultipleOf(int value, int multiple) noexcept;

#endif //MOBILERT_UTILS_HPP
