
#version 420


out vec4 fColor;

vec3 lightDir = vec3(0.5,1.0, 0.0);

layout (binding = 2 ) uniform sampler2D screentex;


in vData
{
	vec3 N;
	vec4 color;
	vec2 UV;
} vIn;


void main ()
{
	float NdotL = max( dot( vIn.N, lightDir ), 0.0 ) + max( dot( vIn.N, -lightDir ), 0.0 );

	fColor = vIn.color * NdotL;
	//fColor = vIn.color;
	//fColor = vec4(vIn.UV, 0.0, 1.0) * NdotL;
	//fColor = texture (screentex, vIn.UV.st );
	
	
}