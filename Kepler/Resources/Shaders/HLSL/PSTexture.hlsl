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
    float4 textureColor = g_Texture.Sample(g_LinearWrapState, input.UV);
    //if (textureColor.a < 0.75f) discard;

    if (textureColor.a < 0.75f)
    {
        textureColor.a = 0.0f;
    }
    textureColor *= input.Color;
    
    return textureColor;
}