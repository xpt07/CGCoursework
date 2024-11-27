cbuffer staticMeshBuffer
{
    float4x4 W; // World matrix
    float4x4 VP; // View-Projection matrix
};

struct VS_INPUT
{
    float3 Pos : POS; // Vertex position
    float3 Normal : NORMAL; // Vertex normal
    float3 Tangent : TANGENT; // Vertex tangent
    float2 TexCoords : TEXCOORDS; // Texture coordinates
};

struct PS_INPUT
{
    float4 Pos : SV_POSITION; // Transformed position
    float3 Normal : NORMAL; // Transformed normal
    float2 TexCoords : TEXCOORDS; // Texture coordinates
};

PS_INPUT VS(VS_INPUT input)
{
    PS_INPUT output;

    // Correctly construct world position with 4 components for multiplication
    float4 worldPos = mul(float4(input.Pos, 1.0f), W);

    // Transform to clip space
    output.Pos = mul(worldPos, VP);

    // Correctly transform normal (3x3 matrix derived from W for rotation-only transformations)
    output.Normal = normalize(mul((float3x3) W, input.Normal));

    // Pass through texture coordinates
    output.TexCoords = input.TexCoords;

    return output;
}
