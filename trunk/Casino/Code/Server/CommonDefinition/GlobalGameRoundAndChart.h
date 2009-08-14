#pragma	  once
#pragma pack(push)
#pragma pack(1)
//排序
static const int SORTTYPE_TimeDesc = 1;//时间降序
static const int SORTTYPE_TimeAsc = SORTTYPE_TimeDesc + 1;//时间升序
static const int SORTTYPE_BetScoreDesc = SORTTYPE_TimeAsc + 1;//投注降序
static const int SORTTYPE_BetScoreAsc  = SORTTYPE_BetScoreDesc + 1;//投注升序
static const int SORTTYPE_WinScoreDesc  = SORTTYPE_BetScoreAsc + 1;//派彩降序
static const int SORTTYPE_WinScoreAsc  = SORTTYPE_WinScoreDesc + 1;//派彩升序
static const int SORTTYPE_ValidScoreDesc  = SORTTYPE_WinScoreAsc + 1;//实货量降序
static const int SORTTYPE_ValidScoreAsc  = SORTTYPE_ValidScoreDesc + 1;//实货量升序
static const int SORTTYPE_RollbackDesc  = SORTTYPE_ValidScoreAsc + 1;//洗码降序
static const int SORTTYPE_RollbackAsc  = SORTTYPE_RollbackDesc + 1;//洗码升序
static const int SORTTYPE_RoundCountDesc  = SORTTYPE_RollbackAsc + 1;//游戏局数目降序
static const int SORTTYPE_RoundCountAsc  = SORTTYPE_RoundCountDesc + 1;//游戏局数目升序

//报表记录查看方式-1
struct tagChartViewOneData
{
	TCHAR	szAccount[NAME_LEN];	//帐号
	//投注
	DWORD	dwGameRoundCount;		//局数
	DOUBLE	fTotalBetScore;			//投注总额
	DOUBLE	fTotalWinScore;			//总派彩
	DOUBLE	fTotalTaxScore;			//总抽水

	//派彩占成
	DOUBLE	fWinScore_High;			//派彩占成-上线
	DOUBLE	fWinScore_Self;			//派彩占成-自身
	DOUBLE	fWinScore_Less;			//派彩占成-下线

	//抽水占成
	DOUBLE	fTaxScore_High;			//抽水占成-上线
	DOUBLE	fTaxScore_Self;			//抽水占成-自身
	DOUBLE	fTaxScore_Less;			//抽水占成-下线
	
	//洗码
	DOUBLE	fValidScore_Total;		//洗码-货量总额
	DOUBLE	fValidScore_High;		//洗码-上线货量
	DOUBLE	fValidScore_High_Rollback;	//洗码-上线洗码
	DOUBLE	fValidScore_Less;		//洗码-下线货量
	DOUBLE	fValidScore_Less_Rollback;	//洗码-下线洗码

	//交收额度
	DOUBLE	fPaidScore_High;		//交收额度-上线交收
	DOUBLE	fPaidScore_Less;		//交收额度-下线交收
};

//报表记录查看方式-2
struct tagChartViewTwoData
{
	TCHAR	szAccount[NAME_LEN];	//帐号
	//投注
	DWORD	dwGameRoundCount;		//局数
	DOUBLE	fTotalBetScore;			//投注总额
	DOUBLE	fTotalWinScore;			//总派彩
	DOUBLE	fTotalTaxScore;			//总抽水

	//洗码
	DOUBLE	fValidScore_Total;		//洗码-货量总额
	DOUBLE	fValidScore_Less_Rollback;	//洗码-下线洗码

};

//游戏局记录
struct tagBetHistoryData
{
	DOUBLE		fUCID;							//报表记录ID

	DOUBLE		fGameRoundID;					//游戏局记录ID
	DOUBLE		fStartTime;						//游戏开始时间
	WORD		wProcessType;					//游戏类型

	TCHAR		szTypeName[TYPE_LEN];			//种类名字
	TCHAR		szKindName[KIND_LEN];			//游戏名字
	TCHAR		szServerName[SERVER_LEN];		//房间名称
	WORD		wTableID;						//桌ID

	DOUBLE		fTotalBetScore;					//投注总额
	DOUBLE		fTotalWinScore;					//总派彩
	DOUBLE		fTotalTaxScore;					//总抽水
	DOUBLE		fValidScore_Less_Rollback;		//洗码-下线洗码

	BYTE		cbEndReason;					//游戏结束原因
	WORD		wEndDataSize;					//游戏结束信息大小
	BYTE		cbEndData[1536];				//游戏结束信息
};
#pragma pack(pop)