Texture2D skyTex : register(t0);
SamplerState samplerLinear : register(s0);

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 TexCoords : TEXCOORD;
};

float4 PS(PS_INPUT input) : SV_Target
{
    return skyTex.Sample(samplerLinear, input.TexCoords);
}
