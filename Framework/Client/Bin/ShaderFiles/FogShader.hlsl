// ========================
// 안개 셰이더 - FX 파일
// ========================

texture gTexture;
float g_Time;
float g_FogHeight = 1.5f; // 안개 기준 높이 (Y축)
float g_FogSharpness = 8.0f; // 값이 클수록 전이 뚜렷함
float g_MaxAlpha = 0.4f; // 안개 최대 알파

matrix gWorldMatrix;
matrix gViewMatrix;
matrix gProjMatrix;

sampler FogSampler = sampler_state
{
    Texture = gTexture;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    AddressU = Wrap;
    AddressV = Wrap;
};

struct VS_IN
{
    float3 vPosition : POSITION;
    float2 vTexUV : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : POSITION;
    float2 vTexUV : TEXCOORD0;
    float vWorldY : TEXCOORD1;
};

VS_OUT VS_MAIN(VS_IN In)
{
    VS_OUT Out;

    float4 worldPos = mul(float4(In.vPosition, 1.0f), gWorldMatrix);
    float4 viewPos = mul(worldPos, gViewMatrix);
    float4 projPos = mul(viewPos, gProjMatrix);

    Out.vPosition = projPos;

    // UV 움직임 (부드럽게 일렁임)
    float wave = sin(g_Time * 0.5f + In.vTexUV.y * 5.0f) * 0.02f;
    Out.vTexUV = frac(In.vTexUV + float2(wave, g_Time * 0.01f));

    Out.vWorldY = worldPos.y;
    return Out;
}

float4 PS_MAIN(VS_OUT In) : COLOR
{
   // 텍스처 샘플
    float4 fogTex = tex2D(FogSampler, In.vTexUV);

    // 안개 색 고정
    float3 fogColor = float3(0.85f, 0.85f, 0.85f);

    // 안개 강도 (Y 기준 부드러운 전이)
    float fogDensity = 1.0 - smoothstep(g_FogHeight - 1.0f, g_FogHeight + 1.0f, In.vWorldY);

    // 시각 확인용: 강도 값을 회색 밝기로 쓰기
    return float4(fogDensity.xxx, 0.3f);

    float finalAlpha = fogDensity * g_MaxAlpha;

    return float4(fogColor, finalAlpha);
}

technique DefaultTechnique
{
    pass P0
    {
        AlphaBlendEnable = TRUE;
        SrcBlend = SRCALPHA;
        DestBlend = INVSRCALPHA;

        ZEnable = TRUE;
        ZWriteEnable = FALSE;

        VertexShader = compile vs_3_0 VS_MAIN();
        PixelShader = compile ps_3_0 PS_MAIN();
    }
}
