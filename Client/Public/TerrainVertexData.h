#pragma once

#include "Client_Defines.h"
#include "DataObject.h"

NS_BEGIN(Engine)
class CVIBuffer_Terrain;
NS_END

NS_BEGIN(Client)

class CTerrainVertexData : public CDataObject
{
private:
	CTerrainVertexData(LPDIRECT3DDEVICE9 pGraphic_Device);
	CTerrainVertexData(const CTerrainVertexData& Prototype);
	virtual ~CTerrainVertexData() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg);

	list<Engine::TERRAINDATATYPE_DESC> Get_Data() { return m_listData; }
	void Set_Data(TERRAINDATATYPE_DESC dataDesc);
	HRESULT Save_Data();

	void Set_Terrain(CVIBuffer_Terrain* pVIBufferCom) { m_pTerrainVIBufferCom = pVIBufferCom; }


private:
	HRESULT SerializeData();
	
private:
	list<Engine::TERRAINDATATYPE_DESC> m_listData = {};
	CVIBuffer_Terrain* m_pTerrainVIBufferCom = { nullptr };

public:
	static CTerrainVertexData* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CGameObject* Clone(void* pArg) override;
	virtual void Free() override;
};

NS_END