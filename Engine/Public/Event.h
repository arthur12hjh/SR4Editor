#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class ENGINE_DLL CEvent final : public CBase
{
private:
	CEvent();
	virtual ~CEvent() = default;

public:
	HRESULT Initialize(void* pArg);

	void Set_ActiveEvent(_bool bActiveType);
	_bool Get_ActiveEvent();

	void* Get_Argument() { return m_pArg; }
	void Set_Argumet(void* pArg) { m_pArg = pArg; }

protected:
	_bool					m_bActive = { true };
	void* m_pArg = { nullptr };


public:
	static CEvent* Create(void* pArg);
	virtual void Free() override;
};

class ENGINE_DLL CEventListenerInfo final : public CBase
{
public:
	typedef struct ListenerInfoDesc
	{
		class CGameObject* pListener;
		std::function<void(void*)> func;
	}LISTENERINFO_DESC;
private:
	CEventListenerInfo();
	virtual ~CEventListenerInfo() = default;

public:
	HRESULT Initialize(void* pArg);
	const class CGameObject* GetListener() { return m_pListener; }
	std::function<void(void*)> m_func = { nullptr };

private:
	class CGameObject* m_pListener = { nullptr };

public:
	static CEventListenerInfo* Create(void* pArg);
	virtual void Free()override;

};

NS_END