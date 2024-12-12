Texture2D tex : register(t0);
SamplerState samplerLinear : register(s0);

cbuffer LightBuffer
{
    float3 LightDirection; // Direction of the light
    float LightIntensity; // Intensity of the skylight
    float3 SkylightColor; // Color of the skylight
    float3 AmbientColor; // Ambient light color
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float2 TexCoords : TEXCOORD;
};

float4 PS(PS_INPUT input) : SV_Target0
{
    float4 colour = tex.Sample(samplerLinear, input.TexCoords);
    
    if (colour.a < 0.5)
        discard;

    // Normalize directions
    float3 normalizedLightDir = normalize(LightDirection);
    float3 normalizedNormal = normalize(input.Normal);
    float3 viewDir = normalize(-input.Pos.xyz);

    // Diffuse lighting
    float diffuse = max(dot(normalizedLightDir, normalizedNormal), 0.0);

    // Specular lighting
    float3 halfwayDir = normalize(normalizedLightDir + viewDir);
    float specular = pow(max(dot(normalizedNormal, halfwayDir), 0.0), 32.0);

    // Combine ambient, diffuse, and specular lighting
    float3 lightEffect = AmbientColor + SkylightColor * (diffuse * LightIntensity) + SkylightColor * (specular * LightIntensity * 0.5f);

    // Combine light effect with texture
    float3 finalColor = colour.rgb * lightEffect;

    return float4(finalColor, 1.0);
}
