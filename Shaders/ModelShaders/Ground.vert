#version 400

uniform mat4 mvpMatrix;
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
void main() {
    gl_Position = mvpMatrix * vec4(position, 1.0);
}