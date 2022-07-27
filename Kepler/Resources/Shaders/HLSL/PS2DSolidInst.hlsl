
struct PS_INPUT
{
    float4 Pos : SV_Position;
    float4 Color : COLOR0;
};

float4 main(PS_INPUT input) : SV_TARGET0
{
	return input.Color;
}