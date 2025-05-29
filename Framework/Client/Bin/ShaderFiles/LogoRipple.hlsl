// =========================
// Shader_Rect.hlsl (FX Format)
// 글씨 전용 물결 효과 + 반투명 처리
// =========================

texture g_Texture;
float g_Time;
matrix g_WorldMatrix;
matrix g_ViewMatrix;
matrix g_ProjMatrix;

// 샘플러 설정
sampler DefaultSampler = sampler_state
{
    texture = g_Texture;
    minfilter = linear;
    magfilter = linear;
    mipfilter = linear;
    AddressU = clamp;
    AddressV = clamp;
};

// 정점 입력 구조체
struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexUV : TEXCOORD0;
};

// 정점 출력 구조체
struct VS_OUT
{
    float4 vPosition : POSITION;
    float2 vTexUV : TEXCOORD0;
};

// 정점 셰이더
VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;

    float4 pos = float4(In.vPosition, 1.0f);
    pos = mul(pos, g_WorldMatrix);
    pos = mul(pos, g_ViewMatrix);
    pos = mul(pos, g_ProjMatrix);

    Out.vPosition = pos;
    Out.vTexUV = In.vTexUV;

    return Out;
}

// 픽셀 셰이더
float4 PS_MAIN(VS_OUT In) : COLOR0
{
    float2 uv = In.vTexUV;

    // 물결 왜곡: y 기준 sin 파형으로 x 좌표 변형
    uv.x += sin(uv.y * 20 + g_Time * 2.0) * 0.005;

    float4 color = tex2D(DefaultSampler, uv);

    // 알파값을 낮춰 반투명
    color.a *= 0.3f;

    // 알파가 너무 낮으면 버림 (투명 제거)
    if (color.a < 0.1f)
        discard;

    return color;
}

// FX 테크닉
technique DefaultTechnique
{
    pass P0
    {
        AlphaBlendEnable = TRUE;
        SrcBlend = SRCALPHA;
        DestBlend = INVSRCALPHA;

        VertexShader = compile vs_3_0 VS_MAIN();
        PixelShader = compile ps_3_0 PS_MAIN();
    }
}
