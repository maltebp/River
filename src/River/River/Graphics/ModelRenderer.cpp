#include "River/pch.h"

#include "ModelRenderer.h"
    
using namespace glm;
using namespace River;

static std::string vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 a_Position;
    layout (location = 1) in vec3 a_Normal;

    uniform mat4 u_CameraMatrix;
    uniform mat4 u_ModelMatrix;
    // uniform vec3 u_Albedo;

    uniform vec3 u_DirectionalDirection;
    uniform vec3 u_DirectionalColor;
    uniform vec3 u_PointPosition;
    uniform vec3 u_PointColor;
    uniform vec3 u_AmbientColor;

    out vec3 o_Albedo;
    out vec3 o_WorldPosition;
    out vec3 o_WorldNormal;

    void main()
    {
        vec3 normalColor = vec3(
            0.5 * a_Normal.x + 0.5,
            0.5 * a_Normal.y + 0.5,
            0.5 * a_Normal.z + 0.5
        );

        o_Albedo = normalColor;
        vec4 worldPosition = u_ModelMatrix * vec4(a_Position, 1.0);
        o_WorldPosition = worldPosition.xyz;

        // TODO: THIS IS NOT CORRECT I BELIEVE
        o_WorldNormal = normalize(u_ModelMatrix * vec4(a_Normal, 0.0)).xyz;

        gl_Position = u_CameraMatrix * worldPosition;
    }
)";


static std::string fragmentShaderSource = R"(
    #version 330 core

    uniform float u_Gamma;
    uniform vec3 u_DirectionalDirection;
    uniform vec3 u_AmbientColor;

    out vec4 FragColor;

    in vec3 o_Albedo;
    in vec3 o_WorldPosition;
    in vec3 o_WorldNormal;

    void main() {
        
        vec3 directionalRadiance = vec3(1.0) * max(dot(o_WorldNormal, -u_DirectionalDirection), 0.0);

        vec3 radiance = o_Albedo * (directionalRadiance + u_AmbientColor);

        vec3 gammaCorrectedColor = pow(radiance, 1.0 / vec3(u_Gamma));

        FragColor = vec4(gammaCorrectedColor, 1.0);
    }
)";



ModelRenderer::ModelRenderer(Camera* camera)
    :   camera(camera)
{
    // Shader program
    VertexShader vertexShader(vertexShaderSource);
    FragmentShader fragmentShader(fragmentShaderSource);
    shaderProgram.build(vertexShader, fragmentShader);
}


void ModelRenderer::setGamma(float gamma) {
    this->gamma = gamma;
}


void ModelRenderer::setDirectionalLight(vec3 direction, vec3 color) {
    directionalLightDirection = direction;
    directionalLightColor = color;
}


void ModelRenderer::setPointLight(vec3 position, vec3 color) {
    pointLightPosition = position;
    pointLightColor = color;
}


void ModelRenderer::setAmbientLight(vec3 color) {
    ambientLightColor = color;
}


void ModelRenderer::renderModelInstance(
    const Transform3D* transform,
    const ModelInstance* modelInstance
) {
    GL(glEnable(GL_DEPTH_TEST));
    GL(glDepthMask(GL_TRUE)); // Enable writing to depth buffer 
    GL(glDisable(GL_BLEND));

    mat4 modelMatrix = transform->getMatrix();

    // Left here unused so that I don't forgot that this operation is required
    // when we include normals
    mat4 modelDirectionMatrix = transpose(inverse(modelMatrix)); 
    
    shaderProgram.use();
    shaderProgram.setFloatMatrix("u_ModelMatrix", 4, value_ptr(modelMatrix));
    // shaderProgram.setFloatMatrix("u_ModelDirectionMatrix", 4, value_ptr(modelMatrix));
    shaderProgram.setFloatMatrix("u_CameraMatrix", 4, value_ptr(camera->getMatrix()));

    shaderProgram.setFloat3("u_DirectionalDirection", directionalLightDirection);
    // shaderProgram.setFloat3("u_DirectionalColor", directionalLightColor);
    // shaderProgram.setFloat3("u_PointPosition", pointLightPosition);
    // shaderProgram.setFloat3("u_PointColor", pointLightColor);
    // shaderProgram.setFloat3("u_DirectionalColor", directionalLightColor);
    shaderProgram.setFloat3("u_AmbientColor", ambientLightColor);

    shaderProgram.setFloat("u_Gamma", gamma);

    for( auto [mesh, material] : modelInstance->getModel()->getMeshes() ) {

        const Material* materialOverride = modelInstance->getMaterialOverride(material);
        const Material* actualMaterial = materialOverride != nullptr ? materialOverride : material;

        // shaderProgram.setFloat3("u_Albedo", actualMaterial->getAlbedo());

        const VertexArray& vertexArray = mesh->getVertexArray();
        vertexArray.bind();
        vertexArray.drawTriangles(mesh->getNumTriangles());
    }
}
