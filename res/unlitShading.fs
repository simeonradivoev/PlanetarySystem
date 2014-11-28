varying vec4		position0;
varying vec3		normal0;
varying vec2		texCoord0;
varying mat4		TBN;

uniform sampler2D	tDiffuse;

void main( void )
{
	gl_FragData[0]		= vec4(texture2D(tDiffuse,texCoord0).rgb, 1);
	gl_FragData[1]		=  vec4(position0.xyz,1);
	gl_FragData[2]		= vec4(normal0.xyz,1);
	gl_FragData[3]		= vec4(texCoord0.xy,0.0,1);
}
