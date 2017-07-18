//
// Created by Tiago on 14-11-2016.
//

#include "Utils.hpp"

unsigned int roundDownToMultipleOf(const unsigned int value, const unsigned int multiple) noexcept {
    const unsigned int rest(value % multiple);
    return rest > 0 ? value - rest : value;
}
