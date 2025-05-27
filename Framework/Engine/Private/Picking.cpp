#include "Picking.h"

CPicking::CPicking(LPDIRECT3DDEVICE9 pGraphic_Device)
    : m_pGraphic_Device { pGraphic_Device }
{
    Safe_AddRef(m_pGraphic_Device);
}

HRESULT CPicking::Initialize(HWND hWnd, _uint iWinSizeX, _uint iWinSizeY)
{
    m_hWnd = hWnd;
    m_iWinSizeX = iWinSizeX;
    m_iWinSizeY = iWinSizeY;

    return S_OK;
}

void CPicking::Update()
{
    POINT ptMouse{};
    GetCursorPos(&ptMouse);
    ScreenToClient(m_hWnd, &ptMouse);

    // 1. 마우스 스크린 좌표를 NDC로 변환 (윈도우 사이즈를 -1 ~ 1 사잇값으로)
    _float4 vPosition{};
    vPosition.x = ptMouse.x / (m_iWinSizeX * 0.5f) - 1.f;
    vPosition.y = ptMouse.y / (m_iWinSizeY * -0.5f) + 1.f;
    vPosition.z = 0.f; // 1.f; // 전방..?
    vPosition.w = 1.f;

    // 2. 역투영 변환
    _float4x4 ProjMatrix{};
    m_pGraphic_Device->GetTransform(D3DTS_PROJECTION, &ProjMatrix);
    D3DXMatrixInverse(&ProjMatrix, nullptr, &ProjMatrix);

    D3DXVec4Transform(&vPosition, &vPosition, &ProjMatrix);

    if (vPosition.w != 0.f)
    {
        vPosition.x /= vPosition.w;
        vPosition.y /= vPosition.w;
        vPosition.z /= vPosition.w;
    }

    // 3. 뷰 공간 → 월드 공간
    _float4x4 ViewMatrix{};
    m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);

    _float4x4 InvViewMatrix{};
    D3DXMatrixInverse(&InvViewMatrix, nullptr, &ViewMatrix);

    D3DXVec3TransformNormal((_float3*)&vPosition, (_float3*)&vPosition, &InvViewMatrix);
    D3DXVec3Normalize((_float3*)&vPosition, (_float3*)&vPosition);

    m_vMouseRay = _float3(vPosition.x, vPosition.y, vPosition.z);

    // 카메라 위치 설정 (원점)
    m_vMousePos = _float3(InvViewMatrix._41, InvViewMatrix._42, InvViewMatrix._43);


}

_bool CPicking::Picking_InWorld(_float3& vPickedPos, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC)
{
    _float  fU, fV, fDist;

    _bool   isPicked = D3DXIntersectTri(&vPointA, &vPointB, &vPointC, &m_vMousePos, &m_vMouseRay, &fU, &fV, &fDist);

    vPickedPos = m_vMousePos + m_vMouseRay * fDist;

    return isPicked;
}

_bool CPicking::Picking_InLocal(_float3& vPickedPos, const _float3& vPointA, const _float3& vPointB, const _float3& vPointC)
{
    _float  fU, fV, fDist;

    _bool   isPicked = D3DXIntersectTri(&vPointA, &vPointB, &vPointC, &m_vLocalMousePos, &m_vLocalMouseRay, &fU, &fV, &fDist);

    vPickedPos = m_vLocalMousePos + m_vLocalMouseRay * fDist;

    return isPicked;
}

_bool CPicking::Get_IntersectAtX(_float targetX, _float3& vIntersectPos)
{
    if (fabsf(m_vMouseRay.x) < 1e-6f)                       // 마우스가 X와 평행하면 계산X
        return false;

    float t = (targetX - m_vMousePos.x) / m_vMouseRay.x;    // X값과 교차하는 데까지 거리 t 찾음

    if (((targetX - m_vMousePos.x) / m_vMouseRay.x) < 0.f)  // t가 음수면 레이의 반대 방향 (화면 뒤쪽)이므로 무시
        return false;

    _float3 vIntersect = m_vMousePos + m_vMouseRay * t;     // 레이 방정식: P = origin + direction * t

    vIntersectPos = _float3(targetX, vIntersect.y, vIntersect.z);

    return true;
}

_bool CPicking::Get_IntersectAtY(_float targetY, _float3& vIntersectPos)
{
    if (fabsf(m_vMouseRay.y) < 1e-6f)                       // 마우스가 Y와 평행하면 계산X
        return false;

    float t = (targetY - m_vMousePos.y) / m_vMouseRay.y;    // Y값과 교차하는 데까지 거리 t 찾음

    if (((targetY - m_vMousePos.y) / m_vMouseRay.y) < 0.f)  // t가 음수면 레이의 반대 방향 (화면 뒤쪽)이므로 무시
        return false;

    _float3 vIntersect = m_vMousePos + m_vMouseRay * t;     // 레이 방정식: P = origin + direction * t

    vIntersectPos = _float3(vIntersect.x, targetY, vIntersect.z);

    return true;
}

_bool CPicking::Get_IntersectAtZ(_float targetZ, _float3& vIntersectPos)
{
    if (fabsf(m_vMouseRay.z) < 1e-6f)                       // 마우스가 X와 평행하면 계산X
        return false;

    float t = (targetZ - m_vMousePos.z) / m_vMouseRay.z;    // X값과 교차하는 데까지 거리 t 찾음

    if (((targetZ - m_vMousePos.z) / m_vMouseRay.z) < 0.f)  // t가 음수면 레이의 반대 방향 (화면 뒤쪽)이므로 무시
        return false;

    _float3 vIntersect = m_vMousePos + m_vMouseRay * t;     // 레이 방정식: P = origin + direction * t

    vIntersectPos = _float3(vIntersect.x, vIntersect.y, targetZ);

    return true;
}


void CPicking::Transform_ToLocalSpace(const _float4x4& WorldMatrixInverse)
{
    D3DXVec3TransformCoord(&m_vLocalMousePos, &m_vMousePos, &WorldMatrixInverse);
    D3DXVec3TransformNormal(&m_vLocalMouseRay, &m_vMouseRay, &WorldMatrixInverse);
}

CPicking* CPicking::Create(LPDIRECT3DDEVICE9 pGraphic_Device, HWND hWnd, _uint iWinSizeX, _uint iWinSizeY)
{
    CPicking* pInstance = new CPicking(pGraphic_Device);

    if (FAILED(pInstance->Initialize(hWnd, iWinSizeX, iWinSizeY)))
    {
        MSG_BOX(TEXT("Failed to Created : CPicking"));
        Safe_Release(pInstance);
    }

    return pInstance;
}

void CPicking::Free()
{
    __super::Free();

    Safe_Release(m_pGraphic_Device);
}
