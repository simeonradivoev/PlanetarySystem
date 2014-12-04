#version 420
#include "basicTransform.sl"
#include "PBL_fragment.sl"

void main( void )
{
	vec4 pos = inverse(ProjectionMatrix * ViewMatrix * ModelMatrix ) * gl_FragCoord ;
	vec3 posn = pos.xyz / pos.w;
	float dis = 1-distance((inverse(ModelMatrix) * vec4(position0,1)).xyz,vec3(0,0,0));
	DiffuseOut		=  vec4(dis,dis,dis,dis);
	WorldPosOut		=  vec4(position0,1.0);
	NormalOut			= 	vec4(normal0,1.0);
}
