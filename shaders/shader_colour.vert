#version 330

uniform mat4 modelViewProjectionMatrix;


layout (location = 0) in vec3 inPosition;
layout (location = 3) in vec3 inColour;

out vec3 colour;

void main()
{
	gl_Position = modelViewProjectionMatrix*vec4(inPosition, 1.0);
	colour = inColour;
}