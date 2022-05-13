#include "Global/VSGlobal.hlsli"
// Vertex Solid Shader

struct VS_INPUT
{
    float3 Pos  : POSITION0;
    float2 UV   : TEXCOORD0;
};

struct VS_OUTPUT
{
    float4 Pos  : POSITION0;
    float2 UV   : TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    
    output.Pos = mul(mul(float4(input.Pos, 1.0), g_World), g_ViewProjection);
    output.UV = input.UV;
    
	return output;
}