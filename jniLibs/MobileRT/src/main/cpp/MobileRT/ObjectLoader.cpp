//
// Created by puscas on 30/07/17.
//

#include "ObjectLoader.hpp"

using MobileRT::ObjectLoader;

ObjectLoader::ObjectLoader () noexcept :
	isProcessed_(false)
{
}

ObjectLoader::~ObjectLoader() noexcept {
    LOG("ObjectLoader DELETED");
}

bool ObjectLoader::isProcessed() const noexcept {
    return isProcessed_;
}
