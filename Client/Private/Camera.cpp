#include "Camera.h"

CCamera::CCamera(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject { pGraphic_Device }
{
}

CCamera::CCamera(const CCamera& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CCamera::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCamera::Initialize(void* pArg)
{
	CAMERA_DESC* pDesc = static_cast<CAMERA_DESC*>(pArg);
	m_fFov = pDesc->fFov;
	m_fAspect = static_cast<_float>(g_iWinSizeX) / g_iWinSizeY;
	m_fNear = pDesc->fNear;
	m_fFar = pDesc->fFar;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	/* ī�޶��� ���� ���¸� Ʈ������ ���ä�����Ʈ���� ����ȭ�Ѵ�.  */
	/* �佺���̽� ��ȯ����� ���Ѵ�. */
	/*D3DXMatrixLookAtLH();*/
	m_pTransformCom->Set_State(STATE::POSITION, pDesc->vEye);
	m_pTransformCom->LookAt(pDesc->vAt);

	/* ���������̽� ��ȯ��� */
	D3DXMatrixPerspectiveFovLH(&m_ProjMatrix, m_fFov, m_fAspect, m_fNear, m_fFar);

	m_pTargetTransformCom = reinterpret_cast<CTransform*>(m_pGameInstance->Get_Component(ENUM_CLASS(LEVEL::GAMEPLAY), TEXT("Layer_Player"), TEXT("Com_Transform")));

	m_matWorld = *m_pTransformCom->Get_WorldMatrixPtr();
	return S_OK;
}

void CCamera::Priority_Update(_float fTimeDelta)
{
	/* ī�޶��� �����ӿ� ���� ó���� ��� �����Ѵ�. */

	//1000 1011 1101 1111 
	//1000 0000 0000 0000

	//1000 0000 0000 0000
	//0000 0000 0000 0000

	_float4x4 matParent = *(m_pTargetTransformCom->Get_WorldMatrixPtr());

	_float4x4 matWorld = *(m_pTransformCom->Get_WorldMatrixPtr());

	_float4x4 matConvert = matWorld * matParent;

	m_pTransformCom->Set_WorldMatrix(matConvert);

	m_matWorld = matWorld;

	/**/

	m_pGraphic_Device->SetTransform(D3DTS_VIEW, m_pTransformCom->Get_WorldMatrixInvPtr());
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, D3DXMatrixPerspectiveFovLH(&m_ProjMatrix, m_fFov, m_fAspect, m_fNear, m_fFar));

	m_pTransformCom->Set_WorldMatrix(m_matWorld);
}

void CCamera::Update(_float fTimeDelta)
{

}

void CCamera::Late_Update(_float fTimeDelta)
{

}

HRESULT CCamera::Render()
{
	return S_OK;
}

HRESULT CCamera::Ready_Components(void* pArg)
{
	if (FAILED(__super::Add_Component(ENUM_CLASS(LEVEL::STATIC), TEXT("Prototype_Component_Transform"),
		TEXT("Com_Transform"), reinterpret_cast<CComponent**>(&m_pTransformCom), pArg)))
		return E_FAIL;
	return S_OK;
}

CCamera* CCamera::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCamera* pInstance = new CCamera(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCamera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCamera::Clone(void* pArg)
{
	CCamera* pInstance = new CCamera(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCamera");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCamera::Free()
{
	__super::Free();

	Safe_Release(m_pTransformCom);
}
