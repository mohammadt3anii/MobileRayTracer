//
// Created by puscas on 27/08/17.
//

#ifndef MOBILERT_ACCELERATORS_BVH_HPP
#define MOBILERT_ACCELERATORS_BVH_HPP

#include "AABB.hpp"
#include "../Scene.hpp"
#include "../Intersection.hpp"

using MobileRT::Scene;
using MobileRT::AABB;
using MobileRT::Intersection;
using MobileRT::Ray;
namespace MobileRT {
  class BVH final {
    class BVHNode final {
      private:
      AABB bbox;
      bool leaf;
      unsigned int n_objs;
      unsigned int index; // if leaf == false: index to left child node,
      // else if leaf == true: index to first Intersectable in Objs vector

      public:
      BVHNode *leftChild;
      BVHNode *rightChild;
      std::vector<Primitive<Triangle> *> triangles_;
      public:
      void setAABB (AABB bbox_) noexcept;
      void makeLeaf (int index_, int n_objs_) noexcept;
      void makeNode (int left_index_, int n_objs) noexcept;
      // n_objs in makeNode is for debug purposes only, and may be omitted later on

      bool isLeaf () noexcept;
      unsigned int getIndex () noexcept;
      unsigned int getNObjs () noexcept;
      AABB &getAABB () noexcept;
    };

    struct StackItem {
      BVHNode *ptr;
      float t;
    };
    private:
    std::vector<Primitive<Triangle> *> Objs;
    BVHNode root {};
    Scene *scene_ {};
    private:
      /*void build_recursive (int left_index, int right_index,
                            BVHNode *node, int depth) noexcept;*/
    public:
    explicit BVH () noexcept {}

    explicit BVH (AABB sceneBounds, Scene *scene) noexcept;
    BVH (const BVH &bVH) noexcept = delete;
    BVH (BVH &&bVH) noexcept = default;
    ~BVH () noexcept = default;
    BVH &operator= (const BVH &bVH) noexcept = delete;
    BVH &operator= (BVH &&bVH) noexcept = default;
    bool trace (Intersection *intersection, const Ray &ray) noexcept;
    bool shadowTrace (Intersection *intersection, Ray &&ray) noexcept;
  };
}
#endif //MOBILERT_ACCELERATORS_BVH_HPP
