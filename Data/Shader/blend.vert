#version 430 core

layout (location = 0) in vec3 uv;
layout (location = 1) in vec3 norm;
layout (location = 2) in vec2 textureUv;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 v_uv;
out vec3 v_norm;
out vec2 v_textureUv;

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(uv, 1.0);
    v_uv = uv;
    v_norm = norm;
    v_textureUv = textureUv;
}