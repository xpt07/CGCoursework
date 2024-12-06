cbuffer LightBuffer
{
    float3 LightDirection; // Direction of the skylight
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
    // Normalize the inputs
    float3 normalizedLightDir = normalize(LightDirection);
    float3 normalizedNormal = normalize(input.Normal);

    // View direction (assume camera is at origin)
    float3 viewDir = normalize(-input.Pos.xyz);

    // Calculate the diffuse light intensity
    float diffuse = max(dot(normalizedLightDir, normalizedNormal), 0.0);

    // Calculate specular light intensity
    float3 halfwayDir = normalize(normalizedLightDir + viewDir);
    float specular = pow(max(dot(normalizedNormal, halfwayDir), 0.0), 32.0); // Shininess factor is 32.0

    // Combine ambient, diffuse, and specular lighting
    float3 lightEffect = AmbientColor + SkylightColor * (diffuse * LightIntensity) + SkylightColor * (specular * LightIntensity * 0.5f);

    // Output final color
    return float4(lightEffect, 1.0);
}
