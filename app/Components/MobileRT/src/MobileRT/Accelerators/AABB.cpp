//
// Created by puscas on 06-03-2017.
//

#include "AABB.hpp"
#include "MobileRT/Utils.hpp"

using ::MobileRT::AABB;
using ::MobileRT::Ray;

AABB::AABB(const Point3D pointMin, const Point3D pointMax) noexcept :
        pointMin_{pointMin}, pointMax_{pointMax} {
}

bool MobileRT::intersect(const AABB box, const Ray ray) noexcept {
    float t1 {(box.pointMin_.x_() - ray.origin_.x_())/ray.direction_.x_()};
    float t2 {(box.pointMax_.x_() - ray.origin_.x_())/ray.direction_.x_()};
 
    float tmin {::std::min(t1, t2)};
    float tmax {::std::max(t1, t2)};
 
    for (size_t axis {1}; axis < 3; ++axis) {
        const float invDir{1.0f / ray.direction_.direction_.at(axis)};
        t1 = (box.pointMin_.position_.at(axis) - ray.origin_.position_.at(axis)) * invDir;
        t2 = (box.pointMax_.position_.at(axis) - ray.origin_.position_.at(axis)) * invDir;
 
        tmin = ::std::max(tmin, ::std::min(t1, t2));
        tmax = ::std::min(tmax, ::std::max(t1, t2));
    }
 
    return tmax > ::std::max(tmin, 0.0f);
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
