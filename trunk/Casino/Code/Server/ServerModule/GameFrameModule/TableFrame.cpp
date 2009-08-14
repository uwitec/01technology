#include "StdAfx.h"
#include "TableFrame.h"
#include "..\..\CommonDefinition\CHINESECODELIB.H"
//////////////////////////////////////////////////////////////////////////

//��ʱ�� ID
#define IDI_OFFLINE					(IDI_MAX_TIME_ID+1)					//���߶�ʱ��


//////////////////////////////////////////////////////////////////////////

//���캯��
CTableFrame::CTableFrame()
{
	//���Ա���
	m_wChairCount=0;
	m_wTableID=INVALID_TABLE;

	//״̬����
	m_bLocked=false;
	m_bStarted=false;
	m_ullTimeStart=0L;
	m_dwTableOwnerID=0L;
	m_bGameStatus=GS_FREE;
	m_szPassword[0]=0;
	m_wUserCount=0;
	m_wAIGameUserCount=0;

	//�����Ϣ
	m_bWriteScore=false;

	//���ñ���
	m_pGameServiceAttrib=NULL;
	m_pGameServiceOption=NULL;

	//�ӿڱ���
	m_pITableFrameSink=NULL;
	m_pITableFrameEvent=NULL;
	m_pIGameServiceFrame=NULL;
	
	m_dwPlayerID		 = NULL;
	m_bCheckBetScoreFlag = NULL;
	m_bAllowLookon		 = NULL;
	m_wOffLineCount		 = NULL;
	m_pIUserItem		 = NULL;

	m_ullGameRoundID	 = 0;

	return;
}

//��������
CTableFrame::~CTableFrame()
{
	//�ͷŶ���
	if (m_pITableFrameSink!=NULL)
	{
		m_pITableFrameSink->Release();
		m_pITableFrameSink=NULL;
	}

	SafeDeleteArray(m_dwPlayerID);
	SafeDeleteArray(m_bCheckBetScoreFlag);
	SafeDeleteArray(m_pIUserItem);
	SafeDeleteArray(m_bAllowLookon);
	SafeDeleteArray(m_wOffLineCount);

	return;
}

//�ӿڲ�ѯ
void * __cdecl CTableFrame::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrame,Guid,dwQueryVer);
	QUERYINTERFACE(ITableFrameManager,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameManager,Guid,dwQueryVer);
	return NULL;
}

//��ʼ��
bool __cdecl CTableFrame::InitTableFrame(WORD wTableID, tagTableFrameParameter * pTableFrameParameter)
{
	//Ч�����
	GT_ASSERT(pTableFrameParameter!=NULL);

	//���ñ���
	m_wTableID=wTableID;
	m_pGameServiceAttrib=pTableFrameParameter->pGameServiceAttrib;
	m_pGameServiceOption=pTableFrameParameter->pGameServiceOption;
	m_wChairCount=pTableFrameParameter->pGameServiceAttrib->wChairCount;

	//��ѯ�ӿ�
	m_pIGameServiceFrame=GET_OBJECTPTR_INTERFACE(pTableFrameParameter->pIGameServiceFrame,IGameServiceFrame);
	GT_ASSERT(m_pIGameServiceFrame!=NULL);
	if (m_pIGameServiceFrame==NULL) return false;

	//��������
	IGameServiceManager * pIGameServiceManager=GET_OBJECTPTR_INTERFACE(pTableFrameParameter->pIGameServiceManager,IGameServiceManager);
	m_pITableFrameSink=(ITableFrameSink *)pIGameServiceManager->CreateTableFrameSink(IID_ITableFrameSink,VER_ITableFrameSink);
	GT_ASSERT(m_pITableFrameSink!=NULL);
	if (m_pITableFrameSink==NULL) return false;

	//���ýӿ�
	IUnknownEx * pIUnknownEx=GET_MYSELF_INTERFACE(IUnknownEx);
	if (m_pITableFrameSink->InitTableFrameSink(pIUnknownEx)==false) return false;

	//���ݿ�����
	IDataBaseEngine* pIDataBaseEngine=GET_OBJECTPTR_INTERFACE(pTableFrameParameter->pIDataBaseEngine,IDataBaseEngine);
	if (pIDataBaseEngine==NULL)
	{
		TRACE(TEXT("��ȡ���ݿ��������ʧ��"));
		return false;
	}
	//���ݿⴥ��
	IUnknownEx * pIQueueService=(IUnknownEx *)pIDataBaseEngine->GetQueueService(IID_IUnknownEx,VER_IUnknownEx);
	if (m_DataBaseEvent.SetQueueService(pIQueueService)==false)
	{
		return false;
	}

	//��չ�ӿ�
	m_pITableFrameEvent=GET_OBJECTPTR_INTERFACE(m_pITableFrameSink,ITableFrameEvent);

	m_CalculateFrame.InitCalculateFrame(wTableID, this, pTableFrameParameter);

	//�����ڴ�
	//�û���Ϣ
	m_dwPlayerID = new DWORD[m_wChairCount];
	m_bCheckBetScoreFlag = new bool[m_wChairCount];
	m_pIUserItem = new IServerUserItem*[m_wChairCount];
	m_bAllowLookon = new bool[m_wChairCount];
	m_wOffLineCount = new WORD[m_wChairCount];

	memset(m_dwPlayerID,0,sizeof(DWORD) * m_wChairCount);
	memset(m_bCheckBetScoreFlag,0,sizeof(bool) * m_wChairCount);
	memset(m_pIUserItem,0,sizeof(IServerUserItem*)*m_wChairCount);
	memset(m_bAllowLookon,0,sizeof(bool)*m_wChairCount);
	memset(m_wOffLineCount,0,sizeof(WORD)*m_wChairCount);

	return true;
}

//��Ϸ״̬
bool __cdecl CTableFrame::IsUserPlaying(IServerUserItem * pIServerUserItem)
{
	//��Ϸ״̬
	if (m_bStarted==false) return false;

	//�û�״̬
	BYTE cbUserStatus=pIServerUserItem->GetUserStatus();
	if ((cbUserStatus!=US_PLAY)&&(cbUserStatus!=US_OFFLINE)) return false;

	//�߼��ж�
	WORD wChairID=pIServerUserItem->GetChairID();
	return m_pITableFrameSink->IsUserPlaying(wChairID);
}
WORD __cdecl CTableFrame::GetNullChairID()
{
	for (WORD i=0;i<m_wChairCount;i++)
	{
		if (m_pIUserItem[i]==NULL)
			return i;
	}
	return INVALID_CHAIR;
}
WORD __cdecl CTableFrame::GetSameIPChairID(IServerUserItem * pIServerUserItem)
{
	DWORD dwUserIP=pIServerUserItem->GetClientIP();
	for (WORD i=0;i<m_wChairCount;i++)
	{
		if ((m_pIUserItem[i]!=NULL)&&(m_pIUserItem[i]->GetClientIP()==dwUserIP))
		{
			return i;
		}
	}
	return INVALID_CHAIR;
}
//��ȡAI��ɱ����ͨ���������
WORD __cdecl CTableFrame::GetAIKillGameUserChairID()
{
	for (WORD i=0;i<m_wChairCount;i++)
	{
		if (m_pIUserItem[i]!=NULL && m_pIUserItem[i]->IsAIUser() == FALSE)
			return i;
	}
	return INVALID_CHAIR;
}
//�Ƿ�AI��ɱ��ͨ���
bool __cdecl CTableFrame::IsAIKillGameUser()
{
	//�ж�ȫ��AI������
	if(GetAIGameUserCount() && GetGameUserCount() == 0)
		return false;
	//�ж�ȫ����ͨ���
	if(GetGameUserCount() && GetAIGameUserCount() == 0)
		return false;
	//ֻ��һλ��ͨ���
	return ( GetGameUserCount() == 1 && GetAIGameUserCount() ) ? true : false;
}

//�뿪����
bool __cdecl CTableFrame::PerformStandUpAction(IServerUserItem * pIServerUserItem)
{
	//Ч�����
	GT_ASSERT(pIServerUserItem!=NULL);
	GT_ASSERT(pIServerUserItem->GetTableID()==m_wTableID);
	GT_ASSERT(pIServerUserItem->GetChairID()<m_wChairCount);

	//��������
	tagServerUserData * pUserData=pIServerUserItem->GetUserData();
	WORD wChairID=pUserData->UserStatus.wChairID;
	bool bLookonUser=(pIServerUserItem->GetUserStatus()==US_LOOKON);

	//�û�����
	if (bLookonUser==true)
	{
		IServerUserItem * pILookonUserItem=NULL;
		for (INT_PTR i=0;i<m_LookonUserItemPtr.GetCount();i++)
		{
			pILookonUserItem=m_LookonUserItemPtr[i];
			GT_ASSERT(pILookonUserItem!=NULL);
			if (pILookonUserItem==pIServerUserItem)
			{
				//��������
				if (m_pITableFrameEvent!=NULL) m_pITableFrameEvent->OnEventUserReqStandUp(wChairID,pIServerUserItem,false);

				//�����û�
				m_LookonUserItemPtr.RemoveAt(i);
				pIServerUserItem->SetUserStatus(US_FREE,INVALID_TABLE,INVALID_CHAIR);
				m_pIGameServiceFrame->SendUserStatus(pIServerUserItem);

				//��������
				if (m_pITableFrameEvent!=NULL) m_pITableFrameEvent->OnEventUserStandUp(wChairID,pIServerUserItem,true);

				return m_CalculateFrame.OnEventUserStandUp(wChairID,pIServerUserItem,true);
			}
		}
	}
	else
	{
		//��������
		if (m_pITableFrameEvent!=NULL) m_pITableFrameEvent->OnEventUserReqStandUp(wChairID,pIServerUserItem,false);

		//������Ϸ
		if (IsUserPlaying(pIServerUserItem)==true)
		{
			m_pITableFrameSink->OnEventGameEnd(pUserData->UserStatus.wChairID,pIServerUserItem,GER_USER_LEFT);
		}

		//�뿪�ж�
		if ((pUserData->UserStatus.wTableID==INVALID_TABLE)||(pUserData->UserStatus.wChairID==INVALID_CHAIR)) return true;

		//�����û�
		m_pIUserItem[pUserData->UserStatus.wChairID]=NULL;
		pIServerUserItem->SetUserStatus(US_FREE,INVALID_TABLE,INVALID_CHAIR);
		m_pIGameServiceFrame->SendUserStatus(pIServerUserItem);

		//��������
		if (m_dwTableOwnerID==pUserData->UserData.dwUserID) m_dwTableOwnerID=0L;

		//������Ϣ
		WORD wUserCount=0;
		m_wAIGameUserCount = 0;
		for (WORD i=0;i<m_wChairCount;i++)
		{
			if (m_pIUserItem[i]!=NULL)
			{
				wUserCount++;
				if(m_pIUserItem[i]->IsAIUser())
				{
					m_wAIGameUserCount ++;
				}
			}
		}
		m_wUserCount = wUserCount;
		if (wUserCount==0)
		{
			m_bLocked=false;
			m_szPassword[0]=0;
			m_pIGameServiceFrame->SendTableStatus(m_wTableID);
		}

		//���´���
		if (m_pITableFrameEvent!=NULL) m_pITableFrameEvent->OnEventUserStandUp(wChairID,pIServerUserItem,false);
		m_CalculateFrame.OnEventUserStandUp(wChairID,pIServerUserItem,false);
		
		//��ʼ�ж�
		if ((StartVerdict()==true))
		{
			TryStartGame();
			return true;
		}

		return true;
	}

	return true;
}
//�˹�������Ϸ����
bool __cdecl CTableFrame::PerformAIGameAction()
{
	//Ч�����
	if(m_pITableFrameSink) return m_pITableFrameSink->OnPerformAIGameAction();
	else return false;
}
//�Թ۶���
bool __cdecl CTableFrame::PerformLookonAction(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	//Ч�����
	GT_ASSERT(pIServerUserItem!=NULL);
	GT_ASSERT(wChairID<m_wChairCount);
	GT_ASSERT(pIServerUserItem->GetTableID()==INVALID_TABLE);
	GT_ASSERT(pIServerUserItem->GetChairID()==INVALID_CHAIR);

	//��������
	DWORD dwMasterRight=pIServerUserItem->GetUserData()->UserData.dwMasterRight;

	//�رղ�ѯ
	if ((dwMasterRight==0L)&&(m_pIGameServiceFrame->IsShallClose()==true))
	{
		SendSitFailedPacket(pIServerUserItem,ErrCode_CMD_GR_SitFailed_RoomShallClose);
		return false;
	}

	//��ֹ��ѯ
	if ((m_pIGameServiceFrame->IsAllowEnterGame()==false))
	{
		SendSitFailedPacket(pIServerUserItem,ErrCode_CMD_GR_SitFailed_AllowEnterGame);
		return false;
	}

	//λ���ж�
	if (m_pIUserItem[wChairID]==NULL)
	{
		SendSitFailedPacket(pIServerUserItem,ErrCode_CMD_GR_SitFailed_NoLookonUser);
		return false;
	}

	//����Ч��
	if (m_bLocked==true)
	{
		const tagUserRule * pUserRule=pIServerUserItem->GetUserRule();
		if ((pUserRule->bPassword==false)||(lstrcmp(m_szPassword,pUserRule->szPassword)!=0))
		{
			SendSitFailedPacket(pIServerUserItem,ErrCode_CMD_GR_SitFailed_Password);
			return false;
		}
	}

	//״̬�ж�
	if ((m_bStarted==false)&&(dwMasterRight==0L))
	{
		SendSitFailedPacket(pIServerUserItem,ErrCode_CMD_GR_SitFailed_PlayingGame);
		return false;
	}

	//�Թ۴���
	if (m_pITableFrameEvent!=NULL) m_pITableFrameEvent->OnEventUserReqSitDown(wChairID,pIServerUserItem,true);

	//�������
	m_LookonUserItemPtr.Add(pIServerUserItem);
	pIServerUserItem->SetUserStatus(US_LOOKON,m_wTableID,wChairID);
	m_pIGameServiceFrame->SendUserStatus(pIServerUserItem);

	//�Թ۴���
	if (m_pITableFrameEvent!=NULL) m_pITableFrameEvent->OnEventUserSitDown(wChairID,pIServerUserItem,true);
	
	return m_CalculateFrame.OnEventUserSitDown(wChairID,pIServerUserItem,true);
}

//���¶���
bool __cdecl CTableFrame::PerformSitDownAction(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	//Ч�����
	GT_ASSERT(pIServerUserItem!=NULL);
	GT_ASSERT(wChairID<m_wChairCount);
	GT_ASSERT(pIServerUserItem->GetTableID()==INVALID_TABLE);
	GT_ASSERT(pIServerUserItem->GetChairID()==INVALID_CHAIR);

	//��������
	const tagUserScore * pUserScore=pIServerUserItem->GetUserScore();;
	const tagUserRule * pUserRule=pIServerUserItem->GetUserRule(),* pTableUserRule=NULL;
	tagServerUserData * pUserData=pIServerUserItem->GetUserData(),* pTableUserData=NULL;

	//�رղ�ѯ
	if ((m_pIGameServiceFrame->IsShallClose()==true))
	{
		SendSitFailedPacket(pIServerUserItem,ErrCode_CMD_GR_SitFailed_RoomShallClose);
		return false;
	}

	//��ֹ��ѯ
	if ((m_pIGameServiceFrame->IsAllowEnterGame()==false))
	{
		SendSitFailedPacket(pIServerUserItem,ErrCode_CMD_GR_SitFailed_AllowEnterGame);
		return false;
	}

	//λ���ж�
	if (m_pIUserItem[wChairID]!=NULL)
	{
		pTableUserData=m_pIUserItem[wChairID]->GetUserData();
		SendSitFailedPacket(pIServerUserItem,ErrCode_CMD_GR_SitFailed_OtherSitFirstly);
		return false;
	}

	//��Ϸ״̬
	if (m_bStarted==true &&
		IsBattleGameProcessType(m_pGameServiceAttrib->wProcessType))
	{
		SendSitFailedPacket(pIServerUserItem,ErrCode_CMD_GR_SitFailed_PlayingGame);
		return false;
	}


	{
		//Ȩ���ж�
		if (CUserRight::CanPlay(m_pGameServiceAttrib->wProcessType,pUserData->dwGameRight)==false)
		{
			//������Ϣ
			SendSitFailedPacket(pIServerUserItem,ErrCode_LimitRight);

			return true;
		}

		//�������
		if (m_pGameServiceOption->dwLessScore!=0.0)
		{
			if (pUserData->UserData.UserScore.fScore<m_pGameServiceOption->dwLessScore)
			{
				SendSitFailedPacket(pIServerUserItem,ErrCode_CMD_GR_SitFailed_NoEnoughScore);
				return false;
			}
		}
		

		//����Ч��
		if (m_bLocked==true)
		{
			if ((pUserRule->bPassword==false)||(lstrcmp(m_szPassword,pUserRule->szPassword)!=0))
			{
				SendSitFailedPacket(pIServerUserItem,ErrCode_CMD_GR_SitFailed_Password);
				return false;
			}
		}


		//��ַЧ��
		bool bCheckSameIP=pUserRule->bCheckSameIP;
		for (WORD i=0;i<m_wChairCount;i++)
		{
			if (m_pIUserItem[i]!=NULL)
			{
				pTableUserRule=m_pIUserItem[i]->GetUserRule();
				if (pTableUserRule->bCheckSameIP==true) 
				{
					bCheckSameIP=true;
					break;
				}
			}
		}
		//��ַЧ��
		if(m_pGameServiceAttrib->bCheckSameIP &&
			pUserData->UserData.cbLevel == USERLEVEL_GAMEUSER)
		{
			bCheckSameIP=true;
		}
#ifdef ALLOW_SAME_IP_SIT
		bCheckSameIP=false;
#endif
		if (bCheckSameIP==true)
		{
			DWORD dwUserIP=pIServerUserItem->GetClientIP();
			for (WORD i=0;i<m_wChairCount;i++)
			{
				if ((m_pIUserItem[i]!=NULL)&&(m_pIUserItem[i]->GetClientIP()==dwUserIP))
				{
					if (pUserRule->bCheckSameIP)
					{
						SendSitFailedPacket(pIServerUserItem,ErrCode_CMD_GR_SitFailed_SameIP);
						return false;
					}
					else
					{
						SendSitFailedPacket(pIServerUserItem,ErrCode_CMD_GR_SitFailed_SameIP);
						return false;
					}
				}
			}
			for (WORD i=0;i<m_wChairCount-1;i++)
			{
				if (m_pIUserItem[i]!=NULL)
				{
					for (WORD j=i+1;j<m_wChairCount;j++)
					{
						if ((m_pIUserItem[j]!=NULL)&&(m_pIUserItem[i]->GetClientIP()==m_pIUserItem[j]->GetClientIP()))
						{
							SendSitFailedPacket(pIServerUserItem,ErrCode_CMD_GR_SitFailed_SameIP);
							return false;
						}
					}
				}
			}
		}
	}

	//���´���
	if (m_pITableFrameEvent!=NULL) m_pITableFrameEvent->OnEventUserReqSitDown(wChairID,pIServerUserItem,false);

	//��������
	WORD wUserCount=0;
	for (WORD i=0;i<m_wChairCount;i++)
	{
		if (m_pIUserItem[i]!=NULL) wUserCount++;
	}
	if (wUserCount==0)
	{
		m_bLocked=pUserRule->bPassword;
		m_dwTableOwnerID=pUserData->UserData.dwUserID;
		if (m_bLocked==true) lstrcpyn(m_szPassword,pUserRule->szPassword,CountArray(m_szPassword));
	}

	//�������
	m_pIUserItem[wChairID]=pIServerUserItem;
	pIServerUserItem->SetUserStatus(US_SIT,m_wTableID,wChairID);
	m_pIGameServiceFrame->SendUserStatus(pIServerUserItem);
	m_wUserCount = wUserCount + 1;
	if(pIServerUserItem->IsAIUser())
	{
		m_wAIGameUserCount ++;
	}
	//���´���
	if (m_pITableFrameEvent!=NULL) m_pITableFrameEvent->OnEventUserSitDown(wChairID,pIServerUserItem,false);
	
	return m_CalculateFrame.OnEventUserSitDown(wChairID,pIServerUserItem,false);
}

//�����¼�
bool __cdecl CTableFrame::OnUserOffLine(WORD wChairID)
{
	//Ч��״̬
	GT_ASSERT(wChairID<m_wChairCount);
	GT_ASSERT(m_pIUserItem[wChairID]!=NULL);

	//����״̬
	m_wOffLineCount[wChairID]++;
	m_pIUserItem[wChairID]->SetUserStatus(US_OFFLINE,m_wTableID,wChairID);
	m_pIGameServiceFrame->SendUserStatus(m_pIUserItem[wChairID]);

	//���ߴ���
	if (m_pITableFrameEvent!=NULL) m_pITableFrameEvent->OnEventUserOffLine(wChairID,m_pIUserItem[wChairID]);

	//���ö�ʱ��
	WORD wTimerID=IDI_OFFLINE+wChairID;
	WPARAM wBindParam=m_pIUserItem[wChairID]->GetUserID();
	m_pIGameServiceFrame->SetTableTimer(m_wTableID,wTimerID,90000L,1,wBindParam);

	return true;
}

//�ؽ��¼�
bool __cdecl CTableFrame::OnUserReConnect(WORD wChairID)
{
	//Ч��״̬
	GT_ASSERT(wChairID<m_wChairCount);
	GT_ASSERT(m_pIUserItem[wChairID]!=NULL);
	if(m_pIUserItem[wChairID] == NULL)
		return false;

	//���봦��
	if (m_pITableFrameEvent!=NULL) m_pITableFrameEvent->OnEventUserReqReConnect(wChairID,m_pIUserItem[wChairID]);

	//ɾ����ʱ��
	m_pIGameServiceFrame->KillTableTimer(m_wTableID,IDI_OFFLINE+wChairID);

	//�����û�
	m_pIUserItem[wChairID]->SetUserStatus(US_PLAY,m_wTableID,wChairID);
	m_pIGameServiceFrame->SendUserStatus(m_pIUserItem[wChairID]);

	//���봦��
	if (m_pITableFrameEvent!=NULL) m_pITableFrameEvent->OnEventUserReConnect(wChairID,m_pIUserItem[wChairID]);

	return true;
}

//�������
bool __cdecl CTableFrame::OnUserReqOffLine(WORD wChairID)
{
	//Ч�����
	GT_ASSERT(wChairID<m_wChairCount);
	GT_ASSERT(m_pIUserItem[wChairID]!=NULL);
	GT_ASSERT(m_pITableFrameSink);
	if (wChairID>=m_wChairCount) return false;
	if (m_pIUserItem[wChairID]==NULL) return false;

	//���ߴ���
	if (m_pITableFrameEvent!=NULL) return m_pITableFrameEvent->OnEventUserReqOffLine(wChairID,
										m_pIUserItem[wChairID],
										m_wOffLineCount[wChairID]);

	//��������s
	if (m_pGameServiceOption->wServerType&GAME_GENRE_MATCH) return m_wOffLineCount[wChairID]<5;

	return m_wOffLineCount[wChairID]<3;
}

//��ʱ���¼�
bool __cdecl CTableFrame::OnEventTimer(WORD wTimerID, WPARAM wBindParam)
{
	//���Ӷ�ʱ��
	if (wTimerID>IDI_MAX_TIME_ID)
	{
		switch (wTimerID)
		{
		case IDI_OFFLINE:
		case IDI_OFFLINE+1:
		case IDI_OFFLINE+2:
		case IDI_OFFLINE+3:
		case IDI_OFFLINE+4:
		case IDI_OFFLINE+5:
		case IDI_OFFLINE+6:
		case IDI_OFFLINE+7:		//���߶�ʱ��
			{
				//Ч�����
				WORD wChairID=wTimerID-IDI_OFFLINE;
				GT_ASSERT(wChairID<m_wChairCount);

				//Ч��״̬
				if (m_bStarted==false) return false;

				//���ߴ���
				if (m_pIUserItem[wChairID]!=NULL)
				{
					tagServerUserData * pUserData=m_pIUserItem[wChairID]->GetUserData();
					if (pUserData->UserData.dwUserID!=wBindParam) return false;
					if (pUserData->UserStatus.cbUserStatus!=US_OFFLINE) return false;
					m_pITableFrameSink->OnEventGameEnd(wChairID,m_pIUserItem[wChairID],GER_USER_LEFT);

					return true;
				}

				return false;
			}
		}

		return false;
	}

	//��Ϸ��ʱ��
	return m_pITableFrameSink->OnTimerMessage(wTimerID,wBindParam);
}

//��Ϸ�¼�����
bool __cdecl CTableFrame::OnEventSocketGame(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	//Ч�����
	GT_ASSERT(pIServerUserItem!=NULL);
	GT_ASSERT(m_pITableFrameSink!=NULL);

	//��Ϣ����
	return m_pITableFrameSink->OnGameMessage(wSubCmdID,pDataBuffer,wDataSize,pIServerUserItem);
}

//����¼�����
bool __cdecl CTableFrame::OnEventSocketFrame(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	//Ч�����
	GT_ASSERT(pIServerUserItem!=NULL);
	GT_ASSERT(m_pITableFrameSink!=NULL);

	//��Ϣ����
	bool bSuccess=m_pITableFrameSink->OnFrameMessage(wSubCmdID,pDataBuffer,wDataSize,pIServerUserItem);
	if (bSuccess==false)
	{
		switch (wSubCmdID)
		{
		case SUB_GF_INFO:	//��Ϸ��Ϣ
			{
				//Ч�����
				GT_ASSERT(wDataSize==sizeof(CMD_GF_Info));
				if (wDataSize<sizeof(CMD_GF_Info)) return false;

				//��������
				CMD_GF_Info * pInfo=(CMD_GF_Info *)pDataBuffer;
				tagServerUserData * pUserData=pIServerUserItem->GetUserData();
				bool bLookonUser=(pUserData->UserStatus.cbUserStatus==US_LOOKON);

				//Ч��״̬
				GT_ASSERT(pUserData->UserStatus.wChairID<m_wChairCount);
				if (pUserData->UserStatus.wChairID>=m_wChairCount) return false;

				//���ñ���
				if (bLookonUser==false) m_bAllowLookon[pUserData->UserStatus.wChairID]=pInfo->bAllowLookon?true:false;

				//��������
				CMD_GF_Option Option;
				Option.bGameStatus=m_bGameStatus;
				Option.bAllowLookon=m_bAllowLookon[pUserData->UserStatus.wChairID]?TRUE:FALSE;
				m_pIGameServiceFrame->SendData(pIServerUserItem,MDM_GF_FRAME,SUB_GF_OPTION,&Option,sizeof(Option));

				//���ͳ���
				bool bSendSecret=((bLookonUser==false)||(m_bAllowLookon[pUserData->UserStatus.wChairID]==true));
				return m_pITableFrameSink->SendGameScene(pUserData->UserStatus.wChairID,pIServerUserItem,m_bGameStatus,bSendSecret);
			}
		case SUB_GF_USER_READY:	//�û�ͬ��
			{
				//��������
				tagServerUserData * pUserData=pIServerUserItem->GetUserData();
				bool bLookonUser=(pUserData->UserStatus.cbUserStatus==US_LOOKON);

				//״̬Ч��
				GT_ASSERT(bLookonUser==false);
				GT_ASSERT(m_pIUserItem[pUserData->UserStatus.wChairID]==pIServerUserItem);
				if (bLookonUser==true) return false;
				if (pUserData->UserStatus.cbUserStatus>=US_PLAY) return true;

				//ͬ�⴦��
				if (m_pITableFrameEvent!=NULL) m_pITableFrameEvent->OnEventUserReqReady(pUserData->UserStatus.wChairID,m_pIUserItem[pUserData->UserStatus.wChairID]);

				//�¼�����
				if ((m_pITableFrameEvent!=NULL)&&(m_pITableFrameEvent->OnEventUserReqReady(
					pUserData->UserStatus.wChairID,pIServerUserItem)==false)) return true;

				//���ñ���
				pUserData->UserStatus.cbUserStatus=US_READY;

				//ͬ�⴦��
				if (m_pITableFrameEvent!=NULL) m_pITableFrameEvent->OnEventUserReady(pUserData->UserStatus.wChairID,m_pIUserItem[pUserData->UserStatus.wChairID]);

				//����״̬
				m_pIGameServiceFrame->SendUserStatus(pIServerUserItem);

				//��ʼ�ж�
				if ((StartVerdict()==true))
				{
					TryStartGame();
					return true;
				}


				return true;
			}

		}
	}

	return bSuccess;
}

//ö���û�
IServerUserItem * __cdecl CTableFrame::EnumLookonUserItem(WORD wIndex)
{
	if (wIndex>=m_LookonUserItemPtr.GetCount()) return NULL;
	return m_LookonUserItemPtr[wIndex];
}

//��ȡ�û�
IServerUserItem * __cdecl CTableFrame::GetServerUserItem(WORD wChairID)
{
	GT_ASSERT(wChairID<m_wChairCount);
	if (wChairID>=m_wChairCount) return NULL;
	return m_pIUserItem[wChairID];
}

//���ö�ʱ��
bool __cdecl CTableFrame::SetGameTimer(WORD wTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM wBindParam)
{
	//Ч�����
	GT_ASSERT(wTimerID<=IDI_MAX_TIME_ID);
	if (wTimerID>IDI_MAX_TIME_ID) return false;

	//���ö�ʱ��
	return m_pIGameServiceFrame->SetTableTimer(m_wTableID,wTimerID,dwElapse,dwRepeat,wBindParam);
}

//ɾ����ʱ��
bool __cdecl CTableFrame::KillGameTimer(WORD wTimerID)
{
	//Ч�����
	GT_ASSERT(wTimerID<=IDI_MAX_TIME_ID);
	if (wTimerID>IDI_MAX_TIME_ID) return false;

	//ɾ����ʱ��
	return m_pIGameServiceFrame->KillTableTimer(m_wTableID,wTimerID);
}

//��������
bool __cdecl CTableFrame::SendUserData(IServerUserItem * pIServerUserItem, WORD wSubCmdID)
{
	m_pIGameServiceFrame->SendData(pIServerUserItem,MDM_GF_GAME,wSubCmdID);
	return true;
}

//��������
bool __cdecl CTableFrame::SendUserData(IServerUserItem * pIServerUserItem, WORD wSubCmdID, void * pData, WORD wDataSize)
{
	m_pIGameServiceFrame->SendData(pIServerUserItem,MDM_GF_GAME,wSubCmdID,pData,wDataSize);
	return true;
}

//��������
bool __cdecl CTableFrame::SendTableData(WORD wChairID, WORD wSubCmdID)
{
	//Ⱥ���û�
	if (wChairID==INVALID_CHAIR)
	{
		for (WORD i=0;i<m_wChairCount;i++)
		{
			if (m_pIUserItem[i]!=NULL) 
			{
				m_pIGameServiceFrame->SendData(m_pIUserItem[i],MDM_GF_GAME,wSubCmdID);
			}
		}
		return true;
	}

	//��һ����
	GT_ASSERT(wChairID<m_wChairCount);
	if ((wChairID<m_wChairCount)&&(m_pIUserItem[wChairID]!=NULL))
	{
		m_pIGameServiceFrame->SendData(m_pIUserItem[wChairID],MDM_GF_GAME,wSubCmdID);
		return true;
	}

	return false;
}

//��������
bool __cdecl CTableFrame::SendTableData(WORD wChairID, WORD wSubCmdID, void * pData, WORD wDataSize)
{
	//Ⱥ���û�
	if (wChairID==INVALID_CHAIR)
	{
		for (WORD i=0;i<m_wChairCount;i++)
		{
			if (m_pIUserItem[i]!=NULL) 
			{
				m_pIGameServiceFrame->SendData(m_pIUserItem[i],MDM_GF_GAME,wSubCmdID,pData,wDataSize);
			}
		}
		return true;
	}

	//��һ����
	GT_ASSERT(wChairID<m_wChairCount);
	if ((wChairID<m_wChairCount)&&(m_pIUserItem[wChairID]!=NULL))
	{
		m_pIGameServiceFrame->SendData(m_pIUserItem[wChairID],MDM_GF_GAME,wSubCmdID,pData,wDataSize);
		return true;
	}

	return false;
}

//��������
bool __cdecl CTableFrame::SendLookonData(WORD wChairID, WORD wSubCmdID)
{
	tagServerUserData * pUserData=NULL;
	IServerUserItem * pIServerUserItem=NULL;
	for (INT_PTR i=0;i<m_LookonUserItemPtr.GetCount();i++)
	{
		pIServerUserItem=m_LookonUserItemPtr[i];
		GT_ASSERT(pIServerUserItem!=NULL);
		if (wChairID<m_wChairCount)
		{
			pUserData=pIServerUserItem->GetUserData();
			if (pUserData->UserStatus.wChairID==wChairID) m_pIGameServiceFrame->SendData(pIServerUserItem,MDM_GF_GAME,wSubCmdID);
		}
		else m_pIGameServiceFrame->SendData(pIServerUserItem,MDM_GF_GAME,wSubCmdID);
	}

	return true;
}

//��������
bool __cdecl CTableFrame::SendLookonData(WORD wChairID, WORD wSubCmdID, void * pData, WORD wDataSize)
{
	tagServerUserData * pUserData=NULL;
	IServerUserItem * pIServerUserItem=NULL;
	for (INT_PTR i=0;i<m_LookonUserItemPtr.GetCount();i++)
	{
		pIServerUserItem=m_LookonUserItemPtr[i];
		GT_ASSERT(pIServerUserItem!=NULL);
		if (wChairID<m_wChairCount)
		{
			pUserData=pIServerUserItem->GetUserData();
			if (pUserData->UserStatus.wChairID==wChairID) m_pIGameServiceFrame->SendData(pIServerUserItem,MDM_GF_GAME,wSubCmdID,pData,wDataSize);
		}
		else m_pIGameServiceFrame->SendData(pIServerUserItem,MDM_GF_GAME,wSubCmdID,pData,wDataSize);
	}

	return true;
}

//���ͷ�����Ϣ
bool __cdecl CTableFrame::SendRoomMessage(IServerUserItem * pIServerUserItem, WORD wErrCode,LPCTSTR lpszMessage, WORD wMessageType)
{
	//�������ݰ�
	CMD_GR_Message Message;
	Message.wErrorCode=wErrCode;
	Message.wMessageType=wMessageType;
	if(lpszMessage != NULL)
	{
		lstrcpyn(Message.szContent,lpszMessage,CountArray(Message.szContent));
		Message.wMessageLength=CountString(Message.szContent);
		ConvertACPToUtf8(Message.szContent,CountArray(Message.szContent),
			Message.szContent, CountArray(Message.szContent));
	}
	else
	{
		Message.wMessageLength = 0;
	}

	//��������
	WORD wSendSize=sizeof(Message)-sizeof(Message.szContent)+Message.wMessageLength*sizeof(TCHAR);
	m_pIGameServiceFrame->SendData(pIServerUserItem,MDM_GR_SYSTEM,SUB_GR_MESSAGE,&Message,wSendSize);

	return true;
}

//������Ϸ��Ϣ
bool __cdecl CTableFrame::SendGameMessage(IServerUserItem * pIServerUserItem, WORD wErrCode,LPCTSTR lpszMessage, WORD wMessageType)
{
	//�������ݰ�
	CMD_GF_Message Message;
	Message.wErrorCode=wErrCode;
	Message.wMessageType=wMessageType;
	if(lpszMessage != NULL)
	{
		lstrcpyn(Message.szContent,lpszMessage,CountArray(Message.szContent));
		Message.wMessageLength=CountString(Message.szContent);
		ConvertACPToUtf8(Message.szContent,CountArray(Message.szContent),
			Message.szContent, CountArray(Message.szContent));
	}
	else
	{
		Message.wMessageLength=0;
	}

	//��������
	WORD wSendSize=sizeof(Message)-sizeof(Message.szContent)+Message.wMessageLength*sizeof(TCHAR);
	m_pIGameServiceFrame->SendData(pIServerUserItem,MDM_GF_FRAME,SUB_GF_MESSAGE,&Message,wSendSize);

	return true;
}

//��ɢ��Ϸ
bool __cdecl CTableFrame::DismissGame()
{
	//״̬�ж�
	if (m_bStarted==false) return false;

	//��ɢ��Ϸ
	m_pITableFrameSink->OnEventGameEnd(INVALID_CHAIR,NULL,GER_DISMISS);

	return true;
}

//������Ϸ
bool __cdecl CTableFrame::ConcludeGame()
{
	//���ñ���
	m_bStarted=false;
	m_ullTimeStart=0L;
	m_bWriteScore=false;
	m_bGameStatus=GS_FREE;
	memset(m_dwPlayerID,0,sizeof(DWORD)*m_wChairCount);
	memset(m_wOffLineCount,0,sizeof(WORD) * m_wChairCount);
	m_ullGameRoundID = 0;


	//��������
	m_pITableFrameSink->RepositTableFrameSink();

	//�����û�
	tagServerUserData * pUserData=NULL;
	for (WORD i=0;i<m_wChairCount;i++)
	{
		if (m_pIUserItem[i]!=NULL)
		{
			pUserData=m_pIUserItem[i]->GetUserData();
			if (pUserData->UserStatus.cbUserStatus!=US_OFFLINE) 
			{
				pUserData->UserStatus.cbUserStatus=US_SIT;
			}
		}
	}

	//���ߴ���
	for (WORD i=0;i<m_wChairCount;i++)
	{
		if (m_pIUserItem[i]!=NULL)
		{
			//��ȡ�û�
			IServerUserItem * pIServerUserItem=m_pIUserItem[i];
			if (pIServerUserItem->GetUserStatus()!=US_OFFLINE) continue;
			
			//ɾ����ʱ��
			WORD wTimerID=IDI_OFFLINE+i;
			m_pIGameServiceFrame->KillTableTimer(m_wTableID,wTimerID);

			//�û��뿪
			PerformStandUpAction(pIServerUserItem);

			//�����û�����
			m_pIGameServiceFrame->DeleteUserItem(pIServerUserItem);
		}
	}

	for (WORD i=0;i<m_wChairCount;i++)
	{
		if (m_pIUserItem[i]!=NULL)
		{
			m_pIGameServiceFrame->SendUserStatus(m_pIUserItem[i]);
		}
	}


	//�����
/*	DWORD dwHighScore=m_pGameServiceOption->dwHighScore;
	DWORD dwLessScore=m_pGameServiceOption->dwLessScore;
	if ((dwLessScore!=0L)||(dwHighScore!=0))
	{
		for (WORD i=0;i<m_wChairCount;i++)
		{
			if (m_pIUserItem[i]!=NULL)
			{
				//��ȡ�û�
				IServerUserItem * pIServerUserItem=m_pIUserItem[i];
				tagServerUserData * pUserData=pIServerUserItem->GetUserData();

				//�ж϶��
				if ((dwLessScore!=0L)&&(pUserData->UserData.UserScore.fScore<dwLessScore))
				{
					SendRoomMessage(pIServerUserItem,ErrCode_Message_LessScore,NULL,SMT_EJECT|SMT_INFO|SMT_CLOSE_GAME|SMT_INTERMIT_LINE);
					PerformStandUpAction(pIServerUserItem);
				}
			}
		}
	}*/

	//�ر��ж�
	if ((m_pIGameServiceFrame->IsAllowEnterGame()==false)||(m_pIGameServiceFrame->IsShallClose()==true))
	{
		for (WORD i=0;i<m_wChairCount;i++)
		{
			if (m_pIUserItem[i]!=NULL)
			{
				//��ȡ�û�
				IServerUserItem * pIServerUserItem=m_pIUserItem[i];
				tagServerUserData * pUserData=pIServerUserItem->GetUserData();

				//�û��ж�
				if (pUserData->UserData.dwMasterRight==0L)
				{
					//�û�����
					SendRoomMessage(pIServerUserItem,ErrCode_Message_CloseRoom,NULL,SMT_EJECT|SMT_INFO|SMT_CLOSE_GAME|SMT_INTERMIT_LINE);
					PerformStandUpAction(pIServerUserItem);

					continue;
				}
			}
		}
	}

	//����״̬
	m_pIGameServiceFrame->SendTableStatus(m_wTableID);

	return true;
}

//���ͳ���
bool __cdecl CTableFrame::SendGameScene(IServerUserItem * pIServerUserItem, void * pData, WORD wDataSize)
{

	//���ͳ���
	m_pIGameServiceFrame->SendData(pIServerUserItem,MDM_GF_FRAME,SUB_GF_SCENE,pData,wDataSize);

	return true;
}
//��ȡ������
ICalculateFrame* __cdecl CTableFrame::GetICalculateFrame()
{
	return GET_OBJECT_INTERFACE(m_CalculateFrame,ICalculateFrame);
}
//����
bool __cdecl CTableFrame::WriteGameEnd(BYTE cbReason,
									   BYTE *pEndData,
									   WORD wEndDataSize,
									   void* pCalculateContext,
									   ICalculateSink* pCalculateSink)
{
	//Ч��״̬
	GT_ASSERT(m_bStarted==true);
	GT_ASSERT(m_bWriteScore==false);
	GT_ASSERT(m_ullGameRoundID != 0);
	if (m_bStarted==false) return false;
	if (m_bWriteScore==true) return false;
	if (m_ullGameRoundID==0) return false;

	//��Ϸʱ��
	ULONGLONG dwPlayTimeCount=(ULONGLONG)_time64(NULL)-m_ullTimeStart;

	//д����
	m_bWriteScore=true;

#ifndef ALLOW_AI_WRITE_GAMEEND
	//�ж�ȫ��������д������¼
	if(IsAllAIGameUser())
	{
		//��λ����
		m_ullGameRoundID = 0;
		m_CalculateFrame.Reset();
		return true;
	}
#endif
	//����
	if(m_CalculateFrame.CalculateCTP(pCalculateContext, pCalculateSink))
	{

		{
			//�������
			DBR_GR_WriteUserChart WriteUserChart;
			DBR_GR_WriteGameRound WriteGameRound;

			for (WORD i=0;i<m_CalculateFrame.GetCalculateItemCount();i++)
			{
				ICalculateItem* pCalculateItem = m_CalculateFrame.EnumCalculateItem(i);
				if (pCalculateItem!=NULL)
				{
					//д��Ͷע����
					//��������
					memset(&WriteUserChart,0,sizeof(WriteUserChart));
					//��������
					WriteUserChart.ullGameRoundID = m_ullGameRoundID;
					WriteUserChart.dwUserID = pCalculateItem->GetUserID();
					WriteUserChart.wChairID = pCalculateItem->GetChairID();
					//��ȡ��Ͷע
					pCalculateItem->GetTotalBetScore(&WriteUserChart.decTotalBetScore);
					//�ж���ͶעΪ0
					if(VarDecCmpR8(&WriteUserChart.decTotalBetScore, 0) == VARCMP_GT)
					{
						//�ж�ֻ����ͨ��Ա��ҲŸ������ö��
						if(pCalculateItem->GetUserLevel() == USERLEVEL_GAMEUSER)
						{
							//�жϻ�����
							if(CAIUtil::IsAIUser(pCalculateItem->GetUserData()->UserData.cbState))
							{
								WriteUserChart.cbUpdateUserScoreFlag = 0;
							}
							else
							{
								WriteUserChart.cbUpdateUserScoreFlag = 1;
							}
						}
						else
							WriteUserChart.cbUpdateUserScoreFlag = 0;
						
						//��ȡ���ɲ�
						pCalculateItem->GetTotalWinScore(&WriteUserChart.decTotalWinScore);
						//��ȡ�ܳ�ˮ
						pCalculateItem->GetTotalTaxScore(&WriteUserChart.decTotalTaxScore);

						//��ȡ����,����,�����ɲ�
						pCalculateItem->GetWinScoreOccupancy(&WriteUserChart.decWinScoreOccupancy_High,
							&WriteUserChart.decWinScoreOccupancy_Self,
							&WriteUserChart.decWinScoreOccupancy_Less);
						//��ȡ����,����,���߳�ˮ
						pCalculateItem->GetTaxScoreOccupancy(&WriteUserChart.decTaxScoreOccupancy_High,
							&WriteUserChart.decTaxScoreOccupancy_Self,
							&WriteUserChart.decTaxScoreOccupancy_Less);

						//��ȡϴ��
						pCalculateItem->GetValidBetScoreAndPaidScore(&WriteUserChart.decValidBetScore_Total,
							&WriteUserChart.decValidBetScore_High,
							&WriteUserChart.decValidBetScore_HighRollback,
							&WriteUserChart.decValidBetScore_Less,
							&WriteUserChart.decValidBetScore_LessRollback,
							&WriteUserChart.decPaidScore_High,
							&WriteUserChart.decPaidScore_Less);
						//��ȡ��ϸͶע��Ϣ
						pCalculateItem->GetDetailBetScore(WriteUserChart.cbDetailBetScore,
							WriteUserChart.wDetailBetScoreSize);
						GT_ASSERT(WriteUserChart.wDetailBetScoreSize > 0 && WriteUserChart.wDetailBetScoreSize < sizeof(WriteUserChart.cbDetailBetScore));

						//Ͷ�����ݿ�
						m_DataBaseEvent.PostDataBaseEvent(DBR_GR_WRITE_USERCHART,0,
							0,&WriteUserChart,sizeof(WriteUserChart));

					}

				}
			}
			
			//д����Ϸ��¼
			//��������
			memset(&WriteGameRound, 0, sizeof(WriteGameRound));
			WriteGameRound.ullGameRoundID = m_ullGameRoundID;
			WriteGameRound.ullStartTime = m_ullTimeStart;
			WriteGameRound.cbCalculatedFlag = TRUE;
			WriteGameRound.cbValidFlag	 = TRUE;
			WriteGameRound.wServerID	 = m_pGameServiceOption->wServerID;
			WriteGameRound.wTableID		 = m_wTableID;
			WriteGameRound.cbEndReason	 = cbReason;
			GT_ASSERT(wEndDataSize < sizeof(WriteGameRound.cbEndData));
			memcpy(WriteGameRound.cbEndData, pEndData, wEndDataSize);
			WriteGameRound.wEndDataSize = wEndDataSize;

			//Ͷ�����ݿ�
			m_DataBaseEvent.PostDataBaseEvent(DBR_GR_WRITE_GAMEROUND,0,
				0,&WriteGameRound,sizeof(WriteGameRound));

		}


		//��λ����
		m_ullGameRoundID = 0;
		m_CalculateFrame.Reset();

		return true;

	}
	else
	{
		GT_ASSERT(FALSE);
		return false;
	}
}
//������Ϸ�ּ�¼-������ö���¼�
void CTableFrame::OnEventAllocGameRoundAndCheckBetScore(DBR_GR_AllocGameRoundAndCheckBetScoreResult * 
															pAllocGameRoundAndCheckBetScoreResult)
{
	if(pAllocGameRoundAndCheckBetScoreResult->ullGameRoundID != 0)
	{
		//Ч��״̬
		//GT_ASSERT(m_bStarted==false);
		if (m_bStarted==true) return;

		//��ʼ�ж�
		if ((StartVerdict()==true))
		{
			m_ullGameRoundID = pAllocGameRoundAndCheckBetScoreResult->ullGameRoundID;
			StartGame();
		}

	}
	else
	{
		for(int i = 0; i < CountArray(pAllocGameRoundAndCheckBetScoreResult->decUserScore); i ++)
		{
			if(m_pIUserItem[i] != NULL && i < m_wChairCount)
			{
				if(pAllocGameRoundAndCheckBetScoreResult->dwUserID[i] ==
					m_pIUserItem[i]->GetUserData()->UserData.dwUserID)
				{
					VarR8FromDec(&pAllocGameRoundAndCheckBetScoreResult->decUserScore[i],
						&m_pIUserItem[i]->GetUserScore()->fScore);
					if(pAllocGameRoundAndCheckBetScoreResult->bCheckScoreFlag[i])
					{
						SendRoomMessage(m_pIUserItem[i],
							ErrCode_Message_KickUser,
							NULL,
							SMT_INFO|SMT_EJECT|SMT_INTERMIT_LINE);
					}
				}
			}
		}
	}
}
//������Ϸ�ּ�¼�¼�
void CTableFrame::OnEventAllocGameRoundResult(DBR_GR_AllocGameRoundResult * pAllocGameRoundResult)
{
	if(pAllocGameRoundResult->ullGameRoundID != 0)
	{
		//Ч��״̬
		GT_ASSERT(m_bStarted==false);
		if (m_bStarted==true) return;

		//��ʼ�ж�
		if ((StartVerdict()==true))
		{
			m_ullGameRoundID = pAllocGameRoundResult->ullGameRoundID;
			StartGame();
		}

	}
}
//���Կ�ʼ��Ϸ
void CTableFrame::TryStartGame()
{
	//Ч��״̬
	GT_ASSERT(m_bStarted==false);
	if (m_bStarted==true) return;

#ifndef ALLOW_AI_WRITE_GAMEEND
	//�ж�ȫ��������
	if(IsAllAIGameUser())
	{
		m_ullGameRoundID = rand() + 1;
		StartGame();
		return;
	}
#endif

	//������ö��
	if(m_pGameServiceAttrib->bCheckCellScoreBeforeStartGame)
	{
		//������Ϸ��¼
		memset(m_bCheckBetScoreFlag,0,sizeof(bool) * m_wChairCount);
		DBR_GR_AllocGameRoundAndCheckBetScore AllocGameRoundAndCheckBetScore;
		memset(&AllocGameRoundAndCheckBetScore, 0, sizeof(AllocGameRoundAndCheckBetScore));
		AllocGameRoundAndCheckBetScore.ullStartTime = _time64(NULL);
		AllocGameRoundAndCheckBetScore.wServerID = m_pGameServiceOption->wServerID;
		AllocGameRoundAndCheckBetScore.wTableID  = m_wTableID;

		GT_ASSERT(m_wChairCount < CountArray(AllocGameRoundAndCheckBetScore.decBetScore));
		for(int i = 0; i < CountArray(AllocGameRoundAndCheckBetScore.decBetScore); i ++)
		{
			if(m_pIUserItem[i] != NULL && i < m_wChairCount)
			{
				AllocGameRoundAndCheckBetScore.dwUserID[i] = m_pIUserItem[i]->GetUserData()->UserData.dwUserID;
				VarDecFromUI4(m_pGameServiceOption->dwCellScore, &AllocGameRoundAndCheckBetScore.decBetScore[i]);
			}
		}
		
		//Ͷ�����ݿ�
		m_DataBaseEvent.PostDataBaseEvent(DBR_GR_ALLOC_GAMEROUNDANDCHECKBETSCORE,0,
				0,&AllocGameRoundAndCheckBetScore,sizeof(AllocGameRoundAndCheckBetScore));
	}
	else
	{
		//������Ϸ��¼
		if(m_ullGameRoundID == 0)
		{
			DBR_GR_AllocGameRound AllocGameRound;
			memset(&AllocGameRound, 0, sizeof(AllocGameRound));
			AllocGameRound.ullStartTime = _time64(NULL);
			AllocGameRound.wServerID = m_pGameServiceOption->wServerID;
			AllocGameRound.wTableID  = m_wTableID;

			//Ͷ�����ݿ�
			m_DataBaseEvent.PostDataBaseEvent(DBR_GR_ALLOC_GAMEROUND,0,
				0,&AllocGameRound,sizeof(AllocGameRound));

		}
		else
			StartGame();
	}
}

//��ʼ��Ϸ
void CTableFrame::StartGame()
{
	//Ч��״̬
	GT_ASSERT(m_bStarted==false);
	if (m_bStarted==true) return;

	GT_ASSERT(m_ullGameRoundID != 0);

	//���ñ���
	m_bStarted=true;
	m_ullTimeStart=(ULONGLONG)_time64(NULL);
	memset(m_wOffLineCount,0,sizeof(WORD) * m_wChairCount);

	//��¼���
	m_bWriteScore=false;
	memset(m_dwPlayerID,0,sizeof(DWORD) * m_wChairCount);

	//��λ����
	m_CalculateFrame.Reset();
	m_CalculateFrame.StructCalculateItem();

	//�������
	tagServerUserData * pUserData=NULL;
	for (WORD i=0;i<m_wChairCount;i++)
	{
		if (m_pIUserItem[i]!=NULL)
		{
			pUserData=m_pIUserItem[i]->GetUserData();
			pUserData->UserStatus.cbUserStatus=US_PLAY;
			m_dwPlayerID[i]=pUserData->UserData.dwUserID;
			m_pIGameServiceFrame->SendUserStatus(m_pIUserItem[i]);
		}
	}

	//����״̬
	m_pIGameServiceFrame->SendTableStatus(m_wTableID);

	//֪ͨ�¼�
	m_pITableFrameSink->OnEventGameStart();

	return;
}

//�жϿ�ʼ
bool CTableFrame::StartVerdict()
{
	//�����ж�
	if (m_bStarted==true) return false;

	//ʱ��ģʽ
	enStartMode StartMode=m_pITableFrameSink->GetGameStartMode();
	if (StartMode==enStartMode_TimeControl) return false;

	//׼������
	WORD wReadyUserCount=0;
	tagServerUserData * pUserData=NULL;
	for (WORD i=0;i<m_wChairCount;i++) 
	{
		if (m_pIUserItem[i]!=NULL)
		{
			wReadyUserCount++;
			pUserData=m_pIUserItem[i]->GetUserData();
			if (pUserData->UserStatus.cbUserStatus!=US_READY) return false;
		}
	}

	//�����ж�
	if (wReadyUserCount>1L)
	{
		if (StartMode==enStartMode_Symmetry)
		{
			if ((wReadyUserCount%2)!=0) return false;
			if (wReadyUserCount==m_wChairCount) return true;
			WORD wHalfCount=m_wChairCount/2;
			for (WORD i=0;i<wHalfCount;i++)
			{
				if ((m_pIUserItem[i]==NULL)&&(m_pIUserItem[i+wHalfCount]!=NULL)) return false;
				if ((m_pIUserItem[i]!=NULL)&&(m_pIUserItem[i+wHalfCount]==NULL)) return false;
			}
			return true;
		}
		else 
		{
			if (StartMode==enStartMode_FullReady) return (wReadyUserCount==m_wChairCount);
			if (StartMode==enStartMode_AllReady) return true;
		}
	}

	return false;
}
//ȫ���������ж�
bool CTableFrame::IsAllAIGameUser()
{
	if(GetGameUserCount() == 0)
		return true;
	else
		return false;
}
//��������ʧ��
void CTableFrame::SendSitFailedPacket(IServerUserItem * pIServerUserItem, WORD wFailedCode)
{
	//��������
	CMD_GR_SitFailed SitFailed;
	SitFailed.wErrorCode = wFailedCode;

	//��������
	WORD wSendSize=sizeof(SitFailed);
	m_pIGameServiceFrame->SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_SIT_FAILED,&SitFailed,wSendSize);

	return;
}

//////////////////////////////////////////////////////////////////////////
