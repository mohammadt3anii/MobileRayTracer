//
// Created by puscas on 27/08/17.
//

#ifndef MOBILERT_ACCELERATORS_BVH_HPP
#define MOBILERT_ACCELERATORS_BVH_HPP

#include "MobileRT/Accelerators/AABB.hpp"
#include "MobileRT/Intersection.hpp"
#include "MobileRT/Scene.hpp"
#include <glm/glm.hpp>
#include <random>

namespace MobileRT {

    template<typename T>
    class BVH final {
    public:
        ::MobileRT::AABB box_{};
        ::std::unique_ptr<BVH> left_{nullptr};
        ::std::unique_ptr<BVH> right_{nullptr};
        ::std::vector<MobileRT::Primitive<T>> primitives_{};

    public:
        explicit BVH() noexcept = default;

        explicit BVH<T>(::std::vector<MobileRT::Primitive<T>> primitives,
                        unsigned depth = 0) noexcept;

        BVH(const BVH &bVH) noexcept = delete;

        BVH(BVH &&bVH) noexcept = default;

        ~BVH() noexcept = default;

        BVH &operator=(const BVH &bVH) noexcept = delete;

        BVH &operator=(BVH &&bVH) noexcept = default;

        Intersection trace(
            ::MobileRT::Intersection intersection,
            ::MobileRT::Ray ray) noexcept;

        Intersection shadowTrace(::MobileRT::Intersection intersection, Ray ray) noexcept;
    };


    using MobileRT::BVH;

    template<typename T>
    BVH<T>::BVH(
        ::std::vector<MobileRT::Primitive<T>> primitives,
        const unsigned depth) noexcept {
        if (primitives.empty()) {
            return;
        }
        const uint32_t N{static_cast<uint32_t>(primitives.size())};

        AABB current_box{
                glm::vec3 {
                        std::numeric_limits<float>::max(),
                        std::numeric_limits<float>::max(),
                        std::numeric_limits<float>::max()},
                glm::vec3 {
                        std::numeric_limits<float>::lowest(),
                        std::numeric_limits<float>::lowest(),
                        std::numeric_limits<float>::lowest()}};
        for (uint32_t i{0}; i < primitives.size(); i++) {
            const AABB new_box{primitives.at(i).getAABB()};
            current_box = surroundingBox(new_box, current_box);
        }
        for (uint32_t i{0}; i < N; i++) {
            const AABB new_box{primitives.at(i).getAABB()};
            box_ = surroundingBox(new_box, box_);
        }

        const int axis{box_.getLongestAxis()};
        switch (axis) {
            case 0:
                ::std::sort(primitives.begin(), primitives.end(),
                            [](const MobileRT::Primitive<T> &a,
                                MobileRT::Primitive<T> &b) noexcept -> bool {
                                return a.getAABB().pointMin_.x < b.getAABB().pointMin_.x;
                            });
                break;

            case 1:
                ::std::sort(primitives.begin(), primitives.end(),
                            [](const MobileRT::Primitive<T> &a,
                                MobileRT::Primitive<T> &b) noexcept -> bool {
                                return a.getAABB().pointMin_.y < b.getAABB().pointMin_.y;
                            });
                break;

            default:
                ::std::sort(primitives.begin(), primitives.end(),
                            [](const MobileRT::Primitive<T> &a,
                                MobileRT::Primitive<T> &b) noexcept -> bool {
                                return a.getAABB().pointMin_.z < b.getAABB().pointMin_.z;
                            });
                break;
        }

        std::vector<AABB> boxes{};
        for (uint32_t i{0}; i < N; i++) {
            boxes.emplace_back(primitives.at(i).getAABB());
        }

        //const uint64_t min_SAH_idx {static_cast<int64_t>(primitives.size()) / 2};
        uint64_t min_SAH_idx{0};
        {
            std::vector<float> left_area{boxes.at(0).getSurfaceArea()};
            AABB left_box{};
            for (uint32_t i{0}; i < N - 1; i++) {
                left_box = surroundingBox(left_box, boxes.at(i));
                left_area.insert(left_area.end(), left_box.getSurfaceArea());
            }

            std::vector<float> right_area{boxes.at(N - 1).getSurfaceArea()};
            AABB right_box{};
            for (uint32_t i{N - 1}; i > 0; i--) {
                right_box = surroundingBox(right_box, boxes.at(i));
                right_area.insert(right_area.begin(), right_box.getSurfaceArea());
            }

            float min_SAH{std::numeric_limits<float>::max()};
            for (uint32_t i{0}; i < N - 1; i++) {
                const float SAH_left{i * left_area.at(i)};
                const float SAH_right{(N - i - 1) * right_area.at(i)};
                const float SAH{SAH_left + SAH_right};
                if (SAH < min_SAH) {
                    min_SAH_idx = i;
                    min_SAH = SAH;
                }
            }
        }
        //LOG("depth = ", depth, ", size = ", primitives.size(), ", min_SAH_idx = ", min_SAH_idx);

        //0 primitives to left || 0 primitives to right
        if (min_SAH_idx == 0 || min_SAH_idx == N - 2
            || primitives.size() <= 10 || depth >= 12
                ) {
            primitives_ = primitives;
        } else {
            using Iterator = typename ::std::vector<MobileRT::Primitive<T>>::const_iterator;
            Iterator leftBegin{primitives.begin()};
            Iterator leftEnd{primitives.begin() + static_cast<int32_t>(min_SAH_idx) + 1};
            Iterator rightBegin{primitives.begin() + static_cast<int32_t>(min_SAH_idx) + 1};
            Iterator rightEnd{primitives.end()};

            ::std::vector<MobileRT::Primitive<T>> leftVector(leftBegin, leftEnd);
            ::std::vector<MobileRT::Primitive<T>> rightVector(rightBegin, rightEnd);

            left_ = std::make_unique<BVH>(std::move(leftVector), depth + 1);
            right_ = std::make_unique<BVH>(std::move(rightVector), depth + 1);
        }
    }


    template<typename T>
    Intersection BVH<T>::trace(
        ::MobileRT::Intersection intersection,
        const ::MobileRT::Ray ray) noexcept {
        if (intersect(box_, ray)) {
            if (left_ == nullptr) {
                for (MobileRT::Primitive<T> &s : primitives_) {
                    intersection = s.intersect(intersection, ray);
                }
                return intersection;
            }
            intersection = left_->trace(intersection, ray);
            intersection = right_->trace(intersection, ray);

            return intersection;
        }
        return intersection;
    }

    template<typename T>
    Intersection BVH<T>::shadowTrace(::MobileRT::Intersection intersection, const Ray ray) noexcept {
        if (intersect(box_, ray)) {
            if (left_ == nullptr) {
                for (MobileRT::Primitive<T> &s : primitives_) {
                    const float dist {intersection.length_};
                    intersection = s.intersect(intersection, ray);
                    if (intersection.length_ < dist) {
                        return intersection;
                    }
                }
                return intersection;
            }

            const float dist {intersection.length_};
            intersection = left_->shadowTrace(intersection, ray);
            if (intersection.length_ < dist) {
                return intersection;
            }
            return right_->shadowTrace(intersection, ray);
        }
        return intersection;
    }

}//namespace MobileRT


#endif //MOBILERT_ACCELERATORS_BVH_HPP
