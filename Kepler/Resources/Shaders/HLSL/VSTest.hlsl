// Test Vertex Shader

float4 main( float3 pos : POSITION0 ) : SV_POSITION
{
    return float4(pos, 1.0f);
}