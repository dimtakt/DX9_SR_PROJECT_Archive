#pragma once

#include "Collider.h"
#include "Transform.h"

BEGIN(Engine)

class ENGINE_DLL CCollider_OBB final : public CCollider
{
public:
    typedef struct tagOBB_Desc {
        CGameObject* pOwner;
        const CTransform* pTransform; // 게임오브젝트의 Transform 참조
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

public:
    virtual void Update_Collider();

public:
    void Get_MatrixData(_float3& vCenter, _float3& vExtent, _float3* vAxis);
   

private:
    const CTransform* m_pTransformRef = nullptr; 

    _float3 m_vWorldCenter = {};
    _float3 m_vWorldExtents = {};
    _float3 m_vAxis[3] = {};
    _float3 m_vScale = {};

public:
    static CCollider_OBB* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
    virtual CComponent* Clone(void* pArg) override;
    virtual void Free() override;
};

END
