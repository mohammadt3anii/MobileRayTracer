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

    public:
        explicit OBJLoader (std::string text);

        virtual void process();

        virtual bool fillTriangles(std::vector<MobileRT::Triangle> &triangles) noexcept;

        virtual ~OBJLoader() noexcept;
    };
}

#endif //MOBILERAYTRACER_OBJLOADER_HPP
