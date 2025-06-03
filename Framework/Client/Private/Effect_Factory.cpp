#include "Effect_Factory.h"
#include "GameInstance.h"
#include "Object_Manager.h"
#include "Effect.h"
#include "PlayerEffect.h"




IMPLEMENT_SINGLETON(CEffect_Factory)

CEffect_Factory::CEffect_Factory()
	: m_pGameInstance(CGameInstance::GetInstance())
{
	//Safe_AddRef(m_pGameInstance);
}

HRESULT CEffect_Factory::Initialize()
{

	return S_OK;
}

// 벡터 위치, 쿼터니언 회전, 벡터 스케일 정보로 생성
void CEffect_Factory::Create_Effect(const _wstring& strEffectTag, _float3 vPos, D3DXQUATERNION qRot, _float3 vScale, _bool isFlippedX)
{
	CEffect::EFFECT_DESC EffectDesc;
	EffectDesc.strEffectTag = strEffectTag;
	EffectDesc.vPos = vPos;
	EffectDesc.qRot = qRot;
	EffectDesc.vScale = vScale;

	m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STATIC), L"Layer_Effect",
		ENUM_CLASS(LEVEL::LEVEL_STATIC), L"Prototype_GameObject_PlayerEffect", &EffectDesc);

	return;
}

// 위치 정보가 담긴 행렬을 넣으면 해당 좌표에 생성
void CEffect_Factory::Create_Effect(const _wstring& strEffectTag, _float4x4 matEffectWorld, _bool isFlippedX)
{
	CEffect::EFFECT_DESC EffectDesc;
	EffectDesc.strEffectTag = strEffectTag;
	EffectDesc.matWorld = matEffectWorld;
	EffectDesc.isMatWorld = true;
	EffectDesc.isFlippedX = isFlippedX;

	m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STATIC), L"Layer_Effect",
		ENUM_CLASS(LEVEL::LEVEL_STATIC), L"Prototype_GameObject_PlayerEffect", &EffectDesc);
	return;
}

// 기준이 될 행렬과 거기서 추가 변화를 줄 행렬을 넣으면 해당 좌표에 생성
void CEffect_Factory::Create_Effect(const _wstring& strEffectTag, _float4x4 matEffectWorld, _float4x4 matOffsetWorld, _bool isFlippedX)
{
	CEffect::EFFECT_DESC EffectDesc;
	EffectDesc.strEffectTag = strEffectTag;
	EffectDesc.matWorld = matEffectWorld * matOffsetWorld;
	EffectDesc.isMatWorld = true;
	EffectDesc.isFlippedX = isFlippedX;

	m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STATIC), L"Layer_Effect",
		ENUM_CLASS(LEVEL::LEVEL_STATIC), L"Prototype_GameObject_PlayerEffect", &EffectDesc);
	return;
}

// + 생성 후 해당 객체의 Position 값만을 따라감
void CEffect_Factory::Create_Effect(const _wstring& strEffectTag, _float4x4 matEffectWorld, _float4x4 matOffsetWorld, CTransform* pFollowTransformCom, _bool isFlippedX)
{
	CEffect::EFFECT_DESC EffectDesc;
	EffectDesc.strEffectTag = strEffectTag;
	EffectDesc.matWorld = matEffectWorld * matOffsetWorld;
	EffectDesc.isMatWorld = true;
	EffectDesc.isFlippedX = isFlippedX;
	EffectDesc.pFollowTransformCom = pFollowTransformCom;

	m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STATIC), L"Layer_Effect",
		ENUM_CLASS(LEVEL::LEVEL_STATIC), L"Prototype_GameObject_PlayerEffect", &EffectDesc);
	return;
}

// + 생성 후 해당 단위벡터의 방향으로 날아감 (투사체)
void CEffect_Factory::Create_Effect(const _wstring& strEffectTag, _float4x4 matEffectWorld, _float4x4 matOffsetWorld, _float3 vThrownDir, _float fThrownPower, _float fLifeTimeSec, _bool isFlippedX)
{
	CEffect::EFFECT_DESC EffectDesc;
	EffectDesc.strEffectTag = strEffectTag;
	EffectDesc.matWorld = matEffectWorld * matOffsetWorld;
	EffectDesc.isMatWorld = true;
	EffectDesc.isFlippedX = isFlippedX;
	D3DXVec3Normalize(&vThrownDir, &vThrownDir);
	EffectDesc.vThrownDir = vThrownDir;
	EffectDesc.fThrownPower = fThrownPower;
	EffectDesc.fLifeTimeSec = fLifeTimeSec;

	m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_STATIC), L"Layer_Effect",
		ENUM_CLASS(LEVEL::LEVEL_STATIC), L"Prototype_GameObject_PlayerEffect", &EffectDesc);
	return;
}

CEffect_Factory* CEffect_Factory::Create()
{
	return new CEffect_Factory();
}

void CEffect_Factory::Free()
{
	__super::Free();
	Safe_Release(m_pGameInstance);
	DestroyInstance();
}
