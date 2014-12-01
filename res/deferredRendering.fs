uniform sampler2D tDiffuse; 
uniform sampler2D tPosition;
uniform sampler2D tNormals;
uniform sampler2D tLighting;

uniform vec3 ambientLight;

void main( void )
{
	vec4 image = texture2D( tDiffuse, gl_TexCoord[0].xy );
	vec4 position = texture2D( tPosition, gl_TexCoord[0].xy );
	vec4 normal = texture2D( tNormals, gl_TexCoord[0].xy );
	vec4 lighting = texture2D( tLighting, gl_TexCoord[0].xy );
	float Emmision = min(1,position.a);
	
	gl_FragColor = image * max(Emmision,(lighting + vec4(ambientLight , 1)));
}


