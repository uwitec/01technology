import flash.utils.ByteArray;
import flash.utils.Endian;

/////////////////////////////////////////////////////////////////////////////////////////
//游戏标识

//通用游戏
const ID_PLAZA:uint=0;//大厅
//对战
const ID_B_HOE:uint=1;//锄大地
const ID_B_LAND:uint=2;//斗地主
const ID_B_UNCOVERPIG:uint=3;//拱猪
const ID_B_BACK_JACK:uint=4;//黑杰克
const ID_B_UPGRADE:uint=5;//拖拉机
const ID_B_SPARROW:uint=6;//麻将
const ID_B_SHOWHAND:uint=7;//梭哈
const ID_B_THREELEAF:uint=8;//诈金花
const ID_B_SICHUAN_LAND:uint=9;//四川斗地主
//真人
const ID_V_DRAGONTIGER:uint=101;//龙虎
const ID_V_BACCARAT:uint=102;//百家乐
const ID_V_SUPER6:uint=103;//Super6
const ID_V_SICBO:uint=104;//骰宝
const ID_V_ROULETTE:uint=105;//轮盘
//电子
const ID_P_SLOT_COOLFRUIT:uint=201;//水果拉霸机
const ID_P_SLOT_POKER:uint=202;//扑克拉霸机
const ID_P_SLOT_MAHJONGBALL:uint=203;//筒子拉霸机
const ID_P_SLOT_SOCCER:uint=204;//足球拉霸机
const ID_P_SLOT_STAR97:uint=205;//明星97拉霸机

const ID_P_VIDEOPOKER_7PK:uint=221;//7PK电动扑克
const ID_P_VIDEOPOKER_5PK:uint=222;//5PK电动扑克
const ID_P_VIDEOPOKER_5PK1:uint=223;//百搭二王电动扑克
const ID_P_VIDEOPOKER_5PK2:uint=224;//花牌电动扑克

const ID_P_BIGGAMEMACHINE_FRUIT:uint=241;//水果大转轮
const ID_P_BIGGAMEMACHINE_LUCKYNUMBER:uint=242;//数字大转轮
const ID_P_BIGGAMEMACHINE_GUESSTRAIN:uint=243;//钻石列车
const ID_P_BIGGAMEMACHINE_MONSTERLEGEND:uint=244;//圣兽传说
const ID_P_BIGGAMEMACHINE_HORSERACE:uint=245;//跑马

const ID_P_TABLEGAME_BLACKJACK:uint=261;//21点
const ID_P_TABLEGAME_WAR:uint=262;//斗大
const ID_P_TABLEGAME_REDDOG:uint=263;//红狗
const ID_P_TABLEGAME_ROULETTE:uint=264;//轮盘
const ID_P_TABLEGAME_DRAGONTIGER:uint=265;//龙虎
const ID_P_TABLEGAME_BACCARAT:uint=266;//百家乐
const ID_P_TABLEGAME_SICBO:uint=267;//骰宝
//彩票
const ID_L_MARKSIX:uint=301;//六合彩
//游戏类型掩码
const dwGameTypeMark_All:uint=0x0;
const dwGameTypeMark_Battle:uint=0x1;
const dwGameTypeMark_Video:uint=0x2;
const dwGameTypeMark_Pokie:uint=0x4;
const dwGameTypeMark_Lottery:uint=0x8;

function IsBattleGameProcessType(wProcessType:uint):Boolean
{
	if (wProcessType >= ID_B_HOE && wProcessType <= ID_B_SICHUAN_LAND)
	{
		return true;
	}
	else
	{
		return false;
	}
}
function IsVideoGameProcessType(wProcessType:uint):Boolean
{
	if (wProcessType >= ID_V_DRAGONTIGER && wProcessType <= ID_V_ROULETTE)
	{
		return true;
	}
	else
	{
		return false;
	}
}
function IsPokieGameProcessType(wProcessType:uint):Boolean
{
	if (wProcessType >= ID_P_SLOT_COOLFRUIT && wProcessType <= ID_P_TABLEGAME_SICBO)
	{
		return true;
	}
	else
	{
		return false;
	}
}
function IsLotteryGameProcessType(wProcessType:uint):Boolean
{
	if (wProcessType >= ID_L_MARKSIX && wProcessType <= ID_L_MARKSIX)
	{
		return true;
	}
	else
	{
		return false;
	}
}/////////////////////////////////////////////////////////////////////////////////////////
//宏定义

const MAX_CHAIR:int=8;//最大游戏桌子
const MAX_CHAT_LEN:int=512;//聊天长度
const INVALID_TABLE:uint=int(-1) >>> 16;//无效桌子号
const INVALID_CHAIR:uint=int(-1) >>> 16;//无效椅子号


//端口定义
const PORT_LOGON_SERVER:int=9000;//登陆服务器
const PORT_CENTER_SERVER:int=9010;//中心服务器

//网络数据定义
const SOCKET_VER:int=68;//数据包版本
const SOCKET_PACKAGE:int=2048;
const SOCKET_BUFFER:int=30000;
const sizeof_DWORD:int=4;
//内核命令码
const MDM_KN_COMMAND:int=0;//内核命令
const SUB_KN_DETECT_SOCKET:int=1;//检测命令

//////////////////////////////////////////////////////////////////////////

//长度宏定义
const TYPE_LEN:int=32;//种类长度
const KIND_LEN:int=32;//类型长度
const STATION_LEN:int=32;//站点长度
const SERVER_LEN:int=32;//房间长度
const MODULE_LEN:int=32;//进程长度


//////////////////////////////////////////////////////////////////////////

//用户状态定义
const US_NULL:int=0x00;//没有状态
const US_FREE:int=0x01;//站立状态
const US_SIT:int=0x02;//坐下状态
const US_READY:int=0x03;//同意状态
const US_LOOKON:int=0x04;//旁观状态
const US_PLAY:int=0x05;//游戏状态
const US_OFFLINE:int=0x06;//断线状态

//长度宏定义
const NAME_LEN:int=32;//名字长度
const PASS_LEN:int=33;//密码长度

//用户等级
const USERLEVEL_GAMEUSER:int=5;//会员
//用户类型
const USERTYPE_GAME:int=0;//普通帐号
const USERTYPE_SUB:int=1;//子帐号
const USERTYPE_DEALER:int=2;//Dealer帐号
//客户端类型
const CLIENTTYPE_GAME:int=0;//游戏端
const CLIENTTYPE_MAN:int=1;//管理端
const CLIENTTYPE_SERVICE:int=2;//客户中心
const CLIENTTYPE_CONTROL:int=3;//控制端

//消息类型
const SMT_INFO:int=0x0001;//信息消息
const SMT_EJECT:int=0x0002;//弹出消息
const SMT_GLOBAL:int=0x0004;//全局消息
const SMT_CLOSE_ROOM:int=0x1000;//关闭房间
const SMT_INTERMIT_LINE:int=0x4000;//中断连接