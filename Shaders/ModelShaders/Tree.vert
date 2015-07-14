#version 400

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texCoords;
layout (location = 3) in vec3 color;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform mat4 translation;

out vec3 fragColor;
out vec3 Normal;
out vec3 FragPos;

void main() {
    gl_Position = projection * view * translation * model * vec4(position, 1.0);
    fragColor = color;
    FragPos = vec3(translation * model * vec4(position, 1.0));
    Normal = mat3(transpose(inverse(translation * model))) * normal;
}