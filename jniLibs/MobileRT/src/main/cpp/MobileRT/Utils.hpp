//
// Created by Tiago on 14-11-2016.
//

#ifndef MOBILERT_UTILS_HPP
#define MOBILERT_UTILS_HPP

#include <cmath>
#include <iostream>
#include <sstream>

#ifndef NO_ANDROID

  #include <android/log.h>
#endif

#define LOG(...) {log(getFileName(__FILE__), ":", __LINE__, ": ", __VA_ARGS__);}

#ifndef LOG
  #define LOG(...)
#endif

template<typename ...Args>
void log (Args &&... args) {
  std::ostringstream oss {""};
  static_cast<void> (std::initializer_list<int> {(oss << std::move (args), 0)...});
  oss << std::endl;
#ifdef NO_ANDROID
  std::cout << oss.str();
#else
  __android_log_print (ANDROID_LOG_INFO, "LOG", "%s", oss . str () . c_str ());
#endif
}

inline std::string getFileName (const char *const filepath) {

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
const int NumberOfBlocks {64};

int roundDownToMultipleOf(int value, int multiple) noexcept;
float haltonSequence (uint32_t index, unsigned base) noexcept;

#endif //MOBILERT_UTILS_HPP
