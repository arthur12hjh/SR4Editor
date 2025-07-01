#pragma once

#include "Component.h"

NS_BEGIN(Engine);

class ENGINE_DLL CStatus : public CComponent
{
private:
	CStatus(LPDIRECT3DDEVICE9 pGraphic_Device);
	CStatus(const CStatus& Prototype);
	virtual ~CStatus() = default;

public:
	virtual HRESULT Initialize_Prototype(_int fMaxHp, _int fDamage, _float fSpeed, _float fAttackSpeed, _int iDefense = 0);
	virtual HRESULT Initialize(void* pArg) override;

public:
	_int Get_MaxHp() { return m_iMaxHp; }
	_int Get_Hp() { return m_iCurrentHp; }
	_int Get_Damage() { return m_iDamage; }
	_int Get_Defense() { return m_iDefense; }
	_uint Get_Phase() { return m_iPhase; }

private:
	_int	m_iMaxHp;
	_int	m_iCurrentHp;
	_int	m_iDamage;
	_int	m_iDefense;

	_float	m_fSpeed;
	_float	m_fAttackSpeed; // 일반 공속, 이건 상태 머신가아하나

	_uint	m_iPhase = {}; // 보스 페이즈
	_uint	m_iSkillDamage = {}; // 보스 스킬데미지 (현재 체력 퍼뎀)
	

public:
	static CStatus* Create(LPDIRECT3DDEVICE9 pGraphic_Device, _int iMaxHp, _int iDamage, _float fSpeed, _float fAttackSpeed, _int iDefense = 0);
	virtual CComponent* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END

