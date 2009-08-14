#pragma pack(push)
#pragma pack(1)
//////////////////////////////////////////////////////////////////////////
//公共宏定义

#define KIND_ID							ID_P_BIGGAMEMACHINE_LUCKYNUMBER		//游戏 I D
#define GAME_PLAYER						1									//游戏人数
#define GAME_NAME						TEXT("电子-数字大转轮")				//游戏名字
#define GAME_GENRE						(GAME_GENRE_GOLD|GAME_GENRE_MATCH)	//游戏类型

//版本定义
#define		MAXCLIENTVER			    MAKELONG(0,4)
#define		LESSCLIENTVER			    0

//游戏状态定义
#define GS_FREE  						0									//空闲状态
#define	GS_PLAYING				        100									//游戏状态

//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)