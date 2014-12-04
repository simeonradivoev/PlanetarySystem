#version 420
#include "PBL_fragment.sl"

void main( void )
{
	DiffuseOut		=  texture2D(tDiffuse,texCoord0) * color;
	WorldPosOut		=  vec4(position0,emmision);
	NormalOut			= 	vec4(normal0,1.0);
}
