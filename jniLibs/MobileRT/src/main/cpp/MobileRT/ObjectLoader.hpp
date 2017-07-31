//
// Created by puscas on 30/07/17.
//

#ifndef MOBILERAYTRACER_OBJECTLOADER_HPP
#define MOBILERAYTRACER_OBJECTLOADER_HPP

#include "Scene.hpp"
#include "Shapes/Triangle.hpp"
#include <string>

namespace MobileRT {
    class ObjectLoader {
    protected:
        bool isProcessed_{false};

    public:
        //explicit ObjectLoader (const std::string text) = delete;
        virtual void process() = 0;

        bool isProcessed() const noexcept;

        virtual bool fillTriangles(MobileRT::Scene &scene) noexcept = 0;

        virtual ~ObjectLoader() noexcept;
    };
}

#endif //MOBILERAYTRACER_OBJECTLOADER_HPP
