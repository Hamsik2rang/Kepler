// Test Vertex Shader

struct VS_INPUT
{
    float3 Pos : POSITION0;
    float4 Color : COLOR0;
};

struct VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float2 NDCPos : POSITION1;
    float4 Color : COLOR0;
};


VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.Pos = float4(input.Pos, 1.0f);
    
    output.Color = input.Color;
    output.NDCPos = input.Pos.xy;
    
    return output;
}