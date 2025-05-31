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

void CEffect_Factory::Create_Effect(const _wstring& strEffectTag, _float3 vPos, D3DXQUATERNION qRot, _float3 vScale)
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

void CEffect_Factory::Create_Effect(const _wstring& strEffectTag, _float4x4 matWorld)
{
	CEffect::EFFECT_DESC EffectDesc;
	EffectDesc.strEffectTag = strEffectTag;
	EffectDesc.matWorld = matWorld;
	EffectDesc.isMatWorld = true;

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
