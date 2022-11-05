#version 430 core

out vec4 resultColor;

const float g_near = 0.1;
const float g_far = 10.0; // 10 for visualization

// result will be in range [near, far];
float linearizeDepth(const float depth, float near, float far)
{
    float ndc = depth * 2.0 - 1.0;
    float linear = (2.0 * near * far) / (far + near - ndc * (far - near));
    return linear;
}

void main()
{
    float depth = linearizeDepth(gl_FragCoord.z, g_near, g_far) / g_far; // divide by g_far to visualize
    resultColor = vec4(vec3(depth), 1.0);    
}