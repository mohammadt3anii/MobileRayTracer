//
// Created by puscas on 27/08/17.
//

#ifndef MOBILERT_ACCELERATORS_BVH_HPP
#define MOBILERT_ACCELERATORS_BVH_HPP

#include "MobileRT/Accelerators/AABB.hpp"
#include "MobileRT/Intersection.hpp"
#include "MobileRT/Scene.hpp"
#include <algorithm>
#include <glm/glm.hpp>
#include <random>

namespace MobileRT {

    struct uBVHNode {
        ::MobileRT::AABB box_ {};
        unsigned indexOffset_ {0};
        unsigned numberPrimitives_ {0};
    };

    template<typename T>
    class BVH final {
    private:
        static const unsigned maxLeafSize {2};

    public:
        ::std::vector<uBVHNode> boxes_{};
        ::std::vector<MobileRT::Primitive<T>> primitives_{};

    private:
        uBVHNode build(::std::vector<MobileRT::Primitive<T>> primitives, unsigned depth,
                   uint32_t currentNodeId) noexcept;

    public:
        explicit BVH() noexcept = default;

        explicit BVH<T>(
            ::std::vector<MobileRT::Primitive<T>> primitives) noexcept;

        BVH(const BVH &bVH) noexcept = delete;

        BVH(BVH &&bVH) noexcept = default;

        ~BVH() noexcept = default;

        BVH &operator=(const BVH &bVH) noexcept = delete;

        BVH &operator=(BVH &&bVH) noexcept = default;

        Intersection trace(
          ::MobileRT::Intersection intersection,
          ::MobileRT::Ray ray) noexcept;

        Intersection shadowTrace(
          ::MobileRT::Intersection intersection,
          ::MobileRT::Ray ray) noexcept;
    };



    using MobileRT::BVH;
    using MobileRT::AABB;

    template<typename T>
    BVH<T>::BVH(
        ::std::vector<MobileRT::Primitive<T>> primitives) noexcept {
            if (primitives.empty()) {
                boxes_.emplace_back(uBVHNode{});
                return;
            }
            uint32_t numberPrimitives{static_cast<uint32_t>(primitives.size())};
            uint32_t maxDepth{0};
            uint32_t maxNodes{1};
            while (maxNodes * maxLeafSize < numberPrimitives) {
                maxDepth++;
                maxNodes = 1u << maxDepth;
            }
            while (maxDepth > 0) {
                maxDepth--;
                maxNodes += 1 << maxDepth;
            }
            //LOG ("maxNodes = ", maxNodes);
            boxes_.resize(maxNodes);

            primitives_.reserve(primitives.size());
            boxes_.at(0) = build(std::move(primitives), 0, 0);

            primitives_.shrink_to_fit();
            ::std::vector<MobileRT::Primitive<T>>(primitives_).swap(primitives_);
    }


    template<typename T>
    uBVHNode BVH<T>::build(
        ::std::vector<MobileRT::Primitive<T>> primitives,
        const unsigned depth,
        const uint32_t currentNodeId) noexcept {
        if (primitives.empty()) {
            return uBVHNode{};
        }
        static uint32_t numberPrimitives {0};
        if (depth == 0) {
            numberPrimitives = static_cast<uint32_t>(primitives.size());
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
        for (uint32_t i{0}; i < primitives.size(); i++) {
            const AABB new_box{primitives.at(i).getAABB()};
            current_box = surroundingBox(new_box, current_box);
            //boxes.emplace_back(new_box);
        }

        const int axis {current_box.getLongestAxis()};
        ::std::sort(primitives.begin(), primitives.end(),
            [=](const MobileRT::Primitive<T> a,
                const MobileRT::Primitive<T> b) noexcept -> bool {
                return a.getAABB().pointMin_[axis] < b.getAABB().pointMin_[axis];
            });

        uint32_t divide{static_cast<uint32_t>(primitives.size()) / 2};
        /*{
            ::std::vector<float> left_area {boxes.at(0).getSurfaceArea()};
            AABB left_box {};
            uint32_t N {static_cast<uint32_t>(primitives.size())};
            for (uint32_t i {0}; i < N - 1; i++) {
                left_box = surroundingBox(left_box, boxes.at(i));
                left_area.insert(left_area.end(), left_box.getSurfaceArea());
            }

            ::std::vector<float> right_area {boxes.at(N - 1).getSurfaceArea()};
            AABB right_box {};
            for (uint32_t i {N - 1}; i > 0; i--) {
                right_box = surroundingBox(right_box, boxes.at(i));
                right_area.insert(right_area.begin(), right_box.getSurfaceArea());
            }

            float min_SAH {::std::numeric_limits<float>::max()};
            for (uint32_t i {0}; i < N - 1; i++) {
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
            currentNode.indexOffset_ = static_cast<unsigned>(primitives_.size());
            currentNode.numberPrimitives_ = static_cast<unsigned>(primitives.size());
            primitives_.insert(primitives_.end(), primitives.begin(), primitives.end());
        } else {
            using Iterator = typename ::std::vector<MobileRT::Primitive<T>>::const_iterator;
            Iterator leftBegin {primitives.begin()};
            Iterator leftEnd {primitives.begin() + divide};
            Iterator rightBegin {primitives.begin() + divide};
            Iterator rightEnd {primitives.end()};

            ::std::vector<MobileRT::Primitive<T>> leftVector(leftBegin, leftEnd);
            ::std::vector<MobileRT::Primitive<T>> rightVector(rightBegin, rightEnd);

            const uint32_t left {currentNodeId * 2 + 1};
            uBVHNode leftBox {build(std::move(leftVector), depth + 1, left)};
            uBVHNode rightBox {build(std::move(rightVector), depth + 1, left + 1)};
            current_box = surroundingBox(leftBox.box_, rightBox.box_);
            currentNode.box_ = current_box;
        }
        currentNode.box_ = current_box;
        boxes_.at(currentNodeId) = currentNode;
        return currentNode;
    }

    template<typename T>
    Intersection BVH<T>::trace(
            ::MobileRT::Intersection intersection,
            const ::MobileRT::Ray ray) noexcept {
        uBVHNode* node {&boxes_.at(0)};
        uint32_t id {0};
        uBVHNode* stack[64];
        uint32_t stackId[64];
        uBVHNode** stackPtr = stack;
        uint32_t* stackPtrId = stackId;
        *stackPtr++ = nullptr;
        *stackPtrId++ = 0;
        do {
            if (intersect(node->box_, ray)) {

                if (node->numberPrimitives_ != 0) {
                    for (unsigned i {0}; i < node->numberPrimitives_; i++) {
                        auto& primitive {primitives_.at(node->indexOffset_ + i)};
                        intersection = primitive.intersect(intersection, ray);
                    }
                    node = *--stackPtr; // pop
                    id = *--stackPtrId; // pop
                } else {
                    const uint32_t left {id * 2 + 1};
                    uBVHNode* childL {&boxes_.at(left)};
                    uBVHNode* childR {&boxes_.at(left + 1)};

                    const bool traverseL {intersect(childL->box_, ray)};
                    const bool traverseR {intersect(childR->box_, ray)};

                    if (!traverseL && !traverseR) {
                        node = *--stackPtr; // pop
                        id = *--stackPtrId; // pop
                    } else {
                        node = (traverseL) ? childL : childR;
                        id = (traverseL) ? left : left + 1;
                        if (traverseL && traverseR) {
                            *stackPtr++ = childR; // push
                            *stackPtrId++ = left + 1; // push
                        }
                    }
                }

            } else {
                node = *--stackPtr; // pop
                id = *--stackPtrId; // pop
            }

        } while (node != nullptr);
        return intersection;
    }

    template<typename T>
    Intersection BVH<T>::shadowTrace(
        ::MobileRT::Intersection intersection,
        const ::MobileRT::Ray ray) noexcept {
        uBVHNode* node {&boxes_.at(0)};
        uint32_t id {0};
        uBVHNode* stack[64];
        uint32_t stackId[64];
        uBVHNode** stackPtr = stack;
        uint32_t* stackPtrId = stackId;
        *stackPtr++ = nullptr;
        *stackPtrId++ = 0;
        do {
            if (intersect(node->box_, ray)) {

                if (node->numberPrimitives_ != 0) {
                    for (unsigned i {0}; i < node->numberPrimitives_; i++) {
                        auto& primitive {primitives_.at(node->indexOffset_ + i)};
                        const float lastDist {intersection.length_};
                        intersection = primitive.intersect(intersection, ray);
                        if (intersection.length_ < lastDist) {
                            return intersection;
                        }
                    }
                    node = *--stackPtr; // pop
                    id = *--stackPtrId; // pop
                } else {
                    const uint32_t left {id * 2 + 1};
                    uBVHNode* childL {&boxes_.at(left)};
                    uBVHNode* childR {&boxes_.at(left + 1)};

                    const bool traverseL {intersect(childL->box_, ray)};
                    const bool traverseR {intersect(childR->box_, ray)};

                    if (!traverseL && !traverseR) {
                        node = *--stackPtr; // pop
                        id = *--stackPtrId; // pop
                    } else {
                        node = (traverseL) ? childL : childR;
                        id = (traverseL) ? left : left + 1;
                        if (traverseL && traverseR) {
                            *stackPtr++ = childR; // push
                            *stackPtrId++ = left + 1; // push
                        }
                    }
                }

            } else {
                node = *--stackPtr; // pop
                id = *--stackPtrId; // pop
            }

        } while (node != nullptr);
        return intersection;
    }

}//namespace MobileRT

#endif //MOBILERT_ACCELERATORS_BVH_HPP
