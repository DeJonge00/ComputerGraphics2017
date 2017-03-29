#version 330 core

// Specify the input locations of attributes
in vec4 vertNormal;
in vec2 vertexTexCoords;

// Specify the Uniforms of the vertex shaders
uniform sampler2D sampler;

// Specify the output of the fragment shader
layout (location = 0) out vec4 fColor;
layout (location = 1) out vec4 fNormal;

void main()
{
    fColor = texture2D(sampler, vertexTexCoords);
    fNormal = (vertNormal + 1) / 2;
}
