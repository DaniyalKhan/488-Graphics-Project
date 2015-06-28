#version 400

layout (location = 0) in vec3 position;
//layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;

out vec2 TexCoords;
uniform mat4 mvpMatrix;
void main()
{
    gl_Position = mvpMatrix * vec4(position, 1.0f);
    TexCoords = texCoords;
}