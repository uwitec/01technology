const GAME_PLAYER:uint=4;//游戏人数

//////////////////////////////////////////////////////////////////////////
//命令码定义

const SUB_C_ADD_GOLD:uint=1;//用户加注
const SUB_C_GIVE_UP:uint=2;//放弃跟注

const SUB_S_GAME_START:uint=100;//游戏开始
const SUB_S_ADD_GOLD:uint=101;//加注结果
const SUB_S_GIVE_UP:uint=102;//放弃跟注
const SUB_S_SEND_CARD:uint=103;//发牌消息
const SUB_S_GAME_END:uint=104;//游戏结束