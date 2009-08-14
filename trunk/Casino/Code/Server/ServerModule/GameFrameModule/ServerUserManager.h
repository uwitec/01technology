#ifndef SERVER_USER_MANAGER_HEAD_FILE
#define SERVER_USER_MANAGER_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "GameServiceExport.h"

typedef CMap<DWORD,DWORD,DWORD,DWORD> CalculateItemPosMap;
//////////////////////////////////////////////////////////////////////////

//�û���Ϣ��
class CServerUserItem : public IServerUserItem
{
	//��Ԫ����
	friend class CServerUserManager;

	//������Ϣ
protected:
	bool								m_bAcitve;						//�����־
	tagUserRule							m_UserRule;						//�û�����
	tagServerUserData					m_ServerUserData;				//�û���Ϣ
	IServerUserItem*					m_pParent;						//����
	LONG								m_lUserRefCount;			//�������ü���
	//������Ϣ
protected:
	WORD								m_wUserIndex;					//�û�����
	DWORD								m_dwClientIP;					//���ӵ�ַ
	DWORD								m_dwLogonTime;					//��¼ʱ��
	DWORD								m_dwPlayTimeCount;				//��Ϸʱ��
	TCHAR								m_szPassword[PASS_LEN];			//�û�����

	CalculateItemPosMap					m_CalculateItemPosMap;			//���㸨��

	//��������
public:
	//���캯��
	CServerUserItem(void);
	//��������
	virtual ~CServerUserItem(void);

	//�����ӿ�
public:
	//�ͷŶ���
	virtual bool __cdecl Release() { if (IsValid()) delete this; return true; }
	//�Ƿ���Ч
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CServerUserItem))?true:false; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//���ýӿ�
public:
	//��������
	virtual void __cdecl AddUserRefCount();
	//��������
	virtual void __cdecl ReleaseUserRefCount();
	//����Ϊ��
	virtual bool __cdecl IsUserRefCountZero();
	//��Ϣ�ӿ�
public:
	//�Ƿ񼤻�
	virtual bool __cdecl IsActive() { return m_bAcitve; }
	//�û�����
	virtual WORD __cdecl GetUserIndex() { return m_wUserIndex; }
	//��ȡ��ַ
	virtual DWORD __cdecl GetClientIP() { return m_dwClientIP; }
	//��ȡ����
	virtual LPCTSTR __cdecl GetPassword() { return m_szPassword; }
	//��ȡ����
	virtual const tagUserRule * __cdecl GetUserRule() { return &m_UserRule; }
	//��ȡ���ö��
	virtual  tagUserScore * __cdecl GetUserScore() { return &m_ServerUserData.UserData.UserScore;}
	//��ȡ��Ϣ
	virtual tagServerUserData * __cdecl GetUserData() { return &m_ServerUserData; };
	//��ȡ���û�
	virtual IServerUserItem * __cdecl GetParent(){return m_pParent;}
	//���ø��û�
	virtual bool  __cdecl SetParent(IServerUserItem* pParent);

	//���Խӿ�
public:
	//�û�����
	virtual LPCTSTR __cdecl GetAccounts() { return m_ServerUserData.UserData.szAccount; }
	//�û� I D
	virtual DWORD __cdecl GetUserID() { return m_ServerUserData.UserData.dwUserID; }
	//���û� I D
	virtual DWORD __cdecl GetParentUserID(){return m_ServerUserData.UserData.dwParentUserID;}
	//�û��ȼ�
	virtual BYTE __cdecl GetUserLevel(){ return m_ServerUserData.UserData.cbLevel;}
	//���Ӻ���
	virtual WORD __cdecl GetTableID() { return m_ServerUserData.UserStatus.wTableID; }
	//���Ӻ���
	virtual WORD __cdecl GetChairID() { return m_ServerUserData.UserStatus.wChairID; }
	//�û�״̬
	virtual BYTE __cdecl GetUserStatus() { return m_ServerUserData.UserStatus.cbUserStatus; }
	//�û���ϵ��ȷ
	virtual bool __cdecl IsUserTreeFrameOK();
	//�жϻ�����
	virtual bool __cdecl IsAIUser();

	//ͳ�ƽӿ�
public:
	//��Ϸʱ��
	virtual DWORD __cdecl GetPlayTimeCount() { return m_dwPlayTimeCount; }
	//����ʱ��
	virtual DWORD __cdecl GetOnlineTimeCount() { return (DWORD)time(NULL)-m_dwLogonTime; }
	//���ö����Ϣ
	virtual bool __cdecl GetUserScoreInfo(tagUserScore & ScoreResultInfo);

	//���ܽӿ�
public:
	//���ù���
	virtual bool __cdecl SetUserRule(tagUserRule & UserRule);
	//�޸����ö��
    virtual bool __cdecl WriteScore(tagScoreInfo & ScoreInfo, DWORD dwPlayTimeCount);
	//����״̬
	virtual bool __cdecl SetUserStatus(BYTE cbUserStatus, WORD wTableID, WORD wChairID);

	//���ܺ���
public:
	//��ʼ��
	void InitServerUserItem();

	//���㸨���ӿ�
public:
	virtual DWORD __cdecl GetCalculateItemPos(WORD wTableID);
	virtual void  __cdecl SetCalculateItemPos(WORD wTableID, DWORD dwItemPos);
};

//////////////////////////////////////////////////////////////////////////

//��˵��
typedef CArrayTemplate<CServerUserItem *> CServerUserItemArray;

//�������û�����
class CServerUserManager : public IServerUserManager
{
	//��������
protected:
	CServerUserItemArray				m_UserItemStore;				//�洢�û�
	CServerUserItemArray				m_OnLineUserItem;				//�����û�
	CServerUserItemArray				m_OffLineUserItem;				//�����û�

	//��������
public:
	//���캯��
	CServerUserManager(void);
	//��������
	virtual ~CServerUserManager(void);

	//�����ӿ�
public:
	//�ͷŶ���
	virtual bool __cdecl Release() { if (IsValid()) delete this; return true; }
	//�Ƿ���Ч
	virtual bool __cdecl IsValid() { return AfxIsValidAddress(this,sizeof(CServerUserManager))?true:false; }
	//�ӿڲ�ѯ
	virtual void * __cdecl QueryInterface(const IID & Guid, DWORD dwQueryVer);

	//����ӿ�
public:
	//�����û�
	virtual bool __cdecl ResetUserManager();
	//�л��û�
	virtual bool __cdecl SwitchOnLineUserItem(IServerUserItem * pIServerUserItem, DWORD dwClientIP, WORD wUserIndex);
	//�����û�
	virtual bool __cdecl ActiveOffLineUserItem(IServerUserItem * pIServerUserItem, DWORD dwClientIP, WORD wUserIndex);
	//�����û�
	virtual IServerUserItem * __cdecl ActiveUserItem(tagServerUserData & ServerUserData, DWORD dwClientIP, WORD wUserIndex, TCHAR szPassword[PASS_LEN]);
	//�����û�
	virtual IServerUserItem * __cdecl ActiveLinkUserItem(tagServerUserData & ServerUserData);
	//ɾ���û�
	virtual bool __cdecl DeleteUserItem(IServerUserItem * pIServerUserItem);
	//ע�����
	virtual bool __cdecl RegOffLineUserItem(IServerUserItem * pIServerUserItem);
	//ɾ������
	virtual bool __cdecl UnRegOffLineUserItem(IServerUserItem * pIServerUserItem);
	//����û�����
	virtual bool __cdecl CheckGameUserRefCount(IServerUserItem * pIServerUserItem);

	//���ҽӿ�
public:
	//ö���û�
	virtual IServerUserItem * __cdecl EnumOnLineUser(WORD wEnumIndex);
	//ö���û�
	virtual IServerUserItem * __cdecl EnumOffLineUser(WORD wEnumIndex);
	//�����û�
	virtual IServerUserItem * __cdecl SearchOnLineUser(DWORD dwUserID);
	//�����û�
	virtual IServerUserItem * __cdecl SearchOffLineUser(DWORD dwUserID);
	//�����û�
	virtual IServerUserItem * __cdecl SearchOnLineUser(LPCTSTR pszAccounts);
	//�����û�
	virtual IServerUserItem * __cdecl SearchOffLineUser(LPCTSTR pszAccounts);
	//�����û�
	virtual IServerUserItem * __cdecl SearchUser(DWORD dwUserID){
		IServerUserItem* pItem = SearchOnLineUser(dwUserID);
		if(pItem)
			return pItem;
		return SearchOffLineUser(dwUserID);
	}

	//��Ϣ�ӿ�
public:
	//��������
	virtual DWORD __cdecl GetOnLineCount() { return (DWORD)m_OnLineUserItem.GetCount(); }
	//��������
	virtual DWORD __cdecl GetOffLineCount() { return (DWORD)m_OffLineUserItem.GetCount(); }
};

//////////////////////////////////////////////////////////////////////////

#endif