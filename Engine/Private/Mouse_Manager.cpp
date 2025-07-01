#include "Mouse_Manager.h"

#include "GameInstance.h"
#include "GameObject.h"
#include "UIObject.h"
#include "Component.h"

CMouse_Manager::CMouse_Manager()
	: m_pGameInstance{ CGameInstance::GetInstance() }
{
}

HRESULT CMouse_Manager::Initialize(HWND hWNd)
{
	m_hWnd = hWNd;
	return S_OK;
}

void CMouse_Manager::Update(_float fTimeDelta)
{
	m_UIObjects = m_pGameInstance->Find_MaxZBuffer();

	if (m_pGameInstance->Key_Pressing(VK_LBUTTON))
	{
		m_SelectUI = m_UIObjects;
		LButton(fTimeDelta, m_SelectUI);
	}
	if (m_pGameInstance->Key_Released(VK_LBUTTON))
	{
		LButtonUp();
	}
}

HRESULT CMouse_Manager::LButton(_float fTimeDelta, CGameObject* UI)
{
	if (nullptr == m_pGameInstance->Find_MaxZBuffer())
		return E_FAIL;

	else
	{
		m_SelectUI = UI;

		if (!m_bclick)
		{
			static_cast<CUIObject*>(m_SelectUI)->Set_ZBuffer(10);
			m_bclick = true;
		}
		
		
		time += fTimeDelta;

		if (0.5f < time)
		{
			POINT ptMouse;
			GetCursorPos(&ptMouse);
			ScreenToClient(m_hWnd, &ptMouse);

			static_cast<CUIObject*>(m_SelectUI)->Set_Move(ptMouse.x, ptMouse.y);
		}
	}

	return S_OK;
}

HRESULT CMouse_Manager::LButtonUp()
{
	if (nullptr == m_UIObjects)
		return E_FAIL;

	time = 0.f;
	m_bclick = false;
	m_SelectUI = nullptr;
	return S_OK;
}

HRESULT CMouse_Manager::RButton()
{
	return S_OK;
}

void CMouse_Manager::Drag()
{

}

CMouse_Manager* CMouse_Manager::Create(HWND hWNd)
{
	CMouse_Manager* pInstance = new CMouse_Manager();

	if (FAILED(pInstance->Initialize(hWNd)))
	{
		MSG_BOX(" Failed to Create : CMouse_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMouse_Manager::Free()
{
	__super::Free();

	Safe_Release(m_UIObjects);
	Safe_Release(m_SelectUI);

}
