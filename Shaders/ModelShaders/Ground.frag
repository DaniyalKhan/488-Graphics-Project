#version 400

out vec4 color;
uniform sampler2D texture_diffuse1;
uniform sampler2D texture_diffuse2;
in vec2 TexCoords;
in vec3 p;

void main() {
    vec4 c2 = texture(texture_diffuse1, TexCoords);
    vec4 c1 = texture(texture_diffuse2, TexCoords);
    float alpha = p.y/15;
    color = mix(c1, c2, 1 - alpha);
//    color = vec4(1,1,1,1) * alpha + vec4(0,0,0,1) * (1 - alpha);
}