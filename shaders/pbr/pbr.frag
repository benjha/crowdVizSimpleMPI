#version 430 core

layout ( binding = 0  ) uniform tranform
{
	uniform mat4 model;
	uniform mat4 view;
	uniform mat4 projection;
} Transform;

in vData
{
	vec4 vertex; 
	vec3 normal;
	vec3 normalEyeSpace;
	vec4 color;
} vIn;

smooth out vec4 fColor;

void main ()
{
	vec3	eyePos	= Transform.view[3].xyz; // camera position
	vec2	xy		= 2.0 * gl_PointCoord - 1.0; // chaging range [0,0]->[1,1] to [-1,-1]->[1,1]
	float	sigmaZ	= -vIn.normalEyeSpace.x / vIn.normalEyeSpace.z * xy.x - vIn.normalEyeSpace.y / vIn.normalEyeSpace.z * xy.y;
	
	if (length ( vec3(xy, sigmaZ) ) > 1.0 )
		discard;

	vec3	V			= normalize (eyePos - vIn.vertex.xyz );
	float	diffuse		= max (dot (vIn.normal.xyz, V),0.0);
	fColor				= vIn.color * diffuse;
	//fColor				= vec4 (1.0,0.8, 0.75,1.0) * diffuse;
	//fColor			= vec4 (Transform.view[3].xyz, 1.0);
	//fColor			= vec4 (vIn.normal,1.0);

	// depth correction
	vec4 vertex		= Transform.projection * vIn.vertex;
	gl_FragDepth	= ( vertex.z / vertex.w + 1.0) / 2.0;
}