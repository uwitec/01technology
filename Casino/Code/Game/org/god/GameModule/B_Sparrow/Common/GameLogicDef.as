
//////////////////////////////////////////////////////////////////////////
//常量定义

const MAX_WEAVE:uint=4;//最大组合
const MAX_INDEX:uint=34;//最大索引
const MAX_COUNT:uint=14;//最大数目
const MAX_REPERTORY:uint=108;//最大库存

//////////////////////////////////////////////////////////////////////////
//逻辑掩码

const MASK_COLOR:uint=0xF0;//花色掩码
const MASK_VALUE:uint=0x0F;//数值掩码

//////////////////////////////////////////////////////////////////////////
//动作定义

//动作标志
const WIK_NULL:uint=0x00;//没有类型
const WIK_LEFT:uint=0x01;//左吃类型
const WIK_CENTER:uint=0x02;//中吃类型
const WIK_RIGHT:uint=0x04;//右吃类型
const WIK_PENG:uint=0x08;//碰牌类型
const WIK_FILL:uint=0x10;//补牌类型
const WIK_GANG:uint=0x20;//杠牌类型
const WIK_CHI_HU:uint=0x40;//吃胡类型

//////////////////////////////////////////////////////////////////////////
//胡牌定义

//小胡牌型
const CHK_NULL:uint=0x0000;//非胡类型
const CHK_JI_HU:uint=0x0001;//鸡胡类型
const CHK_PING_HU:uint=0x0002;//平胡类型

//特殊胡牌
const CHK_SIXI_HU:uint=0x0010;//四喜胡牌
const CHK_BANBAN_HU:uint=0x0020;//板板胡牌
const CHK_LIULIU_HU:uint=0x0040;//六六顺牌
const CHK_QUEYISE_HU:uint=0x0080;//缺一色牌

//大胡牌型
const CHK_PENG_PENG:uint=0x0100;//碰碰胡牌
const CHK_JIANG_JIANG:uint=0x0200;//将将胡牌
const CHK_QI_XIAO_DUI:uint=0x0400;//七小对牌
const CHK_HAO_HUA_DUI:uint=0x0400;//豪华对牌

//需将权位
const CHR_DI:uint=0x0001;//地胡权位
const CHR_TIAN:uint=0x0002;//天胡权位
const CHR_HAI_DI:uint=0x0004;//海底权位
const CHR_QIANG_GANG:uint=0x0008;//抢杆权位

//乱将权位
const CHR_QING_YI_SE:uint=0x0100;//清色权位
const CHR_QUAN_QIU_REN:uint=0x0200;//全求权位

//////////////////////////////////////////////////////////////////////////