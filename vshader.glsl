#version 330 core

attribute vec3 position;

uniform mat4 mvp;
uniform float time;

vec3 time_to_param(float t) {
    float normalized_value = (sin(t) + 1.) / 2.;
    return vec3(normalized_value, normalized_value, normalized_value);
}

void main() {
    vec3 morphed_vertex = mix(
        position.xyz,
        normalize(position.xyz),
        time_to_param(time));
    gl_Position = mvp * vec4(morphed_vertex.xyz, 1.);
}
