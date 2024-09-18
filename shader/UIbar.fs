#version 330 core
out vec4 FragColor;

in vec3 Color;

uniform float value;

void main()
{
    vec4 texColor = vec4(Color, value);
    FragColor = texColor;
}