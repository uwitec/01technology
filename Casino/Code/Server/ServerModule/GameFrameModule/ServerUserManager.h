#ifndef SERVER_USER_MANAGER_HEAD_FILE
#define SERVER_USER_MANAGER_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameServiceExport.h"

typedef CMap<DWORD,DWORD,DWORD,DWORD> CalculateItemPosMap;
//////////////////////////////////////////////////////////////////////////

//用户信息项
class CServerUserItem : public IServerUserItem
{
	//友元定义
	friend class CServerUserManager;

	//基本信息
protected:
	bool								m_bAcitve;						//激活标志
	tagUserRule							m_UserRule;						//用户规则
	tagServerUserData					m_ServerUserData;				//用户信息
	IServerUserItem*					m_pParent;						//父亲
	LONG								m_lUserRefCount;			//下线引用计数
	//辅助信息
protected:
	WORD								m_wUserIndex;					//用户索引
	DWORD								m_dwClientIP;					//连接地址
	DWORD								m_dwLogonTime;					//登录时间
	DWORD								m_dwPlayTimeCount;				//游戏时间
	TCHAR								m_szPassword[PASS_LEN];			//用户密码

	CalculateItemPosMap					m_CalculateItemPosMap;			//计算辅助

	//函数定义
public:
	//构造函数
	CServerUserItem(void);
	//析构函数
	virtual ~CServerUserItem(void);

	//基础接口
public:
	//释放对象
	virtual bool __cdecl Release() { if (IsValid()) delete this; return true; }
	//是否有效
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CServerUserItem))?true:false; }
	//接口查询
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//引用接口
public:
	//增加引用
	virtual void __cdecl AddUserRefCount();
	//减少引用
	virtual void __cdecl ReleaseUserRefCount();
	//引用为空
	virtual bool __cdecl IsUserRefCountZero();
	//信息接口
public:
	//是否激活
	virtual bool __cdecl IsActive() { return m_bAcitve; }
	//用户索引
	virtual WORD __cdecl GetUserIndex() { return m_wUserIndex; }
	//获取地址
	virtual DWORD __cdecl GetClientIP() { return m_dwClientIP; }
	//获取密码
	virtual LPCTSTR __cdecl GetPassword() { return m_szPassword; }
	//获取规则
	virtual const tagUserRule * __cdecl GetUserRule() { return &m_UserRule; }
	//获取信用额度
	virtual  tagUserScore * __cdecl GetUserScore() { return &m_ServerUserData.UserData.UserScore;}
	//获取信息
	virtual tagServerUserData * __cdecl GetUserData() { return &m_ServerUserData; };
	//获取父用户
	virtual IServerUserItem * __cdecl GetParent(){return m_pParent;}
	//设置父用户
	virtual bool  __cdecl SetParent(IServerUserItem* pParent);

	//属性接口
public:
	//用户名字
	virtual LPCTSTR __cdecl GetAccounts() { return m_ServerUserData.UserData.szAccount; }
	//用户 I D
	virtual DWORD __cdecl GetUserID() { return m_ServerUserData.UserData.dwUserID; }
	//父用户 I D
	virtual DWORD __cdecl GetParentUserID(){return m_ServerUserData.UserData.dwParentUserID;}
	//用户等级
	virtual BYTE __cdecl GetUserLevel(){ return m_ServerUserData.UserData.cbLevel;}
	//桌子号码
	virtual WORD __cdecl GetTableID() { return m_ServerUserData.UserStatus.wTableID; }
	//椅子号码
	virtual WORD __cdecl GetChairID() { return m_ServerUserData.UserStatus.wChairID; }
	//用户状态
	virtual BYTE __cdecl GetUserStatus() { return m_ServerUserData.UserStatus.cbUserStatus; }
	//用户体系正确
	virtual bool __cdecl IsUserTreeFrameOK();
	//判断机器人
	virtual bool __cdecl IsAIUser();

	//统计接口
public:
	//游戏时间
	virtual DWORD __cdecl GetPlayTimeCount() { return m_dwPlayTimeCount; }
	//在线时间
	virtual DWORD __cdecl GetOnlineTimeCount() { return (DWORD)time(NULL)-m_dwLogonTime; }
	//信用额度信息
	virtual bool __cdecl GetUserScoreInfo(tagUserScore & ScoreResultInfo);

	//功能接口
public:
	//设置规则
	virtual bool __cdecl SetUserRule(tagUserRule & UserRule);
	//修改信用额度
    virtual bool __cdecl WriteScore(tagScoreInfo & ScoreInfo, DWORD dwPlayTimeCount);
	//设置状态
	virtual bool __cdecl SetUserStatus(BYTE cbUserStatus, WORD wTableID, WORD wChairID);

	//功能函数
public:
	//初始化
	void InitServerUserItem();

	//计算辅助接口
public:
	virtual DWORD __cdecl GetCalculateItemPos(WORD wTableID);
	virtual void  __cdecl SetCalculateItemPos(WORD wTableID, DWORD dwItemPos);
};

//////////////////////////////////////////////////////////////////////////

//类说明
typedef CArrayTemplate<CServerUserItem *> CServerUserItemArray;

//服务器用户管理
class CServerUserManager : public IServerUserManager
{
	//变量定义
protected:
	CServerUserItemArray				m_UserItemStore;				//存储用户
	CServerUserItemArray				m_OnLineUserItem;				//在线用户
	CServerUserItemArray				m_OffLineUserItem;				//断线用户

	//函数定义
public:
	//构造函数
	CServerUserManager(void);
	//析构函数
	virtual ~CServerUserManager(void);

	//基础接口
public:
	//释放对象
	virtual bool __cdecl Release() { if (IsValid()) delete this; return true; }
	//是否有效
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CServerUserManager))?true:false; }
	//接口查询
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//管理接口
public:
	//重置用户
	virtual bool __cdecl ResetUserManager();
	//切换用户
	virtual bool __cdecl SwitchOnLineUserItem(IServerUserItem * pIServerUserItem, DWORD dwClientIP, WORD wUserIndex);
	//激活用户
	virtual bool __cdecl ActiveOffLineUserItem(IServerUserItem * pIServerUserItem, DWORD dwClientIP, WORD wUserIndex);
	//激活用户
	virtual IServerUserItem * __cdecl ActiveUserItem(tagServerUserData & ServerUserData, DWORD dwClientIP, WORD wUserIndex, TCHAR szPassword[PASS_LEN]);
	//激活用户
	virtual IServerUserItem * __cdecl ActiveLinkUserItem(tagServerUserData & ServerUserData);
	//删除用户
	virtual bool __cdecl DeleteUserItem(IServerUserItem * pIServerUserItem);
	//注册断线
	virtual bool __cdecl RegOffLineUserItem(IServerUserItem * pIServerUserItem);
	//删除断线
	virtual bool __cdecl UnRegOffLineUserItem(IServerUserItem * pIServerUserItem);
	//检测用户计数
	virtual bool __cdecl CheckGameUserRefCount(IServerUserItem * pIServerUserItem);

	//查找接口
public:
	//枚举用户
	virtual IServerUserItem * __cdecl EnumOnLineUser(WORD wEnumIndex);
	//枚举用户
	virtual IServerUserItem * __cdecl EnumOffLineUser(WORD wEnumIndex);
	//查找用户
	virtual IServerUserItem * __cdecl SearchOnLineUser(DWORD dwUserID);
	//查找用户
	virtual IServerUserItem * __cdecl SearchOffLineUser(DWORD dwUserID);
	//查找用户
	virtual IServerUserItem * __cdecl SearchOnLineUser(LPCTSTR pszAccounts);
	//查找用户
	virtual IServerUserItem * __cdecl SearchOffLineUser(LPCTSTR pszAccounts);
	//查找用户
	virtual IServerUserItem * __cdecl SearchUser(DWORD dwUserID){
		IServerUserItem* pItem = SearchOnLineUser(dwUserID);
		if(pItem)
			return pItem;
		return SearchOffLineUser(dwUserID);
	}

	//信息接口
public:
	//在线人数
	virtual DWORD __cdecl GetOnLineCount() { return (DWORD)m_OnLineUserItem.GetCount(); }
	//断线人数
	virtual DWORD __cdecl GetOffLineCount() { return (DWORD)m_OffLineUserItem.GetCount(); }
};

//////////////////////////////////////////////////////////////////////////

#endif