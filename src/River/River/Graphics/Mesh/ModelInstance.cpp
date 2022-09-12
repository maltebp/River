#include "River/pch.h"
#include "ModelInstance.h"

#include "River/Error.h"

// TODO...

using namespace River;


void ModelInstance::setModel(const Model* model) {
    this->model = model;
    materialOverrides.clear();
}


const Model* ModelInstance::getModel() const {
    return model;
}


void ModelInstance::addMaterialOverride(
    const Material* materialToOverride,
    const Material* overridingMaterial
) {
    if( model == nullptr ) throw InvalidArgumentException("Model has not been set");
    if( materialToOverride == nullptr ) throw InvalidArgumentException("Material to override must not be nullptr");
    if( overridingMaterial == nullptr ) throw InvalidArgumentException("Overriding material must not be null");
    if( !model->hasMaterial(materialToOverride) ) throw InvalidArgumentException("Model does not have overridden material");

    materialOverrides[materialToOverride] = overridingMaterial;
}


void ModelInstance::removeMaterialOverride(const Material* overriddenMaterial) {
    if( overriddenMaterial == nullptr ) throw InvalidArgumentException("Overridden material must not be nullptr");
    materialOverrides.erase(overriddenMaterial);
}


const Material* ModelInstance::getMaterialOverride(const Material* overriddenMaterial) const {
    auto iterator = materialOverrides.find(overriddenMaterial);
    bool overrideExists = iterator != materialOverrides.end();
    return overrideExists ? iterator->second : nullptr;
}