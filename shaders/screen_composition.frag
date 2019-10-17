
#version 430 core
 
in	vec2 UV;
out	vec4 color;
out float gl_FragDepth;

layout ( binding = 0 ) uniform sampler2D screentex;
 
void main()
{    

	
    color			= texture ( screentex, UV.st );
	gl_FragDepth	= color.a;
    
}
