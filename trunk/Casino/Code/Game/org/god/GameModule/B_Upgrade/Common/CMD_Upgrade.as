//////////////////////////////////////////////////////////////////////////

const GAME_PLAYER:uint=4;//游戏人数


//游戏状态
const GS_UG_FREE:uint=0;//空闲状态
const GS_UG_CALL:uint=100 + 1;//叫牌状态
const GS_UG_BACK:uint=100 + 2;//留底状态
const GS_UG_PLAY:uint=100 + 3;//游戏状态
const GS_UG_WAIT:uint=100 + 4;//等待状态

//////////////////////////////////////////////////////////////////////////
//服务器命令结构

const SUB_S_GAME_START:uint=100;//游戏开始
const SUB_S_SEND_CARD:uint=101;//发送扑克
const SUB_S_CALL_CARD:uint=102;//用户叫牌
const SUB_S_SEND_CONCEAL:uint=103;//底牌扑克
const SUB_S_GAME_PLAY:uint=104;//游戏开始
const SUB_S_OUT_CARD:uint=105;//用户出牌
const SUB_S_THROW_RESULT:uint=106;//甩牌结果
const SUB_S_TURN_BALANCE:uint=107;//一轮统计
const SUB_S_GAME_END:uint=108;//游戏结束



//////////////////////////////////////////////////////////////////////////
//客户端命令结构

const SUB_C_CALL_CARD:uint=1;//用户叫牌
const SUB_C_CALL_FINISH:uint=2;//叫牌完成
const SUB_C_CONCEAL_CARD:uint=3;//底牌扑克
const SUB_C_OUT_CARD:uint=4;//用户出牌



//////////////////////////////////////////////////////////////////////////