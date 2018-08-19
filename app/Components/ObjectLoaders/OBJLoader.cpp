//
// Created by puscas on 30/07/17.
//

#include "Components/ObjectLoaders/OBJLoader.hpp"
#include "Components/Lights/AreaLight.hpp"
#include <cstring>
#include <fstream>
#include <tinyobjloader/tiny_obj_loader.h>

using ::Components::AreaLight;
using ::Components::OBJLoader;
using ::MobileRT::Material;
using ::MobileRT::Scene;

OBJLoader::OBJLoader(::std::string obj, ::std::string materials) noexcept :
        objFilePath_{::std::move(obj)},
        mtlFilePath_{::std::move(materials)} {
}

void OBJLoader::process() noexcept {
    ::std::ifstream objStream {objFilePath_};
    objStream.exceptions(::std::ifstream::goodbit | ::std::ifstream::badbit);
    ::std::ifstream matStream {mtlFilePath_};
    matStream.exceptions(::std::ifstream::goodbit | ::std::ifstream::badbit);
    ::tinyobj::MaterialStreamReader matStreamReader {matStream};
    ::tinyobj::MaterialStreamReader *const matStreamReaderPtr {!mtlFilePath_.empty()? &matStreamReader : nullptr};
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
                          ::std::function<::std::unique_ptr<MobileRT::Sampler>()> lambda) noexcept {
    size_t numberTriangles{0};

    for (const auto &shape : shapes_) {
        for (size_t f{0}; f < shape.mesh.num_face_vertices.size(); ++f) {
            const size_t triangles{static_cast<size_t>(shape.mesh.num_face_vertices.at(f) / 3)};
            numberTriangles += triangles;
        }
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
                const ::MobileRT::Triangle triangle {vertex1, vertex2, vertex3};

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
                    const ::glm::vec3 &specular{s1 / 2.0f, s2 / 2.0f, s3 / 2.0f};
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
                    const Material material {diffuse, specular, transmittance, indexRefraction,
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
                    const Material material {diffuse, specular, transmittance, indexRefraction,
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
    this->objFilePath_.clear();
    this->mtlFilePath_.clear();
    this->attrib_.normals.clear();
    this->attrib_.texcoords.clear();
    this->attrib_.vertices.clear();
    this->shapes_.clear();
    this->materials_.clear();

    this->objFilePath_.shrink_to_fit();
    this->mtlFilePath_.shrink_to_fit();
    this->attrib_.normals.shrink_to_fit();
    this->attrib_.texcoords.shrink_to_fit();
    this->attrib_.vertices.shrink_to_fit();
    this->shapes_.shrink_to_fit();
    this->materials_.shrink_to_fit();

    ::std::vector<::tinyobj::shape_t>{}.swap(this->shapes_);
    ::std::vector<::tinyobj::material_t>{}.swap(this->materials_);

    LOG("OBJLOADER DELETED");
}
