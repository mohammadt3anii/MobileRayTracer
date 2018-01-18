//
// Created by puscas on 30/07/17.
//

#ifndef MOBILERT_OBJECTLOADER_HPP
#define MOBILERT_OBJECTLOADER_HPP

#include "Sampler.hpp"
#include "Scene.hpp"
#include "Shapes/Triangle.hpp"
#include <memory>
#include <string>

namespace MobileRT {
    class ObjectLoader {
    protected:
			bool isProcessed_ {false};

    public:
			explicit ObjectLoader () noexcept = default;

				ObjectLoader(const ObjectLoader &objectLoader) noexcept = delete;

				ObjectLoader(ObjectLoader &&objectLoader) noexcept = delete;

				virtual ~ObjectLoader() noexcept;

				ObjectLoader &operator=(const ObjectLoader &objectLoader) noexcept = delete;

				ObjectLoader &operator=(ObjectLoader &&objectLoader) noexcept = delete;

        virtual void process() noexcept = 0;

        bool isProcessed() const noexcept;

        virtual bool fillScene(::MobileRT::Scene *scene, ::std::function<::std::unique_ptr<::MobileRT::Sampler> ()> lambda) noexcept = 0;
    };
}//namespace MobileRT

#endif //MOBILERT_OBJECTLOADER_HPP
