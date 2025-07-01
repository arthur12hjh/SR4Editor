#include "Player.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "ParticleSystem.h"
#include "AttackRange_Cube.h"

CPlayer::CPlayer(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{

}

CPlayer::CPlayer(const CPlayer& Prototype)
	: CGameObject{ Prototype }
{

}

HRESULT CPlayer::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CPlayer::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	m_fJumpSpeed = 0.f;
	m_fJumpTime = 0.f;
	m_fVelocity = 10.f;
	m_bGroundCheck = true;

	m_eCurrentMotion = MOTION::IDLE_BACK;
	m_eCurDirection = DIRECTION::FORWARD;

	m_pLandCom->Set_Transform(m_pTransformCom);
	m_pLandCom->Set_TerrainTransform(dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_BackGround"), TEXT("Com_Transform"))));
	m_pLandCom->Set_TerrainVIBuffer(dynamic_cast<CVIBuffer*>(m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_BackGround"), TEXT("Com_VIBuffer"))));

	m_pTransformCom->Set_State(STATE::POSITION, _float3(10.f, 0.f, 10.f));

	m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_AttackRange_Cube"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Player_AttackRange_Cube"), m_pTransformCom);
	m_pPlayerAttackRange_Cube = static_cast<CAttackRange_Cube*>(m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Player_AttackRange_Cube")));

	POINT		ptMouse{};

	GetCursorPos(&ptMouse);

	ScreenToClient(g_hWnd, &ptMouse);

	m_vOldMouse = _float2(ptMouse.x, ptMouse.y);


	return S_OK;
}

void CPlayer::Priority_Update(_float fTimeDelta)
{

}

void CPlayer::Update(_float fTimeDelta)
{
	POINT		ptMouse{};

	GetCursorPos(&ptMouse);

	ScreenToClient(g_hWnd, &ptMouse);

	_float			fMove = {};


	if (fMove = ptMouse.x - m_vOldMouse.x)
	{
		m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta * fMove * m_fSensor);
	}

	if (fMove = ptMouse.y - m_vOldMouse.y)
	{
		m_pTransformCom->Turn(m_pTransformCom->Get_State(STATE::RIGHT), fTimeDelta * fMove * m_fSensor);
	}
	if (m_pGameInstance->Key_Pressing('V') && m_bGroundCheck && !m_bAttack)
	{
		m_bJump = true;
		m_bGroundCheck = false;
		m_bJumpEffect = true;
		m_fJumpTime = 0.f;

		switch (m_eCurDirection)
		{
		case DIRECTION::FORWARD:
			Change_Motion(MOTION::JUMP_BACK, fTimeDelta);
			break;
		case DIRECTION::BACKWARD:
			Change_Motion(MOTION::JUMP_FRONT, fTimeDelta);
			break;
		case DIRECTION::LEFT:
			Change_Motion(MOTION::JUMP_LEFT, fTimeDelta);
			break;
		case DIRECTION::RIGHT:
			Change_Motion(MOTION::JUMP_RIGHT, fTimeDelta);
			break;
		default:
			Change_Motion(MOTION::JUMP_BACK, fTimeDelta);
			break;
		}
	}
	else if (m_pGameInstance->Key_Down(VK_LBUTTON))
	{
		m_bAttack = true;

	}
	else if (!m_bAttack && (m_pGameInstance->Key_Pressing('W') || m_pGameInstance->Key_Pressing('S') ||
		m_pGameInstance->Key_Pressing('A') || m_pGameInstance->Key_Pressing('D')))
	{
		if (m_pGameInstance->Key_Pressing(VK_SPACE))
		{
			m_bRolling = true;
		}

		if (m_pGameInstance->Key_Pressing(VK_LSHIFT))
		{
			m_bRunning = true;
		}
		else
			m_bRunning = false;

		if (m_pGameInstance->Key_Pressing('W'))
		{
			m_eCurDirection = DIRECTION::FORWARD;
			if (m_bRunning)
			{
				Change_Motion(MOTION::RUN_BACK, fTimeDelta);
				m_pTransformCom->Go_Straight(fTimeDelta * 1.3f);
				m_isMoving = true;
			}
			else
			{
				Change_Motion(MOTION::WALK_BACK, fTimeDelta);
				m_pTransformCom->Go_Straight(fTimeDelta);
				m_isMoving = true;
			}
		}

		if (m_pGameInstance->Key_Pressing('S'))
		{
			m_eCurDirection = DIRECTION::BACKWARD;
			if (m_bRunning)
			{
				Change_Motion(MOTION::RUN_FRONT, fTimeDelta);
				m_pTransformCom->Go_Backward(fTimeDelta * 1.3f);
				m_isMoving = true;
			}
			else
			{
				Change_Motion(MOTION::WALK_FRONT, fTimeDelta);
				m_pTransformCom->Go_Backward(fTimeDelta);
				m_isMoving = true;
			}
		}

		if (m_pGameInstance->Key_Pressing('A'))
		{
			m_eCurDirection = DIRECTION::LEFT;
			if (m_bRunning)
			{
				Change_Motion(MOTION::RUN_LEFT, fTimeDelta);
				m_pTransformCom->Go_Left(fTimeDelta * 1.3f);
				m_isMoving = true;
			}
			else
			{
				Change_Motion(MOTION::WALK_LEFT, fTimeDelta);
				m_pTransformCom->Go_Left(fTimeDelta);
				m_isMoving = true;
			}
		}
		if (m_pGameInstance->Key_Pressing('D'))
		{
			m_eCurDirection = DIRECTION::RIGHT;
			if (m_bRunning)
			{
				Change_Motion(MOTION::RUN_RIGHT, fTimeDelta);
				m_pTransformCom->Go_Right(fTimeDelta * 1.3f);
				m_isMoving = true;
			}
			else
			{
				Change_Motion(MOTION::WALK_RIGHT, fTimeDelta);
				m_pTransformCom->Go_Right(fTimeDelta);
				m_isMoving = true;
			}
		}
	}

	else if (m_pGameInstance->Key_Down(VK_SPACE))
	{
		m_bRolling = true;
		m_bAttack = false;
	}
	else
	{
		if (!m_bJump)
		{
			switch (m_eCurDirection)
			{
			case DIRECTION::FORWARD:
				Change_Motion(MOTION::IDLE_BACK, fTimeDelta);
				break;
			case DIRECTION::BACKWARD:
				Change_Motion(MOTION::IDLE_FRONT, fTimeDelta);
				break;
			case DIRECTION::LEFT:
				Change_Motion(MOTION::IDLE_LEFT, fTimeDelta);
				break;
			case DIRECTION::RIGHT:
				Change_Motion(MOTION::IDLE_RIGHT, fTimeDelta);
				break;
			default:
				Change_Motion(MOTION::IDLE_BACK, fTimeDelta);
				break;
			}
		}

	}

	//Run();

	_float3 vStandingPos = m_pTransformCom->Get_State(STATE::POSITION);
	vStandingPos.y = m_pLandCom->Standing_On_Terrain(0.5f);

	if (m_bJump)
	{
		Jump(fTimeDelta);

		
		
	}
	else
	{
		m_vPlayerPos = m_pTransformCom->Get_State(STATE::POSITION);

		if (m_pLandCom)
		{
			m_vPlayerPos.y = vStandingPos.y;
			m_bGroundCheck = true;
			if (m_bJumpEffect == true)
			{
				m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_Ogu_Dash_Smoke_Effect"),
					ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Effect"), m_pTransformCom);
				m_bJumpEffect = false;
			}
		}
	}

	m_pTransformCom->Set_State(STATE::POSITION, m_vPlayerPos);

	m_pAnimationCom->Update_Animation(fTimeDelta);

	m_vOldMouse = _float2(ptMouse.x, ptMouse.y);

	if (m_bAttack)
	{
		Attacking(fTimeDelta);
	}

	if (m_bRolling)
	{
		Rolling(fTimeDelta);
	}
		

}

void CPlayer::Late_Update(_float fTimeDelta)
{
	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CPlayer::Render()
{
	_float3 fDefaultScale = m_pTransformCom->Get_Scaled();
	if (ENUM_CLASS(m_eCurrentMotion) >= ENUM_CLASS(MOTION::ATTACK_BACK) && ENUM_CLASS(m_eCurrentMotion) <= ENUM_CLASS(MOTION::ATTACK_RIGHT))
	{
		_float3 fAttackScale = m_pTransformCom->Get_Scaled();
		fAttackScale.x *= 2.f;
		fAttackScale.y *= 2.f;
		fAttackScale.z *= 2.f;

		m_pTransformCom->Set_Scale(fAttackScale);
	}


	m_pTransformCom->Set_Transform();

	if (m_pAnimationCom)
	{
		m_pAnimationCom->Set_Texture(m_pAnimationCom->Get_MotionIndex());
	}

	if (FAILED(Begin_RenderState()))
		return E_FAIL;

	m_pVIBufferCom->Render();

	if (FAILED(End_RenderState()))
		return E_FAIL;

	if (ENUM_CLASS(m_eCurrentMotion) >= ENUM_CLASS(MOTION::ATTACK_BACK) && ENUM_CLASS(m_eCurrentMotion) <= ENUM_CLASS(MOTION::ATTACK_RIGHT))
	{
		m_pTransformCom->Set_Scale(fDefaultScale);
	}

	return S_OK;
}

void CPlayer::OnCollisionEnter(void* pArg, _float fTimeDelta)
{
}

void CPlayer::Jump(_float fTimeDelta)
{
	m_fJumpTime += fTimeDelta * 3.f;

	m_vPlayerPos = m_pTransformCom->Get_State(STATE::POSITION);

	m_fJumpSpeed = m_fVelocity - (9.8f * m_fJumpTime);

	m_vPlayerPos.y += m_fJumpSpeed * fTimeDelta;

	m_pTransformCom->Set_State(STATE::POSITION, m_vPlayerPos);


	if (m_pLandCom)
	{
		_float3 vStandingPos = m_pTransformCom->Get_State(STATE::POSITION);
		vStandingPos.y = m_pLandCom->Standing_On_Terrain(0.5f);

		if (m_vPlayerPos.y <= vStandingPos.y)
		{
			m_vPlayerPos.y = vStandingPos.y;
			m_bJump = false;
			m_fJumpTime = 0.f;
			m_fJumpSpeed = 0.f;
			m_bGroundCheck = true;

			switch (m_eCurDirection)
			{
			case DIRECTION::FORWARD:
				Change_Motion(MOTION::IDLE_BACK, fTimeDelta);
				break;
			case DIRECTION::BACKWARD:
				Change_Motion(MOTION::IDLE_FRONT, fTimeDelta);
				break;
			case DIRECTION::LEFT:
				Change_Motion(MOTION::IDLE_LEFT, fTimeDelta);
				break;
			case DIRECTION::RIGHT:
				Change_Motion(MOTION::IDLE_RIGHT, fTimeDelta);
				break;
			default:
				Change_Motion(MOTION::IDLE_BACK, fTimeDelta);
				break;
			}
		}
	}


}

void CPlayer::Change_Motion(MOTION eNextMotion, _float fTimeDelta)
{
	if (m_eCurrentMotion == eNextMotion) return;

	if (m_eCurrentMotion == MOTION::JUMP_BACK || m_eCurrentMotion == MOTION::JUMP_FRONT
		|| m_eCurrentMotion == MOTION::JUMP_LEFT || m_eCurrentMotion == MOTION::JUMP_RIGHT)
	{
		if (m_bJump)
		{	
			return;
		}
	}

	if (m_eCurrentMotion == MOTION::ATTACK_BACK || m_eCurrentMotion == MOTION::ATTACK_FRONT
		|| m_eCurrentMotion == MOTION::ATTACK_LEFT || m_eCurrentMotion == MOTION::ATTACK_RIGHT)
	{
		if (m_bAttack)
		{
			if (m_pGameInstance->Key_Down(VK_SPACE))
			{
				m_bRolling = true;

				Rolling(fTimeDelta);
			}
			return;
		}
	}

	if (m_eCurrentMotion == MOTION::ROLLING_BACK || m_eCurrentMotion == MOTION::ROLLING_FRONT
		|| m_eCurrentMotion == MOTION::ROLLING_LEFT || m_eCurrentMotion == MOTION::ROLLING_RIGHT)
	{
		if (m_bRolling)
		{
			return;
		}
	}

	if (m_eCurDirection == DIRECTION::FORWARD)
	{
		switch (eNextMotion)
		{
		case MOTION::IDLE_BACK:
			m_pAnimationCom = m_pAnimationIdle_Back;
			break;
		case MOTION::WALK_BACK:
			m_pAnimationCom = m_pAnimationWalk_Back;
			break;
		case MOTION::JUMP_BACK:
			m_pAnimationCom = m_pAnimationJump_Back;
			m_pAnimationCom->Reset();
			break;
		case MOTION::ATTACK_BACK:
			m_pAnimationCom = m_pAnimationAttack_Back;
			m_pAnimationCom->Reset();
			break;
		case MOTION::ROLLING_BACK:
			m_pAnimationCom = m_pAnimationRolling_Back;
			m_pAnimationCom->Reset();
			break;
		case MOTION::RUN_BACK:
			m_pAnimationCom = m_pAnimationRun_Back;
			break;
		default:
			m_pAnimationCom = m_pAnimationIdle_Back;
			break;
		}
	}

	if (m_eCurDirection == DIRECTION::BACKWARD)
	{
		switch (eNextMotion)
		{
		case MOTION::IDLE_FRONT:
			m_pAnimationCom = m_pAnimationIdle_Front;
			break;
		case MOTION::WALK_FRONT:
			m_pAnimationCom = m_pAnimationWalk_Front;
			break;
		case MOTION::JUMP_FRONT:
			m_pAnimationCom = m_pAnimationJump_Front;
			m_pAnimationCom->Reset();
			break;
		case MOTION::ATTACK_BACK:
			m_pAnimationCom = m_pAnimationAttack_Back;
			m_pAnimationCom->Reset();
			break;
		case MOTION::ROLLING_FRONT:
			m_pAnimationCom = m_pAnimationRolling_Front;
			m_pAnimationCom->Reset();
			break;
		case MOTION::RUN_FRONT:
			m_pAnimationCom = m_pAnimationRun_Front;
			break;
		default:
			m_pAnimationCom = m_pAnimationIdle_Front;
			break;
		}
	}

	if (m_eCurDirection == DIRECTION::LEFT)
	{
		switch (eNextMotion)
		{
		case MOTION::IDLE_LEFT:
			m_pAnimationCom = m_pAnimationIdle_Left;
			break;
		case MOTION::WALK_LEFT:
			m_pAnimationCom = m_pAnimationWalk_Left;
			break;
		case MOTION::JUMP_LEFT:
			m_pAnimationCom = m_pAnimationJump_Left;
			m_pAnimationCom->Reset();
			break;
		case MOTION::ATTACK_BACK:
			m_pAnimationCom = m_pAnimationAttack_Back;
			m_pAnimationCom->Reset();
			break;
		case MOTION::ROLLING_LEFT:
			m_pAnimationCom = m_pAnimationRolling_Left;
			m_pAnimationCom->Reset();
			break;
		case MOTION::RUN_LEFT:
			m_pAnimationCom = m_pAnimationRun_Left;
			break;
		default:
			m_pAnimationCom = m_pAnimationIdle_Left;
			break;
		}
	}

	if (m_eCurDirection == DIRECTION::RIGHT)
	{
		switch (eNextMotion)
		{
		case MOTION::IDLE_RIGHT:
			m_pAnimationCom = m_pAnimationIdle_Right;
			break;
		case MOTION::WALK_RIGHT:
			m_pAnimationCom = m_pAnimationWalk_Right;
			break;
		case MOTION::JUMP_RIGHT:
			m_pAnimationCom = m_pAnimationJump_Right;
			m_pAnimationCom->Reset();
			break;
		case MOTION::ATTACK_BACK:
			m_pAnimationCom = m_pAnimationAttack_Back;
			m_pAnimationCom->Reset();
			break;
		case MOTION::ROLLING_RIGHT:
			m_pAnimationCom = m_pAnimationRolling_Right;
			m_pAnimationCom->Reset();
			break;
		case MOTION::RUN_RIGHT:
			m_pAnimationCom = m_pAnimationRun_Right;
			break;
		default:
			m_pAnimationCom = m_pAnimationIdle_Right;
			break;
		}
	}

	m_eCurrentMotion = eNextMotion;
}

void CPlayer::MoveTo(_float fTimeDelta)
{

}

void CPlayer::Attacking(_float fTimeDelta)
{
	switch (m_eCurDirection)
	{
	case DIRECTION::FORWARD:
		Change_Motion(MOTION::ATTACK_BACK, fTimeDelta);
		break;
	case DIRECTION::BACKWARD:
		Change_Motion(MOTION::ATTACK_BACK, fTimeDelta);
		break;
	case DIRECTION::LEFT:
		Change_Motion(MOTION::ATTACK_BACK, fTimeDelta);
		break;
	case DIRECTION::RIGHT:
		Change_Motion(MOTION::ATTACK_BACK, fTimeDelta);
		break;
	default:
		Change_Motion(MOTION::ATTACK_BACK, fTimeDelta);
		break;
	}


	_float3 vPlayerPos = m_pTransformCom->Get_State(STATE::POSITION);
	_float3 vPlayerLook = m_pTransformCom->Get_State(STATE::LOOK);

	_float3 vAttackBoxPos = vPlayerPos + vPlayerLook * 1.0f;

	_float3 vAttackBoxScale = _float3(0.7f, 0.7f, 1.3f);

	vAttackBoxPos.y = vPlayerPos.y;

	m_fTime += fTimeDelta;

	m_pPlayerAttackRange_Cube->Set_RangeSize(vAttackBoxPos, vAttackBoxScale, fTimeDelta);


	m_fAttackTime += fTimeDelta;

	if (1.5f < m_fAttackTime)
	{
		m_bAttack = false;
		m_fAttackTime = 0.f;
	}

	if (false == m_bAttack){
		Change_Motion(MOTION::IDLE_BACK, fTimeDelta);
	}

	m_eCurDirection = DIRECTION::FORWARD;
	
}

void CPlayer::Rolling(_float fTimeDelta)
{

	switch (m_eCurDirection)
	{
	case DIRECTION::FORWARD:
		Change_Motion(MOTION::ROLLING_BACK, fTimeDelta);
		m_pTransformCom->Go_Straight(fTimeDelta);
		break;
	case DIRECTION::BACKWARD:
		Change_Motion(MOTION::ROLLING_FRONT, fTimeDelta);
		m_pTransformCom->Go_Backward(fTimeDelta);
		break;
	case DIRECTION::LEFT:
		Change_Motion(MOTION::ROLLING_LEFT, fTimeDelta);
		m_pTransformCom->Go_Left(fTimeDelta);
		break;
	case DIRECTION::RIGHT:
		Change_Motion(MOTION::ROLLING_RIGHT, fTimeDelta);
		m_pTransformCom->Go_Right(fTimeDelta);
		break;
	default:
		Change_Motion(MOTION::ROLLING_BACK, fTimeDelta);
		break;
	}


	m_fRollingTime += fTimeDelta;

	if (0.5f < m_fRollingTime)
	{
		m_bRolling = false;
		m_fRollingTime = 0.f;
	}

	if (false == m_bRolling)
	{
		switch (m_eCurDirection)
		{
		case DIRECTION::FORWARD:
			Change_Motion(MOTION::IDLE_BACK, fTimeDelta);
			m_pTransformCom->Go_Straight(fTimeDelta);
			break;
		case DIRECTION::BACKWARD:
			Change_Motion(MOTION::IDLE_FRONT, fTimeDelta);
			m_pTransformCom->Go_Backward(fTimeDelta);
			break;
		case DIRECTION::LEFT:
			Change_Motion(MOTION::IDLE_LEFT, fTimeDelta);
			m_pTransformCom->Go_Left(fTimeDelta);
			break;
		case DIRECTION::RIGHT:
			Change_Motion(MOTION::IDLE_RIGHT, fTimeDelta);
			m_pTransformCom->Go_Right(fTimeDelta);
			break;
		default:
			Change_Motion(MOTION::IDLE_BACK, fTimeDelta);
			break;
		}
	}
}

void CPlayer::Run(_float fTimeDelta)
{
	if(m_pGameInstance->Key_Pressing(VK_LSHIFT) 
		&& (m_pGameInstance->Key_Pressing('W') || m_pGameInstance->Key_Pressing('S')
			|| m_pGameInstance->Key_Pressing('A') || m_pGameInstance->Key_Pressing('D')))
	{
		switch (m_eCurDirection)
		{
		case DIRECTION::FORWARD:
			Change_Motion(MOTION::RUN_BACK, fTimeDelta);
			break;
		case DIRECTION::BACKWARD:
			Change_Motion(MOTION::RUN_FRONT, fTimeDelta);
			break;
		case DIRECTION::LEFT:
			Change_Motion(MOTION::RUN_LEFT, fTimeDelta);
			break;
		case DIRECTION::RIGHT:
			Change_Motion(MOTION::RUN_RIGHT, fTimeDelta);
			break;
		default:
			Change_Motion(MOTION::RUN_BACK, fTimeDelta);
			break;
		}
	}
	
}

HRESULT CPlayer::Ready_Components()
{
	/* Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{ 5.f, D3DXToRadian(90.0f) };
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	/* Com_Animation_Idle */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Animation_Idle_Back"),
		TEXT("Com_Animation_Idle_Back"), reinterpret_cast<CComponent**>(&m_pAnimationIdle_Back))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Animation_Idle_Front"),
		TEXT("Com_Animation_Idle_Front"), reinterpret_cast<CComponent**>(&m_pAnimationIdle_Front))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Animation_Idle_Left"),
		TEXT("Com_Animation_Idle_Left"), reinterpret_cast<CComponent**>(&m_pAnimationIdle_Left))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Animation_Idle_Right"),
		TEXT("Com_Animation_Idle_Right"), reinterpret_cast<CComponent**>(&m_pAnimationIdle_Right))))
		return E_FAIL;

	/* Com_Animation_Walk */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Animation_Walk_Back"),
		TEXT("Com_Animation_Walk_Back"), reinterpret_cast<CComponent**>(&m_pAnimationWalk_Back))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Animation_Walk_Front"),
		TEXT("Com_Animation_Walk_Front"), reinterpret_cast<CComponent**>(&m_pAnimationWalk_Front))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Animation_Walk_Left"),
		TEXT("Com_Animation_Walk_Left"), reinterpret_cast<CComponent**>(&m_pAnimationWalk_Left))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Animation_Walk_Right"),
		TEXT("Com_Animation_Walk_Right"), reinterpret_cast<CComponent**>(&m_pAnimationWalk_Right))))
		return E_FAIL;

	/* Com_Animation_Jump */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Animation_Jump_Back"),
		TEXT("Com_Animation_Jump_Back"), reinterpret_cast<CComponent**>(&m_pAnimationJump_Back))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Animation_Jump_Front"),
		TEXT("Com_Animation_Jump_Front"), reinterpret_cast<CComponent**>(&m_pAnimationJump_Front))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Animation_Jump_Left"),
		TEXT("Com_Animation_Jump_Left"), reinterpret_cast<CComponent**>(&m_pAnimationJump_Left))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Animation_Jump_Right"),
		TEXT("Com_Animation_Jump_Right"), reinterpret_cast<CComponent**>(&m_pAnimationJump_Right))))
		return E_FAIL;
	/* Com_Animation_Attack */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Animation_Attack_Back"),
		TEXT("Com_Animation_Attack_Back"), reinterpret_cast<CComponent**>(&m_pAnimationAttack_Back))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Animation_Attack_Front"),
		TEXT("Com_Animation_Attack_Front"), reinterpret_cast<CComponent**>(&m_pAnimationAttack_Front))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Animation_Attack_Left"),
		TEXT("Com_Animation_Attack_Left"), reinterpret_cast<CComponent**>(&m_pAnimationAttack_Left))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Animation_Attack_Right"),
		TEXT("Com_Animation_Attack_Right"), reinterpret_cast<CComponent**>(&m_pAnimationAttack_Right))))
		return E_FAIL;

	/* Com_Animation_Rolling */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Animation_Rolling_Back"),
		TEXT("Com_Animation_Rolling_Back"), reinterpret_cast<CComponent**>(&m_pAnimationRolling_Back))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Animation_Rolling_Front"),
		TEXT("Com_Animation_Rolling_Front"), reinterpret_cast<CComponent**>(&m_pAnimationRolling_Front))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Animation_Rolling_Left"),
		TEXT("Com_Animation_Rolling_Left"), reinterpret_cast<CComponent**>(&m_pAnimationRolling_Left))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Animation_Rolling_Right"),
		TEXT("Com_Animation_Rolling_Right"), reinterpret_cast<CComponent**>(&m_pAnimationRolling_Right))))
		return E_FAIL;


	/* Com_Animation_Run */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Animation_Run_Back"),
		TEXT("Com_Animation_Run_Back"), reinterpret_cast<CComponent**>(&m_pAnimationRun_Back))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Animation_Run_Front"),
		TEXT("Com_Animation_Run_Front"), reinterpret_cast<CComponent**>(&m_pAnimationRun_Front))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Animation_Run_Left"),
		TEXT("Com_Animation_Run_Left"), reinterpret_cast<CComponent**>(&m_pAnimationRun_Left))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Animation_Run_Right"),
		TEXT("Com_Animation_Run_Right"), reinterpret_cast<CComponent**>(&m_pAnimationRun_Right))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;

	/* Com_Land */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Land"),
		TEXT("Com_Land"), reinterpret_cast<CComponent**>(&m_pLandCom))))
		return E_FAIL;

	/* Com_Status */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Status_Player"),
		TEXT("Com_Status"), reinterpret_cast<CComponent**>(&m_pStatusCom))))
		return E_FAIL;

	m_pAnimationCom = m_pAnimationIdle_Back;

	return S_OK;
}


HRESULT CPlayer::Begin_RenderState()
{
	/* 알파 테스트 : 픽셀의 알파를 비교해서 그린다 안그린다를 설정. */
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 200);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);

	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);


	return S_OK;
}

HRESULT CPlayer::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, TRUE);
	// m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);

	return S_OK;
}

CPlayer* CPlayer::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CPlayer* pInstance = new CPlayer(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : pGraphic_Device");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CPlayer::Clone(void* pArg)
{
	CPlayer* pInstance = new CPlayer(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CPlayer");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CPlayer::Free()
{
	__super::Free();

	Safe_Release(m_pAnimationIdle_Front);
	Safe_Release(m_pAnimationIdle_Back);
	Safe_Release(m_pAnimationIdle_Left);
	Safe_Release(m_pAnimationIdle_Right);

	Safe_Release(m_pAnimationJump_Front);
	Safe_Release(m_pAnimationJump_Back);
	Safe_Release(m_pAnimationJump_Left);
	Safe_Release(m_pAnimationJump_Right);

	Safe_Release(m_pAnimationWalk_Front);
	Safe_Release(m_pAnimationWalk_Back);
	Safe_Release(m_pAnimationWalk_Left);
	Safe_Release(m_pAnimationWalk_Right);

	Safe_Release(m_pAnimationRun_Front);
	Safe_Release(m_pAnimationRun_Back);
	Safe_Release(m_pAnimationRun_Left);
	Safe_Release(m_pAnimationRun_Right);

	Safe_Release(m_pAnimationAttack_Front);
	Safe_Release(m_pAnimationAttack_Back);
	Safe_Release(m_pAnimationAttack_Left);
	Safe_Release(m_pAnimationAttack_Right);

	Safe_Release(m_pAnimationRolling_Front);
	Safe_Release(m_pAnimationRolling_Back);
	Safe_Release(m_pAnimationRolling_Left);
	Safe_Release(m_pAnimationRolling_Right);

	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pLandCom);
	Safe_Release(m_pStatusCom);

}