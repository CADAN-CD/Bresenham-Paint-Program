#version 330 core

layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aColor;
layout(location = 2) in float aSize;

out vec3 ourColor;

void main()
{
    gl_Position = vec4(aPos, 0.0, 1.0);

    ourColor = aColor;

    gl_PointSize = aSize;
}