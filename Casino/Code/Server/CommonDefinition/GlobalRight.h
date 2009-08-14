#ifndef GLOBAL_RIGHT_HEAD_FILE
#define GLOBAL_RIGHT_HEAD_FILE

//////////////////////////////////////////////////////////////////////////

#include "GlobalDef.h"

//////////////////////////////////////////////////////////////////////////

//权限辅助类
class CMasterRight
{
	//管理权限
public:
	static const DWORD MR_CAN_ACCOUNT_NEW 		= 0x00000001;//下线帐号新建
	static const DWORD MR_CAN_ACCOUNT_SET 		= 0x00000002;//下线帐号设定
	static const DWORD MR_CAN_SUBACCOUNT_NEW	= 0x00000004;//子帐号新建
	static const DWORD MR_CAN_SUBACCOUNT_SET	= 0x00000008;//子帐号设定
	static const DWORD MR_CAN_CHART_VIEW 		= 0x00000010;//游戏报表查看
	static const DWORD MR_CAN_BETHISTORY_VIEW	= 0x00000020;//游戏记录查看
	static const DWORD MR_CAN_BETMONITOR_VIEW	= 0x00000040;//现场投注查看
	static const DWORD MR_CAN_OPERATION_VIEW	= 0x00000080;//操作记录查看
	static const DWORD MR_CAN_MYSELFDATA_SET	= 0x00000100;//自身帐号设定
public:
	static bool CanAccountNew(DWORD dwMasterRight)//下线帐号新建
	{
		return (dwMasterRight & MR_CAN_ACCOUNT_NEW)!=0 ? false : true;
	}
	static bool CanAccountSet(DWORD dwMasterRight)//下线帐号设定
	{
		return (dwMasterRight & MR_CAN_ACCOUNT_SET)!=0 ? false : true;
	}
	static bool CanSubAccountNew(DWORD dwMasterRight)//子帐号新建
	{
		return (dwMasterRight & MR_CAN_SUBACCOUNT_NEW)!=0 ? false : true;
	}
	static bool CanSubAccountSet(DWORD dwMasterRight)//子帐号设定
	{
		return (dwMasterRight & MR_CAN_SUBACCOUNT_SET)!=0 ? false : true;
	}
	static bool CanChartView(DWORD dwMasterRight)//游戏报表查看
	{
		return (dwMasterRight & MR_CAN_CHART_VIEW)!=0 ? false : true;
	}
	static bool CanBetHistoryView(DWORD dwMasterRight)//游戏记录查看
	{
		return (dwMasterRight & MR_CAN_BETHISTORY_VIEW)!=0 ? false : true;
	}
	static bool CanBetMonitorView(DWORD dwMasterRight)//现场投注查看
	{
		return (dwMasterRight & MR_CAN_BETMONITOR_VIEW)!=0 ? false : true;
	}
	static bool CanOperationView(DWORD dwMasterRight)//操作记录查看
	{
		return (dwMasterRight & MR_CAN_OPERATION_VIEW)!=0 ? false : true;
	}
	static bool CanMyselfDataSet(DWORD dwMasterRight)//自身帐号设定
	{
		return (dwMasterRight & MR_CAN_MYSELFDATA_SET)!=0 ? false : true;
	}
public:
	static bool CanRegisterUser(DWORD dwMasterRight)//注册帐号
	{
		return CanAccountNew(dwMasterRight);
	}
	static bool CanSetUserScore(DWORD dwMasterRight)//存取额度
	{
		return CanAccountSet(dwMasterRight);
	}
	static bool CanSetUserCongealState(DWORD dwMasterRight)//状态设定停用启用
	{
		return CanAccountSet(dwMasterRight);
	}
	static bool CanSetBaiscUserInfo(DWORD dwMasterRight)//基本信息修改
	{
		return CanAccountSet(dwMasterRight);
	}
	static bool CanSetDetailUserInfo(DWORD dwMasterRight)//详细资料修改
	{
		return CanAccountSet(dwMasterRight);
	}
	static bool CanSetUserRight(DWORD dwMasterRight)//权限设定
	{
		return CanAccountSet(dwMasterRight);
	}
	static bool CanRegisterSubUser(DWORD dwMasterRight)//注册子帐号
	{
		return CanSubAccountNew(dwMasterRight);
	}
	static bool CanSetBaiscSubUserInfo(DWORD dwMasterRight)//子帐号基本信息修改
	{
		return CanSubAccountSet(dwMasterRight);
	}
	static bool CanSetSubUserCongealState(DWORD dwMasterRight)//子帐号状态设定停用启用
	{
		return CanSubAccountSet(dwMasterRight);
	}
};

class CUserRight
{
public:
	static const DWORD UR_CAN_PLAYBATTLEGAME 	= dwGameTypeMark_Battle;//是否允许玩对战游戏
	static const DWORD UR_CAN_PLAYVIDEOGAME 	= dwGameTypeMark_Video;//是否允许玩真人视频游戏
	static const DWORD UR_CAN_PLAYPOKIEGAME 	= dwGameTypeMark_Pokie;//是否允许玩电子游戏
	static const DWORD UR_CAN_PLAYLOTTERYGAME	= dwGameTypeMark_Lottery;//是否允许玩彩票游戏
	//玩家权限
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
	static bool CanPlayBattleGame(DWORD dwUserRight)//是否允许玩对战游戏
	{
		return (dwUserRight & UR_CAN_PLAYBATTLEGAME)!=0 ? false : true;
	}
	static bool CanPlayVideoGame(DWORD dwUserRight)//是否允许玩真人视频游戏
	{
		return (dwUserRight & UR_CAN_PLAYVIDEOGAME)!=0 ? false : true;
	}
	static bool CanPlayPokieGame(DWORD dwUserRight)//是否允许玩电子游戏
	{
		return (dwUserRight & UR_CAN_PLAYPOKIEGAME)!=0 ? false : true;
	}
	static bool CanPlayLotteryGameGame(DWORD dwUserRight)//是否允许玩彩票游戏
	{
		return (dwUserRight & UR_CAN_PLAYLOTTERYGAME)!=0 ? false : true;
	}
	////////////////////////////////////////////////////////
	static DWORD LimitPlayBattleGame(DWORD dwUserRight)//允许玩对战游戏
	{
		return (dwUserRight | UR_CAN_PLAYBATTLEGAME);
	}
	static DWORD LimitPlayVideoGame(DWORD dwUserRight)//允许玩真人视频游戏
	{
		return (dwUserRight | UR_CAN_PLAYVIDEOGAME);
	}
	static DWORD LimitPlayPokieGame(DWORD dwUserRight)//允许玩电子游戏
	{
		return (dwUserRight | UR_CAN_PLAYPOKIEGAME);
	}
	static DWORD LimitPlayLotteryGameGame(DWORD dwUserRight)//允许玩彩票游戏
	{
		return (dwUserRight | UR_CAN_PLAYLOTTERYGAME);
	}
};
//////////////////////////////////////////////////////////////////////////

#endif
