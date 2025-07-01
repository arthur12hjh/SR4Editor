#pragma once

#include "Client_Defines.h"
#include "GameObject.h"

NS_BEGIN(Engine)
class CTexture;
class CAnimation;
class CTransform;
class CVIBuffer_Rect;
class CLand;
class CStatus;
class CState_Machine;
class CLayer;
NS_END

NS_BEGIN(Client)

enum class PLAYER_ACTING
{
	IDLE = 0,
	WALK, RUN, JUMP, ATTACK, ROLLING, HURT, DIE, END
};

class CPlayer final : public CGameObject
{
private:
	CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device);
	CPlayer(const CPlayer& Prototype);
	virtual ~CPlayer() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	virtual void Priority_Update(_float fTimeDelta) override;
	virtual void Update(_float fTimeDelta) override;
	virtual void Late_Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

	virtual void OnCollisionEnter(void* pArg, _float fTimeDelta)override;

private:
	void Jump(_float fTimeDelta);
	void Change_Motion(MOTION eNextMotion, _float fTimeDelta);
	void MoveTo(_float fTimeDelta);
	void Attacking(_float fTimeDelta);
	void Rolling(_float fTimeDelta);
	void Run(_float fTimeDelta);

private:
	CAnimation* m_pAnimationCom = { nullptr };
	CAnimation* m_pAnimationIdle_Front = { nullptr };
	CAnimation* m_pAnimationIdle_Back = { nullptr };
	CAnimation* m_pAnimationIdle_Left = { nullptr };
	CAnimation* m_pAnimationIdle_Right = { nullptr };

	CAnimation* m_pAnimationJump_Front = { nullptr };
	CAnimation* m_pAnimationJump_Back = { nullptr };
	CAnimation* m_pAnimationJump_Left = { nullptr };
	CAnimation* m_pAnimationJump_Right = { nullptr };

	CAnimation* m_pAnimationWalk_Front = { nullptr };
	CAnimation* m_pAnimationWalk_Back = { nullptr };
	CAnimation* m_pAnimationWalk_Left = { nullptr };
	CAnimation* m_pAnimationWalk_Right = { nullptr };

	CAnimation* m_pAnimationRun_Front = { nullptr };
	CAnimation* m_pAnimationRun_Back = { nullptr };
	CAnimation* m_pAnimationRun_Left = { nullptr };
	CAnimation* m_pAnimationRun_Right = { nullptr };

	CAnimation* m_pAnimationAttack_Front = { nullptr };
	CAnimation* m_pAnimationAttack_Back = { nullptr };
	CAnimation* m_pAnimationAttack_Left = { nullptr };
	CAnimation* m_pAnimationAttack_Right = { nullptr };

	CAnimation* m_pAnimationRolling_Front = { nullptr };
	CAnimation* m_pAnimationRolling_Back = { nullptr };
	CAnimation* m_pAnimationRolling_Left = { nullptr };
	CAnimation* m_pAnimationRolling_Right = { nullptr };

	CTexture*			m_pTextureCom = { nullptr };
	CTransform*			m_pTransformCom = { nullptr };
	CTransform*			m_pAttackRangeCom = { nullptr };
	CVIBuffer_Rect*		m_pVIBufferCom = { nullptr };
	CLand*				m_pLandCom = { nullptr };
	CStatus*			m_pStatusCom = { nullptr };

	class CAttackRange_Cube* m_pPlayerAttackRange_Cube = { nullptr };

private:
	MOTION				m_eCurrentMotion = {};
	MOTION				m_eNextMotion = {};
	DIRECTION			m_eCurDirection = {};

	_float3				m_vPlayerPos = {};
	_float2				m_vOldMouse = {};
	_float				m_fSensor = { 0.2f };

	_float				m_fVelocity = {};
	_float				m_fJumpTime = {};
	_float				m_fJumpSpeed = {};

	_float				m_fAttackTime = 0.f;
	_float				m_fRollingTime = 0.f;
	_float				m_fTime = 0.f;

	_uint				m_eActing = {};
	_bool				m_isMoving = { false };

	_bool 				m_bJump = { false };
	_bool				m_bAttack = { false };
	_bool				m_bRolling = { false };
	_bool				m_bGroundCheck = { false };
	_bool				m_bRunning = { false };
	_bool				m_bJumpEffect = { false };

	
	
private:
	HRESULT Ready_Components();
	HRESULT Begin_RenderState();
	HRESULT End_RenderState();


public:
	static CPlayer* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END