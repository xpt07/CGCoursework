struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float2 TexCoords : TEXCOORDS;
};

float4 PS(PS_INPUT input) : SV_Target0
{
    return float4(0.6, 0.8, 1.0, 1.0);
}
