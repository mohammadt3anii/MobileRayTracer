//
// Created by puscas on 30/07/17.
//

#include <fstream>
#include <tinyobjloader/tiny_obj_loader.h>
#include "Components/ObjectLoaders/OBJLoader.hpp"
#include "Components/Lights/AreaLight.hpp"
#include "JNI/DrawView.hpp"

using ::Components::AreaLight;
using ::Components::OBJLoader;
using ::MobileRT::Material;
using ::MobileRT::Scene;

OBJLoader::OBJLoader(const ::std::string &obj, const ::std::string &materials) noexcept :
        objText_{obj},
        materialsText_{materials} {
}

void OBJLoader::process() noexcept {
    ::std::ifstream objStream{objText_};
    objStream.exceptions(::std::ifstream::goodbit | ::std::ifstream::badbit);
    ::std::ifstream matStream{materialsText_};
    matStream.exceptions(::std::ifstream::goodbit | ::std::ifstream::badbit);
    ::tinyobj::MaterialStreamReader matStreamReader {matStream};
    ::tinyobj::MaterialStreamReader *const matStreamReaderPtr {!materialsText_.empty()? &matStreamReader : nullptr};
    ::std::string err {};
    errno = 0;

    const bool ret {
            ::tinyobj::LoadObj(&attrib_, &shapes_, &materials_, &err, &objStream,
                               matStreamReaderPtr, true)};

    if (!err.empty()) {
        LOG("Error: ", err);
    }

    if (errno) {
        LOG("Error (errno): ", ::std::strerror(errno));
    }

    if (ret) {
        isProcessed_ = true;
    }
}

bool OBJLoader::fillScene(Scene *const scene,
                          ::std::function<::std::unique_ptr<MobileRT::Sampler>()> lambda,
                          JNIEnv *const env) noexcept {
    size_t numberTriangles{0};

    for (const auto &shape : shapes_) {
        for (size_t f{0}; f < shape.mesh.num_face_vertices.size(); ++f) {
            const size_t triangles{static_cast<size_t>(shape.mesh.num_face_vertices.at(f) / 3)};
            numberTriangles += triangles;
        }
    }

    const jclass mainActivityClass{env->FindClass("puscas/mobilertapp/MainActivity")};
    const jmethodID mainActivityMethodId{
            env->GetStaticMethodID(mainActivityClass, "getFreeMemStatic", "(I)Z")};
    const jint needMem{(static_cast<jint>(numberTriangles * 3 * 3 * sizeof(float) +
                                          numberTriangles * 13 * sizeof(float))) / 1048576};
    const jboolean result{
            env->CallStaticBooleanMethod(mainActivityClass, mainActivityMethodId, needMem)};
    if (result) {
        return false;
    }

    scene->triangles_.reserve(numberTriangles);

    for (const auto &shape : shapes_) {
        // Loop over faces(polygon)
        size_t index_offset{0};
        for (size_t f{0}; f < shape.mesh.num_face_vertices.size(); ++f) {
            const size_t fv{shape.mesh.num_face_vertices.at(f)};

            if (fv % 3 != 0) {
                LOG("num_face_vertices [", f, "] = ", fv);
                continue;
            }

            // Loop over vertices in the face.
            for (size_t v{0}; v < fv; v += 3) {
                const ::tinyobj::index_t idx1{shape.mesh.indices.at(index_offset + v + 0)};
                const ::tinyobj::real_t vx1{
                        attrib_.vertices.at(3 * static_cast<size_t> (idx1.vertex_index) + 0)};
                const ::tinyobj::real_t vy1{
                        attrib_.vertices.at(3 * static_cast<size_t> (idx1.vertex_index) + 1)};
                const ::tinyobj::real_t vz1{
                        attrib_.vertices.at(3 * static_cast<size_t> (idx1.vertex_index) + 2)};

                const ::tinyobj::real_t red {
                        attrib_.colors.at(3 * static_cast<size_t> (idx1.vertex_index) + 0)};
                const ::tinyobj::real_t green {
                        attrib_.colors.at(3 * static_cast<size_t> (idx1.vertex_index) + 1)};
                const ::tinyobj::real_t blue {
                        attrib_.colors.at(3 * static_cast<size_t> (idx1.vertex_index) + 2)};

                /*::tinyobj::real_t tx {0};
                ::tinyobj::real_t ty {0};
                if (!attrib_.texcoords.empty()) {
                    tx = attrib_.texcoords.at(2 * static_cast<size_t> (idx1.texcoord_index) + 0);
                    ty = attrib_.texcoords.at(2 * static_cast<size_t> (idx1.texcoord_index) + 1);
                    LOG(tx, ty);
                }*/


                const ::tinyobj::index_t idx2{shape.mesh.indices.at(index_offset + v + 1)};
                const ::tinyobj::real_t vx2{
                        attrib_.vertices.at(3 * static_cast<size_t> (idx2.vertex_index) + 0)};
                const ::tinyobj::real_t vy2{
                        attrib_.vertices.at(3 * static_cast<size_t> (idx2.vertex_index) + 1)};
                const ::tinyobj::real_t vz2{
                        attrib_.vertices.at(3 * static_cast<size_t> (idx2.vertex_index) + 2)};

                const ::tinyobj::index_t idx3{shape.mesh.indices.at(index_offset + v + 2)};
                const ::tinyobj::real_t vx3{
                        attrib_.vertices.at(3 * static_cast<size_t> (idx3.vertex_index) + 0)};
                const ::tinyobj::real_t vy3{
                        attrib_.vertices.at(3 * static_cast<size_t> (idx3.vertex_index) + 1)};
                const ::tinyobj::real_t vz3{
                        attrib_.vertices.at(3 * static_cast<size_t> (idx3.vertex_index) + 2)};

                const ::glm::vec3 &vertex1 {-vx1, vy1, vz1};
                const ::glm::vec3 &vertex2 {-vx2, vy2, vz2};
                const ::glm::vec3 &vertex3 {-vx3, vy3, vz3};
                ::glm::vec3 normal {};
                if (!attrib_.normals.empty()) {
                    const ::tinyobj::real_t nx1 {
                            attrib_.normals.at(3 * static_cast<size_t> (idx1.normal_index) + 0)};
                    const ::tinyobj::real_t ny1 {
                            attrib_.normals.at(3 * static_cast<size_t> (idx1.normal_index) + 1)};
                    const ::tinyobj::real_t nz1 {
                            attrib_.normals.at(3 * static_cast<size_t> (idx1.normal_index) + 2)};

                    const ::tinyobj::real_t nx2 {
                            attrib_.normals.at(3 * static_cast<size_t> (idx2.normal_index) + 0)};
                    const ::tinyobj::real_t ny2 {
                            attrib_.normals.at(3 * static_cast<size_t> (idx2.normal_index) + 1)};
                    const ::tinyobj::real_t nz2 {
                            attrib_.normals.at(3 * static_cast<size_t> (idx2.normal_index) + 2)};

                    const ::tinyobj::real_t nx3 {
                            attrib_.normals.at(3 * static_cast<size_t> (idx3.normal_index) + 0)};
                    const ::tinyobj::real_t ny3 {
                            attrib_.normals.at(3 * static_cast<size_t> (idx3.normal_index) + 1)};
                    const ::tinyobj::real_t nz3 {
                            attrib_.normals.at(3 * static_cast<size_t> (idx3.normal_index) + 2)};

                    const ::glm::vec3 &normal1 {nx1, ny1, nz1};
                    const ::glm::vec3 &normal2 {nx2, ny2, nz2};
                    const ::glm::vec3 &normal3 {nx3, ny3, nz3};
                    normal = ::glm::normalize((normal1 + normal2 + normal3) / 3.0f);
                }
                const ::MobileRT::Triangle &triangle {vertex1, vertex2, vertex3, normal};

                // per-face material
                const ::std::int32_t materialID{shape.mesh.material_ids.at(f)};
                if (materialID >= 0) {
                    const ::tinyobj::material_t &m {materials_.at(static_cast<size_t> (materialID))};
                    const float d1 {m.diffuse[0]};
                    const float d2 {m.diffuse[1]};
                    const float d3 {m.diffuse[2]};
                    const ::glm::vec3 &diffuse {d1, d2, d3};
                    const float s1 {m.specular[0]};
                    const float s2 {m.specular[1]};
                    const float s3 {m.specular[2]};
                    const ::glm::vec3 &specular{s1 / 10.0f, s2 / 10.0f, s3 / 10.0f};
                    const float t1 {m.transmittance[0] * (1.0f - m.dissolve)};
                    const float t2 {m.transmittance[1] * (1.0f - m.dissolve)};
                    const float t3 {m.transmittance[2] * (1.0f - m.dissolve)};
                    const ::glm::vec3 &transmittance {t1, t2, t3};
                    float e1 {m.emission[0]};
                    float e2 {m.emission[1]};
                    float e3 {m.emission[2]};
                    const float max {::std::max(::std::max(e1, e2), e3)};
                    if (max > 1.0f) {
                        e1 /= max;
                        e2 /= max;
                        e3 /= max;
                    }
                    const ::glm::vec3 &emission {e1, e2, e3};
                    const float indexRefraction{m.ior};
                    const Material &material{diffuse, specular, transmittance, indexRefraction,
                                             emission};
                    if (e1 > 0.0f || e2 > 0.0f || e3 > 0.0f) {
                        const ::glm::vec3 &p1 {vx1, vy1, vz1};
                        const ::glm::vec3 &p2 {vx2, vy2, vz2};
                        const ::glm::vec3 &p3 {vx3, vy3, vz3};
                        scene->lights_.emplace_back(
                                ::std::make_unique<AreaLight>(material, lambda(), p1, p2, p3));
                    } else {
                        scene->triangles_.emplace_back(triangle, material);
                    }
                } else {
                    const ::glm::vec3 &diffuse{red, green, blue};
                    const ::glm::vec3 &specular{0.0f, 0.0f, 0.0f};
                    const ::glm::vec3 &transmittance{0.0f, 0.0f, 0.0f};
                    const float indexRefraction{1.0f};
                    const ::glm::vec3 &emission{0.0f, 0.0f, 0.0f};
                    const Material &material{diffuse, specular, transmittance, indexRefraction,
                                             emission};
                    scene->triangles_.emplace_back(triangle, material);
                }
            }
            index_offset += fv;
        }
    }

    return true;
}

OBJLoader::~OBJLoader() noexcept {
    this->objText_.clear();
    this->materialsText_.clear();
    this->attrib_.normals.clear();
    this->attrib_.texcoords.clear();
    this->attrib_.vertices.clear();
    this->shapes_.clear();
    this->materials_.clear();

    /*this->objText_.shrink_to_fit();
    this->materialsText_.shrink_to_fit();
    this->attrib_.normals.shrink_to_fit();
    this->attrib_.texcoords.shrink_to_fit();
    this->attrib_.vertices.shrink_to_fit();
    this->shapes_.shrink_to_fit();
    this->materials_.shrink_to_fit();*/

    /*::std::vector<::tinyobj::shape_t>{}.swap(this->shapes_);
    ::std::vector<::tinyobj::material_t>{}.swap(this->materials_);*/

    LOG("OBJLOADER DELETED");
}
