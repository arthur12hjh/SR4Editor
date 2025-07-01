#include "FileReader.h"

#include <fstream>


CFileReader::CFileReader()
{
}

HRESULT CFileReader::Initialize()
{
	return S_OK;
}

HRESULT CFileReader::ReadFile(list<_wstring>* strDataList, const _wstring& strFileTag)
{
	string strStream;
	ifstream file(strFileTag);

	if (file.is_open())
	{
		while (!file.eof())
		{
			getline(file, strStream, ',');
			_wstring wstrTemp;
			wstrTemp.assign(strStream.begin(), strStream.end());
			wprintf(wstrTemp.c_str());

			int idx = -1;
			idx = wstrTemp.find('\n');
			if (idx != string::npos)
			{
				wstrTemp.substr(0, idx);
				strDataList->push_back(wstrTemp.substr(0, idx));
				if(idx != wstrTemp.size())
					strDataList->push_back(wstrTemp.substr(idx+1));
			}
			else
			{
				strDataList->push_back(wstrTemp);
			}
		}
	}



	return S_OK;
}

CFileReader* CFileReader::Create()
{
	CFileReader* pInstance = new CFileReader();

	if (FAILED(pInstance->Initialize()))
	{
		MSG_BOX(" Failed to Create : CFileReader");
		Safe_Release(pInstance);
	}

	return pInstance;
}

void CFileReader::Free()
{
	__super::Free();

}
