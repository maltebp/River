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
uniform mat4 u_ModelNormalMatrix;

uniform vec3 u_DirectionalColor;
uniform vec3 u_PointPosition;
uniform vec3 u_PointColor;
uniform vec3 u_PointIntensity;
uniform vec3 u_AmbientColor;

out vec3 o_WorldPosition;
out vec3 o_WorldNormal;

void main()
{
    vec3 normalColor = vec3(
        0.5 * a_Normal.x + 0.5,
        0.5 * a_Normal.y + 0.5,
        0.5 * a_Normal.z + 0.5
    );

    vec4 worldPosition = u_ModelMatrix * vec4(a_Position, 1.0);
    o_WorldPosition = worldPosition.xyz;

    o_WorldNormal = normalize((u_ModelNormalMatrix * vec4(a_Normal, 1.0)).xyz);

    gl_Position = u_CameraMatrix * worldPosition;
}
)";


static std::string fragmentShaderSource = R"(
#version 330 core

uniform float u_Gamma;
uniform vec3 u_EyePosition;

uniform vec3 u_DirectionalDirection;
uniform vec3 u_DirectionalColor;
uniform float u_DirectionalIntensity;

uniform vec3 u_PointPosition;
uniform vec3 u_PointColor;
uniform float u_PointIntensity;

uniform vec3 u_AmbientColor;

uniform vec3 u_Albedo;
uniform float u_Metallicness;
uniform float u_Roughness;

out vec4 FragColor;

in vec3 o_WorldPosition;
in vec3 o_WorldNormal;

#define PI 3.1415926538


// Normal-distribution function
float trowbridgeReitzGGX(vec3 surfaceNormal, vec3 directionToEye, vec3 directionToLight, float roughness) {

    vec3 halfWayVector = normalize(directionToEye + directionToLight);
    float roughnessSquared = roughness * roughness; 
    float denominator = pow(max(dot(surfaceNormal, halfWayVector), 0), 2) * (roughnessSquared - 1) + 1;
    denominator *= denominator;
    denominator *= PI;
    return roughnessSquared / denominator;
}


// Geometry-function
float schlickGGX(vec3 surfaceNormal, vec3 direction, float roughness) {
    // Should be changed if we use Image-Based Lighting (IBL)
    float adjustedRoughness = pow(roughness + 1, 2) / 8.0;  

    float a = max(dot(surfaceNormal, direction), 0.0);
    return a / (a * (1 - adjustedRoughness) + adjustedRoughness);
}


// Geeomtry function that takes into account both geometry obstruction and shadowing
float smith(vec3 surfaceNormal, vec3 directionToEye, vec3 directionToLight, float roughness) {
    
    float geometryObstruction = schlickGGX(surfaceNormal, directionToEye, roughness);

    float geometryShadowing = schlickGGX(surfaceNormal, directionToLight, roughness);

    return geometryObstruction * geometryShadowing;
}


vec3 fresnelSchlick(vec3 surfaceNormal, vec3 directionToEye, vec3 directionToLight, vec3 baseReflectivity) {
    vec3 eyeLightHalfwayVector = normalize(directionToLight + directionToEye);
    float a = max(dot(eyeLightHalfwayVector, directionToEye), 0);
    return baseReflectivity + (1.0 - baseReflectivity) * pow(clamp(1.0 - a, 0.0, 1.0), 5.0);
}


vec3 cookTorranceBrdf(
    vec3 surfaceNormal,
    vec3 directionToEye,
    vec3 directionToLight,
    vec3 albedo,
    float roughness,
    float metallicness
) {
    vec3 DIELECTRIC_BASE_REFLECTIVITY = vec3(0.04);
    vec3 baseReflectivity = mix(DIELECTRIC_BASE_REFLECTIVITY, albedo, metallicness); 
    
    vec3 reflectionFactor = fresnelSchlick(surfaceNormal, directionToEye, directionToLight, baseReflectivity);
    float reflectionProbability = trowbridgeReitzGGX(surfaceNormal, directionToEye, directionToLight, roughness);
    float geometryProbability = smith(surfaceNormal, directionToEye, directionToLight, roughness);
   
    vec3 diffuseFactor = vec3(1.0) - reflectionFactor;
    
    // Metallic surfaces reflects all light
    diffuseFactor *= 1 - metallicness;

    float denominator = 4.0 * max(dot(surfaceNormal, directionToEye), 0.0) * max(dot(surfaceNormal, directionToLight), 0.0) + 0.0001;
    
    float specular = reflectionProbability * geometryProbability / denominator;
    vec3 diffuse = albedo / PI; // Lambertian

    vec3 brdf = diffuseFactor * diffuse + reflectionFactor * specular;
    
    return brdf;
}


vec3 computeDirectionalLightRadiance(vec3 surfaceNormal) {
    vec3 incomingRadiance = u_DirectionalColor * u_DirectionalIntensity;
    incomingRadiance *= max(dot(surfaceNormal, -u_DirectionalDirection), 0.0);

    vec3 directionToEye = normalize(u_EyePosition - o_WorldPosition);

    vec3 brdf = cookTorranceBrdf(
        surfaceNormal, 
        directionToEye, 
        -u_DirectionalDirection, 
        u_Albedo, 
        u_Roughness,
        u_Metallicness
    );

    vec3 reflectedRadiance = brdf * incomingRadiance;

    return reflectedRadiance;
}

vec3 computePointLightRadiance(vec3 surfaceNormal) {
    vec3 pointLightIntensity = u_PointIntensity * u_PointColor;
    
    float distanceToPointLight = distance(u_PointPosition, o_WorldPosition);
    float attenuation = 1.0 / pow(distanceToPointLight, 2);
    vec3 incomingRadiance = pointLightIntensity * attenuation; 

    vec3 directionToPointLight = normalize(u_PointPosition - o_WorldPosition);
    incomingRadiance *= max(dot(surfaceNormal, directionToPointLight), 0);

    vec3 directionToEye = normalize(u_EyePosition - o_WorldPosition);

    vec3 brdf = cookTorranceBrdf(
        surfaceNormal, 
        directionToEye, 
        directionToPointLight, 
        u_Albedo, 
        u_Roughness,
        u_Metallicness
    );

    vec3 reflectedRadiance = brdf * incomingRadiance;

    return reflectedRadiance;
}

void main() {

    // I believe its incorrect (but common) to have GPU linearly interpolate
    // the normals - it should probably be "slerp'ed" instead.
    vec3 surfaceNormal = normalize(o_WorldNormal);

    vec3 directionalLightRadiance = computeDirectionalLightRadiance(
        surfaceNormal
    );
    
    vec3 pointLightRadiance = computePointLightRadiance(
        surfaceNormal
    );
    
    vec3 ambientRadiance = u_Albedo * u_AmbientColor; 

    vec3 totalReflectedRadiance = ambientRadiance + directionalLightRadiance + pointLightRadiance;

    vec3 gammaCorrectedColor = pow(totalReflectedRadiance, vec3(1.0 / u_Gamma));

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


void ModelRenderer::setDirectionalLight(vec3 direction, vec3 color, float intensity) {
    directionalLightDirection = direction;
    directionalLightColor = color;
    directionalLightIntensity = intensity;
}


void ModelRenderer::setPointLight(vec3 position, vec3 color, float intensity) {
    pointLightPosition = position;
    pointLightColor = color;
    pointLightIntensity = intensity;
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
    mat4 modelNormalMatrix = transpose(inverse(modelMatrix)); 
    
    shaderProgram.use();
    shaderProgram.setFloatMatrix("u_ModelMatrix", 4, value_ptr(modelMatrix));
    
    shaderProgram.setFloatMatrix("u_ModelNormalMatrix", 4, value_ptr(modelNormalMatrix));
    
    shaderProgram.setFloatMatrix("u_CameraMatrix", 4, value_ptr(camera->getMatrix()));

    shaderProgram.setFloat3("u_EyePosition", camera->getPosition());

    shaderProgram.setFloat3("u_DirectionalDirection", directionalLightDirection);
    shaderProgram.setFloat3("u_DirectionalColor", directionalLightColor);
    shaderProgram.setFloat("u_DirectionalIntensity", directionalLightIntensity);
    shaderProgram.setFloat3("u_PointPosition", pointLightPosition);
    shaderProgram.setFloat3("u_PointColor", pointLightColor);
    shaderProgram.setFloat("u_PointIntensity", pointLightIntensity);
    shaderProgram.setFloat3("u_AmbientColor", ambientLightColor);

    shaderProgram.setFloat("u_Gamma", gamma);

    for( auto [mesh, material] : modelInstance->getModel()->getMeshes() ) {

        const Material* materialOverride = modelInstance->getMaterialOverride(material);
        const Material* actualMaterial = materialOverride != nullptr ? materialOverride : material;

        shaderProgram.setFloat3("u_Albedo", actualMaterial->getAlbedo());
        shaderProgram.setFloat("u_Roughness", actualMaterial->getRoughness());
        shaderProgram.setFloat("u_Metallicness", actualMaterial->getMetallicness());

        const VertexArray& vertexArray = mesh->getVertexArray();
        vertexArray.bind();
        vertexArray.drawTriangles(mesh->getNumTriangles());
    }
}
