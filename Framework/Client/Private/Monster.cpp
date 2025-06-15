#include "Monster.h"
#include "GameInstance.h"
#include "Stat_Manager.h"
#include "EXP_Ball.h"
#include "GoldLeaf.h"
#include "Field_Font.h"
CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CMonster::CMonster(const CMonster& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CMonster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	MONSTERDESC* desc = static_cast<MONSTERDESC*>(pArg);

	CTransform* pTerrainTransform = dynamic_cast<CTransform*>(desc->pTerrainBox->Find_Component(TEXT("Com_Transform_TerrainBox")));
	_float3 fTerrainPos = pTerrainTransform->Get_State(STATE::POSITION);
	_float3 fTerrainScale = pTerrainTransform->Get_Scaled();
	m_pTransformCom->Set_State(STATE::POSITION, desc->vPosition);
	m_pTerrainBox = desc->pTerrainBox;
	Safe_AddRef(m_pTerrainBox);
	m_eObjType = GAMEOBJ_TYPE::MONSTER;

	m_dwHitTime = 0.f;

	return S_OK;
}

void CMonster::Priority_Update(_float fTimeDelta)
{
	if (m_bIsHit)
	{
		m_dwHitTime += 1.f;
	}

	if (m_dwHitTime >= 10.f)
	{
		m_dwHitTime = 0.f;
		m_bIsHit = false;
	}

	if (m_iCulHp <= 0) {

		if (m_eMonsterType == MONSTER_TYPE::LASERGHOST)
		{
			m_pGameInstance->StopSound(ENUM_CLASS(CHANNELID::SOUND_MONSTER_LONG_EFFECT));
		}
		
		if (m_bDead || m_bDying)
		{
			Ready_ExpBall();
			Ready_Gold();
		}
		
	}
		
}

void CMonster::Update(_float fTimeDelta)
{

}

void CMonster::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDERGROUP::RG_NONBLEND, this);
}

HRESULT CMonster::Render()
{
	m_pTransformCom->Bind_Matrix();

	if (FAILED(m_pTextureCom->Bind_Texture(0)))
		return E_FAIL;

	/* 그리기위해 이용할 자원과 설정들을 장치에 바인딩한다. */
	m_pVIBufferCom->Bind_Buffers();

	SetUp_RenderState();

	m_pVIBufferCom->Render();

	//m_pTerrainBox->Render();
	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ZENABLE, TRUE);
	Reset_RenderState();

	return S_OK;
}

HRESULT CMonster::Ready_Components(void* pArg)
{
	MONSTERDESC* desc = static_cast<MONSTERDESC*>(pArg);
	
	/* For.Com_VIBuffer*/
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer_Monster"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* For.Com_Texture */
	//if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Texture_Monster"),
	//	TEXT("Com_Texture_Monster"), reinterpret_cast<CComponent**>(&m_pTextureCom))))
	//	return E_FAIL;

	/* For.Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{};
	TransformDesc.fSpeedPerSec = 5.f;
	TransformDesc.fRotationPerSec = D3DXToRadian(90.0f);

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform_Monster"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;
	return S_OK;
}

void CMonster::SetUp_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);

	/* 알파 테스트를 수행한다. */
	/* 현재 픽셀을 그릴때 픽셀에 알파값을 비교한다. 통과한 픽셀만 그린다. */
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);

	/* 현재 그리려고하는 픽셀과 0의 값을 비교한다,ㅡ /.*/
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 125);

	/* 위에서 비교한 결과, 내가 그릴려고하는 픽셀의 알파가 0보다 크면 통과. */
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_POINT);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_NONE);
}

void CMonster::Reset_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MINFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MAGFILTER, D3DTEXF_LINEAR);
	m_pGraphic_Device->SetSamplerState(0, D3DSAMP_MIPFILTER, D3DTEXF_LINEAR);
}

void CMonster::Ready_ExpBall()
{
	if(m_pTransformCom != nullptr && m_bDead)
	{
		CEXP_Ball::EXPBALLDESC desc{};
		desc.fValue = m_pGameInstance->Compute_Random(3.f, 7.f);
		desc.vPosition = m_pTransformCom->Get_State(STATE::POSITION);
		CEXP_Ball* pEXP_Ball = dynamic_cast<CEXP_Ball*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_EXP_Ball"), &desc));
		m_pGameInstance->Add_Direct_GameObject_ToLayer(m_pGameInstance->Get_CurrentLevel(), TEXT("Layer_Exp"), pEXP_Ball);
	}

}

void CMonster::Ready_Gold()
{
	if (m_pTransformCom != nullptr && m_bDead)
	{
		CGoldLeaf::GOLDLEAFDESC desc{};
		desc.fValue = m_pGameInstance->Compute_Random(4.f, 10.f);
		desc.vPosition = m_pTransformCom->Get_State(STATE::POSITION);
		CGoldLeaf* pGold = dynamic_cast<CGoldLeaf*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_GoldLeaf"), &desc));
		m_pGameInstance->Add_Direct_GameObject_ToLayer(m_pGameInstance->Get_CurrentLevel(), TEXT("Layer_Gold"), pGold);
	}

}

void CMonster::OnCollision(CGameObject* pGameObject)
{
	if (pGameObject == nullptr)
		return;
	switch (pGameObject->Get_ObjType())
	{
	case GAMEOBJ_TYPE::PLAYER_EFFECT:
		{
		if (!m_bIsHit)
		{
			//m_bIsHit = true;
		}
			break;
		}
	}
}

void CMonster::Set_Damage(_float fDamage)
{

	if (!m_bIsGodMode)
	{
		m_iCulHp += fDamage;

		Render_Font(static_cast<int>(fDamage * -1));
	}
}

HRESULT CMonster::Render_Font(_int iDamage)
{
	CField_Font::FIELD_FONT_DESC Desc = {};

	Desc.eType = CField_Font::FIELD_FONT_TYPE::MONSTER_DAMAGE;
	Desc.iValue = iDamage;
	Desc.pTransform = m_pTransformCom;

	if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(m_pGameInstance->Get_CurrentLevel(), TEXT("Layer_Field_Font"),
		ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Field_Font"), &Desc)))
		return E_FAIL;
	return S_OK;
}

CMonster* CMonster::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMonster* pInstance = new CMonster(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Created : CMonster"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMonster::Clone(void* pArg)
{
	CMonster* pInstance = new CMonster(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Cloned : CMonster"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster::Free()
{
	m_pGameInstance->Remove_Collider_ByOwner(this);
	__super::Free();

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pTextureCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pTerrainBox);
	Safe_Release(m_pHpBar);
}
