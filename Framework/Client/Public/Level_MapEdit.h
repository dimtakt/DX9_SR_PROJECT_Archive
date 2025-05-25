#pragma once

#include "Level.h"
#include "Client_Defines.h"
#include <array>
BEGIN(Engine)
class CGameInstance;
class CGameObject;
class CTexture;
END

BEGIN(Client)
class CImgui_Manager;

class CLevel_MapEdit final : public CLevel
{
public:
	typedef struct EditorObjectInfo
	{
		string strObjectType;       // 오브젝트 타입 
		int iTextureCount = 0;      // 텍스처 개수
		CTexture* pTextureCom = nullptr; // 텍스처 컴포넌트 주소 저장용
	}OBJECT_TEXTURE_INFO;

private:
	explicit CLevel_MapEdit(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CLevel_MapEdit() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

public:
	HRESULT Ready_ImGui(HWND hWnd, LPDIRECT3DDEVICE9 pOut);
	HRESULT Ready_Layer_Camera(const _wstring& strLayerTag);
	HRESULT Ready_Texture_Info();

public:
	HRESULT Delete_Tile_By_Position(D3DXVECTOR3& vTargetPos);
private:
	CImgui_Manager* m_pImgui_Manage;
private:
	void Imgui_Render();
	void ImGui_MenuBar_Render();
	bool ImGui_TextureSelector_Render(string& strSelectedType, int& iTextureIndex);
	void ImGui_TextureId_Render();
	void ImGui_Transform_Render();
	void ImGui_Rotate_Render();
	void ImGui_Scale_Render();
private:
	int m_iTexture_id = {};
	D3DXVECTOR3 m_DeletePos = { 0.f, 0.f, 0.f };
	_float3 m_Scales = { 1.f,1.f,1.f };
	_float3 m_Rotates = { 0.f,0.f, 0.f };
	_float3 m_Translates = { 0.f,0.f,0.f };

	map<string, OBJECT_TEXTURE_INFO>	m_ObjectTextureInfo;
	CGameObject*					m_pPreview = nullptr;
	bool							m_bPreviewReady;

public:
	static CLevel_MapEdit* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};
END
