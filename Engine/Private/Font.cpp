#include "Font.h"

CFont::CFont(LPDIRECT3DDEVICE9 pGraphic_Device)
	: CComponent{ pGraphic_Device }
{
	Safe_AddRef(m_pGraphic_Device);
}

CFont::CFont(const CFont& Prototype)
	: CComponent{ Prototype }
{
}

HRESULT CFont::Initialize_Prototype()
{

	return S_OK;
}

HRESULT CFont::Initialize(void* pArg)
{
	return S_OK;
}

HRESULT CFont::Font_Setting(_uint Height, LPCWSTR Write, _float fX, _float fY, _float fSizeX, _float fSizeY, int Alignment)
{
	m_iHeight = Height;
	m_strWrite = Write;

	D3DXCreateFont(m_pGraphic_Device, m_iHeight, 0, FW_NORMAL, 1,
		FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, L"LiberationSans", &m_pFont);

	D3DXCreateSprite(m_pGraphic_Device, &m_pTextSprite);

	m_lLeft = long(fX);
	m_lTop = long(fY);
	m_lRight = long(fSizeX);
	m_Bottom = long(fSizeY);

	/*rcPos = { m_lLeft,m_lTop,m_lRight,m_Bottom };*/
	rcPos = { long(m_lLeft - m_lRight * 0.5f), long(m_lTop - m_Bottom * 0.5f), long(m_lLeft + m_lRight * 0.5f), long(m_lTop + m_Bottom * 0.5f) };

	switch (Alignment)
	{
	case 0:
		m_lFormat = DT_LEFT;
		break;
	case 1:
		m_lFormat = DT_CENTER;
		break;
	case 2:
		m_lFormat = DT_RIGHT;
		break;
	}

	return S_OK;
}

HRESULT CFont::Font_Setting(_uint Height, _float Write, _float fX, _float fY, _float fSizeX, _float fSizeY, int Alignment)
{
	m_iHeight = Height;
	m_fWrite = Write;

	wstr =  to_wstring(m_fWrite);

	m_strWrite = wstr.c_str();

	D3DXCreateFont(m_pGraphic_Device, m_iHeight, 0, FW_NORMAL, 1,
		FALSE, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY,
		DEFAULT_PITCH | FF_DONTCARE, L"LiberationSans", &m_pFont);

	D3DXCreateSprite(m_pGraphic_Device, &m_pTextSprite);

	m_lLeft = long(fX);
	m_lTop = long(fY);
	m_lRight = long(fSizeX);
	m_Bottom = long(fSizeY);

	rcPos = { m_lLeft,m_lTop,m_lRight,m_Bottom };
	rcPos = { long(m_lLeft - m_lRight * 0.5f), long(m_lTop - m_Bottom * 0.5f), long(m_lLeft + m_lRight * 0.5f), long(m_lTop + m_Bottom * 0.5f) };

	return S_OK;
}

void CFont::Font_Move(_float Left, _float Top)
{
	rcPos = { long(Left),long(Top),m_lRight, m_Bottom };
}

HRESULT CFont::Render()
{
	m_pTextSprite->Begin(D3DXSPRITE_ALPHABLEND | D3DXSPRITE_SORT_TEXTURE);

	m_pFont->DrawTextW(m_pTextSprite, m_strWrite, -1,
		&rcPos, m_lFormat, D3DCOLOR_ARGB(255, 255, 255, 255));

	m_pTextSprite->End();

	return S_OK;
}

CFont* CFont::Create(LPDIRECT3DDEVICE9 pGraphic_Device)
{
	CFont* pInstance = new CFont(pGraphic_Device);

	if (FAILED(pInstance->Initialize_Prototype()))
	{
		MSG_BOX("Failed to Created : CFont_Prototype");
		Safe_Release(pInstance);
	}

	return pInstance;
}

CFont* CFont::Clone(void* pArg)
{
	CFont* pInstance = new CFont(*this);

	if (FAILED(pInstance->Initialize(pArg)))
	{
		MSG_BOX("Failed to Created : CFont_Clone");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFont::Free()
{
	__super::Free();

	Safe_Release(m_pFont);
	Safe_Release(m_pTextSprite);
	Safe_Release(m_pGraphic_Device);
}
