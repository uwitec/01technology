package 
{
	import flash.display.MovieClip;
	import flash.utils.ByteArray;
	import flash.geom.Point;

	import org.god.IGameFrame.*;
	import org.god.GameModule.Common.*;
	import org.god.Net.*;
	import org.god.GameModule.B_Land.Common.*;

	public class GameClass extends GameBaseClass
	{
		//游戏定时器
		private static  const IDI_OUT_CARD:int = 200;//出牌定时器
		private static  const IDI_MOST_CARD:int = 201;//最大定时器
		private static  const IDI_EXIT_GAME:int = 202;//离开游戏定时器
		private static  const IDI_LAND_SCORE:int = 203;//叫分定时器
		private static  const IDI_LAST_TURN_CARD:uint=204;//上轮定时器
		
		protected static  var TIME_OUT_CARD:uint=30;//出牌时间
		protected static  var TIME_MOST_OUT_CARD:uint=3;//叫牌时间

		//游戏变量
		protected var m_wLandUser:uint;//用户
		protected var m_wBombTime:uint;//炸弹倍数
		protected var m_bCardCount:Array = new Array(GAME_PLAYER);//扑克数目
		protected var m_bHandCardCount:uint;//扑克数目
		protected var m_bHandCardData:Array = new Array(20);//手上扑克
		protected var m_cbOutCardCount:Array = new Array(GAME_PLAYER);//出牌数目
		protected var m_cbOutCardData:Array = new Array(GAME_PLAYER);//出牌列表
		protected var m_bPass:Array = new Array(GAME_PLAYER);//放弃数组
		protected var m_wFirstOutUser:uint;
		
		
		//出牌变量
		protected var m_bLastTurn:Boolean;//上轮标志
		protected var m_bTurnOutType:uint;//出牌类型
		protected var m_bTurnCardCount:uint;//出牌数目
		protected var m_bTurnCardData:Array = new Array(20);//出牌列表

		//配置变量
		protected var m_bDeasilOrder:Boolean;//出牌顺序
		protected var m_dwCardHSpace:uint;//扑克象素
		protected var m_bStustee:Boolean;//托管标志

		//辅助变量
		protected var m_wMostUser:uint;//最大玩家
		protected var m_wTimeOutCount:uint;//超时次数

		//控件变量
		protected var m_GameLogic:GameLogic;//游戏逻辑
		
		public function GameClass()
		{
			super();

		}
		public function QueryIGameClass():IGameClass
		{
			return this  as  GameBaseClass  as  IGameClass;
		}
		protected function GetPeerGameView():GameView
		{
			if(m_GameClientView == null)
			{
				throw Error("GetPeerGameView m_GameClientView == null");
				return null;
			}
			return m_GameClientView  as  GameView;
		}
		/////////////////////////////////////
		/////////////////////////////////////
		override protected function CreateGameView():GameBaseView
		{
			return new GameView(this);
		}
		
		override public function InitGameClass():Boolean
		{
			if(super.InitGameClass() == false)
				return false;
			//游戏变量
			m_wBombTime=1;
			m_bHandCardCount=0;
			m_wLandUser=INVALID_CHAIR;
			var i:uint = 0;

			ZeroArray(m_bCardCount, 0);
			ZeroArray(m_bHandCardData, 0x43);
			for (var i:uint = 0; i < m_cbOutCardCount.length; i ++)
			{
				m_cbOutCardCount[i] = new Array(2);
			}
			for (i = 0; i < m_bPass.length; i ++)
			{
				m_bPass[i] = new Array(2);
			}
			for (i = 0; i < m_cbOutCardData.length; i ++)
			{
				m_cbOutCardData[i] = new Array(2);
				for (var k:uint = 0; k < m_cbOutCardData[i].length; k ++)
				{
					m_cbOutCardData[i][k] = new Array(20);
				}
			}
			ZeroThreeDimensionArray(m_cbOutCardData);
			ZeroTwoDimensionArray(m_cbOutCardCount);
			ZeroTwoDimensionArray(m_bPass,false);

			//配置变量
			m_bDeasilOrder=false;
			m_dwCardHSpace=DEFAULT_PELS;
			m_bStustee = false;

			//出牌变量
			m_bTurnCardCount=0;
			m_bTurnOutType=CT_INVALID;
			ZeroArray(m_bTurnCardData, 0);
			m_bLastTurn=false;
			m_wFirstOutUser = INVALID_CHAIR;

			//辅助变量
			m_wTimeOutCount=0;
			m_wMostUser=INVALID_CHAIR;
			
			m_GameLogic = new GameLogic();
			
			//读取配置
			m_dwCardHSpace=DEFAULT_PELS;
			m_bDeasilOrder=false;

			//调整参数
			if ((m_dwCardHSpace>MAX_PELS)||(m_dwCardHSpace<LESS_PELS))
			{
				m_dwCardHSpace=DEFAULT_PELS;
			}
			//配置控件
			GetPeerGameView().SetUserOrder(m_bDeasilOrder);
			GetPeerGameView().m_HandCardControl.SetCardSpace(m_dwCardHSpace,0,20);
			GetPeerGameView().addEventListener(GameViewEvent.strGameViewEvent, OnGameViewEevent);
			
			AddGameSound("GAME_START",SOUND_GAME_START);
			AddGameSound("GAME_WARN",SOUND_GAME_WARN);
			AddGameSound("GAME_END",SOUND_GAME_END);
			AddGameSound("GAME_WIN",SOUND_GAME_WIN);
			AddGameSound("GAME_LOST",SOUND_GAME_LOST);
			AddGameSound("OUT_CARD",SOUND_OUT_CARD);
			AddGameSound("MOST_CARD",SOUND_MOST_CARD);
			AddGameSound("BOMP",SOUND_BOMP);
			
			return true;
		}
		//重置状态
		override public function DestroyGameClass():void
		{
			//游戏变量
			m_wBombTime=1;
			m_bHandCardCount=0;
			m_wLandUser=INVALID_CHAIR;
			
			m_bCardCount = null;
			m_bHandCardData = null;
			m_cbOutCardCount  = null;
			m_cbOutCardData   = null;
			m_bPass = null;
			
			//出牌变量
			m_bTurnCardCount=0;
			m_bTurnOutType=CT_INVALID;
			ZeroArray(m_bTurnCardData);
			m_bTurnCardData = null;

			//辅助变量
			m_wTimeOutCount=0;
			m_wMostUser=INVALID_CHAIR;

			//删除定时
			KillGameTimer(0);
			m_TimerWork.KillTimer(IDI_MOST_CARD);

			if(GetPeerGameView())
				GetPeerGameView().removeEventListener(GameViewEvent.strGameViewEvent, OnGameViewEevent);
			m_GameLogic = null;
			
			super.DestroyGameClass();
		}
		
		////////////////////////////////
		private function GenEmptyCardData(nCardCount:int):Array
		{
			var result:Array = new Array(nCardCount);
			ZeroArray(result, 0x43);
			return result;
		}
		private function UpdateLeaveCardControl(bCardData:Array):void
		{
			for (var n:int=0; n<GAME_PLAYER; n++)
			{
				var wViewChairID:uint=SwitchViewChairID(n);
				if (wViewChairID==0)
				{
					GetPeerGameView().m_LeaveCardControl[0].SetCardData(bCardData,m_bCardCount[n]);
				}
				else if (wViewChairID==2)
				{
					GetPeerGameView().m_LeaveCardControl[1].SetCardData(bCardData,m_bCardCount[n]);
				}
			}	
		}
		////////////////////////////////////////////////////////////////
		//内核事件
		//时间消息
		override protected function OnEventTimer(wChairID:uint,  nElapse:uint,  nTimerID:uint):Boolean
		{
			//变量界面
			var wViewChairID:uint=this.SwitchViewChairID(wChairID);
			GetPeerGameView().SetUserTimer(wViewChairID,nElapse);

			switch (nTimerID)
			{
				case IDI_OUT_CARD ://用户出牌
					{
						//超时判断
						if (nElapse==0 || (m_bStustee==true)&&(nElapse<(TIME_OUT_CARD)))
						{
							if ((IsLookonMode()==false)&&(wChairID==GetMeChairID()))
							{
								AutomatismOutCard();
							}
							return false;
						}

						//播放声音
						if (m_bHandCardCount<m_bTurnCardCount)
						{
							return true;
						}
						if ((nElapse<=10)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false))
						{
							PlayGameSound("GAME_WARN");
						}

						return true;


					};
				case IDI_EXIT_GAME ://离开游戏
					{
						if (nElapse==0)
						{
							if ((IsLookonMode()==false)&&(wChairID==GetMeChairID()))
							{
								OnExit(0,0);
							}
							return false;
						}
						if ((nElapse<=10)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false))
						{
							PlayGameSound("GAME_WARN");
						}

						return true;

					};
				case IDI_LAND_SCORE ://挖坑叫分
					{
						if (nElapse==0 || m_bStustee == true)
						{
							if ((IsLookonMode()==false)&&(wChairID==GetMeChairID()))
							{
								OnLandScore(255,255);
							}
							return false;
						}
						if ((nElapse<=10)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false))
						{
							PlayGameSound("GAME_WARN");
						}

						return true;

				}
			};

			return false;
		}
		//网络消息
		override protected function OnGameMessage(wSubCmdID:uint, pBuffer:ByteArray, wDataSize:uint):Boolean
		{
			switch (wSubCmdID)
			{
				case SUB_S_SEND_CARD ://发送扑克
					{
						return OnSubSendCard(pBuffer,wDataSize);

					};
				case SUB_S_LAND_SCORE ://用户叫分
					{
						return OnSubLandScore(pBuffer,wDataSize);

					};
				case SUB_S_GAME_START ://游戏开始
					{
						return OnSubGameStart(pBuffer,wDataSize);

					};
				case SUB_S_OUT_CARD ://用户出牌
					{
						return OnSubOutCard(pBuffer,wDataSize);

					};
				case SUB_S_PASS_CARD ://放弃出牌
					{
						return OnSubPassCard(pBuffer,wDataSize);

					};
				case SUB_S_GAME_END ://游戏结束
					{
						return OnSubGameEnd(pBuffer,wDataSize);

				}
			};

			return false;

		}
		
		override protected function OnGameSceneMessage(cbGameStation:uint,  bLookonOther:Boolean, pBuffer:ByteArray,  wDataSize:uint):Boolean
		{
			switch (cbGameStation)
			{
				case GS_WK_FREE ://空闲状态
					{
						//效验数据
						if (wDataSize!=CMD_S_StatusFree.sizeof_CMD_S_StatusFree)
						{
							return false;
						}
						var pStatusFree:CMD_S_StatusFree=CMD_S_StatusFree.readData(pBuffer);

						//设置界面
						GetPeerGameView().SetBaseScore(pStatusFree.fBaseScore);

						//设置控件
						if (IsLookonMode()==false)
						{
							GetPeerGameView().m_btStart.ShowWindow(true);
							GetPeerGameView().m_btStart.SetFocus();
						}

						//设置扑克
						if (IsLookonMode()==false)
						{
							GetPeerGameView().m_HandCardControl.SetDisplayFlag(true);
						}

						return true;

					};
				case GS_WK_SCORE ://叫分状态
					{
						//效验数据
						if (wDataSize!=CMD_S_StatusScore.sizeof_CMD_S_StatusScore)
						{
							return false;
						}
						var pStatusScore:CMD_S_StatusScore=CMD_S_StatusScore.readData(pBuffer);

						//设置变量
						m_bHandCardCount=17;
						for (var i:uint=0; i<GAME_PLAYER; i++)
						{
							m_bCardCount[i]=17;
						}
						CopyArray(m_bHandCardData,pStatusScore.bCardData,m_bHandCardCount);

						//设置界面
						for (i=0; i<GAME_PLAYER; i++)
						{
							var wViewChairID:uint=SwitchViewChairID(i);
							GetPeerGameView().SetCardCount(wViewChairID,m_bCardCount[i]);
							GetPeerGameView().SetLandScore(wViewChairID,pStatusScore.bScoreInfo[i]);
						}
						GetPeerGameView().ShowLandTitle(true);
						GetPeerGameView().SetBaseScore(pStatusScore.fBaseScore);

						//按钮控制
						if ((IsLookonMode()==false)&&(pStatusScore.wCurrentUser==GetMeChairID()))
						{
							GetPeerGameView().m_btGiveUpScore.ShowWindow(true);
							GetPeerGameView().m_btOneScore.ShowWindow(pStatusScore.bLandScore<=0?true:false);
							GetPeerGameView().m_btTwoScore.ShowWindow(pStatusScore.bLandScore<=1?true:false);
							GetPeerGameView().m_btThreeScore.ShowWindow(pStatusScore.bLandScore<=2?true:false);
						}

						//设置扑克
						var bCardData:Array = GenEmptyCardData(20);
						UpdateLeaveCardControl(bCardData);
						GetPeerGameView().m_BackCardControl.SetCardData(bCardData,3);
						GetPeerGameView().m_HandCardControl.SetCardData(m_bHandCardData,m_bHandCardCount);
						if (IsLookonMode()==false)
						{
							GetPeerGameView().m_HandCardControl.SetDisplayFlag(true);
						}

						//设置时间
						SetGameTimer(pStatusScore.wCurrentUser,IDI_LAND_SCORE,30);

						return true;

					};
				case GS_WK_PLAYING ://游戏状态
					{
						//效验数据
						if (wDataSize!=CMD_S_StatusPlay.sizeof_CMD_S_StatusPlay)
						{
							return false;
						}
						var pStatusPlay:CMD_S_StatusPlay=CMD_S_StatusPlay.readData(pBuffer);

						//设置变量
						m_bTurnCardCount=pStatusPlay.bTurnCardCount;
						m_bHandCardCount=pStatusPlay.bCardCount[GetMeChairID()];
						m_bTurnOutType=m_GameLogic.GetCardType(pStatusPlay.bTurnCardData,m_bTurnCardCount);
						CopyArray(m_bHandCardData,pStatusPlay.bCardData,m_bHandCardCount);
						CopyArray(m_bTurnCardData,pStatusPlay.bTurnCardData,pStatusPlay.bTurnCardCount);
						
						
						//设置界面
						for (var i:uint=0; i<GAME_PLAYER; i++)
						{
							var wViewChairID:uint=SwitchViewChairID(i);
							m_bCardCount[i]=pStatusPlay.bCardCount[i];
							GetPeerGameView().SetCardCount(wViewChairID,pStatusPlay.bCardCount[i]);
						}
						GetPeerGameView().SetBombTime(pStatusPlay.wBombTime);
						GetPeerGameView().SetBaseScore(pStatusPlay.fBaseScore);
						GetPeerGameView().m_BackCardControl.SetCardData(pStatusPlay.bBackCard,3);
						GetPeerGameView().m_HandCardControl.SetCardData(m_bHandCardData,m_bHandCardCount);
						GetPeerGameView().SetLandUser(SwitchViewChairID(pStatusPlay.wLandUser),pStatusPlay.bLandScore);
						var bCardData:Array = GenEmptyCardData(20);
						UpdateLeaveCardControl(bCardData);
						
						//玩家设置
						if ((IsLookonMode()==false)&&(pStatusPlay.wCurrentUser==GetMeChairID()))
						{
							GetPeerGameView().m_btOutCard.EnableWindow(false);
							GetPeerGameView().m_btOutCard.ShowWindow(true);
							GetPeerGameView().m_btPassCard.ShowWindow(true);
							GetPeerGameView().m_btAutoOutCard.ShowWindow(true);
							GetPeerGameView().m_btPassCard.EnableWindow((m_bTurnCardCount!=0)?true:false);
							GetPeerGameView().m_btAutoOutCard.EnableWindow((m_bTurnCardCount!=0)?true:false);
						}

						//桌面设置
						if (m_bTurnCardCount!=0)
						{
							var wViewChairID:uint=SwitchViewChairID(pStatusPlay.wLastOutUser);
							GetPeerGameView().m_UserCardControl[wViewChairID].SetCardData(m_bTurnCardData,m_bTurnCardCount);
						}

						//设置定时器
						SetGameTimer(pStatusPlay.wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);

						//设置扑克
						if (IsLookonMode()==false)
						{
							GetPeerGameView().m_HandCardControl.SetPositively(true);
							GetPeerGameView().m_HandCardControl.SetDisplayFlag(true);
						}

						return true;

				}
			};

			return false;
		}

		
		/////////////////////////////////////////////////////////////
		//消息处理

		//发送扑克
		protected function OnSubSendCard(pBuffer:ByteArray,  wDataSize:uint):Boolean
		{
			//效验数据

			if (wDataSize!=CMD_S_SendCard.sizeof_CMD_S_SendCard)
			{
				return false;
			}

			//变量定义
			var pSendCard:CMD_S_SendCard=CMD_S_SendCard.readData(pBuffer);

			//设置数据
			m_bHandCardCount=pSendCard.bCardData.length;
			CopyArray(m_bHandCardData,pSendCard.bCardData,pSendCard.bCardData.length);
			for (var i:uint=0; i<GAME_PLAYER; i++)
			{
				//fix me
				m_bCardCount[i]=pSendCard.bCardData.length;
			}

			//设置界面
			for (i=0; i<GAME_PLAYER; i++)
			{
				m_bPass[i][0] = false;
				GetPeerGameView().SetLandScore(i,0);
				GetPeerGameView().SetPassFlag(i,false);
				GetPeerGameView().SetCardCount(i,m_bCardCount[i]);
				GetPeerGameView().m_UserCardControl[i].SetCardData(null,0);
			}
			if (IsLookonMode()==true)
			{
				GetPeerGameView().SetLandUser(INVALID_CHAIR,1);
				GetPeerGameView().m_ScoreView.ShowWindow(false);
				GetPeerGameView().m_LeaveCardControl[0].SetCardData(null,0);
				GetPeerGameView().m_LeaveCardControl[1].SetCardData(null,0);
			}
			GetPeerGameView().ShowLandTitle(true);
			GetPeerGameView().SetBombTime(m_wBombTime);

			//设置扑克
			var bCardData:Array = GenEmptyCardData(20);
			UpdateLeaveCardControl(bCardData);
						
			GetPeerGameView().m_BackCardControl.SetCardData(bCardData,3);
			GetPeerGameView().m_HandCardControl.SetCardData(pSendCard.bCardData,17);
			
			if (IsLookonMode()==true)
			{
				GetPeerGameView().m_HandCardControl.SetDisplayFlag(false);
			}

			//当前玩家
			if ((IsLookonMode()==false)&&(pSendCard.wCurrentUser==GetMeChairID()))
			{
				ActiveGameFrame();
				GetPeerGameView().m_btOneScore.ShowWindow(true);
				GetPeerGameView().m_btTwoScore.ShowWindow(true);
				GetPeerGameView().m_btThreeScore.ShowWindow(true);
				GetPeerGameView().m_btGiveUpScore.ShowWindow(true);
			}

			//播放声音
			PlayGameSound("GAME_START");

			//设置时间
			SetGameTimer(pSendCard.wCurrentUser,IDI_LAND_SCORE,30);

			return true;
		}
		//用户叫分
		protected function OnSubLandScore(pBuffer:ByteArray,  wDataSize:uint):Boolean
		{
			//效验数据

			if (wDataSize!=CMD_S_LandScore.sizeof_CMD_S_LandScore)
			{
				return false;
			}

			//消息处理
			var pLandScore=CMD_S_LandScore.readData(pBuffer);

			//设置界面
			var wViewChairID:uint=SwitchViewChairID(pLandScore.wLandUser);
			GetPeerGameView().SetLandScore(wViewChairID,pLandScore.bLandScore);

			//玩家设置
			if ((IsLookonMode()==false)&&(pLandScore.wCurrentUser==GetMeChairID()))
			{
				ActiveGameFrame();
				GetPeerGameView().m_btGiveUpScore.ShowWindow(true);
				GetPeerGameView().m_btOneScore.ShowWindow(pLandScore.bCurrentScore<=0?true:false);
				GetPeerGameView().m_btTwoScore.ShowWindow(pLandScore.bCurrentScore<=1?true:false);
				GetPeerGameView().m_btThreeScore.ShowWindow(pLandScore.bCurrentScore<=2?true:false);
			}

			//播放声音
			PlayGameSound("OUT_CARD");

			//设置时间
			SetGameTimer(pLandScore.wCurrentUser,IDI_LAND_SCORE,30);

			return true;
		}
		//游戏开始
		protected function OnSubGameStart(pBuffer:ByteArray,  wDataSize:uint):Boolean
		{
			//效验数据

			if (wDataSize!=CMD_S_GameStart.sizeof_CMD_S_GameStart)
			{
				return false;
			}

			//消息处理
			var pGameStart:CMD_S_GameStart=CMD_S_GameStart.readData(pBuffer);

			//设置变量
			m_wBombTime=1;
			m_bTurnCardCount=0;
			m_bTurnOutType=CT_INVALID;
			m_wLandUser=pGameStart.wLandUser;
			m_bCardCount[pGameStart.wLandUser]=20;
			ZeroArray(m_bTurnCardData);

			//设置控件
			GetPeerGameView().ShowLandTitle(false);
			GetPeerGameView().m_BackCardControl.SetCardData(pGameStart.bBackCard,pGameStart.bBackCard.length);

			//设置界面
			GetPeerGameView().SetLandScore(INVALID_CHAIR,0);
			GetPeerGameView().SetCardCount(SwitchViewChairID(pGameStart.wLandUser),m_bCardCount[pGameStart.wLandUser]);

			//地主设置
			if (pGameStart.wLandUser==GetMeChairID())
			{
				var bCardCound:uint=m_bHandCardCount;
				m_bHandCardCount+=pGameStart.bBackCard.length;
				CopyArray(m_bHandCardData,
				pGameStart.bBackCard,
				pGameStart.bBackCard.length,
				bCardCound);
				m_GameLogic.SortCardList(m_bHandCardData,m_bHandCardCount);
				GetPeerGameView().m_HandCardControl.SetCardData(m_bHandCardData,m_bHandCardCount);
			}
			GetPeerGameView().SetLandUser(SwitchViewChairID(pGameStart.wLandUser),pGameStart.bLandScore);

			//玩家设置
			if (IsLookonMode()==false)
			{
				GetPeerGameView().m_HandCardControl.SetPositively(true);
			}

			//当前玩家
			if ((IsLookonMode()==false)&&(pGameStart.wCurrentUser==GetMeChairID()))
			{
				ActiveGameFrame();
				GetPeerGameView().m_btOutCard.EnableWindow(false);
				GetPeerGameView().m_btOutCard.ShowWindow(true);
				GetPeerGameView().m_btPassCard.EnableWindow(false);
				GetPeerGameView().m_btPassCard.ShowWindow(true);
				GetPeerGameView().m_btAutoOutCard.ShowWindow(true);
				GetPeerGameView().m_btAutoOutCard.EnableWindow(false);
			}

			//播放声音
			PlayGameSound("GAME_START");

			//设置时间
			SetGameTimer(pGameStart.wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);

			return true;
		}
		//用户出牌
		protected function OnSubOutCard(pBuffer:ByteArray,  wDataSize:uint):Boolean
		{
			//变量定义
			var pOutCard:CMD_S_OutCard=CMD_S_OutCard.readData(pBuffer);
			var wHeadSize:uint=CMD_S_OutCard.sizeof_Header_CMD_S_OutCard;

			//效验数据
			if (wDataSize<wHeadSize)
			{
				return false;
			}

			if (wDataSize!=(wHeadSize+pOutCard.bCardCount))
			{
				return false;
			}

			//删除定时器
			KillTimer(IDI_MOST_CARD);
			KillGameTimer(IDI_OUT_CARD);

			//界面设置
			var wOutViewChairID:uint=SwitchViewChairID(pOutCard.wOutCardUser);
			m_bCardCount[pOutCard.wOutCardUser]-=pOutCard.bCardCount;
			GetPeerGameView().SetCardCount(wOutViewChairID,m_bCardCount[pOutCard.wOutCardUser]);

			//出牌设置
			if ((IsLookonMode()==true)||(GetMeChairID()!=pOutCard.wOutCardUser))
			{
				GetPeerGameView().m_UserCardControl[wOutViewChairID].SetCardData(pOutCard.bCardData,pOutCard.bCardCount);
			}
			
			//清理桌面
			if (m_bTurnCardCount==0)
			{
				m_wFirstOutUser = pOutCard.wOutCardUser;
				for (var i:uint=0; i<GAME_PLAYER; i++)
				{
					if (i==pOutCard.wOutCardUser)
					{
						continue;
					}
					GetPeerGameView().m_UserCardControl[SwitchViewChairID(i)].SetCardData(null,0);
				}
				GetPeerGameView().SetPassFlag(INVALID_CHAIR,false);
			}
			//历史扑克
			if ((m_bTurnCardCount==0)&&
				(m_wFirstOutUser != INVALID_CHAIR && m_cbOutCardCount[m_wFirstOutUser][0]!=0))
			{
				//设置扑克
				for (var i:uint=0;i<GAME_PLAYER;i++) 
				{
					//保存扑克
					m_cbOutCardCount[i][1]=m_cbOutCardCount[i][0];
					m_bPass[i][1]=m_bPass[i][0];
					CopyArray(m_cbOutCardData[i][1],m_cbOutCardData[i][0],m_cbOutCardCount[i][0]);
		
					//清理扑克
					m_cbOutCardCount[i][0]=0;
					ZeroArray(m_cbOutCardData[i][0]);
					m_bPass[i][0] = false;
				}

			}
			//出牌记录
			m_cbOutCardCount[pOutCard.wOutCardUser][0]=pOutCard.bCardCount;
			CopyArray(m_cbOutCardData[pOutCard.wOutCardUser][0],pOutCard.bCardData,pOutCard.bCardCount);
			
			//当前界面
			SwitchToCurrentCard();
			
			//记录出牌
			m_bTurnCardCount=pOutCard.bCardCount;
			m_bTurnOutType=m_GameLogic.GetCardType(pOutCard.bCardData,pOutCard.bCardCount);
			CopyArray(m_bTurnCardData,pOutCard.bCardData,pOutCard.bCardCount);

			//炸弹判断
			if ((m_bTurnOutType==CT_BOMB_CARD)||(m_bTurnOutType==CT_MISSILE_CARD))
			{
				m_wBombTime*=2;
				GetPeerGameView().SetBombTime(m_wBombTime);
				GetPeerGameView().ShowBombMotion(wOutViewChairID);
				PlayGameSound("BOMP");
			}

			//自己扑克
			if ((IsLookonMode()==true)&&(pOutCard.wOutCardUser==GetMeChairID()))
			{
				//删除扑克 
				var bSourceCount:uint=m_bHandCardCount;
				m_bHandCardCount-=pOutCard.bCardCount;
				m_GameLogic.RemoveCard(pOutCard.bCardData,pOutCard.bCardCount,m_bHandCardData,bSourceCount);

				//设置界面
				GetPeerGameView().m_HandCardControl.SetCardData(m_bHandCardData,m_bHandCardCount);
			}
			//
			
			var bCardData:Array = GenEmptyCardData(20);
			UpdateLeaveCardControl(bCardData);
			//最大判断
			if (pOutCard.wCurrentUser==pOutCard.wOutCardUser)
			{
				//设置变量
				m_bTurnCardCount=0;
				m_bTurnOutType=CT_INVALID;
				m_wMostUser=pOutCard.wCurrentUser;
				ZeroArray(m_bTurnCardData);

				//设置界面
				for (var i:uint=0; i<GAME_PLAYER; i++)
				{
					if (i!=pOutCard.wOutCardUser)
					{
						m_bPass[i][0] = true;
						var wViewChairID:uint=SwitchViewChairID(i);
						GetPeerGameView().SetPassFlag(wViewChairID,true);
						GetPeerGameView().m_UserCardControl[wViewChairID].SetCardData(null,0);
					}
				}

				//播放声音
				PlayGameSound("MOST_CARD");

				//设置定时器
				SetTimer(IDI_MOST_CARD,3000);

				return true;
			}

			//播放声音
			if ((IsLookonMode()==true)||(GetMeChairID()!=pOutCard.wOutCardUser))
			{
				PlayGameSound("OUT_CARD");
			}

			//玩家设置
			if (pOutCard.wCurrentUser!=INVALID_CHAIR)
			{
				m_bPass[pOutCard.wCurrentUser][0] = false;
				var wViewChairID:uint=SwitchViewChairID(pOutCard.wCurrentUser);
				GetPeerGameView().SetPassFlag(wViewChairID,false);
				GetPeerGameView().m_UserCardControl[wViewChairID].SetCardData(null,0);
			}

			//玩家设置
			if ((IsLookonMode()==false)&&(pOutCard.wCurrentUser==GetMeChairID()))
			{
				ActiveGameFrame();
				GetPeerGameView().m_btPassCard.EnableWindow(true);
				GetPeerGameView().m_btOutCard.ShowWindow(true);
				GetPeerGameView().m_btPassCard.ShowWindow(true);
				GetPeerGameView().m_btAutoOutCard.EnableWindow(true);
				GetPeerGameView().m_btAutoOutCard.ShowWindow(true);
				GetPeerGameView().m_btOutCard.EnableWindow((VerdictOutCard()==true)?true:false);
			}

			//设置时间
			if (pOutCard.wCurrentUser!=INVALID_CHAIR)
			{
				var bCardCount:uint=m_bCardCount[pOutCard.wCurrentUser];
				SetGameTimer(pOutCard.wCurrentUser,IDI_OUT_CARD,(bCardCount<m_bTurnCardCount)?TIME_MOST_OUT_CARD:TIME_OUT_CARD);
			}

			return true;
		}
		//放弃出牌
		protected function OnSubPassCard(pBuffer:ByteArray,  wDataSize:uint):Boolean
		{
			//效验数据
			if (wDataSize!=CMD_S_PassCard.sizeof_CMD_S_PassCard)
			{
				return false;
			}
			var pPassCard:CMD_S_PassCard=CMD_S_PassCard.readData(pBuffer);

			//删除定时器
			KillGameTimer(IDI_OUT_CARD);

			//玩家设置
			if ((IsLookonMode()==true)||(pPassCard.wPassUser!=GetMeChairID()))
			{
				m_bPass[pPassCard.wPassUser][0] = true;
				var wViewChairID:uint=SwitchViewChairID(pPassCard.wPassUser);
				GetPeerGameView().SetPassFlag(wViewChairID,true);
				GetPeerGameView().m_UserCardControl[wViewChairID].SetCardData(null,0);
			}

			//一轮判断
			if (pPassCard.bNewTurn==true)
			{
				m_bTurnCardCount=0;
				m_bTurnOutType=CT_INVALID;
				ZeroArray(m_bTurnCardData);
			}
			m_bPass[pPassCard.wCurrentUser][0] = false;
			//设置界面
			var wViewChairID:uint=SwitchViewChairID(pPassCard.wCurrentUser);
			GetPeerGameView().SetPassFlag(wViewChairID,false);
			GetPeerGameView().m_UserCardControl[wViewChairID].SetCardData(null,0);

			//玩家设置
			if ((IsLookonMode()==false)&&(pPassCard.wCurrentUser==GetMeChairID()))
			{
				ActiveGameFrame();
				GetPeerGameView().m_btOutCard.ShowWindow(true);
				GetPeerGameView().m_btPassCard.ShowWindow(true);
				GetPeerGameView().m_btAutoOutCard.ShowWindow(true);
				GetPeerGameView().m_btPassCard.EnableWindow((m_bTurnCardCount>0)?true:false);
				GetPeerGameView().m_btOutCard.EnableWindow((VerdictOutCard()==true)?true:false);
				GetPeerGameView().m_btAutoOutCard.EnableWindow((m_bTurnCardCount>0)?true:false);
			}

			//播放声音
			if ((IsLookonMode()==true)||(pPassCard.wPassUser!=GetMeChairID()))
			{
				PlayGameSound("OUT_CARD");
			}

			//设置时间
			if (m_bTurnCardCount!=0)
			{
				var bCardCount:uint=m_bCardCount[pPassCard.wCurrentUser];
				SetGameTimer(pPassCard.wCurrentUser,IDI_OUT_CARD,(bCardCount<m_bTurnCardCount)?TIME_MOST_OUT_CARD:TIME_OUT_CARD);
			}
			else
			{
				SetGameTimer(pPassCard.wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);

			}
			return true;
		}
		//游戏结束
		protected function OnSubGameEnd(pBuffer:ByteArray,  wDataSize:uint):Boolean
		{
			//效验数据

			if (wDataSize!=CMD_S_GameEnd.sizeof_CMD_S_GameEnd)
			{
				return false;
			}

			//消息处理
			var pGameEnd:CMD_S_GameEnd=CMD_S_GameEnd.readData(pBuffer);

			//删除定时器
			KillTimer(IDI_MOST_CARD);
			KillGameTimer(IDI_OUT_CARD);
			KillGameTimer(IDI_LAND_SCORE);
			KillTimer(IDI_LAST_TURN_CARD);

			//隐藏控件
			GetPeerGameView().m_btOutCard.ShowWindow(false);
			GetPeerGameView().m_btPassCard.ShowWindow(false);
			GetPeerGameView().m_btOneScore.ShowWindow(false);
			GetPeerGameView().m_btTwoScore.ShowWindow(false);
			GetPeerGameView().m_btThreeScore.ShowWindow(false);
			GetPeerGameView().m_btGiveUpScore.ShowWindow(false);
			GetPeerGameView().m_btAutoOutCard.ShowWindow(false);

			//禁用控件
			GetPeerGameView().m_btOutCard.EnableWindow(false);
			GetPeerGameView().m_btPassCard.EnableWindow(false);
			
			SwitchToCurrentCard();

			//设置积分
			for (var i:int=0; i<GAME_PLAYER; i++)
			{
				var pUserData:tagUserInfoHead=GetUserInfo(i);
				GetPeerGameView().m_ScoreView.SetGameScore(i,pGameEnd.fGameScore[i]);
			}
			GetPeerGameView().m_ScoreView.SetMeChairID(GetMeChairID());
			GetPeerGameView().m_ScoreView.ShowWindow(true);

			//设置扑克
			var bCardPos:uint=0;
			for (var i:int=0; i<GAME_PLAYER; i++)
			{
				var wViewChairID:uint=SwitchViewChairID(i);
				if (wViewChairID==0)
				{
					GetPeerGameView().m_LeaveCardControl[0].SetCardData(CloneArray(pGameEnd.bCardData,bCardPos),pGameEnd.bCardCount[i]);
				}
				else if (wViewChairID==2)
				{
					GetPeerGameView().m_LeaveCardControl[1].SetCardData(CloneArray(pGameEnd.bCardData,bCardPos),pGameEnd.bCardCount[i]);
				}
				bCardPos+=pGameEnd.bCardCount[i];
				if (pGameEnd.bCardCount[i]!=0)
				{
					m_bPass[i][0] = false;
					GetPeerGameView().SetPassFlag(wViewChairID,false);
					GetPeerGameView().m_UserCardControl[wViewChairID].SetCardData(null,0);
				}
			}

			//显示扑克
			if (IsLookonMode()==true)
			{
				GetPeerGameView().m_HandCardControl.SetDisplayFlag(true);
			}

			//播放声音
			var wMeChairID:uint=GetMeChairID();
			var fMeScore:Number=pGameEnd.fGameScore[GetMeChairID()];
			if (fMeScore>0)
			{
				PlayGameSound("GAME_WIN");
			}
			else if (fMeScore<0)
			{
				PlayGameSound("GAME_LOST");
			}
			else
			{
				PlayGameSound("GAME_END");

			}//设置界面
			if (IsLookonMode()==false)
			{
				GetPeerGameView().m_btStart.ShowWindow(true);
				SetGameTimer(GetMeChairID(),IDI_EXIT_GAME,30);
			}
			GetPeerGameView().ShowLandTitle(false);

			return true;
		}
		//////////////////////////////////////
		//辅助函数
		//出牌判断
		protected function VerdictOutCard():Boolean
		{
			//状态判断
			if (GetPeerGameView().m_btOutCard.IsWindowVisible()==false)
			{
				return false;
			}

			//获取扑克
			var bCardData:Array = new Array(20);
			var bShootCount:uint=GetPeerGameView().m_HandCardControl.GetShootCard(bCardData,bCardData.length);

			//出牌判断
			if (bShootCount>0)
			{
				//分析类型
				var bCardType:uint=m_GameLogic.GetCardType(bCardData,bShootCount);

				//类型判断
				if (bCardType==CT_INVALID)
				{
					return false;
				}

				//跟牌判断
				if (m_bTurnCardCount==0)
				{
					return true;
				}
				return m_GameLogic.CompareCard(bCardData,m_bTurnCardData,bShootCount,m_bTurnCardCount);
			}

			return false;
		}
		//自动出牌
		protected function AutomatismOutCard():Boolean
		{
			//先出牌者
			if (m_bTurnCardCount==0)
			{
				//控制界面
				KillGameTimer(IDI_OUT_CARD);
				GetPeerGameView().m_btOutCard.ShowWindow(false);
				GetPeerGameView().m_btOutCard.EnableWindow(false);
				GetPeerGameView().m_btPassCard.ShowWindow(false);
				GetPeerGameView().m_btPassCard.EnableWindow(false);
				GetPeerGameView().m_btAutoOutCard.ShowWindow(false);
				GetPeerGameView().m_btAutoOutCard.EnableWindow(false);

				//发送数据
				var OutCard:CMD_C_OutCard = new CMD_C_OutCard;
				OutCard.bCardCount=1;
				OutCard.bCardData[0]=m_bHandCardData[m_bHandCardCount-1];
				var sendData:ByteArray = OutCard.toByteArray();
				SendGameData(SUB_C_OUT_CART,sendData,2);

				//预先处理
				PlayGameSound("OUT_CARD");
				GetPeerGameView().m_UserCardControl[1].SetCardData(OutCard.bCardData,OutCard.bCardCount);

				//预先删除
				var bSourceCount:uint=m_bHandCardCount;
				m_bHandCardCount-=OutCard.bCardCount;
				m_GameLogic.RemoveCard(OutCard.bCardData,OutCard.bCardCount,m_bHandCardData,bSourceCount);
				GetPeerGameView().m_HandCardControl.SetCardData(m_bHandCardData,m_bHandCardCount);
			}
			else
			{
				OnPassCard(0,0);

			}
			return true;
		}
		//自动出牌
		protected function AutoOutCard(WhichOnsKindCard:uint):void
		{
			var i:int=0;
			var j:int=0;
			var n:int=0;
			var s:int=0;
			var bWhichKindSel:uint=0;
			var	m_bTempSCardCount:uint=0;				//扑克数目
			var	m_bTempSCardData:Array = new Array(20);				//手上扑克
			var m_bTempDCardCount=0;				//扑克数目
			var	m_bTempDCardData:Array = new Array(20);				//手上扑克
			var	m_bTempTCardCount:uint=0;				//扑克数目
			var	m_bTempTCardData:Array = new Array(20);				//手上扑克
			var	m_bTempFCardCount:uint=0;				//扑克数目
			var	m_bTempFCardData:Array = new Array(20);				//手上扑克
			var	m_bTempGetCardCount:uint=0;				//扑克数目
			var	m_bTempGetCardData:Array = new Array(20);				//手上扑克
			var	m_bTempGetCardIndex:Array = new Array(20);			//手上扑克
			var m_TempCard:uint=0;
			//如果没有人出牌，不提示
			if(m_bTurnCardCount==0)
				return;
			GetPeerGameView().m_HandCardControl.ShootAllCard(false);
			//获取单牌列表
			for(i=0;i<m_bHandCardCount;i++)
			{	
				var m_GetCard:uint=m_GameLogic.GetCardLogicValue(m_bHandCardData[i]);
				if(m_TempCard!=m_GetCard)
				{
					m_bTempSCardData[m_bTempSCardCount++]=m_bHandCardData[i];
					m_TempCard=m_GetCard;
				}
			}
			//获取对牌列表
			m_TempCard=0;
			for(i=0;i<m_bHandCardCount-1;i++)
			{	
				var m_GetCard1:uint=m_GameLogic.GetCardLogicValue(m_bHandCardData[i]);
				var m_GetCard2:uint=m_GameLogic.GetCardLogicValue(m_bHandCardData[i+1]);
				if(m_TempCard!=m_GetCard1&&m_GetCard1==m_GetCard2&&m_GetCard1<16)
				{
					m_bTempDCardData[m_bTempDCardCount++]=m_bHandCardData[i];
					m_bTempDCardData[m_bTempDCardCount++]=m_bHandCardData[i+1];
					m_TempCard=m_GetCard1;
				}
			}
			//获取三张牌列表
			m_TempCard=0;
			for(i=0;i<m_bHandCardCount-2;i++)
			{	
				var m_GetCard1:uint=m_GameLogic.GetCardLogicValue(m_bHandCardData[i]);
				var m_GetCard2:uint=m_GameLogic.GetCardLogicValue(m_bHandCardData[i+1]);
				var m_GetCard3:uint=m_GameLogic.GetCardLogicValue(m_bHandCardData[i+2]);
				if(m_TempCard!=m_GetCard1&&m_GetCard1==m_GetCard2&&m_GetCard1==m_GetCard3)
				{
					m_bTempTCardData[m_bTempTCardCount++]=m_bHandCardData[i];
					m_bTempTCardData[m_bTempTCardCount++]=m_bHandCardData[i+1];
					m_bTempTCardData[m_bTempTCardCount++]=m_bHandCardData[i+2];
					m_TempCard=m_GetCard1;
				}
			}
			//获取四张牌列表
			m_TempCard=0;
			for(i=0;i<m_bHandCardCount-3;i++)
			{	
				var m_GetCard1:uint=m_GameLogic.GetCardLogicValue(m_bHandCardData[i]);
				var m_GetCard2:uint=m_GameLogic.GetCardLogicValue(m_bHandCardData[i+1]);
				var m_GetCard3:uint=m_GameLogic.GetCardLogicValue(m_bHandCardData[i+2]);
				var m_GetCard4:uint=m_GameLogic.GetCardLogicValue(m_bHandCardData[i+3]);
				if(m_TempCard!=m_GetCard1&&m_GetCard1==m_GetCard2&&m_GetCard1==m_GetCard3&&m_GetCard1==m_GetCard4)
				{
					m_bTempFCardData[m_bTempFCardCount++]=m_bHandCardData[i];
					m_bTempFCardData[m_bTempFCardCount++]=m_bHandCardData[i+1];
					m_bTempFCardData[m_bTempFCardCount++]=m_bHandCardData[i+2];
					m_bTempFCardData[m_bTempFCardCount++]=m_bHandCardData[i+3];
					m_TempCard=m_GetCard1;
				}
			}
			//根据所出牌类型判断
			i=0;
			switch(m_bTurnOutType)
			{
				case CT_SINGLE:
				case CT_ONE_LINE:
				{
					if(WhichOnsKindCard==1)   //判断是不是具有唯一性
					{
						for(i=m_bTempSCardCount;i>0;i--)
						{
							if(i-m_bTurnCardCount>=0&&m_GameLogic.CompareCard(CloneArray(m_bTempSCardData,i-m_bTurnCardCount),m_bTurnCardData,m_bTurnCardCount,m_bTurnCardCount))
							{
								if((bWhichKindSel++)>1)
									i=0;
							}
						}
					}
					for(i=m_bTempSCardCount;i>0;i--)
					{
						if(i-m_bTurnCardCount>=0&&m_GameLogic.CompareCard(CloneArray(m_bTempSCardData,i-m_bTurnCardCount),m_bTurnCardData,m_bTurnCardCount,m_bTurnCardCount))
						{
							//判断是不是最合理的
							var m_bIsHaveCard:Boolean=false;
							for(j=0;j<m_bTempDCardCount;j++)
							{
								for(n=0;n<m_bTurnCardCount;n++)
								{
									if(m_GameLogic.GetCardLogicValue(m_bTempSCardData[i-m_bTurnCardCount+n])==m_GameLogic.GetCardLogicValue(m_bTempDCardData[j]))
										m_bIsHaveCard=true;
								}
							}
							//把最合理的情况保存起来
							if(m_bTempGetCardCount==0||!m_bIsHaveCard)
							{
								CopyArray(m_bTempGetCardData,CloneArray(m_bTempSCardData,i-m_bTurnCardCount),m_bTurnCardCount);
								m_bTempGetCardCount=m_bTurnCardCount;
							}
							if(!m_bIsHaveCard)
								break;
						}
					}
				break;
			 }
			 case CT_DOUBLE:
			 case CT_DOUBLE_LINE:
			 {
				if(WhichOnsKindCard==1)     //判断是不是具有唯一性
				{
					for(i=m_bTempDCardCount;i>0;i--)
					{
						if(i-m_bTurnCardCount>=0&&m_GameLogic.CompareCard(CloneArray(m_bTempDCardData,i-m_bTurnCardCount),m_bTurnCardData,m_bTurnCardCount,m_bTurnCardCount))
						{
							if((bWhichKindSel++)>1)
								i=0;
						}
					}
				}
				for(i=m_bTempDCardCount;i>0;i--)
				{
					if(i-m_bTurnCardCount>=0&&m_GameLogic.CompareCard(CloneArray(m_bTempDCardData,i-m_bTurnCardCount),m_bTurnCardData,m_bTurnCardCount,m_bTurnCardCount))
					{
						//判断是不是最合理的
						var m_bIsHaveCard:Boolean=false;
						for(j=0;j<m_bTempTCardCount;j++)
						{
							for(n=0;n<m_bTurnCardCount;n++)
							{
								if(m_GameLogic.GetCardLogicValue(m_bTempDCardData[i-m_bTurnCardCount+n])==m_GameLogic.GetCardLogicValue(m_bTempTCardData[j]))
									m_bIsHaveCard=true;
							}
						}
						if(m_bTempGetCardCount==0||!m_bIsHaveCard)
						{
							CopyArray(m_bTempGetCardData,CloneArray(m_bTempDCardData,i-m_bTurnCardCount),m_bTurnCardCount);
							m_bTempGetCardCount=m_bTurnCardCount;
						}
						if(!m_bIsHaveCard)
							break;
					}
				}
				break;
			 }
			case CT_THREE:
			case CT_THREE_LINE:
			{
				if(WhichOnsKindCard==1)           //判断是不是具有唯一性
				{
					for(i=m_bTempTCardCount;i>0;i--)
					{
						if(i-m_bTurnCardCount>=0&&m_GameLogic.CompareCard(CloneArray(m_bTempTCardData,i-m_bTurnCardCount),m_bTurnCardData,m_bTurnCardCount,m_bTurnCardCount))
						{
							if((bWhichKindSel++)>1)
								i=0;
						}
					}
				}
				for(i=m_bTempTCardCount;i>0;i--)
				{
					if(i-m_bTurnCardCount>=0&&m_GameLogic.CompareCard(CloneArray(m_bTempTCardData,i-m_bTurnCardCount),m_bTurnCardData,m_bTurnCardCount,m_bTurnCardCount))
					{
						//判断是不是最合理的
						var m_bIsHaveCard:Boolean=false;
						for(j=0;j<m_bTempFCardCount;j++)
						{
							for(n=0;n<m_bTurnCardCount;n++)
							{
								if(m_GameLogic.GetCardLogicValue(m_bTempTCardData[i-m_bTurnCardCount+n])==m_GameLogic.GetCardLogicValue(m_bTempFCardData[j]))
									m_bIsHaveCard=true;
							}
						}
						if(m_bTempGetCardCount==0||!m_bIsHaveCard)
						{
							CopyArray(m_bTempGetCardData,CloneArray(m_bTempTCardData,i-m_bTurnCardCount),m_bTurnCardCount);
							m_bTempGetCardCount=m_bTurnCardCount;
						}
						if(!m_bIsHaveCard&&m_bTempGetCardCount!=0)
						break;
					}
				}
				break;
			 }
		 case CT_THREE_LINE_TAKE_ONE:
		 case CT_THREE_LINE_TAKE_DOUBLE:
		{
				//分析扑克
				var AnalyseResult:tagAnalyseResult =new tagAnalyseResult;
				m_GameLogic.AnalysebCardData(m_bTurnCardData,m_bTurnCardCount,AnalyseResult);      
				if(WhichOnsKindCard==1)               //判断是不是具有唯一性
				{
					for(i=m_bTempTCardCount;i>0;i--)
					{
						if(i-AnalyseResult.bThreeCount*3>=0&&m_GameLogic.CompareCard(CloneArray(m_bTempTCardData,i-AnalyseResult.bThreeCount*3),m_bTurnCardData,AnalyseResult.bThreeCount*3,AnalyseResult.bThreeCount*3))
						{
							if((bWhichKindSel++)>1)
								i=0;
						}
					}
				}
				for(i=m_bTempTCardCount;i>0;i--)
				{
					if(i-AnalyseResult.bThreeCount*3>=0&&m_GameLogic.CompareCard(CloneArray(m_bTempTCardData,i-AnalyseResult.bThreeCount*3),AnalyseResult.m_bTCardData,AnalyseResult.bThreeCount*3,AnalyseResult.bThreeCount*3))
					{
						//判断是不是最合理的
						var m_bIsHaveCard:Boolean=false;
						for(j=0;j<m_bTempFCardCount;j++)
						{
							for(n=0;n<AnalyseResult.bThreeCount*3;n++)
							{
								if(m_GameLogic.GetCardLogicValue(m_bTempTCardData[i-AnalyseResult.bThreeCount*3+n])==m_GameLogic.GetCardLogicValue(m_bTempFCardData[j]))
									m_bIsHaveCard=true;
							}
						}
						if(m_bTempGetCardCount==0||!m_bIsHaveCard)
						{
							CopyArray(m_bTempGetCardData,CloneArray(m_bTempTCardData,i-AnalyseResult.bThreeCount*3),AnalyseResult.bThreeCount*3);
							m_bTempGetCardCount=AnalyseResult.bThreeCount*3;
						}
						if(!m_bIsHaveCard&&m_bTempGetCardCount!=0)
							i=0;
					}
				}
				if(m_bTempGetCardCount>0)
				{
					var m_bIsHaveSame:Boolean;
					for(var m:int=0;m<AnalyseResult.bDoubleCount;m++)
					{
						for(j=0;j<m_bTempDCardCount/2;j++)
						{
							//判断是不是最合理的
							m_bIsHaveSame=false;
							for(n=0;n<m_bTempGetCardCount;n++)
							{
								if(m_GameLogic.GetCardLogicValue(m_bTempDCardData[m_bTempDCardCount-j*2-1])==m_GameLogic.GetCardLogicValue(m_bTempGetCardData[n]))
								{
									m_bIsHaveSame=true;
								}
							}
							if(!m_bIsHaveSame)
							{
								var m_bIsHaveCard:Boolean=false;
								for(s=0;s<m_bTempTCardCount;s++)
								{
									for(n=0;n<m_bTempGetCardCount;n++)
									{
										if(m_GameLogic.GetCardLogicValue(m_bTempDCardData[m_bTempDCardCount-j*2-1])==m_GameLogic.GetCardLogicValue(m_bTempTCardData[s]))
											m_bIsHaveCard=true;
									}
								}
								if(m_bTempGetCardCount==AnalyseResult.bThreeCount*3||!m_bIsHaveCard)
								{
									m_bTempGetCardData[AnalyseResult.bThreeCount*3+m*2]=m_bTempDCardData[m_bTempDCardCount-j*2-1];
									m_bTempGetCardData[AnalyseResult.bThreeCount*3+m*2+1]=m_bTempDCardData[m_bTempDCardCount-j*2-2];
									m_bTempGetCardCount=AnalyseResult.bThreeCount*3+(m+1)*2;
								}
								if(!m_bIsHaveCard)
								{
									n=m_bTempGetCardCount;
									j=m_bTempDCardCount/2;
								}
							}
						}
					}
					for(var m:int=0;m<AnalyseResult.bSignedCount;m++)
					{
						for(j=0;j<m_bTempSCardCount;j++)
						{
							//判断是不是最合理的
							m_bIsHaveSame=false;
							for(n=0;n<m_bTempGetCardCount;n++)
							{
								if(m_GameLogic.GetCardLogicValue(m_bTempSCardData[m_bTempSCardCount-j-1])==m_GameLogic.GetCardLogicValue(m_bTempGetCardData[n]))
								{
									m_bIsHaveSame=true;
								}
							}
							if(!m_bIsHaveSame)
							{
								var m_bIsHaveCard:Boolean=false;
								for(s=0;s<m_bTempDCardCount;s++)
								{
									for(n=0;n<m_bTempGetCardCount;n++)
									{
										if(m_GameLogic.GetCardLogicValue(m_bTempSCardData[m_bTempSCardCount-j-1])==m_GameLogic.GetCardLogicValue(m_bTempDCardData[s]))
											m_bIsHaveCard=true;
									}
								}	
								if(m_bTempGetCardCount==AnalyseResult.bThreeCount*3||!m_bIsHaveCard)
								{
									m_bTempGetCardData[AnalyseResult.bThreeCount*3+m]=m_bTempSCardData[m_bTempSCardCount-j-1];
									m_bTempGetCardCount=AnalyseResult.bThreeCount*3+m+1;
								}
								if(!m_bIsHaveCard)
								{
									n=m_bTempGetCardCount;
									j=m_bTempSCardCount;
								}
							}
						}
					}
				}
				break;
		  }
		 case CT_FOUR_LINE_TAKE_ONE:
		 case CT_FOUR_LINE_TAKE_DOUBLE:
		 {
				//分析扑克
				var AnalyseResult:tagAnalyseResult = new tagAnalyseResult;
				m_GameLogic.AnalysebCardData(m_bTurnCardData,m_bTurnCardCount,AnalyseResult);
				if(WhichOnsKindCard==1)       //判断是不是具有唯一性
				{
				for(i=m_bTempFCardCount;i>0;i--)
				{
					if(i-AnalyseResult.bFourCount*4>=0&&m_GameLogic.CompareCard(CloneArray(m_bTempFCardData,i-AnalyseResult.bFourCount*4),m_bTurnCardData,AnalyseResult.bFourCount*4,AnalyseResult.bFourCount*4))
					{
						if((bWhichKindSel++)>1)
							i=0;
					}
				}
			}
				for(i=m_bTempFCardCount;i>0;i--)
				{
					if(i-AnalyseResult.bFourCount*4>=0&&m_GameLogic.CompareCard(CloneArray(m_bTempFCardData,i-AnalyseResult.bFourCount*4),m_bTurnCardData,AnalyseResult.bFourCount*4,AnalyseResult.bFourCount*4))
					{
						CopyArray(m_bTempGetCardData,CloneArray(m_bTempFCardData,i-AnalyseResult.bFourCount*4),AnalyseResult.bFourCount*4);
						m_bTempGetCardCount=AnalyseResult.bFourCount*4;
						i=0;
					}
				}
				if(m_bTempGetCardCount>0)
				{
				var m_bIsHaveSame:Boolean;
				for(m=0;m<AnalyseResult.bDoubleCount;m++)
				{
					for(j=0;j<m_bTempDCardCount/2;j++)
					{
						//判断是不是最合理的
						m_bIsHaveSame=false;
						for(n=0;n<m_bTempGetCardCount;n++)
						{
							if(m_GameLogic.GetCardLogicValue(m_bTempDCardData[m_bTempDCardCount-j*2-1])==m_GameLogic.GetCardLogicValue(m_bTempGetCardData[n]))
							{
								m_bIsHaveSame=true;
							}
						}
						if(!m_bIsHaveSame)
						{
							var m_bIsHaveCard:Boolean=false;
							for(s=0;s<m_bTempTCardCount;s++)
							{
								for(n=0;n<m_bTempGetCardCount;n++)
								{
									if(m_GameLogic.GetCardLogicValue(m_bTempDCardData[m_bTempDCardCount-j*2-1])==m_GameLogic.GetCardLogicValue(m_bTempTCardData[s]))
										m_bIsHaveCard=true;
								}
							}
							if(m_bTempGetCardCount==AnalyseResult.bFourCount*4||!m_bIsHaveCard)
							{
								m_bTempGetCardData[AnalyseResult.bFourCount*4+m*2]=m_bTempDCardData[m_bTempDCardCount-j*2-1];
								m_bTempGetCardData[AnalyseResult.bFourCount*4+m*2+1]=m_bTempDCardData[m_bTempDCardCount-j*2-2];
								m_bTempGetCardCount=AnalyseResult.bFourCount*4+(m+1)*2;
							}
							if(!m_bIsHaveCard)
							{
								n=m_bTempGetCardCount;
								j=m_bTempDCardCount/2;
							}
						}
					}
				}
				for(var m:int=0;m<AnalyseResult.bSignedCount;m++)
				{
					for(j=0;j<m_bTempSCardCount;j++)
					{
						//判断是不是最合理的
						m_bIsHaveSame=false;
						for(n=0;n<m_bTempGetCardCount;n++)
						{
							if(m_GameLogic.GetCardLogicValue(m_bTempSCardData[m_bTempSCardCount-j-1])==m_GameLogic.GetCardLogicValue(m_bTempGetCardData[n]))
							{
								m_bIsHaveSame=true;
							}
						}
						if(!m_bIsHaveSame)
						{
							var m_bIsHaveCard:Boolean=false;
							for(s=0;s<m_bTempDCardCount;s++)
							{
								for(n=0;n<m_bTempGetCardCount;n++)
								{
									if(m_GameLogic.GetCardLogicValue(m_bTempSCardData[m_bTempSCardCount-j-1])==m_GameLogic.GetCardLogicValue(m_bTempDCardData[j]))
										m_bIsHaveCard=true;
								}
							}
							if(m_bTempGetCardCount==AnalyseResult.bFourCount*4||!m_bIsHaveCard)
							{
								m_bTempGetCardData[AnalyseResult.bFourCount*4+m]=m_bTempSCardData[m_bTempSCardCount-j-1];
								m_bTempGetCardCount=AnalyseResult.bFourCount*4+m+1;
							}
							if(!m_bIsHaveCard)
							{
								n=m_bTempGetCardCount;
								j=m_bTempSCardCount;
							}
							}
							}
						}
					}
					break;
		 		}
		
			}
			if(m_bTempGetCardCount==0)
			{
				bWhichKindSel=0;
				//判断炸弹的可能性
				if(m_bTempFCardCount>3)
				{
					for(i=m_bTempFCardCount-4;i>=0;i--)
					{
						if(m_GameLogic.CompareCard(CloneArray(m_bTempFCardData,i),m_bTurnCardData,4,m_bTurnCardCount))
						{
							if((bWhichKindSel++)==0)
							{
								CopyArray(m_bTempGetCardData,CloneArray(m_bTempFCardData,i),4);
								m_bTempGetCardCount=4;
							}
						}
					}
				}
				if(m_bTempGetCardCount==0)
				{
					if(m_bHandCardCount>1)
					{
						if(m_GameLogic.GetCardLogicValue(m_bHandCardData[0])>15&&m_GameLogic.GetCardLogicValue(m_bHandCardData[1])>15)
						{
							CopyArray(m_bTempGetCardData,m_bHandCardData,2);
							m_bTempGetCardCount=2;
							if(WhichOnsKindCard==1)
								bWhichKindSel=1;
						}
					}
				}
			}
			var m_GetIndex:uint=0;
			if(m_bTempGetCardCount==0)
			{
				if(WhichOnsKindCard!=1)
					OnPassCard(0,0);
			}
			else
			{
				for(j=0;j<m_bTempGetCardCount;j++)
				{
					for(i=0;i<m_bHandCardCount;i++)
					{
						if(m_bHandCardData[i]==m_bTempGetCardData[j])
						{
							m_bTempGetCardIndex[m_GetIndex++]=i;
						}
					}
				}
		
			}
			if(m_GameLogic.CompareCard(m_bTempGetCardData,m_bTurnCardData,m_bTempGetCardCount,m_bTurnCardCount))
			{
				if(WhichOnsKindCard==1&&bWhichKindSel==1||WhichOnsKindCard!=1)
				{
					GetPeerGameView().m_HandCardControl.SetShootCard(m_bTempGetCardIndex,m_GetIndex);
					GetPeerGameView().m_btOutCard.EnableWindow(true);
				}
			}
			else
			{
				if(WhichOnsKindCard!=1)
					OnPassCard(0,0);
			}
		}
		
		/////////////////////////////////////////////
		protected function OnGameViewEevent(e:GameViewEvent):void
		{
			switch(e.m_nMsg)
			{
				case GameViewEvent.nGVM_START:
				{
					OnStart(e.m_nWParam,e.m_nLParam);
					return;
				}
				case GameViewEvent.nGVM_OUT_CARD:
				{
					OnOutCard(e.m_nWParam,e.m_nLParam);
					return;
				}
				case GameViewEvent.nGVM_PASS_CARD:
				{
					OnPassCard(e.m_nWParam,e.m_nLParam);
					return;
				}
				case GameViewEvent.nGVM_LAND_SCORE:
				{
					OnLandScore(e.m_nWParam,e.m_nLParam);
					return;
				}
				case GameViewEvent.nGVM_AUTO_OUTCARD:
				{
					OnAutoOutCard(e.m_nWParam,e.m_nLParam);
					return;
				}
				case GameViewEvent.nGVM_LEFT_HIT_CARD:
				{
					OnLeftHitCard(e.m_nWParam,e.m_nLParam);
					return;
				}
				case GameViewEvent.nGVM_RIGHT_HIT_CARD:
				{
					OnRightHitCard(e.m_nWParam,e.m_nLParam);
					return;
				}
				case GameViewEvent.nGVM_TRUSTEE_CONTROL:
					{
						OnStusteeControl(e.m_nWParam,e.m_nLParam);
						return;


				}
				case GameViewEvent.nGVM_LAST_TURN_CARD:
					{
						OnLastTurnCard(e.m_nWParam,e.m_nLParam);
						return;


				}
			}
		}
		//开始按钮
		override protected function OnStart(wParam:uint, lParam:uint):uint
		{
			//设置变量
			ZeroThreeDimensionArray(m_cbOutCardData);
			ZeroTwoDimensionArray(m_cbOutCardCount);
			ZeroTwoDimensionArray(m_bPass,false);
			
			m_wBombTime=1;
			m_wTimeOutCount=0;
			m_bHandCardCount=0;
			m_bTurnCardCount=0;
			m_bTurnOutType=CT_INVALID;
			m_wMostUser=INVALID_CHAIR;
			ZeroArray(m_bHandCardData);
			ZeroArray(m_bTurnCardData);

			//设置界面
			KillGameTimer(IDI_EXIT_GAME);
			GetPeerGameView().SetBaseScore(0);
			GetPeerGameView().ShowLandTitle(false);
			GetPeerGameView().SetBombTime(m_wBombTime);
			GetPeerGameView().SetCardCount(INVALID_CHAIR,0);
			GetPeerGameView().SetLandUser(INVALID_CHAIR,1);
			GetPeerGameView().SetLandScore(INVALID_CHAIR,0);
			GetPeerGameView().SetPassFlag(INVALID_CHAIR,false);

			//隐藏控件
			GetPeerGameView().m_btStart.ShowWindow(false);
			GetPeerGameView().m_ScoreView.ShowWindow(false);

			//设置扑克
			GetPeerGameView().m_BackCardControl.SetCardData(null,0);
			GetPeerGameView().m_HandCardControl.SetCardData(null,0);
			GetPeerGameView().m_HandCardControl.SetPositively(false);
			GetPeerGameView().m_LeaveCardControl[0].SetCardData(null,0);
			GetPeerGameView().m_LeaveCardControl[1].SetCardData(null,0);
			for (var i:int=0; i<GAME_PLAYER; i++)
			{
				GetPeerGameView().m_UserCardControl[i].SetCardData(null,0);
			}
			//取消托管
			if (m_bStustee==true)
			{
				m_bStustee=false;
				GetPeerGameView().m_btStusteeControl.visible = true;
				GetPeerGameView().m_btStopStusteeControl.visible = false;
			}
				
			//发送消息
			SendUserReady(null,0);

			return 0;
		}

		//出牌消息
		protected function OnOutCard(wParam:uint, lParam:uint):void
		{
			//状态判断
			if ((GetPeerGameView().m_btOutCard.IsWindowEnabled()==false)||
			(GetPeerGameView().m_btOutCard.IsWindowVisible()==false))
			{
				return ;
			}

			//设置界面
			KillGameTimer(IDI_OUT_CARD);
			GetPeerGameView().m_btOutCard.ShowWindow(false);
			GetPeerGameView().m_btOutCard.EnableWindow(false);
			GetPeerGameView().m_btPassCard.ShowWindow(false);
			GetPeerGameView().m_btPassCard.EnableWindow(false);
			GetPeerGameView().m_btAutoOutCard.ShowWindow(false);
			GetPeerGameView().m_btAutoOutCard.EnableWindow(false);
	
			//发送数据
			var OutCard:CMD_C_OutCard = new CMD_C_OutCard;
			OutCard.bCardCount=GetPeerGameView().m_HandCardControl.GetShootCard(OutCard.bCardData,OutCard.bCardData.length);
			var sendData:ByteArray = OutCard.toByteArray();
			SendGameData(SUB_C_OUT_CART,sendData,1 + OutCard.bCardCount);

			//预先显示
			PlayGameSound("OUT_CARD");
			GetPeerGameView().m_UserCardControl[1].SetCardData(OutCard.bCardData,OutCard.bCardCount);

			//预先删除
			var bSourceCount:uint=m_bHandCardCount;
			m_bHandCardCount-=OutCard.bCardCount;
			m_GameLogic.RemoveCard(OutCard.bCardData,OutCard.bCardCount,m_bHandCardData,bSourceCount);
			GetPeerGameView().m_HandCardControl.SetCardData(m_bHandCardData,m_bHandCardCount);

			
		}

		//放弃出牌
		protected function OnPassCard(wParam:uint, lParam:uint):void
		{
			//状态判断
			if (GetPeerGameView().m_btPassCard.IsWindowEnabled()==false)
			{
				return ;
			}

			//设置界面
			KillGameTimer(IDI_OUT_CARD);
			GetPeerGameView().m_btOutCard.ShowWindow(false);
			GetPeerGameView().m_btOutCard.EnableWindow(false);
			GetPeerGameView().m_btPassCard.ShowWindow(false);
			GetPeerGameView().m_btPassCard.EnableWindow(false);
			GetPeerGameView().m_btAutoOutCard.ShowWindow(false);
			GetPeerGameView().m_btAutoOutCard.EnableWindow(false);

			//发送数据
			SendGameData(SUB_C_PASS_CARD);

			//预先显示
			m_bPass[1][0] = true;
			GetPeerGameView().SetPassFlag(1,true);
			PlayGameSound("OUT_CARD");
			GetPeerGameView().m_HandCardControl.SetCardData(m_bHandCardData,m_bHandCardCount);
			var bCardData:Array = GenEmptyCardData(20);
			UpdateLeaveCardControl(bCardData);
			
		}

		//叫分消息
		protected function OnLandScore(wParam:uint, lParam:uint):void
		{
			//设置界面
			KillGameTimer(IDI_LAND_SCORE);
			GetPeerGameView().m_btOneScore.ShowWindow(false);
			GetPeerGameView().m_btTwoScore.ShowWindow(false);
			GetPeerGameView().m_btThreeScore.ShowWindow(false);
			GetPeerGameView().m_btGiveUpScore.ShowWindow(false);

			//发送数据
			var LandScore:CMD_C_LandScore = new CMD_C_LandScore;
			LandScore.bLandScore=wParam;
			var sendData:ByteArray =  LandScore.toByteArray();

			SendGameData(SUB_C_LAND_SCORE,sendData,CMD_C_LandScore.sizeof_CMD_C_LandScore);

			
		}

		//出牌提示
		protected function OnAutoOutCard(wParam:uint, lParam:uint):void
		{
			AutoOutCard(0);
		}
		//拖管控制
		protected function OnStusteeControl(wParam:uint, lParam:uint):uint
		{
			//设置状态
			if (m_bStustee==true)
			{
				m_bStustee=false;
				GetPeerGameView().m_btStusteeControl.visible = true;
				GetPeerGameView().m_btStopStusteeControl.visible = false;
			}
			else
			{
				m_bStustee=true;
				GetPeerGameView().m_btStusteeControl.visible = false;
				GetPeerGameView().m_btStopStusteeControl.visible = true;
			}
		
			return 0;
		}
		//上轮扑克
		protected function OnLastTurnCard(wParam:uint, lParam:uint):uint
		{
			if (m_bLastTurn==false)
			{
				//设置扑克
				for (var i:uint=0;i<GAME_PLAYER;i++)
				{
					var wViewChairID:uint=SwitchViewChairID(i);
					if(m_bPass[i][1] == true)
					{
						GetPeerGameView().SetPassFlag(wViewChairID,true);
						GetPeerGameView().m_UserCardControl[wViewChairID].SetCardData(null,0);
					}
					else
					{
						GetPeerGameView().SetPassFlag(wViewChairID,false);
						GetPeerGameView().m_UserCardControl[wViewChairID].SetCardData(m_cbOutCardData[i][1],m_cbOutCardCount[i][1]);
					}
				}
		
				//环境设置
				m_bLastTurn=true;
				GetPeerGameView().SetLastTurnCard(true);
				SetTimer(IDI_LAST_TURN_CARD,3000);
			}
			else SwitchToCurrentCard();
		
			return 0;
		}
		//右键扑克
		protected function OnLeftHitCard(wParam:uint, lParam:uint):void
		{
			//设置控件
			var bOutCard:Boolean=VerdictOutCard();
			GetPeerGameView().m_btOutCard.EnableWindow(bOutCard?true:false);
		}

		//左键扑克
		protected function OnRightHitCard(wParam:uint, lParam:uint):void
		{
			//用户出牌
			OnOutCard(0,0);
		}
		
		//定时器消息
		override protected function OnTimer(nIDEvent:int):Boolean
		{
			if ((nIDEvent==IDI_MOST_CARD)&&(m_wMostUser!=INVALID_CHAIR))
			{
				//变量定义
				var wCurrentUser:uint=m_wMostUser;
				var m_wMostUser:uint=INVALID_CHAIR;

				//删除定时器
				KillTimer(IDI_MOST_CARD);

				//设置界面
				GetPeerGameView().SetPassFlag(INVALID_CHAIR,false);
				for (var i:uint=0;i<GAME_PLAYER;i++)
				{
					GetPeerGameView().m_UserCardControl[i].SetCardData(null,0);
					m_bPass[i][0] = false;
				}

				//玩家设置
				if ((IsLookonMode()==false)&&(wCurrentUser==GetMeChairID()))
				{
					ActiveGameFrame();
					GetPeerGameView().m_btOutCard.ShowWindow(true);
					GetPeerGameView().m_btPassCard.ShowWindow(true);
					GetPeerGameView().m_btPassCard.EnableWindow(false);
					GetPeerGameView().m_btAutoOutCard.ShowWindow(true);
					GetPeerGameView().m_btAutoOutCard.EnableWindow(false);
					GetPeerGameView().m_btOutCard.EnableWindow((VerdictOutCard()==true)?true:false);
				}

				//设置时间
				SetGameTimer(wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);

				return true;
			}
			if(nIDEvent== IDI_LAST_TURN_CARD)	//上轮扑克
			{
				//还原界面
				SwitchToCurrentCard();
		
				return true;
			}

			return false;
		}
		//当前扑克
		protected function SwitchToCurrentCard():Boolean
		{
			//处理判断
			if (m_bLastTurn==false) return false;
		
			//环境设置
			m_bLastTurn=false;
			KillTimer(IDI_LAST_TURN_CARD);
			GetPeerGameView().SetLastTurnCard(false);
		
			//设置扑克
			for (var i:uint=0;i<GAME_PLAYER;i++)
			{
				var wViewChairID:uint=SwitchViewChairID(i);
				if(m_bPass[i][0] == true)
				{
					GetPeerGameView().SetPassFlag(wViewChairID,true);
					GetPeerGameView().m_UserCardControl[wViewChairID].SetCardData(null,0);
				}
				else
				{
					GetPeerGameView().SetPassFlag(wViewChairID,false);
					GetPeerGameView().m_UserCardControl[wViewChairID].SetCardData(m_cbOutCardData[i][0],m_cbOutCardCount[i][0]);
				}
			}
		
			return true;
		}
	}
}
include "../../Common/Memory.as"
include "./Common/GameLogicDef.as"
include "../../Net/GlobalDef.as"
include "../../Net/GlobalFrame.as"
include "./Common/CMD_LAND.as"