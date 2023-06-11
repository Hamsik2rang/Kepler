
#define MAX_INSTANCE_COUNT 1000

cbuffer ChangedEveryFrame : register(b0)
{
    row_major matrix g_Worlds[MAX_INSTANCE_COUNT];
    matrix g_ViewProjection;
}

struct VS_INPUT
{
    float3  Pos         : POSITION0;
    uint    InstID      : SV_InstanceID;
    
    float4  InstColor   : INST_COLOR0;
};

struct VS_OUTPUT
{
    float4 Pos : SV_Position;
    float4 Color : COLOR0;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.Pos = mul(float4(input.Pos, 1.0f), g_Worlds[input.InstID]);
    output.Pos = mul(output.Pos, g_ViewProjection);
    output.Color = input.InstColor;

    return output;
}