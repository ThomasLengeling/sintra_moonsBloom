#version 400

in vec3 LightIntesity;

layout (location = 0) out vec4 FragColor;

void main()
{
    FragColor = vec4(LightIntesity, 1.0);
}