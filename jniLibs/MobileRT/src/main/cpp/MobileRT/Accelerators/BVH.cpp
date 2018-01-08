//
// Created by puscas on 27/08/17.
//

#include "BVH.hpp"

using MobileRT::BVH;
using MobileRT::Primitive;

bool BVH::BVHNode::isLeaf () noexcept {
  return this->leaf;
}

unsigned int BVH::BVHNode::getIndex () noexcept {
  return this->index;
}

unsigned int BVH::BVHNode::getNObjs () noexcept {
  return this->n_objs;
}

AABB &BVH::BVHNode::getAABB () noexcept {
  return this->bbox;
}

void BVH::BVHNode::setAABB (AABB bbox_) noexcept {
  this->bbox = bbox_;
}

void BVH::BVHNode::makeLeaf (int index_, int n_objs_) noexcept {
  this->index = static_cast<unsigned> (index_);
  this->n_objs = static_cast<unsigned> (n_objs_);
}

void BVH::BVHNode::makeNode (int left_index_, int n_objs_) noexcept {
  this->index = static_cast<unsigned> (left_index_);
  this->n_objs = static_cast<unsigned> (n_objs_);
}

//#define THRESHOLD 1

BVH::BVH (AABB sceneBounds, Scene *scene) noexcept :
  scene_ {scene} {
  //Objs = std::vector<Primitive<Triangle>*> {};
  root = BVHNode {};
  root.triangles_ = std::vector<Primitive<Triangle> *> {};
  std::vector<BVHNode> nodes {};
  Objs = convertVector (this->scene_->triangles_);
  //worldBox = AABB(); // world bounding box

  /*for (Primitive<Triangle>& t : scene->triangles_) {
    //– worldBox.include(intersectable[i] bounding box)
    //– Objs.push_back(intersectable[i])
    //root.triangles_.emplace_back(t.getAABB());
    //Objs.emplace_back (&t);
    root.triangles_.emplace_back(&t);
  }*/
  //Set world bounding box to root node
  root.setAABB (sceneBounds);
  //build_recursive (0, static_cast<int> (Objs.size ()), &root, 0);
  LOG ("BVH");
}

/*void BVH::build_recursive (const int left_index, const int right_index, BVHNode *const node,
                           const int depth) {
  Point3D min {}, max {};
  Scene::getBounds (node->triangles_, &min, &max);
  node->setAABB (AABB {min, max});
  if ((right_index - left_index) <= THRESHOLD) {
    node->makeLeaf (left_index, right_index - left_index);
    for (unsigned i {static_cast<unsigned>(left_index)}; i < right_index; i++) {
      node->triangles_.emplace_back (this->Objs[i]);
    }
    return;
  }
  const int split_index {static_cast<int>(Objs.size () / 2)};
  BVHNode *const leftNode {new BVHNode {}};
  leftNode->makeNode (0, static_cast<int>(Objs.size () / 2));
  for (unsigned i {static_cast<unsigned>(left_index)}; i < right_index / 2; i++) {
    leftNode->triangles_.emplace_back (this->Objs[i]);
  }
  node->leftChild = leftNode;
  BVHNode *const rightNode {new BVHNode {}};
  rightNode->makeNode (0, static_cast<int>(Objs.size ()));
  for (unsigned i {static_cast<unsigned>(right_index / 2)}; i < right_index; i++) {
    rightNode->triangles_.emplace_back (this->Objs[i]);
  }
  node->rightChild = rightNode;
  //std::copy(this->scene_->triangles_.begin(), node->triangles_.end(), std::back_inserter(node->triangles_));
  build_recursive (left_index, split_index, leftNode, depth + 1);
  build_recursive (split_index, right_index, rightNode, depth + 1);
}*/

bool BVH::trace (Intersection *const intersection, const Ray &ray) noexcept {
  bool retVal {false};
  BVHNode &currentNode {root};
  std::vector<StackItem> stack {std::vector<StackItem> {}};
  if (currentNode.getAABB ().intersect (ray) <= 0) {
    return false;
  }
  while (true) {
    if (!currentNode.isLeaf ()) {
      if (currentNode.leftChild == nullptr || currentNode.rightChild == nullptr) {
        break;
      }
      BVHNode &leftChild {*currentNode.leftChild};
      BVHNode &rightChild {*currentNode.rightChild};
      const float leftIntersected {leftChild.getAABB ().intersect (ray)};
      const float rightIntersected {rightChild.getAABB ().intersect (ray)};
      if (leftIntersected > 0 && rightIntersected > 0) {
        if (leftIntersected < rightIntersected) {
          StackItem si {};
          si.ptr = &rightChild;
          si.t = rightIntersected;
          stack.emplace_back (si);
          currentNode = leftChild;
        } else {
          StackItem si {};
          si.ptr = &leftChild;
          si.t = leftIntersected;
          stack.emplace_back (si);
          currentNode = rightChild;
        }
        continue;
      } else {
        if (leftIntersected > 0) {
          currentNode = leftChild;
          continue;
        }
        if (rightIntersected > 0) {
          currentNode = rightChild;
          continue;
        }
      }
    } else {//is Leaf
      for (Primitive<Triangle> *primitive : currentNode.triangles_) {
        retVal |= primitive->intersect (intersection, ray);
      }
    }
    //do {
    currentNode = *stack.back ().ptr;
    stack.pop_back ();
    //} while (stack.size());
    if (stack.empty ()) {
      break;
    }
  }
  return retVal;
}

bool BVH::shadowTrace (Intersection *const /*intersection*/, Ray && /*ray*/) noexcept {
  return false;
}
