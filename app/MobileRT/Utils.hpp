//
// Created by Tiago on 14-11-2016.
//

#ifndef MOBILERT_UTILS_HPP
#define MOBILERT_UTILS_HPP

#include <algorithm>
#include <chrono>
#include <cmath>
#include <glm/glm.hpp>
#include <iostream>
#include <sstream>
#include <thread>
#include <vector>

#ifdef ANDROID

#include <android/log.h>
#endif

namespace MobileRT {
#define LOG(...) {::MobileRT::log(::MobileRT::getFileName(__FILE__), ":", __LINE__, ": ", __VA_ARGS__);}

#ifndef LOG
#define LOG(...)
#endif

    template<typename ...Args>
    void log(Args &&... args) noexcept;

    inline ::std::string getFileName(const char *filepath) noexcept;

    template<typename T>
    ::std::vector<T *> convertVector(::std::vector<T> &source) noexcept;

    const float Epsilon{1.0e-04f};
    const float RayLengthMax{1.0e+30f};
    const float Pi{3.14159265358979323846f};
    const float Pi_4{0.78539816339744830962f};
    const int32_t RayDepthMin{4};
    const int32_t RayDepthMax{8};
    const int32_t NumberOfBlocks{256};

    int roundDownToMultipleOf(int value, int multiple) noexcept;

    float haltonSequence(uint32_t index, unsigned base) noexcept;

    unsigned incrementalAvg(::glm::vec3 sample, unsigned avg, unsigned numSample) noexcept;


    template<typename ...Args>
    void log(Args &&... args) noexcept {
        ::std::ostringstream oss{""};
        static_cast<void> (::std::initializer_list<int> {(oss << ::std::move(args), 0)...});
        oss << '\n';
#ifdef ANDROID
        __android_log_print(ANDROID_LOG_DEBUG, "LOG", "%s", oss.str().c_str());
        const ::std::chrono::duration<int, ::std::micro> timeToSleep {10};
        ::std::this_thread::sleep_for(timeToSleep);
#else
        ::std::cout << oss.str();
#endif
    }

    ::std::string getFileName(const char *const filepath) noexcept {
        const ::std::string filePath{filepath};
        ::std::string::size_type filePos{filePath.rfind('/')};
        if (filePos != ::std::string::npos) {
            ++filePos;
        } else {
            filePos = 0;
        }
        return ::std::string {filePath.substr(filePos)};
    }

    template<typename T>
    ::std::vector<T *> convertVector(::std::vector<T> &source) noexcept {
        ::std::vector<T *> target(source.size());
        ::std::transform(source.begin(), source.end(), target.begin(),
                         [](T &t) noexcept -> T * { return &t; });
        return target;
    }
}//namespace MobileRT

#endif //MOBILERT_UTILS_HPP
