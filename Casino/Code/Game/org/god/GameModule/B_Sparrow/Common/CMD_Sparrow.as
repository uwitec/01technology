//公共宏定义

const GAME_PLAYER:uint=4;//游戏人数

//游戏状态
const GS_MJ_FREE:uint=GS_FREE;//空闲状态
const GS_MJ_PLAY:uint=(GS_PLAYING+1);//游戏状态
const GS_MJ_HAI_DI:uint=(GS_PLAYING+2);//海底状态

//////////////////////////////////////////////////////////////////////////
//服务器命令结构

const SUB_S_GAME_START:uint = 100;//游戏开始
const SUB_S_OUT_SPARROW:uint = 101;//出牌命令
const SUB_S_SEND_SPARROW:uint = 102;//发送扑克
const SUB_S_OPERATE_NOTIFY:uint = 103;//操作提示
const SUB_S_OPERATE_RESULT:uint = 104;//操作命令
const SUB_S_GAME_END:uint = 106;//游戏结束


//////////////////////////////////////////////////////////////////////////
//客户端命令结构

const SUB_C_OUT_SPARROW:uint = 1;//出牌命令
const SUB_C_OPERATE_SPARROW:uint = 2;//操作扑克