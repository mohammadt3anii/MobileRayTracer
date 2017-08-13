#include "../app/src/main/cpp/Scenes.hpp"
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
#include "MobileRT/Shapes/Plane.hpp"
#include "MobileRT/Shapes/Rectangle.hpp"
#include "MobileRT/Shapes/Sphere.hpp"
#include "MobileRT/Shapes/Triangle.hpp"
#include <cstdint>
#include <fstream>
#include <gdk/gdkkeysyms.h>
#include <gtk/gtk.h>
#include <iostream>
#include <omp.h>

static unsigned *bitmap {};
static unsigned char *buffer {};//RGBA

int main(int argc, char **argv) noexcept {
  int repeats {atoi (argv[4])};
  const int scene {atoi (argv[3])};
  const int shader {atoi (argv[2])};
  const int threads {atoi (argv[1])};
  const int sampler {0};
  const int samplesPixel {atoi (argv[5])};
  const int samplesLight {atoi (argv[6])};
  const int width_ {
    roundDownToMultipleOf (atoi (argv[7]), static_cast<int>(std::sqrt (NumberOfBlocks)))};
  const int height_ {
    roundDownToMultipleOf (atoi (argv[8]), static_cast<int>(std::sqrt (NumberOfBlocks)))};
  bitmap = new unsigned[static_cast<unsigned>(width_) * static_cast<unsigned>(height_)];
  buffer = new unsigned char[static_cast<unsigned>(width_) * static_cast<unsigned>(height_) * 4u];
  const float ratio {static_cast<float>(height_) / static_cast<float>(width_)};
  MobileRT::Renderer *renderer_ {nullptr};
  std::ifstream obj {"../app/src/main/assets/WavefrontOBJs/CornellBox/CornellBox-Sphere.obj"};
  std::string line {};
  std::stringstream ssObj {};
  while (std::getline (obj, line)) {
    ssObj << line << std::endl;
  }
  std::ifstream mtl {"../app/src/main/assets/WavefrontOBJs/CornellBox/CornellBox-Sphere.mtl"};
  std::stringstream ssMtl {};
  while (std::getline (mtl, line)) {
    ssMtl << line << std::endl;
  }
  Components::OBJLoader objLoader {ssObj.str ().c_str (), ssMtl.str ().c_str ()};
  objLoader.process();
  LOG("samplesPixel = ", samplesPixel);
  LOG("samplesLight = ", samplesLight);


  [&] ()->void {
    MobileRT::Scene scene_ {};
    std::unique_ptr<MobileRT::Sampler> samplerPixel {};
    std::unique_ptr<MobileRT::Shader> shader_ {};
    std::unique_ptr<MobileRT::Camera> camera {};
    switch (scene) {
      case 0:
        camera = std::make_unique<Components::Perspective> (
          MobileRT::Point3D{0.0f, 0.0f, - 3.4f},
          MobileRT::Point3D{0.0f, 0.0f, 1.0f},
          MobileRT::Vector3D{0.0f, 1.0f, 0.0f},
          45.0f, 45.0f * ratio);
        scene_ = cornellBoxScene (std::move (scene_));
        break;

      case 1:
        camera = std::make_unique<Components::Orthographic> (
          MobileRT::Point3D{0.0f, 1.0f, - 10.0f},
          MobileRT::Point3D{0.0f, 1.0f, 7.0f},
          MobileRT::Vector3D{0.0f, 1.0f, 0.0f},
          10.0f, 10.0f);
        /*camera = std::make_unique<Components::Perspective>(
          MobileRT::Point3D(0.0f, 0.5f, 1.0f),
          MobileRT::Point3D(0.0f, 0.0f, 7.0f),
          MobileRT::Vector3D(0.0f, 1.0f, 0.0f),
          60.0f, 60.0f * ratio);*/
        scene_ = spheresScene (std::move (scene_));
        break;

      case 2:
        camera = std::make_unique<Components::Perspective> (
          MobileRT::Point3D {0.0f, 0.0f, -3.4f},
          MobileRT::Point3D {0.0f, 0.0f, 1.0f},
          MobileRT::Vector3D {0.0f, 1.0f, 0.0f},
          45.0f, 45.0f * ratio);
        scene_ = cornellBoxScene2 (std::move (scene_));
        break;

      case 3:
        camera = std::make_unique<Components::Perspective> (
          MobileRT::Point3D {0.0f, 0.5f, 1.0f},
          MobileRT::Point3D {0.0f, 0.0f, 7.0f},
          MobileRT::Vector3D {0.0f, 1.0f, 0.0f},
          60.0f, 60.0f * ratio);
        scene_ = spheresScene2 (std::move (scene_));
        break;

      default:
        objLoader.fillScene (&scene_);
        camera = std::make_unique<Components::Perspective> (
          MobileRT::Point3D {0.0f, 0.5f, 3.0f},
          MobileRT::Point3D {0.0f, 0.5f, -1.0f},
          MobileRT::Vector3D {0.0f, 1.0f, 0.0f},
          45.0f, 45.0f * ratio);
        break;
    }
    switch (sampler) {
      case 1:
        if (samplesPixel > 1) {
          //samplerPixel = std::make_unique<Components::HaltonSeq> ();
          samplerPixel = std::make_unique<Components::StaticHaltonSeq> ();
        } else {
          samplerPixel = std::make_unique<Components::Constant> (0.5f);
        }
        break;

      default:
        if (samplesPixel > 1) {
          samplerPixel = std::make_unique<Components::Stratified> ();
        } else {
          samplerPixel = std::make_unique<Components::Constant> (0.5f);
        }
        break;
    }
    switch (shader) {
      case 1: {
        shader_ = std::make_unique<Components::Whitted> (std::move (scene_), samplesLight);
        break;
      }

      case 2: {
        //std::unique_ptr<MobileRT::Sampler> samplerRay {std::make_unique<Components::HaltonSeq>()};
        //std::unique_ptr<MobileRT::Sampler> samplerRay {std::make_unique<Components::MersenneTwister>()};
        std::unique_ptr<MobileRT::Sampler> samplerRay {
          std::make_unique<Components::StaticHaltonSeq> ()};
        //std::unique_ptr<MobileRT::Sampler> samplerRay {std::make_unique<Components::StaticMersenneTwister>()};

        //std::unique_ptr<MobileRT::Sampler> samplerLight {std::make_unique<Components::HaltonSeq>()};
        //std::unique_ptr<MobileRT::Sampler> samplerLight {std::make_unique<Components::MersenneTwister>()};
        std::unique_ptr<MobileRT::Sampler> samplerLight {
          std::make_unique<Components::StaticHaltonSeq> ()};
        //std::unique_ptr<MobileRT::Sampler> samplerLight {std::make_unique<Components::StaticMersenneTwister>()};

        //std::unique_ptr<MobileRT::Sampler> samplerRussianRoulette {std::make_unique<Components::HaltonSeq>()};
        //std::unique_ptr<MobileRT::Sampler> samplerRussianRoulette {std::make_unique<Components::MersenneTwister> ()};
        std::unique_ptr<MobileRT::Sampler> samplerRussianRoulette {
          std::make_unique<Components::StaticHaltonSeq> ()};
        //std::unique_ptr<MobileRT::Sampler> samplerRussianRoulette {std::make_unique<Components::StaticMersenneTwister> ()};

        shader_ = std::make_unique<Components::PathTracer> (
          std::move (scene_), std::move (samplerRay), std::move (samplerLight),
          std::move (samplerRussianRoulette), samplesLight);
        break;
      }

      case 3: {
        shader_ = std::make_unique<Components::DepthMap> (std::move (scene_),
                                                          MobileRT::Point3D (1, 1, 1));
        break;
      }

      case 4: {
        shader_ = std::make_unique<Components::DiffuseMaterial> (std::move (scene_));
        break;
      }

      default: {
        shader_ = std::make_unique<Components::NoShadows> (std::move (scene_), samplesLight);
        break;
      }
    }
    renderer_ = new MobileRT::Renderer {
      std::move (shader_), std::move (camera), std::move (samplerPixel),
      static_cast<unsigned>(width_), static_cast<unsigned>(height_),
      static_cast<unsigned>(samplesPixel)};

    LOG("TRIANGLES = ", renderer_->shader_->scene_.triangles_.size ());
    LOG("SPHERES = ", renderer_->shader_->scene_.spheres_.size ());
    LOG("PLANES = ", renderer_->shader_->scene_.planes_.size ());
    LOG("LIGHTS = ", renderer_->shader_->scene_.lights_.size ());
    LOG("MATERIALS = ", renderer_->shader_->scene_.materials_.size ());
  }();
  LOG ("threads = ", threads);
  LOG ("shader = ", shader);
  LOG ("scene = ", scene);
  LOG ("samplesPixel = ", samplesPixel);
  LOG ("samplesLight = ", samplesLight);
  LOG ("width_ = ", width_);
  LOG ("height_ = ", height_);
  const double start {omp_get_wtime ()};
  do {
    renderer_->renderFrame (bitmap, threads);
    renderer_->camera_->position_.x_ += 2.0f;
  } while (repeats-- > 1);
  delete renderer_;
  std::cout << "Time in secs = ";
  std::cout << omp_get_wtime () - start << std::endl;
  for (int i (0), j (0); i < width_ * height_ * 4; i += 4, j += 1) {
    const unsigned color {bitmap[j]};
    buffer[i + 0] = static_cast<unsigned char> ((color & 0x000000FF) >> 0);
    buffer[i + 1] = static_cast<unsigned char> ((color & 0x0000FF00) >> 8);
    buffer[i + 2] = static_cast<unsigned char> ((color & 0x00FF0000) >> 16);
    buffer[i + 3] = static_cast<unsigned char> ((color & 0xFF000000) >> 24);
  }
  gtk_init (&argc, &argv);
  GtkWidget *window {gtk_window_new (GTK_WINDOW_TOPLEVEL)};
  GdkPixbuf *pixbuff {
    gdk_pixbuf_new_from_data (buffer, GDK_COLORSPACE_RGB, TRUE, 8,
                              static_cast<int> (width_),
                              static_cast<int> (height_),
                              static_cast<int> (width_ * 4), nullptr, nullptr)};
  GtkWidget *image {gtk_image_new_from_pixbuf (pixbuff)};
  gtk_signal_connect (GTK_OBJECT (window), "destroy", GTK_SIGNAL_FUNC (
    [] () -> void {
      delete[] bitmap;
      delete[] buffer;
      gtk_main_quit ();
    }
  ), nullptr);
  auto *check_escape {static_cast<bool (*) (
    GtkWidget *gtkWidget, GdkEventKey *event, gpointer /*unused*/)>(
                        [] (GtkWidget *gtkWidget, GdkEventKey *event, gpointer /*unused*/) {
                          if (event -> keyval == GDK_KEY_Escape) {
                            gtk_widget_destroy (gtkWidget);
                            gtk_main_quit ();
                            return true;
                          }
                          return false;
                        })
  };
  gtk_signal_connect (GTK_OBJECT (window), "key_press_event", GTK_SIGNAL_FUNC (check_escape),
                      nullptr);
  gtk_container_add (GTK_CONTAINER (window), image);
  gtk_widget_show_all (window);
  gtk_main ();
  g_object_unref (G_OBJECT (pixbuff));
  return argc;
}
