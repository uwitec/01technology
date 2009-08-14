const GAME_PLAYER:uint=4;//游戏人数

//游戏状态
const GS_RF_FREE:uint=0;//等待开始
const GS_RF_PLAYING:uint=100;//叫分状态

//////////////////////////////////////////////////////////////////////////
//服务器命令结构

const SUB_S_GAME_START:uint=101;//游戏开始
const SUB_S_OUT_CARD:uint=102;//用户出牌
const SUB_S_PASS_CARD:uint=103;//放弃出牌
const SUB_S_GAME_END:uint=104;//游戏结束


//////////////////////////////////////////////////////////////////////////
//客户端命令结构
const SUB_C_OUT_CART:uint=2;//用户出牌
const SUB_C_PASS_CARD:uint=3;//放弃出牌