#version 400

in vec2 TexCoords;
in vec3 FragPos;
in vec3 Normal;

uniform vec3 lightPos;
uniform vec3 viewPos;

out vec4 color;

uniform sampler2D texture1;

void main() {
    // Ambient
    vec3 lightColor = vec3(1, 1, 1);
    float ambientStrength = 0.5f;
    vec3 ambient = vec3(ambientStrength);
    
    // Diffuse
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    vec3 diffuse = vec3(max(dot(norm, lightDir), 0.0));
    
    // Specular
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    
    vec3 halfwayDir = normalize(lightDir + viewDir);
    float spec = pow(max(dot(norm, halfwayDir), 0.0), 32.0);
    
//    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = vec3(specularStrength * spec);
    
    vec3 result = (ambient + diffuse + specular) * lightColor;
    color = vec4(result, 1) * vec4(texture(texture1, TexCoords));
//    color = vec4(texture(texture1, TexCoords));
//    color = vec4(1,1,1,1);
}