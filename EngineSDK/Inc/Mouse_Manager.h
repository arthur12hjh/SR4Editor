#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class CMouse_Manager final : public CBase
{
private:
	CMouse_Manager();
	virtual ~CMouse_Manager() = default;

public:
	HRESULT	Initialize(HWND hWNd);
	void Update(_float fTimeDelta);

	HRESULT LButton(_float fTimeDelta, class CGameObject* UI);
	HRESULT LButtonUp();
	HRESULT RButton();
	void Drag();

private:
	class CGameObject* m_UIObjects = {nullptr};
	class CGameObject* m_SelectUI = {nullptr};

	class CGameInstance*			m_pGameInstance;

	_bool		m_bclick{ false };
	HWND		m_hWnd;
	_float		time{};

public:
	static CMouse_Manager* Create(HWND hWNd);
	virtual void Free() override;

};

NS_END