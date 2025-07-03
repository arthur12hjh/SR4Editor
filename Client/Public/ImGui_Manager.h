/* ImGui_Manager.h */
#pragma once

#include "Client_Defines.h"
#include "Base.h"

NS_BEGIN(Engine)
class CGameInstance;
NS_END

NS_BEGIN(Client)

class CImGui_Manager final : public CBase
{
public:
	DECLARE_SINGLETON(CImGui_Manager);

private:
	CImGui_Manager();
	virtual ~CImGui_Manager() = default;

public:
	HRESULT Initialize(LPDIRECT3DDEVICE9 pGraphic_Device, HWND hWnd);
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);
	HRESULT Render();

private:
	void ShowGUI();
	void LoadFont();

private:
	LPDIRECT3DDEVICE9			m_pGraphic_Device = { nullptr };
	HWND						m_hWnd = {};

public:
	virtual void Free();

};

NS_END