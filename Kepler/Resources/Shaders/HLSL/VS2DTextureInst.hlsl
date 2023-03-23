
#define MAX_INSTANCE_COUNT 1000

cbuffer ChangedEveryFrame : register(b1)
{
    row_major matrix g_Worlds[MAX_INSTANCE_COUNT];
    matrix g_View;
    matrix g_Projection;
    matrix g_ViewProjection;
}

struct VS_INPUT
{
    float3  Pos         : POSITION0;
    float2  UV          : TEXCOORD0;
    uint    InstID      : SV_InstanceID;
    
    float4 InstColor    : INST_COLOR0;
    bool InstUVFlipX    : INST_BOOL0;
    bool InstUVFlipY    : INST_BOOL1;
};

struct VS_OUTPUT
{
    float4 Pos      : SV_Position;
    float4 Color    : COLOR0;
    float2 UV       : TEXCOORD0;
};

VS_OUTPUT main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.Pos = mul(float4(input.Pos, 1.0f), g_Worlds[input.InstID]);
    output.Pos = mul(output.Pos, g_ViewProjection);
    
    output.Color = input.InstColor;
    if (input.InstUVFlipX)
    {
        input.UV.x *= -1.0f;
    }
    if (input.InstUVFlipY)
    {
        input.UV.y *= -1.0f;
    }
    output.UV = input.UV;
    
	return output;
}