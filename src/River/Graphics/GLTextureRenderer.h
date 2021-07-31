#pragma once

#include "River/Graphics/GL.h"
#include "River/Graphics/SimpleVertexArray.h"
#include "River/Graphics/VertexBuffer.h"
#include "River/Graphics/Shader/ShaderProgram.h"


namespace River {

    /**
     * @brief	Renders a single texture to the full size of the viewport
     */
    class GLTextureRenderer {
    public:

        GLTextureRenderer();

        void render(GLuint textureId);

    private:

        VertexBuffer vertexBuffer;

        SimpleVertexArray vertexArray;

		ShaderProgram shader;
    
    };

}