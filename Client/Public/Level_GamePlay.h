#pragma once

#include "Client_Defines.h"
#include "Level.h"

NS_BEGIN(Client)

class CLevel_GamePlay final : public CLevel
{
private:
	CLevel_GamePlay(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevelID);
	virtual ~CLevel_GamePlay() = default;

public:
	virtual HRESULT Initialize() override;
	virtual void Update(_float fTimeDelta) override;
	virtual HRESULT Render() override;

private:
	HRESULT Ready_Events();
	HRESULT Ready_Lights();
	HRESULT Ready_Sounds();
	HRESULT Ready_Layer_Data(const _wstring& strLayerTag);
	HRESULT Ready_Layer_BackGround(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Camera(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Player(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Monster(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Effect(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Player_AttackRange_Cube(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Monster_AttackRange_Cube(const _wstring& strLayerTag);

	HRESULT Ready_Layer_HpBar(const _wstring& strLayerTag);
	HRESULT Ready_Layer_Inven(const _wstring& strLayerTag);

private:
	HRESULT SetEffectEvent();

private:
	_float		m_fAddTimeDelta = {};
	

public:
	static CLevel_GamePlay* Create(LPDIRECT3DDEVICE9 pGraphic_Device, LEVEL eLevelID);
	virtual void Free() override;
};

NS_END