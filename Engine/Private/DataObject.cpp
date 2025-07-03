#include "DataObject.h"
#include "GameInstance.h"

CDataObject::CDataObject(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CGameObject{ pGraphic_Device}
{
}

CDataObject::CDataObject(const CDataObject& Prototype)
	: CGameObject{ Prototype }
{
}

HRESULT CDataObject::Initialize_Prototype()
{
	return S_OK;
}

HRESULT CDataObject::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CDataObject::ReadFile(const _wstring& strFileTag)
{
	m_pGameInstance->ReadFile(&m_listStrData, strFileTag);
	// for (auto iter : m_listStrData)
	// {
	// 	const WCHAR* debugStr = iter.c_str();
	// 	OutputDebugString(debugStr);
	// }

	return S_OK;
}

void CDataObject::WriteFile(const _wstring& strFileTag)
{
	m_pGameInstance->WriteFile(&m_listStrData, strFileTag);
}

void CDataObject::Free()
{
	__super::Free();
}
