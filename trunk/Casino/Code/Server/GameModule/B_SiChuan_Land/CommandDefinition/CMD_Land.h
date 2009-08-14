#ifndef CMD_LAND_HEAD_FILE
#define CMD_LAND_HEAD_FILE
#pragma pack(push)
#pragma pack(1)
//////////////////////////////////////////////////////////////////////////
//公共宏定义

#define KIND_ID						ID_B_SICHUAN_LAND					//游戏 I D
#define GAME_PLAYER					3									//游戏人数
#define GAME_NAME					TEXT("四川斗地主")					//游戏名字
#define GAME_GENRE					(GAME_GENRE_SCORE|GAME_GENRE_GOLD)	//游戏类型

//游戏状态
#define GS_WK_FREE					GS_FREE								//等待开始
#define GS_WK_SCORE					GS_PLAYING							//叫分状态
#define GS_WK_PLAYING				GS_PLAYING+1						//游戏进行
#define GS_WK_DOUBLETIME			GS_PLAYING+2						//反地主状态

//////////////////////////////////////////////////////////////////////////
//服务器命令结构

#define SUB_S_SEND_CARD				100									//发牌命令
#define SUB_S_LAND_SCORE			101									//叫分命令
#define SUB_S_GAME_START			102									//游戏开始
#define SUB_S_OUT_CARD				103									//用户出牌
#define SUB_S_PASS_CARD				104									//放弃出牌
#define SUB_S_GAME_END				105									//游戏结束
#define SUB_S_DOUBLETIME			106									//反地主
#define SUB_S_USERDOUBLETIME		107									//用户反地主

//游戏状态
struct CMD_S_StatusFree
{
	DOUBLE							fBaseScore;							//基础额度
};

//游戏状态
struct CMD_S_StatusScore
{
	BYTE							bLandScore;							//地主分数
	DOUBLE							fBaseScore;							//基础额度
	WORD				 			wCurrentUser;						//当前玩家
	BYTE							bScoreInfo[3];						//叫分信息
	BYTE							bCardData[17];						//手上扑克
};
//游戏状态
struct CMD_S_StatusDoubleTime
{
	WORD							wLandUser;							//坑主玩家
	WORD							wBombTime;							//炸弹倍数
	DOUBLE							fBaseScore;							//基础额度
	BYTE							bLandScore;							//地主分数
	WORD				 			wCurrentUser;						//当前玩家
	BYTE							bBackCard[3];						//底牌扑克
	BYTE							bCardData[20];						//手上扑克
	BYTE							bCardCount[3];						//扑克数目
	BYTE							bDoubleTimeScore[3];				//反牌
};
//游戏状态
struct CMD_S_StatusPlay
{
	WORD							wLandUser;							//坑主玩家
	WORD							wBombTime;							//炸弹倍数
	DOUBLE							fBaseScore;							//基础额度
	BYTE							bLandScore;							//地主分数
	WORD							wLastOutUser;						//出牌的人
	WORD				 			wCurrentUser;						//当前玩家
	BYTE							bBackCard[3];						//底牌扑克
	BYTE							bCardData[20];						//手上扑克
	BYTE							bCardCount[3];						//扑克数目
	BYTE							bTurnCardCount;						//基础出牌
	BYTE							bTurnCardData[20];					//出牌列表
	BYTE							bDoubleTimeScore[3];				//反牌
};

//发送扑克
struct CMD_S_SendCard
{
	WORD				 			wCurrentUser;						//当前玩家
	BYTE							bCardData[17];						//扑克列表
};

//用户叫分
struct CMD_S_LandScore
{
	WORD							bLandUser;							//叫分玩家
	WORD				 			wCurrentUser;						//当前玩家
	BYTE							bLandScore;							//上次叫分
	BYTE							bCurrentScore;						//当前叫分
};
//用户反牌
struct CMD_S_UserDoubleTime
{
	WORD							wDoubleTimeUser;					//反牌玩家
	BYTE							bDoubleTime;						//反牌 1-不反 2-反牌
	WORD				 			wCurrentUser;						//当前玩家
};
//用户反地主
struct CMD_S_DoubleTime
{
	WORD				 			wLandUser;							//地主玩家
	BYTE							bLandScore;							//地主分数
	WORD				 			wCurrentUser;						//当前玩家
	BYTE							bBackCard[3];						//底牌扑克
};

//游戏开始
struct CMD_S_GameStart
{
	WORD				 			wLandUser;							//地主玩家
	BYTE							bLandScore;							//地主分数
	WORD				 			wCurrentUser;						//当前玩家
};

//用户出牌
struct CMD_S_OutCard
{
	BYTE							bCardCount;							//出牌数目
	WORD				 			wCurrentUser;						//当前玩家
	WORD							wOutCardUser;						//出牌玩家
	BYTE							bCardData[20];						//扑克列表
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
	DOUBLE							fGameScore[3];						//游戏额度
	BYTE							bCardCount[3];						//扑克数目
	BYTE							bCardData[54];						//扑克列表 
	BYTE							bBackCard[3];						//底牌扑克
	WORD							wLandUser;							//坑主玩家
	WORD							wBombTime;							//炸弹倍数
	BYTE							bLandScore;							//地主分数
	BYTE							bDoubleTimeScore[3];				//反牌
};

//////////////////////////////////////////////////////////////////////////
//客户端命令结构

#define SUB_C_LAND_SCORE			1									//用户叫分
#define SUB_C_OUT_CART				2									//用户出牌
#define SUB_C_PASS_CARD				3									//放弃出牌
#define SUB_C_DOUBLETIME			4									//用户反牌

//用户叫分
struct CMD_C_LandScore
{
	BYTE							bLandScore;							//地主分数
};

//出牌数据包
struct CMD_C_OutCard
{
	BYTE							bCardCount;							//出牌数目
	BYTE							bCardData[20];						//扑克列表
};
//用户反牌
struct CMD_C_DoubleTime
{
	BYTE							bDoubleTime;						//反牌 1-不反 2-反牌
};
//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)
#endif