//扑克类型
const CT_INVALID:int=0;//错误类型
const CT_SINGLE:int=1;//单牌类型
const CT_DOUBLE:int=2;//对牌类型
const CT_THREE:int=3;//三条类型
const CT_ONE_LINE:int=4;//单连类型
const CT_DOUBLE_LINE=5;//对连类型
const CT_THREE_LINE:int=6;//三连类型
const CT_THREE_LINE_TAKE_ONE:int=7;//三带一单
const CT_THREE_LINE_TAKE_DOUBLE:int=8;//三带一对
const CT_FOUR_LINE_TAKE_ONE:int=9;//四带两单
const CT_FOUR_LINE_TAKE_DOUBLE:int=10;//四带两对
const CT_BOMB_CARD:int=11;//炸弹类型
const CT_MISSILE_CARD:int=12;//火箭类型

//数值掩码
const LOGIC_MASK_COLOR:int=0x000000F0;//花色掩码
const LOGIC_MASK_VALUE:int=0x0000000F;//数值掩码


const MAX_PELS:int=25;//最小象素
const LESS_PELS:int=10;//最小象素
const DEFAULT_PELS:int=20;//默认象素