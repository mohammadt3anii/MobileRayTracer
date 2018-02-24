//
// Created by puscas on 06-03-2017.
//

#include "AABB.hpp"
#include "MobileRT/Utils.hpp"

using ::MobileRT::AABB;
using ::MobileRT::Ray;

AABB::AABB(const ::glm::vec3 pointMin, const ::glm::vec3 pointMax) noexcept :
        pointMin_{pointMin}, pointMax_{pointMax} {
}

bool MobileRT::intersect(const AABB box, const Ray ray) noexcept {
    float t1 {(box.pointMin_.x - ray.origin_.x)/ray.direction_.x};
    float t2 {(box.pointMax_.x - ray.origin_.x)/ray.direction_.x};
 
    float tmin {::std::min(t1, t2)};
    float tmax {::std::max(t1, t2)};
 
    for (int axis {1}; axis < 3; ++axis) {
        const float invDir{1.0f / ray.direction_[axis]};
        t1 = (box.pointMin_[axis] - ray.origin_[axis]) * invDir;
        t2 = (box.pointMax_[axis] - ray.origin_[axis]) * invDir;
 
        tmin = ::std::max(tmin, ::std::min(t1, t2));
        tmax = ::std::min(tmax, ::std::max(t1, t2));
    }
 
    return tmax > ::std::max(tmin, 0.0f);
}

int AABB::getLongestAxis() const noexcept {
    const float lengthX{pointMax_.x - pointMin_.x};
    const float lengthY{pointMax_.y - pointMin_.y};
    const float lengthZ{pointMax_.z - pointMin_.z};

    int longestAxis{lengthX >= lengthY && lengthX >= lengthZ ? 0 :
                    lengthY >= lengthX && lengthY >= lengthZ ? 1 :
                    2
    };

    return longestAxis;
}

float AABB::getSurfaceArea() const noexcept {
    const float lengthX{pointMax_.x - pointMin_.x};
    const float lengthY{pointMax_.y - pointMin_.y};
    const float lengthZ{pointMax_.z - pointMin_.z};

    const float bottomTopArea{2 * lengthX * lengthZ};
    const float sideAreaXY{2 * lengthX * lengthY};
    const float sideAreaZY{2 * lengthZ * lengthY};

    float surfaceArea{bottomTopArea + sideAreaXY + sideAreaZY};

    return surfaceArea;
}

namespace MobileRT {
    AABB surroundingBox(AABB box1, AABB box2) noexcept {
        ::glm::vec3 min{
                ::std::min(box1.pointMin_.x, box2.pointMin_.x),
                ::std::min(box1.pointMin_.y, box2.pointMin_.y),
                ::std::min(box1.pointMin_.z, box2.pointMin_.z)};

        ::glm::vec3 max{
                ::std::max(box1.pointMax_.x, box2.pointMax_.x),
                ::std::max(box1.pointMax_.y, box2.pointMax_.y),
                ::std::max(box1.pointMax_.z, box2.pointMax_.z)};
        return AABB(min, max);
    }
}//namespace MobileRT
