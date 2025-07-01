#include "Utilities.h"

CUtilities::CUtilities()
{
}

HRESULT CUtilities::Initialize()
{
	srand((unsigned)time(NULL));

	return S_OK;
}

_float CUtilities::GetRandomFloat(_float lowBound, _float highBound)
{
	if (lowBound >= highBound)
		return lowBound;

	_float f = (rand() % 10000) * 0.0001f;

	return (f * (highBound - lowBound)) + lowBound;
}

void CUtilities::GetRandomVector(_float3* pOut, _float3* pMin, _float3* pMax)
{
	pOut->x = GetRandomFloat(pMin->x, pMax->x);
	pOut->y = GetRandomFloat(pMin->y, pMax->y);
	pOut->z = GetRandomFloat(pMin->z, pMax->z);
}

WCHAR* CUtilities::CharToWCHAR(char* pChar)
{
	WCHAR* pWch = {};
	int iCharSize = MultiByteToWideChar(CP_ACP, 0, pChar, -1, NULL, NULL);
	pWch = new WCHAR[iCharSize];
	MultiByteToWideChar(CP_ACP, 0, pChar, strlen(pChar) + 1, pWch, iCharSize);

	return pWch;
}

char* CUtilities::WCHARtoChar(WCHAR* pWch)
{
	char* pStr = {};
	int iCharSize = WideCharToMultiByte(CP_ACP, 0, pWch, -1, NULL, 0, NULL, NULL);
	pStr = new char[iCharSize];
	WideCharToMultiByte(CP_ACP, 0, pWch, -1, pStr, iCharSize, 0, 0);

	return pStr;
}


CUtilities* CUtilities::Create()
{
	CUtilities* pInstance = new CUtilities();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX("Failed to Created : CUtilities");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CUtilities::Free()
{
	__super::Free();
}
