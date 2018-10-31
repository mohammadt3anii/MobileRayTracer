#include "c_wrapper.h"
#include "Components/Cameras/Orthographic.hpp"
#include "Components/Cameras/Perspective.hpp"
#include "Components/Lights/AreaLight.hpp"
#include "Components/Lights/PointLight.hpp"
#include "Components/ObjectLoaders/OBJLoader.hpp"
#include "Components/Samplers/Constant.hpp"
#include "Components/Samplers/HaltonSeq.hpp"
#include "Components/Samplers/MersenneTwister.hpp"
#include "Components/Samplers/StaticHaltonSeq.hpp"
#include "Components/Samplers/StaticMersenneTwister.hpp"
#include "Components/Samplers/Stratified.hpp"
#include "Components/Shaders/DepthMap.hpp"
#include "Components/Shaders/DiffuseMaterial.hpp"
#include "Components/Shaders/NoShadows.hpp"
#include "Components/Shaders/PathTracer.hpp"
#include "Components/Shaders/Whitted.hpp"
#include "MobileRT/Renderer.hpp"
#include "MobileRT/Scene.hpp"
#include "Scenes.hpp"
#include <chrono>
#include <cstring>
#include <fstream>

static void
work_thread(
    ::std::uint32_t *const bitmap, const ::std::int32_t width,
    const ::std::int32_t height, const ::std::int32_t threads,
    const ::std::int32_t nShader, const ::std::int32_t nScene, const ::std::int32_t samplesPixel, const ::std::int32_t samplesLight,
    ::std::int32_t repeats, const ::std::int32_t accelerator, const bool printStdOut,
    const char *const objFileName, const char *const mtlFileName) {
    ::std::ostringstream ss{""};
    ::std::streambuf *old_buf_stdout{nullptr};
    ::std::streambuf *old_buf_stderr{nullptr};
    ::std::chrono::duration<double> timeCreating{0};
    ::std::chrono::duration<double> timeRendering{0};
    if (!printStdOut) {
        old_buf_stdout = ::std::cout.rdbuf(ss.rdbuf());
        old_buf_stderr = ::std::cerr.rdbuf(ss.rdbuf());
    }
    {
        LOG("width = ", width);
        LOG("height = ", height);
        LOG("threads = ", threads);
        LOG("nShader = ", nShader);
        LOG("nScene = ", nScene);
        LOG("samplesPixel = ", samplesPixel);
        LOG("samplesLight = ", samplesLight);
        LOG("repeats = ", repeats);
        LOG("accelerator = ", accelerator);
        LOG("printStdOut = ", printStdOut);
        LOG("pathObj = ", objFileName);
        LOG("pathMtl = ", mtlFileName);

        ::std::unique_ptr<::MobileRT::Renderer> renderer_ {};
        ::std::int32_t numberOfLights_ {0};

        const float ratio {
                ::std::max(static_cast<float> (width) / height,
                           static_cast<float> (height) / width)};
        const float hfovFactor {width > height ? ratio : 1.0f};
        const float vfovFactor {width < height ? ratio : 1.0f};
        ::MobileRT::Scene scene {};
        ::std::unique_ptr<::MobileRT::Sampler> samplerPixel {};
        ::std::unique_ptr<::MobileRT::Shader> shader {};
        ::std::unique_ptr<::MobileRT::Camera> camera {
                ::std::make_unique<::Components::Perspective>(
                        ::glm::vec3 {0.0f, 0.0f, -3.4f},
                        ::glm::vec3 {0.0f, 0.0f, 1.0f},
                        ::glm::vec3 {0.0f, 1.0f, 0.0f},
                        45.0f * hfovFactor, 45.0f * vfovFactor)};
        ::glm::vec3 maxDist{};

        switch (nScene) {
            case 0:
                camera = ::std::make_unique<::Components::Perspective>(
                        ::glm::vec3 {0.0f, 0.0f, -3.4f},
                        ::glm::vec3 {0.0f, 0.0f, 1.0f},
                        ::glm::vec3 {0.0f, 1.0f, 0.0f},
                        45.0f * hfovFactor, 45.0f * vfovFactor);
                scene = cornellBoxScene(::std::move(scene));
                maxDist = ::glm::vec3 {1, 1, 1};
                break;

            case 1:
                camera = ::std::make_unique<::Components::Orthographic>(
                        ::glm::vec3 {0.0f, 1.0f, -10.0f},
                        ::glm::vec3 {0.0f, 1.0f, 7.0f},
                        ::glm::vec3 {0.0f, 1.0f, 0.0f},
                        10.0f * hfovFactor, 10.0f * vfovFactor);
                /*camera = ::std::make_unique<::Components::Perspective> (
                  ::glm::vec3 {0.0f, 0.5f, 1.0f},
                  ::glm::vec3 {0.0f, 0.0f, 7.0f},
                  ::glm::vec3 {0.0f, 1.0f, 0.0f},
                  60.0f * hfovFactor, 60.0f * vfovFactor);*/
                scene = spheresScene(::std::move(scene));
                maxDist = ::glm::vec3 {8, 8, 8};
                break;

            case 2:
                camera = ::std::make_unique<::Components::Perspective>(
                        ::glm::vec3 {0.0f, 0.0f, -3.4f},
                        ::glm::vec3 {0.0f, 0.0f, 1.0f},
                        ::glm::vec3 {0.0f, 1.0f, 0.0f},
                        45.0f * hfovFactor, 45.0f * vfovFactor);
                scene = cornellBoxScene2(::std::move(scene));
                maxDist = ::glm::vec3 {1, 1, 1};
                break;

            case 3:
                camera = ::std::make_unique<::Components::Perspective>(
                        ::glm::vec3 {0.0f, 0.5f, 1.0f},
                        ::glm::vec3 {0.0f, 0.0f, 7.0f},
                        ::glm::vec3 {0.0f, 1.0f, 0.0f},
                        60.0f * hfovFactor, 60.0f * vfovFactor);
                scene = spheresScene2(::std::move(scene));
                maxDist = ::glm::vec3 {8, 8, 8};
                break;
            default: {
                ::Components::OBJLoader objLoader {objFileName, mtlFileName};
                objLoader.process();
                if (!objLoader.isProcessed()) {
                    exit(0);
                }
                //objLoader.fillScene (&scene, []() { return ::std::make_unique<::Components::HaltonSeq> (); });
                //objLoader.fillScene (&scene, []() {return ::std::make_unique<::Components::MersenneTwister> (); });
                objLoader.fillScene(&scene,
                                    []() { return ::std::make_unique<Components::StaticHaltonSeq>(); });
                //objLoader.fillScene(&scene, []() { return ::std::make_unique<Components::StaticMersenneTwister> (); });

                const float fovX{45.0f * hfovFactor};
                const float fovY{45.0f * vfovFactor};
                maxDist = ::glm::vec3 {1, 1, 1};
                const ::MobileRT::Material lightMat {::glm::vec3 {0.0f, 0.0f, 0.0f},
                                                     ::glm::vec3 {0.0f, 0.0f, 0.0f},
                                                     ::glm::vec3 {0.0f, 0.0f, 0.0f},
                                                     1.0f,
                                                     ::glm::vec3 {0.9f, 0.9f, 0.9f}};

                //cornellbox
                if (::std::strstr(objFileName, "CornellBox") != nullptr) {
                    camera = ::std::make_unique<Components::Perspective>(
                            ::glm::vec3 {0.0f, 0.7f, 3.0f},
                            ::glm::vec3 {0.0f, 0.7f, -1.0f},
                            ::glm::vec3 {0.0f, 1.0f, 0.0f},
                            fovX, fovY);

                    ::std::unique_ptr<MobileRT::Sampler> samplerPoint1{
                            ::std::make_unique<Components::StaticHaltonSeq>()};
                    scene.lights_.emplace_back(::std::make_unique<::Components::AreaLight>(
                            lightMat,
                            ::std::move(samplerPoint1),
                            ::glm::vec3 {0.5f, 1.58f, 0.5f},
                            ::glm::vec3 {-0.5f, 1.58f, 0.5f},
                            ::glm::vec3 {-0.5f, 1.58f, -0.5f}));
                    ::std::unique_ptr<MobileRT::Sampler> samplerPoint2{
                            ::std::make_unique<Components::StaticHaltonSeq>()};
                    scene.lights_.emplace_back(::std::make_unique<::Components::AreaLight>(
                            lightMat,
                            ::std::move(samplerPoint2),
                            ::glm::vec3 {0.5f, 1.58f, 0.5f},
                            ::glm::vec3 {-0.5f, 1.58f, -0.5f},
                            ::glm::vec3 {0.5f, 1.58f, -0.5f}));
                }
                
                //conference
                if (::std::strstr(objFileName, "conference") != nullptr) {
                    ::std::unique_ptr<MobileRT::Sampler> samplerPoint1{
                            ::std::make_unique<Components::StaticHaltonSeq>()};
                    scene.lights_.emplace_back(::std::make_unique<::Components::AreaLight>(
                            lightMat,
                            ::std::move(samplerPoint1),
                            ::glm::vec3 {-100.0f, 640.0f, -100.0f},
                            ::glm::vec3 {100.0f, 640.0f, -100.0f},
                            ::glm::vec3 {100.0f, 640.0f, 100.0f}));
                    ::std::unique_ptr<MobileRT::Sampler> samplerPoint2{
                            ::std::make_unique<Components::StaticHaltonSeq>()};
                    scene.lights_.emplace_back(::std::make_unique<::Components::AreaLight>(
                            lightMat,
                            ::std::move(samplerPoint2),
                            ::glm::vec3 {-100.0f, 640.0f, -100.0f},
                            ::glm::vec3 {100.0f, 640.0f, 100.0f},
                            ::glm::vec3 {-100.0f, 640.0f, 100.0f}));
                    camera = ::std::make_unique<::Components::Perspective>(
                            ::glm::vec3 {460.0f, 500.0f, -1000.0f},
                            ::glm::vec3 {0.0f, 400.0f, 0.0f},
                            ::glm::vec3 {0.0f, 1.0f, 0.0f},
                            fovX, fovY);
                }

                //teapot
                if (::std::strstr(objFileName, "teapot") != nullptr) {
                    camera = ::std::make_unique<::Components::Perspective>(
                            ::glm::vec3 {0.0f, 30.0f, -200.0f},
                            ::glm::vec3 {0.0f, 30.0f, 100.0f},
                            ::glm::vec3 {0.0f, 1.0f, 0.0f},
                            fovX, fovY);
                    ::std::unique_ptr<MobileRT::Sampler> samplerPoint1{
                            ::std::make_unique<Components::StaticHaltonSeq>()};
                    scene.lights_.emplace_back(::std::make_unique<::Components::AreaLight>(
                            lightMat,
                            ::std::move(samplerPoint1),
                            ::glm::vec3 {-30.0f, 100.0f, -30.0f},
                            ::glm::vec3 {30.0f, 100.0f, -30.0f},
                            ::glm::vec3 {30.0f, 100.0f, 30.0f}));
                    ::std::unique_ptr<MobileRT::Sampler> samplerPoint2{
                            ::std::make_unique<Components::StaticHaltonSeq>()};
                    scene.lights_.emplace_back(::std::make_unique<::Components::AreaLight>(
                            lightMat,
                            ::std::move(samplerPoint2),
                            ::glm::vec3 {-30.0f, 100.0f, -30.0f},
                            ::glm::vec3 {30.0f, 100.0f, 30.0f},
                            ::glm::vec3 {-30.0f, 100.0f, 30.0f}));
                }

                //dragon
                if (::std::strstr(objFileName, "dragon") != nullptr) {
                    camera = ::std::make_unique<::Components::Perspective>(
                            ::glm::vec3 {0.0f, 0.0f, -1.5f},
                            ::glm::vec3 {0.0f, 0.0f, 0.0f},
                            ::glm::vec3 {0.0f, 1.0f, 0.0f},
                            fovX, fovY);
                    ::std::unique_ptr<MobileRT::Sampler> samplerPoint1{
                            ::std::make_unique<Components::StaticHaltonSeq>()};
                    scene.lights_.emplace_back(::std::make_unique<::Components::AreaLight>(
                            lightMat,
                            ::std::move(samplerPoint1),
                            ::glm::vec3 {-0.3f, 1.0f, -0.3f},
                            ::glm::vec3 {0.3f, 1.0f, -0.3f},
                            ::glm::vec3 {0.3f, 1.0f, 0.3f}));
                    ::std::unique_ptr<MobileRT::Sampler> samplerPoint2{
                            ::std::make_unique<Components::StaticHaltonSeq>()};
                    scene.lights_.emplace_back(::std::make_unique<::Components::AreaLight>(
                            lightMat,
                            ::std::move(samplerPoint2),
                            ::glm::vec3 {-0.3f, 1.0f, -0.3f},
                            ::glm::vec3 {0.3f, 1.0f, 0.3f},
                            ::glm::vec3 {-0.3f, 1.0f, 0.3f}));
                }

                //bedroom
                if (::std::strstr(objFileName, "bedroom") != nullptr) {
                    camera = ::std::make_unique<::Components::Perspective>(
                            ::glm::vec3 {0.0f, 0.0f, -2.5f},
                            ::glm::vec3 {0.0f, 0.0f, 0.0f},
                            ::glm::vec3 {0.0f, 1.0f, 0.0f},
                            fovX, fovY);
                    ::std::unique_ptr<MobileRT::Sampler> samplerPoint1{
                            ::std::make_unique<Components::StaticHaltonSeq>()};
                    scene.lights_.emplace_back(::std::make_unique<::Components::AreaLight>(
                            lightMat,
                            ::std::move(samplerPoint1),
                            ::glm::vec3 {-0.3f, 1.0f, -0.3f},
                            ::glm::vec3 {0.3f, 1.0f, -0.3f},
                            ::glm::vec3 {0.3f, 1.0f, 0.3f}));
                    ::std::unique_ptr<MobileRT::Sampler> samplerPoint2{
                            ::std::make_unique<Components::StaticHaltonSeq>()};
                    scene.lights_.emplace_back(::std::make_unique<::Components::AreaLight>(
                            lightMat,
                            ::std::move(samplerPoint2),
                            ::glm::vec3 {-0.3f, 1.0f, -0.3f},
                            ::glm::vec3 {0.3f, 1.0f, 0.3f},
                            ::glm::vec3 {-0.3f, 1.0f, 0.3f}));
                }

                //breakfast_room
                if (::std::strstr(objFileName, "breakfast_room") != nullptr) {
                    camera = ::std::make_unique<::Components::Perspective>(
                            ::glm::vec3 {0.0f, 0.0f, -5.0f},
                            ::glm::vec3 {0.0f, 0.0f, 0.0f},
                            ::glm::vec3 {0.0f, 1.0f, 0.0f},
                            fovX, fovY);
                    ::std::unique_ptr<MobileRT::Sampler> samplerPoint1{
                            ::std::make_unique<Components::StaticHaltonSeq>()};
                    scene.lights_.emplace_back(::std::make_unique<::Components::AreaLight>(
                            lightMat,
                            ::std::move(samplerPoint1),
                            ::glm::vec3 {-0.3f, 1.0f, -0.3f},
                            ::glm::vec3 {0.3f, 1.0f, -0.3f},
                            ::glm::vec3 {0.3f, 1.0f, 0.3f}));
                    ::std::unique_ptr<MobileRT::Sampler> samplerPoint2{
                            ::std::make_unique<Components::StaticHaltonSeq>()};
                    scene.lights_.emplace_back(::std::make_unique<::Components::AreaLight>(
                            lightMat,
                            ::std::move(samplerPoint2),
                            ::glm::vec3 {-0.3f, 1.0f, -0.3f},
                            ::glm::vec3 {0.3f, 1.0f, 0.3f},
                            ::glm::vec3 {-0.3f, 1.0f, 0.3f}));
                }

                //buddha
                if (::std::strstr(objFileName, "buddha") != nullptr) {
                    ::std::unique_ptr<MobileRT::Sampler> samplerPoint1{
                            ::std::make_unique<Components::StaticHaltonSeq>()};
                    scene.lights_.emplace_back(::std::make_unique<::Components::AreaLight>(
                            lightMat,
                            ::std::move(samplerPoint1),
                            ::glm::vec3 {-0.3f, 1.0f, -0.3f},
                            ::glm::vec3 {0.3f, 1.0f, -0.3f},
                            ::glm::vec3 {0.3f, 1.0f, 0.3f}));
                    ::std::unique_ptr<MobileRT::Sampler> samplerPoint2{
                            ::std::make_unique<Components::StaticHaltonSeq>()};
                    scene.lights_.emplace_back(::std::make_unique<::Components::AreaLight>(
                            lightMat,
                            ::std::move(samplerPoint2),
                            ::glm::vec3 {-0.3f, 1.0f, -0.3f},
                            ::glm::vec3 {0.3f, 1.0f, 0.3f},
                            ::glm::vec3 {-0.3f, 1.0f, 0.3f}));

                    camera = ::std::make_unique<::Components::Perspective>(
                            ::glm::vec3 {0.0f, 0.0f, -2.5f},
                            ::glm::vec3 {0.0f, 0.0f, 0.0f},
                            ::glm::vec3 {0.0f, 1.0f, 0.0f},
                            fovX, fovY);
                }

                //erato
                if (::std::strstr(objFileName, "erato") != nullptr) {
                    ::std::unique_ptr<MobileRT::Sampler> samplerPoint1{
                            ::std::make_unique<Components::StaticHaltonSeq>()};
                    scene.lights_.emplace_back(::std::make_unique<::Components::AreaLight>(
                            lightMat,
                            ::std::move(samplerPoint1),
                            ::glm::vec3 {-0.3f, 1.0f, -0.3f},
                            ::glm::vec3 {0.3f, 1.0f, -0.3f},
                            ::glm::vec3 {0.3f, 1.0f, 0.3f}));
                    ::std::unique_ptr<MobileRT::Sampler> samplerPoint2{
                            ::std::make_unique<Components::StaticHaltonSeq>()};
                    scene.lights_.emplace_back(::std::make_unique<::Components::AreaLight>(
                            lightMat,
                            ::std::move(samplerPoint2),
                            ::glm::vec3 {-0.3f, 1.0f, -0.3f},
                            ::glm::vec3 {0.3f, 1.0f, 0.3f},
                            ::glm::vec3 {-0.3f, 1.0f, 0.3f}));

                    camera = ::std::make_unique<::Components::Perspective>(
                            ::glm::vec3 {0.0f, 0.0f, -2.5f},
                            ::glm::vec3 {0.0f, 0.0f, 0.0f},
                            ::glm::vec3 {0.0f, 1.0f, 0.0f},
                            fovX, fovY);
                }

                //gallery
                if (::std::strstr(objFileName, "gallery") != nullptr) {
                    ::std::unique_ptr<MobileRT::Sampler> samplerPoint1{
                            ::std::make_unique<Components::StaticHaltonSeq>()};
                    scene.lights_.emplace_back(::std::make_unique<::Components::AreaLight>(
                            lightMat,
                            ::std::move(samplerPoint1),
                            ::glm::vec3 {-0.3f, 1.0f, -0.3f},
                            ::glm::vec3 {0.3f, 1.0f, -0.3f},
                            ::glm::vec3 {0.3f, 1.0f, 0.3f}));
                    ::std::unique_ptr<MobileRT::Sampler> samplerPoint2{
                            ::std::make_unique<Components::StaticHaltonSeq>()};
                    scene.lights_.emplace_back(::std::make_unique<::Components::AreaLight>(
                            lightMat,
                            ::std::move(samplerPoint2),
                            ::glm::vec3 {-0.3f, 1.0f, -0.3f},
                            ::glm::vec3 {0.3f, 1.0f, 0.3f},
                            ::glm::vec3 {-0.3f, 1.0f, 0.3f}));

                    camera = ::std::make_unique<::Components::Perspective>(
                            ::glm::vec3 {0.0f, 0.0f, -2.5f},
                            ::glm::vec3 {0.0f, 0.0f, 0.0f},
                            ::glm::vec3 {0.0f, 1.0f, 0.0f},
                            fovX, fovY);
                }

                //Porsche
                if (::std::strstr(objFileName, "Porsche") != nullptr) {
                    ::std::unique_ptr<MobileRT::Sampler> samplerPoint1{
                            ::std::make_unique<Components::StaticHaltonSeq>()};
                    scene.lights_.emplace_back(::std::make_unique<::Components::AreaLight>(
                            lightMat,
                            ::std::move(samplerPoint1),
                            ::glm::vec3 {-1.0f, 2.1f, 1.0f},
                            ::glm::vec3 {1.0f, 2.1f, -1.0f},
                            ::glm::vec3 {1.0f, 2.1f, 1.0f}));
                    ::std::unique_ptr<MobileRT::Sampler> samplerPoint2{
                            ::std::make_unique<Components::StaticHaltonSeq>()};
                    scene.lights_.emplace_back(::std::make_unique<::Components::AreaLight>(
                            lightMat,
                            ::std::move(samplerPoint2),
                            ::glm::vec3 {-1.0f, 2.1f, 1.0f},
                            ::glm::vec3 {-1.0f, 2.1f, -1.0f},
                            ::glm::vec3 {1.0f, 2.1f, -1.0f}));

                    const ::MobileRT::Material planeMaterialBack{::glm::vec3 {0.7f, 0.7f, 0.7f}};
                    const ::glm::vec3 planePointDown{0.0f, -1.0f, 0.0f};
                    const ::glm::vec3 planeNormalDown{0.0f, 1.0f, 0.0f};
                    const ::MobileRT::Plane planeDown{planePointDown, planeNormalDown};
                    const ::MobileRT::Primitive<::MobileRT::Plane> planePrimitiveDown{planeDown,
                                                                                      planeMaterialBack};
                    scene.planes_.emplace_back(planePrimitiveDown);

                    const ::glm::vec3 planePointUp{0.0f, 2.2f, 0.0f};
                    const ::glm::vec3 planeNormalUp{0.0f, -1.0f, 0.0f};
                    const ::MobileRT::Plane planeUp{planePointUp, planeNormalUp};
                    const ::MobileRT::Primitive<::MobileRT::Plane> planePrimitiveUp{planeUp,
                                                                                    planeMaterialBack};
                    scene.planes_.emplace_back(planePrimitiveUp);

                    const ::MobileRT::Material planeMaterialLeft{::glm::vec3 {0.9f, 0.0f, 0.0f}};
                    const ::glm::vec3 planePointLeft{-4.1f, 0.0f, 0.0f};
                    const ::glm::vec3 planeNormalLeft{1.0f, 0.0f, 0.0f};
                    const ::MobileRT::Plane planeLeft{planePointLeft, planeNormalLeft};
                    const ::MobileRT::Primitive<::MobileRT::Plane> planePrimitiveLeft{planeLeft,
                                                                                      planeMaterialLeft};
                    scene.planes_.emplace_back(planePrimitiveLeft);

                    const ::MobileRT::Material planeMaterialRight{::glm::vec3 {0.0f, 0.0f, 0.9f}};
                    const ::glm::vec3 planePointRight{1.1f, 0.0f, 0.0f};
                    const ::glm::vec3 planeNormalRight{-1.0f, 0.0f, 0.0f};
                    const ::MobileRT::Plane planeRight{planePointRight, planeNormalRight};
                    const ::MobileRT::Primitive<::MobileRT::Plane> planePrimitiveRight{planeRight,
                                                                                       planeMaterialRight};
                    scene.planes_.emplace_back(planePrimitiveRight);

                    const ::glm::vec3 planePointBack{0.0f, 0.0f, -4.6f};
                    const ::glm::vec3 planeNormalBack{0.0f, 0.0f, 1.0f};
                    const ::MobileRT::Plane planeBack{planePointBack, planeNormalBack};
                    const ::MobileRT::Primitive<::MobileRT::Plane> planePrimitiveBack{planeBack,
                                                                                      planeMaterialBack};
                    scene.planes_.emplace_back(planePrimitiveBack);

                    const ::MobileRT::Material planeMaterialForward{::glm::vec3 {0.0f, 0.9f, 0.9f}};
                    const ::glm::vec3 planePointForward{0.0f, 0.0f, 2.3f};
                    const ::glm::vec3 planeNormalForward{0.0f, 0.0f, -1.0f};
                    const ::MobileRT::Plane planeForward{planePointForward, planeNormalForward};
                    const ::MobileRT::Primitive<::MobileRT::Plane> planePrimitiveForward{
                            planeForward, planeMaterialForward};
                    scene.planes_.emplace_back(planePrimitiveForward);

                    camera = ::std::make_unique<::Components::Perspective>(
                            ::glm::vec3 {-4.0f, 2.0f, -4.5f},
                            ::glm::vec3 {0.0f, 0.0f, 0.0f},
                            ::glm::vec3 {0.0f, 1.0f, 0.0f},
                            fovX, fovY);
                }
            }
                break;
        }
        if (samplesPixel > 1) {
            //samplerPixel = ::std::make_unique<::Components::HaltonSeq> ();
            //samplerPixel = ::std::make_unique<::Components::Stratified> ();
            samplerPixel = ::std::make_unique<::Components::StaticHaltonSeq>();
        } else {
            samplerPixel = ::std::make_unique<::Components::Constant>(0.5f);
        }
        switch (nShader) {
            case 1: {
                shader = ::std::make_unique<::Components::Whitted>(::std::move(scene),
                                                                   samplesLight,
                                                                   ::MobileRT::Shader::Accelerator(
                                                                            accelerator));
                break;
            }

            case 2: {
                //::std::unique_ptr<::MobileRT::Sampler> samplerRussianRoulette {::std::make_unique<::Components::HaltonSeq> ()};
                //::std::unique_ptr<::MobileRT::Sampler> samplerRussianRoulette {::std::make_unique<::Components::MersenneTwister> ()};
                ::std::unique_ptr<MobileRT::Sampler> samplerRussianRoulette{
                        ::std::make_unique<::Components::StaticHaltonSeq>()};
                //::std::unique_ptr<MobileRT::Sampler> samplerRussianRoulette {::std::make_unique<::Components::StaticMersenneTwister> ()};

                shader = ::std::make_unique<::Components::PathTracer>(
                        ::std::move(scene), ::std::move(samplerRussianRoulette), samplesLight,
                        ::MobileRT::Shader::Accelerator(accelerator));
                break;
            }

            case 3: {
                shader = ::std::make_unique<::Components::DepthMap>(::std::move(scene), maxDist,
                                                                    ::MobileRT::Shader::Accelerator(
                                                                             accelerator));
                break;
            }

            case 4: {
                shader = ::std::make_unique<::Components::DiffuseMaterial>(::std::move(scene),
                                                                           ::MobileRT::Shader::Accelerator(
                                                                                    accelerator));
                break;
            }

            default: {
                shader = ::std::make_unique<::Components::NoShadows>(::std::move(scene),
                                                                     samplesLight,
                                                                     ::MobileRT::Shader::Accelerator(
                                                                              accelerator));
                break;
            }
        }
        const ::std::int32_t triangles{static_cast<::std::int32_t> (shader->scene_.triangles_.size())};
        const ::std::int32_t spheres{static_cast<::std::int32_t> (shader->scene_.spheres_.size())};
        const ::std::int32_t planes{static_cast<::std::int32_t> (shader->scene_.planes_.size())};
        numberOfLights_ = static_cast<::std::int32_t> (shader->scene_.lights_.size());
        const ::std::int32_t nPrimitives = triangles + spheres + planes;

        LOG("Started creating Renderer");
        const auto startCreating{::std::chrono::system_clock::now()};
        renderer_ = ::std::make_unique<::MobileRT::Renderer>(
                ::std::move(shader), ::std::move(camera), ::std::move(samplerPixel),
                static_cast<::std::uint32_t> (width), static_cast<::std::uint32_t> (height),
                static_cast<::std::uint32_t> (samplesPixel));
        const auto endCreating{::std::chrono::system_clock::now()};
        timeCreating = endCreating - startCreating;
        LOG("Renderer created = ", timeCreating.count());

        LOG("TRIANGLES = ", triangles);
        LOG("SPHERES = ", spheres);
        LOG("PLANES = ", planes);
        LOG("PRIMITIVES = ", nPrimitives);
        LOG("LIGHTS = ", numberOfLights_);
        LOG("threads = ", threads);
        LOG("nShader = ", nShader);
        LOG("nScene = ", nScene);
        LOG("samplesPixel = ", samplesPixel);
        LOG("samplesLight = ", samplesLight);
        LOG("width_ = ", width);
        LOG("height_ = ", height);

        LOG("Started rendering scene");
        const auto startRendering{::std::chrono::system_clock::now()};
        do {
            renderer_->renderFrame(bitmap, threads, width * sizeof(::std::uint32_t));
            repeats--;
        } while (repeats > 0);
        const auto endRendering{::std::chrono::system_clock::now()};
        timeRendering = endRendering - startRendering;
        LOG("Finished rendering scene");
    }
    if (!printStdOut) {
        ::std::cout.rdbuf(old_buf_stdout);
        ::std::cerr.rdbuf(old_buf_stderr);
    }

    LOG("Creating Time in secs = ", timeCreating.count());
    LOG("Rendering Time in secs = ", timeRendering.count());
}

void RayTrace(::std::uint32_t *const bitmap, const ::std::int32_t width, const ::std::int32_t height, const ::std::int32_t threads,
              const ::std::int32_t shader, const ::std::int32_t scene, const ::std::int32_t samplesPixel, const ::std::int32_t samplesLight,
              const ::std::int32_t repeats, const ::std::int32_t accelerator, const bool printStdOut, const bool async,
              const char *const pathObj, const char *const pathMtl) {
    if (async) {
        ::std::thread thread {work_thread, bitmap, width, height, threads, shader, scene,
                             samplesPixel, samplesLight, repeats, accelerator, printStdOut, pathObj,
                             pathMtl};
        thread.detach();
    } else {
        work_thread(bitmap, width, height, threads, shader, scene, samplesPixel, samplesLight,
                    repeats, accelerator, printStdOut, pathObj, pathMtl);
    }
}
