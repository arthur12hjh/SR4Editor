#pragma once
#include "Base.h"

NS_BEGIN(Engine)
class CSound_Manager : public CBase
{
private:
	CSound_Manager() = default;
	virtual ~CSound_Manager() = default;

public:
	HRESULT Initialize();

public:
	void Manager_PlaySound(const TCHAR* pSoundKey, CHANNELID eID, float fVolume);
	void Manager_PlayBGM(const TCHAR* pSoundKey, float fVolume);
	void Manager_StopSound(CHANNELID eID);
	void Manager_StopAll();
	void Manager_SetChannelVolume(CHANNELID eID, float fVolume);

private:
	// ���� ���ҽ� ������ ���� ��ü 
	map<TCHAR*, FMOD::Sound*> m_mapSound;
	// FMOD_CHANNEL : ����ϰ� �ִ� ���带 ������ ��ü 
	FMOD::Channel* m_pChannelArr[ENUM_CLASS(CHANNELID::END)];
	// ���� ,ä�� ��ü �� ��ġ�� �����ϴ� ��ü 
	FMOD::System* m_pSystem;

private:
	void LoadSoundFile();

public:
	static	CSound_Manager* Create();
	virtual void			Free() override;

};
NS_END