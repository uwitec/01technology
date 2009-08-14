#include "StdAfx.h"
#include "ServerUserManager.h"
#include "..\..\CommonDefinition\GlobalAI.h"
//////////////////////////////////////////////////////////////////////////

//���캯��
CServerUserItem::CServerUserItem(void)
{
	InitServerUserItem();
}

//��������
CServerUserItem::~CServerUserItem(void)
{
}

//�ӿڲ�ѯ
void * __cdecl CServerUserItem::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IServerUserItem,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IServerUserItem,Guid,dwQueryVer);
	return NULL;
}
void __cdecl  CServerUserItem::AddUserRefCount()
{
	m_lUserRefCount++;
}
void __cdecl  CServerUserItem::ReleaseUserRefCount()
{
	m_lUserRefCount--;
}
bool __cdecl  CServerUserItem::IsUserRefCountZero()
{
//	GT_ASSERT(m_lUserRefCount >= 0);
	return m_lUserRefCount <= 0;
}

//���ø��û�
bool  __cdecl CServerUserItem::SetParent(IServerUserItem* pParent)
{
	if(this->GetUserData()->UserData.cbLevel == 0)
		return pParent == NULL ? true : false;
	else
	{
		if(pParent == NULL)
		{
			if(GetParent())
				 GetParent()->ReleaseUserRefCount();
			m_pParent = NULL;
			return true;
		}
		else
		{
			if(this->GetParentUserID() == pParent->GetUserID())
			{
				if(m_pParent == pParent)
					return true;
				else
				{
					if(m_pParent)
						m_pParent->ReleaseUserRefCount();
					m_pParent = pParent;
					m_pParent->AddUserRefCount();
					return true;
				}
			}
			else
			{
				GT_ASSERT(GetParent());
				if(GetParent())
					return GetParent()->SetParent(pParent);
				else
					return false;
			}
		}
	
	}
}
//�ж��û���ϵ��ȷ
bool __cdecl CServerUserItem::IsUserTreeFrameOK()
{
	if(this->GetUserData()->UserData.cbLevel == 0)
		return true;
	else
	{
		//GT_ASSERT(GetParent());
		if(GetParent())
			return GetParent()->IsUserTreeFrameOK();
		else
			return false;
	}
}
//�жϻ�����
bool __cdecl CServerUserItem::IsAIUser()
{
	if(CAIUtil::IsAIUser(this->m_ServerUserData.UserData.cbState))
		return true;
	else
		return false;
}
//���ù���
bool __cdecl CServerUserItem::SetUserRule(tagUserRule & UserRule)
{
	//Ч�����
	GT_ASSERT(m_bAcitve==true);
	if (m_bAcitve==false) return false;

	//���ù���
	m_UserRule=UserRule;

	return true;
}

//�޸����ö��
bool __cdecl CServerUserItem::WriteScore(tagScoreInfo & ScoreInfo, DWORD dwPlayTimeCount)
{
	//Ч�����
	GT_ASSERT(m_bAcitve==true);
	if (m_bAcitve==false) return false;

	//�޸����ö��
	switch (ScoreInfo.ScoreKind)
	{
	case enScoreKind_Win: 
		{
			break;
		}
	case enScoreKind_Lost: 
		{
			break;
		}
	case enScoreKind_Draw:
		{
			break;
		}
	case enScoreKind_Flee:
		{
			break;
		}
	}

	//�޸����ö��
	m_ServerUserData.UserData.UserScore.fScore+=ScoreInfo.fScore;

	//��Ϸʱ��
	m_dwPlayTimeCount+=dwPlayTimeCount;

	return true;
}

//����״̬
bool __cdecl CServerUserItem::SetUserStatus(BYTE cbUserStatus, WORD wTableID, WORD wChairID)
{
	//Ч��״̬
	GT_ASSERT(m_bAcitve==true);
	if (m_bAcitve==false) return false;

	//���ñ���
	m_ServerUserData.UserStatus.wTableID=wTableID;
	m_ServerUserData.UserStatus.wChairID=wChairID;
	m_ServerUserData.UserStatus.cbUserStatus=cbUserStatus;

	return true;
}

//��ʼ��
void CServerUserItem::InitServerUserItem()
{
	m_lUserRefCount=0;	
	m_pParent = NULL;
	m_bAcitve=false;
	m_dwClientIP=0L;
	m_szPassword[0]=0;
	m_wUserIndex=0xFFFF;
	m_dwPlayTimeCount=0L;
	m_dwLogonTime=(LONG)time(NULL);
	memset(&m_UserRule,0,sizeof(m_UserRule));
	memset(&m_ServerUserData,0,sizeof(m_ServerUserData));

	AddUserRefCount();

	return;
}

//���ö����Ϣ
bool __cdecl CServerUserItem::GetUserScoreInfo(tagUserScore & ScoreResultInfo)
{
	//Ч��״̬
	GT_ASSERT(m_bAcitve==true);
	if (m_bAcitve==false) return false;

	//���ñ���
	ScoreResultInfo=m_ServerUserData.UserData.UserScore;

	return true;;
}
//���㸨���ӿ�

DWORD __cdecl CServerUserItem::GetCalculateItemPos(WORD wTableID)
{
	CalculateItemPosMap::CPair* pPair = m_CalculateItemPosMap.PLookup(wTableID);
	if(pPair!=NULL)
		return pPair->value;
	else
		return INVALID_CALCULATEITEMPOS;
}
void  __cdecl CServerUserItem::SetCalculateItemPos(WORD wTableID, DWORD dwItemPos)
{
	m_CalculateItemPosMap[wTableID] = dwItemPos;
}
//////////////////////////////////////////////////////////////////////////

//���캯��
CServerUserManager::CServerUserManager(void)
{
}

//��������
CServerUserManager::~CServerUserManager(void)
{
	//�ͷ��û�
	CServerUserItem * pServerUserItem=NULL;
	for (INT_PTR i=0;i<m_UserItemStore.GetCount();i++)
	{
		pServerUserItem=m_UserItemStore[i];
		GT_ASSERT(pServerUserItem!=NULL);
		SafeDelete(pServerUserItem);
	}
	for (INT_PTR i=0;i<m_OnLineUserItem.GetCount();i++)
	{
		pServerUserItem=m_OnLineUserItem[i];
		GT_ASSERT(pServerUserItem!=NULL);
		SafeDelete(pServerUserItem);
	}
	for (INT_PTR i=0;i<m_OffLineUserItem.GetCount();i++)
	{
		pServerUserItem=m_OffLineUserItem[i];
		GT_ASSERT(pServerUserItem!=NULL);
		SafeDelete(pServerUserItem);
	}
	m_UserItemStore.RemoveAll();
	m_OnLineUserItem.RemoveAll();
	m_OffLineUserItem.RemoveAll();

	return;
}

//�ӿڲ�ѯ
void * __cdecl CServerUserManager::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(IServerUserManager,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(IServerUserManager,Guid,dwQueryVer);
	return NULL;
}

//�����û�
bool __cdecl CServerUserManager::ResetUserManager()
{
	//�ָ�����
	m_UserItemStore.Append(m_OnLineUserItem);
	m_UserItemStore.Append(m_OffLineUserItem);
	m_OnLineUserItem.RemoveAll();
	m_OffLineUserItem.RemoveAll();
	
	return true;
}

//�л��û�
bool __cdecl CServerUserManager::SwitchOnLineUserItem(IServerUserItem * pIServerUserItem, DWORD dwClientIP, WORD wUserIndex)
{
	//Ч�����
	GT_ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//Ѱ���û�
	CServerUserItem * pServerUserItem=NULL;
	for (INT_PTR i=0;i<m_OnLineUserItem.GetCount();i++)
	{
		pServerUserItem=m_OnLineUserItem[i];
		GT_ASSERT(pServerUserItem!=NULL);
		if (pServerUserItem==pIServerUserItem)
		{
			//�����û�
			pServerUserItem->m_dwClientIP=dwClientIP;
			pServerUserItem->m_wUserIndex=wUserIndex;

			return true;
		}
	}
	GT_ASSERT(FALSE);

	return true;
}

//�����û�
bool __cdecl CServerUserManager::ActiveOffLineUserItem(IServerUserItem * pIServerUserItem, DWORD dwClientIP, WORD wUserIndex)
{
	//Ч�����
	GT_ASSERT(pIServerUserItem!=NULL);
	
	if (pIServerUserItem==NULL) return false;

	//Ѱ���û�
	CServerUserItem * pServerUserItem=NULL;
	for (INT_PTR i=0;i<m_OffLineUserItem.GetCount();i++)
	{
		pServerUserItem=m_OffLineUserItem[i];
		GT_ASSERT(pServerUserItem!=NULL);
		if (pServerUserItem==pIServerUserItem)
		{
			//�����û�
			pServerUserItem->m_dwClientIP=dwClientIP;
			pServerUserItem->m_wUserIndex=wUserIndex;
			pServerUserItem->m_bAcitve=true;

			//ע������
			pServerUserItem->AddUserRefCount();
			m_OffLineUserItem.RemoveAt(i);
			m_OnLineUserItem.Add(pServerUserItem);

			return true;
		}
	}
	GT_ASSERT(FALSE);

	return false;
}

//�����û�
IServerUserItem * __cdecl CServerUserManager::ActiveUserItem(tagServerUserData & ServerUserData, DWORD dwClientIP, WORD wUserIndex, TCHAR szPassword[PASS_LEN])
{
	//��ȡ���ж���
	CServerUserItem * pServerUserItem=NULL;
	if (m_UserItemStore.GetCount()>0)
	{
		INT_PTR nItemPostion=m_UserItemStore.GetCount()-1;
		pServerUserItem=m_UserItemStore[nItemPostion];
		GT_ASSERT(pServerUserItem!=NULL);
		m_UserItemStore.RemoveAt(nItemPostion);
	}

	//�����¶���
	if (pServerUserItem==NULL)
	{
		_BEGIN_TRY
		{
			pServerUserItem=new CServerUserItem;
			if (pServerUserItem==NULL) return NULL;
		}
		CATCH_COMMON_EXCEPTION(return NULL;)CATCH_UNKNOWN_EXCEPTION(return NULL;)_END_CATCH

	}

	//�����û�
	GT_ASSERT(pServerUserItem!=NULL);
	m_OnLineUserItem.Add(pServerUserItem);
	pServerUserItem->InitServerUserItem();

	//�����û�
	pServerUserItem->m_bAcitve=true;
	pServerUserItem->m_wUserIndex=wUserIndex;
	pServerUserItem->m_dwClientIP=dwClientIP;
	pServerUserItem->m_dwLogonTime=(LONG)time(NULL);
	CopyMemory(&pServerUserItem->m_ServerUserData,&ServerUserData,sizeof(ServerUserData));
	lstrcpyn(pServerUserItem->m_szPassword,szPassword,CountArray(pServerUserItem->m_szPassword));

	pServerUserItem->AddUserRefCount();

	return pServerUserItem;
}
//�����û�
IServerUserItem * __cdecl CServerUserManager::ActiveLinkUserItem(tagServerUserData & ServerUserData)
{
	//��ȡ���ж���
	CServerUserItem * pServerUserItem=NULL;
	if (m_UserItemStore.GetCount()>0)
	{
		INT_PTR nItemPostion=m_UserItemStore.GetCount()-1;
		pServerUserItem=m_UserItemStore[nItemPostion];
		GT_ASSERT(pServerUserItem!=NULL);
		m_UserItemStore.RemoveAt(nItemPostion);
	}

	//�����¶���
	if (pServerUserItem==NULL)
	{
		_BEGIN_TRY
		{
			pServerUserItem=new CServerUserItem;
			if (pServerUserItem==NULL) return NULL;
		}
		CATCH_COMMON_EXCEPTION(return NULL;)CATCH_UNKNOWN_EXCEPTION(return NULL;)_END_CATCH

	}

	//�����û�
	GT_ASSERT(pServerUserItem!=NULL);
	m_OffLineUserItem.Add(pServerUserItem);
	pServerUserItem->InitServerUserItem();

	//�����û�
	pServerUserItem->m_bAcitve=false;
	pServerUserItem->m_wUserIndex=0xffff;
	pServerUserItem->m_dwClientIP=0;
	pServerUserItem->m_dwLogonTime=0;
	CopyMemory(&pServerUserItem->m_ServerUserData,&ServerUserData,sizeof(ServerUserData));

	return pServerUserItem;
}
//ö���û�
IServerUserItem * __cdecl CServerUserManager::EnumOnLineUser(WORD wEnumIndex)
{
	if (wEnumIndex>=m_OnLineUserItem.GetCount()) return NULL;
	return m_OnLineUserItem[wEnumIndex];
}

//ö���û�
IServerUserItem * __cdecl CServerUserManager::EnumOffLineUser(WORD wEnumIndex)
{
	if (wEnumIndex>=m_OffLineUserItem.GetCount()) return NULL;
	return m_OffLineUserItem[wEnumIndex];
}

//ɾ���û�
bool __cdecl CServerUserManager::DeleteUserItem(IServerUserItem * pIServerUserItem)
{
	//Ч�����
	GT_ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//��������
	CServerUserItem * pEnumServerUserItem=NULL;
	tagServerUserData * pUserData=pIServerUserItem->GetUserData();

	//Ѱ���û�
	if (pIServerUserItem->GetUserIndex()!=0xFFFF)
	{
		for (INT_PTR i=0;i<m_OnLineUserItem.GetCount();i++)
		{
			pEnumServerUserItem=m_OnLineUserItem[i];
			GT_ASSERT(pEnumServerUserItem!=NULL);
			if (pEnumServerUserItem==pIServerUserItem)
			{
				pEnumServerUserItem->ReleaseUserRefCount();

				if(pEnumServerUserItem->IsUserRefCountZero() &&
					pEnumServerUserItem->GetUserLevel() == USERLEVEL_GAMEUSER)
				{
					pEnumServerUserItem->SetParent(NULL);
					pEnumServerUserItem->m_bAcitve=false;
					m_OnLineUserItem.RemoveAt(i);
					m_UserItemStore.Add(pEnumServerUserItem);
					return true;
				}
				else
				{
					pEnumServerUserItem->m_bAcitve=false;
					m_OnLineUserItem.RemoveAt(i);
					m_OffLineUserItem.Add(pEnumServerUserItem);
					return true;
				}
			}
		}
	}
	else
	{
		for (INT_PTR i=0;i<m_OffLineUserItem.GetCount();i++)
		{
			pEnumServerUserItem=m_OffLineUserItem[i];
			GT_ASSERT(pEnumServerUserItem!=NULL);
			if (pEnumServerUserItem==pIServerUserItem)
			{
				if(pEnumServerUserItem->IsUserRefCountZero() &&
					pEnumServerUserItem->GetUserLevel() == USERLEVEL_GAMEUSER)
				{
					pEnumServerUserItem->SetParent(NULL);
					pEnumServerUserItem->m_bAcitve=false;
					m_OffLineUserItem.RemoveAt(i);
					m_UserItemStore.Add(pEnumServerUserItem);
					return true;
				}
				else
				{
					pEnumServerUserItem->m_bAcitve=false;
					return true;
				}
			}
		}
	}

	GT_ASSERT(FALSE);

	return false;
}
bool __cdecl CServerUserManager::CheckGameUserRefCount(IServerUserItem * pIServerUserItem)
{
	CServerUserItem * pEnumServerUserItem = NULL;
	for (INT_PTR i=0;i<m_OffLineUserItem.GetCount();i++)
	{
		pEnumServerUserItem=m_OffLineUserItem[i];
		GT_ASSERT(pEnumServerUserItem!=NULL);
		if (pEnumServerUserItem==pIServerUserItem)
		{
			if(pEnumServerUserItem->IsUserRefCountZero() &&
				pEnumServerUserItem->GetUserLevel() == USERLEVEL_GAMEUSER &&
				pEnumServerUserItem->m_bAcitve == false)
			{
				pEnumServerUserItem->SetParent(NULL);
				pEnumServerUserItem->m_bAcitve=false;
				m_OffLineUserItem.RemoveAt(i);
				m_UserItemStore.Add(pEnumServerUserItem);
				return true;
			}
			else
			{
				return true;
			}
		}
	}
	return false;
}
//ע�����
bool __cdecl CServerUserManager::RegOffLineUserItem(IServerUserItem * pIServerUserItem)
{
	//Ч�����
	GT_ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//��������
	CServerUserItem * pServerUserItem=NULL;
	tagServerUserData * pUserData=pIServerUserItem->GetUserData();

	//Ѱ���û�
	GT_ASSERT(pUserData->UserStatus.cbUserStatus!=US_OFFLINE);
	for (INT_PTR i=0;i<m_OnLineUserItem.GetCount();i++)
	{
		pServerUserItem=m_OnLineUserItem[i];
		GT_ASSERT(pServerUserItem!=NULL);
		if (pServerUserItem==pIServerUserItem)
		{
			pServerUserItem->ReleaseUserRefCount();
			pServerUserItem->m_wUserIndex=0xFFFF;
			m_OnLineUserItem.RemoveAt(i);
			m_OffLineUserItem.Add(pServerUserItem);
			return true;
		}
	}

	GT_ASSERT(FALSE);

	return false;
}

//ɾ������
bool __cdecl CServerUserManager::UnRegOffLineUserItem(IServerUserItem * pIServerUserItem)
{
	//Ч�����
	GT_ASSERT(pIServerUserItem!=NULL);
	if (pIServerUserItem==NULL) return false;

	//��������
	CServerUserItem * pServerUserItem=NULL;
	tagServerUserData * pUserData=pIServerUserItem->GetUserData();

	//Ѱ���û�
	for (INT_PTR i=0;i<m_OffLineUserItem.GetCount();i++)
	{
		pServerUserItem=m_OffLineUserItem[i];
		GT_ASSERT(pServerUserItem!=NULL);
		if (pServerUserItem==pIServerUserItem)
		{
			pServerUserItem->AddUserRefCount();
			m_OffLineUserItem.RemoveAt(i);
			m_OnLineUserItem.Add(pServerUserItem);
			return true;
		}
	}

	return false;
}

//�����û�
IServerUserItem * __cdecl CServerUserManager::SearchOnLineUser(DWORD dwUserID)
{
	CServerUserItem * pServerUserItem=NULL;
	for (INT_PTR i=0;i<m_OnLineUserItem.GetCount();i++)
	{
		pServerUserItem=m_OnLineUserItem[i];
		GT_ASSERT(pServerUserItem!=NULL);
		if (pServerUserItem->m_ServerUserData.UserData.dwUserID==dwUserID) return pServerUserItem;
	}

	return NULL;
}

//�����û�
IServerUserItem * __cdecl CServerUserManager::SearchOffLineUser(DWORD dwUserID)
{
	CServerUserItem * pServerUserItem=NULL;
	for (INT_PTR i=0;i<m_OffLineUserItem.GetCount();i++)
	{
		pServerUserItem=m_OffLineUserItem[i];
		GT_ASSERT(pServerUserItem!=NULL);
		if (pServerUserItem->m_ServerUserData.UserData.dwUserID==dwUserID) return pServerUserItem;
	}

	return NULL;
}

//�����û�
IServerUserItem * __cdecl CServerUserManager::SearchOnLineUser(LPCTSTR pszAccounts)
{
	CString strAccounts=pszAccounts,strAccountsTemp;
	CServerUserItem * pServerUserItem=NULL;
	for (INT_PTR i=0;i<m_OnLineUserItem.GetCount();i++)
	{
		pServerUserItem=m_OnLineUserItem[i];
		GT_ASSERT(pServerUserItem!=NULL);
		strAccountsTemp=pServerUserItem->m_ServerUserData.UserData.szAccount;
		if (strAccounts==strAccountsTemp) return pServerUserItem;
	}

	return NULL;
}

//�����û�
IServerUserItem * __cdecl CServerUserManager::SearchOffLineUser(LPCTSTR pszAccounts)
{
	CString strAccounts=pszAccounts,strAccountsTemp;
	CServerUserItem * pServerUserItem=NULL;
	for (INT_PTR i=0;i<m_OffLineUserItem.GetCount();i++)
	{
		pServerUserItem=m_OffLineUserItem[i];
		GT_ASSERT(pServerUserItem!=NULL);
		strAccountsTemp=pServerUserItem->m_ServerUserData.UserData.szAccount;
		if (strAccounts==strAccountsTemp) return pServerUserItem;
	}

	return NULL;
}

//////////////////////////////////////////////////////////////////////////