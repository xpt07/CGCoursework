cbuffer staticMeshBuffer
{
    float4x4 W; // World matrix
    float4x4 VP; // View-Projection matrix
};

struct VS_INPUT
{
    float4 Pos : POS;
    float2 TexCoords : TEXCOORD;
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 TexCoords : TEXCOORD;
};

PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output;
    output.Pos = mul(input.Pos, W);
    output.Pos = mul(output.Pos, VP);
    output.TexCoords = input.TexCoords;
    return output;
}
