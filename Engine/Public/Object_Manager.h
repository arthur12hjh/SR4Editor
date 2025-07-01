#pragma once

#include "Base.h"

/* 레벨별로 사용해야 할 객체들을 분리하여 보관한다. */

NS_BEGIN(Engine)

class CObject_Manager final : public CBase
{
private:
	CObject_Manager();
	virtual ~CObject_Manager() = default;

public:
	class CComponent* Get_Component(_uint iLevelIndex, const _wstring& strLayerTag, const _wstring& strComponentTag, _uint iIndex = 0);
	class CGameObject* Get_GameObject(_uint iLevelIndex, const _wstring& strLayerTag, _uint iIndex = 0);
public:
	HRESULT Initialize(_uint iNumLevels, HWND hWnd);

	/* 원형을 찾고 복제하여 레이어에 추가한다. */
	HRESULT Add_GameObject_ToLayer(_uint iPrototypeLevelIndex, const _wstring& strPrototypeTag, _uint iLayerLevelIndex, const _wstring& strLayerTag, void* pArg);

	HRESULT Add_List(class CGameObject* pUIObject);
	HRESULT Remove_List(class CGameObject* pUIObject);

	class CGameObject* Find_MaxZBuffer();

	void Priority_Update(_float fTimeDelta);
	void Update(_float fTimeDelta);
	void Late_Update(_float fTimeDelta);
	void Clear(_uint iLevelIndex);
	void Clear_DeadObj();

public:
	_uint	Get_ZBuffer() { return m_iUIZbuffer; }
	_uint	Get_Count() { return m_iUICount; }

	void	Set_ZBuffer(_uint i = 1) { m_iUIZbuffer += i; }
	void	Set_Count(_uint i = 1) { m_iUICount += i; }

	class CLayer* Find_Layer(_uint iLayerLevelIndex, const _wstring& strLayerTag);
private:
	_uint								m_iUICount{};
	_uint								m_iUIZbuffer{};
	HWND								m_hWnd = {};
	_uint								m_iNumLevels = {};
	map<const _wstring, class CLayer*>* m_pLayers = { nullptr };
	class CGameInstance*				m_pGameInstance = { nullptr };

	list<class CGameObject*>			m_pUIObjects;

public:
	static CObject_Manager* Create(_uint iNumLevels, HWND hWnd);
	virtual void Free() override;

};

NS_END