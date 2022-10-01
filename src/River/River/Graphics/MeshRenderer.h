#pragma once

#include "River/Graphics/Mesh/ModelInstance.h"
#include "River/Graphics/Transform3D.h"
#include "River/Graphics/Camera/Camera.h"
#include "River/Graphics/Shader/ShaderProgram.h"


namespace River {

    
    class MeshRenderer {
    public:

        MeshRenderer(Camera* camera);

        void renderModelInstance(const Transform3D* transform, const ModelInstance* modelInstance);

    private:

        Camera* camera;

		ShaderProgram shaderProgram;

    };

}