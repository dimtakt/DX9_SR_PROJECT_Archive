#include "Askard_Dark_Tentacle.h"
#include "Effect_Factory.h"

#include "Room_Manager.h"

CAskard_Dark_Tentacle::CAskard_Dark_Tentacle(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{
}

CAskard_Dark_Tentacle::CAskard_Dark_Tentacle(const CAskard_Dark_Tentacle& Prototype)
	: CMonster{ Prototype }
{
}

HRESULT CAskard_Dark_Tentacle::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAskard_Dark_Tentacle::Initialize(void* pArg)
{
	//if (FAILED(__super::Initialize(pArg)))
	//	return E_FAIL;

	if (FAILED(__super::Ready_Components(pArg)))
		return E_FAIL;

	if (FAILED(this->Ready_Components(pArg)))
		return E_FAIL;


	DARKTENTACLE_DESC* desc = static_cast<DARKTENTACLE_DESC*>(pArg);


	CTransform* pTerrainTransform = dynamic_cast<CTransform*>(desc->pTerrainBox->Find_Component(TEXT("Com_Transform_TerrainBox")));
	m_pTerrainTransformCom = pTerrainTransform;
	_float3 fTerrainPos = pTerrainTransform->Get_State(STATE::POSITION);
	_float3 fTerrainScale = pTerrainTransform->Get_Scaled();
	m_pTransformCom->Scaling(3.f, 3.f, 3.f);

	m_vTargetPos = desc->vPosition;
	m_eDir = desc->eDir;




	m_pTransformCom->Set_State(STATE::POSITION, desc->vPosition);

	m_pTerrainBox = desc->pTerrainBox;
	Safe_AddRef(m_pTerrainBox);
	m_eObjType = GAMEOBJ_TYPE::MONSTER;

	m_dwHitTime = 0.f;

	// 플레이어 Transform 할당
	LEVEL eCurLevel = static_cast<LEVEL>(m_pGameInstance->Get_CurrentLevel());
	m_pTargetTransformCom = dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(ENUM_CLASS(eCurLevel), TEXT("Layer_Player"), TEXT("Com_Transform")));
	if (m_pTargetTransformCom == nullptr)
		return E_FAIL;

	return S_OK;
}


void CAskard_Dark_Tentacle::Priority_Update(_float fTimeDelta)
{

}

void CAskard_Dark_Tentacle::Update(_float fTimeDelta)
{

	// 아래에서 사용할 변수들
#pragma region Variables Setting

	_uint iCurLevel = CGameInstance::GetInstance()->Get_CurrentLevel();

	CTransform* pTargetTransform = dynamic_cast<CTransform*>(m_pGameInstance->GetInstance()->Get_Component(iCurLevel, TEXT("Layer_Player"), TEXT("Com_Transform")));

	_float3 vMonsterPos = m_pTransformCom->Get_State(STATE::POSITION);
	_float3 vTargetPos = m_vTargetPos; //pTargetTransform->Get_State(STATE::POSITION);

	_float3 vDiff = -vMonsterPos + vTargetPos;
	_float fDistance = D3DXVec3Length(&vDiff);


	// ********* matMonster 구하기
	_float4x4 matMonsterWorld = *m_pTransformCom->Get_WorldMatrix();

	//_wstring strCurStateTag = m_pAnimatorCom->Get_CurStateTag();

	_float3 vTerrainPos = m_pTerrainTransformCom->Get_State(STATE::POSITION);
	_float3 vTerrainScale = m_pTerrainTransformCom->Get_Scaled();

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
	D3DXMatrixScaling(&matScale, -1.5f, 1.f, 1.f);

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

	_int iMaxFrame = 150;	// 살아있을 프레임 수

	// 소등촉수 소환 및 진행방향 관련 초기세팅
#pragma region Dark Tentace Transform Setting
	// 이미지 눕히기
	D3DXQUATERNION qRotX = { 0, 0, 0, 1 };
	_float3 vAxisX = { 1, 0, 0 };
	D3DXQuaternionRotationAxis(&qRotX, &vAxisX, D3DXToRadian(90));

	_float3 vScale = { 3.375f, 6.f, 6.f };

	_float3 vStartPos = {};
	_float3 vDir = {};

	D3DXQUATERNION qRotY = { 0, 0, 0, 1 };
	_float3 vAxisY = { 0, 1, 0 };
	_float fRotDegree = 0;

	// 시작위치 재정의
	switch (m_eDir)
	{
	case DARKTENTACLE_DIR::DIR_TO_XNEG:	// x축 음방향
		vDir = { -1, 0, 0 };
		fRotDegree = 270.f;
		vStartPos = { vTerrainPos.x + vTerrainScale.x / 2, vTargetPos.y, vTargetPos.z };
		break;
	case DARKTENTACLE_DIR::DIR_TO_XPOS:	// x축 양방향
		vDir = { 1 ,0, 0 };
		fRotDegree = 90.f;
		vStartPos = { vTerrainPos.x - vTerrainScale.x / 2, vTargetPos.y, vTargetPos.z };
		break;
	case DARKTENTACLE_DIR::DIR_TO_ZNEG:	// z축 음방향
		vDir = { 0, 0, -1 };
		fRotDegree = 180.f;
		vStartPos = { vTargetPos.x, vTargetPos.y,  vTerrainPos.z + vTerrainScale.z / 2 };
		break;
	case DARKTENTACLE_DIR::DIR_TO_ZPOS:	// z축 양방향
		vDir = { 0, 0, 1 };
		fRotDegree = 0.f;
		vStartPos = { vTargetPos.x, vTargetPos.y,  vTerrainPos.z - vTerrainScale.z / 2 };
		break;
	default:
		break;
	}

	D3DXQuaternionRotationAxis(&qRotY, &vAxisY, D3DXToRadian(fRotDegree));
	D3DXQUATERNION qRot = qRotX * qRotY;
#pragma endregion

	// 여기서 제작....
	// 타겟 지점 및 공격 방향을 기준으로
	// 터레인 끝에서 일정 간격을 띄우며 생성,

	if (m_iElapsedFrame == 0)
	{
		CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Boss_Askard_TentacleLaser_Start",
			vStartPos, qRot, vScale);
	}
	else if (m_iElapsedFrame == 39)
	{
		for (int i = 0; i < 6; i++)
		{
			CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::MONSTER_EFFECT, L"Prototype_Component_Boss_Askard_TentacleLaser_Progress",
				vStartPos + vDir * 6.f * i, qRot, vScale, 1.6f);
		}
	}
	else if (m_iElapsedFrame == 135)
	{
		for (int i = 0; i < 6; i++)
		{
			CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Boss_Askard_TentacleLaser_End",
				vStartPos + vDir * 6.f * i, qRot, vScale);
		}
	}

	m_iElapsedFrame++;

	if (m_iElapsedFrame >= iMaxFrame)
		m_bDead = true;
}

void CAskard_Dark_Tentacle::Late_Update(_float fTimeDelta)
{
	__super::Late_Update(fTimeDelta);
}


HRESULT CAskard_Dark_Tentacle::Render()
{
	//if (m_pAnimatorCom->Get_CurState()->pTextureCom == nullptr)
	//	return S_OK;

	//if (!m_pTransformCom)
	//	return S_OK;
	//
	//SetUp_RenderState();
	//
	//m_pTransformCom->Bind_Matrix();
	//
	//
	//m_pAnimatorCom->Update_State(); // Bind_Texture
	//
	//m_pVIBufferCom->Bind_Buffers();
	//
	//
	//m_pVIBufferCom->Render();
	//
	//m_pTerrainBox->Render();
	//
	//
	//Reset_RenderState();


	return S_OK;
}


HRESULT CAskard_Dark_Tentacle::Ready_Components(void* pArg)
{
	//CAnimator::ANIMSTATE_DESC StartAnimStateDesc{};
	//StartAnimStateDesc.strTimerTag = L"Animator_Boss_Askard_Dark_Tentacle";   // 해당 애니메이터가 타이머에서 사용할 태그 key값
	//StartAnimStateDesc.pParentTransform = nullptr;
	//StartAnimStateDesc.pChildTransform = nullptr;

	//if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Animator"),
	//	TEXT("Com_Animator"), reinterpret_cast<CComponent**>(&m_pAnimatorCom), &StartAnimStateDesc)))
	//	return E_FAIL;

	return S_OK;
}



void CAskard_Dark_Tentacle::OnCollision(CGameObject* pGameObject)
{
	__super::OnCollision(pGameObject);
}


CAskard_Dark_Tentacle* CAskard_Dark_Tentacle::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CAskard_Dark_Tentacle* pInstance = new CAskard_Dark_Tentacle(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX(TEXT("Failed to Create : CAskard_Dark_Tentacle"));
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CAskard_Dark_Tentacle::Clone(void* pArg)
{
	CAskard_Dark_Tentacle* pInstance = new CAskard_Dark_Tentacle(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX(TEXT("Failed to Clone : CAskard_Dark_Tentacle"));
		Safe_Release(pInstance);
	}

	return pInstance;
}



void CAskard_Dark_Tentacle::Free()
{
	m_pGameInstance->Remove_Collider_ByOwner(this);
	__super::Free();
	//Safe_Release(m_pAnimatorCom);
}
