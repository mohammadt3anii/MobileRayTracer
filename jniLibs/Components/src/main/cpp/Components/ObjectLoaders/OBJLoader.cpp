//
// Created by puscas on 30/07/17.
//

#include <tinyobjloader/tiny_obj_loader.h>
#include "OBJLoader.hpp"
#include "../Lights/AreaLight.hpp"
#include "../Samplers/MersenneTwister.hpp"

using Components::AreaLight;
using Components::OBJLoader;
using MobileRT::Material;
using MobileRT::Point3D;
using MobileRT::RGB;
using MobileRT::Scene;

OBJLoader::OBJLoader(const std::string &text, const std::string &materials) :
        text_(text),
        materialsText_(materials)
{
}

void OBJLoader::process() {
    std::istringstream inputStream (text_);
    std::istringstream matStream(materialsText_);

    tinyobj::MaterialStreamReader materialStreamReader(matStream);
    tinyobj::MaterialStreamReader *materialReader = &materialStreamReader;
    std::string err;
    bool ret = tinyobj::LoadObj(&attrib_, &shapes_, &materials_, &err, &inputStream, materialReader,
                                true);

    if (!err.empty()) { // `err` may contain warning message.
        std::cerr << err << std::endl;
    }

    if (!ret) {
        return;
    }

    // Loop over shapes
    for (size_t s = 0; s < shapes_.size(); s++) {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes_[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = shapes_[s].mesh.num_face_vertices[f];

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                // access to vertex
                tinyobj::index_t idx = shapes_[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib_.vertices[static_cast<size_t> (3 * idx.vertex_index +
                                                                           0)];
                tinyobj::real_t vy = attrib_.vertices[static_cast<size_t> (3 * idx.vertex_index +
                                                                           1)];
                tinyobj::real_t vz = attrib_.vertices[static_cast<size_t> (3 * idx.vertex_index +
                                                                           2)];
                /*tinyobj::real_t nx = attrib.normals[static_cast<size_t> (3 * idx.normal_index +
                                                                             0)];
                tinyobj::real_t ny = attrib.normals[static_cast<size_t> (3 * idx.normal_index +
                                                                             1)];
                tinyobj::real_t nz = attrib.normals[static_cast<size_t> (3 * idx.normal_index +
                                                                             2)];*/
                /*tinyobj::real_t tx = attrib_.texcoords[static_cast<size_t> (
                            2 * idx.texcoord_index + 0)];
                tinyobj::real_t ty = attrib_.texcoords[static_cast<size_t> (
                            2 * idx.texcoord_index + 1)];*/
            }
            index_offset += fv;

            // per-face material
            shapes_[s].mesh.material_ids[f];
        }
    }
    isProcessed_ = true;
}

bool OBJLoader::fillTriangles(Scene &scene) noexcept {
    for (size_t s = 0; s < shapes_.size(); s++) {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes_[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = shapes_[s].mesh.num_face_vertices[f];

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v += 3) {
                tinyobj::index_t idx1 = shapes_[s].mesh.indices[index_offset + v + 0];
                tinyobj::real_t vx1 = attrib_.vertices[static_cast<size_t> (3 * idx1.vertex_index +
                                                                            0)];
                tinyobj::real_t vy1 = attrib_.vertices[static_cast<size_t> (3 * idx1.vertex_index +
                                                                            1)];
                tinyobj::real_t vz1 = attrib_.vertices[static_cast<size_t> (3 * idx1.vertex_index +
                                                                            2)];
                Point3D vertex1(vx1, vy1, vz1);

                tinyobj::index_t idx2 = shapes_[s].mesh.indices[index_offset + v + 1];
                tinyobj::real_t vx2 = attrib_.vertices[static_cast<size_t> (3 * idx2.vertex_index +
                                                                            0)];
                tinyobj::real_t vy2 = attrib_.vertices[static_cast<size_t> (3 * idx2.vertex_index +
                                                                            1)];
                tinyobj::real_t vz2 = attrib_.vertices[static_cast<size_t> (3 * idx2.vertex_index +
                                                                            2)];
                Point3D vertex2(vx2, vy2, vz2);

                tinyobj::index_t idx3 = shapes_[s].mesh.indices[index_offset + v + 2];
                tinyobj::real_t vx3 = attrib_.vertices[static_cast<size_t> (3 * idx3.vertex_index +
                                                                            0)];
                tinyobj::real_t vy3 = attrib_.vertices[static_cast<size_t> (3 * idx3.vertex_index +
                                                                            1)];
                tinyobj::real_t vz3 = attrib_.vertices[static_cast<size_t> (3 * idx3.vertex_index +
                                                                            2)];
                Point3D vertex3(vx3, vy3, vz3);

                // per-face material
                int materialID = shapes_[s].mesh.material_ids[f];
                if (materialID >= 0) {
                    tinyobj::material_t m = materials_[static_cast<size_t> (materialID)];
                    float d1 = m.diffuse[0];
                    float d2 = m.diffuse[1];
                    float d3 = m.diffuse[2];
                    RGB diffuse(d1, d2, d3);
                    float s1 = m.specular[0];
                    float s2 = m.specular[1];
                    float s3 = m.specular[2];
                    RGB specular(s1, s2, s3);
                    float t1 = m.transmittance[0];
                    float t2 = m.transmittance[1];
                    float t3 = m.transmittance[2];
                    RGB transmittance(t1, t2, t3);
                    float e1 = m.emission[0];
                    float e2 = m.emission[1];
                    float e3 = m.emission[2];
                    float max = std::max(std::max(e1, e2), e3);
                    RGB emission(e1 / max, e2 / max, e3 / max);
                    Material material(diffuse, specular, transmittance, 1.0f, emission);
                    if (e1 > 0.0f || e2 > 0.0f || e3 > 0.0f) {
                        Point3D p1(vx1, vy1, vz1);
                        Point3D p2(vx2, vy2, vz2);
                        Point3D p3(vx3, vy3, vz3);
                        Sampler *sampler = new Components::MersenneTwister(90000000, 32);
                        scene.lights_.emplace_back(new AreaLight(material, sampler, p1, p2, p3));
                    } else {
                        scene.triangles_.emplace_back(vertex1, vertex2, vertex3);
                        scene.materials_.emplace_back(material);
                    }
                }
            }
            index_offset += fv;
        }
    }

    return false;
}

OBJLoader::~OBJLoader() noexcept {

}
