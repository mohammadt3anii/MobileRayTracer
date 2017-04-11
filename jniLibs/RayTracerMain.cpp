#include <iostream>
#include <gtk/gtk.h>
#include <omp.h>
#include <gdk/gdkkeysyms.h>

#include "MobileRT/src/main/cpp/MobileRT/Scene.h"
#include "MobileRT/src/main/cpp/MobileRT/Renderer.h"
#include "MobileRT/src/main/cpp/MobileRT/Shapes/Plane.h"
#include "MobileRT/src/main/cpp/MobileRT/Shapes/Sphere.h"
#include "MobileRT/src/main/cpp/MobileRT/Shapes/Triangle.h"
#include "Components/src/main/cpp/MobileRT/Shaders/NoShadows.h"
#include "Components/src/main/cpp/MobileRT/Shaders/Whitted.h"
#include "Components/src/main/cpp/MobileRT/Shaders/PathTracer.h"
#include "Components/src/main/cpp/MobileRT/Samplers/Stratified.h"
#include "Components/src/main/cpp/MobileRT/Samplers/HaltonSeq.h"
#include "Components/src/main/cpp/MobileRT/Cameras/Perspective.h"
#include "Components/src/main/cpp/MobileRT/Lights/PointLight.h"
#include "Components/src/main/cpp/MobileRT/Lights/AreaLight.h"

static MobileRT::Scene *scene_(nullptr);
static MobileRT::Camera *camera_(nullptr);
static MobileRT::Shader *shader_(nullptr);
static MobileRT::Sampler *samplerCamera_(nullptr);
static MobileRT::Sampler *samplerRay_(nullptr);
static MobileRT::Sampler *samplerPointLight_(nullptr);
static MobileRT::Sampler *samplerLight_(nullptr);
static MobileRT::Renderer *renderer_(nullptr);

static const unsigned int width_(512);
static const unsigned int height_(512);
static unsigned int samplesPixel_(0);
static unsigned int samplesLight_(0);
static unsigned int blockSizeX_(64);
static unsigned int blockSizeY_(64);
static unsigned int canvas[width_ * height_];

MobileRT::Scene *cornellBoxScene(void) {
    MobileRT::Scene &scene = *new MobileRT::Scene();
    // point light - white
    scene.lights_.emplace_back(new MobileRT::PointLight(MobileRT::RGB(1.0f, 1.0f, 1.0f),
                                                        MobileRT::Point3D(0.0f, 0.99f, 0.0f)));

    // back wall - white
    const MobileRT::Material lightGrayMat(MobileRT::RGB(0.9f, 0.9f, 0.9f));
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Plane(
            MobileRT::Point3D(0.0f, 0.0f, 1.0f), MobileRT::Vector3D(0.0f, 0.0f, -1.0f)),
                                                           lightGrayMat));
    // floor - white
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Plane(
            MobileRT::Point3D(0.0f, -1.0f, 0.0f), MobileRT::Vector3D(0.0f, 1.0f, 0.0f)),
                                                           lightGrayMat));
    // ceiling - white
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Plane(
            MobileRT::Point3D(0.0f, 1.0f, 0.0f), MobileRT::Vector3D(0.0f, -1.0f, 0.0f)),
                                                           lightGrayMat));
    // left wall - red
    const MobileRT::Material redMat(MobileRT::RGB(0.9f, 0.0f, 0.0f));
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Plane(
            MobileRT::Point3D(-1.0f, 0.0f, 0.0f), MobileRT::Vector3D(1.0f, 0.0f, 0.0f)), redMat));
    // right wall - blue
    const MobileRT::Material blueMat(MobileRT::RGB(0.0f, 0.0f, 0.9f));
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Plane(
            MobileRT::Point3D(1.0f, 0.0f, 0.0f), MobileRT::Vector3D(-1.0f, 0.0f, 0.0f)), blueMat));

    // sphere - mirror
    const MobileRT::Material MirrorMat(MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                       MobileRT::RGB(0.8f, 0.8f, 0.8f));
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Sphere(
            MobileRT::Point3D(0.45f, -0.65f, 0.4f), 0.35f), MirrorMat));

    // sphere - green
    const MobileRT::Material GreenMat(MobileRT::RGB(0.0f, 0.9f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.2f, 0.0f));
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Sphere(
            MobileRT::Point3D(-0.45f, -0.1f, 0.0f), 0.35f), GreenMat));

    // triangle - yellow
    const MobileRT::Material yellowMat(MobileRT::RGB(1.0f, 1.0f, 0.0f));
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Triangle(
            MobileRT::Point3D(0.5f, -0.5f, 0.99f), MobileRT::Point3D(-0.5f, -0.5f, 0.99f),
            MobileRT::Point3D(0.5f, 0.5f, 1.001f)), yellowMat));
    return &scene;
}

MobileRT::Scene *cornellBoxScene2(void) {
    MobileRT::Scene &scene = *new MobileRT::Scene();
    // point light - white
    /*const unsigned long long int samples1 (static_cast<unsigned long long int>(
            width_ * height_ * samplesPixel_ * (samplesLight_ * RAY_DEPTH_MAX) *
            (samplesLight_ * RAY_DEPTH_MAX)));*/

    const unsigned int max(static_cast<unsigned int> (-1));
    LOG("samplesLight = %u, max = %u", samplesLight_, max);
    const unsigned int domainPointLight (width_ * height_ * 2 * 2 * samplesLight_ * RAY_DEPTH_MAX);
    LOG("domainPointLight = %u", domainPointLight);
    samplerPointLight_ = new MobileRT::HaltonSeq(domainPointLight, 1, true);

    /*scene.lights_.emplace_back(new MobileRT::PointLight(MobileRT::RGB(1.0f, 1.0f, 1.0f),
                                                       MobileRT::Point3D(0.0f, 0.99f, 0.0f)));*/

    /*MobileRT::Triangle(
            MobileRT::Point3D(0.5f, -0.5f, 0.99f),
            MobileRT::Point3D(-0.5f, -0.5f, 0.99f),
            MobileRT::Point3D(0.5f, 0.5f, 0.99f));*/

    scene.lights_.emplace_back(new MobileRT::AreaLight(MobileRT::RGB(1.0f, 1.0f, 1.0f),
                                                       *samplerPointLight_,
                                                       MobileRT::Point3D(-0.25f, 0.98f, -0.25f),
                                                       MobileRT::Point3D(0.25f, 0.98f, -0.25f),
                                                       MobileRT::Point3D(0.25f, 0.98f, 0.25f)));

    scene.lights_.emplace_back(new MobileRT::AreaLight(MobileRT::RGB(1.0f, 1.0f, 1.0f),
                                                       *samplerPointLight_,
                                                       MobileRT::Point3D(0.25f, 0.98f, 0.25f),
                                                       MobileRT::Point3D(-0.25f, 0.98f, 0.25f),
                                                       MobileRT::Point3D(-0.25f, 0.98f, -0.25f)));

    /*const MobileRT::Material lightWhiteMat(MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                      MobileRT::RGB(1.0f, 1.0f, 1.0f));
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Triangle(
            MobileRT::Point3D(0.25f, 0.99f, -0.25f),
            MobileRT::Point3D(-0.25f, 0.99f, -0.25f),
            MobileRT::Point3D(0.25f, 0.99f, 0.25f)), lightWhiteMat));
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Triangle(
            MobileRT::Point3D(-0.25f, 0.99f, 0.25f),
            MobileRT::Point3D(0.25f, 0.99f, 0.25f),
            MobileRT::Point3D(-0.25f, 0.99f, -0.25f)), lightWhiteMat));*/

    // back wall - white
    const MobileRT::Material lightGrayMat(MobileRT::RGB(0.9f, 0.9f, 0.9f));
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Plane(
            MobileRT::Point3D(0.0f, 0.0f, 1.0f), MobileRT::Vector3D(0.0f, 0.0f, -1.0f)),
                                                           lightGrayMat));

    // back wall - mirror
    const MobileRT::Material MirrorMat(MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                       MobileRT::RGB(0.8f, 0.8f, 0.8f));
    /*scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Plane(
        MobileRT::Point3D(0.0f, 0.0f, 1.0f), MobileRT::Vector3D(0.0f, 0.0f, -1.0f)), MirrorMat));*/

    // front wall - light blue
    const MobileRT::Material lightBlueMat(MobileRT::RGB(0.0f, 1.0f, 1.0f));
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Plane(
            MobileRT::Point3D(0.0f, 0.0f, -3.5f), MobileRT::Vector3D(0.0f, 0.0f, 1.0f)),
                                                           lightBlueMat));

    // floor - white
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Plane(
            MobileRT::Point3D(0.0f, -1.0f, 0.0f), MobileRT::Vector3D(0.0f, 1.0f, 0.0f)),
                                                           lightGrayMat));
    // ceiling - white
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Plane(
            MobileRT::Point3D(0.0f, 1.0f, 0.0f), MobileRT::Vector3D(0.0f, -1.0f, 0.0f)),
                                                           lightGrayMat));
    // left wall - red
    const MobileRT::Material redMat(MobileRT::RGB(0.9f, 0.0f, 0.0f));
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Plane(
            MobileRT::Point3D(-1.0f, 0.0f, 0.0f), MobileRT::Vector3D(1.0f, 0.0f, 0.0f)), redMat));

    // right wall - blue
    const MobileRT::Material blueMat(MobileRT::RGB(0.0f, 0.0f, 0.9f));
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Plane(
            MobileRT::Point3D(1.0f, 0.0f, 0.0f), MobileRT::Vector3D(-1.0f, 0.0f, 0.0f)), blueMat));

    // right wall - mirror
    /*scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Plane(
        MobileRT::Point3D(1.0f, 0.0f, 0.0f), MobileRT::Vector3D(-1.0f, 0.0f, 0.0f)), MirrorMat));*/

    // sphere - mirror
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Sphere(
            MobileRT::Point3D(0.45f, -0.65f, 0.4f), 0.35f), MirrorMat));

    // sphere - green
    /*const MobileRT::Material GreenMat(MobileRT::RGB(0.0f, 0.9f, 0.0f),
                                      MobileRT::RGB(0.0f, 0.2f, 0.0f));
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Sphere(
            MobileRT::Point3D(-0.45f, -0.1f, 0.0f), 0.35f), GreenMat));*/

    // sphere - transmission
    const MobileRT::Material TransmissionMat(MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                             MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                             MobileRT::RGB(0.8f, 0.8f, 0.8f), 1.5f);
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Sphere(
            MobileRT::Point3D(-0.45f, -0.1f, 0.0f), 0.35f), TransmissionMat));

    // triangle - yellow
    const MobileRT::Material yellowMat(MobileRT::RGB(1.0f, 1.0f, 0.0f));
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Triangle(
            MobileRT::Point3D(0.5f, -0.5f, 0.99f), MobileRT::Point3D(-0.5f, -0.5f, 0.99f),
            MobileRT::Point3D(0.5f, 0.5f, 0.99f)), yellowMat));

    // triangle - green
    const MobileRT::Material greenMat(MobileRT::RGB(0.0f, 1.0f, 0.0f));
    scene.primitives_.emplace_back(new MobileRT::Primitive(new MobileRT::Triangle(
            MobileRT::Point3D(0.5f, 0.5f, 0.99f), MobileRT::Point3D(-0.5f, 0.5f, 0.99f),
            MobileRT::Point3D(-0.5f, -0.5f, 0.99f)), greenMat));

    return &scene;
}

MobileRT::Scene *spheresScene(void) {
    MobileRT::Scene &scene = *new MobileRT::Scene();
    // create one light source
    scene.lights_.emplace_back(new MobileRT::PointLight(MobileRT::RGB(1.0f, 1.0f, 1.0f),
                                                        MobileRT::Point3D(0.0f, 15.0f, 4.0f)));

    // create diffuse Materials
    const MobileRT::Material sandMat(MobileRT::RGB(0.914f, 0.723f, 0.531f));
    const MobileRT::Material redMat(MobileRT::RGB(0.9f, 0.0f, 0.0f));
    const MobileRT::Material mirrorMat(MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                       MobileRT::RGB(0.8f, 0.8f, 0.8f));
    const MobileRT::Material greenMat(MobileRT::RGB(0.0f, 0.9f, 0.0f));
    // create one sphere
    scene.primitives_.emplace_back(
            new MobileRT::Primitive(
                    new MobileRT::Sphere(MobileRT::Point3D(-1.0f, 1.0f, 6.0f), 1.0f), redMat));
    scene.primitives_.emplace_back(
            new MobileRT::Primitive(
                    new MobileRT::Sphere(MobileRT::Point3D(1.5f, 2.0f, 7.0f), 1.0f),
                    mirrorMat));
    scene.primitives_.emplace_back(
            new MobileRT::Primitive(new MobileRT::Plane(MobileRT::Point3D(0.0f, 0.0f, 0.0f),
                                                        MobileRT::Vector3D(0.0f, 1.0f, 0.0f)),
                                    sandMat));
    scene.primitives_.emplace_back(
            new MobileRT::Primitive(
                    new MobileRT::Sphere(MobileRT::Point3D(0.0f, 0.5f, 4.5f), 0.5f),
                    greenMat));
    return &scene;
}

MobileRT::Scene *spheresScene2(void) {
    MobileRT::Scene &scene = *new MobileRT::Scene();
    // create one light source
    scene.lights_.emplace_back(new MobileRT::PointLight(MobileRT::RGB(1.0f, 1.0f, 1.0f),
                                                        MobileRT::Point3D(0.0f, 15.0f, 4.0f)));

    // create diffuse Materials
    const MobileRT::Material sandMat(MobileRT::RGB(0.914f, 0.723f, 0.531f));
    const MobileRT::Material redMat(MobileRT::RGB(0.9f, 0.0f, 0.0f));
    const MobileRT::Material blueMat(MobileRT::RGB(0.0f, 0.0f, 0.9f));
    const MobileRT::Material yellowMat(MobileRT::RGB(0.9f, 0.9f, 0.0f),
                                       MobileRT::RGB(0.5f, 0.5f, 0.5f));
    const MobileRT::Material mirrorMat(MobileRT::RGB(0.0f, 0.0f, 0.0f),
                                       MobileRT::RGB(0.8f, 0.8f, 0.8f));
    const MobileRT::Material greenMat(MobileRT::RGB(0.0f, 0.9f, 0.0f));
    // create one sphere
    scene.primitives_.emplace_back(
        new MobileRT::Primitive(
        new MobileRT::Sphere(MobileRT::Point3D(-1.0f, 1.0f, 6.0f), 1.0f), redMat));
    scene.primitives_.emplace_back(
        new MobileRT::Primitive(
        new MobileRT::Sphere(MobileRT::Point3D(-1.0f, 2.0f, 5.0f), 0.3f), blueMat));
    scene.primitives_.emplace_back(
        new MobileRT::Primitive(
        new MobileRT::Sphere(MobileRT::Point3D(1.5f, 2.0f, 7.0f), 1.0f), mirrorMat));
    scene.primitives_.emplace_back(
        new MobileRT::Primitive(
        new MobileRT::Sphere(MobileRT::Point3D(1.5f, 0.5f, 5.0f), 0.2f), yellowMat));
    scene.primitives_.emplace_back(
        new MobileRT::Primitive(new MobileRT::Plane(MobileRT::Point3D(0.0f, 0.0f, 0.0f),
                                        MobileRT::Vector3D(0.0f, 1.0f, 0.0f)), sandMat));
    scene.primitives_.emplace_back(
        new MobileRT::Primitive(
        new MobileRT::Sphere(MobileRT::Point3D(0.0f, 0.5f, 4.5f), 0.5f), greenMat));
    return &scene;
}

void destroy(GtkWidget *, gpointer) {
    gtk_main_quit();
}

static bool check_escape(GtkWidget *, GdkEventKey *event, gpointer) {
    if (event->keyval == GDK_KEY_Escape) {
        gtk_main_quit();
        return true;
    }
    return false;
}

int main(int argc, char **argv) {
    unsigned int repeats(1);
    const int scene(3);
    const int shader(2);
    const int threads(4);
    const int sampler(1);
    const int samplesPixel(16);
    const int samplesLight(16);
    const float ratio(static_cast<float>(height_) / static_cast<float>(width_));

    samplesPixel_ = static_cast<unsigned int>(samplesPixel);
    samplesLight_ = static_cast<unsigned int>(samplesLight);
    switch (scene) {
        case 1:
            camera_ = new MobileRT::Perspective(MobileRT::Point3D(0.0f, 0.5f, 1.0f),
                                                60.0f, 60.0f * ratio);
            scene_ = spheresScene();
            break;

        case 2:
            camera_ = new MobileRT::Perspective(MobileRT::Point3D(0.0f, 0.5f, 1.0f),
                                                60.0f, 60.0f * ratio);
            scene_ = spheresScene2();
            break;

        case 3:
            camera_ = new MobileRT::Perspective(MobileRT::Point3D(0.0f, 0.0f, -3.4f),
                                                45.0f, 45.0f * ratio);
            scene_ = cornellBoxScene2();
            break;

        default:
            camera_ = new MobileRT::Perspective(MobileRT::Point3D(0.0f, 0.0f, -3.4f),
                                                45.0f, 45.0f * ratio);
            scene_ = cornellBoxScene();
            break;
    }
    switch (sampler) {
        case 1:
            samplerCamera_ = new MobileRT::HaltonSeq(width_, height_, samplesPixel_,
                                                     blockSizeX_, blockSizeY_);
            break;

        default:
            samplerCamera_ = new MobileRT::Stratified(width_, height_, samplesPixel_,
                                                      blockSizeX_, blockSizeY_);
            break;
    }
    samplerRay_ = nullptr;
    samplerPointLight_ = nullptr;
    const unsigned int domainRay (width_ * height_ * samplesPixel_ * RAY_DEPTH_MAX * 2);
    const unsigned int domainLight (
            width_ * height_ * samplesPixel_ * RAY_DEPTH_MAX * samplesLight_);
    switch (shader) {
        case 1:
            shader_ = new MobileRT::Whitted(*scene_, samplesLight_);
            break;

        case 2:
            LOG("domainRay = %u, domainLight = %u", domainRay, domainLight);
            samplerRay_ = new MobileRT::HaltonSeq(domainRay, 1);
            samplerLight_ = new MobileRT::HaltonSeq(domainLight, 1);
            shader_ = new MobileRT::PathTracer(
                    *scene_, *samplerRay_, *samplerLight_, samplesLight_);
            break;

        default:
            shader_ = new MobileRT::NoShadows(*scene_, samplesLight_);
            break;
    }
    renderer_ = new MobileRT::Renderer(*samplerCamera_, *shader_, *camera_, width_,
                                        height_, blockSizeX_, blockSizeY_);

    LOG("x = %d %d [%d]", roundDownMultipleOf(blockSizeX_, roundDownEvenNumber(width_)),
        roundDownEvenNumber(width_), width_);
    LOG("y = %d %d [%d]", roundDownMultipleOf(blockSizeY_, roundDownEvenNumber(height_)),
        roundDownEvenNumber(height_), height_);


    const double start(omp_get_wtime());
    try {
        do {
            renderer_->renderFrame(canvas, threads);
            repeats--;
        } while (repeats > 0);
    } catch (...) {
        LOG("%s", "EXCEPTION");
        raise(SIGTRAP);
    }
    const double end(omp_get_wtime() - start);
    std::cout << "\nTime in secs::" << end << std::endl;

    //RGBA
    unsigned char *buffer = new unsigned char[width_ * height_ * 4];

    for (unsigned int i(0), j(0); i < width_ * height_ * 4; i += 4, j += 1)
    {
        const unsigned int color(canvas[j]);
        buffer[i + 0] = static_cast<unsigned char> ((color & 0x000000FF) >> 0);
        buffer[i + 1] = static_cast<unsigned char> ((color & 0x0000FF00) >> 8);
        buffer[i + 2] = static_cast<unsigned char> ((color & 0x00FF0000) >> 16);
        buffer[i + 3] = static_cast<unsigned char> ((color & 0xFF000000) >> 24);
    }
    gtk_init(&argc, &argv);
    GtkWidget *window = gtk_window_new(GTK_WINDOW_TOPLEVEL);
    GdkPixbuf *pixbuff = gdk_pixbuf_new_from_data(buffer, GDK_COLORSPACE_RGB, TRUE, 8,
                                                width_, height_, width_ * 4, NULL, NULL);
    GtkWidget *image = gtk_image_new_from_pixbuf(pixbuff);
    gtk_signal_connect(GTK_OBJECT(window), "destroy", GTK_SIGNAL_FUNC(destroy), NULL);
    gtk_signal_connect(GTK_OBJECT(window), "key_press_event",
        GTK_SIGNAL_FUNC(check_escape), NULL);
    gtk_container_add(GTK_CONTAINER(window), image);
    gtk_widget_show_all(window);
    gtk_main();
    return 0;
}
