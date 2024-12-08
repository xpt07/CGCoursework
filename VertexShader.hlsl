cbuffer staticMeshBuffer
{
    float4x4 W; // World matrix
    float4x4 VP; // View-Projection matrix
};

struct VS_INPUT
{
    float4 Pos : POS; // Vertex position
    float3 Normal : NORMAL; // Vertex normal
    float3 Tangent : TANGENT; // Vertex tangent
    float2 TexCoords : TEXCOORD; // Texture coordinates
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION; // Transformed position
    float3 Normal : NORMAL; // Transformed normal
    float3 Tangent : TANGENT;
    float2 TexCoords : TEXCOORD; // Texture coordinates
};

PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output;
    output.Pos = mul(input.Pos, W);
    output.Pos = mul(output.Pos, VP);
    output.Normal = mul(input.Normal, (float3x3) W);
    output.Normal = normalize(output.Normal);
    output.Tangent = mul(input.Tangent, (float3x3) W);
    output.TexCoords = input.TexCoords;
    return output;

}

