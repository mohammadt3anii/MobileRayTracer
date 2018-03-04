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

    struct BVHNode {
        AABB box_ {};
        ::std::uint32_t indexOffset_ {0};
        ::std::uint32_t numberPrimitives_ {0};
    };

    template<typename T>
    ::std::uint32_t getSplitIndex_SAH (
        ::std::vector<AABB> boxes) noexcept;

    template<typename T>
    class BVH final {
    private:
        static const ::std::uint32_t maxLeafSize {2};

    public:
        ::std::vector<BVHNode> boxes_{};
        ::std::vector<Primitive<T>> primitives_{};

    private:
        void build(
            ::std::uint32_t begin, ::std::uint32_t end,
            ::std::uint32_t depth, ::std::uint32_t currentNodeId) noexcept;

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
            boxes_.emplace_back(BVHNode {});
            return;
        }
        primitives_ = primitives;
        const ::std::uint32_t numberPrimitives {static_cast<::std::uint32_t>(primitives.size())};
        ::std::uint32_t maxDepth {0};
        ::std::uint32_t maxNodes {1};
        while (maxNodes * maxLeafSize < numberPrimitives) {
            ++maxDepth;
            maxNodes = 1u << maxDepth;
        }
        maxNodes = (2u << maxDepth) - 1;

        boxes_.resize(maxNodes);
        //boxes_.resize(numberPrimitives);

        build(0, numberPrimitives, 0, 0);
    }


    template<typename T>
    void BVH<T>::build(
        const ::std::uint32_t begin, const ::std::uint32_t end,
        const ::std::uint32_t depth, const ::std::uint32_t currentNodeId) noexcept {

        AABB current_box {
            ::glm::vec3 {::std::numeric_limits<float>::max()},
            ::glm::vec3 {::std::numeric_limits<float>::lowest()}};

        //::std::vector<AABB> boxes {};
        //boxes.reserve(end - begin);
        for (::std::uint32_t i {begin}; i < end; ++i) {
            const AABB new_box {primitives_.at(i).getAABB()};
            current_box = surroundingBox(new_box, current_box);
            //boxes.emplace_back(new_box);
        }

        //static ::std::int32_t axisCounter {0};
        //const ::std::int32_t axis {axisCounter++ % 3};
        const ::std::int32_t axis {current_box.getLongestAxis()};
        ::std::sort(
            primitives_.begin() + static_cast<::std::int32_t>(begin),
            primitives_.begin() + static_cast<::std::int32_t>(end),
            [=](const Primitive<T> a, const Primitive<T> b) noexcept -> bool {
                return a.getAABB().pointMin_[axis] < b.getAABB().pointMin_[axis];
            }
        );

        boxes_.at(currentNodeId).box_ = current_box;

        const ::std::uint32_t splitIndex {(end + begin) / 2};
        /*const ::std::uint32_t splitIndex {
            static_cast<::std::uint32_t>(getSplitIndex_SAH<T>(::std::move(boxes)))};*/

        if (primitives_.size() <= (1 << depth) * maxLeafSize) {
            boxes_.at(currentNodeId).indexOffset_ = begin;
            boxes_.at(currentNodeId).numberPrimitives_ = end - begin;
        } else {
            const ::std::uint32_t left {currentNodeId * 2 + 1};
            build(begin, splitIndex, depth + 1, left);
            build(splitIndex, end, depth + 1, left + 1);
        }
    }

    template<typename T>
    Intersection BVH<T>::trace(
            Intersection intersection,
            const Ray ray) noexcept {
        BVHNode* node {&boxes_.at(0)};
        ::std::uint32_t id {0};
        ::std::array<BVHNode*, 32> stack {};
        ::std::array<::std::uint32_t, 32> stackId {};
        ::std::uint32_t stackPtr {0};
        ::std::uint32_t stackPtrId {0};
        stack.at(stackPtr++) = nullptr;
        stackId.at(stackPtrId++) = 0;
        do {
            if (intersect(node->box_, ray)) {

                if (node->numberPrimitives_ != 0) {
                    for (::std::uint32_t i {0}; i < node->numberPrimitives_; ++i) {
                        auto& primitive {primitives_.at(node->indexOffset_ + i)};
                        intersection = primitive.intersect(intersection, ray);
                    }
                    node = stack.at(--stackPtr); // pop
                    id = stackId.at(--stackPtrId); // pop
                } else {
                    const ::std::uint32_t left {id * 2 + 1};
                    BVHNode* const childL {&boxes_.at(left)};
                    BVHNode* const childR {&boxes_.at(left + 1)};

                    const bool traverseL {intersect(childL->box_, ray)};
                    const bool traverseR {intersect(childR->box_, ray)};

                    if (!traverseL && !traverseR) {
                        node = stack.at(--stackPtr); // pop
                        id = stackId.at(--stackPtrId); // pop
                    } else {
                        node = (traverseL) ? childL : childR;
                        id = (traverseL) ? left : left + 1;
                        if (traverseL && traverseR) {
                            stack.at(stackPtr++) = childR; // push
                            stackId.at(stackPtrId++) = left + 1; // push
                        }
                    }
                }

            } else {
                node = stack.at(--stackPtr); // pop
                id = stackId.at(--stackPtrId); // pop
            }

        } while (node != nullptr);
        return intersection;
    }

    template<typename T>
    Intersection BVH<T>::shadowTrace(
        Intersection intersection,
        const Ray ray) noexcept {
        BVHNode* node {&boxes_.at(0)};
        ::std::uint32_t id {0};
        ::std::array<BVHNode*, 32> stack {};
        ::std::array<::std::uint32_t, 32> stackId {};
        ::std::uint32_t stackPtr {0};
        ::std::uint32_t stackPtrId {0};
        stack.at(stackPtr++) = nullptr;
        stackId.at(stackPtrId++) = 0;
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
                    node = stack.at(--stackPtr); // pop
                    id = stackId.at(--stackPtrId); // pop
                } else {
                    const ::std::uint32_t left {id * 2 + 1};
                    BVHNode* const childL {&boxes_.at(left)};
                    BVHNode* const childR {&boxes_.at(left + 1)};

                    const bool traverseL {intersect(childL->box_, ray)};
                    const bool traverseR {intersect(childR->box_, ray)};

                    if (!traverseL && !traverseR) {
                        node = stack.at(--stackPtr); // pop
                        id = stackId.at(--stackPtrId); // pop
                    } else {
                        node = (traverseL) ? childL : childR;
                        id = (traverseL) ? left : left + 1;
                        if (traverseL && traverseR) {
                            stack.at(stackPtr++) = childR; // push
                            stackId.at(stackPtrId++) = left + 1; // push
                        }
                    }
                }

            } else {
                node = stack.at(--stackPtr); // pop
                id = stackId.at(--stackPtrId); // pop
            }

        } while (node != nullptr);
        return intersection;
    }

    template<typename T>
    ::std::uint32_t getSplitIndex_SAH (
        ::std::vector<AABB> boxes) noexcept {
            const ::std::uint32_t N {static_cast<::std::uint32_t>(boxes.size())};

            ::std::vector<float> left_area {boxes.at(0).getSurfaceArea()};
            AABB left_box {};
            for (::std::uint32_t i {0}; i < N - 1; ++i) {
                left_box = surroundingBox(left_box, boxes.at(i));
                left_area.insert(left_area.end(), left_box.getSurfaceArea());
            }

            ::std::vector<float> right_area {boxes.at(N - 1).getSurfaceArea()};
            AABB right_box {};
            for (::std::uint32_t i {N - 1}; i > 0; --i) {
                right_box = surroundingBox(right_box, boxes.at(i));
                right_area.insert(right_area.begin(), right_box.getSurfaceArea());
            }

            ::std::uint32_t splitIndex {};
            float min_SAH {::std::numeric_limits<float>::max()};
            for (::std::uint32_t i {0}; i < N - 1; ++i) {
                const float SAH_left {i * left_area.at(i)};
                const float SAH_right {(N - i - 1) * right_area.at(i)};
                const float SAH {SAH_left + SAH_right};
                if (SAH < min_SAH) {
                    splitIndex = i;
                    min_SAH = SAH;
                }
            }
            return splitIndex;
    }

}//namespace MobileRT

#endif //MOBILERT_ACCELERATORS_BVH_HPP
