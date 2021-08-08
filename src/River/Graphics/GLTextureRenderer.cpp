#include "River/pch.h"

#include "GLTextureRenderer.h"


namespace River {


    namespace {

        const std::string vertexShaderSource = R"(
            #version 330 core

            layout (location = 0) in vec2 a_Position;
            layout (location = 1) in vec2 a_TextureCoordinates;

            out vec2 o_TextureCoordinates;
            out vec2 o_Position;

            void main()
            {
                o_TextureCoordinates = a_TextureCoordinates;
                o_Position = a_Position;
                gl_Position = vec4(a_Position.x, a_Position.y, 0.0, 1.0);
            }
        )";

	    const std::string fragmentShaderSource = R"(
            
            #version 330 core

            out vec4 FragColor;

            uniform sampler2D u_Texture;

            in vec2 o_TextureCoordinates;
            in vec2 o_Position;

            void main() {
                FragColor = texture(u_Texture, o_TextureCoordinates);
            }

        )";

    }


    GLTextureRenderer::GLTextureRenderer() {

        shader.build(vertexShaderSource, fragmentShaderSource);
        
        std::vector<float> vertexPositions = {
                -1, -1, 0, 0,
                -1,  1, 0, 1,
                 1, -1, 1, 0,
                -1,  1, 0, 1,
                 1,  1, 1, 1,
                 1, -1, 1, 0
        };

        vertexBuffer.setData(vertexPositions, true);

        vertexArray.setAttributes(
            &vertexBuffer,
            VertexAttributes::FLOAT2, // Positions
            VertexAttributes::FLOAT2  // Texture coordinates
        );      

    }


    void GLTextureRenderer::render(GLuint textureId) {

        GL(glActiveTexture(GL_TEXTURE0));
        GL(glBindTexture(GL_TEXTURE_2D, textureId));

        shader.use();
        shader.setInt("u_Texture", 0);

        GL(glDisable(GL_DEPTH_TEST));
        GL(glDepthMask(GL_FALSE));

        vertexArray.drawTriangles(2);

    }  
    

}