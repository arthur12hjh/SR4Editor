#include "AlphaObject.h"

CAlphaObject::CAlphaObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device }
{
}

CAlphaObject::CAlphaObject(const CAlphaObject& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CAlphaObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CAlphaObject::Initialize(void* pArg)
{
	return S_OK;
}

void CAlphaObject::Priority_Update(_float fTimeDelta)
{
}

void CAlphaObject::Update(_float fTimeDelta)
{
}

void CAlphaObject::Late_Update(_float fTimeDelta)
{
}

HRESULT CAlphaObject::Render()
{
	return S_OK;
}

// Renderer 상에서 알파 소팅을 하기 위해 카메라와 객체 사이 거리를 구해준다.
void CAlphaObject::Compute_CamDistance(const _float3& vWorldPos)
{
	// 뷰스페이스 상의 뷰매트릭스 역행렬의 POSITION은? 카메라의 위치를 담고 있다.
	_float4x4		ViewMatrix = {};
	m_pGraphic_Device->GetTransform(D3DTS_VIEW, &ViewMatrix);
	D3DXMatrixInverse(&ViewMatrix, nullptr, &ViewMatrix);

	// 뷰스페이스 상의 POSITION을 _float3에 넣는다.
	_float3			vCamPos = {};
	memcpy(&vCamPos, &ViewMatrix.m[3], sizeof(_float3));

	// 카메라의 위치와 내 위치를 빼서 거리를 구해준다.
	_float3			vDir = vCamPos - vWorldPos;
	m_fCamDistance = D3DXVec3Length(&vDir);
}

void CAlphaObject::Free()
{
	__super::Free();
}
