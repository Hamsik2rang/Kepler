#include "Global/VSGlobal.hlsli"
// Vertex Solid Shader

struct VS_INPUT
{
    float3 Pos      : POSITION0;
    float4 Color    : COLOR0;
};

struct VS_OUTPUT
{
    float4 Pos      : SV_POSITION;
    float4 Color    : COLOR0;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    
    output.Pos = mul(mul(float4(input.Pos, 1.0f), g_World), g_ViewProjection);
    output.Color = input.Color;
    
    return output;
}