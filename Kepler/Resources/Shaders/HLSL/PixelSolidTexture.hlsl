#include "./Global/PSGlobal.hlsli"
// Pixel Solid Texture Shader

Texture2D g_texture : register(t0);

struct PS_INPUT
{
    float4 Pos : SV_POSITION;
    float2 UV : TEXCOORD0;
    float4 Color : COLOR0;
};


float4 main(PS_INPUT input) : SV_Target
{
    float4 mix = g_texture.Sample(g_LinearWrapState, input.UV);
    
    return mix;
}