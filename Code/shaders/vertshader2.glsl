#version 330 core

// Specify the input locations of attributes
layout (location = 0) in vec2 vert_coord_in;
layout (location = 1) in vec2 uv_coord_in;

// Specify the outputs of the vertex shader
//out vec2 uv;

void main()
{
    gl_Position = vec4(vert_coord_in,0,1);
    //uv = uv_coord_in;
}
