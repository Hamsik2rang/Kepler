// Test Vertex Shader

struct VS_INPUT
{
    float3 pos : POSITION0;
    float4 color : COLOR0;
    float2 uv : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 ndcPos : POSITION1;
    float2 uv : TEXCOORD0;
    float4 color : COLOR0;
};


VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.pos = float4(input.pos, 1.0f);
    
    output.color = input.color;
    output.ndcPos = input.pos.xy;
    output.uv = input.uv;
    
    return output;
}