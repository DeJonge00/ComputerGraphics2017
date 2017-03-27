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
    /*vec3 texColor = texture2D(sampler, vertexTexCoords).xyz;
      if (lightingOn == 1.0) {
        vec3 N = normalize(vec3(vertNormal.xyz));
        vec3 L = normalize(lightPos - vertCoordinates); //direction of the ray
        vec3 V = normalize(vertCoordinates - eyepos); //direction to the eye
        vec3 R = reflect(L, N); //reflection direction
        vec3 ambient = texColor * phongComponents[0];
        vec3 diffuse = max(0, dot(N, L)) * texColor * lightColor * phongComponents[1];
        vec3 specular = pow(max(0, dot(R, V)), phongComponents[3]) * lightColor * phongComponents[2];
        fColor = vec4(ambient + diffuse + specular, 1.0);
    } else {
        fColor = vec4(texColor, 1.0);
    }*/
}
