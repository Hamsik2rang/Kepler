// FlatColorVertexShader
// It perform Basic vertex transformation and deliver pos and color to pixel shader

matrix g_World;
matrix g_View;
matrix g_Projection;

struct VS_INPUT
{
    float4 Pos          : POSITION0;
    float4 Color        : COLOR0;
};

struct VS_OUTPUT
{
    float4 Pos          : SV_POSITION;
    float4 Color        : COLOR0;
};

float4 main(VS_INPUT input) : SV_POSITION
{
    VS_OUTPUT output = (VS_OUTPUT)0;
    output.Pos = mul(input.Pos, g_World);
    output.Pos = mul(output.Pos, g_View);
    output.Pos = mul(output.Pos, g_Projection);
    
    output.Color = input.Color;
    
    return output;
}