
//游戏状态
const GS_FREE:int= 0;//空闲状态
const GS_PLAYING:int= 100;//游戏状态
//////////////////////////////////////////////////////////////////////////
//IPC 配置信息

const IPC_MAIN_CONFIG:int=2;//配置信息

const IPC_SUB_SERVER_INFO:int=1;//房间信息
const IPC_SUB_COLUMN_INFO:int=2;//列表信息
//////////////////////////////////////////////////////////////////////////
//IPC 用户信息

const IPC_MAIN_USER:int=3;//用户信息

const IPC_SUB_USER_COME:int=1;//用户信息
const IPC_SUB_USER_STATUS:int=2;//用户状态
const IPC_SUB_USER_SCORE:int=3;//用户积分
const IPC_SUB_GAME_START:int=4;//游戏开始
const IPC_SUB_GAME_FINISH:int=5;//游戏结束

//////////////////////////////////////////////////////////////////////////
//IPC 控制信息

const IPC_MAIN_CONCTROL:int=4;//控制信息

const IPC_SUB_START_FINISH:int=1;//启动完成
const IPC_SUB_CLOSE_FRAME:int=2;//关闭框架

//////////////////////////////////////////////////////////////////////////
//网络命令码

const MDM_GF_GAME:int= 100;//游戏消息
const MDM_GF_FRAME:int= 101;//框架消息

const SUB_GF_INFO:int= 1;//游戏信息
const SUB_GF_USER_READY:int= 2;//用户同意

const SUB_GF_OPTION:int= 100;//游戏配置
const SUB_GF_SCENE:int= 101;//场景信息

const SUB_GF_USER_CHAT:int= 200;//用户聊天
const SUB_GF_MESSAGE:int= 300;//系统消息