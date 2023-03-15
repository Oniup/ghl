#version 450

layout (location = 0) in vec3 in_vertex_position;
layout (location = 1) in vec3 in_normal_coords;
layout (location = 2) in vec2 in_uv_position;

uniform mat4 u_model_matrix;
uniform mat4 u_projection_matrix;

void main() {
    gl_Position = u_projection_matrix * u_model_matrix * vec4(in_vertex_position.xyz, 1.0);
}