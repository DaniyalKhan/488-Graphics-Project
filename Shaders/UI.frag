#version 400

in vec2 TexCoords;
out vec4 color;

uniform sampler2D texture1;

void main() {
    color = vec4(texture(texture1, TexCoords));
//    color = vec4(1,1,1,1);
}