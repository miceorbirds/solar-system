cbuffer cbPerObject
{
    float4x4 WVP;
};

cbuffer colorBuf
{
    float4 face_colors[6];
};

struct VS_OUTPUT
{
    float4 pos : SV_POSITION;
};

float4 VS(float4 pos : POSITION) : SV_Position
{
    return mul(pos, WVP);
}

float4 PS(uint tid : SV_PrimitiveID) : SV_Target
{
    return face_colors[tid / 2];
}