//
// Created by Tiago on 16-10-2016.
//

#include "Scene.hpp"

using MobileRT::Scene;
using MobileRT::Triangle;
using MobileRT::Sphere;
using MobileRT::Plane;
using MobileRT::Material;

static unsigned int counter {0};

Scene::Scene() noexcept :
  triangles_ {std::vector<Triangle>{}},
  spheres_ {std::vector<Sphere>{}},
  planes_ {std::vector<Plane>{}},
  materials_ {std::vector<Material>{}},
  lights_ {std::vector<Light *>{}}
{
    counter++;
}

Scene::~Scene() noexcept {
		this->triangles_.clear();
    this->spheres_.clear();
		this->planes_.clear();
		this->materials_.clear();
    for (Light *light : this->lights_) {
        delete light;
    }
    this->lights_.clear();
		LOG("SCENE DELETED");
}

int Scene::traceLights(Intersection *intersection, Ray ray) const noexcept {

  int res {- 1};
  const unsigned int lightsSize {static_cast<unsigned int> (lights_ . size ())};

  for (unsigned int i {0}; i < lightsSize; i ++) {
    const Light &light {*this -> lights_[static_cast<uint32_t> (i)]};
        if (light.intersect(intersection, ray)) {
            res = static_cast<int> (i);
        }
    }

    return res;
}

template<typename T>
int Scene::trace(const std::vector<T> &primitives, MobileRT::Intersection *const intersection,
                 const MobileRT::Ray ray, const int offset, int res) const noexcept {

  const unsigned int primitivesSize {static_cast<unsigned int> (primitives . size ())};
  for (unsigned int i {0}; i < primitivesSize; i ++) {
    const T &primitive {primitives[static_cast<uint32_t> (i)]};
    if (primitive.intersect(intersection, ray)) {
      res = static_cast<int>(i) + offset;
    }
  }
  return res;
}

int Scene::trace(Intersection *const intersection, Ray ray) const noexcept {

  const int sizeTriangles {static_cast<int>(this -> triangles_ . size ())};
  const int sizeSpheres {static_cast<int>(this -> spheres_ . size ())};
  const int resTriangles {
    trace<MobileRT::Triangle> (this -> triangles_, intersection, ray, 0, - 1)};
  const int resSpheres {
    trace<MobileRT::Sphere> (this -> spheres_, intersection, ray, sizeTriangles, resTriangles)};
  const int res {
    trace<MobileRT::Plane>(this->planes_, intersection, ray, sizeTriangles + sizeSpheres,
                           resSpheres)};

  if (res >= 0) {//if intersects primitive, get its material
    intersection->material_ = &materials_[static_cast<uint32_t>(res)];
  }

  traceLights(intersection, ray);

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

  const bool intersectedTriangle {shadowTrace<Triangle> (this -> triangles_, intersection, ray)};
  const bool intersectedSphere {shadowTrace<Sphere> (this -> spheres_, intersection, ray)};
  const bool intersectedPlane {shadowTrace<Plane> (this -> planes_, intersection, ray)};
  return intersectedTriangle || intersectedSphere || intersectedPlane;
}

unsigned int Scene::getInstances() noexcept {

  const unsigned int res {counter};
  counter = 0;
  return res;
}

void Scene::resetSampling() noexcept {
    for (Light *light : this->lights_) {
        light->resetSampling();
    }
}
