
//////////////////////////////////////////////////////////////////////////

const GAME_PLAYER:uint=4;//游戏人数

//游戏状态定义
//const GS_FREE:uint=0;//空闲状态
//const GS_PLAYING:uint=100;//游戏状态
const GS_ADDGOLDING:uint=101;//加注状态

//结束原因
const GO_NORMAL_FINISH:uint=0x10;//正常结束

//玩家状态
const NO_CARD:uint=0;//无牌状态
const INIT_CARD:uint=1;//初始状态
const CUT_CARD:uint=2;//分牌状态
const STOP_CARD:uint=3;//停牌状态
const BRUST_CARD:uint=4;//爆牌状态
const DOUBLE_GOLD:uint=5;//双倍状态

//////////////////////////////////////////////////////////////////////////
//服务器命令结构

//命令码定义

const SUB_S_GAME_START:uint=100;//游戏开始
const SUB_S_ADD_GOLD:uint=101;//下注结果
const SUB_S_INIT_CARD:uint=102;//初始发牌
const SUB_S_CUT_CARD:uint=103;//分牌消息
const SUB_S_CUT_CARD_MARK:uint=104;//切换分牌
const SUB_S_SEND_CARD:uint=105;//发牌消息
const SUB_S_STOP_CARD:uint=106;//用户停牌
const SUB_S_BRUST_CARD:uint=107;//爆牌消息
const SUB_S_BANKER_SEND_CARD:uint=108;//庄家发牌
const SUB_S_DOUBLE_GOLD:uint=109;//庄家发牌
const SUB_S_USER_LEFT:uint=110;//闲家强退
const SUB_S_GAME_END:uint=111;//游戏结束


//////////////////////////////////////////////////////////////////////////
//客户端命令结构

//命令码定义
const SUB_C_ADD_GOLD:uint=1;//用户下注
const SUB_C_CANCEL_GOLD:uint=2;//取消下注
const SUB_C_GET_CARD:uint=3;//用户要牌
const SUB_C_STOP_CARD:uint=4;//用户停牌
const SUB_C_DOUBLE_GOLD:uint=5;//用户双倍
const SUB_C_CUT_CARD:uint=6;//用户分牌



//////////////////////////////////////////////////////////////////////////