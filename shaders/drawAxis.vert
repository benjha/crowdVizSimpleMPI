

#version 430 core

layout ( location = 0 ) in vec4 position;
layout ( location = 1 ) in vec3 normals;

layout ( binding = 0  ) uniform tranform
{
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;
} Transform;

uniform vec3 color;

out vData
{
	vec3 color;
} vOut;

void main ()
{
	mat4 MVP = Transform.projection * Transform.view * Transform.model;
	vOut.color = color;
	gl_Position = MVP * position;
}