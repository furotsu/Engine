#include "globals.hlsli"

cbuffer cbPerObject : register(b1)
{
	 float4x4 g_meshMat;
};

struct VSIn
{
	float3 position : POSITION;
	float2 texCoord : TEXCOORD;
	float4 rowX : ROWX;
	float4 rowY : ROWY;
	float4 rowZ : ROWZ;
	float4 rowW : ROWW;
};

struct VSOut
{
	float4 position : SV_POSITION;
	float2 texCoord : TEXCOORD;
};

// Vertex shader
VSOut VSMain(VSIn input)
{
	VSOut output;

	float4x4 instanceMat = float4x4(input.rowX, input.rowY, input.rowZ, input.rowW);
	instanceMat = mul(instanceMat, g_meshMat);

	output.position = mul(mul(float4(input.position, 1.0f), instanceMat), g_viewProj);
	output.texCoord = input.texCoord;
	return output;
}

// Pixel shader
Texture2D objTexture;

float4 PSMain(float4 position : SV_POSITION, float2 texCoord : TEXCOORD) : SV_TARGET
{
	return objTexture.Sample(objSamplerState, texCoord);
}