//
// Created by Tiago on 30-10-2018.
//

#ifndef MOBILERT_NAIVE_HPP
#define MOBILERT_NAIVE_HPP

#include "MobileRT/Boxes/AABB.hpp"
#include "MobileRT/Intersection.hpp"
#include "MobileRT/Light.hpp"
#include "MobileRT/Material.hpp"
#include "MobileRT/Ray.hpp"
#include "MobileRT/Utils.hpp"
#include <glm/glm.hpp>
#include <vector>

namespace MobileRT {

    template<typename T>
    class Naive final {
    public:
        ::std::vector<T> primitives_ {};

    public:
        explicit Naive() noexcept = default;

        explicit Naive(::std::vector<T> &&primitives) noexcept;

        Naive(const Naive &naive) noexcept = delete;

        Naive(Naive &&naive) noexcept = default;

        ~Naive() noexcept;

        Naive &operator=(const Naive &naive) noexcept = delete;

        Naive &operator=(Naive &&naive) noexcept = default;

        bool trace(Intersection *intersection, const Ray &ray) noexcept;

        bool shadowTrace(const Ray &ray, float dist) noexcept;
    };

    template<typename T>
    Naive<T>::Naive(::std::vector<T> &&primitives) noexcept :
        primitives_{::std::move(primitives)} {
    }

    template<typename T>
    Naive<T>::~Naive() noexcept {
        primitives_.clear();
        ::std::vector<T> {}.swap(primitives_);
    }

    template<typename T>
    bool Naive<T>::trace(Intersection *const intersection,
                  const Ray &ray) noexcept {
        bool intersected {false};
        for (auto &primitive : primitives_) {
            intersected |= primitive.intersect(intersection, ray);
        }
        return intersected;
    }

    template<typename T>
    bool Naive<T>::shadowTrace(const Ray &ray, const float dist) noexcept {
        for (auto &primitive : primitives_) {
            const bool intersected {primitive.intersect(ray, dist)};
            if (intersected) {
                return true;
            }
        }
        return false;
    }

}//namespace MobileRT

#endif //MOBILERT_NAIVE_HPP
