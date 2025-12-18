#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;

out vec3 v_normal;
out vec3 v_pos;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

void main() {
    v_normal = mat3(transpose(inverse(u_model))) * a_normal;
    v_pos = vec3(u_model * vec4(a_pos, 1));
    gl_Position = u_projection * u_view * u_model * vec4(a_pos, 1.0);
}