#include "TestLevelData.h"

#include "GameInstance.h"

CTestLevelData::CTestLevelData(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CDataObject{ pGraphic_Device }
{
}

CTestLevelData::CTestLevelData(const CTestLevelData& Prototype)
	: CDataObject{ Prototype }
{
}

HRESULT CTestLevelData::Initialize_Prototype()
{
	__super::Initialize_Prototype();

	return S_OK;
}

HRESULT CTestLevelData::Initialize(void* pArg)
{
	__super::Initialize(pArg);

	_wstring* pStrFileName = static_cast<_wstring*>(pArg);

	if (FAILED(ReadFile(*pStrFileName)))
		return E_FAIL;

	if (FAILED(SerializeData()))
		return E_FAIL;

	return S_OK;
}

HRESULT CTestLevelData::SerializeData()
{

	while (!m_listStrData.empty())
	{
		LEVELDATATYPE_DESC dataTmp;
		
		dataTmp.idx = stoi(m_listStrData.front());
		m_listStrData.pop_front();

		dataTmp.strObjectTag = m_listStrData.front();
		m_listStrData.pop_front();

		m_listStrData.front()[0] == 'T' ? dataTmp.bDirectAdd = true : dataTmp.bDirectAdd = false;
		m_listStrData.pop_front();
		
		dataTmp.vecPos.x = stoi(m_listStrData.front());
		m_listStrData.pop_front();

		dataTmp.vecPos.y = stoi(m_listStrData.front());
		m_listStrData.pop_front();

		dataTmp.vecPos.z = stoi(m_listStrData.front());
		m_listStrData.pop_front();

		m_listData.push_back(dataTmp);
	}
	return S_OK;
}

CTestLevelData* CTestLevelData::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CTestLevelData* pInstance = new CTestLevelData(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CTestLevelData");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CGameObject* CTestLevelData::Clone(void* pArg)
{
	CTestLevelData* pInstance = new CTestLevelData(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Cloned : CTestLevelData");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CTestLevelData::Free()
{
	__super::Free();
}
