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
    struct sLeafNode {
        unsigned indexOffset_;
        unsigned numberPrimitives_;
    };

    struct uBVHNode {
        ::MobileRT::AABB box_ {};
        struct sLeafNode leaf_;
    };

    template<typename T>
    class BVH final {
    private:
        static const unsigned maxLeafSize {2};

    public:
        unsigned numberDepth_{0};
        ::std::vector<uBVHNode> boxes_{};
        ::std::vector<MobileRT::Primitive<T>> primitives_{};

    private:
        uBVHNode build(::std::vector<MobileRT::Primitive<T>> primitives, unsigned depth,
                   uint32_t currentNodeId) noexcept;

    public:
        explicit BVH() noexcept = default;

        explicit BVH<T>(
            ::std::vector<MobileRT::Primitive<T>> primitives,
            unsigned depth = 0,
            uint32_t currentId = 0) noexcept;

        BVH(const BVH &bVH) noexcept = delete;

        BVH(BVH &&bVH) noexcept = default;

        ~BVH() noexcept = default;

        BVH &operator=(const BVH &bVH) noexcept = delete;

        BVH &operator=(BVH &&bVH) noexcept = default;

        Intersection trace(
          ::MobileRT::Intersection intersection,
          ::MobileRT::Ray ray,
          unsigned depth = 0,
          uint32_t currentNodeId = 0) noexcept;

        Intersection shadowTrace(
          ::MobileRT::Intersection intersection,
          ::MobileRT::Ray ray,
          unsigned depth = 0,
          uint32_t currentNodeId = 0) noexcept;
    };



    using MobileRT::BVH;
    using MobileRT::AABB;

    template<typename T>
    BVH<T>::BVH(
        ::std::vector<MobileRT::Primitive<T>> primitives,
        const unsigned depth,
        const uint32_t currentId) noexcept {
            if (primitives.empty()) {
                if (depth == 0) {
                    boxes_.emplace_back(uBVHNode{});
                }
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
            boxes_.at(0) = build(std::move(primitives), depth, currentId);

            primitives_.shrink_to_fit();
            ::std::vector<MobileRT::Primitive<T>>(primitives_).swap(primitives_);
    }


    template<typename T>
    uBVHNode BVH<T>::build(
        ::std::vector<MobileRT::Primitive<T>> primitives,
        const unsigned depth,
        const uint32_t currentNodeId) noexcept {
        if (primitives.empty()) {
            if (depth == 0) {
                boxes_.emplace_back(uBVHNode{});
            }
            return uBVHNode{};
        }
        static int64_t numberPrimitives {0};
        if (depth == 0) {
            numberPrimitives = static_cast<int64_t>(primitives.size());
        }
        numberDepth_ = ::std::max(depth, numberDepth_);

        AABB current_box{
                ::glm::vec3 {
                        ::std::numeric_limits<float>::max(),
                        ::std::numeric_limits<float>::max(),
                        ::std::numeric_limits<float>::max()},
                ::glm::vec3 {
                        ::std::numeric_limits<float>::lowest(),
                        ::std::numeric_limits<float>::lowest(),
                        ::std::numeric_limits<float>::lowest()}};
        for (uint32_t i{0}; i < primitives.size(); i++) {
            const AABB new_box{primitives.at(i).getAABB()};
            current_box = surroundingBox(new_box, current_box);
        }

        const int axis {current_box.getLongestAxis()};
        ::std::sort(primitives.begin(), primitives.end(),
            [=](const MobileRT::Primitive<T> a,
                const MobileRT::Primitive<T> b) noexcept -> bool {
                return a.getAABB().pointMin_[axis] < b.getAABB().pointMin_[axis];
            });

        int32_t divide{static_cast<int32_t>(primitives.size()) / 2};
        /*uint64_t divide {0};
        {
            ::std::vector<float> left_area {boxes.at(0).getSurfaceArea()};
            AABB left_box {};
            for (uint64_t i {0}; i < N - 1; i++) {
                left_box = surroundingBox(left_box, boxes.at(i));
                left_area.insert(left_area.end(), left_box.getSurfaceArea());
            }

            ::std::vector<float> right_area {boxes.at(N - 1).getSurfaceArea()};
            AABB right_box {};
            for (uint64_t i {N - 1}; i > 0; i--) {
                right_box = surroundingBox(right_box, boxes.at(i));
                right_area.insert(right_area.begin(), right_box.getSurfaceArea());
            }

            float min_SAH {::std::numeric_limits<float>::max()};
            for (uint64_t i {0}; i < N - 1; i++) {
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
            currentNode.leaf_.indexOffset_ = static_cast<unsigned>(primitives_.size());
            currentNode.leaf_.numberPrimitives_ = static_cast<unsigned>(primitives.size());
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
        const ::MobileRT::Ray ray,
        const unsigned depth,
        const uint32_t currentNodeId) noexcept {
        uBVHNode node {boxes_.at(currentNodeId)};
        if (intersect(node.box_, ray)) {
            if (depth == numberDepth_) {//node at the bottom of tree
                 for (unsigned i {0}; i < node.leaf_.numberPrimitives_; i++) {
                    auto& primitive {primitives_.at(node.leaf_.indexOffset_ + i)};
                    intersection = primitive.intersect(intersection, ray);
                }
                return intersection;
            }

            const uint32_t left {currentNodeId * 2 + 1};
            intersection = trace(intersection, ray, depth + 1, left);
            return trace(intersection, ray, depth + 1, left + 1);
        }
        return intersection;
    }


    template<typename T>
    Intersection BVH<T>::shadowTrace(
        ::MobileRT::Intersection intersection,
        const ::MobileRT::Ray ray,
        const unsigned depth,
        const uint32_t currentNodeId) noexcept {
        uBVHNode node {boxes_.at(currentNodeId)};
        if (intersect(node.box_, ray)) {
            if (depth == numberDepth_) {//node at the bottom of tree
                //const uint32_t primitiveId {currentNodeId - ((1 << depth) - 1)};
                for (unsigned i {0}; i < node.leaf_.numberPrimitives_; i++) {
                    auto& primitive {primitives_.at(node.leaf_.indexOffset_ + i)};
                    const float lastDist {intersection.length_};
                    intersection = primitive.intersect(intersection, ray);
                    if (intersection.length_ < lastDist) {
                        return intersection;
                    }
                }
                return intersection;
            }

            const uint32_t left {currentNodeId * 2 + 1};
            const float lastDist {intersection.length_};
            intersection = shadowTrace(intersection, ray, depth + 1, left);
            return intersection.length_ < lastDist? intersection :
                shadowTrace(intersection, ray, depth + 1, left + 1);
        }
        return intersection;
    }

}//namespace MobileRT

#endif //MOBILERT_ACCELERATORS_BVH_HPP
