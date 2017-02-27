#version 330 core

// Specify the input locations of attributes
in vec3 vertColor;
in vec3 vertNormal;
in vec3 vertCoordinates;

// Specify the Uniforms of the vertex shaders
uniform vec3 materialColor;
uniform vec4 phongComponents; //ambient, diffuse, specular, shininess
uniform vec3 lightPosition;
uniform vec3 lightColor;

// Specify the output of the fragment shader
// Usually a vec4 describing a color (Red, Green, Blue, Alpha/Transparency)
out vec4 fColor;

void main()
{
    vec3 rayDirection = normalize(vertCoordinates - lightPosition);
    vec3 R = reflect(rayDirection, vertNormal);
    vec3 ambient = materialColor * phongComponents[0];
    vec3 diffuse = max(0, dot(vertNormal, rayDirection)) * materialColor;// * lightColor * phongComponents[1];
    vec3 specular;// = pow() * lightColor * phongComponents[2];
    fColor = vec4(ambient + diffuse + specular, 1.0);
}

//fColor = vec4(vertColor, 1.0);
