#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class CImGui_Manager final : public CBase
{
private:
	CImGui_Manager(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual ~CImGui_Manager() = default;

public:
	HRESULT Initialize(HWND hWnd);
	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);
	HRESULT Render();

private:
	class CGameInstance*		m_pGameInstance = {nullptr};
	LPDIRECT3DDEVICE9			m_pGraphic_Device = { nullptr };
	HWND						m_hWnd = {};

public:
	static CImGui_Manager* Create(LPDIRECT3DDEVICE9 pGraphic_Device, HWND hWnd);
	virtual void Free();

};

NS_END