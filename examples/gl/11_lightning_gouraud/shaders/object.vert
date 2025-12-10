#version 330 core

layout (location = 0) in vec3 a_pos;
layout (location = 1) in vec3 a_normal;

out vec3 v_color;

uniform mat4 u_model;
uniform mat4 u_view;
uniform mat4 u_projection;

uniform vec3 u_object_color;
uniform vec3 u_light_color;
uniform vec3 u_light_pos;
uniform vec3 u_view_pos;

void main() {
    vec3 frag_pos = vec3(u_model * vec4(a_pos, 1.0));
    vec3 normal = mat3(transpose(inverse(u_model))) * a_normal;
    normal = normalize(normal);

    // ambient
    float ambient_strength = 0.1;
    vec3 ambient = ambient_strength * u_light_color;

    // diffuse
    vec3 light_dir = normalize(u_light_pos - frag_pos);
    float diff = max(dot(normal, light_dir), 0.0);
    vec3 diffuse = diff * u_light_color;

    // specular
    float spec_strength = 0.5;
    vec3 view_dir = normalize(u_view_pos - frag_pos);
    vec3 reflect_dir = reflect(-light_dir, normal);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 32.0);
    vec3 specular = spec_strength * spec * u_light_color;

    vec3 res = (ambient + diffuse + specular) * u_object_color;
    v_color = res;

    gl_Position = u_projection * u_view * u_model * vec4(a_pos, 1.0);
}
