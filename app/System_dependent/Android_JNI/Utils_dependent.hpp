//
// Created by Tiago on 20-09-2018.
//

#ifndef UTILS_DEPENDENT_HPP
#define UTILS_DEPENDENT_HPP

#include <android/log.h>
#include <string>

namespace Dependent {
    inline void printString(const ::std::string &log) noexcept;
}//namespace Dependent

void Dependent::printString(const ::std::string &log) noexcept {
    __android_log_print(ANDROID_LOG_DEBUG, "LOG", "%s", log.c_str());
}

#endif //UTILS_DEPENDENT_HPP
