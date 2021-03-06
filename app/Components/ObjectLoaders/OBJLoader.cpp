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

::std::int32_t OBJLoader::process() noexcept {
    ::std::ifstream objStream {objFilePath_};
    objStream.exceptions(::std::ifstream::goodbit | ::std::ifstream::badbit);
    ::std::ifstream matStream {mtlFilePath_};
    matStream.exceptions(::std::ifstream::goodbit | ::std::ifstream::badbit);
    ::tinyobj::MaterialStreamReader matStreamReader {matStream};
    ::tinyobj::MaterialStreamReader *const matStreamReaderPtr {!mtlFilePath_.empty()? &matStreamReader : nullptr};
    ::std::string errors {};
    ::std::string warnings {};
    errno = 0;

    LOG("Going to call tinyobj::LoadObj");
    const bool ret {
            ::tinyobj::LoadObj(&attrib_, &shapes_, &materials_, &warnings, &errors, &objStream,
                               matStreamReaderPtr, true)};
    LOG("Called tinyobj::LoadObj");

    if (!errors.empty()) {
        LOG("Error: ", errors);
    }

    if (!warnings.empty()) {
        LOG("Warning: ", warnings);
    }

    if (errno) {
        const ::std::string &strError{::std::strerror(errno)};
        LOG("Error (errno): ", strError);
    }

    if (ret) {
        isProcessed_ = true;
        numberTriangles_ = 0;
        for (const auto &shape : shapes_) {
            for (const auto num_face_vertice : shape.mesh.num_face_vertices) {
                const ::std::size_t triangles{static_cast<::std::size_t> (num_face_vertice / 3)};
                numberTriangles_ += triangles;
            }
        }
    } else {
        isProcessed_ = false;
        numberTriangles_ = -1;
    }

    return numberTriangles_;
}

bool OBJLoader::fillScene(Scene *const scene,
                          ::std::function<::std::unique_ptr<MobileRT::Sampler> ()> lambda) noexcept {
    scene->triangles_.reserve(static_cast<::std::size_t> (numberTriangles_));

    for (const auto &shape : shapes_) {
        // Loop over faces(polygon)
        ::std::size_t index_offset{0};
        for (::std::size_t f{0}; f < shape.mesh.num_face_vertices.size(); ++f) {
            const auto it {shape.mesh.num_face_vertices.begin() + static_cast<::std::int32_t> (f)};
            const ::std::size_t fv {*it};

            if (fv % 3 != 0) {
                LOG("num_face_vertices [", f, "] = ", fv);
                continue;
            }

            // Loop over vertices in the face.
            for (::std::size_t v{0}; v < fv; v += 3) {
                const auto itIdx {shape.mesh.indices.begin() + static_cast<::std::int32_t> (index_offset + v)};

                const ::tinyobj::index_t idx1 {*(itIdx + 0)};
                const auto itV1 {attrib_.vertices.begin() + 3 * idx1.vertex_index};
                const ::tinyobj::real_t vx1 {*(itV1 + 0)};
                const ::tinyobj::real_t vy1 {*(itV1 + 1)};
                const ::tinyobj::real_t vz1 {*(itV1 + 2)};

                const ::tinyobj::index_t idx2 {*(itIdx + 1)};
                const auto itV2 {attrib_.vertices.begin() + 3 * idx2.vertex_index};
                const ::tinyobj::real_t vx2 {*(itV2 + 0)};
                const ::tinyobj::real_t vy2 {*(itV2 + 1)};
                const ::tinyobj::real_t vz2 {*(itV2 + 2)};

                const ::tinyobj::index_t idx3 {*(itIdx + 2)};
                const auto itV3 {attrib_.vertices.begin() + 3 * idx3.vertex_index};
                const ::tinyobj::real_t vx3 {*(itV3 + 0)};
                const ::tinyobj::real_t vy3 {*(itV3 + 1)};
                const ::tinyobj::real_t vz3 {*(itV3 + 2)};

                const ::glm::vec3 &vertex1 {-vx1, vy1, vz1};
                const ::glm::vec3 &vertex2 {-vx2, vy2, vz2};
                const ::glm::vec3 &vertex3 {-vx3, vy3, vz3};

                ::glm::vec3 normal1 {};
                ::glm::vec3 normal2 {};
                ::glm::vec3 normal3 {};
                if (!attrib_.normals.empty()) {
                    const auto itNormal1 {attrib_.normals.begin() + 3 * static_cast<::std::int32_t> (idx1.normal_index)};
                    const ::tinyobj::real_t nx1 {*(itNormal1 + 0)};
                    const ::tinyobj::real_t ny1 {*(itNormal1 + 1)};
                    const ::tinyobj::real_t nz1 {*(itNormal1 + 2)};

                    const auto itNormal2 {attrib_.normals.begin() + 3 * static_cast<::std::int32_t> (idx2.normal_index)};
                    const ::tinyobj::real_t nx2 {*(itNormal2 + 0)};
                    const ::tinyobj::real_t ny2 {*(itNormal2 + 1)};
                    const ::tinyobj::real_t nz2 {*(itNormal2 + 2)};

                    const auto itNormal3 {attrib_.normals.begin() + 3 * static_cast<::std::int32_t> (idx3.normal_index)};
                    const ::tinyobj::real_t nx3 {*(itNormal3 + 0)};
                    const ::tinyobj::real_t ny3 {*(itNormal3 + 1)};
                    const ::tinyobj::real_t nz3 {*(itNormal3 + 2)};

                    normal1 = ::glm::normalize(::glm::vec3 {-nx1, ny1, nz1});
                    normal2 = ::glm::normalize(::glm::vec3 {-nx2, ny2, nz2});
                    normal3 = ::glm::normalize(::glm::vec3 {-nx3, ny3, nz3});
                }
                const float normalDist1 {::glm::length(normal1)};
                const float normalDist2 {::glm::length(normal2)};
                const float normalDist3 {::glm::length(normal3)};
                const bool hasNormal {normalDist1 > 0 && normalDist2 > 0 && normalDist3 > 0};

                const auto itColor1 {attrib_.colors.begin() + 3 * static_cast<::std::int32_t> (idx1.vertex_index)};
                const ::tinyobj::real_t red1 {*(itColor1 + 0)};
                const ::tinyobj::real_t green1 {*(itColor1 + 1)};
                const ::tinyobj::real_t blue1 {*(itColor1 + 2)};

                const auto itColor2 {attrib_.colors.begin() + 3 * static_cast<::std::int32_t> (idx2.vertex_index)};
                const ::tinyobj::real_t red2 {*(itColor2 + 0)};
                const ::tinyobj::real_t green2 {*(itColor2 + 1)};
                const ::tinyobj::real_t blue2 {*(itColor2 + 2)};

                const auto itColor3 {attrib_.colors.begin() + 3 * static_cast<::std::int32_t> (idx3.vertex_index)};
                const ::tinyobj::real_t red3 {*(itColor3 + 0)};
                const ::tinyobj::real_t green3 {*(itColor3 + 1)};
                const ::tinyobj::real_t blue3 {*(itColor3 + 2)};

                const ::glm::vec3 &color1 {red1, green1, blue1};
                const ::glm::vec3 &color2 {red2, green2, blue2};
                const ::glm::vec3 &color3 {red3, green3, blue3};

                /*if (!attrib_.texcoords.empty()) {
                    const auto itTexCoords {attrib_.texcoords.begin() + 2 * static_cast<::std::int32_t> (idx1.texcoord_index)};
                    ::tinyobj::real_t tx {*(itTexCoords + 0)};
                    ::tinyobj::real_t ty {*(itTexCoords + 1)};
                    LOG(tx, ty);
                }*/

                // per-face material
                const auto itMaterial {shape.mesh.material_ids.begin() + static_cast<::std::int32_t> (f)};
                const ::std::int32_t materialID {*itMaterial};
                if (materialID >= 0) {
                    const auto itMaterial2 {materials_.begin() + static_cast<::std::int32_t> (materialID)};
                    const ::tinyobj::material_t &m {*itMaterial2};
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
                        scene->lights_.emplace_back(
                                ::std::make_unique<AreaLight> (material, lambda(), vertex1, vertex2, vertex3));
                    } else {
                        ::std::vector<::MobileRT::Material>::iterator itMat {
                            ::std::find(scene->materials_.begin(), scene->materials_.end(), material)};
                        if (itMat != scene->materials_.end()) {
                            if (hasNormal) {
                                const ::std::int32_t index {static_cast<int32_t>(
                                    ::std::distance(scene->materials_.begin(), itMat))};
                                const ::MobileRT::Triangle triangle {vertex1, vertex2, vertex3, index,
                                    normal1, normal2, normal3, color1, color2, color3};
                                scene->triangles_.emplace_back(triangle);
                            } else {
                                const ::std::int32_t index {static_cast<int32_t>(
                                    ::std::distance(scene->materials_.begin(), itMat))};
                                const ::MobileRT::Triangle triangle {vertex1, vertex2, vertex3, index,
                                    color1, color2, color3};
                                scene->triangles_.emplace_back(triangle);
                            }
                        } else {
                            if (hasNormal) {
                                const ::MobileRT::Triangle triangle {vertex1, vertex2, vertex3,
                                    static_cast<int32_t>(scene->materials_.size()),
                                    normal1, normal2, normal3, color1, color2, color3};
                                scene->materials_.emplace_back(material);
                                scene->triangles_.emplace_back(triangle);
                            } else {
                                const ::MobileRT::Triangle triangle {vertex1, vertex2, vertex3,
                                    static_cast<int32_t>(scene->materials_.size()),
                                    color1, color2, color3};
                                scene->materials_.emplace_back(material);
                                scene->triangles_.emplace_back(triangle);
                            }
                        }
                    }
                } else {
                    if (hasNormal) {
                        const ::MobileRT::Triangle triangle {vertex1, vertex2, vertex3, -1,
                            normal1, normal2, normal3, color1, color2, color3};
                        scene->triangles_.emplace_back(triangle);
                    } else {
                        const ::MobileRT::Triangle triangle {vertex1, vertex2, vertex3, -1,
                            color1, color2, color3};
                        scene->triangles_.emplace_back(triangle);
                    }
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

    ::std::vector<::tinyobj::shape_t> {}.swap(this->shapes_);
    ::std::vector<::tinyobj::material_t> {}.swap(this->materials_);

    LOG("OBJLOADER DELETED");
}
