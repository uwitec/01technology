#ifndef CMD_HOE_HEAD_FILE
#define CMD_HOE_HEAD_FILE
#pragma pack(push)
#pragma pack(1)
//////////////////////////////////////////////////////////////////////////
//公共宏定义

#define KIND_ID						ID_B_HOE								//游戏 I D
#define GAME_PLAYER					4									//游戏人数
#define GAME_NAME					TEXT("锄大地")						//游戏名字
#define GAME_GENRE					(GAME_GENRE_SCORE|GAME_GENRE_GOLD)	//游戏类型

//游戏状态
#define GS_RF_FREE					GS_FREE								//等待开始
#define GS_RF_PLAYING				GS_PLAYING							//叫分状态

//////////////////////////////////////////////////////////////////////////
//服务器命令结构

#define SUB_S_GAME_START			101									//游戏开始
#define SUB_S_OUT_CARD				102									//用户出牌
#define SUB_S_PASS_CARD				103									//放弃出牌
#define SUB_S_GAME_END				104									//游戏结束

//游戏状态
struct CMD_S_StatusFree
{
	DOUBLE							fBaseScore;							//基础额度
};

//游戏状态
struct CMD_S_StatusPlay
{
	DOUBLE							fBaseScore;							//基础额度
	WORD							wBankerUser;						//庄家用户
	WORD							wLastOutUser;						//出牌的人
	WORD				 			wCurrentUser;						//当前玩家
	BYTE							bCardData[13];						//手上扑克
	BYTE							bCardCount[4];						//扑克数目
	BYTE							bTurnCardCount;						//基础出牌
	BYTE							bTurnCardData[13];					//出牌列表
	DOUBLE                          fAllTurnScore[4];					//总局得分
	DOUBLE                          fLastTurnScore[4];					//上局得分
};

//发送扑克
struct CMD_S_GameStart
{
	WORD							wBankerUser;						//庄家用户
	WORD				 			wCurrentUser;						//当前玩家
	BYTE							cbCardData[13];						//扑克列表
};

//用户出牌
struct CMD_S_OutCard
{
	BYTE							bCardCount;							//扑克数目
	WORD				 			wCurrentUser;						//当前玩家
	WORD							wOutCardUser;						//出牌玩家
	BYTE							bCardData[13];						//扑克列表
};

//放弃出牌
struct CMD_S_PassCard
{
	BYTE							bNewTurn;							//一轮开始
	WORD				 			wPassUser;							//放弃玩家
	WORD				 			wCurrentUser;						//当前玩家
};

//游戏结束
struct CMD_S_GameEnd
{
	DOUBLE							fGameScore[4];						//游戏额度
	BYTE							bCardCount[4];						//扑克数目
	BYTE							bCardData[52];						//扑克列表 
};

//////////////////////////////////////////////////////////////////////////
//客户端命令结构

#define SUB_C_OUT_CART				2									//用户出牌
#define SUB_C_PASS_CARD				3									//放弃出牌

//出牌数据包
struct CMD_C_OutCard
{
	BYTE							bCardCount;							//出牌数目
	BYTE							bCardData[13];						//扑克列表
};

//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)
#endif