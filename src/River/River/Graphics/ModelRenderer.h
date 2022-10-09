#pragma once

#include "River/Graphics/Mesh/ModelInstance.h"
#include "River/Graphics/Transform3D.h"
#include "River/Graphics/Camera/Camera.h"
#include "River/Graphics/Shader/ShaderProgram.h"


namespace River {

    
    class ModelRenderer {
    public:

        ModelRenderer(Camera* camera);

        void renderModelInstance(const Transform3D* transform, const ModelInstance* modelInstance);

        // TODO: Just for developing PBR
        void setDirectionalLight(glm::vec3 direction, glm::vec3 color);
        void setPointLight(glm::vec3 position, glm::vec3 color);
        void setAmbientLight(glm::vec3 color);

    private:

        Camera* camera;

		ShaderProgram shaderProgram;

        glm::vec3 directionalLightDirection = glm::vec3(0);

        glm::vec3 directionalLightColor = glm::vec3(1.0);

        glm::vec3 pointLightPosition = glm::vec3(0);

        glm::vec3 pointLightColor = glm::vec3(1.0);

        glm::vec3 ambientLightColor = glm::vec3(0.0f);

    };

}