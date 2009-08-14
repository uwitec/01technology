//扑克类型
const CT_ERROR:uint=0;//错误类型
const CT_SINGLE:uint=1;//单牌类型
const CT_DOUBLE:uint=2;//对子类型
const CT_THREE_TIAO:uint=3;//三条类型
const CT_SHUN_ZI:uint=4;//顺子类型
const CT_TONG_HUA:uint=5;//同花类型
const CT_HU_LU:uint=6;//葫芦类型
const CT_TIE_ZHI:uint=7;//铁支类型
const CT_TONG_HUA_SHUN:uint=8;//同花顺型

//宏定义
const MAX_COUNT:uint=13;//最大数目

//数值掩码
const LOGIC_MASK_COLOR:uint=0xF0;//花色掩码
const LOGIC_MASK_VALUE:uint=0x0F;//数值掩码

//数值掩码
const CARD_MASK_COLOR:uint=0xF0;//花色掩码
const CARD_MASK_VALUE:uint=0x0F;//数值掩码