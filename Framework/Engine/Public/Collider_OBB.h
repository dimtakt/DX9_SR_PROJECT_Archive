#pragma once

#include "Collider.h"
#include "Transform.h"

BEGIN(Engine)

class ENGINE_DLL CCollider_OBB final : public CCollider
{
public:
    typedef struct tagOBB_Desc {
        CGameObject* pOwner;
        CTransform* pTransform; // 게임오브젝트의 Transform 참조
        GAMEOBJ_TYPE eType;
        _float3 vScale;
    } OBB_DESC;

private:
    CCollider_OBB(LPDIRECT3DDEVICE9 pGraphic_Device);
    CCollider_OBB(const CCollider_OBB& Prototype);
    virtual ~CCollider_OBB() = default;

public:
    virtual HRESULT Initialize_Prototype() override;
    virtual HRESULT Initialize(void* pArg) override;
    virtual HRESULT Render() override;

private:
    void Render_AttachedDebugBox(LPDIRECT3DDEVICE9 pDevice,
        const _float3& vCenter,
        const _float3& vRight,
        const _float3& vLook,
        const _float3& vScale,      // OBB 크기
        D3DCOLOR color = D3DCOLOR_ARGB(255, 255, 0, 0)); // 기본 빨간색;

public:
    virtual void Update_Collider();

public:
    const _float3& Get_Center() const { return m_vWorldCenter; }
    const _float3* Get_Axis()   const { return m_vAxis; }      // RIGHT, LOOK, UP
    const _float3& Get_Extents()const { return m_vWorldExtents; }
    const GAMEOBJ_TYPE Get_ObjType() { return m_eOwnerObjType; }
    

private:
    CTransform* m_pTransformRef = nullptr; 

    _float3 m_vWorldCenter = {};
    _float3 m_vWorldExtents = {};
    _float3 m_vAxis[3] = {};
    _float3 m_vScale = {};
    GAMEOBJ_TYPE m_eOwnerObjType = {};

public:
    static CCollider_OBB* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
    virtual CComponent* Clone(void* pArg) override;
    virtual void Free() override;
};

END
