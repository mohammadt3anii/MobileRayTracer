#include "Scenes.hpp"

MobileRT::Scene cornellBoxScene(MobileRT::Scene&& scene) noexcept {
  // point light - white
  const MobileRT::Material lightMat {MobileRT::RGB {0.0f, 0.0f, 0.0f},
                                     MobileRT::RGB {0.0f, 0.0f, 0.0f},
                                     MobileRT::RGB {0.0f, 0.0f, 0.0f},
                                     1.0f,
                                     MobileRT::RGB {0.9f, 0.9f, 0.9f}};
  scene.lights_.emplace_back (new Components::PointLight {lightMat,
                                                          MobileRT::Point3D {0.0f, 0.99f, 0.0f}});

  // triangle - yellow
  const MobileRT::Material yellowMat {MobileRT::RGB (0.9f, 0.9f, 0.0f)};
  scene.triangles_.emplace_back (MobileRT::Triangle {
    MobileRT::Point3D {0.5f, -0.5f, 0.99f}, MobileRT::Point3D {-0.5f, -0.5f, 0.99f},
    MobileRT::Point3D {0.5f, 0.5f, 1.001f}}, yellowMat);

  // sphere - mirror
  const MobileRT::Material MirrorMat {MobileRT::RGB (0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB (0.9f, 0.9f, 0.9f)};
  scene.spheres_.emplace_back (MobileRT::Sphere {
    MobileRT::Point3D {0.45f, -0.65f, 0.4f}, 0.35f}, MirrorMat);

  // sphere - green
  const MobileRT::Material GreenMat {MobileRT::RGB {0.0f, 0.9f, 0.0f},
                                     MobileRT::RGB {0.0f, 0.2f, 0.0f}};
  scene.spheres_.emplace_back (MobileRT::Sphere {
    MobileRT::Point3D {-0.45f, -0.1f, 0.0f}, 0.35f}, GreenMat);

  // back wall - white
  const MobileRT::Material lightGrayMat {MobileRT::RGB {0.7f, 0.7f, 0.7f}};
  scene.planes_.emplace_back (MobileRT::Plane {
    MobileRT::Point3D {0.0f, 0.0f, 1.0f}, MobileRT::Vector3D {0.0f, 0.0f, -1.0f}}, lightGrayMat);

  // front wall - light blue
  const MobileRT::Material lightBlueMat {MobileRT::RGB (0.0f, 0.9f, 0.9f)};
  scene.planes_.emplace_back (MobileRT::Plane {
    MobileRT::Point3D {0.0f, 0.0f, -3.5f}, MobileRT::Vector3D {0.0f, 0.0f, 1.0f}}, lightBlueMat);

  // floor - white
  scene.planes_.emplace_back (MobileRT::Plane {
    MobileRT::Point3D {0.0f, -1.0f, 0.0f}, MobileRT::Vector3D {0.0f, 1.0f, 0.0f}}, lightGrayMat);

  // ceiling - white
  scene.planes_.emplace_back (MobileRT::Plane {
    MobileRT::Point3D {0.0f, 1.0f, 0.0f}, MobileRT::Vector3D {0.0f, -1.0f, 0.0f}}, lightGrayMat);

  // left wall - red
  const MobileRT::Material redMat {MobileRT::RGB (0.9f, 0.0f, 0.0f)};
  scene.planes_.emplace_back (MobileRT::Plane {
    MobileRT::Point3D {-1.0f, 0.0f, 0.0f}, MobileRT::Vector3D {1.0f, 0.0f, 0.0f}}, redMat);

  // right wall - blue
  const MobileRT::Material blueMat {MobileRT::RGB (0.0f, 0.0f, 0.9f)};
  scene.planes_.emplace_back (MobileRT::Plane {
    MobileRT::Point3D {1.0f, 0.0f, 0.0f}, MobileRT::Vector3D {-1.0f, 0.0f, 0.0f}}, blueMat);

  return std::move(scene);
}

MobileRT::Scene cornellBoxScene2 (MobileRT::Scene &&scene) noexcept {
  const MobileRT::Material lightMat {MobileRT::RGB {0.0f, 0.0f, 0.0f},
                                     MobileRT::RGB {0.0f, 0.0f, 0.0f},
                                     MobileRT::RGB {0.0f, 0.0f, 0.0f},
                                     1.0f,
                                     MobileRT::RGB {0.9f, 0.9f, 0.9f}};
  std::unique_ptr<MobileRT::Sampler> samplerPoint1 {
    std::make_unique<Components::MersenneTwister> ()};
  scene.lights_.emplace_back (new Components::AreaLight {lightMat,
                                                             std::move (samplerPoint1),
                                                             MobileRT::Point3D {-0.25f, 0.99f,
                                                                                -0.25f},
                                                             MobileRT::Point3D {0.25f, 0.99f,
                                                                                -0.25f},
                                                             MobileRT::Point3D {0.25f, 0.99f,
                                                                                0.25f}});

  std::unique_ptr<MobileRT::Sampler> samplerPoint2 {
    std::make_unique<Components::MersenneTwister> ()};
  scene.lights_.emplace_back (new Components::AreaLight {lightMat,
                                                             std::move (samplerPoint2),
                                                             MobileRT::Point3D {0.25f, 0.99f,
                                                                                0.25f},
                                                             MobileRT::Point3D {-0.25f, 0.99f,
                                                                                0.25f},
                                                             MobileRT::Point3D {-0.25f, 0.99f,
                                                                                -0.25f}});

  scene.lights_.emplace_back(
    new Components::PointLight {lightMat,
                                MobileRT::Point3D {0.70f, 0.99f, 0.0f}});

  // triangle - yellow
  const MobileRT::Material yellowMat {MobileRT::RGB {0.9f, 0.9f, 0.0f}};
  scene.triangles_.emplace_back (MobileRT::Triangle {
    MobileRT::Point3D {0.5f, -0.5f, 0.99f}, MobileRT::Point3D {-0.5f, -0.5f, 0.99f},
    MobileRT::Point3D {0.5f, 0.5f, 1.001f}}, yellowMat);

  // triangle - green
  const MobileRT::Material greenMat {MobileRT::RGB {0.0f, 0.9f, 0.0f}};
  scene.triangles_.emplace_back (MobileRT::Triangle {
    MobileRT::Point3D {-0.5f, 0.5f, 0.99f},
    MobileRT::Point3D {0.5f, 0.5f, 0.99f},
    MobileRT::Point3D {-0.5f, -0.5f, 0.99f}}, greenMat);

  // sphere - mirror
  const MobileRT::Material MirrorMat {MobileRT::RGB (0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB (0.9f, 0.9f, 0.9f)};
  scene.spheres_.emplace_back (MobileRT::Sphere {
    MobileRT::Point3D {0.45f, -0.65f, 0.4f}, 0.35f}, MirrorMat);

  // sphere - transmission
  const MobileRT::Material TransmissionMat {MobileRT::RGB {0.0f, 0.0f, 0.0f},
                                            MobileRT::RGB {0.0f, 0.0f, 0.0f},
                                            MobileRT::RGB {0.9f, 0.9f, 0.9f}, 1.9f};
  scene.spheres_.emplace_back (MobileRT::Sphere {
    MobileRT::Point3D {-0.4f, -0.3f, 0.0f}, 0.35f}, TransmissionMat);

  // back wall - white
  const MobileRT::Material lightGrayMat {MobileRT::RGB {0.7f, 0.7f, 0.7f}};
  scene.planes_.emplace_back (MobileRT::Plane {
    MobileRT::Point3D {0.0f, 0.0f, 1.0f}, MobileRT::Vector3D {0.0f, 0.0f, -1.0f}}, lightGrayMat);

  // front wall - light blue
  const MobileRT::Material lightBlueMat {MobileRT::RGB (0.0f, 0.9f, 0.9f)};
  scene.planes_.emplace_back (MobileRT::Plane {
    MobileRT::Point3D {0.0f, 0.0f, -4.0f}, MobileRT::Vector3D {0.0f, 0.0f, 1.0f}}, lightBlueMat);

  // floor - white
  scene.planes_.emplace_back (MobileRT::Plane {
    MobileRT::Point3D {0.0f, -1.0f, 0.0f}, MobileRT::Vector3D {0.0f, 1.0f, 0.0f}}, lightGrayMat);
  // ceiling - white
  scene.planes_.emplace_back (MobileRT::Plane {
    MobileRT::Point3D {0.0f, 1.0f, 0.0f}, MobileRT::Vector3D {0.0f, -1.0f, 0.0f}}, lightGrayMat);
  // left wall - red
  const MobileRT::Material redMat {MobileRT::RGB {0.9f, 0.0f, 0.0f}};
  scene.planes_.emplace_back (MobileRT::Plane {
    MobileRT::Point3D {-1.0f, 0.0f, 0.0f}, MobileRT::Vector3D {1.0f, 0.0f, 0.0f}}, redMat);

  // right wall - blue
  const MobileRT::Material blueMat {MobileRT::RGB (0.0f, 0.0f, 0.9f)};
  scene.planes_.emplace_back (MobileRT::Plane {
    MobileRT::Point3D {1.0f, 0.0f, 0.0f}, MobileRT::Vector3D {-1.0f, 0.0f, 0.0f}}, blueMat);

  return std::move(scene);
}

MobileRT::Scene spheresScene(MobileRT::Scene&& scene) noexcept {
  // create one light source
  /*const MobileRT::Material lightMat {MobileRT::RGB {0.0f, 0.0f, 0.0f},
                                     MobileRT::RGB {0.0f, 0.0f, 0.0f},
                                     MobileRT::RGB {0.0f, 0.0f, 0.0f},
                                     1.0f,
                                     MobileRT::RGB {0.9f, 0.9f, 0.9f}};
  scene.lights_.emplace_back (new Components::PointLight {lightMat,
                                                              MobileRT::Point3D {0.0f, 15.0f,
                                                                                 4.0f}});*/

  // create diffuse Materials
  //const MobileRT::Material sandMat {MobileRT::RGB {0.914f, 0.723f, 0.531f}};
  const MobileRT::Material redMat {MobileRT::RGB {0.9f, 0.0f, 0.0f}};
  /*const MobileRT::Material mirrorMat {MobileRT::RGB {0.0f, 0.0f, 0.0f},
                                      MobileRT::RGB {0.9f, 0.9f, 0.9f}};
  const MobileRT::Material greenMat {MobileRT::RGB {0.0f, 0.9f, 0.0f}};*/
  // create one sphere
  scene.spheres_.emplace_back (MobileRT::Sphere {
    MobileRT::Point3D {-1.0f, 1.0f, 6.0f}, 1.0f}, redMat);
  /*scene.spheres_.emplace_back (MobileRT::Sphere {
    MobileRT::Point3D {1.5f, 2.0f, 7.0f}, 1.0f}, mirrorMat);
  scene.spheres_.emplace_back (MobileRT::Sphere {
   MobileRT::Point3D {0.0f, 0.5f, 4.5f}, 0.5f}, greenMat);

  scene.planes_.emplace_back (MobileRT::Plane {
   MobileRT::Point3D {0.0f, 0.0f, 0.0f},
                                  MobileRT::Vector3D {0.0f, 1.0f, 0.0f}}, sandMat);*/
  return std::move(scene);
}

MobileRT::Scene spheresScene2(MobileRT::Scene&& scene) noexcept {
  // create one light source
  const MobileRT::Material lightMat {MobileRT::RGB {0.0f, 0.0f, 0.0f},
                                     MobileRT::RGB {0.0f, 0.0f, 0.0f},
                                     MobileRT::RGB {0.0f, 0.0f, 0.0f},
                                     1.0f,
                                     MobileRT::RGB {0.9f, 0.9f, 0.9f}};
  scene.lights_.emplace_back (new Components::PointLight {lightMat,
                                                          MobileRT::Point3D{0.0f, 15.0f, 4.0f}});

  // create diffuse Materials
  const MobileRT::Material sandMat {MobileRT::RGB {0.914f, 0.723f, 0.531f}};
  const MobileRT::Material redMat {MobileRT::RGB {0.9f, 0.0f, 0.0f}};
  const MobileRT::Material blueMat {MobileRT::RGB {0.0f, 0.0f, 0.9f}};
  const MobileRT::Material yellowMat {MobileRT::RGB {0.9f, 0.9f, 0.0f},
                                      MobileRT::RGB {0.8f, 0.8f, 0.4f}};
  const MobileRT::Material mirrorMat {MobileRT::RGB {0.0f, 0.0f, 0.0f},
                                      MobileRT::RGB {0.9f, 0.9f, 0.9f}};
  const MobileRT::Material greenMat {MobileRT::RGB {0.0f, 0.9f, 0.0f}};
  // create one sphere
  scene.spheres_.emplace_back (MobileRT::Sphere {
    MobileRT::Point3D {-1.0f, 1.0f, 6.0f}, 1.0f}, redMat);
  scene.spheres_.emplace_back (MobileRT::Sphere {
    MobileRT::Point3D {-1.0f, 2.0f, 5.0f}, 0.3f}, blueMat);
  scene.spheres_.emplace_back (MobileRT::Sphere {
    MobileRT::Point3D {1.5f, 2.0f, 7.0f}, 1.0f}, mirrorMat);
  scene.spheres_.emplace_back (MobileRT::Sphere {
    MobileRT::Point3D {1.5f, 0.5f, 5.0f}, 0.2f}, yellowMat);
  scene.spheres_.emplace_back (MobileRT::Sphere {
    MobileRT::Point3D {0.0f, 0.5f, 4.5f}, 0.5f}, greenMat);
  scene.planes_.emplace_back (MobileRT::Plane {
    MobileRT::Point3D {0.0f, 0.0f, 0.0f}, MobileRT::Vector3D {0.0f, 1.0f, 0.0f}}, sandMat);
  return std::move(scene);
}
