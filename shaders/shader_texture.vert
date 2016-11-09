#version 330 core

//input vertex and uv data
layout(location = 0) in vec3 vertexPos;
layout(location = 1) in vec2 VertexUV;

//output data. will interpolate for each fragment
out vec2 UV;

//Values that stay consistent for the whole mesh
uniform mat4 MVP

void main()
{
//output position of the vertex
gl_position = MVP * vec4(vertexPos, 1);

//UV of the vertex
UV = vertexUV
}