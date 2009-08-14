//////////////////////////////////////////////////////////////////////////
//////登陸
const MDM_GR_LOGON:int=1;//房间登录

const SUB_GR_LOGON_ACCOUNT:int=1;//帐户登录

const SUB_GR_LOGON_SUCCESS:int=100;//登陆成功
const SUB_GR_LOGON_ERROR:int=101;//登陆失败
const SUB_GR_LOGON_FINISH:int=102;//登陆完成


//////////////////////////////////////////////////////////////////////////
//用户数据包定义

const MDM_GR_USER:int=2;//用户信息

const SUB_GR_USER_SIT_REQ:int=1;//坐下请求
const SUB_GR_USER_LOOKON_REQ:int=2;//旁观请求
const SUB_GR_USER_STANDUP_REQ:int=3;//起立请求
const SUB_GR_USER_LEFT_GAME_REQ:int=4;//离开游戏

const SUB_GR_USER_COME:int=100;//用户进入
const SUB_GR_USER_STATUS:int=101;//用户状态
const SUB_GR_USER_SCORE:int=102;//用户分数
const SUB_GR_SIT_FAILED=103;//坐下失败

const SUB_GR_USER_RULE:int=200;//用户规则
const SUB_GR_USER_CHAT:int=201;//聊天消息
const SUB_GR_USER_WISPER:int=202;//私语消息

const SUB_GR_USER_INVITE:int=300;//邀请消息
const SUB_GR_USER_INVITE_REQ:int=301;//邀请请求

const AUTO_TABLEID=1024;//服务器自动选择位置
const AUTO_CHAIRID=1024;//服务器自动选择位置
//////////////////////////////////////////////////////////////////////////
//配置信息数据包

const MDM_GR_INFO:int=3;//配置信息

const SUB_GR_SERVER_INFO:int=100;//房间配置
const SUB_GR_CONFIG_FINISH:int=101;//配置完成
//////////////////////////////////////////////////////////////////////////
//房间状态数据包

const MDM_GR_STATUS:int=4;//状态信息

const SUB_GR_TABLE_INFO:int=100;//桌子信息
const SUB_GR_TABLE_STATUS:int=101;//桌子状态


//////////////////////////////////////////////////////////////////////////
//管理数据包

const MDM_GR_MANAGER:int=5;//管理命令

const SUB_GR_SEND_WARNING:int=1;//发送警告
const SUB_GR_SEND_MESSAGE:int=2;//发送消息
const SUB_GR_LOOK_USER_IP:int=3;//查看地址
const SUB_GR_KILL_USER:int=4;//踢出用户
const SUB_GR_OPTION_SERVER:int=5;//房间设置

const SUB_GR_LOOK_USER_IP_RESULT:int=30;//返回查看地址


//////////////////////////////////////////////////////////////////////////
//系统数据包

const MDM_GR_SYSTEM:int=10;//系统信息

const SUB_GR_MESSAGE:int=100;//系统消息



//////////////////////////////////////////////////////////////////////////
//房间数据包

const MDM_GR_SERVER_INFO:int=11;//房间信息

const SUB_GR_ONLINE_COUNT_INFO:int=100;//在线信息

//////////////////////////////////////////////////////////////////////////