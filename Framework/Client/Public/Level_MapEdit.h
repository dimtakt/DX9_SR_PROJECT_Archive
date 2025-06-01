#pragma once

#include "Level.h"
#include "Client_Defines.h"
#include <array>
#include "Client_Struct.h"

BEGIN(Engine)
class CGameInstance;
class CGameObject;
class CTexture;
class CTransform;
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
	HRESULT Ready_Terrain_Texture_Info();

private:
	CImgui_Manager* m_pImgui_Manage = { nullptr };
private:
	void Imgui_Render();
	void ImGui_MenuBar_Render();
	void Picking_Check();

	//오브젝트 전용 메뉴
	void ImGui_Object_MenBar();
	void ImGui_Object_Texture_Redner(int iTextureIndex);
	void ImGui_Option_Button_Reset();
	void ImGui_Transform_Render(); 
	void ImGui_Rotate_Render();
	void ImGui_Scale_Render();

	//선택된 오브젝트 전용
	void ImGui_Picking_Object_MenBar();
	void ImGui_Delete_Object();
	void ImGui_Picking_Object_Translates_Option();
	void ImGui_Picking_Object_Rotate();
	void ImGui_Picking_Object_Scale();
	void ImGui_Picking_UnCheck();
	
	//지형 전용
	void ImGui_Terrain_MenBar();
	void ImGui_Terrain_Transform_Render();
	void ImGui_Terrain_Texture_Render(int iTextureIndex);
	void ImGui_Terrain_Scale_Render();

private:
	int m_iTexture_id = {};							//공용 텍스처 인덱스

	_float3 m_Scales = { 1.f,1.f,1.f };				//오브젝트 전용
	_float3 m_Rotates = { 0.f,0.f, 0.f };
	_float3 m_Translates = { 0.f,0.f,0.f };

	_float3 m_TrrainTranslate = { 0.f, 0.f, 0.f };	//지형 전용
	_float3 m_TrrainScales = { 20.f, 2.f, 20.f };

	map<string, OBJECT_TEXTURE_INFO>			m_ObjectTextureInfo = {};			//이미지뷰 띄우기, string "Object", "TerrainBox"
	CGameObject*								m_pPreview = nullptr;				//텍스처 가져오고자 만든 임시변수
	CTransform*									m_pObjectTransform = nullptr;		// 하나의 오브젝트 선택되면 트랜스폼 설정하고자 함
	CGameObject*								m_pPickingObject = nullptr;			// 피킹된 얘 저장용
	vector<CGameObject*>						m_pObject = {};						// 만든 얘들 주소 저장
	list<MAP_OBJECT_DESC>						m_pObject_Desc = {};				// 저장할 때 쓸 구조체에 값 넣어줌
	_bool									m_bPicking = false;						// 피킹 된 객체 있으면 활성화

public:
	static CLevel_MapEdit* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual void Free() override;
};
END
