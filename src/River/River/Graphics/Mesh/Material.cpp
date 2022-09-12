#include "River/pch.h"
#include "Material.h"

#include "River/Error.h"

using namespace River;
using namespace glm;


void Material::setAlbedo(vec3 albedo) {
    if( albedo.r < 0 || albedo.r > 1.0f ) throw InvalidArgumentException("Albedo red value has to be between 0 and 1");
    if( albedo.g < 0 || albedo.g > 1.0f ) throw InvalidArgumentException("Albedo green value has to be between 0 and 1");
    if( albedo.b < 0 || albedo.b > 1.0f ) throw InvalidArgumentException("Albedo blue value has to be between 0 and 1");

    this->albedo = albedo;
}


vec3 Material::getAlbedo() const {
    return albedo;
}


void Material::setMetallic(float metallic) {
    if( metallic < 0 || metallic > 1.0f ) throw InvalidArgumentException("Metallic value has to be between 0 and 1");
    this->metallic = metallic;
}


float Material::getMetallic() const {
    return metallic;
}


void Material::setRoughness(float roughness) {
    if( roughness < 0 || roughness > 1.0f ) throw InvalidArgumentException("Roughness has to be between 0 and 1");
    this->roughness = roughness;
}


float Material::getRoughness() const {
    return roughness;
}

