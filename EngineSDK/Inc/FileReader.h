#pragma once

#include "Base.h"

NS_BEGIN(Engine)

class CFileReader : public CBase
{
private:
	CFileReader();
	virtual ~CFileReader() = default;

public:
	HRESULT Initialize();
	HRESULT ReadFile(list<_wstring>* strDataList, const _wstring& strFileTag);
	HRESULT WriteFile(list<_wstring>* strDataList, const _wstring& strFileTag);


public:
	static CFileReader* Create();
	virtual void Free();

};

NS_END