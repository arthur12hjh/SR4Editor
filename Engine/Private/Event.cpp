#include "Event.h"
#include "GameObject.h"

CEvent::CEvent()
{
}

HRESULT CEvent::Initialize(void* pArg)
{
	m_pArg = pArg;
	return S_OK;
}

void CEvent::Set_ActiveEvent(_bool bActiveType)
{
}

_bool CEvent::Get_ActiveEvent()
{
	return m_bActive;
}

CEvent* CEvent::Create(void* pArg)
{
	CEvent* pInstance = new CEvent();

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CEvent");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEvent::Free()
{
	__super::Free();
}



CEventListenerInfo::CEventListenerInfo()
{
}

HRESULT CEventListenerInfo::Initialize(void* pArg)
{
	if (nullptr == pArg)
		return E_FAIL;

	LISTENERINFO_DESC* pDesc = static_cast<LISTENERINFO_DESC*>(pArg);

	m_pListener = pDesc->pListener;
	m_func = pDesc->func;

	Safe_AddRef(m_pListener);

	return S_OK;
}

CEventListenerInfo* CEventListenerInfo::Create(void* pArg)
{
	CEventListenerInfo* pInstance = new CEventListenerInfo();

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Create CEventListenerInfo");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEventListenerInfo::Free()
{
	__super::Free();

	Safe_Release(m_pListener);
}
