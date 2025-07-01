#include "Object_Manager.h"
#include "GameInstance.h"

#include "Layer.h"
#include "GameObject.h"
#include "UIObject.h"

CObject_Manager::CObject_Manager()
	: m_pGameInstance{ CGameInstance::GetInstance() }
{
	Safe_AddRef(m_pGameInstance);
}

CComponent* CObject_Manager::Get_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);
	if (nullptr == pLayer)
		return nullptr;

	return pLayer->Get_Component(strComponentTag, iIndex);
}

CGameObject* CObject_Manager::Get_GameObject(_uint iLevelIndex, const _wstring& strLayerTag, _uint iIndex)
{
	CLayer* pLayer = Find_Layer(iLevelIndex, strLayerTag);
	if (nullptr == pLayer)
		return nullptr;

	return pLayer->Get_GameObject(iIndex);
}

HRESULT CObject_Manager::Initialize(_uint iNumLevels, HWND hWnd)
{
	m_iNumLevels = iNumLevels;

	m_hWnd = hWnd;

	m_pLayers = new map<const _wstring, CLayer*>[iNumLevels];

	return S_OK;
}

HRESULT CObject_Manager::Add_GameObject_ToLayer(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLayerLevelIndex, const _wstring& strLayerTag, void* pArg)
{
	CGameObject* pGameObject = dynamic_cast<CGameObject*>(m_pGameInstance->Clone_Prototype(PROTOTYPE::GAMEOBJECT, iPrototypeLevelIndex, strPrototypeTag, pArg));
	if (nullptr == pGameObject)
		return E_FAIL;

	CLayer* pLayer = Find_Layer(iLayerLevelIndex, strLayerTag);
	if (nullptr == pLayer)
	{
		pLayer = CLayer::Create();

		pLayer->Add_GameObject(pGameObject);

		m_pLayers[iLayerLevelIndex].emplace(strLayerTag, pLayer);
	}
	else
		pLayer->Add_GameObject(pGameObject);

	return S_OK;
}

HRESULT CObject_Manager::Add_List(CGameObject* pUIObject)
{
	if (nullptr == pUIObject)
		return E_FAIL;

	m_pUIObjects.push_back(pUIObject);

	Safe_AddRef(pUIObject);

	return S_OK;
}

HRESULT CObject_Manager::Remove_List(CGameObject* pUIObject)
{
	if (nullptr == pUIObject)
		return E_FAIL;

	m_pUIObjects.remove(pUIObject);

	Safe_Release(pUIObject);

	return S_OK;
}

CGameObject* CObject_Manager::Find_MaxZBuffer()
{
	if (m_pUIObjects.empty())
		return nullptr;

	CGameObject* pSour = nullptr;

	POINT ptMouse;
	GetCursorPos(&ptMouse);
	ScreenToClient(m_hWnd, &ptMouse);

	for (auto UIObject : m_pUIObjects)
	{
		RECT* pUIRect = static_cast<CUIObject*>(UIObject)->Get_Pos();
		if (true == PtInRect(pUIRect, ptMouse))

		{
			if (nullptr == pSour || static_cast<CUIObject*>(pSour)->Get_ZBuffer() < static_cast<CUIObject*>(UIObject)->Get_ZBuffer())
				pSour = UIObject;
		}
	}

	if (nullptr == pSour)
		return nullptr;

	return pSour;
}

void CObject_Manager::Priority_Update(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
			Pair.second->Priority_Update(fTimeDelta);
	}
}

void CObject_Manager::Update(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
			Pair.second->Update(fTimeDelta);
	}
}

void CObject_Manager::Late_Update(_float fTimeDelta)
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
			Pair.second->Late_Update(fTimeDelta);
	}
}

void CObject_Manager::Clear(_uint iLevelIndex)
{
	for (auto& Pair : m_pLayers[iLevelIndex])
	{
		Pair.second->Clear();
		Safe_Release(Pair.second);
	}

	m_pLayers[iLevelIndex].clear();
}

void CObject_Manager::Clear_DeadObj()
{
	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
		{
			Pair.second->Clear_DeadObj();
		}
	}
}


CLayer* CObject_Manager::Find_Layer(_uint iLayerLevelIndex, const _wstring& strLayerTag)
{
	if (iLayerLevelIndex >= m_iNumLevels)
		return nullptr;

	auto	iter = m_pLayers[iLayerLevelIndex].find(strLayerTag);
	if (iter == m_pLayers[iLayerLevelIndex].end())
		return nullptr;

	return iter->second;
}

CObject_Manager* CObject_Manager::Create(_uint iNumLevels, HWND hWnd)
{
	CObject_Manager* pInstance = new CObject_Manager();

	if (FAILED(pInstance->Initialize(iNumLevels, hWnd)))
	{
		MSG_BOX("Failed to Created : CObject_Manager");
		Safe_Release(pInstance);
	}

	return pInstance;
}


void CObject_Manager::Free()
{
	__super::Free();

	Safe_Release(m_pGameInstance);

	for (size_t i = 0; i < m_iNumLevels; i++)
	{
		for (auto& Pair : m_pLayers[i])
			Safe_Release(Pair.second);

		m_pLayers[i].clear();
	}

	if (!m_pUIObjects.empty())
	{
		for (auto& UI : m_pUIObjects)
			Safe_Release(UI);
		m_pUIObjects.clear();
	}

	Safe_Delete_Array(m_pLayers);
}	
