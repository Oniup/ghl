#version 450

layout (location = 0) in vec3 in_vertex_position;

void main() {
    gl_Position = vec4(in_vertex_position.xyz, 1.0);
}