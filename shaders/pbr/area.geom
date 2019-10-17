
#version 430 core

layout (triangles,		invocations  = 1) in;
layout (triangle_strip,	max_vertices = 4) out;

layout ( binding = 0  ) uniform tranform
{
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;
} Transform;

in vData
{
	vec3 normal;
} vIn[];

out vData
{
	vec4 color;
} vOut;

float areaTriangle (in vec3 v0, in vec3 v1)
{
	vec3 n = cross (v0, v1);
	return length (n)/256.0;
}

void main ()
{
	float area = areaTriangle (gl_in[1].gl_Position.xyz - gl_in[0].gl_Position.xyz, gl_in[2].gl_Position.xyz - gl_in[0].gl_Position.xyz);

	for(int i = 0; i < gl_in.length(); i++)
	{
		 // copy attributes
		gl_Position = gl_in[i].gl_Position;
		//vOut.color = vec4 (area, area, area, 1.0);
		vOut.color= vec4(vIn[i].normal,1.0);
		
		EmitVertex();
	}
	EndPrimitive ();
}