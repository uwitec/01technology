package 
{
	import flash.display.MovieClip;
	import flash.utils.ByteArray;
	import flash.geom.Point;

	import org.god.IGameFrame.*;
	import org.god.GameModule.Common.*;
	import org.god.Net.*;
	import org.god.GameModule.B_BlackJack.Common.*;
	//游戏类
	public class GameClass extends GameBaseClass
	{
		//定时器标识
		protected static  var IDI_EXIT_GAME:uint=200;//离开游戏定时器
		protected static  var IDI_GIVE_UP:uint=201;//放弃定时器

		protected static  var IDI_ADD_GOLD:uint=202;//加注定时器
		protected static  var IDI_GET_CARD:uint=203;//要牌定时器
	
		//游戏变量
		protected var m_fTurnMaxGold:Number;//最大下注
		protected var m_fTurnBasicGold:Number;//跟注数目
		protected var m_wCardSum:uint;//扑克总值
		protected var m_bCutCard:Boolean;//分牌标识

		//辅助变量
		protected var m_bMeAddGold:Boolean;//是否下注
		protected var m_fGoldShow:Number = 0;//加注筹码

		protected var m_szName:Array = new Array(GAME_PLAYER);	//玩家名字
		//构造函数
		public function GameClass()
		{
			super();
		}
		//查询游戏类接口
		public function QueryIGameClass():IGameClass
		{
			return this  as  GameBaseClass  as  IGameClass;
		}
		//获取游戏视图
		protected function GetPeerGameView():GameView
		{
			if (m_GameClientView == null)
			{
				throw Error("GetPeerGameView m_GameClientView == null");
				return null;
			}
			return m_GameClientView  as  GameView;
		}
		/////////////////////////////////////
		/////////////////////////////////////
		//创建游戏视图
		override protected  function CreateGameView():GameBaseView
		{
			return new GameView(this);
		}
		//初始化游戏类
		override public  function InitGameClass():Boolean
		{
			if (super.InitGameClass() == false)
			{
				return false;
			}
			for(var i:uint = 0; i < m_szName.length; i ++)
			{
				m_szName[i] = "";
			}
			//辅助变量
			m_fGoldShow    = 0;	
			m_fTurnMaxGold = 0;
			m_bMeAddGold   = false;
			m_wCardSum     = 0;
			m_bCutCard     = false;	
			ZeroArray(GetPeerGameView().m_cbPlayStatus);
			GetPeerGameView().addEventListener(GameViewEvent.strGameViewEvent, 
											   OnGameViewEevent);
			//增加声音影射
			AddGameSound("GAME_START",SOUND_GAME_START);
			AddGameSound("GAME_WARN",SOUND_GAME_WARN);
			AddGameSound("TIME_OUT",SOUND_TIME_OUT);
			AddGameSound("HIT_BUTTON",SOUND_HIT_BUTTON);
			AddGameSound("ADD_SCORE",SOUND_ADD_SCORE);
			AddGameSound("GAME_END",SOUND_GAME_END);
			AddGameSound("SEND_CARD",SOUND_SEND_CARD);
			AddGameSound("BRUSH_CARD",SOUND_BRUSH_CARD);
			AddGameSound("GAME_BEGIN",SOUND_GAME_BEGIN);
			AddGameSound("GAME_WIN",SOUND_GAME_WIN);
			
			return true;
		}
		//销毁
		override public  function DestroyGameClass():void
		{
			GetPeerGameView().removeEventListener(GameViewEvent.strGameViewEvent,
												  OnGameViewEevent);
			
			for(var i:uint = 0; i < m_szName.length; i ++)
			{
				m_szName[i] = null;
			}
			m_szName = null;
			
			super.DestroyGameClass();
		}

		////////////////////////////////////////////////////////////////
		//内核事件
		//时间消息
		override protected function OnEventTimer(wChairID:uint,nElapse:uint,nTimerID:uint):Boolean
		{
			//变量界面
			var wViewChairID:uint=this.SwitchViewChairID(wChairID);
			GetPeerGameView().SetUserTimer(wViewChairID,nElapse);

			switch (nTimerID)
			{
				case IDI_ADD_GOLD:		    //加注定时器
				{
					if (nElapse == 0)
					{
						PlayGameSound("TIME_OUT");
						
						OnAddGold(0,0);			
						return true;
					}
					if (nElapse <= 10) PlayGameSound("GAME_WARN");
		
					return true;
				}
			case IDI_GET_CARD:			//要牌定时器
				{	
					if (nElapse == 0)
					{			
						PlayGameSound("TIME_OUT");
						
						OnStopCard(0,0);
						return true;
					}
					if (nElapse <= 10) PlayGameSound("GAME_WARN");
		
					return true;
				}
			case IDI_EXIT_GAME:       //离开游戏定时器
				{
					if (nElapse==0)
					{
						OnExit(0,0);
						return false;
					}
					if (nElapse<=10) PlayGameSound("GAME_WARN");
					return true;
				}
			}

			return false;
		}
		//网络消息
		override protected function OnGameMessage(wSubCmdID:uint,pBuffer:ByteArray,wDataSize:uint):Boolean
		{
			switch (wSubCmdID)
			{
			case SUB_S_GAME_START:           //游戏开始
				{
					return OnSubGameStart(pBuffer,wDataSize);
				}
			case SUB_S_ADD_GOLD:			//加注结果
				{
					return OnSubAddGold(pBuffer,wDataSize);
				}
			case SUB_S_INIT_CARD:           //初始发牌
				{
					return OnSubInitCard(pBuffer,wDataSize);
				}
			case SUB_S_CUT_CARD:			//分牌结果
				{
					return OnSubCutCard(pBuffer,wDataSize);
				}
			case SUB_S_SEND_CARD:			//加注结果
				{
					return OnSubSendCard(pBuffer,wDataSize);
				}
			case SUB_S_STOP_CARD:			//用户停牌
				{
					return OnSubStopCard(pBuffer,wDataSize);
				}
			case SUB_S_BRUST_CARD:			//用户爆牌
				{
					return OnSubBrustCard(pBuffer,wDataSize);
				}
			case SUB_S_BANKER_SEND_CARD:    //庄家发牌
				{			
					return OnSubBankerSendCard(pBuffer,wDataSize);
				}	
			case SUB_S_USER_LEFT:		   //用户强退
				{						
					return OnSubUserLeft(pBuffer,wDataSize);
				}	
			case SUB_S_GAME_END:		   //游戏结束
				{			
					return OnSubGameEnd(pBuffer,wDataSize);
				}	
			case SUB_S_CUT_CARD_MARK:     //切换分牌
				{
					return OnCutCardMark(pBuffer,wDataSize);
				}
			}

			return false;

		}
		//游戏场景
		override protected function OnGameSceneMessage(cbGameStation:uint,bLookonOther:Boolean,pBuffer:ByteArray,wDataSize:uint):Boolean
		{
			switch (cbGameStation)
			{
				case GS_FREE:		//空闲状态
				{		
					//效验数据
					if (wDataSize != CMD_S_StatusFree.sizeof_CMD_S_StatusFree) return false;
		
					var pStatusFree:CMD_S_StatusFree = CMD_S_StatusFree.readData(pBuffer);
		
					//辅助变量
					m_bMeAddGold = false;
		
					//设置界面
					m_fGoldShow      = 0;
					m_fTurnBasicGold = 0;
					GetPeerGameView().SetGoldTitleInfo(0, pStatusFree.fBasicGold);
		
					//设置控件
					if (IsLookonMode() == false)
					{
						GetPeerGameView().m_btStart.ShowWindow(true);
						
						GetPeerGameView().m_btStart.SetFocus();
					}
					return true;
				}
			case GS_ADDGOLDING:		//加注状态
				{		
					//效验数据
					if (wDataSize != CMD_S_StatusAddGold.sizeof_CMD_S_StatusAddGold) return false;
		
					var pStatusAddGold:CMD_S_StatusAddGold = CMD_S_StatusAddGold.readData(pBuffer);	
		
					//游戏变量
					m_fTurnMaxGold   = pStatusAddGold.fMaxGold;
					m_fTurnBasicGold = pStatusAddGold.fBasicGold;
		
					//变量定义
					var wMeChairID:uint  = GetMeChairID();
					var bLookonMode:Boolean = IsLookonMode();
					var fBaseGold:Number   = m_fTurnBasicGold;
		
					//设置状态
					SetGameStatus(GS_ADDGOLDING);
					GetPeerGameView().SetGoldTitleInfo(pStatusAddGold.fMaxGold,fBaseGold);
					GetPeerGameView().m_wBankerStation = SwitchViewChairID(pStatusAddGold.wBankerStation);
					
					for(var i:uint = 0; i < GAME_PLAYER; i++)
					{
						var wViewChairID:uint = SwitchViewChairID(i);			
		
						//加注金币
						GetPeerGameView().SetUserGoldInfo(wViewChairID,true,pStatusAddGold.fTableGold[2 * i]);
						GetPeerGameView().SetUserGoldInfo(wViewChairID,false,pStatusAddGold.fTableGold[2 * i + 1]);
						
						var pUserData:tagUserInfoHead = GetUserInfo(i);
						if (pUserData != null)
						{
							m_szName[i] = String(pUserData.szName);
							if((IsLookonMode() == false) && (GetMeChairID() != pStatusAddGold.wBankerStation))
							{
								//加注界面
								if(!pStatusAddGold.bAddGoldEnd[GetMeChairID()])
								{
									GetPeerGameView().m_GoldControl.SetMaxGold(pStatusAddGold.fMaxGold - m_fTurnBasicGold);
									GetPeerGameView().m_GoldControl.ShowWindow(true);	
									GetPeerGameView().m_btAddGold.ShowWindow(true);
									GetPeerGameView().m_btGiveUp.ShowWindow(true);	
								}
								
								//设置定时器
								if(GetMeChairID() == pStatusAddGold.wCurrentUser)
								{
									SetGameTimer(GetMeChairID(), IDI_ADD_GOLD, 30);
								}						
							}
							//标识状态
							if(!pStatusAddGold.bAddGoldEnd[i] && i != pStatusAddGold.wBankerStation )
							{
								GetPeerGameView().m_cbPlayStatus[wViewChairID] = 1;
							}		
						}			
					}			
					return true;
				}
		
			case GS_PLAYING:	//游戏状态
				{
					//效验数据
					if (wDataSize != CMD_S_StatusPlay.sizeof_CMD_S_StatusPlay) return false;
		
					var pStatusPlay:CMD_S_StatusPlay = CMD_S_StatusPlay.readData(pBuffer);
		
					//游戏变量
					m_fTurnMaxGold   = pStatusPlay.fMaxGold;
					m_fTurnBasicGold = pStatusPlay.fBasicGold;
		
					//变量定义
					var wMeChairID:uint  = GetMeChairID();
					var bLookonMode:Boolean = IsLookonMode();
					var fBaseGold:Number   = m_fTurnBasicGold;
		
					//设置状态
					SetGameStatus(GS_PLAYING);
					GetPeerGameView().SetGoldTitleInfo(pStatusPlay.fMaxGold,fBaseGold);
					//庄家标识
					GetPeerGameView().m_wBankerStation = SwitchViewChairID(pStatusPlay.wBankerStation);

					for(var i:uint = 0; i < GAME_PLAYER; i++)
					{
						//变量定义
						var wViewChairID:uint = SwitchViewChairID(i);
						var  pUserData:tagUserInfoHead = GetUserInfo(i);
		
						if (pUserData != null)
						{
							m_szName[i] = pUserData.szName;			
							GetPeerGameView().m_CardControl[wViewChairID].SetCardData(pStatusPlay.cbTableCardArray[i],pStatusPlay.cbTableCardCount[i]);
		
							if(i != pStatusPlay.wBankerStation)
							{
								//标识状态			
								if(pStatusPlay.cbPlayStatus[i] == STOP_CARD)
								{
									GetPeerGameView().m_cbPlayStatus[wViewChairID] = 3;
								}	
								else if(pStatusPlay.cbPlayStatus[i] == BRUST_CARD)
								{
									GetPeerGameView().m_cbPlayStatus[wViewChairID] = 4;
								}
								else					
								{
									GetPeerGameView().m_cbPlayStatus[wViewChairID] = 2;
								}	
		
							}
							
						}	
						else
						{
							GetPeerGameView().m_CardControl[wViewChairID].SetCardData(null,0);
						}
		
						//加注金币
						GetPeerGameView().SetUserGoldInfo(wViewChairID, true,  pStatusPlay.fTableGold[2 * i]);
						GetPeerGameView().SetUserGoldInfo(wViewChairID, false, pStatusPlay.fTableGold[2 * i + 1]);							
					}			
		
					//设置界面
					if((IsLookonMode() == false) && (GetMeChairID() != pStatusPlay.wBankerStation ))
					{
						if(pStatusPlay.cbPlayStatus[GetMeChairID()] == STOP_CARD || 
						   pStatusPlay.cbPlayStatus[GetMeChairID()] == BRUST_CARD)
						{
							GetPeerGameView().m_btGetCard.ShowWindow(false);
							GetPeerGameView().m_btDouble.ShowWindow(false);
							GetPeerGameView().m_btStop.ShowWindow(false);
							KillGameTimer(IDI_GET_CARD);
						}
						else
						{
							GetPeerGameView().m_btGetCard.ShowWindow(true);
							//GetPeerGameView().m_btDouble.ShowWindow(true);
							GetPeerGameView().m_btStop.ShowWindow(true);	
							SetGameTimer(GetMeChairID(), IDI_GET_CARD, 30);
						}			
				
					}	
		
					//设置界面
					if((IsLookonMode() == false) && (GetMeChairID() == pStatusPlay.wBankerStation))
					{
						if(pStatusPlay.bBankerGetCard)
						{					
							GetPeerGameView().m_btGetCard.ShowWindow(true);		
							GetPeerGameView().m_btStop.ShowWindow(true);
		
							//设置定时器
							SetGameTimer(GetMeChairID(), IDI_GET_CARD, 20);
						}
						else
						{
							GetPeerGameView().m_btGetCard.ShowWindow(false);		
							GetPeerGameView().m_btStop.ShowWindow(false);
		
							//设置定时器
							KillGameTimer(IDI_GET_CARD);
						}
					}
		
					//设置控件
					GetPeerGameView().m_CardControl[SwitchViewChairID(GetMeChairID())].ShowFirstCard(true);
					
					if (IsLookonMode() == false) 
					{
						ActiveGameFrame();
						GetPeerGameView().m_CardControl[2].AllowPositively(true);
					}	
		
					GetPeerGameView().UpdateGameView();
		
					return true;
				}		
			}

			return false;
		}


		/////////////////////////////////////////////////////////////
		//消息处理
		//游戏开始
		protected function OnSubGameStart(pBuffer:ByteArray,wDataSize:uint):Boolean
		{	
			//效验数据
			if (wDataSize != CMD_S_GameStart.sizeof_CMD_S_GameStart) return false;
			var pGameStart:CMD_S_GameStart = CMD_S_GameStart.readData(pBuffer);
		
			//游戏变量
			m_fTurnMaxGold   = pGameStart.fMaxGold;
			m_fTurnBasicGold = pGameStart.fTurnBasicGold;
		
			ZeroArray(m_szName, "");
		
			//变量定义
			var wMeChairID:uint  = GetMeChairID();
			var bLookonMode:Boolean = IsLookonMode();
			var fBaseGold:Number   = m_fTurnBasicGold;
		
			//设置状态
			SetGameStatus(GS_ADDGOLDING);
			GetPeerGameView().SetGoldTitleInfo(pGameStart.fMaxGold,fBaseGold);
		
			//设置界面
			GetPeerGameView().m_wBankerStation = SwitchViewChairID(pGameStart.wBankerStation);	
			
			//设置控制
			if (bLookonMode == false && wMeChairID != pGameStart.wBankerStation)
			{
				//设置界面
				GetPeerGameView().m_btGiveUp.ShowWindow(true);
				GetPeerGameView().m_btAddGold.ShowWindow(true);
				
				GetPeerGameView().SetUserGoldInfo(2, false, m_fTurnBasicGold);
				GetPeerGameView().m_GoldControl.SetMaxGold(pGameStart.fMaxGold - m_fTurnBasicGold);
				GetPeerGameView().m_GoldControl.ShowWindow(true);
		
				//设置定时器	
				SetGameTimer(GetMeChairID(), IDI_ADD_GOLD, 30);		
			}
		
			for (var i:uint = 0; i < GAME_PLAYER; i++)
			{
				//变量定义
				var wViewChairID:uint = SwitchViewChairID(i);
				var pUserData:tagUserInfoHead = GetUserInfo(i);
		
				//设置界面
				if (pUserData != null)
				{
					GetPeerGameView().SetUserGoldInfo(wViewChairID,false,0);
					GetPeerGameView().SetUserGoldInfo(wViewChairID,true,fBaseGold);
					m_szName[i] = String(pUserData.szName);			
					if(wViewChairID != GetPeerGameView().m_wBankerStation)
						GetPeerGameView().m_cbPlayStatus[wViewChairID] = 1;
				}
				else
				{	
					GetPeerGameView().SetUserGoldInfo(wViewChairID,true,0);
					GetPeerGameView().SetUserGoldInfo(wViewChairID,false,0);	
				}
		
			}
			if (bLookonMode == false) 
			{
				ActiveGameFrame();
				GetPeerGameView().m_CardControl[2].AllowPositively(true);
			}	
			else
			{	
				//设置界面			
				GetPeerGameView().m_ScoreView.ShowWindow(false);	
				for (var i:uint=0;i<CountArray(GetPeerGameView().m_CardControl);i++) 
					GetPeerGameView().m_CardControl[i].SetCardData(null,0);
				ZeroArray(GetPeerGameView().m_cbTopCardSum);
				ZeroArray(GetPeerGameView().m_cbBottomCardSum);
			}
			GetPeerGameView().UpdateGameView();
			
			//播放声音
			PlayGameSound("GAME_BEGIN");
		
			return true;
		}
		
		//初始发牌
		protected function OnSubInitCard(pBuffer:ByteArray,wDataSize:uint):Boolean
		{
			//播放声音
			PlayGameSound("SEND_CARD");
		
			//效验数据
			if (wDataSize != CMD_S_InitCard.sizeof_CMD_S_InitCard) return false;
			var  pInitCard:CMD_S_InitCard = CMD_S_InitCard.readData(pBuffer);
		
			var cbCardData:Array = new Array(2);	
			
			for (var i:uint = 0; i < GAME_PLAYER; i++)
			{
				//变量定义
				var wViewChairID:uint = SwitchViewChairID(i);
				var pUserData:tagUserInfoHead = GetUserInfo(i);
		
				//设置界面
				if (pUserData != null)
				{					
					cbCardData[1] = pInitCard.cbCardData[i];			
					cbCardData[0] = (wViewChairID == 2) ? pInitCard.cbFundusCard : 0;
		
					GetPeerGameView().m_CardControl[wViewChairID].SetCardData(cbCardData,2);	
		
					//标识状态
					if(wViewChairID != GetPeerGameView().m_wBankerStation)
						GetPeerGameView().m_cbPlayStatus[wViewChairID] = 2;
				}
				else
				{
					GetPeerGameView().m_CardControl[wViewChairID].SetCardData(null,0);
				}		
		
				//设置控件
				GetPeerGameView().m_CardControl[wViewChairID].ShowFirstCard(true);
			}
		
			//界面设置
			if(SwitchViewChairID(GetMeChairID()) != GetPeerGameView().m_wBankerStation && !IsLookonMode())
			{
				GetPeerGameView().m_btGetCard.ShowWindow(true);
				//GetPeerGameView().m_btDouble.ShowWindow(true);
				GetPeerGameView().m_btStop.ShowWindow(true);	
			
				//能否分牌
				//if(pInitCard.bCanCutCard) GetPeerGameView().m_btCutCard.ShowWindow(true);	
		
				//设置定时器
				SetGameTimer(GetMeChairID(), IDI_GET_CARD, 30);
			}	
		
			//显示扑克总值
			GetPeerGameView().m_cbTopCardSum[2] = pInitCard.cbCardSum;
			GetPeerGameView().UpdateGameView();
		
			return true;
		}
		
		//用户分牌
		protected function OnSubCutCard(pBuffer:ByteArray,wDataSize:uint):Boolean
		{
			//效验数据
			if (wDataSize != CMD_S_CutCard.sizeof_CMD_S_CutCard) return false;
			var pCutCard:CMD_S_CutCard =CMD_S_CutCard.readData(pBuffer);
		
			var cbFirstCard:Array = new Array(1); 
			var cbSecondCard:Array = new Array(1);
			cbFirstCard[0]     = pCutCard.cbFirstCard;
			cbSecondCard[0]    = pCutCard.cbSecondCard;
			var wViewChairID:uint = SwitchViewChairID(pCutCard.wCutCardUser);
		
			GetPeerGameView().m_CardControl[wViewChairID].SetCardData(cbFirstCard, 1);			
			GetPeerGameView().m_CardControl[wViewChairID + 4].SetCardData(cbSecondCard,1);
		
			if(pCutCard.wCutCardUser == GetMeChairID())
			{
				GetPeerGameView().m_CardControl[wViewChairID + 4].ShowFirstCard(true);
				GetPeerGameView().m_cbCutCardMark  = 1;
				GetPeerGameView().m_cbTopCardSum[wViewChairID]    = pCutCard.cbFirstCardSum;
				GetPeerGameView().m_cbBottomCardSum[wViewChairID] = pCutCard.cbSecondCardSum;
			}
			else
			{
				GetPeerGameView().m_CardControl[wViewChairID + 4].ShowFirstCard(false);
				GetPeerGameView().m_CardControl[wViewChairID].ShowFirstCard(false);
			}
			GetPeerGameView().UpdateGameView();
		
			//标识分牌
			m_bCutCard = false;
		
			return true;
		}
		
		//用户加注
		protected function OnSubAddGold(pBuffer:ByteArray,wDataSize:uint):Boolean
		{
			//效验数据
			if (wDataSize != CMD_S_AddGold.sizeof_CMD_S_AddGold) return false;
			var pAddGold:CMD_S_AddGold = CMD_S_AddGold.readData(pBuffer);
		
			//设置状态
			SetGameStatus(GS_PLAYING);
		
			//变量定义
			var wMeChairID:uint   = GetMeChairID();
			var wViewChairID:uint = SwitchViewChairID(pAddGold.wCurrentUser);
		
			//处理数据
			GetPeerGameView().SetUserGoldInfo(wViewChairID,false,pAddGold.fLastAddGold);
		
			if ((IsLookonMode() == false)&&(pAddGold.wCurrentUser == wMeChairID))
			{
				//设置数据
				m_bMeAddGold = true;
			
				//不是双倍
				if(!pAddGold.bDoubleGold)
				{
					//获取扑克
					var wCardCount:uint = GetPeerGameView().m_CardControl[2].GetCardData(null,0);
				}
				else
				{
					PlayGameSound("ADD_SCORE");
				}
				ActiveGameFrame();
			}
		
			//标识状态
			if(wViewChairID != GetPeerGameView().m_wBankerStation)
				GetPeerGameView().m_cbPlayStatus[wViewChairID] = 0;
			GetPeerGameView().UpdateGameView();
		
			return true;
		}
		
		//切换分牌
		protected function OnCutCardMark(pBuffer:ByteArray,wDataSize:uint):Boolean
		{	
			GetPeerGameView().m_cbCutCardMark = 2;
		
			GetPeerGameView().m_btGetCard.ShowWindow(true);
			GetPeerGameView().m_btStop.ShowWindow(true);
			//GetPeerGameView().m_btDouble.ShowWindow(true);
		
			GetPeerGameView().UpdateGameView();
		
			//标识分牌
			m_bCutCard = true;
		
			return true;
		}
		//发牌消息
		protected function OnSubSendCard(pBuffer:ByteArray,wDataSize:uint):Boolean
		{		
			//播放声音
			PlayGameSound("SEND_CARD");
			
			//效验数据
			if (wDataSize != CMD_S_SendCard.sizeof_CMD_S_SendCard) return false;
			var pSendCard:CMD_S_SendCard = CMD_S_SendCard.readData(pBuffer);
		
			//更新界面
			var cbCardData:Array = new Array(8);
			var fUserTableGold:Number = 0;
		
			var wCardCount:uint;
			var wViewChairID:uint = SwitchViewChairID(pSendCard.wGetCardUser);
		
			//左手牌
			if(!pSendCard.bCutCard)
			{
				wCardCount = GetPeerGameView().m_CardControl[wViewChairID].GetCardData(cbCardData,CountArray(cbCardData));
				cbCardData[wCardCount++] = pSendCard.cbUserCard;
				GetPeerGameView().m_CardControl[wViewChairID].SetCardData(cbCardData,wCardCount);
		
				//显示扑克总值
				if(wViewChairID == 2)
				{
					GetPeerGameView().m_cbTopCardSum[2] = pSendCard.cbCardSum;	
				}	
			}
			else
			{
				//右手牌
				wCardCount = GetPeerGameView().m_CardControl[wViewChairID + 4].GetCardData(cbCardData,CountArray(cbCardData));
				cbCardData[wCardCount++] = pSendCard.cbUserCard;
				GetPeerGameView().m_CardControl[wViewChairID + 4].SetCardData(cbCardData,wCardCount);
		
				//显示扑克总值
				if(wViewChairID == 2)
				{
					GetPeerGameView().m_cbBottomCardSum[2] = pSendCard.cbCardSum;			
				}	
			}
		
			GetPeerGameView().UpdateGameView();
		
			return true;
		}
		
		//停牌消息
		protected function OnSubStopCard(pBuffer:ByteArray,wDataSize:uint):Boolean
		{
			//效验数据
			if (wDataSize != CMD_S_StopCard.sizeof_CMD_S_StopCard) return false;
			var pStopCard:CMD_S_StopCard = CMD_S_StopCard.readData(pBuffer);
		
			var wViewChairID:uint = SwitchViewChairID(pStopCard.wStopUser);
		
			//标识状态
			if((wViewChairID != GetPeerGameView().m_wBankerStation) && (wViewChairID != 2)) 
			{
				if(!pStopCard.bDoubleBrsut)
					GetPeerGameView().m_cbPlayStatus[wViewChairID] = 3;
				else
					GetPeerGameView().m_cbPlayStatus[wViewChairID] = 4;
			}	
		
			if(wViewChairID == GetPeerGameView().m_wBankerStation) GetPeerGameView().m_cbPlayStatus[wViewChairID] = 0;
		
			GetPeerGameView().UpdateGameView();
		
			return true;
		}
		
		//爆牌消息
		protected function OnSubBrustCard(pBuffer:ByteArray,wDataSize:uint):Boolean
		{
			if (wDataSize != CMD_S_BrustCard.sizeof_CMD_S_BrustCard) return false;
			var pBrustCard:CMD_S_BrustCard = CMD_S_BrustCard.readData(pBuffer);
		
			var wViewChairID:uint = SwitchViewChairID(pBrustCard.wBrustUser);
			
			if(pBrustCard.wBrustUser == GetMeChairID())
			{
				//播放声音
				PlayGameSound("BRUSH_CARD");
		
				//设置定时器
				KillGameTimer(IDI_GET_CARD);
				
				//更新界面
				GetPeerGameView().m_btGetCard.ShowWindow(false);
				GetPeerGameView().m_btStop.ShowWindow(false);
				GetPeerGameView().m_btDouble.ShowWindow(false);
				GetPeerGameView().m_btCutCard.ShowWindow(false);		
			}
		
			//设置界面
			GetPeerGameView().m_CardControl[wViewChairID].ShowFirstCard(true);	
		
			//标识状态
			if(wViewChairID != GetPeerGameView().m_wBankerStation)
				GetPeerGameView().m_cbPlayStatus[wViewChairID] = 4;
			GetPeerGameView().UpdateGameView();
		
			return true;
		}
		
		//庄家发牌
		protected function OnSubBankerSendCard(pBuffer:ByteArray,wDataSize:uint):Boolean
		{
			//设置界面
			GetPeerGameView().m_btGetCard.ShowWindow(true);
			GetPeerGameView().m_btStop.ShowWindow(true);
		
			//设置定时器
			SetGameTimer(GetMeChairID(), IDI_GET_CARD, 30);
			GetPeerGameView().UpdateGameView();
		
			return true;
		}
		
		//用户强退
		 protected function OnSubUserLeft(pBuffer:ByteArray,wDataSize:uint):Boolean
		{
			return true;
		}
		
		//游戏结束
		 protected function OnSubGameEnd(pBuffer:ByteArray,wDataSize:uint):Boolean
		{	
			//设置定时器
			KillGameTimer(IDI_GET_CARD);
		
			//效验参数
			if (wDataSize != CMD_S_GameEnd.sizeof_CMD_S_GameEnd) return false;
			var pGameEnd:CMD_S_GameEnd = CMD_S_GameEnd.readData(pBuffer);
		    
			//游戏正常结束
			var cbCardData:Array =new Array(13);
			GetPeerGameView().m_ScoreView.ResetScore();
		
			for (var i:uint = 0; i < GAME_PLAYER; i++)
			{
				//设置信息
				var  pUserData:tagUserInfoHead = GetUserInfo(i);
		
				if (pUserData != null)
				{
					m_szName[i] = String(pUserData.szName);
				}
				
				if (m_szName[i] != "") 
				{	
					GetPeerGameView().m_ScoreView.SetGameScore(i,pGameEnd.fGameGold[i]);
		
					if (pGameEnd.cbUserCard[i] != 0)
					{
						var wViewStation:uint = SwitchViewChairID(i);
						var wCardCount:uint = GetPeerGameView().m_CardControl[wViewStation].GetCardData(cbCardData,CountArray(cbCardData));
						cbCardData[0] = pGameEnd.cbUserCard[i];
						GetPeerGameView().m_CardControl[wViewStation].SetCardData(cbCardData,wCardCount);
						GetPeerGameView().m_CardControl[wViewStation].SortCardData();
						GetPeerGameView().m_CardControl[wViewStation].ShowFirstCard(true);	
		
						wCardCount = GetPeerGameView().m_CardControl[wViewStation+4].GetCardData(cbCardData,CountArray(cbCardData));
						cbCardData[0] = pGameEnd.cbCutCard[i];
						GetPeerGameView().m_CardControl[wViewStation+4].SetCardData(cbCardData[0] == 0 ? null :cbCardData,
																		cbCardData[0] == 0 ? 0 : wCardCount);
						GetPeerGameView().m_CardControl[wViewStation+4].SortCardData();
						GetPeerGameView().m_CardControl[wViewStation+4].ShowFirstCard(true);		
						
						//显示总值
						GetPeerGameView().m_cbTopCardSum[wViewStation]    = pGameEnd.cbCardSum[i];
						GetPeerGameView().m_cbBottomCardSum[wViewStation] = pGameEnd.cbCutCardSum[i];
					}			
				}			
			}
			GetPeerGameView().m_ScoreView.SetMeChairID(GetMeChairID());
			GetPeerGameView().m_ScoreView.ShowWindow(true);	
		
			//游戏变量
			m_fTurnMaxGold   = 0;
			m_fTurnBasicGold = 0;
		
			//界面变量
			m_fGoldShow  = 0;	
			GetPeerGameView().m_wBankerStation = 255;
			GetPeerGameView().m_cbCutCardMark  = 0;
			ZeroArray(m_szName, "");
			ZeroArray(GetPeerGameView().m_cbPlayStatus);
		
			//标识分牌
			m_bCutCard = false;
		
			//设置界面
			if (IsLookonMode() == false)
			{
				GetPeerGameView().m_btStart.ShowWindow(true);
				
				GetPeerGameView().m_btStart.SetFocus();
				SetGameTimer(GetMeChairID(),IDI_EXIT_GAME,30);
			}
			
			GetPeerGameView().m_GoldControl.ShowWindow(false);
			GetPeerGameView().m_btGiveUp.ShowWindow(false);
			GetPeerGameView().m_btAddGold.ShowWindow(false);
		
			GetPeerGameView().m_btDouble.ShowWindow(false);
			GetPeerGameView().m_btGetCard.ShowWindow(false);
			GetPeerGameView().m_btStop.ShowWindow(false);
			GetPeerGameView().m_btCutCard.ShowWindow(false);	
			
			GetPeerGameView().UpdateGameView();
		
			//设置状态
			SetGameStatus(GS_FREE);
		
			//播放声音
			if(pGameEnd.fGameGold[GetMeChairID()] > 0)
				PlayGameSound("GAME_WIN");
			else
				PlayGameSound("GAME_END");	
		
			return true;
		}
		
		//游戏开始
		override protected function OnStart(wParam:uint, lParam:uint):uint
		{
			//播放声音
			PlayGameSound("HIT_BUTTON");
			
			//设置界面	
			GetPeerGameView().m_btStart.ShowWindow(false);
			
			GetPeerGameView().m_ScoreView.ShowWindow(false);	
			for (var i:uint=0;i<CountArray(GetPeerGameView().m_CardControl);i++) GetPeerGameView().m_CardControl[i].SetCardData(null,0);
			ZeroArray(GetPeerGameView().m_cbTopCardSum);
			ZeroArray(GetPeerGameView().m_cbBottomCardSum);
			ZeroArray(GetPeerGameView().m_cbPlayStatus);
		
			KillGameTimer(IDI_EXIT_GAME);
		
			//发送消息
		   SendUserReady(null,0);
		
			return 0;
		}
		
		//用户下注
		protected function OnAddGold(wParam:uint, lParam:uint):uint
		{	
			//播放声音
			PlayGameSound("ADD_SCORE");
			
			var fGold:uint = /* GetPeerGameView().m_GoldView[4].GetGold() +*/ GetPeerGameView().m_GoldControl.GetGold();
		
			//fGold = __max(fGold,1);
			//if(fGold < m_fTurnBasicGold)
				//fGold = m_fTurnBasicGold;
		
			//设置信息
			var AddPoint:CMD_C_AddGold = new CMD_C_AddGold;
			AddPoint.bDoubleGold = 0;
			AddPoint.fGold       = fGold;
			var sendData:ByteArray = AddPoint.toByteArray();
			SendGameData(SUB_C_ADD_GOLD,sendData,CMD_C_AddGold.sizeof_CMD_C_AddGold);
		
			//设置定时器
			KillGameTimer(IDI_ADD_GOLD);	
		
			//更新界面
			GetPeerGameView().m_GoldControl.ShowWindow(false);
			GetPeerGameView().m_btGiveUp.ShowWindow(false);
			GetPeerGameView().m_btAddGold.ShowWindow(false);
			GetPeerGameView().UpdateGameView();
		
			return 0;
		}
		
		//用户取消
		protected function OnGiveUp(wParam:uint, lParam:uint):uint
		{
			//播放声音
			PlayGameSound("HIT_BUTTON");
		
			var AddPoint:CMD_C_AddGold =new CMD_C_AddGold;
			//AddPoint.fGold       = 1;
			AddPoint.fGold       = 0;
			AddPoint.bDoubleGold = 0;
			//if(AddPoint.fGold < m_fTurnBasicGold)
				//AddPoint.fGold = m_fTurnBasicGold;
			//设置定时器
			KillGameTimer(IDI_ADD_GOLD);
			
			//设置界面
			GetPeerGameView().m_GoldControl.ShowWindow(false);
			GetPeerGameView().m_btGiveUp.ShowWindow(false);
			GetPeerGameView().m_btAddGold.ShowWindow(false);
		    
			//发送数据
			var sendData:ByteArray = AddPoint.toByteArray();
			SendGameData(SUB_C_ADD_GOLD,sendData,CMD_C_AddGold.sizeof_CMD_C_AddGold);
		
			return 0;
		}
		
		//用户要牌
		protected function OnGetCard(wParam:uint, lParam:uint):uint
		{	
			//播放声音
			PlayGameSound("HIT_BUTTON");
			
			//设置定时器
			KillGameTimer(IDI_GET_CARD);
			SetGameTimer(GetMeChairID(), IDI_GET_CARD, 30);	

		
			//发送消息
			var GetCard:CMD_C_GetCard = new CMD_C_GetCard;
			GetCard.bCutCard = m_bCutCard ? 1 : 0;
		
			var sendData:ByteArray = GetCard.toByteArray();
			SendGameData(SUB_C_GET_CARD,sendData,CMD_C_GetCard.sizeof_CMD_C_GetCard);
		
			//设置界面
			GetPeerGameView().m_btCutCard.ShowWindow(false);	
		
			return 0;
		}
		
		//用户停牌
		protected function OnStopCard(wParam:uint, lParam:uint):uint
		{
			//播放声音
			PlayGameSound("HIT_BUTTON");
			
			//停止定时器
			KillGameTimer(IDI_GET_CARD);
		
			//设置界面
			GetPeerGameView().m_btGetCard.ShowWindow(false);
			GetPeerGameView().m_btDouble.ShowWindow(false);
			GetPeerGameView().m_btStop.ShowWindow(false);
			GetPeerGameView().m_btCutCard.ShowWindow(false);
		
			//发送消息
			var StopCard:CMD_C_StopCard = new CMD_C_StopCard;
			StopCard.bCutCard = m_bCutCard ? 1 : 0;
			var sendData:ByteArray = StopCard.toByteArray();
			SendGameData(SUB_C_STOP_CARD,sendData,CMD_C_StopCard.sizeof_CMD_C_StopCard);
		
			return 0;
		}
		
		//用户双倍
		protected function OnDoubleCard(wParam:uint, lParam:uint):uint
		{
			//播放声音
			PlayGameSound("ADD_SCORE");
			
			//停止定时器
			KillGameTimer(IDI_GET_CARD);
			
			//发送消息
			var AddPoint:CMD_C_AddGold = new CMD_C_AddGold;
			AddPoint.bDoubleGold = 1;
			AddPoint.fGold       = 0;   
			var sendData:ByteArray = AddPoint.toByteArray();
			SendGameData(SUB_C_ADD_GOLD,sendData,CMD_C_AddGold.sizeof_CMD_C_AddGold);	
			
			var GetCard:CMD_C_GetCard = new CMD_C_GetCard;
			GetCard.bCutCard = m_bCutCard ? 1 : 0;
			var sendData1:ByteArray = GetCard.toByteArray();
			SendGameData(SUB_C_GET_CARD,sendData1,CMD_C_GetCard.sizeof_CMD_C_GetCard);
		
			SendGameData(SUB_C_STOP_CARD);	
		
			//设置界面
			GetPeerGameView().m_btGetCard.ShowWindow(false);
			GetPeerGameView().m_btDouble.ShowWindow(false);
			GetPeerGameView().m_btStop.ShowWindow(false);
			GetPeerGameView().m_btCutCard.ShowWindow(false);
		
			return 0;
		}
		
		//用户分牌
		protected function OnCutCard(wParam:uint, lParam:uint):uint
		{
			//播放声音
			PlayGameSound("HIT_BUTTON");	
		
			//发送消息
			SendGameData(SUB_C_CUT_CARD);
		
			//设置界面
			GetPeerGameView().m_btCutCard.ShowWindow(false);
			
			return 0;
		}
		//游戏视图按钮事件
		private function OnGameViewEevent(e:GameViewEvent):void
		{
			switch(e.m_nMsg)
			{
				case GameViewEvent.nGVM_GAME_START://开始游戏
				{
					OnStart(e.m_nWParam,e.m_nLParam);
					return;
				}
				case GameViewEvent.nGVM_ADD_GOLD://下注
				{
					OnAddGold(e.m_nWParam,e.m_nLParam);
					return;
				}
				case GameViewEvent.nGVM_GIVE_UP://放弃
				{
					OnGiveUp(e.m_nWParam,e.m_nLParam);
					return;
				}
				case GameViewEvent.nGVM_GET_CARD://要牌
				{
					OnGetCard(e.m_nWParam,e.m_nLParam);
					return;
				}
				case GameViewEvent.nGVM_STOP_CARD://停牌
				{
					OnStopCard(e.m_nWParam,e.m_nLParam);
					return;
				}
				case GameViewEvent.nGVM_DOUBLE_CARD://双倍
				{
					OnDoubleCard(e.m_nWParam,e.m_nLParam);
					return;
				}
				case GameViewEvent.nGVM_CUT_CARD://分牌
				{
					OnCutCard(e.m_nWParam,e.m_nLParam);
					return;
				}
			}
		}

		//////////////////////////////////////
		//辅助函数

	}
}
include "../../Common/Memory.as"
include "../../Net/GlobalDef.as"
include "../../Net/GlobalFrame.as"
include "./Common/GameLogicDef.as"
include "./Common/CMD_BLACKJACK.as"