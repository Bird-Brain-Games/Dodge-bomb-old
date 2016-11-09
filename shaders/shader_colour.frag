#version 330

in vec3 inColour;
out vec4 outputColor;

void main()
{
	outputColor = vec4(inColour, 1.0);
}