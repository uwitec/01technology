const GAME_PLAYER=3;//游戏人数

//游戏状态
const GS_WK_FREE=GS_FREE;//等待开始
const GS_WK_SCORE=GS_PLAYING;//叫分状态
const GS_WK_PLAYING=GS_PLAYING+1;//游戏进行

//////////////////////////////////////////////////////////////////////////
//服务器命令结构

const SUB_S_SEND_CARD=100;//发牌命令
const SUB_S_LAND_SCORE=101;//叫分命令
const SUB_S_GAME_START=102;//游戏开始
const SUB_S_OUT_CARD=103;//用户出牌
const SUB_S_PASS_CARD=104;//放弃出牌
const SUB_S_GAME_END=105;//游戏结束


//////////////////////////////////////////////////////////////////////////
//客户端命令结构

const SUB_C_LAND_SCORE=1;//用户叫分
const SUB_C_OUT_CART=2;//用户出牌
const SUB_C_PASS_CARD=3;//放弃出牌