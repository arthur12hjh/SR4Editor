#pragma once

#include "Prototype_Manager.h"

NS_BEGIN(Engine)

class ENGINE_DLL CGameInstance final : public CBase
{
	DECLARE_SINGLETON(CGameInstance);

private:
	CGameInstance();
	virtual ~CGameInstance() = default;

public:
	HRESULT Initialize_Engine(const ENGINE_DESC& EngineDesc, LPDIRECT3DDEVICE9* ppOut);
	void Update_Engine(_float fTimeDelta);
	HRESULT Draw();
	void Clear_Resources(_uint iLevelIndex);

	_float Random_Normal();
	_float Random(_float fMin, _float fMax);

#pragma region GRAPHIC_DEVICE
public:
	void Render_Begin(D3DXCOLOR Color);
	void Render_End(HWND hWnd = nullptr);
#pragma endregion

#pragma region TIMER_MANAGER
	_float	Get_TimeDelta(const _wstring& strTimerTag);
	HRESULT	Add_Timer(const _wstring& strTimerTag);
	void	Compute_TimeDelta(const _wstring& strTimerTag);
#pragma endregion

#pragma region LEVEL_MANAGER
public:
	HRESULT Change_Level(class CLevel* pNewLevel);
#pragma endregion

#pragma region PROTOTYPE_MANAGER
	HRESULT Add_Prototype(_uint iLevelIndex, const _wstring& strPrototypeTag, class CBase* pPrototype);
	class CBase* Clone_Prototype(PROTOTYPE ePrototype, _uint iLevelIndex, const _wstring& strPrototypeTag, void* pArg = nullptr);
#pragma endregion

#pragma region OBJECT_MANAGER
	CComponent* Get_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex = 0);
	class CGameObject* Get_GameObject(_uint iLevelIndex, const _wstring& strLayerTag, _uint iIndex = 0);
	HRESULT Add_GameObject_ToLayer(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLayerLevelIndex, const _wstring& strLayerTag, void* pArg = nullptr);
	class CGameObject* Find_MaxZBuffer();

	HRESULT	Add_List(class CGameObject* pUIObject);
	HRESULT	Remove_List(class CGameObject* pUIObject);

	void	Set_UICount(_uint i = 1);
	void	Set_ZBuffer(_uint i = 1);
	_uint	Get_UICount();
	_uint	Get_ZBuffer();
#pragma endregion

#pragma region Event_Manager
	HRESULT AddEvent(const _wstring& strEventTag, void* pArg);
	HRESULT SubscribeEvent(const _wstring& strEventTag, class CEventListenerInfo* refInfo);
	HRESULT UnSubscribeEvent(const _wstring& strEventTag, const class CGameObject* listener);

	HRESULT RemoveEvent(const _wstring& strEventTag);
	HRESULT RemoveListener(const class CGameObject* listener);
	HRESULT RemoveSearchedListener(const _wstring& strEventTag, const class CGameObject* listener);

	HRESULT RemoveEventsAll();
	HRESULT RemoveListenerAll();

	void Set_ActiveEvent(const _wstring& strEventTag, _bool isActive);
	bool Get_ActiveEvent(const _wstring& strEventTag);

	void CallBackEvent(const _wstring& strEventTag);
#pragma endregion

#pragma region RENDERER
	HRESULT Add_RenderGroup(RENDER eRenderGroup, class CGameObject* pRenderObject);
#pragma endregion


#pragma region PICKING
	void Transform_Picking_ToLocalSpace(const _float4x4* pWorldMatrixInverse);
	_bool Picking_InWorldSpace(const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut);
	_bool Picking_InLocalSpace(const _float3& vPointA, const _float3& vPointB, const _float3& vPointC, _float3* pOut);
#pragma endregion

#pragma region KEY_MANAGER
public:
	_bool Key_Pressing(_uint _ikey);
	_bool Key_Up(_uint _ikey);
	_bool Key_Down(_uint _ikey);
	_bool Key_Released(_uint _ikey);
#pragma endregion

#pragma region COLLISION_MANAGER

public:
	void AddCollisionLayer(_uint iLayerLevelIndexSrc, const _wstring& strLayerTagSrc, _uint iLayerLevelIndexDst, const _wstring& strLayerTagDst);

#pragma endregion

#pragma region MOUSE_MANAGER


#pragma endregion

#pragma region UTILITIES

	_float GetRandomFloat(_float lowBound, _float highBound);
	void   GetRandomVector(_float3* pOut, _float3* pMin, _float3* pMax);
	WCHAR* CharToWCHAR(char* pChar);
	char* WCHARtoChar(WCHAR* pWch);

#pragma endregion

#pragma region SOUND_MANAGER

	void Manager_PlaySound(const TCHAR* pSoundKey, CHANNELID eID, float fVolume);
	void Manager_PlayBGM(const TCHAR* pSoundKey, float fVolume);
	void Manager_StopSound(CHANNELID eID);
	void Manager_StopAll();
	void Manager_SetChannelVolume(CHANNELID eID, float fVolume);

#pragma endregion

#pragma region FILEREADER

	HRESULT ReadFile(list<_wstring>* strDataList, const _wstring& strFileTag);
	HRESULT WriteFile(list<_wstring>* strDataList, const _wstring& strFileTag);

#pragma endregion

private:
	class CGraphic_Device*			m_pGraphic_Device = { nullptr };
	class CTimer_Manager*			m_pTimer_Manager = { nullptr };
	class CLevel_Manager*			m_pLevel_Manager = { nullptr };
	class CPrototype_Manager*		m_pPrototype_Manager = { nullptr };
	class CObject_Manager*			m_pObject_Manager = { nullptr };
	class CEvent_Manager*			m_pEvent_Manager = { nullptr };
	class CCollision_Manager*		m_pCollision_Manager = { nullptr };
	class CRenderer*				m_pRenderer = { nullptr };
	class CPicking*					m_pPicking = { nullptr };
	class CKey_Manager*				m_pKey_Manager = { nullptr };
	class CMouse_Manager*			m_pMouse_Manager = { nullptr };
	class CUtilities*				m_pUtilities = { nullptr };
	class CSound_Manager*			m_pSound_Manager = { nullptr };
	class CFileReader*				m_pFileReader = { nullptr };

public:
	void Release_Engine();
	virtual void Free() override;
};

NS_END