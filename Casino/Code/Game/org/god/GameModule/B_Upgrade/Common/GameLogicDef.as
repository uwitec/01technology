//牌副定义
const CELL_PACK:uint=54;//单元数目

//属性定义
const MAX_COUNT:uint=60;//最大数目
const MAX_TRACKOR:uint=18;//最大连牌
const COLOR_RIGHT:uint=40;//花色权位

//无效定义
const VALUE_ERROR:uint=0x00;//数值定义
const COLOR_ERROR:uint=0xFF;//花色定义

//花色定义
const COLOR_NT:uint=0x40;//花色定义
const COLOR_HEI_TAO:uint=0x30;//花色定义
const COLOR_HONG_TAO:uint=0x20;//花色定义
const COLOR_MEI_HUA:uint=0x10;//花色定义
const COLOR_FANG_KUAI:uint=0x00;//花色定义

//数值掩码
const LOGIC_MASK_COLOR:uint=0xF0;//花色掩码
const LOGIC_MASK_VALUE:uint=0x0F;//数值掩码

//扑克类型
const CT_ERROR:uint=0;//错误类型
const CT_SINGLE:uint=1;//单牌类型
const CT_SAME_2:uint=2;//对牌类型
const CT_SAME_3:uint=3;//三牌类型
const CT_SAME_4:uint=4;//四牌类型
const CT_TRACKOR_2:uint=5;//拖拉机型
const CT_TRACKOR_3:uint=6;//拖拉机型
const CT_TRACKOR_4:uint=7;//拖拉机型
const CT_THROW_CARD:uint=8;//甩牌类型

//////////////////////////////////////////////////////////////////////////
//索引定义
const INDEX_OUT:uint=0;//出牌索引
const INDEX_HAND:uint=1;//手牌索引
const INDEX_FIRST:uint=2;//首出索引