#pragma once

#include "Component.h"
#include "Event.h"

NS_BEGIN(Engine)

class ENGINE_DLL CEvent_Handler final : public CComponent
{
private:
	CEvent_Handler(LPDIRECT3DDEVICE9 pGraphic_Device);
	CEvent_Handler(const CEvent_Handler& Prototype);
	virtual ~CEvent_Handler() = default;

public:
	template<typename T>
	void SetEventListener(T* pObj, void(T::* func)(void*))
	{

		CEventListenerInfo::LISTENERINFO_DESC ListenerDesc = { pObj, [pObj, func](void* pArg) {(static_cast<T*>(pObj)->*func)(pArg); } };
		CEventListenerInfo* pInfo = CEventListenerInfo::Create(&ListenerDesc);
		m_pInfo = pInfo;

	}


	CEventListenerInfo* GetEventListener()
	{
		return m_pInfo;
	}

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;

private:
	CEventListenerInfo* m_pInfo = { nullptr };

public:
	static CEvent_Handler* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CComponent* Clone(void* pArg);
	virtual void Free();
};

NS_END

