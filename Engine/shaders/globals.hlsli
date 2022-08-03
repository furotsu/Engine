cbuffer PerFrame : register(b0)
{
	row_major float4x4 g_viewProj;
	float g_screenWidth;
	float g_screenHeight;
	float2 padding0;
};

SamplerState objSamplerState;
