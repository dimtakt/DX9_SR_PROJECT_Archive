#include "Erma.h"
#include "Effect_Factory.h"

#include "Erma_Body.h"
#include "Erma_Hand_L.h"
#include "Erma_Hand_R.h"
#include "Erma_Head.h"
#include "Interaction_Normal.h"
#include "Room_Manager.h"
#include "GameInstance.h"

CErma::CErma(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CMonster{ pGraphic_Device }
{
}

CErma::CErma(const CErma& Prototype)
	: CMonster{ Prototype }
{
}

HRESULT CErma::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CErma::Initialize(void* pArg)
{
    if (FAILED(__super::Initialize(pArg)))
        return E_FAIL;

    if (FAILED(this->Ready_Components(pArg)))
        return E_FAIL;

    MONSTERDESC* desc = static_cast<MONSTERDESC*>(pArg);

    CTransform* pTerrainTransform = dynamic_cast<CTransform*>(desc->pTerrainBox->Find_Component(TEXT("Com_Transform_TerrainBox")));
    m_pTerrainTransformCom = pTerrainTransform;
    _float3 fTerrainPos = pTerrainTransform->Get_State(STATE::POSITION);
    _float3 fTerrainScale = pTerrainTransform->Get_Scaled();

    // 위치 정해지는 부분. from __super
    m_pTransformCom->Set_State(STATE::POSITION, _float3(
        fTerrainPos.x,
        0.f,
        fTerrainPos.z + fTerrainScale.z / 2 - 15.f));

    m_pTransformCom->Scaling(3.f, 3.f, 3.f);

    m_isSummoned = true;
    Ready_Object();

    m_iMaxHp = 800;
    m_iCulHp = 800;

    m_eMonsterType = MONSTER_TYPE::ERMA;

    if(FAILED(Ready_Chat()))
        return E_FAIL;

    return S_OK;
}

void CErma::Priority_Update(_float fTimeDelta)
{
    __super::Priority_Update(fTimeDelta);

    if (m_iChatCount < m_iCulChatCount) {
        m_bStart = true;
        m_pChat->Off_Chat();
        m_pChat->End_Chat();
    }
        
    
    /*if (m_pHpBar != nullptr &&
        m_isSummoned)
        m_pHpBar->Render_HP_Progress(m_pTransformCom, m_iCulHp, m_iMaxHp);*/

    // Erma가 각 부속 객체들을 제어하도록 연결
    if (m_pObj_Body == nullptr)     m_pObj_Body     = dynamic_cast<CErma_Body*>     (CRoom_Manager::GetInstance()->Find_CurrentRoom_Monster(MONSTER_TYPE::ERMA_BODY));
    if (m_pObj_Hand_L == nullptr)   m_pObj_Hand_L   = dynamic_cast<CErma_Hand_L*>   (CRoom_Manager::GetInstance()->Find_CurrentRoom_Monster(MONSTER_TYPE::ERMA_HAND_L));
    if (m_pObj_Hand_R == nullptr)   m_pObj_Hand_R   = dynamic_cast<CErma_Hand_R*>   (CRoom_Manager::GetInstance()->Find_CurrentRoom_Monster(MONSTER_TYPE::ERMA_HAND_R));
    if (m_pObj_Head == nullptr)     m_pObj_Head     = dynamic_cast<CErma_Head*>     (CRoom_Manager::GetInstance()->Find_CurrentRoom_Monster(MONSTER_TYPE::ERMA_HEAD));
    

    if (m_pObj_Body->Get_HP() < 0)
    {
        m_iPhase++;
        m_pObj_Body->Fill_HP();
        // 체력 0 되면 강제 무장해제
        m_pAnimatorCom->Change_State(L"Airborne", true, 0.3f, true);
        m_isInCombat = false;
        m_iStackedFrame = 0;
        m_iPatternRandOffset = 0;

        m_pObj_Body->Set_GodMode(true);

        m_pObj_Head->PlayPattern(CErma_Head::PATTERN_HEAD::PT_IDLE, true);
        m_pObj_Body->PlayPattern(CErma_Body::PATTERN_BODY::PT_IDLE, true);
        m_pObj_Hand_L->PlayPattern(CErma_Hand_L::PATTERN_HAND_L::PT_IDLE, true);
        m_pObj_Hand_R->PlayPattern(CErma_Hand_R::PATTERN_HAND_R::PT_IDLE, true);
        m_pGameInstance->StopSound(ENUM_CLASS(CHANNELID::SOUND_MONSTER));
        m_pGameInstance->PlaySoundW(L"golemDie01.wav", ENUM_CLASS(CHANNELID::SOUND_MONSTER), g_fEFFECTVolume - 0.6f);
    }
    if (m_iCulHp <= 0)
    {
        m_pObj_Body->PlayPattern(CErma_Body::PATTERN_BODY::PT_BROKEN, true);
        m_pObj_Hand_L->PlayPattern(CErma_Hand_L::PATTERN_HAND_L::PT_IDLE, true);
        m_pObj_Hand_R->PlayPattern(CErma_Hand_R::PATTERN_HAND_R::PT_IDLE, true);
        m_pObj_Head->PlayPattern(CErma_Head::PATTERN_HEAD::PT_BROKEN, true);
        m_pGameInstance->StopSound(ENUM_CLASS(CHANNELID::SOUND_MONSTER));
        m_pGameInstance->PlaySoundW(L"golemDie02.wav", ENUM_CLASS(CHANNELID::SOUND_MONSTER), g_fEFFECTVolume - 0.6f);
        _float3 vPos = m_pTransformCom->Get_State(STATE::POSITION);

        CGameObject* pGameObject = nullptr;
        MAP_OBJECT_DESC tSrc{};
        tSrc.eType = GAMEOBJ_TYPE::ATIFACT;
        tSrc.vPos = _float3(vPos.x - 0.9f, 3.f, vPos.z - 2.f);
        tSrc.vScale = _float3(1.f, 1.f, 1.f);
        tSrc.vRotate = _float3(0.f, 0.f, 0.f);

        pGameObject = dynamic_cast<CGameObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Interaction_Normal"), &tSrc));
        CRoom_Manager::GetInstance()->CurrentRoom_AddObject(pGameObject);

        MAP_OBJECT_DESC tSrc2{};
        tSrc2.eType = GAMEOBJ_TYPE::STONE_TABLET;
        tSrc2.vPos = _float3(vPos.x + 0.4f, 3.f, vPos.z - 2.f);
        tSrc2.vScale = _float3(1.5f, 1.5f, 1.5f);
        tSrc2.vRotate = _float3(0.f, 0.f, 0.f);

        pGameObject = dynamic_cast<CGameObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_Interaction_Normal"), &tSrc2));
        CRoom_Manager::GetInstance()->CurrentRoom_AddObject(pGameObject);

        CRoom_Manager::GetInstance()->CurrentRoom_ForcePotalActive();

        m_iCulHp = 0;
        m_bDead = true;
    }

}

void CErma::Update(_float fTimeDelta)
{
    // Update
    // 


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
    _float fDistanceOffset = 1.0f;                   // 중점으로부터 떨어져 있을 거리
    vDiff *= fDistanceOffset;
    //D3DXMatrixTranslation(&matTransAddition, vDiff.x, 0, vDiff.z);

    matMonsterWorld = matTransToOrigin * matScale * matRotateChild * matRotateChildtoPlayer * matTransReturn * matTransAddition;

#pragma endregion
    // ***********************





    // 밖에 있는 상태일때 들어가는 상태변화.
    // m_pAnimatorCom->Get_CurStateTag() == L"Idle" &&
    // !m_isInCombat << 일 때 자동 진행
#pragma region Entering BigGolem (when First Encounter, Phase Change...)
    if (!m_isInCombat)
    {
        if (strCurStateTag == L"Airborne")
        {
            if (!m_pAnimatorCom->Change_State(L"Stun", true, 8.f))
            {
                _float3 vCurPos = m_pTransformCom->Get_State(STATE::POSITION);
                _float3 vToPos = {};
                    
                vToPos = vCurPos + _float3{ 0.f, -m_iStackedFrame * 0.03f,-0.1f };
                m_iStackedFrame++;

                _float fY = m_pTerrainTransformCom->Get_State(STATE::POSITION).y +
                    m_pTerrainTransformCom->Get_Scaled().y / 2;

                if (vToPos.y <= fY)
                    vToPos.y = fY + 0.5f;

                m_pTransformCom->Set_State(STATE::POSITION, vToPos);
            }
        }
        else if (strCurStateTag == L"Stun")
        {
            m_pAnimatorCom->Change_State(L"Down_End");
        }
        else if (strCurStateTag == L"Down_End")
            m_pAnimatorCom->Change_State(L"Idle");

        if (strCurStateTag == L"Idle")
        {
            m_iStackedFrame = 0;
            //if (보스와의 대화 완료 트리거 발동 시)
            if (m_bStart)
            {
                m_pAnimatorCom->Change_State(L"Enter_Progress");
                m_pGameInstance->StopSound(ENUM_CLASS(CHANNELID::SOUND_MONSTER));
                m_pGameInstance->PlaySoundW(L"spellArcaneStart.wav", ENUM_CLASS(CHANNELID::SOUND_MONSTER), g_fEFFECTVolume - 0.6f);
            }
        }
        else if (strCurStateTag == L"Enter_Progress")
        {
            if (m_pAnimatorCom->Change_State(L"Enter_End"))
                m_isEntering = true;
        }
        else if (strCurStateTag == L"Enter_End" && !m_isEntering)
        {
            // ksta : 위치에 도달 할 때 발동 조건도 넣어야 함
            if (m_pAnimatorCom->Change_State(L"Entered"))
            {
                m_pObj_Body->Set_GodMode(false);

                // effect setting
                D3DXMatrixIdentity(&matTransAddition);
                D3DXMatrixTranslation(&matTransAddition, 0.05f, -0.3f, -0.2f);

                matMonsterWorld = matTransToOrigin * matScale * matRotateChild * matRotateChildtoPlayer * matTransReturn * matTransAddition;
                m_pGameInstance->StopSound(ENUM_CLASS(CHANNELID::SOUND_MONSTER));
                m_pGameInstance->PlaySoundW(L"GolemEnter.wav", ENUM_CLASS(CHANNELID::SOUND_MONSTER), g_fEFFECTVolume - 0.6f);
                CEffect_Factory::GetInstance()->Create_Effect(GAMEOBJ_TYPE::NORMAL_EFFECT, L"Prototype_Component_Boss_Erma_Effect_HeadStart",
                    *m_pTransformCom->Get_WorldMatrix(), matMonsterWorld);

                // effect reset
                D3DXMatrixIdentity(&matTransAddition);
                matMonsterWorld = matTransToOrigin * matScale * matRotateChild * matRotateChildtoPlayer * matTransReturn * matTransAddition;

                m_pObj_Head->PlayPattern(CErma_Head::PATTERN_HEAD::PT_AWAKEN);
                m_isInCombat = true;        // 교전 개시
                m_pObj_Body->Fill_HP();
            }
        }
    }

    if (m_isEntering)
    {
        _float3 vHeadPos = m_pObj_Head->Get_Transform()->Get_State(STATE::POSITION);
        _float3 vErmaPos = m_pTransformCom->Get_State(STATE::POSITION);

        _float3 vTargetPos = vHeadPos + _float3{ -0.05f, -0.1f, -0.2f }; // 들어갈 위치 지정...

        m_pTransformCom->Move_To(vTargetPos, fTimeDelta, 0.01f); 
        _float3 vDiff = vTargetPos - vErmaPos;
        if (D3DXVec3Length(&vDiff) < 0.05f)
            m_isEntering = false;
    }
#pragma endregion

    // 전투 개시
#pragma region In Combat.. (Pattern Loop)
    if (m_isInCombat &&
        m_iPauseLeftFrame <= 0)
    {
#pragma region memo

        // 패턴 시간 표     (sec)    (frame)
        // * CErma_Head::PATTERN_HEAD
        // PT_BULLETFIRE    (6)     (360)
        // * CErma_Hand_L::PATTERN_HAND_L
        // PT_STRIKE        (8)     (480)
        // PT_LASER         (10)    (600)
        // * CErma_Hand_R::PATTERN_HAND_R
        // PT_STRIKE        (8)     (480)
        // PT_LASER         (10)    (600)
        // * CErma_Body::PATTERN_BODY
        // PT_MISSILE_L     (5)     (300)
        // PT_MISSILE_R     (5)     (300)

        // 패턴 순서?
        // 1. 손으로 쿵쿵
        // 2. 레이저
        // 3. 머리 탄막 * 2
        // 4. 레이저
        // a. 미사일은 주기적으로
        // 이후 반복?

#pragma endregion
        if (!m_isAllStop)
            m_iStackedFrame++;
        // ===============
        _int iStandardPatternFrame = m_iStackedFrame % 2800;

        // 정규 패턴
        switch (iStandardPatternFrame)
        {
        case 30:
            m_isTriggerSuccess = false;
            m_pObj_Hand_L->PlayPattern(CErma_Hand_L::PATTERN_HAND_L::PT_STRIKE);
            m_pObj_Hand_R->PlayPattern(CErma_Hand_R::PATTERN_HAND_R::PT_STRIKE);
            break;
        case 510:
            m_pObj_Hand_L->PlayPattern(CErma_Hand_L::PATTERN_HAND_L::PT_LASER);
            m_pObj_Hand_R->PlayPattern(CErma_Hand_R::PATTERN_HAND_R::PT_LASER);
            break;
        case 1110:
            m_pObj_Head->PlayPattern(CErma_Head::PATTERN_HEAD::PT_BULLETFIRE);
            break;
        case 1480:
            m_pObj_Head->PlayPattern(CErma_Head::PATTERN_HEAD::PT_BULLETFIRE);
            break;
        case 1840:
            m_pObj_Hand_L->PlayPattern(CErma_Hand_L::PATTERN_HAND_L::PT_LASER);
            m_pObj_Hand_R->PlayPattern(CErma_Hand_R::PATTERN_HAND_R::PT_LASER);
            break;
        case 2440:
            m_pObj_Hand_L->PlayPattern(CErma_Hand_L::PATTERN_HAND_L::PT_KEYPATTERN);
            break;
        case 2620:
            m_isTriggerSuccess = false;
            m_pObj_Hand_R->PlayPattern(CErma_Hand_R::PATTERN_HAND_R::PT_KEYPATTERN);
            break;
        }

        // 수시 패턴 (미사일)
        m_iPatternRandOffset = (m_iPatternRandOffset == 0) ?
            static_cast<int>(m_pGameInstance->Compute_Random(0, 200)) :
            m_iPatternRandOffset;

        if (m_iStackedFrame % 500 == m_iPatternRandOffset)
        {
            if (m_iPatternRandOffset % 2 == 0)
                m_pObj_Body->PlayPattern(CErma_Body::PATTERN_BODY::PT_MISSILE_L);
            else
                m_pObj_Body->PlayPattern(CErma_Body::PATTERN_BODY::PT_MISSILE_R);

            m_iPatternRandOffset = 0;
        }
        

    }
#pragma endregion

    // 키입력 패턴용
    PlayKeyInputPattern();
 


    if (m_pTerrainBox != nullptr &&
        strCurStateTag == L"Idle") {
        m_pTerrainBox->SetUp_OnTerrainBox(m_pTransformCom, _float3(0.05f, 0.7f, 0.05f));
    }
}

void CErma::Late_Update(_float fTimeDelta)
{
    __super::Late_Update(fTimeDelta);

    //_float3 vPos = m_pTransformCom->Get_State(STATE::POSITION);
    //std::cout << "Erma Pos : " << vPos.x << ", " << vPos.y << ", " << vPos.z << std::endl;
    if (m_bStart)
        m_pBossHp->Render_Hpbar(m_iCulHp, m_iMaxHp, m_pObj_Body->Get_HP(), m_pObj_Body->Get_MaxHP(), fTimeDelta);
}

HRESULT CErma::Render()
{
    if (m_pAnimatorCom->Get_CurState()->pTextureCom == nullptr)
        return S_OK;

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

HRESULT CErma::Ready_Components(void* pArg)
{
    MONSTERDESC* desc = static_cast<MONSTERDESC*>(pArg);

    // 컴포넌트 추가
    /* For.Com_Texture */
    // Idle
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Erma_Idle"),
        TEXT("Com_Texture_Idle"), reinterpret_cast<CComponent**>(&m_pTextureCom_Idle))))
        return E_FAIL;
    // Move
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Erma_Move"),
        TEXT("Com_Texture_Move"), reinterpret_cast<CComponent**>(&m_pTextureCom_Move))))
        return E_FAIL;
    // Airborne
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Erma_Airborne"),
        TEXT("Com_Texture_Airborne"), reinterpret_cast<CComponent**>(&m_pTextureCom_Airborne))))
        return E_FAIL;
    // Down
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Erma_Down"),
        TEXT("Com_Texture_Down"), reinterpret_cast<CComponent**>(&m_pTextureCom_Down))))
        return E_FAIL;
    // Down_End
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Erma_Down_End"),
        TEXT("Com_Texture_Down_End"), reinterpret_cast<CComponent**>(&m_pTextureCom_Down_End))))
        return E_FAIL;
    // Stun
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Erma_Stun"),
        TEXT("Com_Texture_Stun"), reinterpret_cast<CComponent**>(&m_pTextureCom_Stun))))
        return E_FAIL;
    // Enter_Progress (Core로 변신)
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Erma_Enter_Progress"),
        TEXT("Com_Texture_Enter_Progress"), reinterpret_cast<CComponent**>(&m_pTextureCom_Enter_Progress))))
        return E_FAIL;
    // Enter_End (변신 완)
    if (FAILED(__super::Add_Component(desc->iLayerLevelIndex, TEXT("Prototype_Component_Boss_Erma_Enter_End"),
        TEXT("Com_Texture_Enter_End"), reinterpret_cast<CComponent**>(&m_pTextureCom_Enter_End))))
        return E_FAIL;
    // Entered (합체 완 / texture은 nullptr로, 기체 탑승한 상태)
    // 이는 상태로 관리


    /* For Com_Animator */
    CAnimator::ANIMSTATE_DESC StartAnimStateDesc{};
    StartAnimStateDesc.strTimerTag = L"Animator_Boss_Erma";   // 해당 애니메이터가 타이머에서 사용할 태그 key값
    StartAnimStateDesc.pParentTransform = nullptr;
    StartAnimStateDesc.pChildTransform = nullptr;

    if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_Component_Animator"),
        TEXT("Com_Animator"), reinterpret_cast<CComponent**>(&m_pAnimatorCom), &StartAnimStateDesc)))
        return E_FAIL;
    // State 삽입
    m_pAnimatorCom->Add_State(L"Idle",              { m_pTextureCom_Idle, 4, true });
    m_pAnimatorCom->Add_State(L"Move",              { m_pTextureCom_Move, 4, true });
    m_pAnimatorCom->Add_State(L"Airborne",          { m_pTextureCom_Airborne, 4, true });
    m_pAnimatorCom->Add_State(L"Down",              { m_pTextureCom_Down, 4, true });
    m_pAnimatorCom->Add_State(L"Down_End",          { m_pTextureCom_Down_End, 4, true });
    m_pAnimatorCom->Add_State(L"Stun",              { m_pTextureCom_Stun, 4, true });
    m_pAnimatorCom->Add_State(L"Enter_Progress",    { m_pTextureCom_Enter_Progress, 4, false });
    m_pAnimatorCom->Add_State(L"Enter_End",         { m_pTextureCom_Enter_End, 4, true });
    m_pAnimatorCom->Add_State(L"Entered",           { nullptr, 4, true });    // 기체 탑승 상태, 텍스쳐 사용 X
    
    // ..




    return S_OK;
}

HRESULT CErma::Ready_Object()
{
    m_pHpBar = dynamic_cast<CField_Hp*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Field_Hp")));

    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Layer_HPBar"), ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_BossHp_Ema"))))
        return E_FAIL;

    m_pBossHp = static_cast<CBossHp_Ema*>(m_pGameInstance->Find_UIObj(ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("BossHp_Ema")));

    return S_OK;
}

HRESULT CErma::Ready_Chat()
{
    CField_Npc_Chat::FIELD_CHAT_DESC desc{};

    desc.pTransform = m_pTransformCom;
    desc.szChatTag = TEXT("ERMA_CHAT");
    desc.m_iLevel = ENUM_CLASS(LEVEL::LEVEL_BOSS1);
    desc.fY = -100;

    if (FAILED(m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("Layer_UI_Chat"), ENUM_CLASS(LEVEL::LEVEL_STATIC), TEXT("Prototype_GameObject_UI_Field_Npc_Chat"), &desc)))
        return E_FAIL;

    m_pChat = static_cast<CField_Npc_Chat*>(m_pGameInstance->Find_UIObj(ENUM_CLASS(LEVEL::LEVEL_BOSS1), TEXT("ERMA_CHAT")));

    m_pChat->Add_Chat(TEXT("집에가고 싶어요...."));
    m_pChat->Add_Chat(TEXT("보내주세요..."));

    m_iChatCount = 2;

    return S_OK;
}

void CErma::Set_AllPartsStop(_bool isStop)
{
    m_pObj_Body->Set_Stop(isStop);
    m_pObj_Hand_L->Set_Stop(isStop);
    m_pObj_Hand_R->Set_Stop(isStop);
    m_pObj_Head->Set_Stop(isStop);
}

void CErma::PlayKeyInputPattern()
{
    // 이상한거..
#pragma region AZ Pattern

    //  키입력 패턴 설정
    _int iKeyNumMin = 4;    // 키입력 최소 갯수 (랜덤)
    _int ikeyNumMax = 8;    // 키입력 최대 갯수 (랜덤)
    _int iPauseSec = 5;     // 키입력을 위해 주어진 시간
    vector<int> vecKeysTable = { 'A', 'W', 'S', 'D' };  // 나올 수 있는 키의 목록

    _int iPauseFrame = iPauseSec * 60;  // 프레임으로 변환



    if (m_iPauseLeftFrame == 0 && m_isTriggerKeyPattern)
    {
        m_iPauseLeftFrame = iPauseFrame;
        Set_AllPartsStop(true);
        m_isTriggerKeyPattern = false;
        m_isTriggerKeyPattern_Activated = true;
    }

    if (!(m_isInCombat &&
        m_iPauseLeftFrame > 0))
        return;


    // 최초 프레임에 한해 입력해야 할 키 생성
    if (iPauseFrame == m_iPauseLeftFrame)
    {
        // 입력해야 하는 키 갯수
        _int iKeyNum = static_cast<_int>(m_pGameInstance->Compute_Random(iKeyNumMin, ikeyNumMax));

        // 키 갯수만큼 생성하여 벡터에 삽입
        for (int i = 0; i < iKeyNum; i++)
        {
            _int iKey = static_cast<_int>(m_pGameInstance->Compute_Random(0, vecKeysTable.size()));
            m_vecKeys.push_back(vecKeysTable[iKey]);
            m_vecOriginKeys.push_back(vecKeysTable[iKey]);
        }

        std::cout << "Input Keys... : ";
        for (auto it = m_vecKeys.begin(); it != m_vecKeys.end(); ++it) {
            std::cout << "[" << static_cast<char>(*it) << "] ";
        }
        std::wcout << std::endl;
    }

    m_iPauseLeftFrame--;


    // 입력한 키가 잘못되었는지 검사
    _bool isWrongKey = false;
    for (auto key : vecKeysTable) {
        if (key != m_vecKeys.front() && m_pGameInstance->IsKeyDown(key)) {
            m_vecInputKeys.push_back(key);
            isWrongKey = true;
        }
    }

    // 키 입력 시 맨 앞의 원소와 일치하는지 확인, 일치 시 제거
    if (m_pGameInstance->IsKeyDown(m_vecKeys.front()))
    {
        //키 입력 체크 사운드
        m_pGameInstance->StopSound(ENUM_CLASS(CHANNELID::EVENT_CLICK));
        m_pGameInstance->PlaySoundW(TEXT("enchantEquipment.wav"), ENUM_CLASS(CHANNELID::EVENT_CLICK), g_fUIVolume-0.4f);

        m_vecInputKeys.push_back(m_vecKeys.front());

        std::cout << "Inputed Keys... : ";
        for (auto it = m_vecInputKeys.begin(); it != m_vecInputKeys.end(); ++it) {
            std::cout << "[" << static_cast<char>(*it) << "] ";
        }
        std::cout << std::endl;

        if (!m_vecKeys.empty())
            m_vecKeys.erase(m_vecKeys.begin());
    }
    
    if (isWrongKey)
    {
        m_pGameInstance->StopSound(ENUM_CLASS(CHANNELID::EVENT_FAIL));
        m_pGameInstance->PlaySoundW(TEXT("Event_AZ_Fail.wav"), ENUM_CLASS(CHANNELID::EVENT_FAIL), g_fUIVolume);

        // 실패 시 다시 생성
        // 키 갯수만큼 생성하여 벡터에 삽입
        m_vecKeys.clear();
        m_vecOriginKeys.clear();
        m_vecInputKeys.clear();
        
        _int iKeyNum = static_cast<_int>(m_pGameInstance->Compute_Random(iKeyNumMin, ikeyNumMax));

        for (int i = 0; i < iKeyNum; i++)
        {
            _int iKey = static_cast<_int>(m_pGameInstance->Compute_Random(0, vecKeysTable.size()));
            m_vecKeys.push_back(vecKeysTable[iKey]);
            m_vecOriginKeys.push_back(vecKeysTable[iKey]);
        }

        std::cout << "Input Keys... : ";
        for (auto it = m_vecKeys.begin(); it != m_vecKeys.end(); ++it) {
            std::cout << "[" << static_cast<char>(*it) << "] ";
        }
        std::wcout << std::endl;
    }


    // 입력을 다 마치면 성공, 아니면 실패
    if (m_vecKeys.empty())
    {
        m_iPauseLeftFrame = 0;
        // kstaA : 패턴 파훼 성공으로, 보상을 주거나 보스의 체력을 깎는 기능을 삽입.
        m_pGameInstance->StopSound(ENUM_CLASS(CHANNELID::EVENT_CLICK));
        m_pGameInstance->StopSound(ENUM_CLASS(CHANNELID::EVENT_CLEAR));
        m_pGameInstance->PlaySoundW(TEXT("legendaryItemFound.wav"), ENUM_CLASS(CHANNELID::EVENT_FAIL), g_fUIVolume);


        std::cout << "Success" << std::endl;
        m_vecKeys.clear();
        m_vecOriginKeys.clear();
        m_vecInputKeys.clear();
        Set_AllPartsStop(false);
        m_isTriggerSuccess = true;

        m_isTriggerKeyPattern_Activated = false;
    }
    else if (m_iPauseLeftFrame == 0)
    {
        // kstaA : 패턴 파훼 실패로, 플레이어의 체력을 깎는 기능을 삽입.
        m_pGameInstance->StopSound(ENUM_CLASS(CHANNELID::EVENT_CLICK));
        m_pGameInstance->StopSound(ENUM_CLASS(CHANNELID::EVENT_FAIL));
        m_pGameInstance->PlaySoundW(TEXT("reroll.wav"), ENUM_CLASS(CHANNELID::EVENT_FAIL), g_fUIVolume);

        std::cout << "Fail" << std::endl;
        m_vecKeys.clear();
        m_vecOriginKeys.clear();
        m_vecInputKeys.clear();
        Set_AllPartsStop(false);

        m_isTriggerKeyPattern_Activated = false;
    }


#pragma endregion
}

void CErma::OnCollision(CGameObject* pGameObject)
{
    __super::OnCollision(pGameObject);

    switch (pGameObject->Get_ObjType())
    {
    case GAMEOBJ_TYPE::PLAYER:
        m_pChat->On_Chat(0, false);
        if (m_pGameInstance->IsKeyDown('F'))
        {
            m_pChat->Off_Chat();
            m_pChat->Cinematic_Chat(0, false);
            m_iCulChatCount++;
        }
        break;
    }
}

CErma* CErma::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
    CErma* pInstance = new CErma(pGraphic_Device);

    if (FAILED(pInstance->Initialize_Prototype()))
    {
        MSG_BOX(TEXT("Failed to Create : CErma"));
        Safe_Release(pInstance);
    }

    return pInstance;
}


CGameObject* CErma::Clone(void* pArg)
{
    CErma* pInstance = new CErma(*this);

    if (FAILED(pInstance->Initialize(pArg)))
    {
        MSG_BOX(TEXT("Failed to Clone : CErma"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CErma::Free()
{
    __super::Free();

    Safe_Release(m_pTextureCom_Idle);
    Safe_Release(m_pTextureCom_Move);
    Safe_Release(m_pTextureCom_Airborne);
    Safe_Release(m_pTextureCom_Down);
    Safe_Release(m_pTextureCom_Down_End);
    Safe_Release(m_pTextureCom_Stun);
    Safe_Release(m_pTextureCom_Enter_Progress);
    Safe_Release(m_pTextureCom_Enter_End);

    Safe_Release(m_pAnimatorCom);
    Safe_Release(m_pBossHp);
    Safe_Release(m_pChat);
    //Safe_Release(m_pAnimatorCom);
}