

#version 420
#extension GL_EXT_gpu_shader4: enable

layout ( location = 0 ) in vec4 vertex;
layout ( location = 1 ) in vec3 normals;
layout ( location = 2 ) in vec2 uv;

layout ( binding = 0  ) uniform tranform
{
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;
} Transform;

layout (binding = 1 ) uniform samplerBuffer agentPos;

 vec3 COLOR_MASKS[9];


uniform int color;

out vData
{
	vec3 N;
	vec4 color;
	vec2 UV;
} vOut;

out gl_PerVertex
{
	vec4 gl_Position;
};
//
//=======================================================================================
//
void initColorMask ()
{
  COLOR_MASKS[0] = vec3( 0.0, 0.5, 0.0 );
  COLOR_MASKS[1] = vec3( 0.0, 0.38, 0.27 );
  COLOR_MASKS[2] = vec3( 1.0, 0.5, 0.5 );
  COLOR_MASKS[3] = vec3( 0.5, 1.0, 0.5 );
  COLOR_MASKS[4] = vec3( 0.5, 0.5, 1.0 );
  COLOR_MASKS[5] = vec3( 0.5, 1.0, 1.0 );
  COLOR_MASKS[6] = vec3( 1.0, 0.5, 1.0 );
  COLOR_MASKS[7] = vec3( 1.0, 1.0, 0.5 );
  COLOR_MASKS[8] = vec3( 0.54, 0.49, 0.42 );
}
//
//=======================================================================================
//
void main ()
{
	initColorMask ();
	vec4 pos 	= texelFetchBuffer(agentPos, gl_InstanceID);
	
	pos.w		= 1.0;

	mat4 MVP	= Transform.projection * Transform.view * Transform.model;
	vOut.N 		= normals;
	vOut.UV 	= uv;
	vOut.color 	= vec4 (COLOR_MASKS[color],1.0);
		
	gl_Position = MVP * ( vertex + pos);
}
