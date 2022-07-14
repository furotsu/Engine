float4 main(float4 position : SV_POSITION, float4 color : COLOR) : SV_TARGET
{
    float2 uv = position.xy / float2(400.0f, 200.0f) * 2.0f - 1.0f;
    uv.x *= 2.0f;

    if (length(uv) < 0.3f)
        return float4(0.2f, 0.4f, 0.6f, 0.0f);

    return color;

}