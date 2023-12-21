#version 460 core

in vec3 normal;
in vec2 texCoords;
in vec3 objPos;
out vec4 color;

uniform sampler2D tex0;

uniform float ambient;
uniform vec3 lightPos;
uniform mat4 normalMatrix;

void main() {
    float diffuse = clamp(dot(normalize(lightPos - objPos), mat3(normalMatrix) * normal), 0.0, 1.0);
    float specular = 0.0;
    color = (ambient + diffuse + specular) * texture(tex0, texCoords);
}
