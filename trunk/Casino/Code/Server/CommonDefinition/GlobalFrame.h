#ifndef GLOBAL_FRAME_HEAD_FILE
#define GLOBAL_FRAME_HEAD_FILE

#pragma once

#include "GlobalDef.h"
#pragma pack(push)
#pragma pack(1)
//////////////////////////////////////////////////////////////////////////
//宏定义

//游戏状态
#define GS_FREE							0								//空闲状态
#define GS_PLAYING						100								//游戏状态


//////////////////////////////////////////////////////////////////////////
//网络命令码

#define MDM_GF_GAME						100								//游戏消息
#define MDM_GF_FRAME					101								//框架消息

#define SUB_GF_INFO						1								//游戏信息
#define SUB_GF_USER_READY				2								//用户同意

#define SUB_GF_OPTION					100								//游戏配置
#define SUB_GF_SCENE					101								//场景信息

#define SUB_GF_USER_CHAT				200								//用户聊天

#define SUB_GF_MESSAGE					300								//系统消息

//版本信息
struct CMD_GF_Info
{
	BYTE								bAllowLookon;					//旁观标志
};

//游戏配置
struct CMD_GF_Option
{
	BYTE								bGameStatus;					//游戏状态
	BYTE								bAllowLookon;					//允许旁观
};

//聊天结构
struct CMD_GF_UserChat
{
	WORD								wChatLength;					//信息长度
	DWORD								dwSendUserID;					//发送用户
	DWORD								dwTargetUserID;					//目标用户
	TCHAR								szChatMessage[MAX_CHAT_LEN];	//聊天信息
};

//消息类型
#define SMT_INFO						0x0001							//信息消息
#define SMT_EJECT						0x0002							//弹出消息
#define SMT_GLOBAL						0x0004							//全局消息
#define SMT_CLOSE_GAME					0x1000							//关闭游戏
#define SMT_INTERMIT_LINE				0x4000							//中断连接

//消息数据包
struct CMD_GF_Message
{
	WORD							wMessageType;						//消息类型
	WORD							wErrorCode;							//错误消息代码
	WORD							wMessageLength;						//消息长度
	TCHAR							szContent[1024];					//消息内容
};


//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)
#endif