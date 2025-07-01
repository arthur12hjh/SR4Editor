#pragma once

#include "Component.h"

NS_BEGIN(Engine)

class ENGINE_DLL CFont final : public CComponent
{
public:
	typedef struct tagFontDesc
	{
		_uint		Height{};		// 글자 높이
		_uint		Weight{};		// 글자 굵기 타입 재정의 되어있음 0~9
		LPCWSTR		Write{};		// 출력 할 글
	}FONT_DESC;

private:
	CFont(LPDIRECT3DDEVICE9 pGraphic_Device);
	CFont(const CFont& Prototype);
	virtual ~CFont() = default;

public:
	virtual HRESULT Initialize_Prototype() override;
	virtual HRESULT Initialize(void* pArg) override;
	HRESULT Font_Setting(_uint Height, LPCWSTR Write, _float fX, _float fY, _float fSizeX, _float fSizeY, int Alignment = 0);
	HRESULT Font_Setting(_uint Height, _float Write, _float fX, _float fY, _float fSizeX, _float fSizeY, int Alignment = 0);
	void Font_Move(_float Left, _float Top);
	HRESULT Render();

private:
	ID3DXFont* m_pFont = { nullptr };
	ID3DXSprite* m_pTextSprite = { nullptr };

	_uint			m_iHeight{};
	LPCWSTR			m_strWrite{};
	_wstring		wstr{};
	_float			m_fWrite{};
	_ulong			m_lFormat{};
	RECT			rcPos{};
	_long			m_lLeft{}, m_lTop{}, m_lRight{}, m_Bottom{};

public:
	static CFont* Create(LPDIRECT3DDEVICE9 pGraphic_Device);
	virtual CFont* Clone(void* pArg);
	virtual void Free() override;
};

NS_END