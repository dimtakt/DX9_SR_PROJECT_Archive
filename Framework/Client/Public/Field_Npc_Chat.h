#pragma once
#include "Client_Defines.h"
#include "UIObject.h"
#include "Engine_Defines.h"
#include "Client_Defines_Item.h"

BEGIN(Engine)
class CVIBuffer_Rect;
class CTexture;
END

BEGIN(Client)
class CField_Npc_Chat final : public CUIObject
{
public:
	typedef struct tagFiled_Chat : public UIOBJECT_DESC
	{
		CTransform*		pTransform;
		_wstring		szChatTag;
	}FIELD_CHAT_DESC;
private:
								CField_Npc_Chat(LPDIRECT3DDEVICE9 pGraphic_Device);
								CField_Npc_Chat(const CField_Npc_Chat& Prototype);
	virtual						~CField_Npc_Chat() = default;

public:
	void						Add_Chat(_wstring szChatting);
	
	void						On_Chat(_int iFaceNum, _bool bIsFace);
	void						Off_Chat();

	//일반 대화
	void						StartToEnd_Chat();
	//일반 대화 분리
	void						Start_Chat();
	void						Next_Chat();
	void						End_Chat();
	//시네마틱 대화
	void						Cinematic_Chat(_int iFaceNum, _bool bIsFace);	//0번 갑옷 강아지 1번 바바(소)
public:
	virtual HRESULT				Initialize_Prototype(LEVEL eLevel);
	virtual HRESULT				Initialize(void* pArg) override;
	virtual void				Priority_Update(_float fTimeDelta) override;
	virtual void				Update(_float fTimeDelta) override;
	virtual void				Late_Update(_float fTimeDelta);
	virtual HRESULT				Render() override;

private:
	CVIBuffer_Rect*				m_pVIBufferCom = { nullptr };
	CTexture*					m_pTextureCom = { nullptr };

	LEVEL						m_eLevel = {};
	CTransform*					m_pTarget_Transform = { nullptr };

	_bool						m_bIsOn = { false };
	_bool						m_bIschat = { false };
	_bool						m_bIsRenderchat = { false };
	_bool						m_bIsFinish = { false };

	vector<_wstring>			m_vecChatting;
	_int						m_iVecIndex = {};			//벡터안에 텍스트 개수
	
	
	_wstring					m_szAllText = {};
	_wstring					m_szRenderText = {};

	_uint						m_iChatIndex = {};			//배열 인덱스
	_uint						m_iTextIndex = {};			//텍스트 인덱스

	_float						m_fDeleyTime = {};
	_float						m_fAccTime = {};

	_int						m_iWinPosX = {};
	_int						m_iWinPosY = {};

private:
	HRESULT						Ready_Components();

	HRESULT						Ready_ChildPrototype(LEVEL eLevel);
	HRESULT						Ready_Children();

	void						Target_Pos();
	void						Render_Font();
	void						On_Chat_Font();
public:
	static CField_Npc_Chat* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevel);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void				Free() override;
};
END

