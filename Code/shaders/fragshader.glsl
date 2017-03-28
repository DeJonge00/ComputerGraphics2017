#version 330 core

// Specify the input locations of attributes
in vec3 vertColor;
in vec4 vertNormal;
in vec3 vertCoordinates;
in vec3 lightPos;
in vec2 vertexTexCoords;
in vec3 eyepos;

// Specify the Uniforms of the vertex shaders
uniform vec3 materialColor;
uniform vec4 phongComponents; //ambient, diffuse, specular, shininess
uniform vec3 lightColor;
uniform sampler2D sampler;
uniform float lightingOn;

// Specify the output of the fragment shader
// Usually a vec4 describing a color (Red, Green, Blue, Alpha/Transparency)
layout (location = 0) out vec4 fColor;
layout (location = 1) out vec4 fNormal;

void main()
{
    fColor = texture2D(sampler, vertexTexCoords);
    fNormal = (vertNormal + 1) / 2;
}
