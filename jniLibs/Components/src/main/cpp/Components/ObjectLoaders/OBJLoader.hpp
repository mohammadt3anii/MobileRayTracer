//
// Created by puscas on 30/07/17.
//

#ifndef MOBILERAYTRACER_OBJLOADER_HPP
#define MOBILERAYTRACER_OBJLOADER_HPP

#include "ObjectLoader.hpp"
#include "tinyobjloader/tiny_obj_loader.h"

namespace Components {
    class OBJLoader : public ObjectLoader {
    private:
        std::string text_;
        std::string materialsText_;
        tinyobj::attrib_t attrib_;
        std::vector<tinyobj::shape_t> shapes_;
        std::vector<tinyobj::material_t> materials_;

    public:
        explicit OBJLoader(const std::string &text, const std::string &materials);

        virtual void process();

        virtual bool fillTriangles(MobileRT::Scene &triangles) noexcept;

        virtual ~OBJLoader() noexcept;
    };
}

#endif //MOBILERAYTRACER_OBJLOADER_HPP
