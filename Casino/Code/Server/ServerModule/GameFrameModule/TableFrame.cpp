#include "StdAfx.h"
#include "TableFrame.h"
#include "..\..\CommonDefinition\CHINESECODELIB.H"
//////////////////////////////////////////////////////////////////////////

//定时器 ID
#define IDI_OFFLINE					(IDI_MAX_TIME_ID+1)					//断线定时器


//////////////////////////////////////////////////////////////////////////

//构造函数
CTableFrame::CTableFrame()
{
	//属性变量
	m_wChairCount=0;
	m_wTableID=INVALID_TABLE;

	//状态变量
	m_bLocked=false;
	m_bStarted=false;
	m_ullTimeStart=0L;
	m_dwTableOwnerID=0L;
	m_bGameStatus=GS_FREE;
	m_szPassword[0]=0;
	m_wUserCount=0;
	m_wAIGameUserCount=0;

	//额度信息
	m_bWriteScore=false;

	//配置变量
	m_pGameServiceAttrib=NULL;
	m_pGameServiceOption=NULL;

	//接口变量
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

//析构函数
CTableFrame::~CTableFrame()
{
	//释放对象
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

//接口查询
void * __cdecl CTableFrame::QueryInterface(const IID & Guid, DWORD dwQueryVer)
{
	QUERYINTERFACE(ITableFrame,Guid,dwQueryVer);
	QUERYINTERFACE(ITableFrameManager,Guid,dwQueryVer);
	QUERYINTERFACE_IUNKNOWNEX(ITableFrameManager,Guid,dwQueryVer);
	return NULL;
}

//初始化
bool __cdecl CTableFrame::InitTableFrame(WORD wTableID, tagTableFrameParameter * pTableFrameParameter)
{
	//效验参数
	GT_ASSERT(pTableFrameParameter!=NULL);

	//设置变量
	m_wTableID=wTableID;
	m_pGameServiceAttrib=pTableFrameParameter->pGameServiceAttrib;
	m_pGameServiceOption=pTableFrameParameter->pGameServiceOption;
	m_wChairCount=pTableFrameParameter->pGameServiceAttrib->wChairCount;

	//查询接口
	m_pIGameServiceFrame=GET_OBJECTPTR_INTERFACE(pTableFrameParameter->pIGameServiceFrame,IGameServiceFrame);
	GT_ASSERT(m_pIGameServiceFrame!=NULL);
	if (m_pIGameServiceFrame==NULL) return false;

	//创建桌子
	IGameServiceManager * pIGameServiceManager=GET_OBJECTPTR_INTERFACE(pTableFrameParameter->pIGameServiceManager,IGameServiceManager);
	m_pITableFrameSink=(ITableFrameSink *)pIGameServiceManager->CreateTableFrameSink(IID_ITableFrameSink,VER_ITableFrameSink);
	GT_ASSERT(m_pITableFrameSink!=NULL);
	if (m_pITableFrameSink==NULL) return false;

	//设置接口
	IUnknownEx * pIUnknownEx=GET_MYSELF_INTERFACE(IUnknownEx);
	if (m_pITableFrameSink->InitTableFrameSink(pIUnknownEx)==false) return false;

	//数据库引擎
	IDataBaseEngine* pIDataBaseEngine=GET_OBJECTPTR_INTERFACE(pTableFrameParameter->pIDataBaseEngine,IDataBaseEngine);
	if (pIDataBaseEngine==NULL)
	{
		TRACE(TEXT("获取数据库引擎服务失败"));
		return false;
	}
	//数据库触发
	IUnknownEx * pIQueueService=(IUnknownEx *)pIDataBaseEngine->GetQueueService(IID_IUnknownEx,VER_IUnknownEx);
	if (m_DataBaseEvent.SetQueueService(pIQueueService)==false)
	{
		return false;
	}

	//扩展接口
	m_pITableFrameEvent=GET_OBJECTPTR_INTERFACE(m_pITableFrameSink,ITableFrameEvent);

	m_CalculateFrame.InitCalculateFrame(wTableID, this, pTableFrameParameter);

	//申请内存
	//用户信息
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

//游戏状态
bool __cdecl CTableFrame::IsUserPlaying(IServerUserItem * pIServerUserItem)
{
	//游戏状态
	if (m_bStarted==false) return false;

	//用户状态
	BYTE cbUserStatus=pIServerUserItem->GetUserStatus();
	if ((cbUserStatus!=US_PLAY)&&(cbUserStatus!=US_OFFLINE)) return false;

	//逻辑判断
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
//获取AI对杀的普通玩家座号码
WORD __cdecl CTableFrame::GetAIKillGameUserChairID()
{
	for (WORD i=0;i<m_wChairCount;i++)
	{
		if (m_pIUserItem[i]!=NULL && m_pIUserItem[i]->IsAIUser() == FALSE)
			return i;
	}
	return INVALID_CHAIR;
}
//是否AI对杀普通玩家
bool __cdecl CTableFrame::IsAIKillGameUser()
{
	//判断全部AI机器人
	if(GetAIGameUserCount() && GetGameUserCount() == 0)
		return false;
	//判断全部普通玩家
	if(GetGameUserCount() && GetAIGameUserCount() == 0)
		return false;
	//只有一位普通玩家
	return ( GetGameUserCount() == 1 && GetAIGameUserCount() ) ? true : false;
}

//离开动作
bool __cdecl CTableFrame::PerformStandUpAction(IServerUserItem * pIServerUserItem)
{
	//效验参数
	GT_ASSERT(pIServerUserItem!=NULL);
	GT_ASSERT(pIServerUserItem->GetTableID()==m_wTableID);
	GT_ASSERT(pIServerUserItem->GetChairID()<m_wChairCount);

	//变量定义
	tagServerUserData * pUserData=pIServerUserItem->GetUserData();
	WORD wChairID=pUserData->UserStatus.wChairID;
	bool bLookonUser=(pIServerUserItem->GetUserStatus()==US_LOOKON);

	//用户处理
	if (bLookonUser==true)
	{
		IServerUserItem * pILookonUserItem=NULL;
		for (INT_PTR i=0;i<m_LookonUserItemPtr.GetCount();i++)
		{
			pILookonUserItem=m_LookonUserItemPtr[i];
			GT_ASSERT(pILookonUserItem!=NULL);
			if (pILookonUserItem==pIServerUserItem)
			{
				//起立处理
				if (m_pITableFrameEvent!=NULL) m_pITableFrameEvent->OnEventUserReqStandUp(wChairID,pIServerUserItem,false);

				//设置用户
				m_LookonUserItemPtr.RemoveAt(i);
				pIServerUserItem->SetUserStatus(US_FREE,INVALID_TABLE,INVALID_CHAIR);
				m_pIGameServiceFrame->SendUserStatus(pIServerUserItem);

				//起立处理
				if (m_pITableFrameEvent!=NULL) m_pITableFrameEvent->OnEventUserStandUp(wChairID,pIServerUserItem,true);

				return m_CalculateFrame.OnEventUserStandUp(wChairID,pIServerUserItem,true);
			}
		}
	}
	else
	{
		//起立请求
		if (m_pITableFrameEvent!=NULL) m_pITableFrameEvent->OnEventUserReqStandUp(wChairID,pIServerUserItem,false);

		//结束游戏
		if (IsUserPlaying(pIServerUserItem)==true)
		{
			m_pITableFrameSink->OnEventGameEnd(pUserData->UserStatus.wChairID,pIServerUserItem,GER_USER_LEFT);
		}

		//离开判断
		if ((pUserData->UserStatus.wTableID==INVALID_TABLE)||(pUserData->UserStatus.wChairID==INVALID_CHAIR)) return true;

		//设置用户
		m_pIUserItem[pUserData->UserStatus.wChairID]=NULL;
		pIServerUserItem->SetUserStatus(US_FREE,INVALID_TABLE,INVALID_CHAIR);
		m_pIGameServiceFrame->SendUserStatus(pIServerUserItem);

		//设置桌主
		if (m_dwTableOwnerID==pUserData->UserData.dwUserID) m_dwTableOwnerID=0L;

		//更新信息
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

		//坐下处理
		if (m_pITableFrameEvent!=NULL) m_pITableFrameEvent->OnEventUserStandUp(wChairID,pIServerUserItem,false);
		m_CalculateFrame.OnEventUserStandUp(wChairID,pIServerUserItem,false);
		
		//开始判断
		if ((StartVerdict()==true))
		{
			TryStartGame();
			return true;
		}

		return true;
	}

	return true;
}
//人工智能游戏动作
bool __cdecl CTableFrame::PerformAIGameAction()
{
	//效验参数
	if(m_pITableFrameSink) return m_pITableFrameSink->OnPerformAIGameAction();
	else return false;
}
//旁观动作
bool __cdecl CTableFrame::PerformLookonAction(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	//效验参数
	GT_ASSERT(pIServerUserItem!=NULL);
	GT_ASSERT(wChairID<m_wChairCount);
	GT_ASSERT(pIServerUserItem->GetTableID()==INVALID_TABLE);
	GT_ASSERT(pIServerUserItem->GetChairID()==INVALID_CHAIR);

	//变量定义
	DWORD dwMasterRight=pIServerUserItem->GetUserData()->UserData.dwMasterRight;

	//关闭查询
	if ((dwMasterRight==0L)&&(m_pIGameServiceFrame->IsShallClose()==true))
	{
		SendSitFailedPacket(pIServerUserItem,ErrCode_CMD_GR_SitFailed_RoomShallClose);
		return false;
	}

	//禁止查询
	if ((m_pIGameServiceFrame->IsAllowEnterGame()==false))
	{
		SendSitFailedPacket(pIServerUserItem,ErrCode_CMD_GR_SitFailed_AllowEnterGame);
		return false;
	}

	//位置判断
	if (m_pIUserItem[wChairID]==NULL)
	{
		SendSitFailedPacket(pIServerUserItem,ErrCode_CMD_GR_SitFailed_NoLookonUser);
		return false;
	}

	//密码效验
	if (m_bLocked==true)
	{
		const tagUserRule * pUserRule=pIServerUserItem->GetUserRule();
		if ((pUserRule->bPassword==false)||(lstrcmp(m_szPassword,pUserRule->szPassword)!=0))
		{
			SendSitFailedPacket(pIServerUserItem,ErrCode_CMD_GR_SitFailed_Password);
			return false;
		}
	}

	//状态判断
	if ((m_bStarted==false)&&(dwMasterRight==0L))
	{
		SendSitFailedPacket(pIServerUserItem,ErrCode_CMD_GR_SitFailed_PlayingGame);
		return false;
	}

	//旁观处理
	if (m_pITableFrameEvent!=NULL) m_pITableFrameEvent->OnEventUserReqSitDown(wChairID,pIServerUserItem,true);

	//设置玩家
	m_LookonUserItemPtr.Add(pIServerUserItem);
	pIServerUserItem->SetUserStatus(US_LOOKON,m_wTableID,wChairID);
	m_pIGameServiceFrame->SendUserStatus(pIServerUserItem);

	//旁观处理
	if (m_pITableFrameEvent!=NULL) m_pITableFrameEvent->OnEventUserSitDown(wChairID,pIServerUserItem,true);
	
	return m_CalculateFrame.OnEventUserSitDown(wChairID,pIServerUserItem,true);
}

//坐下动作
bool __cdecl CTableFrame::PerformSitDownAction(WORD wChairID, IServerUserItem * pIServerUserItem)
{
	//效验参数
	GT_ASSERT(pIServerUserItem!=NULL);
	GT_ASSERT(wChairID<m_wChairCount);
	GT_ASSERT(pIServerUserItem->GetTableID()==INVALID_TABLE);
	GT_ASSERT(pIServerUserItem->GetChairID()==INVALID_CHAIR);

	//变量定义
	const tagUserScore * pUserScore=pIServerUserItem->GetUserScore();;
	const tagUserRule * pUserRule=pIServerUserItem->GetUserRule(),* pTableUserRule=NULL;
	tagServerUserData * pUserData=pIServerUserItem->GetUserData(),* pTableUserData=NULL;

	//关闭查询
	if ((m_pIGameServiceFrame->IsShallClose()==true))
	{
		SendSitFailedPacket(pIServerUserItem,ErrCode_CMD_GR_SitFailed_RoomShallClose);
		return false;
	}

	//禁止查询
	if ((m_pIGameServiceFrame->IsAllowEnterGame()==false))
	{
		SendSitFailedPacket(pIServerUserItem,ErrCode_CMD_GR_SitFailed_AllowEnterGame);
		return false;
	}

	//位置判断
	if (m_pIUserItem[wChairID]!=NULL)
	{
		pTableUserData=m_pIUserItem[wChairID]->GetUserData();
		SendSitFailedPacket(pIServerUserItem,ErrCode_CMD_GR_SitFailed_OtherSitFirstly);
		return false;
	}

	//游戏状态
	if (m_bStarted==true &&
		IsBattleGameProcessType(m_pGameServiceAttrib->wProcessType))
	{
		SendSitFailedPacket(pIServerUserItem,ErrCode_CMD_GR_SitFailed_PlayingGame);
		return false;
	}


	{
		//权限判断
		if (CUserRight::CanPlay(m_pGameServiceAttrib->wProcessType,pUserData->dwGameRight)==false)
		{
			//发送消息
			SendSitFailedPacket(pIServerUserItem,ErrCode_LimitRight);

			return true;
		}

		//额度限制
		if (m_pGameServiceOption->dwLessScore!=0.0)
		{
			if (pUserData->UserData.UserScore.fScore<m_pGameServiceOption->dwLessScore)
			{
				SendSitFailedPacket(pIServerUserItem,ErrCode_CMD_GR_SitFailed_NoEnoughScore);
				return false;
			}
		}
		

		//密码效验
		if (m_bLocked==true)
		{
			if ((pUserRule->bPassword==false)||(lstrcmp(m_szPassword,pUserRule->szPassword)!=0))
			{
				SendSitFailedPacket(pIServerUserItem,ErrCode_CMD_GR_SitFailed_Password);
				return false;
			}
		}


		//地址效验
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
		//地址效验
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

	//坐下处理
	if (m_pITableFrameEvent!=NULL) m_pITableFrameEvent->OnEventUserReqSitDown(wChairID,pIServerUserItem,false);

	//更新密码
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

	//设置玩家
	m_pIUserItem[wChairID]=pIServerUserItem;
	pIServerUserItem->SetUserStatus(US_SIT,m_wTableID,wChairID);
	m_pIGameServiceFrame->SendUserStatus(pIServerUserItem);
	m_wUserCount = wUserCount + 1;
	if(pIServerUserItem->IsAIUser())
	{
		m_wAIGameUserCount ++;
	}
	//坐下处理
	if (m_pITableFrameEvent!=NULL) m_pITableFrameEvent->OnEventUserSitDown(wChairID,pIServerUserItem,false);
	
	return m_CalculateFrame.OnEventUserSitDown(wChairID,pIServerUserItem,false);
}

//断线事件
bool __cdecl CTableFrame::OnUserOffLine(WORD wChairID)
{
	//效验状态
	GT_ASSERT(wChairID<m_wChairCount);
	GT_ASSERT(m_pIUserItem[wChairID]!=NULL);

	//设置状态
	m_wOffLineCount[wChairID]++;
	m_pIUserItem[wChairID]->SetUserStatus(US_OFFLINE,m_wTableID,wChairID);
	m_pIGameServiceFrame->SendUserStatus(m_pIUserItem[wChairID]);

	//断线处理
	if (m_pITableFrameEvent!=NULL) m_pITableFrameEvent->OnEventUserOffLine(wChairID,m_pIUserItem[wChairID]);

	//设置定时器
	WORD wTimerID=IDI_OFFLINE+wChairID;
	WPARAM wBindParam=m_pIUserItem[wChairID]->GetUserID();
	m_pIGameServiceFrame->SetTableTimer(m_wTableID,wTimerID,90000L,1,wBindParam);

	return true;
}

//重进事件
bool __cdecl CTableFrame::OnUserReConnect(WORD wChairID)
{
	//效验状态
	GT_ASSERT(wChairID<m_wChairCount);
	GT_ASSERT(m_pIUserItem[wChairID]!=NULL);
	if(m_pIUserItem[wChairID] == NULL)
		return false;

	//重入处理
	if (m_pITableFrameEvent!=NULL) m_pITableFrameEvent->OnEventUserReqReConnect(wChairID,m_pIUserItem[wChairID]);

	//删除定时器
	m_pIGameServiceFrame->KillTableTimer(m_wTableID,IDI_OFFLINE+wChairID);

	//设置用户
	m_pIUserItem[wChairID]->SetUserStatus(US_PLAY,m_wTableID,wChairID);
	m_pIGameServiceFrame->SendUserStatus(m_pIUserItem[wChairID]);

	//重入处理
	if (m_pITableFrameEvent!=NULL) m_pITableFrameEvent->OnEventUserReConnect(wChairID,m_pIUserItem[wChairID]);

	return true;
}

//请求断线
bool __cdecl CTableFrame::OnUserReqOffLine(WORD wChairID)
{
	//效验参数
	GT_ASSERT(wChairID<m_wChairCount);
	GT_ASSERT(m_pIUserItem[wChairID]!=NULL);
	GT_ASSERT(m_pITableFrameSink);
	if (wChairID>=m_wChairCount) return false;
	if (m_pIUserItem[wChairID]==NULL) return false;

	//断线处理
	if (m_pITableFrameEvent!=NULL) return m_pITableFrameEvent->OnEventUserReqOffLine(wChairID,
										m_pIUserItem[wChairID],
										m_wOffLineCount[wChairID]);

	//比赛过虑s
	if (m_pGameServiceOption->wServerType&GAME_GENRE_MATCH) return m_wOffLineCount[wChairID]<5;

	return m_wOffLineCount[wChairID]<3;
}

//定时器事件
bool __cdecl CTableFrame::OnEventTimer(WORD wTimerID, WPARAM wBindParam)
{
	//桌子定时器
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
		case IDI_OFFLINE+7:		//断线定时器
			{
				//效验参数
				WORD wChairID=wTimerID-IDI_OFFLINE;
				GT_ASSERT(wChairID<m_wChairCount);

				//效验状态
				if (m_bStarted==false) return false;

				//断线处理
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

	//游戏定时器
	return m_pITableFrameSink->OnTimerMessage(wTimerID,wBindParam);
}

//游戏事件处理
bool __cdecl CTableFrame::OnEventSocketGame(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	//效验参数
	GT_ASSERT(pIServerUserItem!=NULL);
	GT_ASSERT(m_pITableFrameSink!=NULL);

	//消息处理
	return m_pITableFrameSink->OnGameMessage(wSubCmdID,pDataBuffer,wDataSize,pIServerUserItem);
}

//框架事件处理
bool __cdecl CTableFrame::OnEventSocketFrame(WORD wSubCmdID, const void * pDataBuffer, WORD wDataSize, IServerUserItem * pIServerUserItem)
{
	//效验参数
	GT_ASSERT(pIServerUserItem!=NULL);
	GT_ASSERT(m_pITableFrameSink!=NULL);

	//消息处理
	bool bSuccess=m_pITableFrameSink->OnFrameMessage(wSubCmdID,pDataBuffer,wDataSize,pIServerUserItem);
	if (bSuccess==false)
	{
		switch (wSubCmdID)
		{
		case SUB_GF_INFO:	//游戏信息
			{
				//效验参数
				GT_ASSERT(wDataSize==sizeof(CMD_GF_Info));
				if (wDataSize<sizeof(CMD_GF_Info)) return false;

				//变量定义
				CMD_GF_Info * pInfo=(CMD_GF_Info *)pDataBuffer;
				tagServerUserData * pUserData=pIServerUserItem->GetUserData();
				bool bLookonUser=(pUserData->UserStatus.cbUserStatus==US_LOOKON);

				//效验状态
				GT_ASSERT(pUserData->UserStatus.wChairID<m_wChairCount);
				if (pUserData->UserStatus.wChairID>=m_wChairCount) return false;

				//设置变量
				if (bLookonUser==false) m_bAllowLookon[pUserData->UserStatus.wChairID]=pInfo->bAllowLookon?true:false;

				//发送配置
				CMD_GF_Option Option;
				Option.bGameStatus=m_bGameStatus;
				Option.bAllowLookon=m_bAllowLookon[pUserData->UserStatus.wChairID]?TRUE:FALSE;
				m_pIGameServiceFrame->SendData(pIServerUserItem,MDM_GF_FRAME,SUB_GF_OPTION,&Option,sizeof(Option));

				//发送场景
				bool bSendSecret=((bLookonUser==false)||(m_bAllowLookon[pUserData->UserStatus.wChairID]==true));
				return m_pITableFrameSink->SendGameScene(pUserData->UserStatus.wChairID,pIServerUserItem,m_bGameStatus,bSendSecret);
			}
		case SUB_GF_USER_READY:	//用户同意
			{
				//变量定义
				tagServerUserData * pUserData=pIServerUserItem->GetUserData();
				bool bLookonUser=(pUserData->UserStatus.cbUserStatus==US_LOOKON);

				//状态效验
				GT_ASSERT(bLookonUser==false);
				GT_ASSERT(m_pIUserItem[pUserData->UserStatus.wChairID]==pIServerUserItem);
				if (bLookonUser==true) return false;
				if (pUserData->UserStatus.cbUserStatus>=US_PLAY) return true;

				//同意处理
				if (m_pITableFrameEvent!=NULL) m_pITableFrameEvent->OnEventUserReqReady(pUserData->UserStatus.wChairID,m_pIUserItem[pUserData->UserStatus.wChairID]);

				//事件处理
				if ((m_pITableFrameEvent!=NULL)&&(m_pITableFrameEvent->OnEventUserReqReady(
					pUserData->UserStatus.wChairID,pIServerUserItem)==false)) return true;

				//设置变量
				pUserData->UserStatus.cbUserStatus=US_READY;

				//同意处理
				if (m_pITableFrameEvent!=NULL) m_pITableFrameEvent->OnEventUserReady(pUserData->UserStatus.wChairID,m_pIUserItem[pUserData->UserStatus.wChairID]);

				//发送状态
				m_pIGameServiceFrame->SendUserStatus(pIServerUserItem);

				//开始判断
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

//枚举用户
IServerUserItem * __cdecl CTableFrame::EnumLookonUserItem(WORD wIndex)
{
	if (wIndex>=m_LookonUserItemPtr.GetCount()) return NULL;
	return m_LookonUserItemPtr[wIndex];
}

//获取用户
IServerUserItem * __cdecl CTableFrame::GetServerUserItem(WORD wChairID)
{
	GT_ASSERT(wChairID<m_wChairCount);
	if (wChairID>=m_wChairCount) return NULL;
	return m_pIUserItem[wChairID];
}

//设置定时器
bool __cdecl CTableFrame::SetGameTimer(WORD wTimerID, DWORD dwElapse, DWORD dwRepeat, WPARAM wBindParam)
{
	//效验参数
	GT_ASSERT(wTimerID<=IDI_MAX_TIME_ID);
	if (wTimerID>IDI_MAX_TIME_ID) return false;

	//设置定时器
	return m_pIGameServiceFrame->SetTableTimer(m_wTableID,wTimerID,dwElapse,dwRepeat,wBindParam);
}

//删除定时器
bool __cdecl CTableFrame::KillGameTimer(WORD wTimerID)
{
	//效验参数
	GT_ASSERT(wTimerID<=IDI_MAX_TIME_ID);
	if (wTimerID>IDI_MAX_TIME_ID) return false;

	//删除定时器
	return m_pIGameServiceFrame->KillTableTimer(m_wTableID,wTimerID);
}

//发送数据
bool __cdecl CTableFrame::SendUserData(IServerUserItem * pIServerUserItem, WORD wSubCmdID)
{
	m_pIGameServiceFrame->SendData(pIServerUserItem,MDM_GF_GAME,wSubCmdID);
	return true;
}

//发送数据
bool __cdecl CTableFrame::SendUserData(IServerUserItem * pIServerUserItem, WORD wSubCmdID, void * pData, WORD wDataSize)
{
	m_pIGameServiceFrame->SendData(pIServerUserItem,MDM_GF_GAME,wSubCmdID,pData,wDataSize);
	return true;
}

//发送数据
bool __cdecl CTableFrame::SendTableData(WORD wChairID, WORD wSubCmdID)
{
	//群发用户
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

	//单一发送
	GT_ASSERT(wChairID<m_wChairCount);
	if ((wChairID<m_wChairCount)&&(m_pIUserItem[wChairID]!=NULL))
	{
		m_pIGameServiceFrame->SendData(m_pIUserItem[wChairID],MDM_GF_GAME,wSubCmdID);
		return true;
	}

	return false;
}

//发送数据
bool __cdecl CTableFrame::SendTableData(WORD wChairID, WORD wSubCmdID, void * pData, WORD wDataSize)
{
	//群发用户
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

	//单一发送
	GT_ASSERT(wChairID<m_wChairCount);
	if ((wChairID<m_wChairCount)&&(m_pIUserItem[wChairID]!=NULL))
	{
		m_pIGameServiceFrame->SendData(m_pIUserItem[wChairID],MDM_GF_GAME,wSubCmdID,pData,wDataSize);
		return true;
	}

	return false;
}

//发送数据
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

//发送数据
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

//发送房间消息
bool __cdecl CTableFrame::SendRoomMessage(IServerUserItem * pIServerUserItem, WORD wErrCode,LPCTSTR lpszMessage, WORD wMessageType)
{
	//构造数据包
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

	//发送数据
	WORD wSendSize=sizeof(Message)-sizeof(Message.szContent)+Message.wMessageLength*sizeof(TCHAR);
	m_pIGameServiceFrame->SendData(pIServerUserItem,MDM_GR_SYSTEM,SUB_GR_MESSAGE,&Message,wSendSize);

	return true;
}

//发送游戏消息
bool __cdecl CTableFrame::SendGameMessage(IServerUserItem * pIServerUserItem, WORD wErrCode,LPCTSTR lpszMessage, WORD wMessageType)
{
	//构造数据包
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

	//发送数据
	WORD wSendSize=sizeof(Message)-sizeof(Message.szContent)+Message.wMessageLength*sizeof(TCHAR);
	m_pIGameServiceFrame->SendData(pIServerUserItem,MDM_GF_FRAME,SUB_GF_MESSAGE,&Message,wSendSize);

	return true;
}

//解散游戏
bool __cdecl CTableFrame::DismissGame()
{
	//状态判断
	if (m_bStarted==false) return false;

	//解散游戏
	m_pITableFrameSink->OnEventGameEnd(INVALID_CHAIR,NULL,GER_DISMISS);

	return true;
}

//结束游戏
bool __cdecl CTableFrame::ConcludeGame()
{
	//设置变量
	m_bStarted=false;
	m_ullTimeStart=0L;
	m_bWriteScore=false;
	m_bGameStatus=GS_FREE;
	memset(m_dwPlayerID,0,sizeof(DWORD)*m_wChairCount);
	memset(m_wOffLineCount,0,sizeof(WORD) * m_wChairCount);
	m_ullGameRoundID = 0;


	//重置桌子
	m_pITableFrameSink->RepositTableFrameSink();

	//设置用户
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

	//断线处理
	for (WORD i=0;i<m_wChairCount;i++)
	{
		if (m_pIUserItem[i]!=NULL)
		{
			//获取用户
			IServerUserItem * pIServerUserItem=m_pIUserItem[i];
			if (pIServerUserItem->GetUserStatus()!=US_OFFLINE) continue;
			
			//删除定时器
			WORD wTimerID=IDI_OFFLINE+i;
			m_pIGameServiceFrame->KillTableTimer(m_wTableID,wTimerID);

			//用户离开
			PerformStandUpAction(pIServerUserItem);

			//清理用户资料
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


	//检测额度
/*	DWORD dwHighScore=m_pGameServiceOption->dwHighScore;
	DWORD dwLessScore=m_pGameServiceOption->dwLessScore;
	if ((dwLessScore!=0L)||(dwHighScore!=0))
	{
		for (WORD i=0;i<m_wChairCount;i++)
		{
			if (m_pIUserItem[i]!=NULL)
			{
				//获取用户
				IServerUserItem * pIServerUserItem=m_pIUserItem[i];
				tagServerUserData * pUserData=pIServerUserItem->GetUserData();

				//判断额度
				if ((dwLessScore!=0L)&&(pUserData->UserData.UserScore.fScore<dwLessScore))
				{
					SendRoomMessage(pIServerUserItem,ErrCode_Message_LessScore,NULL,SMT_EJECT|SMT_INFO|SMT_CLOSE_GAME|SMT_INTERMIT_LINE);
					PerformStandUpAction(pIServerUserItem);
				}
			}
		}
	}*/

	//关闭判断
	if ((m_pIGameServiceFrame->IsAllowEnterGame()==false)||(m_pIGameServiceFrame->IsShallClose()==true))
	{
		for (WORD i=0;i<m_wChairCount;i++)
		{
			if (m_pIUserItem[i]!=NULL)
			{
				//获取用户
				IServerUserItem * pIServerUserItem=m_pIUserItem[i];
				tagServerUserData * pUserData=pIServerUserItem->GetUserData();

				//用户判断
				if (pUserData->UserData.dwMasterRight==0L)
				{
					//用户起立
					SendRoomMessage(pIServerUserItem,ErrCode_Message_CloseRoom,NULL,SMT_EJECT|SMT_INFO|SMT_CLOSE_GAME|SMT_INTERMIT_LINE);
					PerformStandUpAction(pIServerUserItem);

					continue;
				}
			}
		}
	}

	//发送状态
	m_pIGameServiceFrame->SendTableStatus(m_wTableID);

	return true;
}

//发送场景
bool __cdecl CTableFrame::SendGameScene(IServerUserItem * pIServerUserItem, void * pData, WORD wDataSize)
{

	//发送场景
	m_pIGameServiceFrame->SendData(pIServerUserItem,MDM_GF_FRAME,SUB_GF_SCENE,pData,wDataSize);

	return true;
}
//获取计算框架
ICalculateFrame* __cdecl CTableFrame::GetICalculateFrame()
{
	return GET_OBJECT_INTERFACE(m_CalculateFrame,ICalculateFrame);
}
//结算
bool __cdecl CTableFrame::WriteGameEnd(BYTE cbReason,
									   BYTE *pEndData,
									   WORD wEndDataSize,
									   void* pCalculateContext,
									   ICalculateSink* pCalculateSink)
{
	//效验状态
	GT_ASSERT(m_bStarted==true);
	GT_ASSERT(m_bWriteScore==false);
	GT_ASSERT(m_ullGameRoundID != 0);
	if (m_bStarted==false) return false;
	if (m_bWriteScore==true) return false;
	if (m_ullGameRoundID==0) return false;

	//游戏时间
	ULONGLONG dwPlayTimeCount=(ULONGLONG)_time64(NULL)-m_ullTimeStart;

	//写入额度
	m_bWriteScore=true;

#ifndef ALLOW_AI_WRITE_GAMEEND
	//判断全部机器人写入结算记录
	if(IsAllAIGameUser())
	{
		//复位结算
		m_ullGameRoundID = 0;
		m_CalculateFrame.Reset();
		return true;
	}
#endif
	//结算
	if(m_CalculateFrame.CalculateCTP(pCalculateContext, pCalculateSink))
	{

		{
			//定义变量
			DBR_GR_WriteUserChart WriteUserChart;
			DBR_GR_WriteGameRound WriteGameRound;

			for (WORD i=0;i<m_CalculateFrame.GetCalculateItemCount();i++)
			{
				ICalculateItem* pCalculateItem = m_CalculateFrame.EnumCalculateItem(i);
				if (pCalculateItem!=NULL)
				{
					//写入投注报表
					//构造数据
					memset(&WriteUserChart,0,sizeof(WriteUserChart));
					//更新数据
					WriteUserChart.ullGameRoundID = m_ullGameRoundID;
					WriteUserChart.dwUserID = pCalculateItem->GetUserID();
					WriteUserChart.wChairID = pCalculateItem->GetChairID();
					//获取总投注
					pCalculateItem->GetTotalBetScore(&WriteUserChart.decTotalBetScore);
					//判断总投注为0
					if(VarDecCmpR8(&WriteUserChart.decTotalBetScore, 0) == VARCMP_GT)
					{
						//判断只有普通会员玩家才更新信用额度
						if(pCalculateItem->GetUserLevel() == USERLEVEL_GAMEUSER)
						{
							//判断机器人
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
						
						//获取总派彩
						pCalculateItem->GetTotalWinScore(&WriteUserChart.decTotalWinScore);
						//获取总抽水
						pCalculateItem->GetTotalTaxScore(&WriteUserChart.decTotalTaxScore);

						//获取上线,自身,下线派彩
						pCalculateItem->GetWinScoreOccupancy(&WriteUserChart.decWinScoreOccupancy_High,
							&WriteUserChart.decWinScoreOccupancy_Self,
							&WriteUserChart.decWinScoreOccupancy_Less);
						//获取上线,自身,下线抽水
						pCalculateItem->GetTaxScoreOccupancy(&WriteUserChart.decTaxScoreOccupancy_High,
							&WriteUserChart.decTaxScoreOccupancy_Self,
							&WriteUserChart.decTaxScoreOccupancy_Less);

						//获取洗码
						pCalculateItem->GetValidBetScoreAndPaidScore(&WriteUserChart.decValidBetScore_Total,
							&WriteUserChart.decValidBetScore_High,
							&WriteUserChart.decValidBetScore_HighRollback,
							&WriteUserChart.decValidBetScore_Less,
							&WriteUserChart.decValidBetScore_LessRollback,
							&WriteUserChart.decPaidScore_High,
							&WriteUserChart.decPaidScore_Less);
						//获取详细投注信息
						pCalculateItem->GetDetailBetScore(WriteUserChart.cbDetailBetScore,
							WriteUserChart.wDetailBetScoreSize);
						GT_ASSERT(WriteUserChart.wDetailBetScoreSize > 0 && WriteUserChart.wDetailBetScoreSize < sizeof(WriteUserChart.cbDetailBetScore));

						//投递数据库
						m_DataBaseEvent.PostDataBaseEvent(DBR_GR_WRITE_USERCHART,0,
							0,&WriteUserChart,sizeof(WriteUserChart));

					}

				}
			}
			
			//写入游戏记录
			//构造数据
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

			//投递数据库
			m_DataBaseEvent.PostDataBaseEvent(DBR_GR_WRITE_GAMEROUND,0,
				0,&WriteGameRound,sizeof(WriteGameRound));

		}


		//复位结算
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
//申请游戏局记录-检测信用额度事件
void CTableFrame::OnEventAllocGameRoundAndCheckBetScore(DBR_GR_AllocGameRoundAndCheckBetScoreResult * 
															pAllocGameRoundAndCheckBetScoreResult)
{
	if(pAllocGameRoundAndCheckBetScoreResult->ullGameRoundID != 0)
	{
		//效验状态
		//GT_ASSERT(m_bStarted==false);
		if (m_bStarted==true) return;

		//开始判断
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
//申请游戏局记录事件
void CTableFrame::OnEventAllocGameRoundResult(DBR_GR_AllocGameRoundResult * pAllocGameRoundResult)
{
	if(pAllocGameRoundResult->ullGameRoundID != 0)
	{
		//效验状态
		GT_ASSERT(m_bStarted==false);
		if (m_bStarted==true) return;

		//开始判断
		if ((StartVerdict()==true))
		{
			m_ullGameRoundID = pAllocGameRoundResult->ullGameRoundID;
			StartGame();
		}

	}
}
//尝试开始游戏
void CTableFrame::TryStartGame()
{
	//效验状态
	GT_ASSERT(m_bStarted==false);
	if (m_bStarted==true) return;

#ifndef ALLOW_AI_WRITE_GAMEEND
	//判断全部机器人
	if(IsAllAIGameUser())
	{
		m_ullGameRoundID = rand() + 1;
		StartGame();
		return;
	}
#endif

	//检测信用额度
	if(m_pGameServiceAttrib->bCheckCellScoreBeforeStartGame)
	{
		//申请游戏记录
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
		
		//投递数据库
		m_DataBaseEvent.PostDataBaseEvent(DBR_GR_ALLOC_GAMEROUNDANDCHECKBETSCORE,0,
				0,&AllocGameRoundAndCheckBetScore,sizeof(AllocGameRoundAndCheckBetScore));
	}
	else
	{
		//申请游戏记录
		if(m_ullGameRoundID == 0)
		{
			DBR_GR_AllocGameRound AllocGameRound;
			memset(&AllocGameRound, 0, sizeof(AllocGameRound));
			AllocGameRound.ullStartTime = _time64(NULL);
			AllocGameRound.wServerID = m_pGameServiceOption->wServerID;
			AllocGameRound.wTableID  = m_wTableID;

			//投递数据库
			m_DataBaseEvent.PostDataBaseEvent(DBR_GR_ALLOC_GAMEROUND,0,
				0,&AllocGameRound,sizeof(AllocGameRound));

		}
		else
			StartGame();
	}
}

//开始游戏
void CTableFrame::StartGame()
{
	//效验状态
	GT_ASSERT(m_bStarted==false);
	if (m_bStarted==true) return;

	GT_ASSERT(m_ullGameRoundID != 0);

	//设置变量
	m_bStarted=true;
	m_ullTimeStart=(ULONGLONG)_time64(NULL);
	memset(m_wOffLineCount,0,sizeof(WORD) * m_wChairCount);

	//记录额度
	m_bWriteScore=false;
	memset(m_dwPlayerID,0,sizeof(DWORD) * m_wChairCount);

	//复位结算
	m_CalculateFrame.Reset();
	m_CalculateFrame.StructCalculateItem();

	//设置玩家
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

	//发送状态
	m_pIGameServiceFrame->SendTableStatus(m_wTableID);

	//通知事件
	m_pITableFrameSink->OnEventGameStart();

	return;
}

//判断开始
bool CTableFrame::StartVerdict()
{
	//比赛判断
	if (m_bStarted==true) return false;

	//时间模式
	enStartMode StartMode=m_pITableFrameSink->GetGameStartMode();
	if (StartMode==enStartMode_TimeControl) return false;

	//准备人数
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

	//条件判断
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
//全部机器人判断
bool CTableFrame::IsAllAIGameUser()
{
	if(GetGameUserCount() == 0)
		return true;
	else
		return false;
}
//发送坐下失败
void CTableFrame::SendSitFailedPacket(IServerUserItem * pIServerUserItem, WORD wFailedCode)
{
	//构造数据
	CMD_GR_SitFailed SitFailed;
	SitFailed.wErrorCode = wFailedCode;

	//发送数据
	WORD wSendSize=sizeof(SitFailed);
	m_pIGameServiceFrame->SendData(pIServerUserItem,MDM_GR_USER,SUB_GR_SIT_FAILED,&SitFailed,wSendSize);

	return;
}

//////////////////////////////////////////////////////////////////////////
