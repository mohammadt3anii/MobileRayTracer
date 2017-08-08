//
// Created by puscas on 30/07/17.
//

#ifndef MOBILERT_OBJECTLOADER_HPP
#define MOBILERT_OBJECTLOADER_HPP

#include "Scene.hpp"
#include "Shapes/Triangle.hpp"
#include <string>

namespace MobileRT {
    class ObjectLoader {
    protected:
      bool isProcessed_ {};

    public:
      	explicit ObjectLoader () noexcept;

				ObjectLoader(const ObjectLoader &objectLoader) noexcept = delete;

				ObjectLoader(ObjectLoader &&objectLoader) noexcept = delete;

				virtual ~ObjectLoader() noexcept;

				ObjectLoader &operator=(const ObjectLoader &objectLoader) noexcept = delete;

				ObjectLoader &operator=(ObjectLoader &&objectLoader) noexcept = delete;

        virtual void process() noexcept = 0;

        bool isProcessed() const noexcept;

        virtual bool fillScene(MobileRT::Scene *scene) noexcept = 0;
    };
}//namespace MobileRT

#endif //MOBILERT_OBJECTLOADER_HPP
