// Pixel Test Shader


cbuffer cb : register(b0)
{
    float g_Time    : packoffset(c0.w);
    float3 padding  : packoffset(c0);
}

Texture2D g_texture : register(t0);
SamplerState g_samplerState : register(s0);

struct PS_INPUT
{
    float4 pos : SV_Position;
    float2 ndcPos : POSITION1;
    float2 uv : POSITION0;
    float4 color : COLOR0;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    float3 rgb = 0.5 + 0.5 * cos(g_Time + input.ndcPos.xyx + float3(0.0f, 2.0f, 4.0f));
    float4 col = float4(rgb, 1.0f);
    float4 tex = g_texture.Sample(g_samplerState, input.uv);
 
    return tex * col;
}