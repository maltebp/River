#pragma once

#include <vector>
#include <set>

#include "River/Graphics/Mesh/Mesh.h"
#include "River/Graphics/Mesh/Material.h"


namespace River {

    // TODO: Add mesh builder

    class Model {
    public:

        void addMesh(const Mesh* mesh, const Material* material);

        const std::vector<std::tuple<const Mesh*, const Material*>>& getMeshes() const;

        bool hasMaterial(const Material* material) const;

    private:

        std::vector<std::tuple<const Mesh*, const Material*>> meshesWithMaterials;

    };

}