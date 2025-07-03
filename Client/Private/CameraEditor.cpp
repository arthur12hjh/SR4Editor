#include "CameraEditor.h"


CCameraEditor::CCameraEditor(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CCamera{ pGraphic_Device }
{
}

CCameraEditor::CCameraEditor(const CCameraEditor& Prototype)
	: CCamera{ Prototype }
{
}

HRESULT CCameraEditor::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CCameraEditor::Initialize(void* pArg)
{
	CAMERA_DESC* pDesc = static_cast<CAMERA_DESC*>(pArg);
	m_fFov = pDesc->fFov;
	m_fAspect = static_cast<_float>(g_iWinSizeX) / g_iWinSizeY;
	m_fNear = pDesc->fNear;
	m_fFar = pDesc->fFar;

	if (FAILED(Ready_Components(pArg)))
		return E_FAIL;

	/* 카메라의 월드 상태를 트랜스폼 컴포넌트에게 동기화한다.  */
	/* 뷰스페이스 변환행렬을 구한다. */
	/*D3DXMatrixLookAtLH();*/
	m_pTransformCom->Set_State(STATE::POSITION, pDesc->vEye);
	m_pTransformCom->LookAt(pDesc->vAt);

	/* 투영스페이스 변환행렬 */
	D3DXMatrixPerspectiveFovLH(&m_ProjMatrix, m_fFov, m_fAspect, m_fNear, m_fFar);

	POINT		ptMouse{};

	GetCursorPos(&ptMouse);

	ScreenToClient(g_hWnd, &ptMouse);

	m_vOldMouse = _float2(ptMouse.x, ptMouse.y);

	m_pTransformCom->Set_State(STATE::POSITION, _float3(0, 15, -5));

	return S_OK;
}

void CCameraEditor::Priority_Update(_float fTimeDelta)
{

	m_pGraphic_Device->SetTransform(D3DTS_VIEW, m_pTransformCom->Get_WorldMatrixInvPtr());
	m_pGraphic_Device->SetTransform(D3DTS_PROJECTION, D3DXMatrixPerspectiveFovLH(&m_ProjMatrix, m_fFov, m_fAspect, m_fNear, m_fFar));

}

void CCameraEditor::Update(_float fTimeDelta)
{
	POINT		ptMouse{};

	GetCursorPos(&ptMouse);

	ScreenToClient(g_hWnd, &ptMouse);


	if (m_pGameInstance->Key_Pressing(VK_MBUTTON))
	{
		_float			fMove = {};

		if (fMove = ptMouse.x - m_vOldMouse.x)
		{
			m_pTransformCom->Turn(_float3(0.f, 1.f, 0.f), fTimeDelta * fMove * m_fSensor);
		}

		if (fMove = ptMouse.y - m_vOldMouse.y)
		{
			m_pTransformCom->Turn(m_pTransformCom->Get_State(STATE::RIGHT), fTimeDelta * fMove * m_fSensor);
		}
	}

	if (m_pGameInstance->Key_Pressing('W'))
	{
		m_pTransformCom->Go_Straight(fTimeDelta);
	}
	if (m_pGameInstance->Key_Pressing('S'))
	{
		m_pTransformCom->Go_Backward(fTimeDelta);
	}
	if (m_pGameInstance->Key_Pressing('A'))
	{
		m_pTransformCom->Go_Left(fTimeDelta);
	}
	if (m_pGameInstance->Key_Pressing('D'))
	{
		m_pTransformCom->Go_Right(fTimeDelta);
	}
	if (m_pGameInstance->Key_Pressing(VK_SPACE))
	{
		m_pTransformCom->Go_Up(fTimeDelta);
	}
	if (m_pGameInstance->Key_Pressing(VK_LSHIFT))
	{
		m_pTransformCom->Go_Down(fTimeDelta);
	}

	m_vOldMouse = _float2(ptMouse.x, ptMouse.y);
}

void CCameraEditor::Late_Update(_float fTimeDelta)
{
}

HRESULT CCameraEditor::Render()
{
	return S_OK;
}

HRESULT CCameraEditor::Ready_Components(void* pArg)
{
	return __super::Ready_Components(pArg);
}

CCameraEditor* CCameraEditor::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CCameraEditor* pInstance = new CCameraEditor(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CCameraEditor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CCameraEditor::Clone(void* pArg)
{
	CCameraEditor* pInstance = new CCameraEditor(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CCameraEditor");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCameraEditor::Free()
{
	__super::Free();
}
