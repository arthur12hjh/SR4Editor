#include "GameInstance.h"

#include "Prototype_Manager.h"
#include "Object_Manager.h"
#include "Graphic_Device.h"
#include "Level_Manager.h"
#include "Timer_Manager.h"
#include "Collision_Manager.h"
#include "Event_Manager.h"
#include "Renderer.h"
#include "Key_Manager.h"
#include "Picking.h"
#include "Mouse_Manager.h"
#include "Utilities.h"
#include "Sound_Manager.h"
#include "FileReader.h"

#include "Event.h"
#include "Layer.h"

IMPLEMENT_SINGLETON(CGameInstance)

CGameInstance::CGameInstance()
{
}

HRESULT CGameInstance::Initialize_Engine(const ENGINE_DESC& EngineDesc, LPDIRECT3DDEVICE9* ppOut)
{
	m_pGraphic_Device = CGraphic_Device::Create(EngineDesc.hWnd, EngineDesc.eWindowMode, EngineDesc.iWinSizeX, EngineDesc.iWinSizeY, ppOut);
	if (nullptr == m_pGraphic_Device)
		return E_FAIL;

	m_pTimer_Manager = CTimer_Manager::Create();
	if (nullptr == m_pTimer_Manager)
		return E_FAIL;
		
	m_pPrototype_Manager = CPrototype_Manager::Create(EngineDesc.iNumLevels);
	if (nullptr == m_pPrototype_Manager)
		return E_FAIL;

	m_pObject_Manager = CObject_Manager::Create(EngineDesc.iNumLevels, EngineDesc.hWnd);
	if (nullptr == m_pObject_Manager)
	{
		MSG_BOX("Failed to create Object Manager!");
		return E_FAIL;
	}

	m_pEvent_Manager = CEvent_Manager::Create();
	if (nullptr == m_pEvent_Manager)
		return E_FAIL;

	m_pLevel_Manager = CLevel_Manager::Create();
	if (nullptr == m_pLevel_Manager)
		return E_FAIL;
	
	m_pRenderer = CRenderer::Create(*ppOut);
	if (nullptr == m_pRenderer)
		return E_FAIL;

	m_pPicking = CPicking::Create(*ppOut, EngineDesc.hWnd);
	if (nullptr == m_pPicking)
		return E_FAIL;

	m_pCollision_Manager = CCollision_Manager::Create();
	if (nullptr == m_pCollision_Manager)
		return E_FAIL;

	m_pKey_Manager = CKey_Manager::Create();
	if (nullptr == m_pRenderer)
		return E_FAIL;


	m_pMouse_Manager = CMouse_Manager::Create(EngineDesc.hWnd);
	if (nullptr == m_pMouse_Manager)
		return E_FAIL;

	m_pUtilities = CUtilities::Create();
	if (nullptr == m_pUtilities)
		return E_FAIL;

	m_pSound_Manager = CSound_Manager::Create();
	if (nullptr == m_pSound_Manager)
		return E_FAIL;

	m_pFileReader = CFileReader::Create();
	if (nullptr == m_pFileReader)
		return E_FAIL;

	return S_OK;
}

void CGameInstance::Update_Engine(_float fTimeDelta)
{
	m_pObject_Manager->Priority_Update(fTimeDelta);

	m_pPicking->Update();

	m_pObject_Manager->Update(fTimeDelta);

	m_pMouse_Manager->Update(fTimeDelta);

	m_pKey_Manager->Update();

	m_pObject_Manager->Late_Update(fTimeDelta);

	m_pCollision_Manager->Late_Update(fTimeDelta);

	m_pObject_Manager->Clear_DeadObj();

	m_pLevel_Manager->Update(fTimeDelta);
}

HRESULT CGameInstance::Draw()
{
	m_pRenderer->Render();

	m_pLevel_Manager->Render();

	return S_OK;
}

void CGameInstance::Clear_Resources(_uint iLevelIndex)
{
	m_pPrototype_Manager->Clear(iLevelIndex);
	m_pObject_Manager->Clear(iLevelIndex);
}

_float CGameInstance::Random_Normal()
{
	return static_cast<_float>(rand()) / RAND_MAX;	
}

_float CGameInstance::Random(_float fMin, _float fMax)
{
	return fMin + Random_Normal() * (fMax - fMin);	
}

#pragma region GRAPHIC_DEVICE

void CGameInstance::Render_Begin(D3DXCOLOR Color)
{
	m_pGraphic_Device->Render_Begin(Color);
}

void CGameInstance::Render_End(HWND hWnd)
{
	m_pGraphic_Device->Render_End(hWnd);
}

#pragma endregion

#pragma region TIMER_MANAGER

_float CGameInstance::Get_TimeDelta(const _wstring& strTimerTag)
{
	return m_pTimer_Manager->Get_TimeDelta(strTimerTag);
}

HRESULT CGameInstance::Add_Timer(const _wstring& strTimerTag)
{
	return m_pTimer_Manager->Add_Timer(strTimerTag);
}

void CGameInstance::Compute_TimeDelta(const _wstring& strTimerTag)
{
	m_pTimer_Manager->Compute_TimeDelta(strTimerTag);
}

#pragma endregion

#pragma region LEVEL_MANAGER

HRESULT CGameInstance::Change_Level(CLevel* pNewLevel)
{
	return m_pLevel_Manager->Change_Level(pNewLevel);
}

#pragma endregion

#pragma region PROTOTYPE_MANAGER

HRESULT CGameInstance::Add_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag, CBase* pPrototype)
{
	return m_pPrototype_Manager->Add_Prototype(iLevelIndex, strPrototypeTag, pPrototype);
}

CBase* CGameInstance::Clone_Prototype(PROTOTYPE ePrototype, _uint iLevelIndex, const _wstring& strPrototypeTag, void* pArg)
{
	return m_pPrototype_Manager->Clone_Prototype(ePrototype, iLevelIndex, strPrototypeTag, pArg);;
}

#pragma endregion

#pragma region OBJECT_MANAGER

CComponent* CGameInstance::Get_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex)
{
	return m_pObject_Manager->Get_Component(iLevelIndex, strLayerTag, strComponentTag, iIndex);	
}

CGameObject* CGameInstance::Get_GameObject(_uint iLevelIndex, const _wstring& strLayerTag, _uint iIndex)
{
	return m_pObject_Manager->Get_GameObject(iLevelIndex, strLayerTag, iIndex);
}

HRESULT CGameInstance::Add_GameObject_ToLayer(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLayerLevelIndex, const _wstring& strLayerTag, void* pArg)
{
	return m_pObject_Manager->Add_GameObject_ToLayer(iPrototypeLevelIndex, strPrototypeTag, iLayerLevelIndex, strLayerTag, pArg);
}

CGameObject* CGameInstance::Find_MaxZBuffer()
{
	return m_pObject_Manager->Find_MaxZBuffer();
}

HRESULT CGameInstance::Add_List(CGameObject* pUIObject)
{
	return m_pObject_Manager->Add_List(pUIObject);
}

HRESULT CGameInstance::Remove_List(CGameObject* pUIObject)
{
	return m_pObject_Manager->Remove_List(pUIObject);
}

void CGameInstance::Set_UICount(_uint i)
{
	m_pObject_Manager->Set_Count(i);
}

void CGameInstance::Set_ZBuffer(_uint i)
{
	m_pObject_Manager->Set_ZBuffer(i);
}

_uint CGameInstance::Get_UICount()
{
	return m_pObject_Manager->Get_Count();
}

_uint CGameInstance::Get_ZBuffer()
{
	return m_pObject_Manager->Get_ZBuffer();
}

#pragma endregion

#pragma region EVENT_MANAGER

HRESULT CGameInstance::AddEvent(const _wstring& strEventTag, void* pArg)
{
	return m_pEvent_Manager->AddEvent(strEventTag, pArg);
}

HRESULT CGameInstance::SubscribeEvent(const _wstring& strEventTag, CEventListenerInfo* refInfo)
{
	return m_pEvent_Manager->SubscribeEvent(strEventTag, refInfo);
}

HRESULT CGameInstance::UnSubscribeEvent(const _wstring& strEventTag, const CGameObject* listener)
{
	return m_pEvent_Manager->UnSubscribeEvent(strEventTag, listener);
}

HRESULT CGameInstance::RemoveEvent(const _wstring& strEventTag)
{
	return m_pEvent_Manager->RemoveEvent(strEventTag);
}

HRESULT CGameInstance::RemoveListener(const CGameObject* listener)
{
	return m_pEvent_Manager->RemoveListener(listener);
}

HRESULT CGameInstance::RemoveSearchedListener(const _wstring& strEventTag, const CGameObject* listener)
{
	return m_pEvent_Manager->RemoveSearchedListener(strEventTag, listener);
}

HRESULT CGameInstance::RemoveEventsAll()
{
	return m_pEvent_Manager->RemoveEventsAll();
}

HRESULT CGameInstance::RemoveListenerAll()
{
	return m_pEvent_Manager->RemoveListenerAll();
}

void CGameInstance::Set_ActiveEvent(const _wstring& strEventTag, _bool isActive)
{
	m_pEvent_Manager->Set_ActiveEvent(strEventTag, isActive);
}

bool CGameInstance::Get_ActiveEvent(const _wstring& strEventTag)
{
	return m_pEvent_Manager->Get_ActiveEvent(strEventTag);
}

void CGameInstance::CallBackEvent(const _wstring& strEventTag)
{
	m_pEvent_Manager->CallBackEvent(strEventTag);
}

#pragma endregion

#pragma region RENDERER

HRESULT CGameInstance::Add_RenderGroup(RENDER eRenderGroup, CGameObject* pRenderObject)
{
	return m_pRenderer->Add_RenderGroup(eRenderGroup, pRenderObject);
}

#pragma endregion

#pragma region PICKING

void CGameInstance::Transform_Picking_ToLocalSpace(const _float4x4* pWorldMatrixInverse)
{
	m_pPicking->Transform_ToLocalSpace(pWorldMatrixInverse);
}

_bool CGameInstance::Picking_InWorldSpace(const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut)
{
	return m_pPicking->Picking_InWorldSpace(vPointA, vPointB, vPointC, pOut);
}

_bool CGameInstance::Picking_InLocalSpace(const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut)
{
	return m_pPicking->Picking_InLocalSpace(vPointA, vPointB, vPointC, pOut);
}

#pragma endregion

#pragma region KEY_MANAGER

_bool CGameInstance::Key_Pressing(_uint _ikey)
{
	return m_pKey_Manager->Key_Pressing(_ikey);
}

_bool CGameInstance::Key_Up(_uint _ikey)
{
	return m_pKey_Manager->Key_Up(_ikey);
}

_bool CGameInstance::Key_Down(_uint _ikey)
{
	return m_pKey_Manager->Key_Down(_ikey);
}

_bool CGameInstance::Key_Released(_uint _ikey)
{
	return m_pKey_Manager->Key_Released(_ikey);
}

#pragma endregion

#pragma region COLLISION_MANAGER

void CGameInstance::AddCollisionLayer(_uint iLayerLevelIndexSrc, const _wstring& strLayerTagSrc, _uint iLayerLevelIndexDst, const _wstring& strLayerTagDst)
{
	m_pCollision_Manager->Set_Collision_Layer(m_pObject_Manager->Find_Layer(iLayerLevelIndexSrc, strLayerTagSrc), 
		m_pObject_Manager->Find_Layer(iLayerLevelIndexDst, strLayerTagDst));
}
#pragma endregion

#pragma region MOUSE_MANAGER


#pragma endregion

#pragma region UTILITIES

_float CGameInstance::GetRandomFloat(_float lowBound, _float highBound)
{
	return m_pUtilities->GetRandomFloat(lowBound, highBound);
}
void CGameInstance::GetRandomVector(_float3* pOut, _float3* pMin, _float3* pMax)
{
	m_pUtilities->GetRandomVector(pOut, pMin, pMax);
}

WCHAR* CGameInstance::CharToWCHAR(char* pChar)
{
	return m_pUtilities->CharToWCHAR(pChar);
}

char* CGameInstance::WCHARtoChar(WCHAR* pWch)
{
	return m_pUtilities->WCHARtoChar(pWch);
}

#pragma endregion

#pragma region SOUND_MANAGER

void CGameInstance::Manager_PlaySound(const TCHAR* pSoundKey, CHANNELID eID, float fVolume)
{
	m_pSound_Manager->Manager_PlaySound(pSoundKey, eID, fVolume);
}

void CGameInstance::Manager_PlayBGM(const TCHAR* pSoundKey, float fVolume)
{
	m_pSound_Manager->Manager_PlayBGM(pSoundKey, fVolume);
}

void CGameInstance::Manager_StopSound(CHANNELID eID)
{
	m_pSound_Manager->Manager_StopSound(eID);
}

void CGameInstance::Manager_StopAll()
{
	m_pSound_Manager->Manager_StopAll();
}

void CGameInstance::Manager_SetChannelVolume(CHANNELID eID, float fVolume)
{
	m_pSound_Manager->Manager_SetChannelVolume(eID, fVolume);
}

#pragma endregion


#pragma region FILEREADER

HRESULT CGameInstance::ReadFile(list<_wstring>* strDataList, const _wstring& strFileTag)
{
	return m_pFileReader->ReadFile(strDataList, strFileTag);
}

HRESULT CGameInstance::WriteFile(list<_wstring>* strDataList, const _wstring& strFileTag)
{
	return m_pFileReader->WriteFile(strDataList, strFileTag);
}

#pragma endregion


void CGameInstance::Release_Engine()
{
	DestroyInstance();

	Safe_Release(m_pMouse_Manager);
	Safe_Release(m_pKey_Manager);
	Safe_Release(m_pPicking);
	Safe_Release(m_pTimer_Manager);
	Safe_Release(m_pCollision_Manager);
	Safe_Release(m_pRenderer);
	Safe_Release(m_pPrototype_Manager);
	Safe_Release(m_pObject_Manager);
	Safe_Release(m_pEvent_Manager);
	Safe_Release(m_pLevel_Manager);
	Safe_Release(m_pCollision_Manager);
	Safe_Release(m_pGraphic_Device);
	Safe_Release(m_pKey_Manager);
	Safe_Release(m_pUtilities);
	Safe_Release(m_pSound_Manager);
	Safe_Release(m_pFileReader);

}

void CGameInstance::Free()
{
	__super::Free();

	
}
