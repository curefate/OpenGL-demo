#version 330 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec3 mColor;

out vec3 Color;

uniform mat4 model;

void main()
{
    gl_Position = model * vec4(aPos.x, aPos.y, 0.0, 1.0); 
    Color = mColor;
}