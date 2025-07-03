#include "TerrainVertexData.h"

#include "GameInstance.h"

CTerrainVertexData::CTerrainVertexData(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CDataObject{ pGraphic_Device }
{
}

CTerrainVertexData::CTerrainVertexData(const CTerrainVertexData& Prototype)
	: CDataObject{ Prototype }
{
}

HRESULT CTerrainVertexData::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CTerrainVertexData::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	m_pStrFileName = (static_cast<_wstring*>(pArg));

	if (FAILED(ReadFile(*m_pStrFileName)))
		return E_FAIL;

	if (FAILED(SerializeData()))
		return E_FAIL;

	return S_OK;
}

void CTerrainVertexData::Set_Data(TERRAINDATATYPE_DESC dataDesc)
{
	for (auto& dataTmp : m_listData)
	{
		if (dataTmp.iVertexPositionX == dataDesc.iVertexPositionX
			&& dataTmp.iVertexPositionZ == dataDesc.iVertexPositionZ)
		{
			dataTmp.vPosition = dataDesc.vPosition;

			list<TERRAINDATATYPE_DESC> listDataTmp;
			listDataTmp.push_back(dataDesc);

			if (m_pTerrainVIBufferCom != nullptr)
				m_pTerrainVIBufferCom->Apply_TerrainData(listDataTmp);

			return;
		}
	}
	m_listData.push_back(dataDesc);

	list<TERRAINDATATYPE_DESC> listDataTmp;
	listDataTmp.push_back(dataDesc);

	if (m_pTerrainVIBufferCom != nullptr)
		m_pTerrainVIBufferCom->Apply_TerrainData(listDataTmp);

	return;
}

HRESULT CTerrainVertexData::SerializeData()
{
	while (!m_listStrData.empty())
	{
		/*
		_uint m_iVertexPositionX;
		_uint m_iVertexPositionZ;
		_float3 m_vPosition;
		*/
		TERRAINDATATYPE_DESC dataTmp;

		dataTmp.iVertexPositionX = stoi(m_listStrData.front());
		m_listStrData.pop_front();

		dataTmp.iVertexPositionZ = stoi(m_listStrData.front());
		m_listStrData.pop_front();
		
		dataTmp.vPosition.x = stoi(m_listStrData.front());
		m_listStrData.pop_front();

		dataTmp.vPosition.y = stoi(m_listStrData.front());
		m_listStrData.pop_front();

		dataTmp.vPosition.z = stoi(m_listStrData.front());
		m_listStrData.pop_front();

		m_listData.push_back(dataTmp);

		if (m_listStrData.size() == 1)
			m_listStrData.pop_back();

	}
	
	return S_OK;
}

HRESULT CTerrainVertexData::Save_Data()
{
	for (auto& pStr : m_listStrData)
	{
		pStr.clear();
		pStr.shrink_to_fit();
	}
	m_listStrData.clear();

	for (auto& pData : m_listData)
	{
		string strTmp;

		strTmp.append(to_string(pData.iVertexPositionX));
		strTmp.append(",");
		strTmp.append(to_string(pData.iVertexPositionZ));
		strTmp.append(",");
		strTmp.append(to_string(pData.vPosition.x));
		strTmp.append(",");
		strTmp.append(to_string(pData.vPosition.y));
		strTmp.append(",");
		strTmp.append(to_string(pData.vPosition.z));
		strTmp.append("\n");

		m_listStrData.push_back(_wstring().assign(strTmp.begin(), strTmp.end()));
	}

	WriteFile(*m_pStrFileName);

	return S_OK;
}


CTerrainVertexData* CTerrainVertexData::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTerrainVertexData* pInstance = new CTerrainVertexData(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTerrainVertexData");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTerrainVertexData::Clone(void* pArg)
{
	CTerrainVertexData* pInstance = new CTerrainVertexData(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTerrainVertexData");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTerrainVertexData::Free()
{
	__super::Free();
}
