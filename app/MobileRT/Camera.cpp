//
// Created by Tiago on 23-Jan-17.
//

#include "MobileRT/Camera.hpp"
#include <array>

using ::MobileRT::AABB;
using ::MobileRT::Camera;
using ::MobileRT::NumberOfBlocks;

namespace {
    ::std::array<float, NumberOfBlocks> VALUES;

    bool FillThings() {
        static ::std::mt19937 generator(::std::random_device{}());
        for (::std::uint32_t i{0}; i < NumberOfBlocks; ++i) {
            VALUES.at(i) = ::MobileRT::haltonSequence(i, 2);
        }
        ::std::shuffle(VALUES.begin(), VALUES.end(), generator);
        return true;
    }
}//namespace

float Camera::getBlock(const ::std::uint32_t sample) noexcept {
    const ::std::uint32_t current{
            this->block_.fetch_add(1, ::std::memory_order_relaxed) - NumberOfBlocks * sample};
    if (current >= NumberOfBlocks) {
        this->block_.fetch_sub(1, ::std::memory_order_relaxed);
        return 1.0f;
    }
    return VALUES.at(current);
}

AABB Camera::getAABB() const noexcept {
    const ::glm::vec3 min{position_};
    const ::glm::vec3 max{position_};
    return AABB {min, max};
}

//Left hand rule
Camera::Camera(const ::glm::vec3 position, const ::glm::vec3 lookAt, const ::glm::vec3 up) noexcept :
        position_{position},
        direction_{::glm::normalize(lookAt - position)},
        right_{::glm::cross(up, direction_)},
        up_{::glm::cross(direction_,right_)} {
    static bool unused{FillThings()};
    static_cast<void> (unused);
}

Camera::Camera(const Camera &camera) noexcept {
    this->block_.store(camera.block_);
    this->position_ = camera.position_;
    this->direction_ = camera.direction_;
    this->right_ = camera.right_;
    this->up_ = camera.up_;
}

Camera::~Camera() noexcept {
    LOG("CAMERA DELETED");
}

void Camera::resetSampling() noexcept {
    this->block_ = 0;
}
