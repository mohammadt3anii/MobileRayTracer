//
// Created by puscas on 30/07/17.
//

#ifndef MOBILERT_OBJECTLOADER_HPP
#define MOBILERT_OBJECTLOADER_HPP

#include "MobileRT/Sampler.hpp"
#include "MobileRT/Scene.hpp"
#include "MobileRT/Shapes/Triangle.hpp"
#include <jni.h>
#include <memory>
#include <string>

namespace MobileRT {
    class ObjectLoader {
    protected:
        bool isProcessed_{false};

    public:
        explicit ObjectLoader() noexcept = default;

        ObjectLoader(const ObjectLoader &objectLoader) noexcept = delete;

        ObjectLoader(ObjectLoader &&objectLoader) noexcept = delete;

        virtual ~ObjectLoader() noexcept;

        ObjectLoader &operator=(const ObjectLoader &objectLoader) noexcept = delete;

        ObjectLoader &operator=(ObjectLoader &&objectLoader) noexcept = delete;

        virtual void process() noexcept = 0;

        bool isProcessed() const noexcept;

        virtual bool fillScene(Scene *scene,
                               ::std::function<::std::unique_ptr<Sampler>()> lambda,
                               JNIEnv *env) noexcept = 0;
    };
}//namespace MobileRT

#endif //MOBILERT_OBJECTLOADER_HPP
