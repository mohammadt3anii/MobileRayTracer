//
// Created by Tiago on 14-11-2016.
//

#include "Utils.hpp"

int roundDownToMultipleOf(const int value, const int multiple) noexcept {
    const int rest(value % multiple);
    return rest > 0 ? value - rest : value;
}
