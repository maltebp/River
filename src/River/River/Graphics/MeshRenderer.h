#pragma once

#include "River/Graphics/Mesh/ModelInstance.h"
#include "River/Graphics/Transform3D.h"
#include "River/Graphics/Camera/Camera.h"
#include "River/Graphics/Shader/ShaderProgram.h"


namespace River {

    
    struct Box {
 
        glm::vec3 position { 0, 0, 0 };
 
        glm::vec3 scale = { 1.0, 1.0, 1.0 };
 
        glm::quat rotation = { 1.0f, 0.0f, 0.0f, 0.0f };
 
        glm::vec3 color = { 1.0, 1.0, 1.0 };

        void rotateX(float angle);

        void rotateY(float angle);

        void rotateZ(float angle);

    };
    
    
    class MeshRenderer {
    public:

        MeshRenderer(Camera* camera);

        void drawBox(const Box& box);

        void renderModelInstance(const Transform3D* transform, const ModelInstance* modelInstance);

    private:

        Camera* camera;

		ShaderProgram shaderProgram;

    };

}