#version 400

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
out vec2 TexCoords;
out vec3 p;

uniform mat4 projection;
uniform mat4 view;

void main() {
    gl_Position = projection * view * vec4(position, 1.0);
    p = normal;
    TexCoords = texCoords;
}