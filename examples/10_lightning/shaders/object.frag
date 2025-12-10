#version 330 core

out vec4 frag_color;

in vec3 v_normal;
in vec3 v_pos;

uniform vec3 u_object_color;
uniform vec3 u_light_color;

uniform vec3 u_light_pos;
uniform vec3 u_view_pos;

void main() {
    // ambient
    const float ambient_stength = 0.1;
    vec3 ambient = ambient_stength * u_light_color;

    // diffuse
    vec3 norm = normalize(v_normal);
    vec3 light_dir = normalize(u_light_pos - v_pos);
    float diff = max(dot(norm, light_dir), 0.0);
    vec3 diffuse = diff * u_light_color;

    // specular
    const float spec_strength = 0.5;
    vec3 view_dir = normalize(u_view_pos - v_pos);
    vec3 reflect_dir = reflect(-light_dir, norm);
    float spec = pow(max(dot(view_dir, reflect_dir), 0.0), 128);
    vec3 specular = spec_strength * spec * u_light_color;

    // result
    vec3 res = (ambient + diffuse + specular) * u_object_color;

    frag_color = vec4(res, 1.0);
}