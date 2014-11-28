#version 420

in vec3		position0;
in vec3		normal0;
in vec2		texCoord0;

layout (location = 0) out vec4 DiffuseOut; 
layout (location = 1) out vec4 WorldPosOut; 
layout (location = 2) out vec4 NormalOut; 
layout (location = 3) out vec4 TexCoordOut;

uniform sampler2D	tDiffuse;

void main( void )
{
	DiffuseOut		= texture2D(tDiffuse,texCoord0);
	WorldPosOut		=  vec4(position0,0.0);
	NormalOut		= 		vec4(normal0,10.);
	TexCoordOut		=  vec4(texCoord0,0.0,1.0);
}
