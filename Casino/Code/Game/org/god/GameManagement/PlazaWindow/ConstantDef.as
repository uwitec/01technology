const nNavCmd_MySelfData:uint 	= 0;//自身资料
const nNavCmd_Account:uint 		= nNavCmd_MySelfData + 1;//帐号管理
const nNavCmd_Chart:uint 		= nNavCmd_Account+1;//报表
const nNavCmd_BetHistory:uint 	= nNavCmd_Chart+1;//局记录
const nNavCmd_BetMonitor:uint 	= nNavCmd_BetHistory+1;//现场投注
const nNavCmd_Operation:uint 	= nNavCmd_BetMonitor+1;//操作记录
const nNavCmd_Logout:uint 		= nNavCmd_Operation+1;//登出