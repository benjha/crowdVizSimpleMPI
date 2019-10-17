
#version 430 core
#extension GL_EXT_gpu_shader4: enable

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
	vec4 color;
} vOut;

uint wang_hash(uint seed)
{
    seed = (seed ^ 61) ^ (seed >> 16);
    seed *= 9;
    seed = seed ^ (seed >> 4);
    seed *= 0x27d4eb2d;
    seed = seed ^ (seed >> 15);
    return seed;
}

float rnd(vec2 x)
{
    int n = int(x.x * 40.0 + x.y * 6400.0);
    n = (n << 13) ^ n;
    return 1.0 - float( (n * (n * n * 15731 + 789221) + 1376312589) & 0x7fffffff) / 1073741824.0;
}

void calcCoeff (inout vec3 a, vec2 n)
{
	a[0] = rnd(n);
	a[1] = rnd(vec2(rnd(n), rnd(n)));

	a[0] = abs(a[0]);
	a[1] = abs(a[1]);

	if ( (a[0] + a[1]) >= 1.0)
	{
		a[0] = 1.0 - a[0];
		a[1] = 1.0 - a[1];
	}
	
	a[2] = 1 - a[0] - a[1];
}



void barycentricCoords (in vec4 p1, in vec4 p2, in vec4 p3, out vec4 v[4])
{
	vec3 a = vec3(0.0);
	calcCoeff (a, p1.xy);
	v[0].xyz = a[0] * p1.xyz + a[1] * p2.xyz + a[2] * p3.xyz;

	calcCoeff (a, p2.xy);
	v[1].xyz = a[0] * p1.xyz + a[1] * p2.xyz + a[2] * p3.xyz;

	calcCoeff (a, p3.xy);
	v[2].xyz = a[0] * p1.xyz + a[1] * p2.xyz + a[2] * p3.xyz;

	calcCoeff (a, p1.yx);
	v[3].xyz = a[0] * p1.xyz + a[1] * p2.xyz + a[2] * p3.xyz;


	v[0].w = 1.0;
	v[1].w = 1.0;
	v[2].w = 1.0;
	v[3].w = 1.0;
}

void main ()
{
	int		i			= 0;
	float	frontFace	= 0.0;
	float	point_size	= 0.0;
	vec3	normalAVG	= vec3 ( 0.0 );
	vec4	vertex[4];
	mat4	MV			= Transform.view * Transform.model;
	mat4	MVP			= Transform.projection * MV;

	point_size			= 1.f / -( MV * vec4(gl_in[0].gl_Position.xyz, 1.0)).z;	
	normalAVG			= (vIn[0].normal + vIn[1].normal + vIn[2].normal) / 3.0;
	vOut.vertex			= MV * gl_in[0].gl_Position;
	vOut.normal			= normalAVG;
	vOut.normalEyeSpace	= vec3( MV * vec4 (normalAVG, 1.0) );
	frontFace			= dot (normalize(vOut.normalEyeSpace), Transform.view[3].xyz); // cam's position
	
	barycentricCoords (gl_in[0].gl_Position, gl_in[1].gl_Position, gl_in[2].gl_Position, vertex);

	if ( frontFace >= 0.0 )
	{

		// Vertex 1
		gl_Position		= Transform.projection * vOut.vertex;
		gl_PointSize	= point_size;
		vOut.color		= vec4 (1,1, 1.0, 1.0);
		//gl_PointSize	= vOut.area;
		EmitVertex ();
		
		//for (int i = 0; i < 3; i++ )
		{
			
			gl_Position		= MVP * vertex[0];
			gl_PointSize	= point_size;
			vOut.color		= vec4 (1, 0, 0.0, 1.0);
			EmitVertex ();

			gl_Position		= MVP * vertex[1];
			gl_PointSize	= point_size;
			vOut.color		= vec4 (0,1, 0.0, 1.0);
			EmitVertex ();

			gl_Position		= MVP * vertex[2];
			gl_PointSize	= point_size;
			vOut.color		= vec4 (0,0, 1.0, 1.0);
			EmitVertex ();

		}
/*
		// Vertex 3
		gl_Position		= Transform.projection * Transform.view * Transform.model * vertex[1];
		gl_PointSize	= 1.5f / -( MV * vec4(gl_in[2].gl_Position.xyz, 1.0)).z ;
		//gl_PointSize	= vOut.area;
		EmitVertex ();
		
		// Vertex 3
		gl_Position		= Transform.projection * Transform.view * Transform.model * vertex[2];
		gl_PointSize	= 1.5f / -( MV * vec4(gl_in[2].gl_Position.xyz, 1.0)).z ;
		//gl_PointSize	= vOut.area;
		EmitVertex ();
*/
		EndPrimitive ();
	}
}