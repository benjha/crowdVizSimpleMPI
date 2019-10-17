
#version 430 core

layout (triangles,		invocations  = 1) in;
layout (points,			max_vertices = 4) out;

layout ( binding = 0  ) uniform tranform
{
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;
} Transform;

in vData{
	vec3 normal;
} vIn[];

out vData   //used for lighting
{
	vec4 vertex; 
	vec3 normal;
	vec3 normalEyeSpace;
} vOut;

float areaTriangle ( in vec3 v0, in vec3 v1 )
{
	vec3 crossP = cross (v0, v1);
	return length (crossP);
} 

void main ()
{
	int		i			= 0;
	float	frontFace	= 0.0;
	float	area		= 0.0;
	vec3	normalAVG	= vec3 ( 0.0 );
	vec4	vertex1		= vec4 ( 0.0 ); 
	vec4	vertex2		= vec4 ( 0.0 );
	vec4	vertex3		= vec4 ( 0.0 );
	mat4	MV			= Transform.view * Transform.model;
		
	normalAVG			= (vIn[0].normal + vIn[1].normal + vIn[2].normal) / 3.0;
	vertex1				= (gl_in[0].gl_Position + gl_in[1].gl_Position)/2.0;
	vertex2				= (gl_in[0].gl_Position + gl_in[2].gl_Position)/2.0;
	vertex3				= (gl_in[1].gl_Position + gl_in[2].gl_Position)/2.0;
	vOut.vertex			= MV * gl_in[0].gl_Position;
	vOut.normal			= normalAVG;
	vOut.normalEyeSpace	= vec3( MV * vec4 (normalAVG, 1.0) );
	frontFace			= dot (normalize(vOut.normalEyeSpace), Transform.view[3].xyz); // cam's position
	/*
	vOut.area			= areaTriangle ((Transform.projection * (gl_in[1].gl_Position - gl_in[0].gl_Position)).xyz, 
										(Transform.projection * (gl_in[2].gl_Position - gl_in[0].gl_Position)).xyz);
	*/
	if ( frontFace >= 0.0 )
	{


		// Vertex 1
		gl_Position		= Transform.projection * vOut.vertex;
		gl_PointSize	= 1.5f / -( MV * vec4(gl_in[0].gl_Position.xyz, 1.0)).z ;
		//gl_PointSize	= vOut.area;
		EmitVertex ();
		
		// Vertex 2
		gl_Position		= Transform.projection * Transform.view * Transform.model * vertex1;
		gl_PointSize	= 1.5f / -( MV * vec4(gl_in[1].gl_Position.xyz, 1.0)).z ;
		//gl_PointSize	= vOut.area;
		EmitVertex ();

		// Vertex 3
		gl_Position		= Transform.projection * Transform.view * Transform.model * vertex2;
		gl_PointSize	= 1.5f / -( MV * vec4(gl_in[2].gl_Position.xyz, 1.0)).z ;
		//gl_PointSize	= vOut.area;
		EmitVertex ();
		
		// Vertex 3
		gl_Position		= Transform.projection * Transform.view * Transform.model * vertex3;
		gl_PointSize	= 1.5f / -( MV * vec4(gl_in[2].gl_Position.xyz, 1.0)).z ;
		//gl_PointSize	= vOut.area;
		EmitVertex ();

		EndPrimitive ();
	}
}