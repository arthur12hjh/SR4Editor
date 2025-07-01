#include "Monster.h"
#include "Client_Defines.h"
#include "GameInstance.h"
#include "AttackRange_Cube.h"


CMonster::CMonster(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{

}

CMonster::CMonster(const CMonster& Prototype)
	: CGameObject{ Prototype }
{

}

HRESULT CMonster::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CMonster::Initialize(void* pArg)
{
	if (FAILED(Ready_Components()))
		return E_FAIL;

	
	m_pPlayer = m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Player"));

	m_pMonsterStatus = dynamic_cast<CStatus*>(Find_Component(TEXT("Com_Status")));
	m_pPlayerStatus = dynamic_cast<CStatus*>(m_pPlayer->Find_Component(TEXT("Com_Status")));

	
	m_iMonsterMaxHP = m_pMonsterStatus->Get_MaxHp();
	m_iMonsterDefense = m_pMonsterStatus->Get_Defense();
	m_iMonsterCurHP = m_iMonsterMaxHP;
	
	
	if (m_pPlayer)
	{
		m_iPlayerDamage = m_pPlayerStatus->Get_Damage();
	}

	m_pLandCom->Set_Transform(m_pTransformCom);
	m_pLandCom->Set_TerrainTransform(dynamic_cast<CTransform*>(m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_BackGround"), TEXT("Com_Transform"))));
	m_pLandCom->Set_TerrainVIBuffer(dynamic_cast<CVIBuffer*>(m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_BackGround"), TEXT("Com_VIBuffer"))));

	m_pTransformCom->Set_State(STATE::POSITION, _float3(
		m_pGameInstance->Random(0.f, 50.f), 
		0.f, 
		m_pGameInstance->Random(0.f, 50.f)));


	m_pTarget = reinterpret_cast<CTransform*>(m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Player"), TEXT("Com_Transform")));


	return S_OK;
}

void CMonster::Priority_Update(_float fTimeDelta)
{
	
}

void CMonster::Update(_float fTimeDelta)
{
	_float3 vStandingPos = m_pTransformCom->Get_State(STATE::POSITION);
	vStandingPos.y = m_pLandCom->Standing_On_Terrain(0.5f);
	m_pTransformCom->Set_State(STATE::POSITION, vStandingPos);

	Look();
	Chase(fTimeDelta);
	if (m_bAttackAnimation)
    {
        m_fAttackAnimationTimer += fTimeDelta * 0.7f; 
		m_fAttackAnimationTotalTime = 0.f;
        if (m_pAnimationCom) {
			m_fAttackAnimationTotalTime = m_pAnimationCom->Get_AnimationDuration() / 0.7f;
        }
        if (m_fAttackAnimationTimer >= m_fAttackAnimationTotalTime)
        {
            m_bAttackAnimation = false; 
            m_fAttackAnimationTimer = 0.f;  
        }
    }
	Attacking(fTimeDelta);
	Attack_Delay(fTimeDelta);

	m_pAnimationCom->Update_Animation(fTimeDelta * 0.7f);

	m_pGameInstance->Add_GameObject_ToLayer(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_GameObject_AttackRange_Cube"), ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Monster_AttackRange_Cube"), m_pTransformCom);
	m_pMonsterAttackRange_Cube = static_cast<CAttackRange_Cube*>(m_pGameInstance->Get_GameObject(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Monster_AttackRange_Cube")));

}

void CMonster::Late_Update(_float fTimeDelta)
{
	for (auto iter = m_Objmap.begin(); iter != m_Objmap.end();)
	{
		iter->second -= fTimeDelta;

		if (iter->second < 0.f)
		{
			m_Objmap.erase(iter++);
		}
		else
			++iter;
	}


	m_pGameInstance->Add_RenderGroup(RENDER::NONBLEND, this);
}

HRESULT CMonster::Render()
{
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

	return S_OK;
}

void CMonster::OnCollisionEnter(void* pArg, _float fTimeDelta)
{
	if (m_Objmap.end() != m_Objmap.find(static_cast<CAttackRange_Cube*>(pArg)))
		return;

	m_iMonsterCurHP -= m_iPlayerDamage;

	wchar_t szBuffer[100];
	swprintf_s(szBuffer, 100, L"Monster HP: %d, Player Damage: %d\n", m_iMonsterCurHP, m_iPlayerDamage);
	OutputDebugString(szBuffer);
	
	m_Objmap.insert({ static_cast<CAttackRange_Cube*>(pArg), m_fAttackAnimationTotalTime });

	if(m_iMonsterCurHP <= 0)
		m_isDead = true;
}

void CMonster::Change_Motion(MOTION eNextMotion)
{
	if (m_eCurrentMotion == eNextMotion) return;

	switch (eNextMotion)
	{
	case MOTION::IDLE_BACK:
		m_pAnimationCom = m_pAnimationIdle_Back;
		break;
	case MOTION::WALK_BACK:
		m_pAnimationCom = m_pAnimationChase_Back;
		break;
	case MOTION::WALK_FRONT:
		m_pAnimationCom = m_pAnimationChase_Front;
		break;
	case MOTION::WALK_LEFT:
		m_pAnimationCom = m_pAnimationChase_Left;
		break;
	case MOTION::WALK_RIGHT:
		m_pAnimationCom = m_pAnimationChase_Right;
		break;
	case MOTION::ATTACK_BACK:
		m_pAnimationCom = m_pAnimationAttack_Back;
		m_pAnimationCom->Reset();
		break;
	case MOTION::ATTACK_FRONT:
		m_pAnimationCom = m_pAnimationAttack_Front;
		m_pAnimationCom->Reset();
		break;
	case MOTION::ATTACK_LEFT:
		m_pAnimationCom = m_pAnimationAttack_Left;
		m_pAnimationCom->Reset();
		break;
	case MOTION::ATTACK_RIGHT:
		m_pAnimationCom = m_pAnimationAttack_Right;
		m_pAnimationCom->Reset();
		break;
	default:
		m_pAnimationCom = m_pAnimationIdle_Back;
		break;
	}
	
	m_eCurrentMotion = eNextMotion;

}

void CMonster::Attacking(_float fTimeDelta)
{
	if (!m_bAttackReady || !m_bChaseFinish || m_bAttackAnimation) return;

	Change_Motion(MOTION::ATTACK_FRONT); 
	m_bAttack = true; 

	_float3 vMonsterPos = m_pTransformCom->Get_State(STATE::POSITION);
	_float3 vLook = m_pTransformCom->Get_State(STATE::LOOK);
	_float3 vAttackBoxPos = vMonsterPos + vLook * 1.0f;
	vAttackBoxPos.y = vMonsterPos.y;

	_float3 vAttackBoxScale = _float3(0.3f, 0.6f, 0.5f);
	m_pMonsterAttackRange_Cube->Set_RangeSize(vAttackBoxPos, vAttackBoxScale, fTimeDelta);

	m_bAttackReady = false;         
	m_fAttackCoolTime = 0.f;         
	m_bAttackAnimation = true;  
	m_fAttackAnimationTimer = 0.f;          
}

void CMonster::Attack_Delay(_float fTimeDelta)
{
	if (m_bAttackAnimation)
	{
		return;
	}

	if (!m_bAttackReady)
	{
		m_fAttackCoolTime += fTimeDelta;

		if (m_fAttackCoolTime >= 1.5f) 
		{
			m_bAttackReady = true;
			m_fAttackCoolTime = 0.f;
		}
	}

	if (!m_bAttackReady && !m_bChaseFinish) {
		Change_Motion(MOTION::WALK_FRONT); 
	}
	else if (!m_bAttackReady && m_bChaseFinish) {
		Change_Motion(MOTION::IDLE_BACK); 
	}
}

void CMonster::Chase(_float fTimeDelta)
{
	_float3 vTargetPos = m_pTarget->Get_State(STATE::POSITION);
	m_vMonsterPos = m_pTransformCom->Get_State(STATE::POSITION);

	_float3 fDistance = m_vMonsterPos - vTargetPos;

	_float fDist = D3DXVec3Length(&fDistance);

	if (m_bLook)
	{	
		m_pTransformCom->LookAt(vTargetPos);
		m_pTransformCom->Chase(vTargetPos, fTimeDelta, 1.f);

		if(!m_bChaseFinish)
			Change_Motion(MOTION::WALK_FRONT);
	}
	else
		Change_Motion(MOTION::IDLE_BACK);

	if (fDist <= 1.0f)
	{
		m_bChaseFinish = true;
	}
	else
		m_bChaseFinish = false;
	
}

void CMonster::Look()
{
	_float3 vTargetPos = m_pTarget->Get_State(STATE::POSITION);
	m_vMonsterPos = m_pTransformCom->Get_State(STATE::POSITION);

	
	_float3 fDistance = m_vMonsterPos - vTargetPos;

	_float fDist = D3DXVec3Length(&fDistance);

	_float3 vToTarget = m_vMonsterPos - vTargetPos;

	_float3 vMonsterLook = m_pTransformCom->Get_State(STATE::LOOK);

	D3DXVec3Normalize(&vToTarget, &vToTarget);
	D3DXVec3Normalize(&vMonsterLook, &vMonsterLook);

	_float fRadianAngle = D3DXVec3Dot(&vToTarget, &vMonsterLook);
	_float fAngle = D3DXToDegree(fRadianAngle);

	if (fDist < 3.f)
		m_bLook = true;

	else
		m_bLook = false;



}



HRESULT CMonster::Ready_Components()
{
	/* Com_Transform */
	CTransform::TRANSFORM_DESC		TransformDesc{ 2.f, D3DXToRadian(90.0f) };
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), &TransformDesc)))
		return E_FAIL;

	/* Com_Animation */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Animation_Mon_Idle"),
		TEXT("Com_Animation_Mon_Idle"), reinterpret_cast<CComponent**>(&m_pAnimationIdle_Back))))
		return E_FAIL;

	/* Com_Animation_Walk */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Animation_Mon_Chase_Back"),
		TEXT("Com_Animation_Mon_Chase_Back"), reinterpret_cast<CComponent**>(&m_pAnimationChase_Back))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Animation_Mon_Chase_Front"),
		TEXT("Com_Animation_Mon_Chase_Front"), reinterpret_cast<CComponent**>(&m_pAnimationChase_Front))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Animation_Mon_Chase_Left"),
		TEXT("Com_Animation_Mon_Chase_Left"), reinterpret_cast<CComponent**>(&m_pAnimationChase_Left))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Animation_Mon_Chase_Right"),
		TEXT("Com_Animation_Mon_Chase_Right"), reinterpret_cast<CComponent**>(&m_pAnimationChase_Right))))
		return E_FAIL;

	/* Com_Animation_Attack */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Animation_Mon_Attack_Back"),
		TEXT("Com_Animation_Mon_Attack_Back"), reinterpret_cast<CComponent**>(&m_pAnimationAttack_Back))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Animation_Mon_Attack_Front"),
		TEXT("Com_Animation_Mon_Attack_Front"), reinterpret_cast<CComponent**>(&m_pAnimationAttack_Front))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Animation_Mon_Attack_Left"),
		TEXT("Com_Animation_Mon_Attack_Left"), reinterpret_cast<CComponent**>(&m_pAnimationAttack_Left))))
		return E_FAIL;

	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Animation_Mon_Attack_Right"),
		TEXT("Com_Animation_Mon_Attack_Right"), reinterpret_cast<CComponent**>(&m_pAnimationAttack_Right))))
		return E_FAIL;

	/* Com_VIBuffer */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_VIBuffer_Rect"),
		TEXT("Com_VIBuffer"), reinterpret_cast<CComponent**>(&m_pVIBufferCom))))
		return E_FAIL;	

	/* Com_EventHandler */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Event_Handler"),
		TEXT("Com_Event_Handler"), reinterpret_cast<CComponent**>(&m_pEvent_HandlerCom))))
		return E_FAIL;

	/* Com_Land */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Land"),
		TEXT("Com_Land"), reinterpret_cast<CComponent**>(&m_pLandCom))))
		return E_FAIL;

	/* Com_Status */
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Prototype_Component_Status_Monster"),
		TEXT("Com_Status"), reinterpret_cast<CComponent**>(&m_pStatusCom))))
		return E_FAIL;

	m_pAnimationCom = m_pAnimationIdle_Back;


	return S_OK;
}

HRESULT CMonster::Begin_RenderState()
{
	/* 렌더링할 때 알파값을 기준으로 섞어준다.*/

	//m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	//m_pGraphic_Device->SetRenderState(D3DRS_BLENDOP, D3DBLENDOP_ADD);
	//m_pGraphic_Device->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//m_pGraphic_Device->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, FALSE);

	/* 알파 테스트 : 픽셀의 알파를 비교해서 그린다 안그린다를 설정. */
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAREF, 0);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATER);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_NONE);



	return S_OK;
}

HRESULT CMonster::End_RenderState()
{
	m_pGraphic_Device->SetRenderState(D3DRS_LIGHTING, TRUE);

	// m_pGraphic_Device->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_ALPHATESTENABLE, FALSE);
	m_pGraphic_Device->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);

	return S_OK;
}

CMonster* CMonster::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CMonster* pInstance = new CMonster(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : pGraphic_Device");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CMonster::Clone(void* pArg)
{
	CMonster* pInstance = new CMonster(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CMonster");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CMonster::Free()
{
	__super::Free();

	Safe_Release(m_pAnimationIdle_Front);
	Safe_Release(m_pAnimationIdle_Back);
	Safe_Release(m_pAnimationIdle_Left);
	Safe_Release(m_pAnimationIdle_Right);

	Safe_Release(m_pAnimationChase_Front);
	Safe_Release(m_pAnimationChase_Back);
	Safe_Release(m_pAnimationChase_Left);
	Safe_Release(m_pAnimationChase_Right);

	Safe_Release(m_pAnimationAttack_Front);
	Safe_Release(m_pAnimationAttack_Back);
	Safe_Release(m_pAnimationAttack_Left);
	Safe_Release(m_pAnimationAttack_Right);
	
	Safe_Release(m_pTransformCom);
	Safe_Release(m_pVIBufferCom);
	Safe_Release(m_pEvent_HandlerCom);
	Safe_Release(m_pLandCom);
	Safe_Release(m_pStatusCom);

}
