// Pixel Test Shader

cbuffer cb : register(b0)
{
    float g_Time;
    float3 padding;
}

struct PS_INPUT
{
    float4 pos : SV_Position;
    float2 uv : POSITION1;
    float4 color : COLOR0;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float3 col = 0.5 + 0.5 * cos(g_Time + input.uv.xyx + float3(0.0f, 2.0f, 4.0f));
    
    return float4(col, 1.0f);
}