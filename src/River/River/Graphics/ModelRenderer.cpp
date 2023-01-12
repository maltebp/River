#include "River/pch.h"

#include <cassert>

#include "ModelRenderer.h"
    
using namespace std;
using namespace glm;
using namespace River;

static string vertexShaderSource = R"(
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
    vec4 worldPosition = u_ModelMatrix * vec4(a_Position, 1.0);
    o_WorldPosition = worldPosition.xyz;

    o_WorldNormal = normalize((u_ModelNormalMatrix * vec4(a_Normal, 1.0)).xyz);

    gl_Position = u_CameraMatrix * worldPosition;
}
)";


static string fragmentShaderSource = R"(
#version 330 core

#define PI 3.1415926538

// These should match the ones defined in C++
#define MAX_POINT_LIGHTS 16
#define MAX_DIRECTIONAL_LIGHTS 8

struct DirectionalLight {
    vec3 coloredIntensity;
    vec3 direction;
};

uniform DirectionalLight u_DirectionalLights[MAX_DIRECTIONAL_LIGHTS];
uniform int u_NumDirectionalLights;

struct PointLight {
    vec3 coloredIntensity;
    vec3 position;
};

uniform PointLight u_PointLights[MAX_POINT_LIGHTS];
uniform int u_NumPointLights;

uniform float u_Gamma;
uniform float u_Exposure;
uniform bool u_ExposureIsEnabled;
uniform vec3 u_EyePosition;

uniform vec3 u_AmbientColor;

uniform vec3 u_Albedo;
uniform float u_Metallicness;
uniform float u_Roughness;

out vec4 FragColor;

in vec3 o_WorldPosition;
in vec3 o_WorldNormal;


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


vec3 computeDirectionalLightRadiance(
    DirectionalLight light,
    vec3 surfaceNormal,
    vec3 directionToEye,
    vec3 albedo,
    float roughness,
    float metallicness
) {
    vec3 directionToLight = -light.direction;
    vec3 irradiance = light.coloredIntensity * max(dot(surfaceNormal, directionToLight), 0.0);

    vec3 brdf = cookTorranceBrdf(
        surfaceNormal, 
        directionToEye, 
        directionToLight, 
        albedo, 
        roughness,
        metallicness
    );

    vec3 radiance = brdf * irradiance;

    return radiance;
}

vec3 computePointLightRadiance(
    PointLight light,
    vec3 worldPosition,
    vec3 surfaceNormal,
    vec3 directionToEye,
    vec3 albedo,
    float roughness,
    float metallicness
) {
    float distanceToLight = distance(light.position, worldPosition);
    float attenuation = 1.0 / pow(distanceToLight, 2);

    vec3 directionToLight = normalize(light.position - worldPosition);
    float cosTheta = max(dot(surfaceNormal, directionToLight), 0);

    vec3 irradiance = cosTheta * light.coloredIntensity * attenuation; 

    vec3 brdf = cookTorranceBrdf(
        surfaceNormal, 
        directionToEye, 
        directionToLight, 
        albedo, 
        roughness,
        metallicness
    );

    vec3 radiance = brdf * irradiance;

    return radiance;
}

void main() {

    vec3 directionToEye = normalize(u_EyePosition - o_WorldPosition);

    // I believe its incorrect (but common) to have GPU linearly interpolate
    // the normals - it should probably be "slerp'ed" instead.
    vec3 surfaceNormal = normalize(o_WorldNormal);

    vec3 totalReflectedRadiance = vec3(0);

    for( int i = 0; i < u_NumDirectionalLights; i++ ) {
        totalReflectedRadiance += computeDirectionalLightRadiance(
            u_DirectionalLights[i],
            surfaceNormal,
            directionToEye,
            u_Albedo,
            u_Roughness,
            u_Metallicness          
        );
    }

    for( int i = 0; i < u_NumPointLights; i++ ) {
        totalReflectedRadiance += computePointLightRadiance(
            u_PointLights[i],
            o_WorldPosition,
            surfaceNormal,
            directionToEye,
            u_Albedo,
            u_Roughness,
            u_Metallicness          
        );
    }
    
    vec3 ambientRadiance = u_Albedo * u_AmbientColor; 
    totalReflectedRadiance += ambientRadiance;

    // Post-processing

    // Tone Mapping
    if( u_ExposureIsEnabled ) {
        totalReflectedRadiance = vec3(1.0f) - exp(-totalReflectedRadiance * u_Exposure);
    }

    // Gamme correction
    totalReflectedRadiance = pow(totalReflectedRadiance, vec3(1.0 / u_Gamma));

    FragColor = vec4(totalReflectedRadiance, 1.0); 
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


void ModelRenderer::setExposureIsEnabled(bool exposureIsEnabled) {
    this->exposureIsEnabled = exposureIsEnabled;
}


bool ModelRenderer::getExposureIsEnabled() const {
    return exposureIsEnabled;
}


void ModelRenderer::setExposure(float exposure) {
    this->exposure = glm::max(exposure, 0.0f);
}


float ModelRenderer::getExposure() const {
    return exposure;
}


void ModelRenderer::setAmbientLight(vec3 color) {
    ambientLightColor = color;
}


void ModelRenderer::renderModelInstance(
    const Transform3D* transform,
    const ModelInstance* modelInstance,
    const vector<DirectionalLight>* directionalLights,
    const vector<PointLight>* pointLights
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

    shaderProgram.setFloat3("u_AmbientColor", ambientLightColor);

    shaderProgram.setFloat("u_Gamma", gamma);
    shaderProgram.setFloat("u_Exposure", exposure);
    shaderProgram.setBool("u_ExposureIsEnabled", exposureIsEnabled);

    if( directionalLights != nullptr ) {
        shaderProgram.setInt("u_NumDirectionalLights", directionalLights->size());
        assert(directionalLights->size() <= MAX_DIRECTIONAL_LIGHTS);

        int i = 0;
        for( const DirectionalLight& directionalLight : *directionalLights ) {
            const string lightUniformName = "u_DirectionalLights[" + to_string(i) + "]";
            shaderProgram.setFloat3(lightUniformName + ".direction", directionalLight.direction);
            shaderProgram.setFloat3(lightUniformName + ".coloredIntensity", directionalLight.intensity * directionalLight.color);
            i++;
        }   
    } else {
        shaderProgram.setInt("u_NumDirectionalLights", 0);
    }

    if( pointLights != nullptr ) {
        shaderProgram.setInt("u_NumPointLights", pointLights->size());
        assert(pointLights->size() <= MAX_POINT_LIGHTS);

        int i = 0;
        for( const PointLight& pointLight : *pointLights ) {
            const string lightUniformName = "u_PointLights[" + to_string(i) + "]";
            shaderProgram.setFloat3(lightUniformName + ".position", pointLight.position);
            shaderProgram.setFloat3(lightUniformName + ".coloredIntensity", pointLight.intensity * pointLight.color);
            i++;
        }   
    } else {
        shaderProgram.setInt("u_PointLights", 0);
    }

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
