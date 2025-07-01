#include "Collision_Manager.h"

#include "Layer.h"
#include "GameObject.h"
#include "Transform.h"
#include "VIBuffer.h"


CCollision_Manager::CCollision_Manager()
{
}

HRESULT CCollision_Manager::Initialize()
{
	return S_OK;
}

void CCollision_Manager::Late_Update(_float fTimeDelta)
{
	for (auto& pPairLayer : m_listCollisionLayer)
	{
		CheckPlaneCollision(pPairLayer.first, pPairLayer.second, fTimeDelta);
	}
}

void CCollision_Manager::Set_Collision_Layer(CLayer* pLayerSrc, CLayer* pLayerDst)
{
	pair<CLayer*, CLayer*> pairLayer = make_pair(pLayerSrc, pLayerDst);

	Safe_AddRef(pLayerSrc);
	Safe_AddRef(pLayerDst);

	m_listCollisionLayer.push_back(pairLayer);

}

HRESULT CCollision_Manager::CheckPlaneCollision(CLayer* pLayerSrc, CLayer* pLayerDst, _float fTimeDelta)
{
	list<CGameObject*>* pListSrc = pLayerSrc->Get_Layer();
	list<CGameObject*>* pListDst = pLayerDst->Get_Layer();

	for (auto& pSrc : *pListSrc)
	{
		for (auto& pDst : *pListDst)
		{
			if (PlaneCollisionEnter(pSrc, pDst))
			{
				pSrc->OnCollisionEnter(pDst, fTimeDelta);
				pDst->OnCollisionEnter(pSrc, fTimeDelta);
			}
		}
	}


	return S_OK;
}

_bool CCollision_Manager::PlaneCollisionEnter(CGameObject* pSrc, CGameObject* pDst)
{ 
	CVIBuffer* pSrcVIBuffer = dynamic_cast<CVIBuffer*>(pSrc->Find_Component(TEXT("Com_VIBuffer")));
	CVIBuffer* pDstVIBuffer = dynamic_cast<CVIBuffer*>(pDst->Find_Component(TEXT("Com_VIBuffer")));

	CTransform* pSrcTransform = dynamic_cast<CTransform*>(pSrc->Find_Component(TEXT("Com_Transform")));
	CTransform* pDstTransform = dynamic_cast<CTransform*>(pDst->Find_Component(TEXT("Com_Transform")));

	_float3* pSrcPos = pSrcVIBuffer->Get_Vertex();
	_float3* pDstPos = pDstVIBuffer->Get_Vertex();

	_float3 fSrcPos[2] = {};
	_float3 fDstPos[2] = {};

	D3DXVec3TransformCoord(&fSrcPos[0], &pSrcPos[0], pSrcTransform->Get_WorldMatrixPtr());
	D3DXVec3TransformCoord(&fSrcPos[1], &pSrcPos[2], pSrcTransform->Get_WorldMatrixPtr());
	
	D3DXVec3TransformCoord(&fDstPos[0], &pDstPos[0], pDstTransform->Get_WorldMatrixPtr());
	D3DXVec3TransformCoord(&fDstPos[1], &pDstPos[2], pDstTransform->Get_WorldMatrixPtr());


	_float srcXMax, srcXMin, srcYMax, srcYMin, srcZMax, srcZMin = {};
	_float dstXMax, dstXMin, dstYMax, dstYMin, dstZMax, dstZMin = {};

	srcXMax = max(fSrcPos[0].x, fSrcPos[1].x);
	srcYMax = max(fSrcPos[0].y, fSrcPos[1].y);
	srcZMax = max(fSrcPos[0].z, fSrcPos[1].z);

	srcXMin = min(fSrcPos[0].x, fSrcPos[1].x);
	srcYMin = min(fSrcPos[0].y, fSrcPos[1].y);
	srcZMin = min(fSrcPos[0].z, fSrcPos[1].z);

	dstXMax = max(fDstPos[0].x, fDstPos[1].x);
	dstYMax = max(fDstPos[0].y, fDstPos[1].y);
	dstZMax = max(fDstPos[0].z, fDstPos[1].z);

	dstXMin = min(fDstPos[0].x, fDstPos[1].x);
	dstYMin = min(fDstPos[0].y, fDstPos[1].y);
	dstZMin = min(fDstPos[0].z, fDstPos[1].z);

	if (!(srcXMax > dstXMin && dstXMax > srcXMin))
		return false;

	if (!(srcYMax > dstYMin && dstYMax > srcYMin))
		return false;

	if (!(srcZMax > dstZMin && dstZMax > srcZMin))
		return false;

	return true;
}


CCollision_Manager* CCollision_Manager::Create()
{
	CCollision_Manager* pInstance = new CCollision_Manager();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CCollision_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CCollision_Manager::Free()
{
	__super::Free();

	for (auto& pPairLayer : m_listCollisionLayer)
	{
		Safe_Release(pPairLayer.first);
		Safe_Release(pPairLayer.second);
	}

	m_listCollisionLayer.clear();

	Safe_Release(m_pGraphic_Device);
}
