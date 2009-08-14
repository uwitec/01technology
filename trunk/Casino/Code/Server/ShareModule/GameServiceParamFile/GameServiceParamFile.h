#pragma once

struct GameServiceParam
{
	TCHAR							m_szCenterServerAddr[16];			//CenterServer
	TCHAR							m_szLoaderServerPass[1024];			//服务密码
	WORD							m_wServerID;						//服务ID
	WORD							m_wGameNetPort;						//游戏端口
	TCHAR							m_szGameNetAddr[16];				//游戏地址
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
