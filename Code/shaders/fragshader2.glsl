#version 330 core

uniform sampler2D diffuse;
uniform sampler2D normals;
uniform sampler2D depth;
uniform mat4 inverse;
uniform vec4 phongComponents;
uniform vec3 lightColor;
uniform vec3 eyePosition;
uniform vec3 lightPosition;

in vec2 uv;
in vec2 frag_coords;

layout (location = 0) out vec4 fColor;

void main()
{
    vec3 color = texture2D(diffuse,uv).xyz;
    vec4 vert_coords = vec4(frag_coords, 0, 1) * inverse;
    vert_coords.z = texture2D(depth,uv).x * 2 - 1;

    vec4 vertNormal = texture2D(normals,uv);

    vec3 N = normalize(vertNormal.xyz);
    vec3 L = normalize(lightPosition - vert_coords.xyz); //direction of the ray
    vec3 V = normalize(vert_coords.xyz - eyePosition); //direction to the eye
    vec3 R = reflect(L, N); //reflection direction
    vec3 amb = color * phongComponents[0];
    vec3 diff = max(0, dot(N, L)) * color * lightColor * phongComponents[1];
    vec3 spec = pow(max(0, dot(R, V)), phongComponents[3]) * lightColor * phongComponents[2];
    fColor = vec4(amb + diff + spec, 1.0);
}
