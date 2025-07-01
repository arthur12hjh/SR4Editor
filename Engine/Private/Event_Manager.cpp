#include "Event_Manager.h"
#include "GameInstance.h"

#include "Event.h"
#include "Event_Handler.h"


CEvent_Manager::CEvent_Manager()
	: m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

HRESULT CEvent_Manager::Initialize()
{
	return S_OK;
}

HRESULT CEvent_Manager::AddEvent(const _wstring& strEventTag, void* pArg)
{
	CEvent* pEvent = Find_Event(strEventTag);
	if (nullptr == pEvent)
	{
		pEvent = CEvent::Create(pArg);

		m_mapEvent.emplace(strEventTag, pEvent);
	}

	return S_OK;

}

HRESULT CEvent_Manager::SubscribeEvent(const _wstring& strEventTag, CEventListenerInfo* refInfo)
{
	CEvent* pEvent = Find_Event(strEventTag);

	if (nullptr == pEvent)
	{
		AddEvent(strEventTag, nullptr);
	}

	if (CheckSubscribeEvent(strEventTag, refInfo) == true)
	{
		return E_FAIL;
	}

	m_multimapListener.emplace(strEventTag, refInfo);


	return S_OK;
}

HRESULT CEvent_Manager::UnSubscribeEvent(const _wstring& strEventTag, const CGameObject* listener)
{
	auto range = m_multimapListener.equal_range(strEventTag);

	for (auto iter = range.first; iter != range.second; ++iter)
	{
		if (iter->second->GetListener() == listener)
		{
			iter = m_multimapListener.erase(iter);
		}
		else
			++iter;
	}

	return S_OK;
}

HRESULT CEvent_Manager::RemoveEvent(const _wstring& strEventTag)
{
	auto range = m_multimapListener.equal_range(strEventTag);

	for (auto iter = range.first; iter != range.second; ++iter)
	{
		iter = m_multimapListener.erase(iter);

		++iter;
	}

	auto eventIter = m_mapEvent.find(strEventTag);
	if (eventIter != m_mapEvent.end())
		eventIter = m_mapEvent.erase(eventIter);

	return S_OK;
}

HRESULT CEvent_Manager::RemoveListener(const CGameObject* listener)
{
	for (auto iter = m_multimapListener.begin(); iter != m_multimapListener.end();)
	{
		if (iter->second->GetListener() == listener)
		{
			iter = m_multimapListener.erase(iter);
		}
		else
			++iter;
	}
	return S_OK;
}

HRESULT CEvent_Manager::RemoveSearchedListener(const _wstring& strEventTag, const CGameObject* listener)
{
	auto range = m_multimapListener.equal_range(strEventTag);

	for (auto iter = range.first; iter != range.second; ++iter)
	{
		if (iter->second->GetListener() == listener)
		{
			iter = m_multimapListener.erase(iter);
			return S_OK;
		}
	}
	return E_FAIL;
}


HRESULT CEvent_Manager::RemoveEventsAll()
{
	for (auto& Pair : m_mapEvent)
		Safe_Release(Pair.second);

	m_mapEvent.clear();
	return S_OK;
}

HRESULT CEvent_Manager::RemoveListenerAll()
{
	for (auto& Pair : m_multimapListener)
		Safe_Release(Pair.second);

	m_multimapListener.clear();
	return S_OK;
}

void CEvent_Manager::Set_ActiveEvent(const _wstring& strEventTag, _bool isActive)
{
	CEvent* pEvent = Find_Event(strEventTag);
	if (nullptr == pEvent)
	{
		MSG_BOX("Failed to Find Event. Check your input");
		Safe_Release(pEvent);
	}

	pEvent->Set_ActiveEvent(isActive);

}

bool CEvent_Manager::Get_ActiveEvent(const _wstring& strEventTag)
{
	CEvent* pEvent = Find_Event(strEventTag);
	if (nullptr == pEvent)
	{
		return false;
	}

	return pEvent->Get_ActiveEvent();
}


void CEvent_Manager::CallBackEvent(const _wstring& strEventTag)
{
	if (Get_ActiveEvent(strEventTag) == false)
	{
		return;
	}

	CEvent* pEvent = Find_Event(strEventTag);

	auto range = m_multimapListener.equal_range(strEventTag);

	for (auto iter = range.first; iter != range.second; ++iter)
	{
		iter->second->m_func(pEvent->Get_Argument());
	}
}


CEvent* CEvent_Manager::Find_Event(const _wstring& strEventTag)
{
	auto iter = m_mapEvent.find(strEventTag);
	if (iter == m_mapEvent.end())
		return nullptr;

	return iter->second;
}

_bool CEvent_Manager::CheckSubscribeEvent(const _wstring& strEventTag, const class CEventListenerInfo* refInfo)
{
	auto range = m_multimapListener.equal_range(strEventTag);

	for (auto iter = range.first; iter != range.second; ++iter)
	{
		if (iter->second == refInfo)
		{
			iter = m_multimapListener.erase(iter);
			return true;
		}
	}
	return false;
}

CEvent_Manager* CEvent_Manager::Create()
{
	CEvent_Manager* pInstance = new CEvent_Manager();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Create : CEvent_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEvent_Manager::Free()
{
	RemoveEventsAll();
	RemoveListenerAll();

	__super::Free();

	Safe_Release(m_pGameInstance);
}
