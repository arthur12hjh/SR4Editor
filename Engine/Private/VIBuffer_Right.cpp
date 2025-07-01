#include "VIBuffer_Right.h"

CVIBuffer_Right::CVIBuffer_Right(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CVIBuffer{ pGraphic_Device }
{
}

CVIBuffer_Right::CVIBuffer_Right(const CVIBuffer_Right& Prototype)
	: CVIBuffer{ Prototype }
{
}

HRESULT CVIBuffer_Right::Initialize_Prototype()
{
	m_iNumVertices = 4;
	m_iVertexStride = sizeof(VTXPOSTEX);
	m_iFVF = D3DFVF_XYZ | D3DFVF_TEX1 | D3DFVF_TEXCOORDSIZE2(0);
	m_ePrimitiveType = D3DPT_TRIANGLELIST;
	m_iNumPrimitive = 2;

	m_iIndexStride = 2;
	m_iNumIndices = 6;
	m_eIndexFormat = D3DFMT_INDEX16;

	/* 정점을 배열로 선언한다. */
	/*USAGE : 0 OR D3DUSAGE_DYNAMIC*/

	/*
	D3DPOOL_SYSTEMMEM : RAM
	D3DPOOL_DEFAULT : VRAM
	*/
	if (FAILED(m_pGraphic_Device->CreateVertexBuffer(m_iNumVertices * m_iVertexStride, 0, m_iFVF, D3DPOOL_MANAGED, &m_pVB, nullptr)))
		return E_FAIL;

	VTXPOSTEX* pVertices = { nullptr };

	/* 할당한 공간에 접근하여 값을 기록하낟. */
	m_pVB->Lock(0, /*m_iNumVertices * m_iVertexStride*/0, reinterpret_cast<void**>(&pVertices), 0);

	pVertices[0].vPosition = _float3(0.f, 0.5f, 0.f);
	pVertices[0].vTexcoord = _float2(0.f, 0.f);

	pVertices[1].vPosition = _float3(1.f, 0.5f, 0.f);
	pVertices[1].vTexcoord = _float2(1.f, 0.f);

	pVertices[2].vPosition = _float3(1.f, -0.5f, 0.f);
	pVertices[2].vTexcoord = _float2(1.f, 1.f);

	pVertices[3].vPosition = _float3(0.f, -0.5f, 0.f);
	pVertices[3].vTexcoord = _float2(0.f, 1.f);

	m_pVB->Unlock();

	if (FAILED(m_pGraphic_Device->CreateIndexBuffer(m_iIndexStride * m_iNumIndices, 0, m_eIndexFormat, D3DPOOL_MANAGED, &m_pIB, nullptr)))
		return E_FAIL;


	_ushort* pIndices = { nullptr };

	m_pIB->Lock(0, 0, reinterpret_cast<void**>(&pIndices), 0);

	pIndices[0] = 0;
	pIndices[1] = 1;
	pIndices[2] = 2;

	pIndices[3] = 0;
	pIndices[4] = 2;
	pIndices[5] = 3;

	m_pIB->Unlock();



	return S_OK;
}

HRESULT CVIBuffer_Right::Initialize(void* pArg)
{
	return S_OK;
}

CVIBuffer_Right* CVIBuffer_Right::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CVIBuffer_Right* pInstance = new CVIBuffer_Right(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CVIBuffer_Right");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CComponent* CVIBuffer_Right::Clone(void* pArg)
{
	CVIBuffer_Right* pInstance = new CVIBuffer_Right(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CVIBuffer_Right");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CVIBuffer_Right::Free()
{
	__super::Free();

}
