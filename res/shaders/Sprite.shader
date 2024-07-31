#shader vertex
#version 330 core

layout(location = 0) in vec2 position;
layout(location = 1) in vec2 texCoord;

out vec2 v_TexCoord;

layout(std140) uniform ub_CameraMatrices
{
    mat4 u_ProjectionMatrix;
    mat4 u_ViewMatrix;
    mat4 u_UIProjectionMatrix;
};

uniform mat4 u_TransformationMatrix;

void main()
{
    gl_Position = u_UIProjectionMatrix * u_TransformationMatrix * vec4(position.x, -position.y, 0.0, 1.0);
    v_TexCoord = texCoord;
}

#shader fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform sampler2D u_Texture;
uniform vec4 u_Color;


void main()
{
    vec4 texColor = texture(u_Texture, v_TexCoord);
    color = texColor * u_Color;
}