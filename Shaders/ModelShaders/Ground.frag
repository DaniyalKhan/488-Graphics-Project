#version 400

out vec4 color;
uniform sampler2D grass;
uniform sampler2D rock;
in vec2 TexCoords;
in vec3 p;
void main() {
    vec4 c2 = texture(rock, TexCoords);
    vec4 c1 = texture(grass, TexCoords);
    float alpha = p.y;
    color = mix(c1, c2, 1 - alpha * alpha * alpha * alpha);
}