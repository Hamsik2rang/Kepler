

struct VSInput
{
    float3 position : POSITION0;
    float3 normal : NORMAL0;
    float2 uv : TEXCOORD0;
};

struct VSOutput
{
    float4 position : SV_Position;
    float2 uv : TEXCOORD0;
};


VSOutput main(VSInput input)
{
    VSOutput output = (VSOutput) 0;
    
    output.position = float4(input.position, 1.0);
    output.uv = input.uv;

    return output;
}