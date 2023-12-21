#version 460 core

layout(location = 0) in vec3 _coords;
layout(location = 1) in vec3 _normal;
layout(location = 2) in vec2 _texCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 normal;
out vec2 texCoords;
out vec3 objPos;

void main() {
    normal = _normal;
    texCoords = _texCoords;
    objPos = vec3(model * vec4(_coords, 1.0));
    gl_Position = projection * view * model * vec4(_coords, 1.0);
}