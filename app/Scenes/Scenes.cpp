#include "Components/Lights/AreaLight.hpp"
#include "Components/Lights/PointLight.hpp"
#include "Components/Samplers/MersenneTwister.hpp"
#include "Scenes/Scenes.hpp"
#include <glm/glm.hpp>

MobileRT::Scene cornellBoxScene(MobileRT::Scene scene) noexcept {
    // point light - white
    const ::MobileRT::Material lightMat {::glm::vec3 {0.0f, 0.0f, 0.0f},
                                      ::glm::vec3 {0.0f, 0.0f, 0.0f},
                                      ::glm::vec3 {0.0f, 0.0f, 0.0f},
                                      1.0f,
                                      ::glm::vec3 {0.9f, 0.9f, 0.9f}};
    scene.lights_.emplace_back(::std::make_unique<Components::PointLight>(lightMat,
                                                                        ::glm::vec3 {0.0f,
                                                                                           0.99f,
                                                                                           0.0f}));

    // triangle - yellow
    //const ::MobileRT::Material yellowMat {::glm::vec3 {0.9f, 0.9f, 0.0f}};
    scene.triangles_.emplace_back(MobileRT::Triangle {
            ::glm::vec3 {0.5f, -0.5f, 0.99f},
            ::glm::vec3 {0.5f, 0.5f, 1.001f},
            ::glm::vec3 {-0.5f, -0.5f, 0.99f},
            -1,
            ::glm::vec3 {0, 0, -1},
            ::glm::vec3 {0, 0, -1},
            ::glm::vec3 {0, 0, -1},
            ::glm::vec3 {1, 0, 0},
            ::glm::vec3 {0, 1, 0},
            ::glm::vec3 {0, 0, 1}});
    //scene.materials_.emplace_back(yellowMat);

    // sphere - mirror
    /*const ::MobileRT::Material MirrorMat {::glm::vec3 {0.0f, 0.0f, 0.0f},
                                       ::glm::vec3 {0.9f, 0.9f, 0.9f}};
    scene.spheres_.emplace_back(MobileRT::Sphere {
            ::glm::vec3 {0.45f, -0.65f, 0.4f}, 0.35f,
            static_cast<int32_t>(scene.materials_.size())});
    scene.materials_.emplace_back(MirrorMat);

    // sphere - green
    const ::MobileRT::Material GreenMat {::glm::vec3 {0.0f, 0.9f, 0.0f},
                                      ::glm::vec3 {0.0f, 0.2f, 0.0f}};
    scene.spheres_.emplace_back(MobileRT::Sphere {
            ::glm::vec3 {-0.45f, -0.1f, 0.0f}, 0.35f,
            static_cast<int32_t>(scene.materials_.size())});
    scene.materials_.emplace_back(GreenMat);

    // back wall - white
    const ::MobileRT::Material lightGrayMat {::glm::vec3 {0.7f, 0.7f, 0.7f}};
    scene.planes_.emplace_back(MobileRT::Plane {
                                       ::glm::vec3 {0.0f, 0.0f, 1.0f}, ::glm::vec3 {0.0f, 0.0f, -1.0f},
                                       static_cast<int32_t>(scene.materials_.size())});
    scene.materials_.emplace_back(lightGrayMat);*/

    // front wall - light blue
    /*const ::MobileRT::Material &lightBlueMat {::glm::vec3 {0.0f, 0.9f, 0.9f}};
     scene.planes_.emplace_back (MobileRT::Plane {
       ::glm::vec3 {0.0f, 0.0f, -3.5f}, ::glm::vec3 {0.0f, 0.0f, 1.0f}}, lightBlueMat);*/

    // floor - white
    /*scene.planes_.emplace_back(MobileRT::Plane {
                                       ::glm::vec3 {0.0f, -1.0f, 0.0f}, ::glm::vec3 {0.0f, 1.0f, 0.0f},
                                       static_cast<int32_t>(scene.materials_.size())});
    scene.materials_.emplace_back(lightGrayMat);

    // ceiling - white
    scene.planes_.emplace_back(MobileRT::Plane {
                                       ::glm::vec3 {0.0f, 1.0f, 0.0f}, ::glm::vec3 {0.0f, -1.0f, 0.0f},
                                       static_cast<int32_t>(scene.materials_.size())});
    scene.materials_.emplace_back(lightGrayMat);

    // left wall - red
    const ::MobileRT::Material redMat {::glm::vec3 {0.9f, 0.0f, 0.0f}};
    scene.planes_.emplace_back(MobileRT::Plane {
            ::glm::vec3 {-1.0f, 0.0f, 0.0f}, ::glm::vec3 {1.0f, 0.0f, 0.0f},
            static_cast<int32_t>(scene.materials_.size())});
    scene.materials_.emplace_back(redMat);

    // right wall - blue
    const ::MobileRT::Material blueMat {::glm::vec3 {0.0f, 0.0f, 0.9f}};
    scene.planes_.emplace_back(MobileRT::Plane {
            ::glm::vec3 {1.0f, 0.0f, 0.0f}, ::glm::vec3 {-1.0f, 0.0f, 0.0f},
            static_cast<int32_t>(scene.materials_.size())});
    scene.materials_.emplace_back(blueMat);*/

    return scene;
}

MobileRT::Scene cornellBoxScene2(MobileRT::Scene scene) noexcept {
    const ::MobileRT::Material lightMat {::glm::vec3 {0.0f, 0.0f, 0.0f},
                                      ::glm::vec3 {0.0f, 0.0f, 0.0f},
                                      ::glm::vec3 {0.0f, 0.0f, 0.0f},
                                      1.0f,
                                      ::glm::vec3 {0.9f, 0.9f, 0.9f}};
    ::std::unique_ptr<MobileRT::Sampler> samplerPoint1{
            ::std::make_unique<Components::MersenneTwister>()};
    //::std::unique_ptr<MobileRT::Sampler> samplerPoint1 {::std::make_unique<Components::MersenneTwister> ()};
    ::std::unique_ptr<MobileRT::Sampler> samplerPoint2{
            ::std::make_unique<Components::MersenneTwister>()};
    //::std::unique_ptr<MobileRT::Sampler> samplerPoint2 {::std::make_unique<Components::MersenneTwister> ()};

    scene.lights_.emplace_back(::std::make_unique<Components::AreaLight>(lightMat,
            /*scene.lights_.emplace_back(
::std::make_unique<Components::PointLight> (lightMat,
   ::glm::vec3 {0.70f, 0.99f, 0.0f}));*/
                                                                         ::std::move(samplerPoint1),
                                                                         ::glm::vec3 {-0.25f,
                                                                                            0.99f,
                                                                                            -0.25f},
                                                                         ::glm::vec3 {0.25f,
                                                                                            0.99f,
                                                                                            -0.25f},
                                                                         ::glm::vec3 {0.25f,
                                                                                            0.99f,
                                                                                            0.25f}));

    scene.lights_.emplace_back(::std::make_unique<Components::AreaLight>(lightMat,
                                                                         ::std::move(samplerPoint2),
                                                                         ::glm::vec3 {0.25f,
                                                                                            0.99f,
                                                                                            0.25f},
                                                                         ::glm::vec3 {-0.25f,
                                                                                            0.99f,
                                                                                            0.25f},
                                                                         ::glm::vec3 {-0.25f,
                                                                                            0.99f,
                                                                                            -0.25f}));

    // triangle - yellow
    const ::MobileRT::Material yellowMat {::glm::vec3 {0.9f, 0.9f, 0.0f}};
    scene.triangles_.emplace_back(MobileRT::Triangle {
            ::glm::vec3 {0.5f, -0.5f, 0.99f},
            ::glm::vec3 {0.5f, 0.5f, 1.001f},
            ::glm::vec3 {-0.5f, -0.5f, 0.99f},
            static_cast<int32_t>(scene.materials_.size())});
    scene.materials_.emplace_back(yellowMat);

    // triangle - green
    //const ::MobileRT::Material greenMat {::glm::vec3 {0.0f, 0.9f, 0.0f}};
    const ::MobileRT::Material MirrorMat {::glm::vec3 {0.0f, 0.0f, 0.0f},
                                       ::glm::vec3 {0.9f, 0.9f, 0.9f}};
    scene.triangles_.emplace_back(MobileRT::Triangle {
            ::glm::vec3 {-0.5f, 0.5f, 0.99f},
            ::glm::vec3 {-0.5f, -0.5f, 0.99f},
            ::glm::vec3 {0.5f, 0.5f, 0.99f},
            static_cast<int32_t>(scene.materials_.size()),
            ::glm::vec3 {-1, 1, -1},
            ::glm::vec3 {-1, -1, -1},
            ::glm::vec3 {1, 1, -1},
            ::glm::vec3 {1, 1, 1},
            ::glm::vec3 {1, 1, 1},
            ::glm::vec3 {1, 1, 1}});
    scene.materials_.emplace_back(MirrorMat);

    // sphere - mirror
    scene.spheres_.emplace_back(MobileRT::Sphere {
            ::glm::vec3 {0.45f, -0.65f, 0.4f}, 0.35f,
            static_cast<int32_t>(scene.materials_.size())});
    scene.materials_.emplace_back(MirrorMat);

    // sphere - transmission
    const ::MobileRT::Material TransmissionMat {::glm::vec3 {0.0f, 0.0f, 0.0f},
                                             ::glm::vec3 {0.0f, 0.0f, 0.0f},
                                             ::glm::vec3 {0.9f, 0.9f, 0.9f}, 1.9f};
    scene.spheres_.emplace_back(MobileRT::Sphere {
            ::glm::vec3 {-0.4f, -0.3f, 0.0f}, 0.35f,
            static_cast<int32_t>(scene.materials_.size())});
    scene.materials_.emplace_back(TransmissionMat);

    // back wall - white
    const ::MobileRT::Material lightGrayMat {::glm::vec3 {0.7f, 0.7f, 0.7f}};
    scene.planes_.emplace_back(MobileRT::Plane {
                                       ::glm::vec3 {0.0f, 0.0f, 1.0f}, ::glm::vec3 {0.0f, 0.0f, -1.0f},
                                       static_cast<int32_t>(scene.materials_.size())});
    scene.materials_.emplace_back(lightGrayMat);

    // front wall - light blue
    const ::MobileRT::Material lightBlueMat {::glm::vec3 {0.0f, 0.9f, 0.9f}};
    scene.planes_.emplace_back(MobileRT::Plane {
                                       ::glm::vec3 {0.0f, 0.0f, -4.0f}, ::glm::vec3 {0.0f, 0.0f, 1.0f},
                                       static_cast<int32_t>(scene.materials_.size())});
    scene.materials_.emplace_back(lightBlueMat);

    // floor - white
    scene.planes_.emplace_back(MobileRT::Plane {
                                       ::glm::vec3 {0.0f, -1.0f, 0.0f}, ::glm::vec3 {0.0f, 1.0f, 0.0f},
                                       static_cast<int32_t>(scene.materials_.size())});
    scene.materials_.emplace_back(lightGrayMat);

    // ceiling - white
    scene.planes_.emplace_back(MobileRT::Plane {
                                       ::glm::vec3 {0.0f, 1.0f, 0.0f}, ::glm::vec3 {0.0f, -1.0f, 0.0f},
                                       static_cast<int32_t>(scene.materials_.size())});
    scene.materials_.emplace_back(lightGrayMat);

    // left wall - red
    const ::MobileRT::Material redMat {::glm::vec3 {0.9f, 0.0f, 0.0f}};
    scene.planes_.emplace_back(MobileRT::Plane {
            ::glm::vec3 {-1.0f, 0.0f, 0.0f}, ::glm::vec3 {1.0f, 0.0f, 0.0f},
            static_cast<int32_t>(scene.materials_.size())});
    scene.materials_.emplace_back(redMat);

    // right wall - blue
    const ::MobileRT::Material blueMat {::glm::vec3 {0.0f, 0.0f, 0.9f}};
    scene.planes_.emplace_back(MobileRT::Plane {
            ::glm::vec3 {1.0f, 0.0f, 0.0f}, ::glm::vec3 {-1.0f, 0.0f, 0.0f},
            static_cast<int32_t>(scene.materials_.size())});
    scene.materials_.emplace_back(blueMat);

    return scene;
}

MobileRT::Scene spheresScene(MobileRT::Scene scene) noexcept {
    // create one light source
    /*const ::MobileRT::Material &lightMat {::glm::vec3 {0.0f, 0.0f, 0.0f},
                                       ::glm::vec3 {0.0f, 0.0f, 0.0f},
                                       ::glm::vec3 {0.0f, 0.0f, 0.0f},
                                       1.0f,
                                       ::glm::vec3 {0.9f, 0.9f, 0.9f}};
    scene.lights_.emplace_back (::std::make_unique<Components::PointLight> (lightMat,
                                                                ::glm::vec3 {0.0f, 15.0f,
                                                                                   4.0f}));*/

    // create diffuse Materials
    const ::MobileRT::Material sandMat{::glm::vec3 {0.914f, 0.723f, 0.531f}};
    const ::MobileRT::Material redMat {::glm::vec3 {0.9f, 0.0f, 0.0f}};
    /*const ::MobileRT::Material &mirrorMat {::glm::vec3 {0.0f, 0.0f, 0.0f},
                                        ::glm::vec3 {0.9f, 0.9f, 0.9f}};*/
    //const ::MobileRT::Material &greenMat {::glm::vec3 {0.0f, 0.9f, 0.0f}};
    // create one sphere
    scene.spheres_.emplace_back(MobileRT::Sphere {
            ::glm::vec3 {4.0f, 4.0f, 4.0f}, 4.0f,
            static_cast<int32_t>(scene.materials_.size())});
    scene.materials_.emplace_back(redMat);

    scene.triangles_.emplace_back (MobileRT::Triangle {
            ::glm::vec3 {0.0f, 10.0f, 10.0f},
            ::glm::vec3 {0.0f, 0.0f, 10.0f},
            ::glm::vec3 {10.0f, 0.0f, 10.0f},
            static_cast<int32_t>(scene.materials_.size())});
    scene.materials_.emplace_back(sandMat);
    /*scene.triangles_.emplace_back (MobileRT::Triangle {
      ::glm::vec3 {10.0f, 10.0f, 10.0f},
      ::glm::vec3 {10.0f, 0.0f, 10.0f},
      ::glm::vec3 {0.0f, 10.0f, 10.0f}}, greenMat);*/
    /*scene.spheres_.emplace_back (MobileRT::Sphere {
      ::glm::vec3 {-1.0f, 1.0f, 6.0f}, 1.0f}, redMat);*/
    /*scene.spheres_.emplace_back (MobileRT::Sphere {
      ::glm::vec3 {1.5f, 2.0f, 7.0f}, 1.0f}, mirrorMat);
    scene.spheres_.emplace_back (MobileRT::Sphere {
     ::glm::vec3 {0.0f, 0.5f, 4.5f}, 0.5f}, greenMat);

    scene.planes_.emplace_back (MobileRT::Plane {
     ::glm::vec3 {0.0f, 0.0f, 0.0f},
                                    ::glm::vec3 {0.0f, 1.0f, 0.0f}}, sandMat);*/
    return scene;
}

MobileRT::Scene spheresScene2(MobileRT::Scene scene) noexcept {
    // create one light source
    const ::MobileRT::Material lightMat {::glm::vec3 {0.0f, 0.0f, 0.0f},
                                      ::glm::vec3 {0.0f, 0.0f, 0.0f},
                                      ::glm::vec3 {0.0f, 0.0f, 0.0f},
                                      1.0f,
                                      ::glm::vec3 {0.9f, 0.9f, 0.9f}};
    scene.lights_.emplace_back(::std::make_unique<Components::PointLight>(lightMat,
                                                                          ::glm::vec3 {0.0f,
                                                                                            15.0f,
                                                                                            4.0f}));

    // create diffuse Materials
    const ::MobileRT::Material sandMat {::glm::vec3 {0.914f, 0.723f, 0.531f}};
    const ::MobileRT::Material redMat {::glm::vec3 {0.9f, 0.0f, 0.0f}};
    const ::MobileRT::Material blueMat {::glm::vec3 {0.0f, 0.0f, 0.9f}};
    const ::MobileRT::Material yellowMat {::glm::vec3 {0.9f, 0.9f, 0.0f},
                                       ::glm::vec3 {0.8f, 0.8f, 0.4f}};
    const ::MobileRT::Material mirrorMat {::glm::vec3 {0.2f, 0.2f, 0.2f},
                                       ::glm::vec3 {0.9f, 0.9f, 0.9f}};
    const ::MobileRT::Material greenMat {::glm::vec3 {0.0f, 0.9f, 0.0f}};
    // create one sphere
    scene.spheres_.emplace_back(MobileRT::Sphere {
            ::glm::vec3 {-1.0f, 1.0f, 6.0f}, 1.0f,
            static_cast<int32_t>(scene.materials_.size())});
    scene.materials_.emplace_back(redMat);

    scene.spheres_.emplace_back(MobileRT::Sphere {
            ::glm::vec3 {-0.5f, 2.0f, 5.0f}, 0.3f,
            static_cast<int32_t>(scene.materials_.size())});
    scene.materials_.emplace_back(blueMat);

    scene.spheres_.emplace_back(MobileRT::Sphere {
            ::glm::vec3 {0.0f, 2.0f, 7.0f}, 1.0f,
            static_cast<int32_t>(scene.materials_.size())});
    scene.materials_.emplace_back(mirrorMat);

    scene.spheres_.emplace_back(MobileRT::Sphere {
            ::glm::vec3 {0.5f, 0.5f, 5.0f}, 0.2f,
            static_cast<int32_t>(scene.materials_.size())});
    scene.materials_.emplace_back(yellowMat);

    scene.spheres_.emplace_back(MobileRT::Sphere {
            ::glm::vec3 {1.0f, 0.5f, 4.5f}, 0.5f,
            static_cast<int32_t>(scene.materials_.size())});
    scene.materials_.emplace_back(greenMat);

    scene.planes_.emplace_back(MobileRT::Plane {
            ::glm::vec3 {0.0f, 0.0f, 0.0f}, ::glm::vec3 {0.0f, 1.0f, 0.0f},
            static_cast<int32_t>(scene.materials_.size())});
    scene.materials_.emplace_back(sandMat);
    
    return scene;
}
