//
// Created by puscas on 06-03-2017.
//

#include "AABB.hpp"
#include "../Utils.hpp"

using ::MobileRT::AABB;
using ::MobileRT::Ray;

AABB::AABB(const Point3D pointMin, const Point3D pointMax) noexcept :
        pointMin_{pointMin}, pointMax_{pointMax} {
}

bool AABB::intersect(const Ray &ray) const noexcept {
    float tmin{::MobileRT::Epsilon};
    float tmax{::MobileRT::RayLengthMax};
    for (int axis{0}; axis < 3; axis++) {
        const float invDir{1.0f / ray.direction_.direction_[axis]};
        float t0{(pointMin_.position_[axis] - ray.origin_.position_[axis]) * invDir};
        float t1{(pointMax_.position_[axis] - ray.origin_.position_[axis]) * invDir};
        if (invDir < 0.0f) {
            ::std::swap(t0, t1);
        }
        tmin = t0 > tmin ? t0 : tmin;
        tmax = t1 < tmax ? t1 : tmax;
        if (tmax <= tmin) {
            return false;
        }
    }
    return true;
}

int AABB::getLongestAxis() const noexcept {
    const float lengthX{pointMax_.x_() - pointMin_.x_()};
    const float lengthY{pointMax_.y_() - pointMin_.y_()};
    const float lengthZ{pointMax_.z_() - pointMin_.z_()};

    int longestAxis{lengthX >= lengthY && lengthX >= lengthZ ? 0 :
                    lengthY >= lengthX && lengthY >= lengthZ ? 1 :
                    2
    };

    return longestAxis;
}

float AABB::getSurfaceArea() const noexcept {
    const float lengthX{pointMax_.x_() - pointMin_.x_()};
    const float lengthY{pointMax_.y_() - pointMin_.y_()};
    const float lengthZ{pointMax_.z_() - pointMin_.z_()};

    const float bottomTopArea{2 * lengthX * lengthZ};
    const float sideAreaXY{2 * lengthX * lengthY};
    const float sideAreaZY{2 * lengthZ * lengthY};

    float surfaceArea{bottomTopArea + sideAreaXY + sideAreaZY};

    return surfaceArea;
}

namespace MobileRT {
    AABB surroundingBox(AABB box1, AABB box2) noexcept {
        Point3D min{
                ::std::min(box1.pointMin_.x_(), box2.pointMin_.x_()),
                ::std::min(box1.pointMin_.y_(), box2.pointMin_.y_()),
                ::std::min(box1.pointMin_.z_(), box2.pointMin_.z_())};

        Point3D max{
                ::std::max(box1.pointMax_.x_(), box2.pointMax_.x_()),
                ::std::max(box1.pointMax_.y_(), box2.pointMax_.y_()),
                ::std::max(box1.pointMax_.z_(), box2.pointMax_.z_())};
        return AABB(min, max);
    }
}//namespace MobileRT