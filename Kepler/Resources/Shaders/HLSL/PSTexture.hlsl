#include "Global/PSGlobal.hlsli"
// Solid Pixel Shader

Texture2D Texture : register(t0);

struct PS_INPUT
{
    float4 Pos  : POSITION0;
    float2 UV   : TEXCOORD0;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    return Texture.Sample(g_LinearWrapState, input.UV) * float4(1.0f, 1.0f, 1.0f, 1.0f);
}