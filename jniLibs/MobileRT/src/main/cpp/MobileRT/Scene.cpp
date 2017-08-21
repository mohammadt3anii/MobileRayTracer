//
// Created by Tiago on 16-10-2016.
//

#include "Scene.hpp"

using MobileRT::Scene;
using MobileRT::Triangle;
using MobileRT::Sphere;
using MobileRT::Plane;
static unsigned counter {0};

Scene::~Scene() noexcept {
  this->triangles_.clear ();
  this->spheres_.clear ();
  this->planes_.clear ();
  for (Light *const light : this->lights_) {
        delete light;
    }
    this->lights_.clear();
  LOG("SCENE DELETED");
}

int Scene::traceLights (Intersection *const intersection, const Ray ray) const noexcept {
  int res {- 1};
  const unsigned lightsSize {static_cast<unsigned> (lights_.size ())};
  for (unsigned i {0}; i < lightsSize; i++) {
    const Light &light {*this -> lights_[static_cast<uint32_t> (i)]};
        if (light.intersect(intersection, ray)) {
            res = static_cast<int> (i);
        }
    }

    return res;
}

template<typename T>
int Scene::trace (const std::vector<T> &primitives, Intersection *const intersection,
                  const Ray ray, int res) const noexcept {
  const unsigned primitivesSize {static_cast<unsigned> (primitives.size ())};
  for (unsigned i {0}; i < primitivesSize; i++) {
    const T &primitive {primitives[static_cast<uint32_t> (i)]};
    if (primitive.intersect(intersection, ray)) {
      res = static_cast<int>(i);
    }
  }
  return res;
}

int Scene::trace(Intersection *const intersection, Ray ray) const noexcept {
  const int resTriangles {
    trace<MobileRT::Primitive<MobileRT::Triangle>> (this->triangles_, intersection, ray, -1)};
  const int resSpheres {
    trace<MobileRT::Primitive<MobileRT::Sphere>> (this->spheres_, intersection, ray,
                                                  resTriangles)};
  const int resPlanes {
    trace<MobileRT::Primitive<MobileRT::Plane>> (this->planes_, intersection, ray, resSpheres)};

  const int res {
    trace<MobileRT::Primitive<MobileRT::Rectangle>> (this->rectangles_, intersection, ray, resPlanes)};

  traceLights (intersection, std::move (ray));

  return res;
}

template<typename T>
bool Scene::shadowTrace(const std::vector<T> &primitives, Intersection *const intersection,
                        const Ray ray) const noexcept {
  for (const T &primitive : primitives) {
    if (primitive.intersect(intersection, ray)) {
      return true;
    }
  }
  return false;
}

bool Scene::shadowTrace(Intersection *const intersection, Ray &&ray) const noexcept {
  const bool intersectedTriangle {
    shadowTrace<MobileRT::Primitive<Triangle>> (this->triangles_, intersection, ray)};
  const bool intersectedSphere {
    shadowTrace<MobileRT::Primitive<Sphere>> (this->spheres_, intersection, ray)};
  const bool intersectedPlane {
    shadowTrace<MobileRT::Primitive<Plane>> (this->planes_, intersection, std::move (ray))};
  return intersectedTriangle || intersectedSphere || intersectedPlane;
}

unsigned Scene::getInstances () noexcept {
  const unsigned res {counter};
  counter = 0;
  return res;
}

void Scene::resetSampling() noexcept {
  for (Light *const light : this->lights_) {
    light->resetSampling ();
  }
}
