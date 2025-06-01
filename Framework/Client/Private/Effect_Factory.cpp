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

// 커서와 같이 특정 기준점을 기준으로 회전할 필요가 있을 때에 사용
void CEffect_Factory::Create_Effect_RotationByParent(const _wstring& strEffectTag, _float4x4 matEffectWorld, const _float3 axis, _bool isFlippedX, CTransform* parent, _float fRadian)
{
	// 객체가 돌아갈 때의 기준점 행렬 설정, 없으면 원점 기준.
	_float4x4 matTrackTarget;
	if (parent != nullptr)
		matTrackTarget = *parent->Get_WorldMatrix();
	else
		D3DXMatrixIdentity(&matTrackTarget);

	// 변환..

	// 원점으로 중심축 이동
	_float4x4 matToOrigin;
	D3DXMatrixTranslation(&matToOrigin,
		-matTrackTarget._41,
		-matTrackTarget._42,
		-matTrackTarget._43);

	// 축 기준 회전행렬
	_float4x4 matRot;
	D3DXMatrixRotationAxis(&matRot, &axis, fRadian);

	// 다시 제자리로
	_float4x4 matFromOrigin;
	D3DXMatrixTranslation(&matFromOrigin,
		matTrackTarget._41,
		matTrackTarget._42,
		matTrackTarget._43);

	// 다 합치기
	_float4x4 matRotationTotal = matToOrigin * matRot * matFromOrigin;

	// 반영..

	_float4x4 matResult = matEffectWorld * matRotationTotal;
	matEffectWorld = matResult;

	CEffect::EFFECT_DESC EffectDesc;
	EffectDesc.strEffectTag = strEffectTag;
	EffectDesc.matWorld = matEffectWorld;
	EffectDesc.isMatWorld = true;
	EffectDesc.isFlippedX = isFlippedX;


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
