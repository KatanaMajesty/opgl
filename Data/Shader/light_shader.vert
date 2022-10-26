#version 330 core

layout (location = 0) in vec3 uv;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 textureUv;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

out vec3 v_normal;
out vec3 v_uv;
out vec2 v_textureUv; 

void main()
{
    gl_Position = u_projection * u_view * u_model * vec4(uv, 1.0);   
    /*
    Inversing matrices is a costly operation for shaders, 
    so wherever possible try to avoid doing inverse operations 
    since they have to be done on each vertex of your scene. 
    For learning purposes this is fine, but for an efficient 
    application you'll likely want to calculate the normal matrix on the 
    CPU and send it to the shaders via a uniform before drawing (just like the model matrix). 
    */
    v_normal = mat3(transpose(inverse(u_model))) * normal;
    v_uv = vec3(u_model * vec4(uv, 1.0));
    v_textureUv = textureUv;
}