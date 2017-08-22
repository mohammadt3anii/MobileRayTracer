//
// Created by Tiago on 22/08/17.
//

#ifndef MOBILERT_ACCELERATORS_KDTREE_HPP
#define MOBILERT_ACCELERATORS_KDTREE_HPP

#include "../Scene.hpp"

namespace MobileRT {
  class KdTree final {
    public:
    explicit KdTree () = default;
    bool trace (Intersection *intersection, const Ray &ray) const noexcept;
    bool shadowTrace (Intersection *intersection, Ray &&ray) const noexcept;
  };
}
#endif //MOBILERT_ACCELERATORS_KDTREE_HPP
