//////////////////////////////////////////////////////////////////////////
import flash.utils.ByteArray;

//广场版本
const VER_PLAZA_LOW:int=1;//广场版本
const VER_PLAZA_HIGH:int=1;//广场版本
const VER_PLAZA_FRAME:int=VER_PLAZA_LOW | VER_PLAZA_HIGH << 16;
//////////////////////////////////////////////////////////////////////////
//登录命令码
const MDM_GP_LOGON:int=1;//广场登录
const SUB_GP_LOGON_ACCOUNT:int=1;//帐号登录
const SUB_GP_LOGON_SUCCESS:int=100;//登陆成功
const SUB_GP_LOGON_ERROR:int=101;//登陆失败
const SUB_GP_LOGON_GAMEINFO:int=102;//游戏信息
const SUB_GP_LOGON_NOTICE:int=103;//公告
const SUB_GP_LOGON_FINISH:int=104;//登陆完成
//////////////////////////////////////////////////////////////////////////
//游戏列表命令码
const MDM_GP_SERVER_LIST:int=2;//列表信息

const SUB_GP_LIST_TYPE:int=100;//类型列表
const SUB_GP_LIST_KIND:int=101;//种类列表
const SUB_GP_LIST_STATION:int=102;//站点列表
const SUB_GP_LIST_SERVER:int=103;//房间列表
const SUB_GP_LIST_FINISH:int=104;//发送完成
const SUB_GP_LIST_CONFIG:int=105;//列表配置
const SUB_GP_LIST_PROCESS:int=106;//进程列表

const SUB_GP_LIST_ROOT:int=300;//总节点
const SUB_GP_LIST_CASINOFIELD:int=301;//現場

//////////////////////////////////////////////////////////////////////////
//系统命令码
const MDM_GP_SYSTEM:int=3;//系统命令
const SUB_GP_VERSION:int=100;//版本通知
const SUB_SP_SYSTEM_MSG:int=101;//系统消息
//////////////////////////////////////////////////////////////////////////
//编辑帐号命令
const MDM_GP_EDIT_USERINFO:int=4;//编辑帐号
//-------------注册帐号
const SUB_GP_REGISTER_USER:int=1;//注册帐号
const SUB_GP_REGISTER_SUCCESS:int=101;//注册成功
const SUB_GP_REGISTER_ERROR:int=102;//注册失败
//-----------状态设定
const SUB_GP_SET_USERCONGEALSTATE:int=2;//状态设定停用启用
const SUB_GP_SET_USERCONGEALSTATE_SUCCESS:int=201;//状态设定成功
const SUB_GP_SET_USERCONGEALSTATE_ERROR:int=202;//状态设定失败
//-----------额度存取
const SUB_GP_SET_SETUSERSCORE:int=3;
const SUB_GP_SET_SETUSERSCORE_SUCCESS:int=301;//额度存取成功
const SUB_GP_SET_SETUSERSCORE_ERROR:int=302;//额度存取失败
//---------基本信息修改
const SUB_GP_SET_BASICUSERINFO:int=4;
const SUB_GP_SET_BASICUSERINFO_SUCCESS:int=401;//基本信息修改成功
const SUB_GP_SET_BASICUSERINFO_ERROR:int=402;//基本信息修改失败
//---------详细信息修改
const SUB_GP_SET_DETAILUSERINFO:int=5;
const SUB_GP_SET_DETAILUSERINFO_SUCCESS:int=501;//详细信息修改成功
const SUB_GP_SET_DETAILUSERINFO_ERROR:int=502;//详细信息修改失败
//-----------权限设定
const SUB_GP_SET_USERRIGHT:int=6;//权限设定
const SUB_GP_SET_USERRIGHT_SUCCESS:int=601;//权限设定成功
const SUB_GP_SET_USERRIGHT_ERROR:int=602;//权限设定失败
//---------自身基本资料修改
const SUB_GP_SET_MYSELFBASICUSERINFO:int=7;
const SUB_GP_SET_MYSELFBASICUSERINFO_SUCCESS:int=701;//基本资料修改成功
const SUB_GP_SET_MYSELFBASICUSERINFO_ERROR:int=702;//基本资料修改失败

//////////////////////////////////////////////////////////////////////////
//编辑子帐号命令
const MDM_GP_EDIT_SUBUSERINFO:int=5;//编辑子帐号
//-------------注册子帐号
const SUB_GP_REGISTER_SUBUSER:int=1;//注册子帐号
const SUB_GP_REGISTER_SUBUSER_SUCCESS:int=101;//注册成功
const SUB_GP_REGISTER_SUBUSER_ERROR:int=102;//注册失败
//-----------状态设定
const SUB_GP_SET_SUBUSERCONGEALSTATE:int=2;//状态设定停用启用

const SUB_GP_SET_SUBUSERCONGEALSTATE_SUCCESS:int=201;//状态设定成功
const SUB_GP_SET_SUBUSERCONGEALSTATE_ERROR:int=202;//状态设定失败
//---------基本资料修改
const SUB_GP_SET_BASICSUBUSERINFO:int=3;
const SUB_GP_SET_BASICSUBUSERINFO_SUCCESS:int=301;//基本资料修改成功
const SUB_GP_SET_BASICSUBUSERINFO_ERROR:int=302;//基本资料修改失败

//////////////////////////////////////////////////////////////////////////
//帐号获取
const MDM_GP_GET_USER:int=6;//获取帐号
//-------------获取帐号
const SUB_GP_GET_LOWUSERLIST:int=1;//获取下线帐号

const SUB_GP_LIST_LOWUSER:int=101;//帐号
const SUB_GP_LIST_LOWUSERERROR:int=102;//获取帐号失败
const SUB_GP_LIST_SUBUSER:int=103;//子帐号
/////////////////////////////////////////////////////////////////////////////////
//记录获取
const MDM_GP_GET_RECORD:int=7;//获取记录
//-----------------获取操作记录
const SUB_GP_GET_OPERATION:int=1;//获取操作记录

const SUB_GP_LIST_OPERATIONCONFIG:int=101;//操作记录列表配置
const SUB_GP_LIST_OPERATION:int=102;//操作记录
const SUB_GP_LIST_OPERATIONFINISH:int=103;//发送完毕
const SUB_GP_LIST_OPERATIONERROR:int=104;//请求失败
//------------------获取报表记录查看方式-1
const SUB_GP_GET_CHARTVIEWONE:int=2;//获取报表记录查看-1

const SUB_GP_LIST_CHARTVIEWONECONFIG:int=201;//报表记录列表配置
const SUB_GP_LIST_CHARTVIEWONE:int=202;//报表记录
const SUB_GP_LIST_CHARTVIEWONEFINISH:int=203;//发送完毕
const SUB_GP_LIST_CHARTVIEWONEERROR:int=204;//请求失败
//------------------获取报表记录查看-2
const SUB_GP_GET_CHARTVIEWTWO:int=3;//获取报表记录查看-2

const SUB_GP_LIST_CHARTVIEWTWOCONFIG:int=301;//报表记录列表配置
const SUB_GP_LIST_CHARTVIEWTWO:int=302;//操作记录
const SUB_GP_LIST_CHARTVIEWTWOFINISH:int=303;//发送完毕
const SUB_GP_LIST_CHARTVIEWTWOERROR:int=304;//请求失败
//------------------获取游戏记录查看
const SUB_GP_GET_BETHISTORY:int=4;//获取游戏记录查看

const SUB_GP_LIST_BETHISTORYCONFIG:int=401;//报表记录列表配置
const SUB_GP_LIST_BETHISTORY:int=402;//操作记录
const SUB_GP_LIST_BETHISTORYFINISH:int=403;//发送完毕
const SUB_GP_LIST_BETHISTORYERROR:int=404;//请求失败
//------------------获取游戏局记录详细投注信息
const SUB_GP_GET_DETAILBETSCORE:int=5;//获取游戏局记录详细投注信息

const SUB_GP_GET_DETAILBETSCORESUCCESS:int=501;//获取游戏局记录详细投注信息成功
const SUB_GP_GET_DETAILBETSCOREERROR:int=502;//获取游戏局记录详细投注信息成功
//------------------获取报表记录查看-3
const SUB_GP_GET_CHARTVIEWTHREE:int=6;//获取报表记录查看-3

const SUB_GP_CHARTVIEWTHREESUCCESS:int=601;//报表记录列表配置
const SUB_GP_CHARTVIEWTHREEERROR:int=602;//请求失败
/////////////////////////////////////////////////////
const MDM_GP_SERVER_INFO:int=8;//房间信息
const SUB_GP_ONLINE_COUNT_INFO:int=1;//在线信息