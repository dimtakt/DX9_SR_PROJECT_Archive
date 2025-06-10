#include "Askard_Tentacle.h"
#include "Effect_Factory.h"

CAskard_Tentacle::CAskard_Tentacle(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{
}

CAskard_Tentacle::CAskard_Tentacle(const CAskard_Tentacle& Prototype)
	: CMonster{ Prototype }
{
}

HRESULT CAskard_Tentacle::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAskard_Tentacle::Initialize(void* pArg)
{
	if (FAILED(__super::Initialize(pArg)))
		return E_FAIL;



	// 타입 우선 결정 후 Ready_Components 실행
	TENTACLE_DESC* pDesc = static_cast<TENTACLE_DESC*>(pArg);

	// 1. 일반(1번)
	// 2. 일반(2번)
	// 3. 일반(3번)
	// 4. 별종(1번 기반, 패턴 적용, 보스 패턴 하나 당 한개만 랜덤히 생성)
	m_eType = pDesc->eType;

	// 위치 반영
	_float4x4 matTransform = pDesc->matTransform;
	for (int i = 0; i < 3; i++)
		m_pTransformCom->Set_State(STATE(i), *reinterpret_cast<_float3*>(&matTransform.m[i]));
	m_pTransformCom->Set_State(STATE::POSITION, *reinterpret_cast<_float3*>(&matTransform.m[3]));
	

	if (FAILED(this->Ready_Components(pArg)))
		return E_FAIL;


	// 플레이어 Transform 할당
	m_pTargetTransformCom = m_pTargetTransformCom = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Layer_Player"), TEXT("Com_Transform")));
	if (m_pTargetTransformCom == nullptr)
		return E_FAIL;




	return S_OK;
}

void CAskard_Tentacle::Priority_Update(_float fTimeDelta)
{
}

void CAskard_Tentacle::Update(_float fTimeDelta)
{
	// 타입에 따라 패턴 다르게.




	switch (m_eType)
	{
	case Client::CAskard_Tentacle::TYPE_TENTACLE::TYPE_NORMAL_1:
	case Client::CAskard_Tentacle::TYPE_TENTACLE::TYPE_NORMAL_2:
	case Client::CAskard_Tentacle::TYPE_TENTACLE::TYPE_NORMAL_3:
	{
		// 일반 타입은 이미지만 다르게, 패턴은 동일
		// (튀어나오고 다시 들어가는게 끝)

		// ..
	}
		break;
	case Client::CAskard_Tentacle::TYPE_TENTACLE::TYPE_ODD_1:
	{
		// 별종 타입은 생성 뒤 눈 생기고 플레이어 대기모드
		// 다가오면 공격 후 다시 들어감
		// 안 다가오면 일정시간 뒤 폭발후 유도탄 방출

		// ..
	}
		break;
	case Client::CAskard_Tentacle::TYPE_TENTACLE::TYPE_END:
		break;
	default:
		break;
	}





	if (m_pTerrainBox != nullptr) {		// 확인해보고 높이 수정
		m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, _float3(0.05f, 0.8f, 0.05f));
	}
}

void CAskard_Tentacle::Late_Update(_float fTimeDelta)
{
}

HRESULT CAskard_Tentacle::Render()
{
	return S_OK;
}

HRESULT CAskard_Tentacle::Ready_Components(void* pArg)
{
	/* For.Com_Texture */

	// 참고
	// 
	// BigRoot_Hall_Ready		: 튀어나올 때 생기는 균열
	//+BigRoot_Hall_FX_Open		: 튀어나올 때 생기는 [이펙트]
	// BigRoot_Hall_Open		: 튀어나올 때 열리는 이미지
	// BigRoot_Hall_Opened		: 열려있는 상태. 단일 이미지
	// BigRoot_Hall_Close		: 들어갈 때 닫히는 이미지
	// 
	// BigRoot_Root_EyeOpen		: 별종이 눈 생기는 이미지
	//+BigRoot_Hall_FX_EyeOpen	: 별종이 눈 생길때의 [이펙트]
	// 
	// BigRoot_Root_Up			: 튀어나올 때의 본체 이미지, 이하 2종 동일
	// BigRoot_Root2_Up
	// BigRoot_Root3_Up 
	// BigRoot_Root_Down		: 들어갈 때의 본체 이미지, 이하 2종 동일
	// BigRoot_Root2_Down
	// BigRoot_Root3_Down
	//
	// BigRoot_Root_ExplosionReady	: 별종관리 안할 시 폭발 준비하는 이미지
	//+BigRoot_Root_FX_Explosion	: 폭발하는 [이펙트]
	//
	// BigRoot_Root_AttackReady		: 별종과 가까이 있을 때 공격준비 상태
	// BigRoot_Root_Attack			: 별종의 공격 모션 (이미지 수정 필요)
	//+BigRoot_Root_FX_Swing		: 공격시 휘두르는 [이펙트]


	// Argument 로 1, 2, 3번중 어떤게 초기상태가 될지,
	// 만약 1번이면 별종인지, 생성 위치는 어디로 할 지 정보 받아오면 될듯

	// 행동 패턴에 따라 이펙트 생성은 여기서 실행시켜야 할 듯
	// 생성 자체는 for문 돌려서 보스 본체에서 생성하도록
	

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_BOSS1), L"Prototype_Component_Boss_Askard_BigRoot_Root1_Up",
		TEXT("Com_Texture_Root_Up_1"), reinterpret_cast<CComponent**>(&m_pTextureCom_Root_Up_1))))
		return E_FAIL;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_BOSS1), L"Prototype_Component_Boss_Askard_BigRoot_Root2_Up",
		TEXT("Com_Texture_Root_Up_2"), reinterpret_cast<CComponent**>(&m_pTextureCom_Root_Up_2))))
		return E_FAIL;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_BOSS1), L"Prototype_Component_Boss_Askard_BigRoot_Root3_Up",
		TEXT("Com_Texture_Root_Up_3"), reinterpret_cast<CComponent**>(&m_pTextureCom_Root_Up_3))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_BOSS1), L"Prototype_Component_Boss_Askard_BigRoot_Root1_Down",
		TEXT("Com_Texture_Root_Down_1"), reinterpret_cast<CComponent**>(&m_pTextureCom_Root_Down_1))))
		return E_FAIL;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_BOSS1), L"Prototype_Component_Boss_Askard_BigRoot_Root2_Down",
		TEXT("Com_Texture_Root_Down_2"), reinterpret_cast<CComponent**>(&m_pTextureCom_Root_Down_2))))
		return E_FAIL;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_BOSS1), L"Prototype_Component_Boss_Askard_BigRoot_Root3_Down",
		TEXT("Com_Texture_Root_Down_3"), reinterpret_cast<CComponent**>(&m_pTextureCom_Root_Down_3))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_BOSS1), L"Prototype_Component_Boss_Askard_BigRoot_Root_EyeOpen",
		TEXT("Com_Texture_Root_EyeOpen"), reinterpret_cast<CComponent**>(&m_pTextureCom_Root_EyeOpen))))
		return E_FAIL;
	// Hall_FX_EyeOpen (6)
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_BOSS1), L"Prototype_Component_Boss_Askard_BigRoot_Root_EyeOpened",
		TEXT("Com_Texture_Root_EyeOpened"), reinterpret_cast<CComponent**>(&m_pTextureCom_Root_EyeOpened))))
		return E_FAIL;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_BOSS1), L"Prototype_Component_Boss_Askard_BigRoot_Root_AttackReady",
		TEXT("Com_Texture_Root_AttackReady"), reinterpret_cast<CComponent**>(&m_pTextureCom_Root_AttackReady))))
		return E_FAIL;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_BOSS1), L"Prototype_Component_Boss_Askard_BigRoot_Root_Attack",
		TEXT("Com_Texture_Root_Attack"), reinterpret_cast<CComponent**>(&m_pTextureCom_Root_Attack))))
		return E_FAIL;
	// Root_FX_Swing (6)

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_BOSS1), L"Prototype_Component_Boss_Askard_BigRoot_Root_ExplosionReady",
		TEXT("Com_Texture_Root_ExplosionReady"), reinterpret_cast<CComponent**>(&m_pTextureCom_Root_ExplosionReady))))
		return E_FAIL;
	// Root_FX_Explosion (8)


	/* For Com_Animator */
	CAnimator::ANIMSTATE_DESC StartAnimStateDesc{};
	StartAnimStateDesc.strTimerTag = L"Animator_Boss_Askard_Tentacle";   // 해당 애니메이터가 타이머에서 사용할 태그 key값
	StartAnimStateDesc.pParentTransform = nullptr;
	StartAnimStateDesc.pChildTransform = nullptr;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Animator"),
		TEXT("Com_Animator"), reinterpret_cast<CComponent**>(&m_pAnimatorCom), &StartAnimStateDesc)))
		return E_FAIL;

	// 컴포넌트 추가해야함



	CAnimator::ANIMSTATE_DESC StartAnimStateDesc2{};
	StartAnimStateDesc2.strTimerTag = L"Animator_Boss_Askard_Tentacle_Pattern";   // 해당 애니메이터가 타이머에서 사용할 태그 key값
	StartAnimStateDesc2.pParentTransform = nullptr;
	StartAnimStateDesc2.pChildTransform = nullptr;





}

void CAskard_Tentacle::OnCollision(CGameObject* pGameObject)
{
	__super::OnCollision(pGameObject);
}

CAskard_Tentacle* CAskard_Tentacle::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CAskard_Tentacle* pInstance = new CAskard_Tentacle(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CAskard_Tentacle"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CAskard_Tentacle::Clone(void* pArg)
{
	CAskard_Tentacle* pInstance = new CAskard_Tentacle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CAskard_Tentacle"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CAskard_Tentacle::Free()
{
	__super::Free();

	Safe_Release(m_pTextureCom_Root_Up_1);
	Safe_Release(m_pTextureCom_Root_Up_2);
	Safe_Release(m_pTextureCom_Root_Up_3);

	Safe_Release(m_pTextureCom_Root_Down_1);
	Safe_Release(m_pTextureCom_Root_Down_2);
	Safe_Release(m_pTextureCom_Root_Down_3);

	Safe_Release(m_pTextureCom_Root_EyeOpen);
	Safe_Release(m_pTextureCom_Root_EyeOpened);
	Safe_Release(m_pTextureCom_Root_AttackReady);
	Safe_Release(m_pTextureCom_Root_Attack);

	Safe_Release(m_pTextureCom_Root_ExplosionReady);

	Safe_Release(m_pAnimatorCom);
}