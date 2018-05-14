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
#include <vector>

namespace MobileRT {

    static const ::std::uint32_t maxLeafSize {2};

    struct BVHNode {
        AABB box_ {};
        ::std::uint32_t left_ {0};
        ::std::uint32_t indexOffset_ {0};
        ::std::uint32_t numberPrimitives_ {0};
    };

    template<typename T>
    ::std::uint32_t getSplitIndex_SAH (
        const ::std::vector<AABB> &boxes) noexcept;

    template<typename T>
    class BVH final {
    private:
        ::std::vector<BVHNode> boxes_{};
        ::std::vector<Primitive<T>> primitives_{};

    private:
        void build() noexcept;

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
          const Ray &ray) noexcept;

        Intersection shadowTrace(
          Intersection intersection,
          const Ray &ray) noexcept;
    };



    template<typename T>
    BVH<T>::BVH(::std::vector<Primitive<T>> primitives) noexcept {
        if (primitives.empty()) {
            boxes_.emplace_back(BVHNode {});
            return;
        }
        primitives_ = primitives;
        const ::std::uint32_t numberPrimitives {
            static_cast<::std::uint32_t>(primitives.size())};
        ::std::uint32_t maxNodes {1};
        while (maxNodes * maxLeafSize < numberPrimitives) {
            maxNodes *= 2;
        }
        const ::std::uint32_t maxNodes1 {numberPrimitives * 2 - 1};
        const ::std::uint32_t maxNodes2 {maxNodes * 2 - 1};

        LOG("maxNodes1 = ", maxNodes1);
        LOG("maxNodes2 = ", maxNodes2);

        boxes_.resize(maxNodes2);

        build();
    }

    template<typename T>
    void BVH<T>::build() noexcept {
        ::std::uint32_t id {0};
        ::std::uint32_t begin {0};
        ::std::uint32_t end {static_cast<::std::uint32_t>(primitives_.size())};

        ::std::array<::std::uint32_t, 512> stackId {};
        ::std::array<::std::uint32_t, 512> stackBegin {};
        ::std::array<::std::uint32_t, 512> stackEnd {};

        ::std::uint32_t stackPtrId {0};
        ::std::uint32_t stackPtrBegin {0};
        ::std::uint32_t stackPtrEnd {0};

        ::std::uint32_t maxId {0};

        stackId.at(stackPtrId++) = 0;
        stackBegin.at(stackPtrBegin++) = 0;
        stackEnd.at(stackPtrEnd++) = 0;
        do {
            boxes_.at(id).box_ = primitives_.at(begin).getAABB();
            ::std::vector<AABB> boxes {boxes_.at(id).box_};
            const ::std::uint32_t boxPrimitivesSize {end - begin};
            boxes.reserve(boxPrimitivesSize);
            for (::std::uint32_t i {begin + 1}; i < end; ++i) {
                const AABB &new_box {primitives_.at(i).getAABB()};
                boxes_.at(id).box_ = surroundingBox(new_box, boxes_.at(id).box_);
                boxes.emplace_back(new_box);
            }

            if (boxPrimitivesSize <= maxLeafSize) {
                boxes_.at(id).indexOffset_ = begin;
                boxes_.at(id).numberPrimitives_ = boxPrimitivesSize;

                id = stackId.at(--stackPtrId); // pop
                begin = stackBegin.at(--stackPtrBegin); // pop
                end = stackEnd.at(--stackPtrEnd); // pop
            } else {
                const ::std::uint32_t left {maxId + 1};
                boxes_.at(id).left_ = left;
                maxId = left + 1 > maxId? left + 1 : maxId;

                const ::std::uint32_t splitIndex {boxPrimitivesSize <= 2*maxLeafSize? 2 :
                    //(boxPrimitivesSize + 1) / 2
                    static_cast<::std::uint32_t>(getSplitIndex_SAH<T>(boxes))
                };

                stackId.at(stackPtrId++) = left + 1; // push
                stackBegin.at(stackPtrBegin++) = begin + splitIndex; // push
                stackEnd.at(stackPtrEnd++) = end; // push

                id = left;
                end = begin + splitIndex;
            }
        } while(stackPtrId > 0);
        LOG("maxNodeId = ", maxId);
        boxes_.erase (boxes_.begin() + static_cast<::std::int32_t>(maxId) + 1, boxes_.end());
        boxes_.shrink_to_fit();
        ::std::vector<BVHNode>{boxes_}.swap(boxes_);
    }

    template<typename T>
    Intersection BVH<T>::trace(
            Intersection intersection,
            const Ray &ray) noexcept {
        if(primitives_.empty()) {
            return intersection;
        }
        ::std::uint32_t id {0};
        ::std::array<::std::uint32_t, 512> stackId {};
        ::std::uint32_t stackPtrId {0};
        stackId.at(stackPtrId++) = 0;
        do {
            const BVHNode &node {boxes_.at(id)};
            if (intersect(node.box_, ray)) {

                const ::std::uint32_t numberPrimitives {node.numberPrimitives_};
                if (numberPrimitives > 0) {
                    for (::std::uint32_t i {0}; i < numberPrimitives; ++i) {
                        auto& primitive {primitives_.at(node.indexOffset_ + i)};
                        intersection = primitive.intersect(intersection, ray);
                    }
                    id = stackId.at(--stackPtrId); // pop
                } else {
                    const ::std::uint32_t left {node.left_};
                    const BVHNode &childL {boxes_.at(left)};
                    const BVHNode &childR {boxes_.at(left + 1)};

                    const bool traverseL {intersect(childL.box_, ray)};
                    const bool traverseR {intersect(childR.box_, ray)};

                    if (!traverseL && !traverseR) {
                        id = stackId.at(--stackPtrId); // pop
                    } else {
                        id = (traverseL) ? left : left + 1;
                        if (traverseL && traverseR) {
                            stackId.at(stackPtrId++) = left + 1; // push
                        }
                    }
                }

            } else {
                id = stackId.at(--stackPtrId); // pop
            }

        } while (stackPtrId > 0);
        return intersection;
    }

    template<typename T>
    Intersection BVH<T>::shadowTrace(
        Intersection intersection,
        const Ray &ray) noexcept {
        if(primitives_.empty()) {
            return intersection;
        }
        ::std::uint32_t id {0};
        ::std::array<::std::uint32_t, 512> stackId {};
        ::std::uint32_t stackPtrId {0};
        stackId.at(stackPtrId++) = 0;
        do {
            const BVHNode &node {boxes_.at(id)};
            if (intersect(node.box_, ray)) {

                const ::std::uint32_t numberPrimitives {node.numberPrimitives_};
                if (numberPrimitives > 0) {
                    for (::std::uint32_t i {0}; i < numberPrimitives; ++i) {
                        auto& primitive {primitives_.at(node.indexOffset_ + i)};
                        const float lastDist {intersection.length_};
                        intersection = primitive.intersect(intersection, ray);
                        if (intersection.length_ < lastDist) {
                            return intersection;
                        }
                    }
                    id = stackId.at(--stackPtrId); // pop
                } else {
                    const ::std::uint32_t left {node.left_};
                    const BVHNode &childL {boxes_.at(left)};
                    const BVHNode &childR {boxes_.at(left + 1)};

                    const bool traverseL {intersect(childL.box_, ray)};
                    const bool traverseR {intersect(childR.box_, ray)};

                    if (!traverseL && !traverseR) {
                        id = stackId.at(--stackPtrId); // pop
                    } else {
                        id = (traverseL) ? left : left + 1;
                        if (traverseL && traverseR) {
                            stackId.at(stackPtrId++) = left + 1; // push
                        }
                    }
                }

            } else {
                id = stackId.at(--stackPtrId); // pop
            }

        } while (stackPtrId > 0);
        return intersection;
    }

    template<typename T>
    ::std::uint32_t getSplitIndex_SAH (
        const ::std::vector<AABB> &boxes) noexcept {
            const ::std::uint32_t N {static_cast<::std::uint32_t>(boxes.size())};

            ::std::vector<float> left_area (N - maxLeafSize);
            AABB left_box {boxes.at(0)};
            left_area.at(0) = left_box.getSurfaceArea();
            for (::std::uint32_t i {1}; i < N - maxLeafSize; ++i) {
                left_box = surroundingBox(left_box, boxes.at(i));
                left_area.at(i) = left_box.getSurfaceArea();
            }

            ::std::vector<float> right_area (N - maxLeafSize);
            AABB right_box {boxes.at(N - 1)};
            right_area.at(N - maxLeafSize - 1) = right_box.getSurfaceArea();
            for (::std::uint32_t i {N - 2}; i > maxLeafSize; --i) {
                right_box = surroundingBox(right_box, boxes.at(i));
                right_area.at(i - maxLeafSize) = right_box.getSurfaceArea();
            }

            ::std::uint32_t splitIndex {maxLeafSize};
            float min_SAH {
                maxLeafSize * left_area.at(maxLeafSize - 1) +
                (N - maxLeafSize) * right_area.at(maxLeafSize - 1)};
            for (::std::uint32_t i {maxLeafSize}; i < N - maxLeafSize; ++i) {
                const float SAH_left {(i + 1) * left_area.at(i)};
                const float SAH_right {(N - (i + 1)) * right_area.at(i)};
                const float SAH {SAH_left + SAH_right};
                if (SAH < min_SAH) {
                    splitIndex = i + 1;
                    min_SAH = SAH;
                }
            }
            return splitIndex;
    }

}//namespace MobileRT

#endif //MOBILERT_ACCELERATORS_BVH_HPP
