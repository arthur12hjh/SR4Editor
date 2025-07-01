#include "Status.h"

CStatus::CStatus(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent { pGraphic_Device }
{
}

CStatus::CStatus(const CStatus& Prototype)
	: CComponent { Prototype }
	, m_iMaxHp{ Prototype.m_iMaxHp }
	,m_iCurrentHp {Prototype.m_iCurrentHp}
	,m_iDamage {Prototype.m_iDamage}
	,m_fSpeed {Prototype.m_fSpeed}
	,m_fAttackSpeed {Prototype.m_fAttackSpeed}
	,m_iDefense {Prototype.m_iDefense}
	,m_iPhase {Prototype.m_iPhase}
	,m_iSkillDamage {Prototype.m_iSkillDamage}
{
}

HRESULT CStatus::Initialize_Prototype(_int iMaxHp, _int iDamage, _float fSpeed, _float fAttackSpeed, _int iDefense)
{
	m_iMaxHp = iMaxHp;
	m_iDamage = iDamage;
	m_fSpeed = fSpeed;
	m_fAttackSpeed = fAttackSpeed;
	m_iDefense = iDefense;

	m_iPhase = 1;
	m_iSkillDamage = 0;


	return S_OK;
}

HRESULT CStatus::Initialize(void* pArg)
{
	return S_OK;
}


CStatus* CStatus::Create(LPDIRECT3DDEVICE9 pGraphic_Device, _int iMaxHp, _int iDamage, _float fSpeed, _float fAttackSpeed, _int iDefense)
{
	CStatus* pInstance = new CStatus(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype(iMaxHp, iDamage, fSpeed, fAttackSpeed, iDefense)))
	{
		MSG_BOX("Failed to Created : CStatus");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CStatus::Clone(void* pArg)
{
	CStatus* pInstance = new CStatus(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CStatus");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CStatus::Free()
{
	__super::Free();

}
