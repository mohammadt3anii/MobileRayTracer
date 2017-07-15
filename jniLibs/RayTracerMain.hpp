#ifndef RAYTRACERMAIN_HPP
#define RAYTRACERMAIN_HPP

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

static MobileRT::Scene *cornellBoxScene();

static MobileRT::Scene *cornellBoxScene2();

static MobileRT::Scene *spheresScene();

static MobileRT::Scene *spheresScene2();

static void destroy(GtkWidget* /*unused*/, gpointer /*unused*/);

static bool check_escape(GtkWidget* /*unused*/, GdkEventKey *event, gpointer /*unused*/);

#endif //RAYTRACERMAIN_HPP
