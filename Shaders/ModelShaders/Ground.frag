#version 400

out vec4 color;
uniform sampler2D texture1;
uniform sampler2D texture2;
in vec2 TexCoords;
in vec3 p;

void main() {
    vec4 c2 = texture(texture1, TexCoords);
    vec4 c1 = texture(texture2, TexCoords);
    float alpha = p.y/15;
    color = mix(c1, c2, 1 - alpha);
}