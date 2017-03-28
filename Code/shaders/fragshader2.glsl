#version 330 core

uniform sampler2D diffuse;
uniform sampler2D normals;
uniform sampler2D depth;

in vec2 uv;

layout (location = 0) out vec4 fColor;

void main()
{
    fColor = vec4(texture2D(diffuse, uv).rgb, 1);
}
