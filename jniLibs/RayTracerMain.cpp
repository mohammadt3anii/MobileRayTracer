#include "Components/src/main/cpp/Components/Cameras/Orthographic.hpp"
#include "Components/src/main/cpp/Components/Cameras/Perspective.hpp"
#include "Components/src/main/cpp/Components/Lights/AreaLight.hpp"
#include "Components/src/main/cpp/Components/Lights/PointLight.hpp"
#include "Components/src/main/cpp/Components/Samplers/Constant.hpp"
#include "Components/src/main/cpp/Components/Samplers/HaltonSeq.hpp"
#include "Components/src/main/cpp/Components/Samplers/Random.hpp"
#include "Components/src/main/cpp/Components/Samplers/Stratified.hpp"
#include "Components/src/main/cpp/Components/Shaders/NoShadows.hpp"
#include "Components/src/main/cpp/Components/Shaders/PathTracer.hpp"
#include "Components/src/main/cpp/Components/Shaders/Whitted.hpp"
#include "MobileRT/src/main/cpp/MobileRT/Renderer.hpp"
#include "MobileRT/src/main/cpp/MobileRT/Scene.hpp"
#include "MobileRT/src/main/cpp/MobileRT/Shapes/Plane.hpp"
#include "MobileRT/src/main/cpp/MobileRT/Shapes/Sphere.hpp"
#include "MobileRT/src/main/cpp/MobileRT/Shapes/Triangle.hpp"
#include <cstdint>
#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include <iostream>
#include <omp.h>

static MobileRT::Camera *camera_(nullptr);
static MobileRT::Shader *shader_(nullptr);
static MobileRT::Sampler *samplerCamera_(nullptr);
static MobileRT::Sampler *samplerPixel_(nullptr);
static MobileRT::Sampler *samplerRay_(nullptr);
static MobileRT::Sampler *samplerLight_(nullptr);
static MobileRT::Renderer *renderer_(nullptr);
static MobileRT::Sampler *samplerPointLight_(nullptr);
static unsigned int blockSize_(4u);
static unsigned int blockSizeX_(0u);
static unsigned int blockSizeY_(0u);
static unsigned int samplesPixel_(0u);
static unsigned int samplesLight_(0u);
static const unsigned int width_(900u);
static const unsigned int height_(900u);
static unsigned int bitmap[width_ * height_];
static unsigned char buffer[width_ * height_ * 4u];//RGBA

static MobileRT::Scene cornellBoxScene(MobileRT::Scene&& scene) noexcept {
    // point light - white
    const MobileRT::Material lightMat(MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      1.0f,
                                      MobileRT::RGB(0.9f, 0.9f, 0.9f));
    scene.lights_.emplace_back(new Components::PointLight(lightMat,
                                                          MobileRT::Point3D(0.0f, 0.99f, 0.0f)));

    // back wall - white
    const MobileRT::Material lightGrayMat(MobileRT::RGB(0.7f, 0.7f, 0.7f));
    scene.primitives_.emplace_back(new MobileRT::Plane(
            MobileRT::Point3D(0.0f, 0.0f, 1.0f), MobileRT::Vector3D(0.0f, 0.0f, -1.0f)),
                                   lightGrayMat);

    // front wall - light blue
    const MobileRT::Material lightBlueMat(MobileRT::RGB(0.0f, 0.9f, 0.9f));
    scene.primitives_.emplace_back(new MobileRT::Plane(
            MobileRT::Point3D(0.0f, 0.0f, -3.5f), MobileRT::Vector3D(0.0f, 0.0f, 1.0f)),
                                   lightBlueMat);
    // floor - white
    scene.primitives_.emplace_back(new MobileRT::Plane(
            MobileRT::Point3D(0.0f, -1.0f, 0.0f), MobileRT::Vector3D(0.0f, 1.0f, 0.0f)),
                                   lightGrayMat);
    // ceiling - white
    scene.primitives_.emplace_back(new MobileRT::Plane(
            MobileRT::Point3D(0.0f, 1.0f, 0.0f), MobileRT::Vector3D(0.0f, -1.0f, 0.0f)),
                                   lightGrayMat);
    // left wall - red
    const MobileRT::Material redMat(MobileRT::RGB(0.9f, 0.0f, 0.0f));
    scene.primitives_.emplace_back(new MobileRT::Plane(
            MobileRT::Point3D(-1.0f, 0.0f, 0.0f), MobileRT::Vector3D(1.0f, 0.0f, 0.0f)), redMat);

    // right wall - blue
    const MobileRT::Material blueMat(MobileRT::RGB(0.0f, 0.0f, 0.9f));
    scene.primitives_.emplace_back(new MobileRT::Plane(
            MobileRT::Point3D(1.0f, 0.0f, 0.0f), MobileRT::Vector3D(-1.0f, 0.0f, 0.0f)), blueMat);

    // sphere - mirror
    const MobileRT::Material MirrorMat(MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                       MobileRT::RGB(0.9f, 0.9f, 0.9f));
    scene.primitives_.emplace_back(new MobileRT::Sphere(
            MobileRT::Point3D(0.45f, -0.65f, 0.4f), 0.35f), MirrorMat);

    // sphere - green
    const MobileRT::Material GreenMat(MobileRT::RGB(0.0f, 0.9f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.2f, 0.0f));
    scene.primitives_.emplace_back(new MobileRT::Sphere(
            MobileRT::Point3D(-0.45f, -0.1f, 0.0f), 0.35f), GreenMat);

    // triangle - yellow
    const MobileRT::Material yellowMat(MobileRT::RGB(0.9f, 0.9f, 0.0f));
    scene.primitives_.emplace_back(new MobileRT::Triangle(
            MobileRT::Point3D(0.5f, -0.5f, 0.99f), MobileRT::Point3D(-0.5f, -0.5f, 0.99f),
            MobileRT::Point3D(0.5f, 0.5f, 1.001f)), yellowMat);

    return std::move(scene);
}

static MobileRT::Scene cornellBoxScene2(MobileRT::Scene&& scene) noexcept {
    const auto max(static_cast<uint64_t> (-1));
    LOG("samplesLight = ", samplesLight_, " max = ", max);
    const uint64_t domainPointLight(/*roundUpPower2*/
            ((width_ * height_ * 2ull) * 2ull * samplesLight_ * samplesPixel_ * RAY_DEPTH_MAX));
    LOG("domainPointLight = ", domainPointLight);
    LOG("width_ = ", width_);
    LOG("height_ = ", height_);
    LOG("samplesLight_ = ", samplesLight_);
    LOG("samplesPixel_ = ", samplesPixel_);
    LOG("RAY_DEPTH_MAX = ", RAY_DEPTH_MAX);
    //samplerPointLight_ = new Components::HaltonSeq(domainPointLight, 1);
    samplerPointLight_ = new Components::Random(domainPointLight, 1);

    const MobileRT::Material lightMat(MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      1.0f,
                                      MobileRT::RGB(0.9f, 0.9f, 0.9f));

    scene.lights_.emplace_back(new Components::AreaLight(lightMat,
                                                         *samplerPointLight_,
                                                         MobileRT::Point3D(-0.25f, 0.99f, -0.25f),
                                                         MobileRT::Point3D(0.25f, 0.99f, -0.25f),
                                                         MobileRT::Point3D(0.25f, 0.99f, 0.25f)));

    scene.lights_.emplace_back(new Components::AreaLight(lightMat,
                                                         *samplerPointLight_,
                                                         MobileRT::Point3D(0.25f, 0.99f, 0.25f),
                                                         MobileRT::Point3D(-0.25f, 0.99f, 0.25f),
                                                         MobileRT::Point3D(-0.25f, 0.99f, -0.25f)));

    // block light - black
    /*const MobileRT::Material blockMat(MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.0f, 0.0f));
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Triangle(
            MobileRT::Point3D(0.3f, 0.98f, -0.3f),
            MobileRT::Point3D(-0.3f, 0.98f, -0.3f),
            MobileRT::Point3D(0.3f, 0.98f, 0.3f)), blockMat));
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Triangle(
            MobileRT::Point3D(-0.3f, 0.98f, 0.3f),
            MobileRT::Point3D(0.3f, 0.98f, 0.3f),
            MobileRT::Point3D(-0.3f, 0.98f, -0.3f)), blockMat));*/

    // back wall - white
    const MobileRT::Material lightGrayMat(MobileRT::RGB(0.7f, 0.7f, 0.7f));
    scene.primitives_.emplace_back(new MobileRT::Plane(
            MobileRT::Point3D(0.0f, 0.0f, 1.0f), MobileRT::Vector3D(0.0f, 0.0f, -1.0f)),
                                   lightGrayMat);

    // front wall - light blue
    const MobileRT::Material lightBlueMat(MobileRT::RGB(0.0f, 0.9f, 0.9f));
    scene.primitives_.emplace_back(new MobileRT::Plane(
            MobileRT::Point3D(0.0f, 0.0f, -4.0f), MobileRT::Vector3D(0.0f, 0.0f, 1.0f)),
                                   lightBlueMat);

    // floor - white
    scene.primitives_.emplace_back(new MobileRT::Plane(
            MobileRT::Point3D(0.0f, -1.0f, 0.0f), MobileRT::Vector3D(0.0f, 1.0f, 0.0f)),
                                   lightGrayMat);
    // ceiling - white
    scene.primitives_.emplace_back(new MobileRT::Plane(
            MobileRT::Point3D(0.0f, 1.0f, 0.0f), MobileRT::Vector3D(0.0f, -1.0f, 0.0f)),
                                   lightGrayMat);
    // left wall - red
    const MobileRT::Material redMat(MobileRT::RGB(0.9f, 0.0f, 0.0f));
    scene.primitives_.emplace_back(new MobileRT::Plane(
            MobileRT::Point3D(-1.0f, 0.0f, 0.0f), MobileRT::Vector3D(1.0f, 0.0f, 0.0f)), redMat);

    // right wall - blue
    const MobileRT::Material blueMat(MobileRT::RGB(0.0f, 0.0f, 0.9f));
    scene.primitives_.emplace_back(new MobileRT::Plane(
            MobileRT::Point3D(1.0f, 0.0f, 0.0f), MobileRT::Vector3D(-1.0f, 0.0f, 0.0f)), blueMat);

    // sphere - mirror
    const MobileRT::Material MirrorMat(MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                       MobileRT::RGB(0.9f, 0.9f, 0.9f));
    scene.primitives_.emplace_back(new MobileRT::Sphere(
            MobileRT::Point3D(0.45f, -0.65f, 0.4f), 0.35f), MirrorMat);

    // sphere - transmission
    const MobileRT::Material TransmissionMat(MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                             MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                             MobileRT::RGB(0.9f, 0.9f, 0.9f), 1.1f);
    scene.primitives_.emplace_back(new MobileRT::Sphere(
            MobileRT::Point3D(-0.4f, -0.3f, 0.0f), 0.35f), TransmissionMat);

    // triangle - yellow
    const MobileRT::Material yellowMat(MobileRT::RGB(0.9f, 0.9f, 0.0f));
    scene.primitives_.emplace_back(new MobileRT::Triangle(
            MobileRT::Point3D(0.5f, -0.5f, 1.0f), MobileRT::Point3D(-0.5f, -0.5f, 1.0f),
            MobileRT::Point3D(0.5f, 0.5f, 1.0f)), yellowMat);

    // triangle - green
    const MobileRT::Material greenMat(MobileRT::RGB(0.0f, 0.9f, 0.0f));
    scene.primitives_.emplace_back(new MobileRT::Triangle(
            MobileRT::Point3D(-0.5f, 0.5f, 1.0f),
            MobileRT::Point3D(0.5f, 0.5f, 1.0f),
            MobileRT::Point3D(-0.5f, -0.5f, 1.0f)), greenMat);

    return std::move(scene);
}

static MobileRT::Scene spheresScene(MobileRT::Scene&& scene) noexcept {
    // create one light source
    const MobileRT::Material lightMat(MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      1.0f,
                                      MobileRT::RGB(0.9f, 0.9f, 0.9f));
    scene.lights_.emplace_back(new Components::PointLight(lightMat,
                                                          MobileRT::Point3D(0.0f, 15.0f, 4.0f)));

    // create diffuse Materials
    const MobileRT::Material sandMat(MobileRT::RGB(0.914f, 0.723f, 0.531f));
    const MobileRT::Material redMat(MobileRT::RGB(0.9f, 0.0f, 0.0f));
    const MobileRT::Material mirrorMat(MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                       MobileRT::RGB(0.9f, 0.9f, 0.9f));
    const MobileRT::Material greenMat(MobileRT::RGB(0.0f, 0.9f, 0.0f));
    // create one sphere
    scene.primitives_.emplace_back(
            new MobileRT::Sphere(MobileRT::Point3D(-1.0f, 1.0f, 6.0f), 1.0f), redMat);
    scene.primitives_.emplace_back(
            new MobileRT::Sphere(MobileRT::Point3D(1.5f, 2.0f, 7.0f), 1.0f),
            mirrorMat);
    scene.primitives_.emplace_back(new MobileRT::Plane(MobileRT::Point3D(0.0f, 0.0f, 0.0f),
                                                        MobileRT::Vector3D(0.0f, 1.0f, 0.0f)),
                                   sandMat);
    scene.primitives_.emplace_back(
            new MobileRT::Sphere(MobileRT::Point3D(0.0f, 0.5f, 4.5f), 0.5f),
            greenMat);
    return std::move(scene);
}

static MobileRT::Scene spheresScene2(MobileRT::Scene&& scene) noexcept {
    // create one light source
    const MobileRT::Material lightMat(MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      1.0f,
                                      MobileRT::RGB(0.9f, 0.9f, 0.9f));
    scene.lights_.emplace_back(new Components::PointLight(lightMat,
                                                          MobileRT::Point3D(0.0f, 15.0f, 4.0f)));

    // create diffuse Materials
    const MobileRT::Material sandMat(MobileRT::RGB(0.914f, 0.723f, 0.531f));
    const MobileRT::Material redMat(MobileRT::RGB(0.9f, 0.0f, 0.0f));
    const MobileRT::Material blueMat(MobileRT::RGB(0.0f, 0.0f, 0.9f));
    const MobileRT::Material yellowMat(MobileRT::RGB(0.9f, 0.9f, 0.0f),
                                       MobileRT::RGB(0.8f, 0.8f, 0.4f));
    const MobileRT::Material mirrorMat(MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                       MobileRT::RGB(0.9f, 0.9f, 0.9f));
    const MobileRT::Material greenMat(MobileRT::RGB(0.0f, 0.9f, 0.0f));
    // create one sphere
    scene.primitives_.emplace_back(
            new MobileRT::Sphere(MobileRT::Point3D(-1.0f, 1.0f, 6.0f), 1.0f), redMat);
    scene.primitives_.emplace_back(
            new MobileRT::Sphere(MobileRT::Point3D(-1.0f, 2.0f, 5.0f), 0.3f), blueMat);
    scene.primitives_.emplace_back(
            new MobileRT::Sphere(MobileRT::Point3D(1.5f, 2.0f, 7.0f), 1.0f), mirrorMat);
    scene.primitives_.emplace_back(
            new MobileRT::Sphere(MobileRT::Point3D(1.5f, 0.5f, 5.0f), 0.2f), yellowMat);
    scene.primitives_.emplace_back(new MobileRT::Plane(MobileRT::Point3D(0.0f, 0.0f, 0.0f),
                                                        MobileRT::Vector3D(0.0f, 1.0f, 0.0f)),
                                   sandMat);
    scene.primitives_.emplace_back(
            new MobileRT::Sphere(MobileRT::Point3D(0.0f, 0.5f, 4.5f), 0.5f), greenMat);
    return std::move(scene);
}

int main(int argc, char **argv) noexcept {
    int repeats(atoi(argv[4]));
    const int scene(atoi(argv[3]));
    const int shader(atoi(argv[2]));
    const int threads(atoi(argv[1]));
    const int sampler(0);
    const int samplesPixel(atoi(argv[5]));
		const int samplesLight(atoi(argv[6]));
		const float ratio(static_cast<float>(height_) / static_cast<float>(width_));
		
		MobileRT::Scene scene_;
    blockSizeX_ = width_ / blockSize_;
    blockSizeY_ = height_ / blockSize_;
    samplesPixel_ = static_cast<unsigned int>(samplesPixel);
    samplesLight_ = static_cast<unsigned int>(samplesLight);
    LOG("samplesPixel_ = ", samplesPixel_);
    LOG("samplesLight_ = ", samplesLight_);
    switch (scene) {
        case 1:
            camera_ = new Components::Perspective(MobileRT::Point3D(0.0f, 0.5f, 1.0f),
                                                  MobileRT::Point3D(0.0f, 0.0f, 7.0f),
                                                  MobileRT::Vector3D(0.0f, 1.0f, 0.0f),
                                                  60.0f, 60.0f * ratio);
            scene_ = spheresScene(std::move(scene_));
            break;

        case 2:
            camera_ = new Components::Perspective(MobileRT::Point3D(0.0f, 0.5f, 1.0f),
                                                  MobileRT::Point3D(0.0f, 0.0f, 7.0f),
                                                  MobileRT::Vector3D(0.0f, 1.0f, 0.0f),
                                                  60.0f, 60.0f * ratio);
            scene_ = spheresScene2(std::move(scene_));
            break;

        case 3:
            camera_ = new Components::Perspective(MobileRT::Point3D(0.0f, 0.0f, -3.4f),
                                                  MobileRT::Point3D(0.0f, 0.0f, 1.0f),
                                                  MobileRT::Vector3D(0.0f, 1.0f, 0.0f),
                                                  45.0f, 45.0f * ratio);
            scene_ = cornellBoxScene2(std::move(scene_));
            break;

        default:
            camera_ = new Components::Perspective(MobileRT::Point3D(0.0f, 0.0f, -3.4f),
                                                  MobileRT::Point3D(0.0f, 0.0f, 1.0f),
                                                  MobileRT::Vector3D(0.0f, 1.0f, 0.0f),
                                                  45.0f, 45.0f * ratio);
            scene_ = cornellBoxScene(std::move(scene_));
            break;
    }
    switch (sampler) {
        case 1:
            if (samplesPixel_ > 1u) {
                samplerPixel_ = new Components::HaltonSeq(width_ * height_ * 2ull * samplesPixel_,
                                                          1u);
            } else {
                samplerPixel_ = new Components::Constant(0.5f);
            }
            samplerCamera_ = new Components::HaltonSeq(width_, height_, samplesPixel_,
                                                       blockSizeX_, blockSizeY_);
            break;

        default:
            if (samplesPixel_ > 1u) {
                samplerPixel_ = new Components::Stratified(width_ * height_ * 2ull * samplesPixel_,
                                                           1u);
            } else {
                samplerPixel_ = new Components::Constant(0.5f);
            }
            samplerCamera_ = new Components::Stratified(width_, height_, samplesPixel_,
                                                        blockSizeX_, blockSizeY_);
            break;
    }
		samplerRay_ = nullptr;
    samplerPointLight_ = nullptr;
    const uint64_t domainRay(
            (width_ * height_ * 2ull) * samplesPixel_ * RAY_DEPTH_MAX);
    const uint64_t domainLight(
            (width_ * height_ * 2ull) * samplesPixel_ * RAY_DEPTH_MAX * samplesLight_);
    switch (shader) {
        case 1:
            shader_ = new Components::Whitted(std::move(scene_), samplesLight_);
            break;

        case 2:
            LOG("domainRay = ", domainRay, " domainLight = ", domainLight);
            //samplerRay_ = new Components::HaltonSeq(domainRay, 1);
            samplerRay_ = new Components::Random(domainRay, 1);
            //samplerLight_ = new Components::HaltonSeq(domainLight, 1);
            samplerLight_ = new Components::Random(domainLight, 1);
            shader_ = new Components::PathTracer(
                    std::move(scene_), *samplerRay_, *samplerLight_, samplesLight_);
            break;

        default:
            shader_ = new Components::NoShadows(std::move(scene_), samplesLight_);
            break;
    }
    renderer_ = new MobileRT::Renderer(*samplerCamera_, *shader_, *camera_, width_,
                                       height_, blockSizeX_, blockSizeY_, *samplerPixel_);

    LOG("x = ", blockSizeX_, "[", width_, "]");
    LOG("y = ", blockSizeY_, "[", height_, "]");


    LOG("Threads = ", threads);
    const double start(omp_get_wtime());
    try {
        do {
            renderer_->renderFrame(bitmap, static_cast<unsigned int>(threads));
			camera_->position_.x_ += 2.0f;
        } while (repeats-- > 1);
    } catch (...) {
        LOG("EXCEPTION");
        raise(SIGTRAP);
    }
    delete renderer_;
    const double end(omp_get_wtime() - start);
    LOG("Time in secs = ", end);

    for (unsigned int i(0u), j(0u); i < width_ * height_ * 4u; i += 4u, j += 1u) {
        const unsigned int color(bitmap[j]);
        buffer[i + 0] = static_cast<unsigned char> ((color & 0x000000FF) >> 0);
        buffer[i + 1] = static_cast<unsigned char> ((color & 0x0000FF00) >> 8);
        buffer[i + 2] = static_cast<unsigned char> ((color & 0x00FF0000) >> 16);
        buffer[i + 3] = static_cast<unsigned char> ((color & 0xFF000000) >> 24);
    }
  //   gtk_init(&argc, &argv);
  //   GtkWidget *window (gtk_window_new(GTK_WINDOW_TOPLEVEL));
  //   GdkPixbuf *pixbuff (gdk_pixbuf_new_from_data(buffer, GDK_COLORSPACE_RGB, TRUE, 8,
  //                                                 width_, height_, width_ * 4, nullptr, nullptr));
  //   GtkWidget *image (gtk_image_new_from_pixbuf(pixbuff));
	// 	gtk_signal_connect(GTK_OBJECT(window), "destroy", GTK_SIGNAL_FUNC(
	// 		[]() -> void {
	// 			gtk_main_quit();
	// 		}
	// 	), nullptr);
	// 	auto *check_escape (static_cast<bool(*)(
	// 		GtkWidget* /*unused*/, GdkEventKey *event, gpointer /*unused*/)>([](GtkWidget* /*unused*/, GdkEventKey *event, gpointer /*unused*/){
	// 		if (event->keyval == GDK_KEY_Escape) {
	// 				gtk_main_quit();
	// 				return true;
	// 			}
	// 			return false;
	// 		})
	// 	);
  //   gtk_signal_connect(GTK_OBJECT(window), "key_press_event", GTK_SIGNAL_FUNC(check_escape), nullptr);
  //   gtk_container_add(GTK_CONTAINER(window), image);
  //   gtk_widget_show_all(window);
  //   gtk_main();
	// g_object_unref (G_OBJECT(pixbuff));
    return argc;
}
