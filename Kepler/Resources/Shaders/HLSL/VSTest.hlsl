// Test Vertex Shader

struct VS_INPUT
{
    float3 pos : POSITION0;
    float4 color : COLOR0;
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
    float2 uv : POSITION1;
    float4 color : COLOR0;
};


VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.pos = float4(input.pos, 1.0f);
    output.color = input.color;
    output.uv = input.pos.xy;
    
    return output;
}