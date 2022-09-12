#pragma once

#include <unordered_map>

#include "Model.h"


namespace River {

    // TODO: Convert to entity
    
    class ModelInstance {
    public:

        void setModel(const Model* model);

        const Model* getModel() const;

        void addMaterialOverride(const Material* materialToOverride, const Material* overridingMaterial);

        void removeMaterialOverride(const Material* overriddenMaterial);

        /**
         * @return  The material that overrides the given material, or nullptr if the material has
         *          not been overridden.
         */
        const Material* getMaterialOverride(const Material* overriddenMaterial) const;

    private:

        const Model* model;

        std::unordered_map<const Material*, const Material*> materialOverrides;

    };

}