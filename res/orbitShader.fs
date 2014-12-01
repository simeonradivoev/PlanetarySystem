#version 420

in vec3		position0;
in vec3		normal0;
in vec2		texCoord0;

layout (location = 0) out vec4 DiffuseOut; 
layout (location = 1) out vec4 WorldPosOut;
layout (location = 2) out vec4 NormalOut;

uniform sampler2D	tDiffuse;
uniform vec4 color;
uniform vec4 emission;

void main( void )
{
	DiffuseOut		=  color;
	WorldPosOut		=  vec4(position0,emission);
	NormalOut		= 		vec4(0,1.0,0,1.0);
}
