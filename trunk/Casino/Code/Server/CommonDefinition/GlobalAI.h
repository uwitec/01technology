#pragma	  once
#pragma pack(push)
#pragma pack(1)
//�˹�����ͨ��
class CAIUtil
{
public:

	//�жϵ������
	static bool	IsAIUser(TCHAR *szAccount)
	{
		static const TCHAR* szAIAccount = TEXT("computer");
		if(_tcslen(szAccount) < 8)
			return false;
		if(_tcsnicmp(szAccount + 1, szAIAccount, _tcslen(szAIAccount)) == 0)
			return true;
		else
			return false;
	}
	//�жϵ������
	static bool	IsAIUser(DWORD  dwUserState)
	{
		static const DWORD  dwAIUserStateMark = 0x1;
		return (dwAIUserStateMark & dwUserState) != 0 ? true : false;
	}
	//��鼸��
	static bool DetectionProbability(BYTE r)
	{
		if((rand() % 100) <= r)
			return true;
		else
			return false;
	}
};

#pragma pack(pop)