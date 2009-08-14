#ifndef INIT_PARAMTER_HEAD_FILE
#define INIT_PARAMTER_HEAD_FILE

#pragma once

#include "Commonality.h"

//////////////////////////////////////////////////////////////////////////

//配置参数
class CInitParamter
{
	//变量定义
public:
	WORD							m_wListenPort;						//监听端口
	TCHAR							m_szLogonServerAddr[16];			//登陆服务地址
	WORD							m_wMaxConnect;						//最大连接
	TCHAR							m_szCenterServerAddr[16];			//CenterServer

	TCHAR							m_szMainPage[32];					//站点页面
	//登录数据库
public:
	WORD							m_wUserDataBasePort;				//数据库端口
	TCHAR							m_szUserDataBasePipeName[32];		//数据库通道
	TCHAR							m_szUserDataBaseAddr[16];			//数据库地址
	TCHAR							m_szUserDataBaseUser[32];			//数据库用户
	TCHAR							m_szUserDataBasePass[32];			//数据库密码
	TCHAR							m_szUserDataBaseName[32];			//数据库名字

	//登陆密码
public:
	TCHAR							m_szLogonServerPass[32];			//启动服务密码

	//函数定义
public:
	//构造函数
	CInitParamter();
	//析构函数
	virtual ~CInitParamter();

	//功能函数
public:
	//初始化
	void InitParamter();
	//加载配置
	void LoadInitParamter();
	//保存配置
	void SaveInitParamter(bool bSaveAll);
};

//////////////////////////////////////////////////////////////////////////

#endif