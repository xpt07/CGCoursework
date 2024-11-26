cbuffer staticMeshBuffer
{
    float4x4 W;
    float4x4 VP;
};

struct VS_INPUT
{
	float4 Pos : POS;
	float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float2 TexCoords : TEXCOORDS;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float3 Normal : NORMAL;
    float3 Tangent : TANGENT;
    float2 TexCoords : TEXCOORDS;
};

PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output;
    output.Pos = mul(input.Pos, W);
    output.Pos = mul(output.Pos, VP);
    output.Normal = mul(input.Normal, (float3x3) W);
    output.Tangent = mul(input.Tangent, (float3x3) W);
    output.TexCoords = input.TexCoords;
    return output;

}
