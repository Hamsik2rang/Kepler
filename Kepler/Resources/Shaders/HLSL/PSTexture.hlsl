#include "Global/PSGlobal.hlsli"
// Pixel Texture Shader

Texture2D g_Texture : register(t0);

struct PS_INPUT
{
    float4 Pos      : SV_POSITION;
    float2 UV       : TEXCOORD0;
    float4 Color    : COLOR0;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    return g_Texture.Sample(g_LinearWrapState, input.UV) * input.Color;
}