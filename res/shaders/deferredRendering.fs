#version 120
uniform sampler2D tDiffuse; 
uniform sampler2D tPosition;
uniform sampler2D tNormals;
uniform  sampler2D tLighting;
uniform float fWidth;
uniform float fHeight;
uniform vec3 ambientLight;
uniform float fHdrExposure;

#define KERNEL_SIZE 9
float step_w = 4.0/fWidth;
float step_h = 4.0/fHeight;

const float kernel[KERNEL_SIZE] =  float[KERNEL_SIZE]( 1.0/16.0, 2.0/16.0, 1.0/16.0,
																										2.0/16.0, 4.0/16.0, 2.0/16.0,
																										1.0/16.0, 2.0/16.0, 1.0/16.0 );

vec2 offset[KERNEL_SIZE] = vec2[KERNEL_SIZE]( vec2(-step_w, -step_h), vec2(0.0, -step_h), vec2(step_w, -step_h), 
																						vec2(-step_w, 0.0), vec2(0.0, 0.0), vec2(step_w, 0.0), 
																						vec2(-step_w, step_h), vec2(0.0, step_h), vec2(step_w, step_h) );

void main( void )
{
	vec4 image = texture2D( tDiffuse, gl_TexCoord[0].xy );
	vec4 position = texture2D( tPosition, gl_TexCoord[0].xy );
	vec4 normal = texture2D( tNormals, gl_TexCoord[0].xy );
	vec4 lighting = texture2D( tLighting, gl_TexCoord[0].xy );
	float Emmision = position.a;
	//vec4 color = image * max(Emmision,(lighting + vec4(ambientLight , 1)));
	
	int i = 0;
   vec4 sum = vec4(0.0);
   
   for( i=0; i<KERNEL_SIZE; i++ )
	   {
			vec4 tmp = texture2D(tLighting, gl_TexCoord[0].xy + offset[i]);
			sum += tmp * kernel[i];
	   }
	   
	vec4 color = image + lighting;
	//color = 1.0 - exp(color * -fHdrExposure);
	
	gl_FragColor =  color;
}


