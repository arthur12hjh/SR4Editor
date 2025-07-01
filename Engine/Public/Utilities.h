#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class CUtilities final : public CBase
{
private:
	CUtilities();
	virtual ~CUtilities() = default;

public:
	HRESULT Initialize();

public:
	_float GetRandomFloat(_float lowBound, _float highBound);
	void   GetRandomVector(_float3* pOut, _float3* pMin, _float3* pMax);
	WCHAR* CharToWCHAR(char* pChar);
	char* WCHARtoChar(WCHAR* pWch);

public:
	static CUtilities* Create();
	virtual void Free()override;

};

NS_END