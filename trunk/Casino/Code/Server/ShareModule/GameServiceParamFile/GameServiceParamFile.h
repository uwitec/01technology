#pragma once

struct GameServiceParam
{
	TCHAR							m_szCenterServerAddr[16];			//CenterServer
	TCHAR							m_szLoaderServerPass[1024];			//��������
	WORD							m_wServerID;						//����ID
	WORD							m_wGameNetPort;						//��Ϸ�˿�
	TCHAR							m_szGameNetAddr[16];				//��Ϸ��ַ
};
typedef CArray<GameServiceParam> GameServiceParamArray;

class CGameServiceParamFile
{
public:
	CGameServiceParamFile(void);
	~CGameServiceParamFile(void);

	bool Load(const char* szFileName, GameServiceParamArray& array);
	bool Save(const char* szFileName, GameServiceParamArray& array);

};
