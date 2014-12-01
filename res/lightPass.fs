#version 330 

struct BaseLight
{
    vec3 Color;
    float AmbientIntensity;
    float DiffuseIntensity;
};

struct Attenuation
{
    float Constant;
    float Linear;
    float Exp;
};

struct PointLight
{
    BaseLight Base;
    vec3 Position;
    Attenuation Atten;
};

uniform sampler2D gColorMap;
uniform sampler2D gPositionMap;
uniform sampler2D gNormalMap;

uniform vec2 gScreenSize;
uniform PointLight gPointLight;

layout (location = 0) out vec4 LightOut; 

uniform mat4 viewProjectionMatrix;
uniform mat4 modelMatrix;
uniform vec3 gEyeWorldPos;

vec4 CalcLightInternal(BaseLight Light,vec3 LightDirection, vec3 WorldPos,vec3 Normal)
{
    vec4 AmbientColor = vec4(Light.Color, 1.0) * Light.AmbientIntensity;
    float DiffuseFactor = dot(Normal, -LightDirection);

    vec4 DiffuseColor  = vec4(0, 0, 0, 0);
    vec4 SpecularColor = vec4(0, 0, 0, 0);

    if (DiffuseFactor > 0.0) {
        DiffuseColor = vec4(Light.Color, 1.0) * Light.DiffuseIntensity * DiffuseFactor;

        vec3 VertexToEye = normalize(gEyeWorldPos - WorldPos);
        vec3 LightReflect = normalize(reflect(LightDirection, Normal));
        float SpecularFactor = dot(VertexToEye, LightReflect);
    }

    return (AmbientColor + DiffuseColor);
}

vec4 CalcPointLight(vec3 WorldPos, vec3 Normal)
{
    vec3 LightDirection = WorldPos - gPointLight.Position;
    float Distance = length(LightDirection);
    LightDirection = normalize(LightDirection);

    vec4 Color = CalcLightInternal(gPointLight.Base, LightDirection, WorldPos, Normal);

    //float Attenuation =  (gPointLight.Atten.Constant + (gPointLight.Atten.Linear * Distance) + ());
	float Attenuation = 1.0 / (1.0 + gPointLight.Atten.Exp * pow(Distance,2));
	
    return Color * Attenuation;
}

vec2 CalcTexCoord()
{
    return gl_FragCoord.xy / gScreenSize;
}

void main()
{
	vec2 TexCoord = CalcTexCoord();
	vec3 WorldPos = texture(gPositionMap, TexCoord).xyz;
	vec3 Normal = texture(gNormalMap, TexCoord).xyz;
    vec4 Color = texture(gColorMap, TexCoord);
	
	LightOut = CalcPointLight(WorldPos,Normal);
	//LightOut = vec4(WorldPos,1);
}