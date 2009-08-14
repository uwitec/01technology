package 
{
	import flash.display.MovieClip;
	import flash.utils.ByteArray;
	import flash.geom.Point;

	import org.god.IGameFrame.*;
	import org.god.GameModule.Common.*;
	import org.god.Net.*;
	import org.god.GameModule.B_UncoverPig.Common.*;

	public class GameClass extends GameBaseClass
	{
		//象素定义
		protected static var MAX_PELS:uint=25;//最小象素
		protected static var LESS_PELS:uint=10;//最小象素
		protected static var DEFAULT_PELS:uint=20;//默认象素
		//游戏定时器
		protected static var IDI_OUT_CARD:uint=200;	//出牌定时器
		protected static var IDI_EXIT_GAME:uint=201;//离开游戏定时器
		protected static var IDI_WAIT_START:uint=202;//举手定时器
		protected static var IDI_SHOW_CARD:uint=203;//亮牌定时器
		protected static var IDI_LAST_CARD_STOP:uint=204;                             //停留定时器
		
		//游戏变量
		protected var m_bCardCount:Array=new Array(GAME_PLAYER);//扑克数目
		protected var m_bHandCardCount:uint;//当前数目
		protected var m_bHandCardData:Array=new Array(HAND_CARD_COUNT);//手上扑克
		protected var m_bFirstGame:Boolean;//首局游戏
		protected var m_bAutoOutCard:Boolean;//自动代打

		//出牌变量
		protected var m_bTurnCardData:Array=new Array(GAME_PLAYER);//桌面出牌
		protected var m_wLastOutUser:uint;//最后出牌
		protected var m_wFirstOutUser:uint;//首先出牌
		protected var m_bTurnCardCount:uint;//桌面出牌  
		protected var m_bFirstOutSpade:Boolean;//第一张牌
		protected var m_bFirstHearts:Boolean;//第一张牌
		protected var m_bFirstClubs:Boolean;//第一张牌
		protected var m_bFirstDiamonds:Boolean;//第一张牌
		protected var m_bFirstOutCard:Boolean;//第一张牌

		//配置变量
		protected var m_bDeasilOrder:Boolean;//出牌顺序
		protected var m_dwCardHSpace:uint;//扑克象素
		protected var m_bTimerElapse:uint;//定时器长


		//控件变量
		protected var m_GameLogic;//游戏逻辑

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
			if (m_GameClientView == null)
			{
				throw Error("GetPeerGameView m_GameClientView == null");
				return null;
			}
			return m_GameClientView  as  GameView;
		}
		/////////////////////////////////////
		/////////////////////////////////////
		override protected  function CreateGameView():GameBaseView
		{
			return new GameView(this);
		}
		override public  function InitGameClass():Boolean
		{
			if (super.InitGameClass() == false)
			{
				return false;
			}
			m_GameLogic = new GameLogic;
			
			//游戏变量
			ZeroArray(m_bCardCount);
			m_bHandCardCount=0;
			ZeroArray(m_bHandCardData);
			m_bFirstGame=true;
			m_bAutoOutCard=false;

			//出牌变量
			ZeroArray(m_bTurnCardData);//出牌列表
			m_bTurnCardCount=0;
			m_wLastOutUser=INVALID_CHAIR;
			m_wFirstOutUser=INVALID_CHAIR;
			m_bFirstOutSpade=true;
			m_bFirstHearts=true;
			m_bFirstClubs=true;
			m_bFirstDiamonds=true;
			m_bFirstOutCard=false;


			//配置变量
			m_bDeasilOrder=false;//出牌顺序
			m_dwCardHSpace=DEFAULT_PELS;//扑克象素
			m_bTimerElapse=30;
			
				//配置控件
			GetPeerGameView().SetUserOrder(m_bDeasilOrder);
			GetPeerGameView().m_HandCardControl.SetCardSpace(m_dwCardHSpace,0,20);

			GetPeerGameView().addEventListener(GameViewEvent.strGameViewEvent,OnGameViewEevent);

			AddGameSound("GAME_END",SOUND_GAME_END);
			AddGameSound("GAME_LOST",SOUND_GAME_LOST);
			AddGameSound("GAME_START",SOUND_GAME_START);
			AddGameSound("GAME_WARN",SOUND_GAME_WARN);
			AddGameSound("GAME_WIN",SOUND_GAME_WIN);
			AddGameSound("OUT_CARD",SOUND_OUT_CARD);
			AddGameSound("MOST_CARD",SOUND_MOST_CARD);
			AddGameSound("PIG_CARD",SOUND_PIG_CARD);
			AddGameSound("SHEEP_CARD",SOUND_SHEEP_CARD);
			
			return true;
		}
		//重置状态
		override public  function DestroyGameClass():void
		{
			GetPeerGameView().removeEventListener(GameViewEvent.strGameViewEvent,OnGameViewEevent);

			m_bCardCount= null;
			m_bHandCardData = null;
			m_bTurnCardData = null;
			m_GameLogic = null;
			
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
				case IDI_OUT_CARD:			//用户出牌
				{
					//超时判断
					if (nElapse==0)
					{
						if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) AutomatismOutCard();
						return false;
					}
		
					//播放声音
					if ((nElapse<=10)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound("GAME_WARN");
		
					return true;
				}
			case IDI_EXIT_GAME:		//离开游戏
				{
					if (nElapse==0)
					{
						if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) OnExit(0,0);
						return false;
					}
					if ((nElapse<=10)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound("GAME_WARN");
		
					return true;
				}
			case IDI_SHOW_CARD:        //玩家亮牌
				{
					if (nElapse==0)
					{
						if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) OnNotShowCard(0,0);
						 GetPeerGameView().m_HandCardControl.ShootAllCard(false) ;
						return false;
					}
					if ((nElapse<=10)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound("GAME_WARN");
		
				}
			case IDI_LAST_CARD_STOP:   //扑克停留
				{
					if(nElapse==0)
					{
						//清除桌面
						for (var i:uint=0;i<GAME_PLAYER;i++) 
						{	
							GetPeerGameView().m_UserCardControl[i].SetCardData(null,0);
							ZeroArray(m_bTurnCardData) ;
						}	
					}
					return true ;
				}
			case IDI_WAIT_START:
				{
					if (nElapse==0)
					{
						if (IsLookonMode()==false)
						{
							SendEventExitGameClient();
						}
						return false;
					}
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
					case SUB_S_SEND_CARD:		//发送扑克
				{
					return OnSubSendCard(pBuffer,wDataSize);
				}
			case SUB_S_GAME_START:		//游戏开始
				{
					return OnSubGameStart(pBuffer,wDataSize);
				}
			case SUB_S_OUT_CARD:		//用户出牌
				{		
					return OnSubOutCard(pBuffer,wDataSize);
				}
			case SUB_S_SHOW_CARD:      //玩家亮牌
				{
					return OnSubShowCard(pBuffer , wDataSize) ;
				}
			case SUB_S_GAME_END:		//游戏结束
				{
					return OnSubGameEnd(pBuffer,wDataSize);
				}
			}

			return false;

		}
		
		override protected function OnGameSceneMessage(cbGameStation:uint,bLookonOther:Boolean,pBuffer:ByteArray,wDataSize:uint):Boolean
		{
			switch (cbGameStation)
			{
				case GS_WK_FREE:	//空闲状态
				{
					//设置界面
				
					//设置控件
					if (IsLookonMode()==false)
					{
						
						if(false==GetPeerGameView().m_btStart.IsWindowVisible())
						{
							GetPeerGameView().m_btStart.ShowWindow(true);
							GetPeerGameView().m_btStart.SetFocus();
						}
					}
					//设置扑克
					if (IsLookonMode()==false) GetPeerGameView().m_HandCardControl.SetDisplayFlag(true);
		
					return true;
				}
			case GS_WK_SHOWCARD:	//玩家亮牌
				{
					//效验数据
					if(wDataSize!=CMD_S_StatusShowCard.sizeof_CMD_S_StatusShowCard) return false ;
					var pSatusShowCard:CMD_S_StatusShowCard = CMD_S_StatusShowCard.readData(pBuffer);
		
					
					//设置变量
					m_bHandCardCount=HAND_CARD_COUNT;
					var wViewChairID:uint=SwitchViewChairID(pSatusShowCard.wCurrentUser);
					for (var i:uint=0;i<GAME_PLAYER;i++) m_bCardCount[i]=HAND_CARD_COUNT;
					CopyArray(m_bHandCardData,pSatusShowCard.bCardData,m_bHandCardCount);
					GetPeerGameView().SetCardCount(wViewChairID , m_bHandCardCount) ;
					GetPeerGameView().m_HandCardControl.SetCardData(m_bHandCardData,m_bHandCardCount); 
					FocusUpdateLeaveCardControl();
					
					if (IsLookonMode()==false) 
					{
						GetPeerGameView().m_HandCardControl.SetPositively(true);  
						GetPeerGameView().m_HandCardControl.SetDisplayFlag(true);
					}
		
					for(var i:uint=0 ; i<GAME_PLAYER ; ++i)
					{
						GetPeerGameView().SetCardCount(i , HAND_CARD_COUNT) ;
						wViewChairID = SwitchViewChairID(i) ;
						if(0!=pSatusShowCard.ShowCardCount[i])	GetPeerGameView().SetShowCard(wViewChairID , pSatusShowCard.ShowCard[i] , pSatusShowCard.ShowCardCount[i]) ;
					}
			
							//按钮控制
					if ((IsLookonMode()==false)&&(pSatusShowCard.wCurrentUser==GetMeChairID()) && 0==pSatusShowCard.ShowCardCount[pSatusShowCard.wCurrentUser])
					{
						//设置扑克
						GetPeerGameView().m_HandCardControl.SetPositively(true) ;
		
						var bCanShowCard:Boolean=false ;
						var bCardColor:uint ;
						var bCardValue:uint ;
						var bCardDataIndex:Array = new Array(4) ;
						var dwCardCount:uint=0 ; 
						for(var i:uint=0 ; i<HAND_CARD_COUNT ; ++i)
						{
							bCardColor = m_GameLogic.GetCardColor(m_bHandCardData[i]) ;
							bCardColor = bCardColor>>4 ;
							bCardValue = m_GameLogic.GetCardValue(m_bHandCardData[i]) ;
							//判断扑克
							if((3==bCardColor && 12==bCardValue) || //黑桃Q
								(2==bCardColor && 1==bCardValue)  || //红桃A
								(0==bCardColor && 11==bCardValue) || //方块J
								(1==bCardColor && 10==bCardValue))   //梅花10
							{
								GetPeerGameView().m_btShowCard.ShowWindow(true);
								GetPeerGameView().m_btShowCard.EnableWindow(true);
		
								GetPeerGameView().m_btNotShowCard.ShowWindow(true);
								GetPeerGameView().m_btNotShowCard.EnableWindow(true);
		
								GetPeerGameView().m_bShowCardHint = true ;
		
								//设置时间
								SetGameTimer(pSatusShowCard.wCurrentUser,IDI_SHOW_CARD,m_bTimerElapse);
		
								bCanShowCard = true ;
		
								//保存亮牌
								bCardDataIndex[dwCardCount++] = i ;
							}
						}
		
						//不能亮牌
						if(false==bCanShowCard)
						{
							var  ShowCard:CMD_C_ShowCard = new CMD_C_ShowCard ;
							var  sendData:ByteArray = ShowCard.toByteArray();
		
							SendGameData(SUB_C_SHOW_CART , sendData , CMD_C_ShowCard.sizeof_CMD_C_ShowCard) ;
						}
						else    //设置亮牌 
							GetPeerGameView().m_HandCardControl.SetShootCard(bCardDataIndex , dwCardCount) ;
					}
		
					return true ;
				}
			case GS_WK_PLAYING:		//游戏状态
				{
					//效验数据
					if (wDataSize!=CMD_S_StatusPlay.sizeof_CMD_S_StatusPlay) return false;
					var pStatusPlay:CMD_S_StatusPlay=CMD_S_StatusPlay.readData(pBuffer);
				
					//设置变量
					m_bHandCardCount=pStatusPlay.bCardCount[pStatusPlay.wCurrentUser];
		
					CopyArray(m_bHandCardData,pStatusPlay.bCardData,m_bHandCardCount);			
					//设置界面
			
					for(var i:uint=0 ; i<GAME_PLAYER ; ++i)
					{
						var wViewChairID:uint = SwitchViewChairID(i);
						GetPeerGameView().SetCardCount(wViewChairID , pStatusPlay.bCardCount[i]) ;
						m_bCardCount[i]= pStatusPlay.bCardCount[i] ;
					}
					if(0!=m_bHandCardCount)	
						GetPeerGameView().m_HandCardControl.SetCardData(m_bHandCardData,m_bHandCardCount); 
					FocusUpdateLeaveCardControl();
					m_wLastOutUser = pStatusPlay.wLastOutUser ;
					m_wFirstOutUser= pStatusPlay.wFirstOutUser ;
					m_bTurnCardCount = pStatusPlay.bTurnCardCount;
					if(m_bTurnCardCount == 0 && pStatusPlay.wCurrentUser == GetMeChairID())
					{
						m_wLastOutUser   = INVALID_CHAIR ;
						m_wFirstOutUser  = INVALID_CHAIR ;
					}
				
					var tmpArray:Array = new Array(1);
					for(var i:uint=0 ; i<GAME_PLAYER ; ++i)
					{
						var wViewChairID:uint = SwitchViewChairID(i);
						if(0!=pStatusPlay.bScoreCardCount[i])  GetPeerGameView().AddScoreCard(wViewChairID , pStatusPlay.ScoreCard[i] , pStatusPlay.bScoreCardCount[i]) ;
						if(0!=pStatusPlay.bShowCardCount[i])   GetPeerGameView().SetShowCard(wViewChairID , pStatusPlay.ShowCard[i] , pStatusPlay.bShowCardCount[i]) ;
									
						tmpArray[0] = pStatusPlay.bTurnCardData[i];
						if(tmpArray[0] != 0)
						GetPeerGameView().m_UserCardControl[wViewChairID].SetCardData(tmpArray,1);
					}
					CopyArray(m_bTurnCardData , pStatusPlay.bTurnCardData) ;
					    
					
					//玩家设置
					if ((IsLookonMode()==false)&&(pStatusPlay.wCurrentUser==GetMeChairID()))
					{
						GetPeerGameView().m_btOutCard.EnableWindow(false);  
						GetPeerGameView().m_btOutCard.ShowWindow(true);
					}
		
					//设置定时器
					SetGameTimer(pStatusPlay.wCurrentUser,IDI_OUT_CARD,m_bTimerElapse);
		
					//设置扑克
					if (IsLookonMode()==false) 
					{
						GetPeerGameView().m_HandCardControl.SetPositively(true);  
						GetPeerGameView().m_HandCardControl.SetDisplayFlag(true);
					}
		
					if(false==IsLookonMode())
					{
						//设置界面
						GetPeerGameView().m_btAutoOutCard.ShowWindow(true) ;
						GetPeerGameView().m_btAutoOutCard.EnableWindow(true) ;
					}
					return true;
				}
			}
			return false;
		}

		
		/////////////////////////////////////////////////////////////
		//消息处理
		//发送扑克
		protected function OnSubSendCard(pBuffer:ByteArray,wDataSize:uint):Boolean
		{
			//效验数据
			ASSERT(wDataSize==CMD_S_SendCard.sizeof_CMD_S_SendCard);
			if (wDataSize!=CMD_S_SendCard.sizeof_CMD_S_SendCard) return false;
		
			//设置状态
			SetGameStatus(GS_WK_SHOWCARD);
		
			//变量定义
			var pSendCard:CMD_S_SendCard=CMD_S_SendCard.readData(pBuffer);
			//设置数据
			m_bHandCardCount=CountArray(pSendCard.bCardData);  
			CopyArray(m_bHandCardData,pSendCard.bCardData);
		
			//设置界面
			for (var i:uint=0;i<GAME_PLAYER;i++)
			{
				GetPeerGameView().SetCardCount(i,HAND_CARD_COUNT);
				m_bCardCount[i] = HAND_CARD_COUNT ;
				GetPeerGameView().m_UserCardControl[i].SetCardData(null,0);
			}
			
			//设置扑克
			GetPeerGameView().m_HandCardControl.SetCardData(pSendCard.bCardData,HAND_CARD_COUNT);
			FocusUpdateLeaveCardControl();
			
			if (IsLookonMode()==true) 
			{
				GetPeerGameView().m_ScoreView.ShowWindow(false);
				GetPeerGameView().m_HandCardControl.SetDisplayFlag(false);
			}
		
			//设置按钮
			if ((IsLookonMode()==false)&&(pSendCard.wCurrentUser==GetMeChairID()))
			{
				//设置扑克
				GetPeerGameView().m_HandCardControl.SetPositively(true) ;
		
				var bCanShowCard:Boolean=false ;
				var bCardColor:uint ;
				var bCardValue:uint ;
				var bCardDataIndex:Array = new Array(4);
				var dwCardCount:uint=0 ; 
				for(var i:uint=0 ; i<HAND_CARD_COUNT ; ++i)
				{
		      		bCardColor = m_GameLogic.GetCardColor(pSendCard.bCardData[i]) ;
					bCardColor = bCardColor>>4 ;
					bCardValue = m_GameLogic.GetCardValue(pSendCard.bCardData[i]) ;
					//判断扑克
					if((3==bCardColor && 12==bCardValue) || //黑桃Q
					   (2==bCardColor && 1==bCardValue)  || //红桃A
					   (0==bCardColor && 11==bCardValue) || //方块J
					   (1==bCardColor && 10==bCardValue))   //梅花10
					{
		         	   GetPeerGameView().m_btShowCard.ShowWindow(true);
					   GetPeerGameView().m_btShowCard.EnableWindow(true);
		
					   GetPeerGameView().m_btNotShowCard.ShowWindow(true);
		          	   GetPeerGameView().m_btNotShowCard.EnableWindow(true);
		
					   GetPeerGameView().m_bShowCardHint = true ;
		
					   //设置时间
					   SetGameTimer(pSendCard.wCurrentUser,IDI_SHOW_CARD,m_bTimerElapse);
		
					   bCanShowCard = true ;
		
					   //保存亮牌
					   bCardDataIndex[dwCardCount++] = i ;
					}
				}
		
				//不能亮牌
				if(false==bCanShowCard)
				{
					var  ShowCard:CMD_C_ShowCard = new CMD_C_ShowCard ;
					var  sendData:ByteArray = ShowCard.toByteArray();
		
					SendGameData(SUB_C_SHOW_CART , sendData , CMD_C_ShowCard.sizeof_CMD_C_ShowCard) ;
				}
				else    //设置亮牌 
					GetPeerGameView().m_HandCardControl.SetShootCard(bCardDataIndex , dwCardCount) ;
			}
		
			//播放声音
			PlayGameSound("GAME_START");
		
			return true;
		}
		protected function OnSubGameStart(pBuffer:ByteArray,wDataSize:uint):Boolean
		{
			//游戏变量
			ZeroArray(m_bCardCount , HAND_CARD_COUNT ) ;       	    	
			m_bHandCardCount = HAND_CARD_COUNT;		
			FocusUpdateLeaveCardControl();
		
			//出牌变量
			ZeroArray(m_bTurnCardData) ;					
			m_bTurnCardCount = 0 ;
			m_wLastOutUser   = INVALID_CHAIR ;
			m_wFirstOutUser  = INVALID_CHAIR ;
		
			//效验数据
			ASSERT(wDataSize==CMD_S_GameStart.sizeof_CMD_S_GameStart);
			if (wDataSize!=CMD_S_GameStart.sizeof_CMD_S_GameStart) return false;
			SetGameStatus(GS_WK_PLAYING);
			//消息处理
			var pGameStart:CMD_S_GameStart=CMD_S_GameStart.readData(pBuffer);
			m_bFirstGame   = pGameStart.bFirstGame > 0 ? true : false;
		
			//玩家设置
			if (IsLookonMode()==false)
			{
				GetPeerGameView().m_HandCardControl.SetPositively(true);
			}
		
			//设置玩家
			if(false==IsLookonMode() && pGameStart.wCurrentUser==GetMeChairID())
			{
				ActiveGameFrame();
				GetPeerGameView().m_btOutCard.ShowWindow(true);
				GetPeerGameView().m_btOutCard.EnableWindow(false);
			}
		
			//播放声音
			PlayGameSound("GAME_START");
		
			//设置定时器
			SetGameTimer(pGameStart.wCurrentUser,IDI_OUT_CARD,m_bTimerElapse);
		
			if(false==IsLookonMode())
			{
				//设置界面
				GetPeerGameView().m_btAutoOutCard.ShowWindow(true) ;
				GetPeerGameView().m_btAutoOutCard.EnableWindow(true) ;
			}
			return true;
		}
		
		//用户亮牌
		protected function OnSubShowCard(pBuffer:ByteArray,wDataSize:uint):Boolean
		{
			//效验数据
			ASSERT(wDataSize==CMD_S_ShowCard.sizeof_CMD_S_ShowCard);
			if (wDataSize!=CMD_S_ShowCard.sizeof_CMD_S_ShowCard) return false;
		
			var pShowCard:CMD_S_ShowCard = CMD_S_ShowCard.readData(pBuffer);
		
			var wChairID:uint = SwitchViewChairID(pShowCard.wShowCardUser) ;
		
			//数据校验
			ASSERT(wChairID<GAME_PLAYER) ;
			if(wChairID>=GAME_PLAYER) return false ;
		
			var bCardColor:uint ;
			var bCardValue:uint ;
			for(var i:uint=0 ; i<pShowCard.bShowCardCount ; ++i)
			{
				bCardColor = m_GameLogic.GetCardColor(pShowCard.bShowCard[i]) ;
				bCardColor = bCardColor>>4 ;
				bCardValue = m_GameLogic.GetCardValue(pShowCard.bShowCard[i]) ;
		
				//数据校验
				ASSERT(0!=pShowCard.bShowCard[i]) ;
				if(0==pShowCard.bShowCard[i]) return false ;
				ASSERT((3==bCardColor && 12==bCardValue) || //黑桃Q
					   (2==bCardColor && 1==bCardValue)  || //红桃A
					   (0==bCardColor && 11==bCardValue) || //方块J
					   (1==bCardColor && 10==bCardValue));  //梅花10
		
			}
			//保存亮牌
			if(0!=pShowCard.bShowCardCount) GetPeerGameView().SetShowCard(wChairID , pShowCard.bShowCard , pShowCard.bShowCardCount) ;
		
			return true ;
		}
		
		//用户出牌
		 protected function OnSubOutCard(pBuffer:ByteArray,wDataSize:uint):Boolean
		{
			//变量定义
			var pOutCard:CMD_S_OutCard=CMD_S_OutCard.readData(pBuffer);
			var wHeadSize:uint=CMD_S_OutCard.sizeof_CMD_S_OutCard-CMD_S_OutCard.sizeof_bCardData;
			
			//效验数据
			ASSERT(wDataSize>=wHeadSize);
			if (wDataSize<wHeadSize) return false;
			ASSERT(wDataSize==(wHeadSize+pOutCard.bCardCount*CMD_S_OutCard.sizeof_bCardData));
			if (wDataSize!=(wHeadSize+pOutCard.bCardCount*CMD_S_OutCard.sizeof_bCardData)) return false;
			var tmp:Array = new Array(1);
			tmp[0] = pOutCard.bCardData;
			//删除定时器
			KillGameTimer(IDI_OUT_CARD);
			KillGameTimer(IDI_LAST_CARD_STOP) ;
		
			//清除桌面
			if(0==m_bTurnCardCount)
			{
				m_wFirstOutUser = pOutCard.wOutCardUser ;
		
				for (var i:uint=0;i<GAME_PLAYER;i++) 
				{	
					GetPeerGameView().m_UserCardControl[i].SetCardData(null,0);
					ZeroArray(m_bTurnCardData) ;
				}	
				if(GetMeChairID()==pOutCard.wOutCardUser) GetPeerGameView().m_UserCardControl[2].SetCardData(tmp,pOutCard.bCardCount);
			}
		
			//界面设置
			var wOutViewChairID:uint=SwitchViewChairID(pOutCard.wOutCardUser);
		
			m_bCardCount[pOutCard.wOutCardUser] -= pOutCard.bCardCount;
			GetPeerGameView().SetCardCount(wOutViewChairID,m_bCardCount[pOutCard.wOutCardUser]);
		
			//出牌设置
			if ((IsLookonMode()==true)||(GetMeChairID()!=pOutCard.wOutCardUser))
			{
				GetPeerGameView().m_UserCardControl[wOutViewChairID].SetCardData(tmp,pOutCard.bCardCount);
			}

			//保存出牌
			CopyArray(m_bTurnCardData,
					  tmp,
					  pOutCard.bCardCount,
					  pOutCard.wOutCardUser,0);
			m_wLastOutUser = pOutCard.wOutCardUser ;
			m_bTurnCardCount++ ;
		
			//出牌变量
			if(GAME_PLAYER==m_bTurnCardCount)
			{	
				//最大出牌
				var wMostUser:uint = m_wFirstOutUser ;
				var bFirstCardColor:uint = m_GameLogic.GetCardColor(m_bTurnCardData[m_wFirstOutUser]) ;
				bFirstCardColor = bFirstCardColor >> 4 ;
				var bMostCardValue:uint  = m_GameLogic.GetCardValue(m_bTurnCardData[m_wFirstOutUser]) ;
		
				if(1==bMostCardValue) bMostCardValue += 13 ;
		        //得分玩家
				for(var i:uint=0 ; i<GAME_PLAYER ; ++i)
				{
					var bCurrentCarColor:uint  = m_GameLogic.GetCardColor(m_bTurnCardData[i]) ;
					bCurrentCarColor = bCurrentCarColor >> 4 ;
					var bCurrentCardValue:uint = m_GameLogic.GetCardValue(m_bTurnCardData[i]) ;
					if(1==bCurrentCardValue) bCurrentCardValue += 13 ;
					if(bCurrentCarColor == bFirstCardColor)
					{
						if(bMostCardValue < bCurrentCardValue)
						{
							bMostCardValue = bCurrentCardValue ;
							wMostUser = i ;
						}
					}
				}
		
				//获取分数
				var bScoreCard:Array = new Array(GAME_PLAYER);
				var bScoreCount:uint=0 ;
				bScoreCount = m_GameLogic.GetScoreCard(m_bTurnCardData , bScoreCard , GAME_PLAYER , bScoreCount);
				if(0!=bScoreCount )
				{
					GetPeerGameView().AddScoreCard(SwitchViewChairID(wMostUser) , bScoreCard , bScoreCount) ;
				}
		
				//出牌列表		
				ZeroArray(m_bTurnCardData) ;					
				m_bTurnCardCount = 0 ;
				m_wLastOutUser   = INVALID_CHAIR ;
				m_wFirstOutUser  = INVALID_CHAIR ;
		
				SetGameTimer(pOutCard.wCurrentUser,IDI_LAST_CARD_STOP,2);
			}
			    
			//自己扑克
			if ((IsLookonMode()==true)&&(pOutCard.wOutCardUser==GetMeChairID()))
			{
				//删除扑克 
				var bSourceCount:uint=m_bHandCardCount;
				m_bHandCardCount-=pOutCard.bCardCount;
				m_GameLogic.RemoveCard(tmp,pOutCard.bCardCount,m_bHandCardData,bSourceCount);
				
				//设置界面
				GetPeerGameView().m_HandCardControl.SetCardData(m_bHandCardData,m_bHandCardCount);
			}
			FocusUpdateLeaveCardControl();
			//播放声音
		
			var bCardColor:uint  = m_GameLogic.GetCardColor(tmp) ;
			bCardColor = bCardColor >> 4 ;
			var bCardValue:uint = m_GameLogic.GetCardValue(tmp) ;
			if(3==bCardColor && 12==bCardValue) PlayGameSound("PIG_CARD");
			else if (0==bCardColor && 11==bCardValue) PlayGameSound("SHEEP_CARD");
			else PlayGameSound("OUT_CARD");
			
			//玩家设置
			if (pOutCard.wCurrentUser==GetMeChairID()&&IsLookonMode()==false)
			{		
				ActiveGameFrame();
				GetPeerGameView().m_btOutCard.ShowWindow(true);
				GetPeerGameView().m_btOutCard.EnableWindow(false);
				GetPeerGameView().m_HandCardControl.ShootAllCard(false) ;
		
				//设置定时器
				SetGameTimer(pOutCard.wCurrentUser,IDI_OUT_CARD,m_bTimerElapse);
			}
		
			return true;
		}
		//游戏结束
		protected function OnSubGameEnd(pBuffer:ByteArray,wDataSize:uint):Boolean
		{
		//	InsertSystemString("protected function OnSubGameEnd");
			//效验数据
			ASSERT(wDataSize==CMD_S_GameEnd.sizeof_CMD_S_GameEnd);
			if (wDataSize!=CMD_S_GameEnd.sizeof_CMD_S_GameEnd) return false;
		
			//消息处理
			var pGameEnd:CMD_S_GameEnd=CMD_S_GameEnd.readData(pBuffer);
		
			//删除定时器
			KillGameTimer(IDI_OUT_CARD);
			KillGameTimer(IDI_SHOW_CARD);
		
			//隐藏控件
			GetPeerGameView().m_btOutCard.ShowWindow(false);
			GetPeerGameView().m_btShowCard.ShowWindow(false);
			GetPeerGameView().m_btNotShowCard.ShowWindow(false);
			//禁用控件
			GetPeerGameView().m_btOutCard.EnableWindow(false);
			//设置界面
			GetPeerGameView().m_btAutoOutCard.ShowWindow(false) ;
			GetPeerGameView().m_btAutoOutCard.EnableWindow(false) ;
			GetPeerGameView().m_btCancelAutoOutCard.ShowWindow(false) ;
			GetPeerGameView().m_btCancelAutoOutCard.EnableWindow(false) ;
			SetGameStatus(GS_WK_FREE);
			GetPeerGameView().m_bShowCardHint = false ;
			GetPeerGameView().UpdateGameView();
			GetPeerGameView().m_HandCardControl.ShootAllCard(false) ;
		
			//设置扑克
			for (var i:uint=0;i<GAME_PLAYER;i++)
			{
				GetPeerGameView().m_UserCardControl[i].SetCardData(null,0);
			}
		
			//显示扑克
			if (IsLookonMode()==true) GetPeerGameView().m_HandCardControl.SetDisplayFlag(true);
		
			//设置积分
			for (var i:uint=0;i<GAME_PLAYER;i++)
			{
				GetPeerGameView().m_ScoreView.SetGameScore(i,pGameEnd.fGameScore[i]);
			}
			GetPeerGameView().m_ScoreView.SetMeChairID(GetMeChairID());
			GetPeerGameView().m_ScoreView.ShowWindow(true);
		
			//设置界面
			if (IsLookonMode()==false)
			{
				if(false==GetPeerGameView().m_btStart.IsWindowVisible())
				{
					GetPeerGameView().m_btStart.ShowWindow(true);
			    	
				}
				GetPeerGameView().m_bSendWaiMsg=true;
				SetGameTimer(GetMeChairID(),IDI_EXIT_GAME,30);
			}
		
			//设置变量
			GetPeerGameView().AddScoreCard(INVALID_CHAIR , null , 0) ;
			GetPeerGameView().SetShowCard(INVALID_CHAIR , null , 0) ;
		
			return true;
		}
		//出牌判断
		protected function VerdictOutCard(bAutoOutCard:Boolean = false):Boolean
		{
			//状态判断
			if (GetPeerGameView().m_btOutCard.IsWindowVisible()==false) return false;
		
			//获取扑克
			var bCardData:Array = new Array(HAND_CARD_COUNT);
			var bShootCount:uint=GetPeerGameView().m_HandCardControl.GetShootCard(bCardData,HAND_CARD_COUNT);
		
			//出牌判断
			if (bShootCount==1)
			{
				if(true==m_bFirstGame)
				{
					//首牌判断
					var bFisrCard:Boolean=true ;
					for(var i:uint=0 ; i<GAME_PLAYER ; ++i)
						if(HAND_CARD_COUNT!=m_bCardCount[i]) bFisrCard = false ;
		
					if(true==bFisrCard)
					{
						var bCardColor:uint = m_GameLogic.GetCardColor(bCardData[0]) ;
						bCardColor = bCardColor>>4 ;
						var bCardValue:uint = m_GameLogic.GetCardValue(bCardData[0]) ;
						if(1==bCardColor && 2==bCardValue) 
						{
							m_bFirstClubs = false ;
							return true ;
						}
						if(false==bAutoOutCard)
						{
							//MessageBox("第一局的第一张牌必须先出梅花2！" , "拱猪提示" , MB_OK|MB_ICONINFORMATION) ;
						}
						GetPeerGameView().m_HandCardControl.ShootAllCard(false) ;
						return false ;
					}
				}
		
				if(INVALID_CHAIR==m_wFirstOutUser) 
				{
					var bCardColor:uint = m_GameLogic.GetCardColor(bCardData[0]) ;
					bCardColor      = bCardColor>>4 ; 
					return VerdictFirstGameColor(bCardColor , bCardData[0] , true) ;
				}
				var bFirstOutColor:uint = m_GameLogic.GetCardColor(m_bTurnCardData[m_wFirstOutUser]) ;
				bFirstOutColor = bFirstOutColor>>4 ;
				var bCurrentColor:uint = m_GameLogic.GetCardColor(bCardData[0]) ;
				bCurrentColor = bCurrentColor>>4 ;
		
				//是否垫牌
				if(bFirstOutColor!=bCurrentColor) 
				{
					for(var i:uint=0 ; i<m_bHandCardCount ; ++i)
					{
						bCurrentColor = m_GameLogic.GetCardColor(m_bHandCardData[i]) ;
						bCurrentColor = bCurrentColor >> 4 ;
						if(bFirstOutColor==bCurrentColor)
						{
							if(false==bAutoOutCard)	
							{
								//MessageBox("您必须出和第一张牌相同花色的牌！" , "拱猪提示" , MB_OK|MB_ICONINFORMATION) ;
							}
							GetPeerGameView().m_HandCardControl.ShootAllCard(false) ;
							return false ;
						}
					}
					return true ;
				}
				else
					return VerdictFirstGameColor(bFirstOutColor , bCardData[0] ,true) ;
				
				
			}
		
			return false;
		}
		//第一轮花色
		protected function VerdictFirstGameColor(bFirstOutColor:uint , bCardData:uint ,  bAutoOutCard:Boolean):Boolean
		{
			var bCurrentColor:uint = m_GameLogic.GetCardColor(bCardData) ;
			bCurrentColor = bCurrentColor>>4 ;
		
			//第一轮花色
			switch(bFirstOutColor)
			{
			case 0:      //方块
				{
					//恢复变量
					if(true==m_bFirstOutCard) m_bFirstDiamonds = true ;
		
					//第一轮判断
					if(true==m_bFirstDiamonds)
					{
						m_bFirstDiamonds = false ;
						var bIsShowCard:Boolean = false ;
		
						//是否亮牌
						for(var i:uint=0 ; i<GetPeerGameView().m_bShowCardCount[2] ; ++i)
							if(bCardData==GetPeerGameView().m_bShowCard[2][i]) 
							{
								bIsShowCard = true ;
								break;
							}
		
							if(true==bIsShowCard)
							{
								for(var i:uint=0 ; i<m_bHandCardCount ; ++i)
								{
									if(bCardData==m_bHandCardData[i]) continue ;
									var bCardColor:uint = m_GameLogic.GetCardColor(m_bHandCardData[i]) ;
									bCardColor = bCardColor>>4 ;
									if(bCardColor==bCurrentColor) 
									{
										if(false==bAutoOutCard)
										{
										 //MessageBox("亮过的牌不能在该花色首轮出牌时出！" , "拱猪提示" , MB_OK|MB_ICONINFORMATION) ;
										 }
										GetPeerGameView().m_HandCardControl.ShootAllCard(false) ;
										//恢复变量
										m_bFirstDiamonds = true ;
										return false ;
									}
								}
		
								return true ;
							}
							else
							{
								//恢复变量
								m_bFirstOutCard = true ;
								return true ;
							}
					}		
					else return true ;
		
					return true ;
				}
			case 1:     //梅花
				{
					//恢复变量
					if(true==m_bFirstOutCard) m_bFirstClubs = true ;
		
					//第一轮判断
					if(true==m_bFirstClubs)
					{
						m_bFirstClubs = false ;
						var bIsShowCard:Boolean = false ;
		
						//是否亮牌
						for(var i:uint=0 ; i<GetPeerGameView().m_bShowCardCount[2] ; ++i)
							if(bCardData==GetPeerGameView().m_bShowCard[2][i]) 
							{
								bIsShowCard = true ;
								break;
							}
		
							if(true==bIsShowCard)
							{
								for(var i:uint=0 ; i<m_bHandCardCount ; ++i)
								{
									if(bCardData==m_bHandCardData[i]) continue ;
									var bCardColor:uint = m_GameLogic.GetCardColor(m_bHandCardData[i]) ;
									bCardColor = bCardColor>>4 ;
									if(bCardColor==bCurrentColor) 
									{
										if(false==bAutoOutCard)
										{
										// MessageBox("亮过的牌不能在该花色首轮出牌时出！" , "拱猪提示" , MB_OK|MB_ICONINFORMATION) ;
										 }
										GetPeerGameView().m_HandCardControl.ShootAllCard(false) ;
										//恢复变量
										m_bFirstClubs = true ;
										return false ;
									}
								}
		
								return true ;
							}
							else
							{
								//恢复变量
								m_bFirstOutCard = true ;
								return true ;
							}
					}		
					else return true ;
		
					return true ;
				}
			case 2:     //红桃
				{
					//恢复变量
					if(true==m_bFirstOutCard) m_bFirstHearts = true ;
		
					if(true==m_bFirstHearts)
					{
						m_bFirstHearts = false ;
						var bIsShowCard:Boolean = false ;
		
						//是否亮牌
						for(var i:uint=0 ; i<GetPeerGameView().m_bShowCardCount[2] ; ++i)
							if(bCardData==GetPeerGameView().m_bShowCard[2][i]) 
							{
								bIsShowCard = true ;
								break;
							}
		
							if(true==bIsShowCard)
							{
								for(var i:uint=0 ; i<m_bHandCardCount ; ++i)
								{
									if(bCardData==m_bHandCardData[i]) continue ;
									var bCardColor:uint = m_GameLogic.GetCardColor(m_bHandCardData[i]) ;
									bCardColor = bCardColor>>4 ;
									if(bCardColor==bCurrentColor) 
									{
										if(false==bAutoOutCard) 
										{
										//MessageBox("亮过的牌不能在该花色首轮出牌时出！" , "拱猪提示" , MB_OK|MB_ICONINFORMATION) ;
										}
										GetPeerGameView().m_HandCardControl.ShootAllCard(false) ;
										//恢复变量
										m_bFirstHearts = true ;
										return false ;
									}
								}
								return true ;
							}
							else
							{
								//恢复变量
								m_bFirstOutCard = true ;
								return true ;
							}
					}		
					else return true ;
		
					return true ;
				}
			case 3:     //黑桃
				{
					//恢复变量
					if(true==m_bFirstOutCard) m_bFirstHearts = true ;
		
					if(true==m_bFirstOutSpade)
					{
						m_bFirstOutSpade = false ;
						var bIsShowCard:Boolean = false ;
		
						//是否亮牌
						for(var i:uint=0 ; i<GetPeerGameView().m_bShowCardCount[2] ; ++i)
							if(bCardData==GetPeerGameView().m_bShowCard[2][i]) 
							{
								bIsShowCard = true ;
								break;
							}
		
							if(true==bIsShowCard)
							{
								for(var i:uint=0 ; i<m_bHandCardCount ; ++i)
								{
									if(bCardData==m_bHandCardData[i]) continue ;
									var bCardColor:uint = m_GameLogic.GetCardColor(m_bHandCardData[i]) ;
									bCardColor = bCardColor>>4 ;
									if(bCardColor==bCurrentColor) 
									{
										if(false==bAutoOutCard)
										{
										 //MessageBox("亮过的牌不能在该花色首轮出牌时出！" , "拱猪提示" , MB_OK|MB_ICONINFORMATION) ;
										 }
										GetPeerGameView().m_HandCardControl.ShootAllCard(false) ;
										//恢复变量
										m_bFirstOutSpade = true ;
										return false ;
									}
								}
								return true ;
							}
							else 
							{
								//恢复变量
								m_bFirstOutCard = true ;
								return true ;
							}
					}		
					else return true ;
		
					return true ;
				}
			default:    //错误
				return false ;
			}
			return true ;
		}
		
		//亮牌判断
		protected function VerdictShowCard():Boolean
		{
			//状态判断
			if (GetPeerGameView().m_btShowCard.IsWindowVisible()==false) return false;
		
			//获取扑克
			var bCardData:Array = new Array(HAND_CARD_COUNT);
			var bShootCount:uint=GetPeerGameView().m_HandCardControl.GetShootCard(bCardData,HAND_CARD_COUNT);
		
			//判断亮牌
			if(bShootCount>=1 && bShootCount<=4)
			{
				var bCardColor:uint ;
				var bCardValue:uint ;
		
				var bShowCardCount:uint=0;
				for(var i:uint=0 ; i<bShootCount ; ++i)
				{
					bCardColor = m_GameLogic.GetCardColor(bCardData[i]) ;
					bCardColor = bCardColor>>4 ;
					bCardValue = m_GameLogic.GetCardValue(bCardData[i]) ;
					//判断扑克
					if((3==bCardColor && 12==bCardValue)  || //黑桃Q
						(2==bCardColor && 1==bCardValue)  || //红桃A
						(0==bCardColor && 11==bCardValue) || //方块J
						(1==bCardColor && 10==bCardValue))   //梅花10
					{
						++bShowCardCount;
					}
				}
				return bShootCount==bShowCardCount;
			}
			return false ;
		}
		

		//开始按钮
		override protected function OnStart(wParam:uint, lParam:uint):uint
		{
			
			//设置变量
			m_bHandCardCount=0;
			m_bTurnCardCount=0;
			ZeroArray(m_bHandCardData);
			ZeroArray(m_bTurnCardData);
			m_bAutoOutCard = false ;
		
			//出牌变量
			m_wFirstOutUser  = INVALID_CHAIR ;
			m_wLastOutUser   = INVALID_CHAIR ;
			m_bFirstOutSpade = true;
			m_bFirstHearts   = true ;  
			m_bFirstClubs    = true ;   
			m_bFirstDiamonds = true ;
			m_bFirstOutCard  = false ;
		
			//配置变量
			m_bDeasilOrder  = false ;			        			
			m_dwCardHSpace  = DEFAULT_PELS ;	    				
			m_bTimerElapse  = 30 ;
		
			//设置界面
			KillGameTimer(IDI_EXIT_GAME);
			KillGameTimer(IDI_WAIT_START);
			GetPeerGameView().SetCardCount(INVALID_CHAIR,0);
		
			//隐藏控件
			GetPeerGameView().m_btStart.ShowWindow(false);
			GetPeerGameView().m_ScoreView.ShowWindow(false);
			GetPeerGameView().m_btAutoOutCard.ShowWindow(false) ;
			GetPeerGameView().m_btAutoOutCard.EnableWindow(false) ;
			GetPeerGameView().m_btCancelAutoOutCard.ShowWindow(false) ;
			GetPeerGameView().m_btCancelAutoOutCard.EnableWindow(false) ;
			GetPeerGameView().m_btShowCard.ShowWindow(false);
			GetPeerGameView().m_btNotShowCard.ShowWindow(false);

			//设置扑克
			GetPeerGameView().m_HandCardControl.SetCardData(null,0);
			GetPeerGameView().m_HandCardControl.SetPositively(false);
			//设置变量
			GetPeerGameView().AddScoreCard(INVALID_CHAIR , null , 0) ;
			GetPeerGameView().SetShowCard(INVALID_CHAIR , null , 0) ;
			GetPeerGameView().m_bShowCardHint = false ;
			GetPeerGameView().UpdateGameView();
		
			//清空扑克
			for (var i:uint=0;i<GAME_PLAYER;i++) GetPeerGameView().m_UserCardControl[i].SetCardData(null,0);
		
			//发送消息
			SendUserReady(null,0);
		
			return 0;
		}
		//出牌消息
		protected function OnOutCard(wParam:uint, lParam:uint):uint
		{
			
			//状态判断
			if(GetPeerGameView().m_btOutCard.IsWindowVisible()==false) return 0;
			if(GetPeerGameView().m_btOutCard.IsWindowEnabled()==false) return 0;
		
			//恢复变量
			m_bFirstOutCard = false ;
		
			//设置界面
			KillGameTimer(IDI_OUT_CARD);
			GetPeerGameView().m_btOutCard.ShowWindow(false);
			GetPeerGameView().m_btOutCard.EnableWindow(false);
		
			//发送数据
			var OutCard:CMD_C_OutCard = new CMD_C_OutCard;
			var tmp:Array = new Array(1);
			OutCard.bCardCount=GetPeerGameView().m_HandCardControl.GetShootCard(tmp,1);
			OutCard.bCardData = tmp[0];
			var sendData:ByteArray = OutCard.toByteArray();
			SendGameData(SUB_C_OUT_CART,sendData,CMD_C_OutCard.sizeof_CMD_C_OutCard-CMD_C_OutCard.sizeof_bCardData+OutCard.bCardCount);
		
			//预先显示
			PlayGameSound("OUT_CARD");
		
			GetPeerGameView().m_UserCardControl[2].SetCardData(tmp,
															   OutCard.bCardCount);
		
			//预先删除
			var bSourceCount:uint=m_bHandCardCount;
			m_bHandCardCount-=OutCard.bCardCount;
			m_GameLogic.RemoveCard(tmp,OutCard.bCardCount,m_bHandCardData,bSourceCount);
			GetPeerGameView().m_HandCardControl.SetCardData(m_bHandCardData,m_bHandCardCount);
		
			return 0;
		}
		//左键扑克
		protected function OnLeftHitCard(wParam:uint, lParam:uint):uint
		{
			
			//出牌按钮
			if(true==GetPeerGameView().m_btOutCard.IsWindowVisible())
			{
				//设置控件
				var bResult:Boolean = VerdictOutCard() ;
		
				GetPeerGameView().m_btOutCard.EnableWindow((true==bResult)?true:false); 
			}
		
			//亮牌按钮
			if(true==GetPeerGameView().m_btShowCard.IsWindowVisible())
			{
				//设置控件
				var bResult:Boolean = VerdictShowCard() ;
		
				GetPeerGameView().m_btShowCard.EnableWindow((true==bResult)?true:false); 
		
			}
		
			return 0;
		}
		
		//右键扑克
		protected function OnRightHitCard(wParam:uint, lParam:uint):uint
		{

			//用户出牌
			var bResult:Boolean = VerdictOutCard() ;
			if(false==bResult) return 0 ;
			OnOutCard(0,0);
		
			return 0;
		}
		
		protected function OnWaitStart(wParam:uint, lParam:uint):uint
		{
			
			KillGameTimer(IDI_EXIT_GAME);
			SetGameTimer(GetMeChairID(),IDI_WAIT_START,10);
			return 0;
		}
		//玩家不亮
		protected function OnNotShowCard(wParam:uint, lParam:uint):uint
		{
			//无用参数

			//删除定时器
			KillGameTimer(IDI_SHOW_CARD);
		
			var  ShowCard:CMD_C_ShowCard = new CMD_C_ShowCard;
			var  sendData:ByteArray = ShowCard.toByteArray();
		
			SendGameData(SUB_C_SHOW_CART , sendData , CMD_C_ShowCard.sizeof_CMD_C_ShowCard) ;
		
			GetPeerGameView().m_btShowCard.ShowWindow(false);
		
			GetPeerGameView().m_btNotShowCard.ShowWindow(false);
		
			GetPeerGameView().m_HandCardControl.ShootAllCard(false) ;
		
			GetPeerGameView().m_bShowCardHint = false ;
			GetPeerGameView().UpdateGameView();
		
			return 0 ;
		}
		//玩家亮牌
		protected function OnShowCard(wParam:uint, lParam:uint):uint
		{
			//无用参数
			
			//删除定时器
			KillGameTimer(IDI_SHOW_CARD);
		
			//获取扑克
			var bCardData:Array = new Array(4);
			var bShootCount:uint=GetPeerGameView().m_HandCardControl.GetShootCard(bCardData,4);
		
			var  ShowCard:CMD_C_ShowCard = new CMD_C_ShowCard ;
			CopyArray(ShowCard.bShowCard , bCardData , bShootCount) ;
			ShowCard.bShowCardCount = bShootCount ;
			var  sendData:ByteArray = ShowCard.toByteArray();
		   SendGameData(SUB_C_SHOW_CART ,sendData , CMD_C_ShowCard.sizeof_CMD_C_ShowCard) ;
		
		   GetPeerGameView().m_HandCardControl.ShootAllCard(false) ;
		
			GetPeerGameView().m_btShowCard.ShowWindow(false);
		
			GetPeerGameView().m_btNotShowCard.ShowWindow(false);
		
			GetPeerGameView().m_bShowCardHint = false ;
			GetPeerGameView().UpdateGameView();
		
		   return 0 ;
		}
		//自动代打
		protected function OnAutoOutCard(wParam:uint, lParam:uint):uint
		{
			//无用参数

			m_bAutoOutCard = !m_bAutoOutCard ; 
		
			//设置时间
			if(true==m_bAutoOutCard) m_bTimerElapse = 1 ;
		
			AutomatismOutCard() ;
		
			//设置界面
			GetPeerGameView().m_btAutoOutCard.ShowWindow(false) ;
			GetPeerGameView().m_btAutoOutCard.EnableWindow(false) ;
			GetPeerGameView().m_btCancelAutoOutCard.ShowWindow(true) ;
			GetPeerGameView().m_btCancelAutoOutCard.EnableWindow(true) ;
			return 0 ;
		}
		//取消代打
		protected function OnCancelAutoOutCard(wParam:uint, lParam:uint):uint
		{
			//无用参数
			
			m_bAutoOutCard = !m_bAutoOutCard ;
			if(!m_bAutoOutCard)	m_bTimerElapse = 30 ;
		
			//设置界面
			GetPeerGameView().m_btCancelAutoOutCard.ShowWindow(false) ;
			GetPeerGameView().m_btCancelAutoOutCard.EnableWindow(false) ;
			GetPeerGameView().m_btAutoOutCard.ShowWindow(true) ;
			GetPeerGameView().m_btAutoOutCard.EnableWindow(true) ;
		
			return 0 ;
		}
		//自动出牌
		protected function AutomatismOutCard():Boolean
		{
			//设置扑克
			var bCardDataIndex:uint;
			GetPeerGameView().m_HandCardControl.ShootAllCard(false) ;
			for(var i:uint=m_bHandCardCount-1 ; i!=255 ; --i)
			{
				//数据验证
				ASSERT(i>=0 && i<=GetPeerGameView().m_HandCardControl.GetCardCount()) ;
		        if(i==255 || i>GetPeerGameView().m_HandCardControl.GetCardCount()) return false ;
			
		    	bCardDataIndex = i ;
				var tmp:Array = new Array(1);
				tmp[0] = bCardDataIndex;
				GetPeerGameView().m_HandCardControl.SetShootCard(tmp , 1) ;	
				//有效判断
				if(true==VerdictOutCard(true)) 
				{
					GetPeerGameView().m_btOutCard.EnableWindow(true) ;
					OnOutCard(0 , 0) ;
					return true ;
				}
				GetPeerGameView().m_HandCardControl.ShootAllCard(false) ;
			}
			return false ;
		}
		private function OnGameViewEevent(e:GameViewEvent):void
		{
			switch (e.m_nMsg)
			{
				case GameViewEvent.nGVM_START :
					{
						OnStart(e.m_nWParam,e.m_nLParam);
						return;


					};
				case GameViewEvent.nGVM_OUT_CARD :
					{
						OnOutCard(e.m_nWParam,e.m_nLParam);
						return;


					};
				case GameViewEvent.nGVM_LEFT_HIT_CARD:
					{
						OnLeftHitCard(e.m_nWParam,e.m_nLParam);
						return;


					};
				case GameViewEvent.nGVM_SHOW_CARD :
					{
						OnShowCard(e.m_nWParam,e.m_nLParam);
						return;


					};
				case GameViewEvent.nGVM_NOT_SHOW_CARD :
					{
						OnNotShowCard(e.m_nWParam,e.m_nLParam);
						return;


					};
				case GameViewEvent.nGVM_AUTO_OUT_CARD :
					{
						OnAutoOutCard(e.m_nWParam,e.m_nLParam);
						return;


					};
				case GameViewEvent.nGVM_CANCEL_AUTO_OUT_CARD :
					{
						OnCancelAutoOutCard(e.m_nWParam,e.m_nLParam);
						return;


				}
			}
		};

		//////////////////////////////////////
		//辅助函数
		private function GenEmptyCardData(nCardCount:int):Array
		{
			var result:Array = new Array(nCardCount);
			ZeroArray(result, 54);
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
				else if (wViewChairID==1)
				{
					GetPeerGameView().m_LeaveCardControl[1].SetCardData(bCardData,m_bCardCount[n]);
				}
				else if (wViewChairID==3)
				{
					GetPeerGameView().m_LeaveCardControl[2].SetCardData(bCardData,m_bCardCount[n]);
				}
			}	
		}
		private function FocusUpdateLeaveCardControl():void
		{
			var bCardData:Array = GenEmptyCardData(14);
			UpdateLeaveCardControl(bCardData);
		}
	}
}
include "../../Common/Memory.as"
include "../../Net/GlobalDef.as"
include "../../Net/GlobalFrame.as"
include "./Common/CMD_UncoverPig.as"
