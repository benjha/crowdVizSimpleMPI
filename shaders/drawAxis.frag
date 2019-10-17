
#version 430 core


smooth out vec4 fColor;



in vData
{
	vec4 color;
} vIn;


void main ()
{
	fColor = vIn.color;
	//fColor = vec4 (normalColor, 1.0);
	//fColor = vec4 (1.0, 1.0, 0.0, 1.0);
}