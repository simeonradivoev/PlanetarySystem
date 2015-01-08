#version 330
#include "PBL_fragment.sl"

in vec4 frontColor;
in vec4 backColor;

void main(void)
{
	vec4 color = frontColor + 0.25 * backColor;
	
	DiffuseOut		=  texture2D(tDiffuse,texCoord0) * color;
	WorldPosOut		=  vec4(position0,emmision);
	NormalOut			= 	vec4(normal0,1.0);
}