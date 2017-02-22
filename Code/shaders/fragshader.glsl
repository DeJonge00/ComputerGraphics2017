#version 330 core

// Specify the input locations of attributes
in vec3 vertColor;

// Specify the Uniforms of the vertex shaders
// uniform vec3 lightPosition; for example

// Specify the output of the fragment shader
// Usually a vec4 describing a color (Red, Green, Blue, Alpha/Transparency)
out vec4 fColor;

void main()
{
    fColor = vec4(vertColor, 1.0);
}
