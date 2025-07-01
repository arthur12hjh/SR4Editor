#pragma once

#include "Client_Defines.h"
#include "Level.h"

NS_BEGIN(Client)

class CLevel_Editor final : public CLevel
{
private:
	CLevel_Editor(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevelID);
	virtual ~CLevel_Editor() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Layer_Data(const _wstring& strLayerTag);
	HRESULT Ready_Layer_BackGround(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Camera(const _wstring& strLayerTag);

public:
	static CLevel_Editor* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevelID);
	virtual void Free() override;
};

NS_END