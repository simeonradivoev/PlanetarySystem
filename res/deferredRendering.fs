uniform sampler2D tDiffuse; 
uniform sampler2D tPosition;
uniform sampler2D tNormals;
uniform sampler2D tTexCoords;
uniform float UwhitePoint = 1.0;

uniform mat4 worldToLightViewMatrix;
uniform mat4 lightViewToProjectionMatrix;
uniform mat4 worldToCameraViewMatrix;

uniform vec3 cameraPosition;

float linearToSRGB(float linear) {
	if (linear <= 0.0031308) return linear * 12.92;
	else return 1.055 * pow(linear, 1/2.4) - 0.055;
	}

	vec2 CalcTexCoord()
{
    return gl_FragCoord.xy / vec2(1280.0,720.0);
}

void main( void )
{
	vec4 image = texture2D( tDiffuse, gl_TexCoord[0].xy );
	vec4 position = texture2D( tPosition, gl_TexCoord[0].xy );
	vec4 normal = texture2D( tNormals, gl_TexCoord[0].xy );
	vec4 texCoord = texture2D( tTexCoords, gl_TexCoord[0].xy );
	
	mat4 lightViewToWolrdMatrix = inverse(worldToLightViewMatrix);
    vec3 light = lightViewToWolrdMatrix[3].xyz;
	vec3 lightDir = light - position.xyz ;
	
	normal = normalize(normal);
	lightDir = normalize(lightDir);
	
	vec3 eyeDir = cameraPosition-position.xyz;
	vec3 reflectedEyeVector = normalize(reflect(eyeDir, normal));
	
	vec3 ambientLight = 0.05;
	
	float Lwhite2 = UwhitePoint * UwhitePoint;
	float lightIntensity = max(dot(normal,lightDir),0) + pow(max(dot(normal,reflectedEyeVector),0.0), 100) * 1.5;
	vec3 diffuse = image.xyz;
	vec3 rgb = diffuse * (lightIntensity  + ambientLight);
	float L = 0.2126 * rgb.r + 0.7152 * rgb.g + 0.0722 * rgb.b ;
	float Lfact = (1+L/Lwhite2)/(1+L);
	gl_FragColor = vec4(rgb * Lfact,1.0);
	gl_FragColor = normal;
}


