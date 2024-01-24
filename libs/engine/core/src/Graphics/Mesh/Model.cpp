#include "River/pch.h"
#include "Model.h"

#include "River/Error.h"

using namespace River;


void Model::addMesh(const Mesh* mesh, const Material* material) {
    for( auto [existingMesh, existingMaterial] : meshesWithMaterials ) {
        if( mesh == existingMesh ) {
            throw InvalidArgumentException("Model already contains mesh");
        }
    }

    meshesWithMaterials.push_back({mesh, material});        
}


const std::vector<std::tuple<const Mesh*, const Material*>>& Model::getMeshes() const {
    return meshesWithMaterials;
}

    
bool Model::hasMaterial(const Material* materialToCheck) const {
    for( auto [mesh, material] : meshesWithMaterials ) {
        if( materialToCheck == material ) return true;
    }
    return false;
}
