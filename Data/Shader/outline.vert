#version 430 core

layout (location = 0) in vec3 uv;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureUv;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;
uniform float u_outlineFactor;

vec3 getOutlineFactor() { return normal * u_outlineFactor; }

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(uv + getOutlineFactor(), 1.0);
}