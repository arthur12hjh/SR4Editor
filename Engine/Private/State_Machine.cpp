#include "State_Machine.h"
#include "Key_Manager.h"

CState_Machine::CState_Machine(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent{ pGraphic_Device }
{
}

CState_Machine::CState_Machine(const CState_Machine& Prototype)
	: CComponent { Prototype }
{
}

HRESULT CState_Machine::Initialize_Prototype()
{
	m_iStateIndex = 0;

	return S_OK;
}

HRESULT CState_Machine::Initialize(void* pArg)
{
	return S_OK;
}

void CState_Machine::Enter(_uint iStateIndex)
{
	
}

void CState_Machine::Exit(_uint iStateIndex)
{
}

void CState_Machine::KeyInput()
{
}

void CState_Machine::State_Waiting()
{
}

void CState_Machine::State_Find()
{
}

void CState_Machine::State_Move()
{
}

void CState_Machine::State_Attacking()
{
}

void CState_Machine::State_Rolling()
{
}

void CState_Machine::State_Jumping()
{
}

CState_Machine* CState_Machine::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CState_Machine* pInstance = new CState_Machine(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CState_Machine");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CState_Machine::Clone(void* pArg)
{
	CState_Machine* pInstance = new CState_Machine(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CState_Machine");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CState_Machine::Free()
{
	__super::Free();

}
