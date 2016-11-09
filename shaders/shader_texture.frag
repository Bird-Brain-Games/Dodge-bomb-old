#version 330 core

//interpolated values from the vertex shader

//output data
out vec3 color

// values that stay consitant for the whole mesh
uniform sampler2d myTextureSampler

void main()
{
color = texture (myTextureSampler, UV).rgb;
}