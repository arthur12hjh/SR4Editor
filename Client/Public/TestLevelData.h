#pragma once

#include "Client_Defines.h"
#include "DataObject.h"

NS_BEGIN(Client)

class CTestLevelData final : public CDataObject
{
public:
	typedef struct LevelDataTypeDesc
	{
		_uint idx;
		_wstring strObjectTag;
		_bool bDirectAdd = { true };
		_float3 vecPos;

	}LEVELDATATYPE_DESC;

private:
	CTestLevelData(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTestLevelData(const CTestLevelData& Prototype);
	virtual ~CTestLevelData() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg);

private:
	HRESULT SerializeData();

private:
	list<LEVELDATATYPE_DESC> m_listData = {};

public:
	static CTestLevelData* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END