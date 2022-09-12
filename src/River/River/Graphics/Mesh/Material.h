#pragma once

#include "River/External/glm/glm.hpp"


namespace River {


    class Material {
    public:
    
        void setAlbedo(glm::vec3 albedo);

        glm::vec3 getAlbedo() const;

        void setMetallic(float metallic);

        float getMetallic() const;

        void setRoughness(float roughness);

        float getRoughness() const;

    private:

        glm::vec3 albedo;

        float roughness;

        float metallic;

    };

}