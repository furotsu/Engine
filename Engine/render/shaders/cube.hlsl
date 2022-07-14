#include "globals.hlsli"

cbuffer cbPerObject : register(b1)
{
   row_major float4x4 modelMat;
};

// Vertex shader
struct VSOut
{
    float4 position : SV_POSITION;
    float2 texCoord : TEXCOORD;
};

VSOut VSMain(float4 position : POSITION, float2 texCoord : TEXCOORD)
{
    VSOut output;

    output.position = mul(mul(position, modelMat), g_viewProj);
    output.texCoord = texCoord;

    return output;
}

// Pixel shader
Texture2D objTexture;
SamplerState objSamplerState;

float4 PSMain(float4 position : SV_POSITION, float2 texCoord : TEXCOORD) : SV_TARGET
{
    return objTexture.Sample(objSamplerState, texCoord);
}