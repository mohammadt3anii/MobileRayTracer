//
// Created by puscas on 27/08/17.
//

#ifndef MOBILERT_ACCELERATORS_BVH_HPP
#define MOBILERT_ACCELERATORS_BVH_HPP

#include "MobileRT/Accelerators/AABB.hpp"
#include "MobileRT/Intersection.hpp"
#include "MobileRT/Scene.hpp"
#include <algorithm>
#include <array>
#include <glm/glm.hpp>
#include <random>

namespace MobileRT {

    struct uBVHNode {
        AABB box_ {};
        ::std::uint32_t indexOffset_ {0};
        ::std::uint32_t numberPrimitives_ {0};
    };

    template<typename T>
    class BVH final {
    private:
        static const ::std::uint32_t maxLeafSize {2};

    public:
        ::std::vector<uBVHNode> boxes_{};
        ::std::vector<Primitive<T>> primitives_{};

    private:
        uBVHNode build(::std::vector<Primitive<T>> primitives, ::std::uint32_t depth,
                   ::std::uint32_t currentNodeId) noexcept;

    public:
        explicit BVH() noexcept = default;

        explicit BVH<T>(
            ::std::vector<Primitive<T>> primitives) noexcept;

        BVH(const BVH &bVH) noexcept = delete;

        BVH(BVH &&bVH) noexcept = default;

        ~BVH() noexcept = default;

        BVH &operator=(const BVH &bVH) noexcept = delete;

        BVH &operator=(BVH &&bVH) noexcept = default;

        Intersection trace(
          Intersection intersection,
          Ray ray) noexcept;

        Intersection shadowTrace(
          Intersection intersection,
          Ray ray) noexcept;
    };



    template<typename T>
    BVH<T>::BVH(::std::vector<Primitive<T>> primitives) noexcept {
        if (primitives.empty()) {
            boxes_.emplace_back(uBVHNode {});
            return;
        }
        const ::std::uint32_t numberPrimitives {static_cast<::std::uint32_t>(primitives.size())};
        ::std::uint32_t maxDepth {0};
        ::std::uint32_t maxNodes {1};
        while (maxNodes * maxLeafSize < numberPrimitives) {
            ++maxDepth;
            maxNodes = 1u << maxDepth;
        }
        maxNodes = (2u << maxDepth) - 1;

        boxes_.resize(maxNodes);
        primitives_.reserve(numberPrimitives);

        build(::std::move(primitives), 0, 0);
    }


    template<typename T>
    uBVHNode BVH<T>::build(
        ::std::vector<Primitive<T>> primitives,
        const ::std::uint32_t depth,
        const ::std::uint32_t currentNodeId) noexcept {
        if (primitives.empty()) {
            return uBVHNode{};
        }
        static ::std::uint32_t numberPrimitives {0};
        if (depth == 0) {
            numberPrimitives = static_cast<::std::uint32_t>(primitives.size());
        }

        AABB current_box{
                ::glm::vec3 {
                        ::std::numeric_limits<float>::max(),
                        ::std::numeric_limits<float>::max(),
                        ::std::numeric_limits<float>::max()},
                ::glm::vec3 {
                        ::std::numeric_limits<float>::lowest(),
                        ::std::numeric_limits<float>::lowest(),
                        ::std::numeric_limits<float>::lowest()}};
        //::std::vector<AABB> boxes {};
        for (::std::uint32_t i{0}; i < primitives.size(); ++i) {
            const AABB new_box{primitives.at(i).getAABB()};
            current_box = surroundingBox(new_box, current_box);
            //boxes.emplace_back(new_box);
        }

        const ::std::int32_t axis {current_box.getLongestAxis()};
        ::std::sort(primitives.begin(), primitives.end(),
            [=](const Primitive<T> a, const Primitive<T> b) noexcept -> bool {
                return a.getAABB().pointMin_[axis] < b.getAABB().pointMin_[axis];
            });

        ::std::int32_t divide{static_cast<::std::int32_t>(primitives.size()) / 2};
        /*{
            ::std::vector<float> left_area {boxes.at(0).getSurfaceArea()};
            AABB left_box {};
            ::std::uint32_t N {static_cast<::std::uint32_t>(primitives.size())};
            for (::std::uint32_t i {0}; i < N - 1; ++i) {
                left_box = surroundingBox(left_box, boxes.at(i));
                left_area.insert(left_area.end(), left_box.getSurfaceArea());
            }

            ::std::vector<float> right_area {boxes.at(N - 1).getSurfaceArea()};
            AABB right_box {};
            for (::std::uint32_t i {N - 1}; i > 0; i--) {
                right_box = surroundingBox(right_box, boxes.at(i));
                right_area.insert(right_area.begin(), right_box.getSurfaceArea());
            }

            float min_SAH {::std::numeric_limits<float>::max()};
            for (::std::uint32_t i {0}; i < N - 1; ++i) {
                const float SAH_left {i * left_area.at(i)};
                const float SAH_right {(N - i - 1) * right_area.at(i)};
                const float SAH {SAH_left + SAH_right};
                if (SAH < min_SAH) {
                    divide = i;
                    min_SAH = SAH;
                }
            }
        }*/
        divide = primitives.size() % 2 == 0 ? divide : divide + 1;

        uBVHNode currentNode {};
        if (numberPrimitives <= (1 << depth) * maxLeafSize) {
            currentNode.indexOffset_ = static_cast<::std::uint32_t>(primitives_.size());
            currentNode.numberPrimitives_ = static_cast<::std::uint32_t>(primitives.size());
            primitives_.insert(primitives_.end(), primitives.begin(), primitives.end());
        } else {
            using Iterator = typename ::std::vector<Primitive<T>>::const_iterator;
            Iterator leftBegin {primitives.begin()};
            Iterator leftEnd {primitives.begin() + divide};
            Iterator rightBegin {primitives.begin() + divide};
            Iterator rightEnd {primitives.end()};

            ::std::vector<Primitive<T>> leftVector(leftBegin, leftEnd);
            ::std::vector<Primitive<T>> rightVector(rightBegin, rightEnd);

            const ::std::uint32_t left {currentNodeId * 2 + 1};
            uBVHNode leftBox {build(::std::move(leftVector), depth + 1, left)};
            uBVHNode rightBox {build(::std::move(rightVector), depth + 1, left + 1)};
            current_box = surroundingBox(leftBox.box_, rightBox.box_);
            currentNode.box_ = current_box;
        }
        currentNode.box_ = current_box;
        boxes_.at(currentNodeId) = currentNode;
        return currentNode;
    }

    template<typename T>
    Intersection BVH<T>::trace(
            Intersection intersection,
            const Ray ray) noexcept {
        uBVHNode* node {&boxes_.at(0)};
        ::std::uint32_t id {0};
        ::std::array<uBVHNode*, 32> stack {};
        ::std::array<::std::uint32_t, 32> stackId {};
        ::std::uint32_t stackPtr {0};
        stack.at(stackPtr) = nullptr;
        stackId.at(stackPtr) = 0;
        ++stackPtr;
        do {
            if (intersect(node->box_, ray)) {

                if (node->numberPrimitives_ > 0) {
                    for (::std::uint32_t i {0}; i < node->numberPrimitives_; ++i) {
                        auto& primitive {primitives_.at(node->indexOffset_ + i)};
                        intersection = primitive.intersect(intersection, ray);
                    }
                    --stackPtr;
                    node = stack.at(stackPtr); // pop
                    id = stackId.at(stackPtr); // pop
                } else {
                    const ::std::uint32_t left {id * 2 + 1};
                    uBVHNode* childL {&boxes_.at(left)};
                    uBVHNode* childR {&boxes_.at(left + 1)};

                    /*for (::std::uint32_t i {0}; i < childL->numberPrimitives_; ++i) {
                        auto& primitive {primitives_.at(childL->indexOffset_ + i)};
                        const float blue {primitive.material_.Kd_[2]};
                        if(blue > 0.7f) {
                            LOG("b = ", blue);
                        }
                    }

                    for (::std::uint32_t i {0}; i < childR->numberPrimitives_; ++i) {
                        auto& primitive {primitives_.at(childR->indexOffset_ + i)};
                        const float blue {primitive.material_.Kd_[2]};
                        if(blue > 0.7f && ray.direction_[0] >= 0.3f) {
                            LOG("b = ", blue);
                        }
                    }*/

                    const bool traverseL {intersect(childL->box_, ray)};
                    const bool traverseR {intersect(childR->box_, ray)};

                    if (!traverseL && !traverseR) {
                        --stackPtr;
                        node = stack.at(stackPtr); // pop
                        id = stackId.at(stackPtr); // pop
                    } else {
                        node = (traverseL) ? childL : childR;
                        id = (traverseL) ? left : left + 1;
                        if (traverseL && traverseR) {
                            stack.at(stackPtr) = childR; // push
                            stackId.at(stackPtr) = left + 1; // push
                            ++stackPtr;
                        }
                    }
                }

            } else {
                --stackPtr;
                node = stack.at(stackPtr); // pop
                id = stackId.at(stackPtr); // pop
            }

        } while (node != nullptr);
        return intersection;
    }

    template<typename T>
    Intersection BVH<T>::shadowTrace(
        Intersection intersection,
        const Ray ray) noexcept {
        uBVHNode* node {&boxes_.at(0)};
        ::std::uint32_t id {0};
        ::std::array<uBVHNode*, 32> stack {};
        ::std::array<::std::uint32_t, 32> stackId {};
        ::std::uint32_t stackPtr {0};
        stack.at(stackPtr) = nullptr;
        stackId.at(stackPtr) = 0;
        ++stackPtr;
        do {
            if (intersect(node->box_, ray)) {

                if (node->numberPrimitives_ != 0) {
                    for (::std::uint32_t i {0}; i < node->numberPrimitives_; ++i) {
                        auto& primitive {primitives_.at(node->indexOffset_ + i)};
                        const float lastDist {intersection.length_};
                        intersection = primitive.intersect(intersection, ray);
                        if (intersection.length_ < lastDist) {
                            return intersection;
                        }
                    }
                    --stackPtr;
                    node = stack.at(stackPtr); // pop
                    id = stackId.at(stackPtr); // pop
                } else {
                    const ::std::uint32_t left {id * 2 + 1};
                    uBVHNode* childL {&boxes_.at(left)};
                    uBVHNode* childR {&boxes_.at(left + 1)};

                    const bool traverseL {intersect(childL->box_, ray)};
                    const bool traverseR {intersect(childR->box_, ray)};

                    if (!traverseL && !traverseR) {
                        --stackPtr;
                        node = stack.at(stackPtr); // pop
                        id = stackId.at(stackPtr); // pop
                    } else {
                        node = (traverseL) ? childL : childR;
                        id = (traverseL) ? left : left + 1;
                        if (traverseL && traverseR) {
                            stack.at(stackPtr) = childR; // push
                            stackId.at(stackPtr) = left + 1; // push
                            ++stackPtr;
                        }
                    }
                }

            } else {
                --stackPtr;
                node = stack.at(stackPtr); // pop
                id = stackId.at(stackPtr); // pop
            }

        } while (node != nullptr);
        return intersection;
    }

}//namespace MobileRT

#endif //MOBILERT_ACCELERATORS_BVH_HPP
