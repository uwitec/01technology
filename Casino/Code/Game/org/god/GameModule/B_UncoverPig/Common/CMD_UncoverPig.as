const GAME_PLAYER:uint=4;//游戏人数
const HAND_CARD_COUNT:uint=13;//扑克数目    

//游戏状态
const GS_WK_FREE:uint=0;//等待开始
const GS_WK_SHOWCARD:uint=100 + 1;//玩家亮牌
const GS_WK_PLAYING:uint=100 + 2;//游戏进行

///////////////////////////////////////////////////////////////////////////
//服务器命令结构

const SUB_S_SEND_CARD:uint=100;//发牌命令
const SUB_S_GAME_START:uint=101;//游戏开始
const SUB_S_OUT_CARD:uint=102;//用户出牌
const SUB_S_GAME_END:uint=103;//游戏结束
const SUB_S_SHOW_CARD:uint=104;//玩家亮牌


//////////////////////////////////////////////////////////////////////////
//客户端命令结构

const SUB_C_OUT_CART:uint=1;//用户出牌
const SUB_C_SHOW_CART:uint=2;//玩家亮牌