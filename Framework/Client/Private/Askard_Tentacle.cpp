#include "Askard_Tentacle.h"
#include "Effect_Factory.h"

#include "Room_Manager.h"

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
	//if (FAILED(__super::Initialize(pArg)))
	//	return E_FAIL;

	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;
	
	if (FAILED(this->Ready_Components(pArg)))
		return E_FAIL;

	//desc..

	TENTACLE_DESC* desc = static_cast<TENTACLE_DESC*>(pArg);

	CTransform* pTerrainTransform = dynamic_cast<CTransform*>(desc->pTerrainBox->Find_Component(TEXT("Com_Transform_TerrainBox")));
	m_pTerrainTransformCom = pTerrainTransform;
	_float3 fTerrainPos = pTerrainTransform->Get_State(STATE::POSITION);
	_float3 fTerrainScale = pTerrainTransform->Get_Scaled();
	m_pTransformCom->Scaling(6.f, 6.f, 6.f);

	m_pTransformCom->Set_State(STATE::POSITION, desc->vPosition);


	m_pTerrainBox = desc->pTerrainBox;
	Safe_AddRef(m_pTerrainBox);
	m_eObjType = GAMEOBJ_TYPE::MONSTER;

	m_dwHitTime = 0.f;

	// 타입 우선 결정 후 Ready_Components 실행
	TENTACLE_DESC* pDesc = static_cast<TENTACLE_DESC*>(pArg);

	// 1. 일반(1번)
	// 2. 일반(2번)
	// 3. 일반(3번)
	// 4. 별종(1번 기반, 패턴 적용, 보스 패턴 하나 당 한개만 랜덤히 생성)
	m_eType = pDesc->eType;
	//m_eType = CAskard_Tentacle::TYPE_TENTACLE::TYPE_NORMAL_1;


	// 시작부터 이미지 분기처리
	switch (m_eType)
	{
	case Client::CAskard_Tentacle::TYPE_TENTACLE::TYPE_NORMAL_1:
		m_pAnimatorCom->Change_State(L"Root_Ready_1", true, 0.1f, true);
		break;
	case Client::CAskard_Tentacle::TYPE_TENTACLE::TYPE_NORMAL_2:
		m_pAnimatorCom->Change_State(L"Root_Ready_2", true, 0.1f, true);
		break;
	case Client::CAskard_Tentacle::TYPE_TENTACLE::TYPE_NORMAL_3:
		m_pAnimatorCom->Change_State(L"Root_Ready_3", true, 0.1f, true);
		break;
	case Client::CAskard_Tentacle::TYPE_TENTACLE::TYPE_ODD_1:
		m_pAnimatorCom->Change_State(L"Root_Ready_1", true, 0.1f, true);
		break;
	}


	// 위치 반영
	//_float4x4 matTransform = {};
	//D3DXMatrixIdentity(&matTransform);

	//D3DXMatrixTranslation(&matTransform, pDesc->vPosition.x, pDesc->vPosition.y, pDesc->vPosition.z);
	//
	//for (int i = 0; i < 3; i++)
	//	m_pTransformCom->Set_State(STATE(i), *reinterpret_cast<_float3*>(&matTransform.m[i]));
	//m_pTransformCom->Set_State(STATE::POSITION, *reinterpret_cast<_float3*>(&matTransform.m[3]));

	// 플레이어 Transform 할당
	LEVEL eCurLevel = static_cast<LEVEL>(m_pGameInstance->Get_CurrentLevel());
	m_pTargetTransformCom = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(ENUM_CLASS(eCurLevel), TEXT("Layer_Player"), TEXT("Com_Transform")));
	if (m_pTargetTransformCom == nullptr)
		return E_FAIL;




	return S_OK;
}

void CAskard_Tentacle::Priority_Update(_float fTimeDelta)
{

}

void CAskard_Tentacle::Update(_float fTimeDelta)
{

	// 아래에서 사용할 변수들
#pragma region Variables Setting

	_uint iCurLevel = CGameInstance::GetInstance()->Get_CurrentLevel();

	CTransform* pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->GetInstance()->Get_Component(iCurLevel, TEXT("Layer_Player"), TEXT("Com_Transform")));

	_float3 vMonsterPos = m_pTransformCom->Get_State(STATE::POSITION);
	_float3 vTargetPos = pTargetTransform->Get_State(STATE::POSITION);

	_float3 vDiff = -vMonsterPos + vTargetPos;
	_float fDistance = D3DXVec3Length(&vDiff);


	// ********* matMonster 구하기
	_float4x4 matMonsterWorld = *m_pTransformCom->Get_WorldMatrix();

	_wstring strCurStateTag = m_pAnimatorCom->Get_CurStateTag();

#pragma endregion

	// **** 이펙트 크기조절용 초기설정
#pragma region Effect Setting

	// 이펙트용
	// 1. 원점으로 이동
	_float4x4 matTransToOrigin = {};
	D3DXMatrixIdentity(&matTransToOrigin);
	D3DXMatrixTranslation(&matTransToOrigin, -matMonsterWorld._41, -matMonsterWorld._42, -matMonsterWorld._43);

	// 2. 크기
	_float4x4 matScale = {};
	D3DXMatrixIdentity(&matScale);
	D3DXMatrixScaling(&matScale, 6.f, 6.f, 6.f);

	// 3. 자전
	_float4x4 matRotateChild = {};
	D3DXMatrixIdentity(&matRotateChild);
	//D3DXMatrixRotationX(&matRotateChild, D3DXToRadian(-90)); // 안되면 -90도도 해보기

	_float4x4 matRotateChildtoPlayer = {};
	D3DXMatrixIdentity(&matRotateChildtoPlayer);
	_float fAngle = atan2f(vTargetPos.x - vMonsterPos.x, vTargetPos.z - vMonsterPos.z);
	_float fDegree = D3DXToDegree(fAngle) + 180;
	//D3DXMatrixRotationY(&matRotateChildtoPlayer, D3DXToRadian(fDegree));

	// 4. 원래 위치(몬스터)로 재이동
	_float4x4 matTransReturn = {};
	D3DXMatrixIdentity(&matTransReturn);
	D3DXMatrixTranslation(&matTransReturn, matMonsterWorld._41, matMonsterWorld._42, matMonsterWorld._43);

	// 5. 거기에 추가 이동 (플레이어 방향)
	_float4x4 matTransAddition = {};
	D3DXMatrixIdentity(&matTransAddition);
	//_float3 vDiff = -vPlayerPos + vMonsterPos;       // 플레이어 위치에서 마우스 교차좌표로 가는 벡터
	D3DXVec3Normalize(&vDiff, &vDiff);              // 를 단위벡터화, 안되면 vDiff 순서 바꿔보기
	_float fDistanceOffset = 1.0f;                   // ** ksta : 중점으로부터 떨어져 있을 거리 **
	vDiff *= fDistanceOffset;
	//D3DXMatrixTranslation(&matTransAddition, vDiff.x, 0, vDiff.z);

	matMonsterWorld = matTransToOrigin * matScale * matRotateChild * matRotateChildtoPlayer * matTransReturn * matTransAddition;

#pragma endregion
	// ***********************


	// 공통 이펙트

	// 단순 바닥에 붙는 이펙트 요구값 통일
	_float3 vPos = vMonsterPos + _float3{ 0.f, -0.99f, 0.f };

	D3DXQUATERNION qRot = { 0, 0, 0, 1 };
	D3DXQUATERNION qNonRot = { 0, 0, 0, 1 };

	_float3 vAxis = { 1, 0, 0 };
	_float3 vScale = { 6, 6, 6 };
	D3DXQuaternionRotationAxis(&qRot, &vAxis, D3DXToRadian(90.f));

	if (m_iStackedFrames == 1)
	{
		// 돌출 이펙트
		CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Boss_Askard_BigRoot_Hall_Ready",
			vPos, qRot, vScale);
	}
	else if (m_iStackedFrames == 30)
	{
		// 열리는 이펙트
		CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Boss_Askard_BigRoot_Hall_Open",
			vPos, qRot, vScale);
		CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::MONSTER_EFFECT, L"Prototype_Component_Boss_Askard_BigRoot_Hall_FX_Open",
			vPos, qRot, vScale);
	}

	if (m_iStackedFrames >= 40 &&			// 열린 이후 1프레임마다 계속 생성. 성능 많이 쓰면 바꿔야할듯
		m_iStackedFrames % 4 == 0 &&
		(m_ePattern == PATTERN_TENTACLE::PT_DOWNWAIT ||	// 이 상태에만 열려있어야 함
		m_ePattern == PATTERN_TENTACLE::PT_WAITTARGET ||
		m_ePattern == PATTERN_TENTACLE::PT_ATTACK))
		CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Boss_Askard_BigRoot_Hall_Opened",
			vPos, qRot, vScale);




	// 유형별 패턴

	switch (m_eType)
	{
	case Client::CAskard_Tentacle::TYPE_TENTACLE::TYPE_NORMAL_1:
	{
		if (strCurStateTag == L"Root_Ready_1")
			m_pAnimatorCom->Change_State(L"Root_Up_1");
		else if (strCurStateTag == L"Root_Up_1")
		{
			if (m_pAnimatorCom->Change_State(L"Root_DownWait_1"))
				m_ePattern = PATTERN_TENTACLE::PT_DOWNWAIT;
		}
		else if (strCurStateTag == L"Root_DownWait_1")
		{
			if (m_pAnimatorCom->Change_State(L"Root_Down_1"))
			{
				m_ePattern = PATTERN_TENTACLE::PT_DOWN;
				CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Boss_Askard_BigRoot_Hall_Close",
					vPos, qRot, vScale);
			}
		}
		else if (strCurStateTag == L"Root_Down_1" &&
			m_pAnimatorCom->Get_IsLastFrame())
			m_bDead = true;

	}
		break;
	case Client::CAskard_Tentacle::TYPE_TENTACLE::TYPE_NORMAL_2:
	{
		if (strCurStateTag == L"Root_Ready_2")
			m_pAnimatorCom->Change_State(L"Root_Up_2");
		else if (strCurStateTag == L"Root_Up_2")
		{
			if (m_pAnimatorCom->Change_State(L"Root_DownWait_2"))
				m_ePattern = PATTERN_TENTACLE::PT_DOWNWAIT;
		}
		else if (strCurStateTag == L"Root_DownWait_2")
		{
			if (m_pAnimatorCom->Change_State(L"Root_Down_2"))
			{
				m_ePattern = PATTERN_TENTACLE::PT_DOWN;
				CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Boss_Askard_BigRoot_Hall_Close",
					vPos, qRot, vScale);
			}
		}
		else if (strCurStateTag == L"Root_Down_2" &&
			m_pAnimatorCom->Get_IsLastFrame())
			m_bDead = true;

	}
		break;
	case Client::CAskard_Tentacle::TYPE_TENTACLE::TYPE_NORMAL_3:
	{
		if (strCurStateTag == L"Root_Ready_3")
			m_pAnimatorCom->Change_State(L"Root_Up_3");
		else if (strCurStateTag == L"Root_Up_3")
		{
			if (m_pAnimatorCom->Change_State(L"Root_DownWait_3"))
				m_ePattern = PATTERN_TENTACLE::PT_DOWNWAIT;
		}
		else if (strCurStateTag == L"Root_DownWait_3")
		{
			if (m_pAnimatorCom->Change_State(L"Root_Down_3"))
			{
				m_ePattern = PATTERN_TENTACLE::PT_DOWN;
				CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Boss_Askard_BigRoot_Hall_Close",
					vPos, qRot, vScale);
			}
		}
		else if (strCurStateTag == L"Root_Down_3" &&
			m_pAnimatorCom->Get_IsLastFrame())
			m_bDead = true;
	}
		break;
	case Client::CAskard_Tentacle::TYPE_TENTACLE::TYPE_ODD_1:
	{
		// 별종 타입은 생성 뒤 눈 생기고 플레이어 대기모드
		// 다가오면 공격 후 다시 들어감
		// 안 다가오면 일정시간 뒤 폭발후 유도탄 방출
		if (strCurStateTag == L"Root_Ready_1")
			m_pAnimatorCom->Change_State(L"Root_Up_1");
		else if (strCurStateTag == L"Root_Up_1")
		{
			if (m_pAnimatorCom->Change_State(L"Root_DownWait_1"))
				m_ePattern = PATTERN_TENTACLE::PT_DOWNWAIT;
		}
		else if (strCurStateTag == L"Root_DownWait_1" &&
			m_pAnimatorCom->Get_CurStackedFrame() >= 12)
		{
			if (m_pAnimatorCom->Change_State(L"Root_EyeOpen"))
			{
				// ksta : 눈떠짐 이펙트 생성
				//Prototype_Component_Boss_Askard_BigRoot_Hall_FX_EyeOpen;
				CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Boss_Askard_BigRoot_Hall_FX_EyeOpen",
					vPos + _float3{0.f, 0.f, -0.01f}, qNonRot, vScale);
			}
		}
		else if (strCurStateTag == L"Root_EyeOpen")
		{
			m_pAnimatorCom->Change_State(L"Root_EyeOpened");
			m_ePattern = PATTERN_TENTACLE::PT_WAITTARGET;
		}
		else if (strCurStateTag == L"Root_AttackReady")
		{
			// ksta : 공격 이펙트 생성
			if (m_pAnimatorCom->Change_State(L"Root_Attack"))
				m_pGameInstance->StopSound(ENUM_CLASS(CHANNELID::SOUND_MONSTER_EFFECT));
				m_pGameInstance->PlaySoundW(L"hitBash03.wav", ENUM_CLASS(CHANNELID::SOUND_MONSTER_EFFECT), g_fEFFECTVolume - 0.6f);
				CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::MONSTER_EFFECT, L"Prototype_Component_Boss_Askard_BigRoot_Root_FX_Swing",
					vPos + _float3(0, 0.5f, 0) + _float3{0.f, 0.f, -0.01f}, qRot, vScale * 1.8f);
		}
		else if (strCurStateTag == L"Root_Attack")
		{
			if (m_pAnimatorCom->Change_State(L"Root_Down_2"))
			{
				m_ePattern = PATTERN_TENTACLE::PT_DOWN;
				CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Boss_Askard_BigRoot_Hall_Close",
					vPos, qRot, vScale);
			}
		}
		else if (strCurStateTag == L"Root_Down_2" &&
			m_pAnimatorCom->Get_IsLastFrame())
			m_bDead = true;


		if (m_ePattern == PATTERN_TENTACLE::PT_WAITTARGET)
		{
			_float3 vPos = m_pTransformCom->Get_State(STATE::POSITION);
			_float3 vTargetPos = m_pTargetTransformCom->Get_State(STATE::POSITION);
			_float3 vDiff = vPos - vTargetPos;

			_float fDistance = D3DXVec3Length(&vDiff);

			// 촉수가 근접 시 공격할 거리
			if (fDistance <= 3.f)
			{
				m_ePattern = PATTERN_TENTACLE::PT_ATTACK;
				m_pAnimatorCom->Change_State(L"Root_AttackReady");
			}
		}


	}
		break;
	}



	m_iStackedFrames++;

	if (m_pTerrainBox != nullptr) {		// 확인해보고 높이 수정
		m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, _float3(0.f, 0.5f, 0.f));
	}
}

void CAskard_Tentacle::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}

HRESULT CAskard_Tentacle::Render()
{
	//if (m_pAnimatorCom->Get_CurState()->pTextureCom == nullptr)
	//	return S_OK;

	if (!m_pTransformCom)
		return S_OK;

	SetUp_RenderState();

	m_pTransformCom->Bind_Matrix();


	m_pAnimatorCom->Update_State(); // Bind_Texture

	m_pVIBufferCom->Bind_Buffers();


	m_pVIBufferCom->Render();

	m_pTerrainBox->Render();


	Reset_RenderState();


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
	

	


	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_BOSS2), L"Prototype_Component_Boss_Askard_BigRoot_Root_Standby",
		TEXT("Com_Texture_Root_Standby"), reinterpret_cast<CComponent**>(&m_pTextureCom_Root_Standby))))
		return E_FAIL;


	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_BOSS2), L"Prototype_Component_Boss_Askard_BigRoot_Root1_Up",
		TEXT("Com_Texture_Root_Up_1"), reinterpret_cast<CComponent**>(&m_pTextureCom_Root_Up_1))))
		return E_FAIL;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_BOSS2), L"Prototype_Component_Boss_Askard_BigRoot_Root2_Up",
		TEXT("Com_Texture_Root_Up_2"), reinterpret_cast<CComponent**>(&m_pTextureCom_Root_Up_2))))
		return E_FAIL;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_BOSS2), L"Prototype_Component_Boss_Askard_BigRoot_Root3_Up",
		TEXT("Com_Texture_Root_Up_3"), reinterpret_cast<CComponent**>(&m_pTextureCom_Root_Up_3))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_BOSS2), L"Prototype_Component_Boss_Askard_BigRoot_Root1_DownWait",
		TEXT("Com_Texture_Root_DownWait_1"), reinterpret_cast<CComponent**>(&m_pTextureCom_Root_DownWait_1))))
		return E_FAIL;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_BOSS2), L"Prototype_Component_Boss_Askard_BigRoot_Root2_DownWait",
		TEXT("Com_Texture_Root_DownWait_2"), reinterpret_cast<CComponent**>(&m_pTextureCom_Root_DownWait_2))))
		return E_FAIL;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_BOSS2), L"Prototype_Component_Boss_Askard_BigRoot_Root3_DownWait",
		TEXT("Com_Texture_Root_DownWait_3"), reinterpret_cast<CComponent**>(&m_pTextureCom_Root_DownWait_3))))
		return E_FAIL;




	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_BOSS2), L"Prototype_Component_Boss_Askard_BigRoot_Root1_Down",
		TEXT("Com_Texture_Root_Down_1"), reinterpret_cast<CComponent**>(&m_pTextureCom_Root_Down_1))))
		return E_FAIL;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_BOSS2), L"Prototype_Component_Boss_Askard_BigRoot_Root2_Down",
		TEXT("Com_Texture_Root_Down_2"), reinterpret_cast<CComponent**>(&m_pTextureCom_Root_Down_2))))
		return E_FAIL;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_BOSS2), L"Prototype_Component_Boss_Askard_BigRoot_Root3_Down",
		TEXT("Com_Texture_Root_Down_3"), reinterpret_cast<CComponent**>(&m_pTextureCom_Root_Down_3))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_BOSS2), L"Prototype_Component_Boss_Askard_BigRoot_Root_EyeOpen",
		TEXT("Com_Texture_Root_EyeOpen"), reinterpret_cast<CComponent**>(&m_pTextureCom_Root_EyeOpen))))
		return E_FAIL;
	// Hall_FX_EyeOpen (6)
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_BOSS2), L"Prototype_Component_Boss_Askard_BigRoot_Root_EyeOpened",
		TEXT("Com_Texture_Root_EyeOpened"), reinterpret_cast<CComponent**>(&m_pTextureCom_Root_EyeOpened))))
		return E_FAIL;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_BOSS2), L"Prototype_Component_Boss_Askard_BigRoot_Root_AttackReady",
		TEXT("Com_Texture_Root_AttackReady"), reinterpret_cast<CComponent**>(&m_pTextureCom_Root_AttackReady))))
		return E_FAIL;
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_BOSS2), L"Prototype_Component_Boss_Askard_BigRoot_Root_Attack",
		TEXT("Com_Texture_Root_Attack"), reinterpret_cast<CComponent**>(&m_pTextureCom_Root_Attack))))
		return E_FAIL;
	// Root_FX_Swing (6)

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_BOSS2), L"Prototype_Component_Boss_Askard_BigRoot_Root_ExplosionReady",
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
	m_pAnimatorCom->Add_State(L"Root_Ready_1",			{ m_pTextureCom_Root_Standby, 26, false });		// 1
	m_pAnimatorCom->Add_State(L"Root_Ready_2",			{ m_pTextureCom_Root_Standby, 26, false });
	m_pAnimatorCom->Add_State(L"Root_Ready_3",			{ m_pTextureCom_Root_Standby, 26, false });
	m_pAnimatorCom->Add_State(L"Root_Up_1",				{ m_pTextureCom_Root_Up_1, 4, false });			// 4
	m_pAnimatorCom->Add_State(L"Root_Up_2",				{ m_pTextureCom_Root_Up_2, 4, false });
	m_pAnimatorCom->Add_State(L"Root_Up_3",				{ m_pTextureCom_Root_Up_3, 4, false });
	m_pAnimatorCom->Add_State(L"Root_DownWait_1",		{ m_pTextureCom_Root_DownWait_1, 20, false }); // 1
	m_pAnimatorCom->Add_State(L"Root_DownWait_2",		{ m_pTextureCom_Root_DownWait_2, 20, false });
	m_pAnimatorCom->Add_State(L"Root_DownWait_3",		{ m_pTextureCom_Root_DownWait_3, 20, false });
	m_pAnimatorCom->Add_State(L"Root_Down_1",			{ m_pTextureCom_Root_Down_1, 4, false });		// 5
	m_pAnimatorCom->Add_State(L"Root_Down_2",			{ m_pTextureCom_Root_Down_2, 4, false });
	m_pAnimatorCom->Add_State(L"Root_Down_3",			{ m_pTextureCom_Root_Down_3, 4, false });
	m_pAnimatorCom->Add_State(L"Root_EyeOpen",			{ m_pTextureCom_Root_EyeOpen, 4, false });		// 3
	m_pAnimatorCom->Add_State(L"Root_EyeOpened",		{ m_pTextureCom_Root_EyeOpened, 4, true});		// 1
	m_pAnimatorCom->Add_State(L"Root_AttackReady",		{ m_pTextureCom_Root_AttackReady, 4, true});	// 7
	m_pAnimatorCom->Add_State(L"Root_Attack",			{ m_pTextureCom_Root_Attack, 4, false});		// 7
	m_pAnimatorCom->Add_State(L"Root_ExplosionReady",	{ m_pTextureCom_Root_Attack, 4, false});		// 5
	


	//CAnimator::ANIMSTATE_DESC StartAnimStateDesc2{};
	//StartAnimStateDesc2.strTimerTag = L"Animator_Boss_Askard_Tentacle_Pattern";   // 해당 애니메이터가 타이머에서 사용할 태그 key값
	//StartAnimStateDesc2.pParentTransform = nullptr;
	//StartAnimStateDesc2.pChildTransform = nullptr;

	//if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Animator"),
	//	TEXT("Com_AnimatorPattern"), reinterpret_cast<CComponent**>(&m_pAnimatorPatternCom), &StartAnimStateDesc2)))
	//	return E_FAIL;


	
	return S_OK;

}

void CAskard_Tentacle::OnCollision(CGameObject* pGameObject)
{
	if (pGameObject == nullptr)
		return;
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
	m_pGameInstance->Remove_Collider_ByOwner(this);
	__super::Free();

	Safe_Release(m_pTextureCom_Root_Standby);

	Safe_Release(m_pTextureCom_Root_Up_1);
	Safe_Release(m_pTextureCom_Root_Up_2);
	Safe_Release(m_pTextureCom_Root_Up_3);

	Safe_Release(m_pTextureCom_Root_DownWait_1);
	Safe_Release(m_pTextureCom_Root_DownWait_2);
	Safe_Release(m_pTextureCom_Root_DownWait_3);

	Safe_Release(m_pTextureCom_Root_Down_1);
	Safe_Release(m_pTextureCom_Root_Down_2);
	Safe_Release(m_pTextureCom_Root_Down_3);

	Safe_Release(m_pTextureCom_Root_EyeOpen);
	Safe_Release(m_pTextureCom_Root_EyeOpened);
	Safe_Release(m_pTextureCom_Root_AttackReady);
	Safe_Release(m_pTextureCom_Root_Attack);

	Safe_Release(m_pTextureCom_Root_ExplosionReady);

	Safe_Release(m_pAnimatorCom);
	Safe_Release(m_pAnimatorPatternCom);
}