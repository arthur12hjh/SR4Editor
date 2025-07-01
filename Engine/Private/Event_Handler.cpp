#include "Event_Handler.h"
#include "Event_Manager.h"

CEvent_Handler::CEvent_Handler(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent{ pGraphic_Device }
{
}

CEvent_Handler::CEvent_Handler(const CEvent_Handler& Prototype)
	: CComponent{ Prototype }
{
}

HRESULT CEvent_Handler::Initialize_Prototype()
{
	return S_OK;
}


HRESULT CEvent_Handler::Initialize(void* pArg)
{
	return S_OK;
}


CEvent_Handler* CEvent_Handler::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CEvent_Handler* pInstance = new CEvent_Handler(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CEvent_Handler");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CEvent_Handler::Clone(void* pArg)
{
	CEvent_Handler* pInstance = new CEvent_Handler(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CEvent_Handler");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CEvent_Handler::Free()
{
	__super::Free();

}
