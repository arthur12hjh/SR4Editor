#pragma once

#include "GameObject.h"

NS_BEGIN(Engine)

class ENGINE_DLL CDataObject abstract : public CGameObject
{
	protected:
	CDataObject(LPDIRECT3DDEVICE9 pGraphic_Device);
	CDataObject(const CDataObject& Prototype);
	virtual ~CDataObject() = default;

public:
	virtual HRESULT Initialize_Prototype();
	virtual HRESULT Initialize(void* pArg);

protected:
	HRESULT ReadFile(const _wstring& strFileTag);
	void WriteFile(const _wstring& strFileTag);


protected:
	list<_wstring>		m_listStrData = {};
	_wstring*			m_pStrFileName = { nullptr };

public:
	virtual CGameObject* Clone(void* pArg) = 0;
	virtual void Free() override;

};

NS_END