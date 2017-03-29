#version 330 core

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
uniform float size;
uniform vec2 rotation;

// Specify the outputs of the vertex shader
out vec4 vertNormal;
out vec2 vertexTexCoords;

void main()
{
    vec3 vertCoordinates = size * vertCoordinates_in;
    gl_Position = projection * view * model * vec4(vertCoordinates, 1.0);
    vertNormal = normal * vertNormal_in;
    vertexTexCoords = textureCoords_in + rotation;
}
