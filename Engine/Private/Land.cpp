#include "Land.h"
#include "Transform.h"
#include "VIBuffer.h"

CLand::CLand(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent{ pGraphic_Device }
{
}

CLand::CLand(const CLand& Prototype)
	: CComponent{ Prototype }
{
}

HRESULT CLand::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CLand::Initialize(void* pArg)
{

	return S_OK;
}

void CLand::Set_Transform(CTransform* pTransform)
{
	m_pTransform = pTransform;

	Safe_AddRef(m_pTransform);
}

void CLand::Set_TerrainTransform(CTransform* pTerrainTransform)
{
	m_pTerrainTransform = pTerrainTransform;

	Safe_AddRef(m_pTerrainTransform);
}

void CLand::Set_TerrainVIBuffer(CVIBuffer* pTerrainVIBuffer)
{
	m_pTerrainVIBuffer = pTerrainVIBuffer;

	Safe_AddRef(m_pTerrainVIBuffer);
}

_float CLand::Standing_On_Terrain(_float fOffset)
{
	_float3 vWorldPos = m_pTransform->Get_State(STATE::POSITION);

	_float3 vLocalPos = {};
	// 현재 위치는 월드좌표계이다. 그걸 터레인 기준의 로컬좌표계로 바꿔줘야한다.
	D3DXVec3TransformCoord(&vLocalPos, &vWorldPos, m_pTerrainTransform->Get_WorldMatrixInvPtr());
	// 가져온 로컬좌표의 y값을 구한다.
	vLocalPos.y = m_pTerrainVIBuffer->Compute_Height(vLocalPos);

	D3DXVec3TransformCoord(&vWorldPos, &vLocalPos, m_pTerrainTransform->Get_WorldMatrixPtr());

	vWorldPos.y += fOffset;

	return vWorldPos.y;
}

CLand* CLand::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CLand* pInstance = new CLand(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CLand");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CLand::Clone(void* pArg)
{
	CLand* pInstance = new CLand(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CLand");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CLand::Free()
{
	__super::Free();

	Safe_Release(m_pTerrainTransform);
	Safe_Release(m_pTerrainVIBuffer);
	Safe_Release(m_pTransform);
}
