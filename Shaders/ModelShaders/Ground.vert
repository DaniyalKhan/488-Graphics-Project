#version 400

uniform mat4 mvpMatrix;
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
out vec2 TexCoords;
out vec3 p;
void main() {
    gl_Position = mvpMatrix * vec4(position, 1.0);
    p = position;
    TexCoords = texCoords;
}