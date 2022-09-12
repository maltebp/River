#include "River/pch.h"

#include "MeshRenderer.h"
    
using namespace glm;
using namespace River;

static std::string vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 a_Pos;

    uniform mat4 u_CameraMatrix;
    uniform mat4 u_ModelMatrix;

    out vec3 o_Color;

    void main()
    {
        o_Color = a_Pos + vec3(0.5, 0.5, 0.5);
        gl_Position = u_CameraMatrix * u_ModelMatrix * vec4(a_Pos, 1.0);
    }
)";


static std::string fragmentShaderSource = R"(
    #version 330 core

    out vec4 FragColor;

    in vec3 o_Color;

    void main() {
        FragColor = vec4(o_Color, 1.0);
    }
)";


Mesh* BOX_MESH = nullptr;


void Box::rotateX(float angle) {
    vec3 xAxis = { 1.0f, 0, 0 };

    rotation = rotation * rotate(quat(1.0f,0,0,0), radians(angle), xAxis);
}


void Box::rotateY(float angle) {
    vec3 yAxis = { 0, 1.0f, 0 };
    rotation = rotation * rotate(quat(1.0f,0,0,0), radians(angle), yAxis);
}


void Box::rotateZ(float angle) {
    vec3 zAxis = { 0, 0, 1.0f };
    rotation = rotation * rotate(quat(1.0f,0,0,0), radians(angle), zAxis);
}


MeshRenderer::MeshRenderer(Camera* camera)
    :   camera(camera)
{

    // Shader program
    VertexShader vertexShader(vertexShaderSource);
    FragmentShader fragmentShader(fragmentShaderSource);
    shaderProgram.build(vertexShader, fragmentShader);

     BOX_MESH = new Mesh(
        {
            { -0.5f, -0.5f, -0.5f },
            { -0.5f,  0.5f, -0.5f },
            {  0.5f,  0.5f, -0.5f },
            {  0.5f, -0.5f, -0.5f },
            { -0.5f, -0.5f,  0.5f },
            { -0.5f,  0.5f,  0.5f },
            {  0.5f,  0.5f,  0.5f },
            {  0.5f, -0.5f,  0.5f }
        },
        {
            // TODO: These are not correct, but they are not used right now
            { -0.5f, -0.5f, -0.5f },
            { -0.5f,  0.5f, -0.5f },
            {  0.5f,  0.5f, -0.5f },
            {  0.5f, -0.5f, -0.5f },
            { -0.5f, -0.5f,  0.5f },
            { -0.5f,  0.5f,  0.5f },
            {  0.5f,  0.5f,  0.5f },
            {  0.5f, -0.5f,  0.5f }
        },
        { 
            0, 1, 4,  // Left 1
            1, 5, 4,  // Left 2
            1, 2, 5,  // Back 1
            2, 6, 5,  // Back 2
            2, 3, 6,  // Right 1
            3, 7, 6,  // Right 2
            3, 0, 7,  // Front 1
            0, 4, 7,  // Front 2
            7, 4, 6,  // Top 1
            4, 5, 6,  // Top 2
            3, 0, 2,  // Bottom 1
            0, 1, 2   // Bottom 2
        }
    );
}


void MeshRenderer::drawBox(const Box& box) {

    mat4 modelMatrix = translate(mat4(1.0), box.position);

    modelMatrix = mat4_cast(box.rotation);

    modelMatrix = scale(modelMatrix, box.scale);

    BOX_MESH->getVertexArray().bind();

    // Enable alpha testing, and discarding any fragment, which has an alpha of 0
    // GL(glEnable(GL_ALPHA_TEST));
    // GL(glAlphaFunc(GL_GREATER, 0));
  
    GL(glEnable(GL_DEPTH_TEST));
    GL(glDepthMask(GL_TRUE)); // Enable writing to depth buffer 
    GL(glDisable(GL_BLEND));
    
    shaderProgram.use();
    shaderProgram.setFloatMatrix("u_CameraMatrix", 4, value_ptr(camera->getMatrix()));
    shaderProgram.setFloatMatrix("u_ModelMatrix", 4, value_ptr(modelMatrix));

    BOX_MESH->getVertexArray().drawTriangles(12);
}


void MeshRenderer::renderModelInstance(
    const Transform3D* transform,
    const ModelInstance* modelInstance
) {
    // TODO: Use transform matrix
    mat4 modelMatrix = transform->getMatrix();
    
    // Enable alpha testing, and discarding any fragment, which has an alpha of 0
    // GL(glEnable(GL_ALPHA_TEST));
    // GL(glAlphaFunc(GL_GREATER, 0));

    GL(glEnable(GL_DEPTH_TEST));
    GL(glDepthMask(GL_TRUE)); // Enable writing to depth buffer 
    GL(glDisable(GL_BLEND));
    
    shaderProgram.use();
    shaderProgram.setFloatMatrix("u_CameraMatrix", 4, value_ptr(camera->getMatrix()));
    shaderProgram.setFloatMatrix("u_ModelMatrix", 4, value_ptr(modelMatrix));

    for( auto [mesh, material] : modelInstance->getModel()->getMeshes() ) {
        // TODO: Use material
        const VertexArray& vertexArray = mesh->getVertexArray();
        vertexArray.bind();
        vertexArray.drawTriangles(mesh->getNumTriangles());
    }

    
}
