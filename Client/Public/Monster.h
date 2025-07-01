#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CTexture;
class CTransform;
class CVIBuffer_Rect;
class CEvent_Handler;
class CLand;
class CAnimation;
class CStatus;
NS_END

NS_BEGIN(Client)

class CMonster final : public CGameObject
{
private:
	CMonster(LPDIRECT3DDEVICE9 pGraphic_Device);
	CMonster(const CMonster& Prototype);
	virtual ~CMonster() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void OnCollisionEnter(void* pArg, _float fTimeDelta)override;

private:
	CTexture*			m_pTextureCom = { nullptr };
	CTransform*			m_pTransformCom = { nullptr };	
	CTransform*			m_pTarget = { nullptr };
	CVIBuffer_Rect*		m_pVIBufferCom = { nullptr };
	CEvent_Handler*		m_pEvent_HandlerCom = { nullptr };
	CLand*				m_pLandCom = { nullptr };
	CStatus*			m_pStatusCom = { nullptr };

	CAnimation*	m_pAnimationCom = { nullptr };
	CAnimation*	m_pAnimationIdle_Front = { nullptr };
	CAnimation*	m_pAnimationIdle_Back = { nullptr };
	CAnimation*	m_pAnimationIdle_Left = { nullptr };
	CAnimation*	m_pAnimationIdle_Right = { nullptr };

	CAnimation* m_pAnimationChase_Front = { nullptr };
	CAnimation* m_pAnimationChase_Back = { nullptr };
	CAnimation* m_pAnimationChase_Left = { nullptr };
	CAnimation* m_pAnimationChase_Right = { nullptr };

	CAnimation* m_pAnimationAttack_Front = { nullptr };
	CAnimation* m_pAnimationAttack_Back = { nullptr };
	CAnimation* m_pAnimationAttack_Left = { nullptr };
	CAnimation* m_pAnimationAttack_Right = { nullptr };

	class CAttackRange_Cube* m_pMonsterAttackRange_Cube = { nullptr };
	class CAttackRange_Cube* m_pPlayerAttackRange_Cube = { nullptr };

private:
	void Change_Motion(MOTION eNextMotion);
	void Attacking(_float fTimeDelta);
	void Attack_Delay(_float fTimeDelta);
	void Chase(_float fTimeDelta);
	void Look();


private:
	CGameObject* m_pPlayer = { nullptr };
	CStatus*	m_pMonsterStatus = { nullptr };
	CStatus*	m_pPlayerStatus = { nullptr };

	MOTION		m_eCurrentMotion = MOTION::IDLE_BACK;
	MOTION		m_eNextMotion = {};
	DIRECTION	m_eCurDirection = DIRECTION::FORWARD;

	_float3		m_vMonsterPos = {};

	_float		m_fAttackTime = {};
	_float		m_fAttackCoolTime = 0.f;
	_float		m_fTime = 0.f;
	_float      m_fAttackAnimationTimer = 0.f;     
	_float		m_fAttackAnimationTotalTime = 0.f;

	

	_int		m_iMonsterMaxHP	= {};
	_int		m_iMonsterCurHP = {};
	_int		m_iMonsterDefense	= {};
	_int		m_iPlayerDamage = {};
	

	_bool		m_bGroundCheck = { false };
	_bool		m_bChasing = { false };
	_bool		m_bLook = { false };
	_bool		m_bChaseFinish = { false };
	_bool		m_bAttack = { false };
	_bool		m_bAttackReady = true;
	_bool       m_bAttackAnimation = false;

	map<class CAttackRange_Cube*, _float> m_Objmap;

private:
	HRESULT Ready_Components();
	HRESULT Begin_RenderState();
	HRESULT End_RenderState();



public:
	static CMonster* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END