#include "globals.hlsli"

cbuffer cbPerObject : register(b1)
{
	 float4x4 meshMat;
};

struct VSOut
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

// Vertex shader
VSOut VSMain(float3 position : POSITION, float2 texCoord : TEXCOORD, float3 instancePosition : INSTANCEPOS)
{
	VSOut output;

	position.x += instancePosition.x;
	position.y += instancePosition.y;
	position.z += instancePosition.z;

	output.position = mul(mul(float4(position, 1.0f), meshMat), g_viewProj);
	output.texCoord = texCoord;
	return output;
}

// Pixel shader
Texture2D objTexture;

float4 PSMain(float4 position : SV_POSITION, float2 texCoord : TEXCOORD) : SV_TARGET
{
	return objTexture.Sample(objSamplerState, texCoord);
}