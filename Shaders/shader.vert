#version 400

uniform mat4 mvpMatrix;
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
out vec4 vertColor;
void main()
{
    gl_Position = mvpMatrix * vec4(position, 1.0);
    vertColor = vec4(color, 1.0);
}