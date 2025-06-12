// ==========================
// Shader_FadeInEffect.hlsl
// 페이드인: 검정 → 텍스처로 천천히 밝아지는 효과
// ==========================

texture g_Texture;
float g_Time; // CPU에서 전달 (초 단위)
float g_FadeDuration = 2.0f; // 몇 초 동안 페이드인할지 설정

matrix g_WorldMatrix;
matrix g_ViewMatrix;
matrix g_ProjMatrix;

sampler DefaultSampler = sampler_state
{
    texture = g_Texture;
    minfilter = linear;
    magfilter = linear;
    mipfilter = linear;
    AddressU = clamp;
    AddressV = clamp;
};

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexUV : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : SV_POSITION;
    float2 vTexUV : TEXCOORD0;
};

VS_OUT main(VS_IN In)
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

float4 PS_MAIN(VS_OUT In) : COLOR0
{
    float2 uv = In.vTexUV;
    float4 texColor = tex2D(DefaultSampler, uv);

    // 0.0 ~ 1.0 로 제한된 페이드 비율
    float fade = saturate(g_Time / g_FadeDuration);

    // 페이드 인 완료 후 고정 (fade가 1.0 이상이면 연산 중단)
    if (fade >= 1.0f)
        return texColor;

    // 부드러운 보간 (smoothstep)
    float smoothFade = fade * fade * (3.0f - 2.0f * fade);

    float3 resultColor = lerp(float3(0, 0, 0), texColor.rgb, smoothFade);
    return float4(resultColor, texColor.a);
}

technique FadeInTechnique
{
    pass P0
    {
        AlphaBlendEnable = TRUE;
        SrcBlend = SRCALPHA;
        DestBlend = INVSRCALPHA;

        VertexShader = compile vs_2_0 main();
        PixelShader = compile ps_2_0 PS_MAIN();
    }
}
