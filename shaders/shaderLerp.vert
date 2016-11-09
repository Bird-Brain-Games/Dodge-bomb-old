#version 330

uniform float dt;

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec2 inCoord;
layout (location = 3) in vec3 lerpTarget;
// vertex attribute for the lerp target

out vec2 texCoord;

void main()
{
	gl_Position = vec4(mix(inPosition, lerpTarget, dt), 1.0);
	texCoord = inCoord;
}