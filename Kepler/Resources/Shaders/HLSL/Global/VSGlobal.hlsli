
cbuffer cb : register(b0)
{
    row_major matrix g_World;
    matrix g_View;
    matrix g_Projection;
    matrix g_ViewProjection;
    matrix g_WorldViewProjection;
}