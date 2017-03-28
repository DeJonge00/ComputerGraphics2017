#version 330 core

uniform sampler2D diffuse;
uniform sampler2D normals;
uniform sampler2D depth;

in vec2 uv;

layout (location = 0) out vec4 fColor;

void main()
{
    fColor = vec4(texture2D(diffuse,uv).rgb, 1);
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
