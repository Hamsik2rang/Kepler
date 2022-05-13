#include "Global/PSGlobal.hlsli"


struct PS_INPUT
{
    float4 Pos      : POSITION0;
    float4 Color    : COLOR0;
};

float4 main(PS_INPUT input) : SV_TARGET
{
    return input.Color;
}