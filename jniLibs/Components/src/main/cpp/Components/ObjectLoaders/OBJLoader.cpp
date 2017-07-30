//
// Created by puscas on 30/07/17.
//

#include "OBJLoader.hpp"

using Components::OBJLoader;

OBJLoader::OBJLoader (std::string text) :
        text_(std::move(text))
{
}

void OBJLoader::process() {
    //std::string inputfile = "cornell_box.obj";
    std::istringstream inputStream (text_);
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;

    std::string err;
    bool ret = tinyobj::LoadObj(&attrib, &shapes, &materials, &err, &inputStream);

    if (!err.empty()) { // `err` may contain warning message.
        std::cerr << err << std::endl;
    }

    if (!ret) {
        return;
    }

    // Loop over shapes
    for (size_t s = 0; s < shapes.size(); s++) {
        // Loop over faces(polygon)
        size_t index_offset = 0;
        for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
            size_t fv = shapes[s].mesh.num_face_vertices[f];

            // Loop over vertices in the face.
            for (size_t v = 0; v < fv; v++) {
                // access to vertex
                tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
                tinyobj::real_t vx = attrib.vertices[static_cast<size_t> (3*idx.vertex_index+0)];
                tinyobj::real_t vy = attrib.vertices[static_cast<size_t> (3*idx.vertex_index+1)];
                tinyobj::real_t vz = attrib.vertices[static_cast<size_t> (3*idx.vertex_index+2)];
                tinyobj::real_t nx = attrib.normals[static_cast<size_t> (3*idx.normal_index+0)];
                tinyobj::real_t ny = attrib.normals[static_cast<size_t> (3*idx.normal_index+1)];
                tinyobj::real_t nz = attrib.normals[static_cast<size_t> (3*idx.normal_index+2)];
                tinyobj::real_t tx = attrib.texcoords[static_cast<size_t> (2*idx.texcoord_index+0)];
                tinyobj::real_t ty = attrib.texcoords[static_cast<size_t> (2*idx.texcoord_index+1)];
            }
            index_offset += fv;

            // per-face material
            shapes[s].mesh.material_ids[f];
        }
    }
    isProcessed_ = true;
}

bool OBJLoader::fillTriangles(std::vector<MobileRT::Triangle> & /*triangles*/) noexcept {
    return false;
}

OBJLoader::~OBJLoader() noexcept {

}
