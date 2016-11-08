#version 330

uniform vec3 lerpTarget;
uniform float dt;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;

out vec2 texCoord;

void main()
{
	gl_Position = vec4(mix(inPosition, lerpTarget, dt), 1.0);
	texCoord = inCoord;
}