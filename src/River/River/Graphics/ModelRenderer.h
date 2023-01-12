#pragma once

#include <vector>

#include "River/Graphics/Mesh/ModelInstance.h"
#include "River/Graphics/Transform3D.h"
#include "River/Graphics/Camera/Camera.h"
#include "River/Graphics/Shader/ShaderProgram.h"


namespace River {

    const size_t MAX_DIRECTIONAL_LIGHTS = 8;

    // TODO: For developing PBR
    struct DirectionalLight {
        glm::vec3 direction{0};
        glm::vec3 color{0};
        float intensity = 0;
    };

    class ModelRenderer {
    public:

        ModelRenderer(Camera* camera);

        void renderModelInstance(
            const Transform3D* transform,
            const ModelInstance* modelInstance,
            const std::vector<DirectionalLight>* directionalLights
        );

        void setGamma(float gamma);

        void setExposureIsEnabled(bool exposureIsEnabled);

        // TODO: Not a fan of the name of this method
        [[nodiscard]] bool getExposureIsEnabled() const;

        void setExposure(float exposure);

        [[nodiscard]] float getExposure() const;
        
        // TODO: Just for developing PBR
        void setPointLight(glm::vec3 position, glm::vec3 color, float intensity);
        void setAmbientLight(glm::vec3 color);

    private:

        Camera* camera;

		ShaderProgram shaderProgram;

        float gamma = 2.2f;

        float exposure = 1.0f;

        bool exposureIsEnabled = true;

        glm::vec3 pointLightPosition = glm::vec3(0);

        glm::vec3 pointLightColor = glm::vec3(1.0);

        float pointLightIntensity = 0.0f;

        glm::vec3 ambientLightColor = glm::vec3(0.0f);

    };

}