#version 330 core

// Define constants
#define M_PI 3.141593

// Specify the input locations of attributes
layout (location = 0) in vec3 vertCoordinates_in;
layout (location = 1) in vec3 vertColor_in;
layout (location = 2) in vec4 vertNormal_in;
layout (location = 3) in vec2 textureCoords_in;

// Specify the Uniforms of the vertex shader
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat4 normal;
uniform vec3 position;
uniform vec3 lightPosition;
uniform float size;

// Specify the outputs of the vertex shader
out vec3 vertColor;
out vec4 vertNormal;
out vec3 vertCoordinates;
out vec3 lightPos;
out vec2 vertexTexCoords;

void main()
{
    vec3 sizedcoordinates = size * vertCoordinates_in;
    gl_Position = projection * view * model * (vec4(sizedcoordinates, 1.0) + vec4(position,1.0));
    vec4 p = model * vec4(lightPosition, 1.0);
    lightPos = vec3(p.xyz);
    vertColor = vertColor_in;
    vertNormal = normal * vertNormal_in;
    vertCoordinates = vertCoordinates_in;
    vertexTexCoords = textureCoords_in;
}
