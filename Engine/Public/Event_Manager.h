#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class CEvent_Manager final : public CBase
{
private:
	CEvent_Manager();
	virtual ~CEvent_Manager() = default;

public:
	HRESULT Initialize();

public:
	HRESULT AddEvent(const _wstring& strEventTag, void* pArg);
	HRESULT SubscribeEvent(const _wstring& strEventTag, class CEventListenerInfo* refInfo);
	HRESULT UnSubscribeEvent(const _wstring& strEventTag, const class CGameObject* listener);

	HRESULT RemoveEvent(const _wstring& strEventTag);
	HRESULT RemoveListener(const class CGameObject* listener);
	HRESULT RemoveSearchedListener(const _wstring& strEventTag, const class CGameObject* listener);

	HRESULT RemoveEventsAll();
	HRESULT RemoveListenerAll();

	void Set_ActiveEvent(const _wstring& strEventTag, _bool isActive);
	bool Get_ActiveEvent(const _wstring& strEventTag);

	void CallBackEvent(const _wstring& strEventTag);

private:
	class CEvent* Find_Event(const _wstring& strEventTag);
	_bool CheckSubscribeEvent(const _wstring& strEventTag, const class CEventListenerInfo* refInfo);


private:
	map<const _wstring, class CEvent*> m_mapEvent;
	multimap<const _wstring, class CEventListenerInfo*> m_multimapListener;
	class CGameInstance* m_pGameInstance = { nullptr };

public:
	static CEvent_Manager* Create();
	virtual void Free() override;
};

NS_END