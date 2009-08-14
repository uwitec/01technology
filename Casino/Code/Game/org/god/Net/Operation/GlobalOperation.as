//操作记录类型

//公有
//全部
const OPT_All:uint= 0x00000000;
////////登陆
const OPT_Logon:uint= 0x00000001;
//管理端
///////新增下线帐号
const OPT_NewAccount:uint= 0x00000002;
///////新增子帐号
const OPT_NewSubAccount:uint= 0x00000004;
///////设定密码及名称
const OPT_SetBasicInfo:uint= 0x00000008;
///////存取额度
const OPT_SetScore:uint= 0x00000010;
///////设定状态
const OPT_SetCongealState:uint= 0x00000020;
///////设定占成比
const OPT_SetOccupancyRate:uint= 0x00000040;
//////设定洗码比
const OPT_SetRollbackRate:uint= 0x00000080;
///////设定权限
const OPT_SetUserRight:uint= 0x00000100;
///////踢玩家
const OPT_KickUser:uint= 0x00000200;

//客户中心
const OPT_Notice:uint= 0x00010000;//发布公告
const OPT_CancelBetHistory:uint= 0x00020000;//取消游戏记录
const OPT_RestoreBetHistory:uint= 0x00040000;//恢复游戏记录
const OPT_SetBetLimit:uint= 0x00080000;//设定游戏上下限
const OPT_SetTax:uint= 0x00100000;//设定抽水率
const OPT_SetRevenueSharing:uint= 0x00200000;//设定限红
const OPT_DismissGame:uint= 0x00400000;//解散游戏