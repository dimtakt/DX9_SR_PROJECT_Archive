// ===== 상수 버퍼 =====
float4x4 gWorldMatrix;
float4x4 gViewMatrix;
float4x4 gProjMatrix;

float gTime; // 시간 값 (외부에서 전달)

// ===== 다중 광원 구조 =====
#define MAX_LIGHT_COUNT 8

struct Light
{
    float4 vPosition; // 16바이트
    float4 vDirection; // 16바이트
    float4 vDiffuse; // 16바이트
    float4 vSpecular; // 16바이트
    float4 vAmbient; // 16바이트
    float fSpecPower; // 4바이트
    float fRange; // 4바이트
    int eType; // 4바이트
    int padding;
};
float4 gMaterialSpecular; // 머티리얼 스펙큘러
int gLightCount;
Light gLights[MAX_LIGHT_COUNT];

// ===== 텍스처 샘플러 =====
texture gTexture;
sampler DiffuseSampler = sampler_state
{
    Texture = <gTexture>;
    MinFilter = LINEAR;
    MagFilter = LINEAR;
    MipFilter = LINEAR;
    AddressU = WRAP;
    AddressV = WRAP;
};

// ===== 정점 구조체 =====
struct VS_IN
{
    float3 vPos : POSITION;
    float3 vNormal : NORMAL;
    float2 vTexUV : TEXCOORD0;
};

struct VS_OUT
{
    float4 vPosition : POSITION;
    float3 vNormal : TEXCOORD0;
    float2 vTexUV : TEXCOORD1;
    float3 vWorldPos : TEXCOORD2;
};

// ===== 정점 셰이더 =====
VS_OUT VS_Main(VS_IN input)
{
    VS_OUT output;

    float4 worldPos = mul(float4(input.vPos, 1.0f), gWorldMatrix);
    output.vWorldPos = worldPos.xyz;

    float3 worldNormal = mul(input.vNormal, (float3x3) gWorldMatrix);
    output.vNormal = normalize(worldNormal);

    output.vPosition = mul(worldPos, gViewMatrix);
    output.vPosition = mul(output.vPosition, gProjMatrix);
    output.vTexUV = input.vTexUV;

    return output;
}

// ===== 픽셀 셰이더 =====
float4 PS_Main(VS_OUT input) : COLOR0
{
    float4 texColor = tex2D(DiffuseSampler, input.vTexUV);
    if (texColor.a < 0.1f)
        discard;

    float3 normal = normalize(input.vNormal);

    // 카메라 방향 기준 시선 벡터 (Z축 기준으로 고정하거나 카메라 위치가 필요)
    float3 viewDir = normalize(-input.vWorldPos); // 카메라가 원점에 있다고 가정

    float3 resultColor = float3(0, 0, 0);

    for (int i = 0; i < gLightCount; ++i)
    {
        float3 lightDir;
        float attenuation = 1.0f;

        if (gLights[i].eType == 0) // DIRECTIONAL
        {
            lightDir = normalize(-gLights[i].vDirection.xyz);
            attenuation = 1.0f; // 감쇠 없음
        }
        else if (gLights[i].eType == 1) // POINT
        {
            float3 lightVec = gLights[i].vPosition.xyz - input.vWorldPos;
            float dist = length(lightVec);
            lightDir = normalize(lightVec);

            float t = dist / gLights[i].fRange;
            attenuation = saturate(1.0f / (1.0f + t * t));
        }
        else if (gLights[i].eType == 2) // SPOT (추후 필요시 구현)
        {
            continue;
        }

        // 양면 처리 (뒷면도 조명 받게 함)
        float diff = max(dot(normal, lightDir), 0.0f);
        diff = max(diff, max(dot(-normal, lightDir), 0.0f));

        float3 reflectDir = reflect(-lightDir, normal);
        float spec = pow(max(dot(viewDir, reflectDir), 0.0f), gLights[i].fSpecPower);

        float3 ambient = texColor.rgb * gLights[i].vAmbient.rgb;
        float3 diffuse = texColor.rgb * gLights[i].vDiffuse.rgb * diff * attenuation;
        float3 specular = gMaterialSpecular.rgb * gLights[i].vSpecular.rgb * spec * attenuation;

        resultColor += ambient + diffuse + specular;
    }

    return float4(saturate(resultColor), texColor.a);
}

// ===== 테크닉 정의 =====
technique MultiLightTechnique
{
    pass P0
    {
        Lighting = TRUE;
        SpecularEnable = TRUE;

        VertexShader = compile vs_3_0 VS_Main();
        PixelShader = compile ps_3_0 PS_Main();
    }
}
