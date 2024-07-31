#shader vertex
#version 330 core

layout(location = 0) in vec3 position;
layout(location = 1) in vec2 texCoord;
layout(location = 2) in vec3 normal;

out vec2 v_TexCoord;
out vec3 v_SurfaceNormal;
out vec3 v_ToLightVector;
out vec3 v_ToCameraVector;

layout(std140) uniform ub_CameraMatrices
{
    mat4 u_ProjectionMatrix;
    mat4 u_ViewMatrix;
    mat4 u_UIProjectionMatrix;
};

uniform mat4 u_TransformationMatrix;
layout(std140) uniform ub_LightData
{
    vec3 u_LightPosition;
    vec3 u_LightColor;
};

void main()
{
    vec4 worldPosition = u_TransformationMatrix * vec4(position, 1.0);
    gl_Position = u_ProjectionMatrix * u_ViewMatrix * worldPosition;
    v_TexCoord = texCoord;
    v_SurfaceNormal = (u_TransformationMatrix * vec4(normal, 0.0)).xyz;
    v_ToLightVector = u_LightPosition - worldPosition.xyz;
    v_ToCameraVector = (inverse(u_ViewMatrix) * vec4(0.0, 0.0, 0.0, 1.0)).xyz - worldPosition.xyz;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;
in vec3 v_SurfaceNormal;
in vec3 v_ToLightVector;
in vec3 v_ToCameraVector;

layout(std140) uniform ub_LightData
{
    vec3 u_LightPosition;
    vec3 u_LightColor;
};

uniform vec3 u_Color;
uniform float u_ShineDamper;
uniform float u_Reflectivity;
uniform float u_AmbientLightLevel;

void main()
{
    vec3 unitNormal = normalize(v_SurfaceNormal);
    vec3 unitToLightVector = normalize(v_ToLightVector);

    float nDot1 = dot(unitNormal, unitToLightVector);
    float brightness = max(nDot1, u_AmbientLightLevel);
    vec3 diffuse = brightness * u_LightColor;

    vec3 unitToCameraVector = normalize(v_ToCameraVector);
    vec3 lightDirection = -unitToLightVector;
    vec3 reflectedLightDirection = reflect(lightDirection, unitNormal);
    float specularFactor = dot(reflectedLightDirection, unitToCameraVector);
    specularFactor = max(specularFactor, 0.0);
    float dampedFactor = pow(specularFactor, u_ShineDamper);
    vec3 finalSpecular = dampedFactor * u_Reflectivity * u_LightColor;

    color = vec4(diffuse, 1.0) * vec4(u_Color, 1.0) + vec4(finalSpecular, 1.0);
}