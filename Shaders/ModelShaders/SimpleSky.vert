#version 400

layout (location = 0) in vec3 position;
out vec3 TexCoords;
uniform mat4 mvpMatrix;

void main()
{
    vec4 pos = mvpMatrix * vec4(position, 0.0);
    gl_Position = pos.xyww;
    TexCoords = position;
}