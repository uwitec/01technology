#ifndef GLOBAL_RIGHT_HEAD_FILE
#define GLOBAL_RIGHT_HEAD_FILE

//////////////////////////////////////////////////////////////////////////

#include "GlobalDef.h"

//////////////////////////////////////////////////////////////////////////

//Ȩ�޸�����
class CMasterRight
{
	//����Ȩ��
public:
	static const DWORD MR_CAN_ACCOUNT_NEW 		= 0x00000001;//�����ʺ��½�
	static const DWORD MR_CAN_ACCOUNT_SET 		= 0x00000002;//�����ʺ��趨
	static const DWORD MR_CAN_SUBACCOUNT_NEW	= 0x00000004;//���ʺ��½�
	static const DWORD MR_CAN_SUBACCOUNT_SET	= 0x00000008;//���ʺ��趨
	static const DWORD MR_CAN_CHART_VIEW 		= 0x00000010;//��Ϸ����鿴
	static const DWORD MR_CAN_BETHISTORY_VIEW	= 0x00000020;//��Ϸ��¼�鿴
	static const DWORD MR_CAN_BETMONITOR_VIEW	= 0x00000040;//�ֳ�Ͷע�鿴
	static const DWORD MR_CAN_OPERATION_VIEW	= 0x00000080;//������¼�鿴
	static const DWORD MR_CAN_MYSELFDATA_SET	= 0x00000100;//�����ʺ��趨
public:
	static bool CanAccountNew(DWORD dwMasterRight)//�����ʺ��½�
	{
		return (dwMasterRight & MR_CAN_ACCOUNT_NEW)!=0 ? false : true;
	}
	static bool CanAccountSet(DWORD dwMasterRight)//�����ʺ��趨
	{
		return (dwMasterRight & MR_CAN_ACCOUNT_SET)!=0 ? false : true;
	}
	static bool CanSubAccountNew(DWORD dwMasterRight)//���ʺ��½�
	{
		return (dwMasterRight & MR_CAN_SUBACCOUNT_NEW)!=0 ? false : true;
	}
	static bool CanSubAccountSet(DWORD dwMasterRight)//���ʺ��趨
	{
		return (dwMasterRight & MR_CAN_SUBACCOUNT_SET)!=0 ? false : true;
	}
	static bool CanChartView(DWORD dwMasterRight)//��Ϸ����鿴
	{
		return (dwMasterRight & MR_CAN_CHART_VIEW)!=0 ? false : true;
	}
	static bool CanBetHistoryView(DWORD dwMasterRight)//��Ϸ��¼�鿴
	{
		return (dwMasterRight & MR_CAN_BETHISTORY_VIEW)!=0 ? false : true;
	}
	static bool CanBetMonitorView(DWORD dwMasterRight)//�ֳ�Ͷע�鿴
	{
		return (dwMasterRight & MR_CAN_BETMONITOR_VIEW)!=0 ? false : true;
	}
	static bool CanOperationView(DWORD dwMasterRight)//������¼�鿴
	{
		return (dwMasterRight & MR_CAN_OPERATION_VIEW)!=0 ? false : true;
	}
	static bool CanMyselfDataSet(DWORD dwMasterRight)//�����ʺ��趨
	{
		return (dwMasterRight & MR_CAN_MYSELFDATA_SET)!=0 ? false : true;
	}
public:
	static bool CanRegisterUser(DWORD dwMasterRight)//ע���ʺ�
	{
		return CanAccountNew(dwMasterRight);
	}
	static bool CanSetUserScore(DWORD dwMasterRight)//��ȡ���
	{
		return CanAccountSet(dwMasterRight);
	}
	static bool CanSetUserCongealState(DWORD dwMasterRight)//״̬�趨ͣ������
	{
		return CanAccountSet(dwMasterRight);
	}
	static bool CanSetBaiscUserInfo(DWORD dwMasterRight)//������Ϣ�޸�
	{
		return CanAccountSet(dwMasterRight);
	}
	static bool CanSetDetailUserInfo(DWORD dwMasterRight)//��ϸ�����޸�
	{
		return CanAccountSet(dwMasterRight);
	}
	static bool CanSetUserRight(DWORD dwMasterRight)//Ȩ���趨
	{
		return CanAccountSet(dwMasterRight);
	}
	static bool CanRegisterSubUser(DWORD dwMasterRight)//ע�����ʺ�
	{
		return CanSubAccountNew(dwMasterRight);
	}
	static bool CanSetBaiscSubUserInfo(DWORD dwMasterRight)//���ʺŻ�����Ϣ�޸�
	{
		return CanSubAccountSet(dwMasterRight);
	}
	static bool CanSetSubUserCongealState(DWORD dwMasterRight)//���ʺ�״̬�趨ͣ������
	{
		return CanSubAccountSet(dwMasterRight);
	}
};

class CUserRight
{
public:
	static const DWORD UR_CAN_PLAYBATTLEGAME 	= dwGameTypeMark_Battle;//�Ƿ��������ս��Ϸ
	static const DWORD UR_CAN_PLAYVIDEOGAME 	= dwGameTypeMark_Video;//�Ƿ�������������Ƶ��Ϸ
	static const DWORD UR_CAN_PLAYPOKIEGAME 	= dwGameTypeMark_Pokie;//�Ƿ������������Ϸ
	static const DWORD UR_CAN_PLAYLOTTERYGAME	= dwGameTypeMark_Lottery;//�Ƿ��������Ʊ��Ϸ
	//���Ȩ��
public:
	static bool CanPlay(WORD wProcessType, DWORD dwUserRight)
	{
		if(IsBattleGameProcessType(wProcessType))
			return CanPlayBattleGame(dwUserRight);
		else if(IsVideoGameProcessType(wProcessType))
			return CanPlayVideoGame(dwUserRight);
		else if(IsPokieGameProcessType(wProcessType))
			return CanPlayPokieGame(dwUserRight);
		else if(IsLotteryGameProcessType(wProcessType))
			return CanPlayLotteryGameGame(dwUserRight);
		else
			return false;
	}
	static bool CanPlayBattleGame(DWORD dwUserRight)//�Ƿ��������ս��Ϸ
	{
		return (dwUserRight & UR_CAN_PLAYBATTLEGAME)!=0 ? false : true;
	}
	static bool CanPlayVideoGame(DWORD dwUserRight)//�Ƿ�������������Ƶ��Ϸ
	{
		return (dwUserRight & UR_CAN_PLAYVIDEOGAME)!=0 ? false : true;
	}
	static bool CanPlayPokieGame(DWORD dwUserRight)//�Ƿ������������Ϸ
	{
		return (dwUserRight & UR_CAN_PLAYPOKIEGAME)!=0 ? false : true;
	}
	static bool CanPlayLotteryGameGame(DWORD dwUserRight)//�Ƿ��������Ʊ��Ϸ
	{
		return (dwUserRight & UR_CAN_PLAYLOTTERYGAME)!=0 ? false : true;
	}
	////////////////////////////////////////////////////////
	static DWORD LimitPlayBattleGame(DWORD dwUserRight)//�������ս��Ϸ
	{
		return (dwUserRight | UR_CAN_PLAYBATTLEGAME);
	}
	static DWORD LimitPlayVideoGame(DWORD dwUserRight)//������������Ƶ��Ϸ
	{
		return (dwUserRight | UR_CAN_PLAYVIDEOGAME);
	}
	static DWORD LimitPlayPokieGame(DWORD dwUserRight)//�����������Ϸ
	{
		return (dwUserRight | UR_CAN_PLAYPOKIEGAME);
	}
	static DWORD LimitPlayLotteryGameGame(DWORD dwUserRight)//�������Ʊ��Ϸ
	{
		return (dwUserRight | UR_CAN_PLAYLOTTERYGAME);
	}
};
//////////////////////////////////////////////////////////////////////////

#endif
