package 
{
	import flash.display.MovieClip;
	import flash.utils.ByteArray;
	import flash.geom.Point;

	import org.god.IGameFrame.*;
	import org.god.GameModule.Common.*;
	import org.god.Net.*;
	import org.god.GameModule.B_Hoe.Common.*;

	public class GameClass extends GameBaseClass
	{
		//游戏定时器
		protected static  const IDI_OUT_CARD:uint=200;//出牌定时器
		protected static  const IDI_MOST_CARD:uint=201;//最大定时器
		protected static  const IDI_EXIT_GAME:uint=202;//离开游戏定时器

		//象素定义
		protected static  const MAX_PELS:uint=25;//最小象素
		protected static  const LESS_PELS:uint=10;//最小象素
		protected static  const DEFAULT_PELS:uint=20;//默认象素

		//游戏变量
		protected var m_wBankerUser:uint;//庄家用户
		protected var m_bCardCount:Array=new Array(4);//扑克数目
		protected var m_bHandCardCount:uint;//扑克数目
		protected var m_bHandCardData:Array=new Array(13);//手上扑克

		//出牌变量
		protected var m_bTurnOutType:uint;//出牌类型
		protected var m_bTurnCardCount:uint;//出牌数目
		protected var m_bTurnCardData:Array=new Array(13);//出牌列表

		//配置变量
		protected var m_bDeasilOrder:Boolean;//出牌顺序
		protected var m_dwCardHSpace:uint;//扑克象素

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
			if (m_GameClientView == null)
			{
				throw Error("GetPeerGameView GetPeerGameView() == null");
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
			m_GameLogic=new GameLogic;

			GetPeerGameView().addEventListener(GameViewEvent.strGameViewEvent,OnGameViewEevent);

			//游戏变量
			m_bHandCardCount=0;
			m_wBankerUser=INVALID_CHAIR;
			ZeroArray(m_bCardCount);
			ZeroArray(m_bHandCardData);

			//配置变量
			m_bDeasilOrder=false;
			m_dwCardHSpace=DEFAULT_PELS;

			//出牌变量
			m_bTurnCardCount=0;
			m_bTurnOutType=CT_ERROR;
			ZeroArray(m_bTurnCardData);

			//辅助变量
			m_wTimeOutCount=0;
			m_wMostUser=INVALID_CHAIR;
	
			//配置控件
			GetPeerGameView().SetUserOrder(m_bDeasilOrder);
			GetPeerGameView().m_HandCardControl.SetCardSpace(m_dwCardHSpace,0,20);

			AddGameSound("GAME_START",SOUND_GAME_START);
			AddGameSound("GAME_WARN",SOUND_GAME_WARN);
			AddGameSound("GAME_END",SOUND_GAME_END);
			AddGameSound("GAME_WIN",SOUND_GAME_WIN);
			AddGameSound("GAME_LOST",SOUND_GAME_LOST);
			AddGameSound("OUT_CARD",SOUND_OUT_CARD);
			AddGameSound("MOST_CARD",SOUND_MOST_CARD);
			
			return true;
		}
		//重置状态
		override public  function DestroyGameClass():void
		{
			m_bTurnCardData=null;
			m_bHandCardData=null;
			m_bCardCount=null;
			m_GameLogic=null;

			GetPeerGameView().removeEventListener(GameViewEvent.strGameViewEvent,OnGameViewEevent);

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
					if (m_bHandCardCount<m_bTurnCardCount) return true;
					if ((nElapse<=3)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound("GAME_WARN");
		
					return true;
				}
			case IDI_EXIT_GAME:		//离开游戏
				{
					if (nElapse==0)
					{
						if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) OnExit(0,0);
						return false;
					}
					if ((nElapse<=3)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound("GAME_WARN");
		
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
				case SUB_S_GAME_START:		//游戏开始
				{
					return OnSubGameStart(pBuffer,wDataSize);
				}
			case SUB_S_OUT_CARD:		//用户出牌
				{
					return OnSubOutCard(pBuffer,wDataSize);
				}
			case SUB_S_PASS_CARD:		//放弃出牌
				{
					return OnSubPassCard(pBuffer,wDataSize);
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
				case GS_RF_FREE:	//空闲状态
				{
					//效验数据
					if (wDataSize!=CMD_S_StatusFree.sizeof_CMD_S_StatusFree) return false;
					var pStatusFree:CMD_S_StatusFree=CMD_S_StatusFree.readData(pBuffer);
		
					//设置控件
					if (IsLookonMode()==false)
					{
						GetPeerGameView().m_btStart.ShowWindow(true);
						GetPeerGameView().m_btStart.SetFocus();
					}
					//设置扑克
					if (IsLookonMode()==false) GetPeerGameView().m_HandCardControl.SetDisplayFlag(true);
		
					return true;
				}
			case GS_RF_PLAYING:		//游戏状态
				{
					//效验数据
					if (wDataSize!=CMD_S_StatusPlay.sizeof_CMD_S_StatusPlay) return false;
					var  pStatusPlay:CMD_S_StatusPlay=CMD_S_StatusPlay.readData(pBuffer);
		
					//设置变量
					m_wBankerUser=pStatusPlay.wBankerUser;
					m_bTurnCardCount=pStatusPlay.bTurnCardCount;
					m_bHandCardCount=pStatusPlay.bCardCount[GetMeChairID()];
					m_bTurnOutType=m_GameLogic.GetCardType(pStatusPlay.bTurnCardData,m_bTurnCardCount);
					CopyArray(m_bHandCardData,pStatusPlay.bCardData,m_bHandCardCount);
					CopyArray(m_bTurnCardData,pStatusPlay.bTurnCardData,pStatusPlay.bTurnCardCount);
		
					//设置界面
					for (var i:uint=0;i<GAME_PLAYER;i++)
					{
						var wViewChairID:uint=SwitchViewChairID(i);
						m_bCardCount[i]=pStatusPlay.bCardCount[i];
						GetPeerGameView().SetCardCount(wViewChairID,pStatusPlay.bCardCount[i]);
						GetPeerGameView().m_fAllTurnScore[wViewChairID]=pStatusPlay.fAllTurnScore[i];
						GetPeerGameView().m_fLastTurnScore[wViewChairID]=pStatusPlay.fLastTurnScore[i];
					}
					GetPeerGameView().m_HandCardControl.SetCardData(m_bHandCardData,m_bHandCardCount);
					var bCardData:Array = GenEmptyCardData(13);
					UpdateLeaveCardControl(bCardData);
					
					//玩家设置
					if ((IsLookonMode()==false)&&(pStatusPlay.wCurrentUser==GetMeChairID()))
					{
						GetPeerGameView().m_btOutCard.EnableWindow(false);
						GetPeerGameView().m_btOutCard.ShowWindow(true);
						GetPeerGameView().m_btOutPrompt.ShowWindow(true);
						GetPeerGameView().m_btPassCard.ShowWindow(true);
						GetPeerGameView().m_btPassCard.EnableWindow((m_bTurnCardCount!=0)?true:false);
					}
		
					//桌面设置
					if (m_bTurnCardCount!=0)
					{
						var wViewChairID:uint=SwitchViewChairID(pStatusPlay.wLastOutUser);
						GetPeerGameView().m_UserCardControl[wViewChairID].SetCardData(m_bTurnCardData,m_bTurnCardCount);
					}
		
					//设置定时器
					var wTimeCount:uint=30;
					if (GetPeerGameView().m_btPassCard.IsWindowEnabled()) wTimeCount=10;
					SetGameTimer(pStatusPlay.wCurrentUser,IDI_OUT_CARD,wTimeCount);
		
					//设置扑克
					if (IsLookonMode()==false) 
					{
						GetPeerGameView().m_HandCardControl.SetPositively(true);
						GetPeerGameView().m_HandCardControl.SetDisplayFlag(true);
					}
		
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
			ASSERT(wDataSize==CMD_S_GameStart.sizeof_CMD_S_GameStart);
			if (wDataSize!=CMD_S_GameStart.sizeof_CMD_S_GameStart) return false;
		
			//消息处理
			var pGameStart:CMD_S_GameStart=CMD_S_GameStart.readData(pBuffer);
		
			//设置变量
			m_bTurnCardCount=0;
			m_bTurnOutType=CT_ERROR;
			m_wBankerUser=pGameStart.wBankerUser;
			ZeroArray(m_bTurnCardData);
		
			//设置界面
			for (var i:uint=0;i<GAME_PLAYER;i++)
			{
				GetPeerGameView().SetPassFlag(i,false);
				GetPeerGameView().m_UserCardControl[i].SetCardData(null,0);
			}
			if (IsLookonMode()==true)
			{
				GetPeerGameView().m_ScoreView.ShowWindow(false);
				GetPeerGameView().m_LeaveCardControl[0].SetCardData(null,0);
				GetPeerGameView().m_LeaveCardControl[1].SetCardData(null,0);
				GetPeerGameView().m_LeaveCardControl[2].SetCardData(null,0);
			}
		
			//设置扑克
			m_bHandCardCount=CountArray(pGameStart.cbCardData);
			CopyArray(m_bHandCardData,pGameStart.cbCardData,pGameStart.cbCardData.length);
			GetPeerGameView().m_HandCardControl.SetCardData(pGameStart.cbCardData,m_bHandCardCount);
		
			//扑克数目
			for (var i:uint=0;i<GAME_PLAYER;i++)
			{
				m_bCardCount[i]=CountArray(pGameStart.cbCardData);
				GetPeerGameView().SetCardCount(SwitchViewChairID(i),m_bCardCount[i]);
			}
			var bCardData:Array = GenEmptyCardData(13);
			UpdateLeaveCardControl(bCardData);
		
			//控制设置
			var bPlayerMode:Boolean=(IsLookonMode()==false);
			GetPeerGameView().m_HandCardControl.SetPositively(bPlayerMode);
			GetPeerGameView().m_HandCardControl.SetDisplayFlag(bPlayerMode);
		
			//当前玩家
			if ((IsLookonMode()==false)&&(pGameStart.wCurrentUser==GetMeChairID()))
			{
				ActiveGameFrame();
				GetPeerGameView().m_btOutCard.ShowWindow(true);
				GetPeerGameView().m_btOutCard.EnableWindow(false);
				GetPeerGameView().m_btPassCard.EnableWindow(false);
				GetPeerGameView().m_btPassCard.ShowWindow(true);
				GetPeerGameView().m_btOutPrompt.ShowWindow(true);
			}
		
			//设置定时器
			var wTimeCount:uint=30;
			if (pGameStart.wCurrentUser!=INVALID_CHAIR) SetGameTimer(pGameStart.wCurrentUser,IDI_OUT_CARD,wTimeCount);
		
			//环境设置
			PlayGameSound("GAME_START");
		
			return true;
		}
		
		//用户出牌
		protected function OnSubOutCard(pBuffer:ByteArray,wDataSize:uint):Boolean
		{
			//变量定义
			var pOutCard:CMD_S_OutCard=CMD_S_OutCard.readData(pBuffer);
			var wHeadSize:uint=CMD_S_OutCard.sizeof_CMD_S_OutCard -CMD_S_OutCard.sizeof_bCardData;
		
			//效验数据
			ASSERT(wDataSize>=wHeadSize);
			if (wDataSize<wHeadSize) return false;
			ASSERT(wDataSize==(wHeadSize+pOutCard.bCardCount));
			if (wDataSize!=(wHeadSize+pOutCard.bCardCount)) return false;
		
			//删除定时器
			KillTimer(IDI_MOST_CARD);
			KillGameTimer(IDI_OUT_CARD);
		
			//界面设置
			var wOutViewChairID:uint=SwitchViewChairID(pOutCard.wOutCardUser);
			m_bCardCount[pOutCard.wOutCardUser]-=pOutCard.bCardCount;
			GetPeerGameView().SetCardCount(wOutViewChairID,m_bCardCount[pOutCard.wOutCardUser]);
			var bCardData:Array = GenEmptyCardData(13);
			UpdateLeaveCardControl(bCardData);
			
			//出牌设置
			if ((IsLookonMode()==true)||(GetMeChairID()!=pOutCard.wOutCardUser))
			{
				GetPeerGameView().m_UserCardControl[wOutViewChairID].SetCardData(pOutCard.bCardData,pOutCard.bCardCount);
			}
		
			//清理桌面
			if (m_bTurnCardCount==0)
			{
				for (var i:uint=0;i<GAME_PLAYER;i++) 
				{
					if (i==pOutCard.wOutCardUser) continue;
					GetPeerGameView().m_UserCardControl[SwitchViewChairID(i)].SetCardData(null,0);
				}
				GetPeerGameView().SetPassFlag(INVALID_CHAIR,false);
			}
		
			//记录出牌
			m_bTurnCardCount=pOutCard.bCardCount;
			m_bTurnOutType=m_GameLogic.GetCardType(pOutCard.bCardData,pOutCard.bCardCount);
			CopyArray(m_bTurnCardData,pOutCard.bCardData,pOutCard.bCardCount);
		
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
		
			//最大判断
			if (pOutCard.wCurrentUser==pOutCard.wOutCardUser)
			{
				//设置变量
				m_bTurnCardCount=0;
				m_bTurnOutType=CT_ERROR;
				m_wMostUser=pOutCard.wCurrentUser;
				ZeroArray(m_bTurnCardData);
		
				//设置界面
				for (var i:uint=0;i<GAME_PLAYER;i++)
				{
					if (i!=pOutCard.wOutCardUser)
					{
						var wViewChairID:uint=SwitchViewChairID(i);
						GetPeerGameView().SetPassFlag(wViewChairID,true);
						GetPeerGameView().m_UserCardControl[wViewChairID].SetCardData(null,0);
					}
				}
		
				//环境设置
				SetTimer(IDI_MOST_CARD,3000);
				PlayGameSound("MOST_CARD");
		
				return true;
			}
		
			//播放声音
			if ((IsLookonMode()==true)||(GetMeChairID()!=pOutCard.wOutCardUser)) PlayGameSound("OUT_CARD");
		
			//玩家设置
			if (pOutCard.wCurrentUser!=INVALID_CHAIR)
			{
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
				GetPeerGameView().m_btOutPrompt.ShowWindow(true);
				GetPeerGameView().m_btOutCard.EnableWindow((VerdictOutCard()==true)?true:false);
			}
		
			//设置定时器
			if (pOutCard.wCurrentUser!=INVALID_CHAIR)
			{
				var wTimeCount:uint=30;
				if (GetPeerGameView().m_btPassCard.IsWindowEnabled()) wTimeCount=30;
				SetGameTimer(pOutCard.wCurrentUser,IDI_OUT_CARD,wTimeCount);
			}
		
			return true;
		}
		
		//放弃出牌
		protected function OnSubPassCard(pBuffer:ByteArray,wDataSize:uint):Boolean
		{
			//效验数据
			if (wDataSize!=CMD_S_PassCard.sizeof_CMD_S_PassCard) return false;
			var pPassCard:CMD_S_PassCard=CMD_S_PassCard.readData(pBuffer);
		
			//删除定时器
			KillGameTimer(IDI_OUT_CARD);
		
			//玩家设置
			if ((IsLookonMode()==true)||(pPassCard.wPassUser!=GetMeChairID()))
			{
				var wViewChairID:uint=SwitchViewChairID(pPassCard.wPassUser);
				GetPeerGameView().SetPassFlag(wViewChairID,true);
				GetPeerGameView().m_UserCardControl[wViewChairID].SetCardData(null,0);
			}
		
			//一轮判断
			if (pPassCard.bNewTurn==true)
			{
				m_bTurnCardCount=0;
				m_bTurnOutType=CT_ERROR;
				ZeroArray(m_bTurnCardData);
			}
		
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
				GetPeerGameView().m_btOutPrompt.ShowWindow(true);
				GetPeerGameView().m_btPassCard.EnableWindow((m_bTurnCardCount>0)?true:false);
				GetPeerGameView().m_btOutCard.EnableWindow((VerdictOutCard()==true)?true:false);
			}
		
			//播放声音
			if ((IsLookonMode()==true)||(pPassCard.wPassUser!=GetMeChairID()))	PlayGameSound("OUT_CARD");
		
			//设置时间
			if (m_bTurnCardCount!=0)
			{
				var wTimeCount:uint=30;
				if (GetPeerGameView().m_btPassCard.IsWindowEnabled()) wTimeCount=30;
				SetGameTimer(pPassCard.wCurrentUser,IDI_OUT_CARD,wTimeCount);
			}
			else SetGameTimer(pPassCard.wCurrentUser,IDI_OUT_CARD,30);
		
			return true;
		}
		
		//游戏结束
		protected function OnSubGameEnd(pBuffer:ByteArray,wDataSize:uint):Boolean
		{
			//效验数据
			ASSERT(wDataSize==CMD_S_GameEnd.sizeof_CMD_S_GameEnd);
			if (wDataSize!=CMD_S_GameEnd.sizeof_CMD_S_GameEnd) return false;
		
			//消息处理
			var pGameEnd:CMD_S_GameEnd=CMD_S_GameEnd.readData(pBuffer);
		
			//设置状态
			SetGameStatus(GS_RF_FREE);
		
			//删除定时器
			KillTimer(IDI_MOST_CARD);
			KillGameTimer(IDI_OUT_CARD);
		
			//隐藏控件
			GetPeerGameView().m_btOutCard.ShowWindow(false);
			GetPeerGameView().m_btPassCard.ShowWindow(false);
			GetPeerGameView().m_btOutPrompt.ShowWindow(false);
		
			//禁用控件
			GetPeerGameView().m_btOutCard.EnableWindow(false);
			GetPeerGameView().m_btPassCard.EnableWindow(false);
		
			//设置积分
			for (var i:uint=0;i<GAME_PLAYER;i++)
			{
				var pUserData:tagUserInfoHead=GetUserInfo(i);
				GetPeerGameView().m_ScoreView.SetGameScore(i,pGameEnd.fGameScore[i]);
				GetPeerGameView().m_fAllTurnScore[SwitchViewChairID(i)]+=pGameEnd.fGameScore[i];
				GetPeerGameView().m_fLastTurnScore[SwitchViewChairID(i)]=pGameEnd.fGameScore[i];
			}
			GetPeerGameView().m_ScoreView.SetMeChairID(GetMeChairID());
			GetPeerGameView().m_ScoreView.ShowWindow(true);
		
			//设置扑克
			var bCardPos:uint=0;
			for (var i:uint=0;i<GAME_PLAYER;i++)
			{
				//设置扑克
				var wViewChairID:uint=SwitchViewChairID(i);
				if (wViewChairID!=2)
				{
					if (wViewChairID>2) 
					{
					  var tmpArr:Array = new Array(pGameEnd.bCardCount[i]);
					  CopyArray(tmpArr, pGameEnd.bCardData, pGameEnd.bCardCount[i], 0, bCardPos);
						GetPeerGameView().m_LeaveCardControl[wViewChairID-1].SetCardData(tmpArr,pGameEnd.bCardCount[i]);
					}
					else
					{
					 	var tmpArr:Array = new Array(pGameEnd.bCardCount[i]);
					  CopyArray(tmpArr, pGameEnd.bCardData, pGameEnd.bCardCount[i], 0, bCardPos);
						 GetPeerGameView().m_LeaveCardControl[wViewChairID].SetCardData(tmpArr,pGameEnd.bCardCount[i]);
					}
				}
		
				//界面设置
				bCardPos+=pGameEnd.bCardCount[i];
				if (pGameEnd.bCardCount[i]!=0)
				{
					GetPeerGameView().SetPassFlag(wViewChairID,false);
					GetPeerGameView().m_UserCardControl[wViewChairID].SetCardData(null,0);
				}
			}
		
			//显示扑克
			if (IsLookonMode()==true) GetPeerGameView().m_HandCardControl.SetDisplayFlag(true);
		
			//播放声音
			var wMeChairID:uint=GetMeChairID();
			var fMeScore:Number=pGameEnd.fGameScore[GetMeChairID()];
			if (fMeScore>0) PlayGameSound("GAME_WIN");
			else if (fMeScore<0) PlayGameSound("GAME_LOST");
			else PlayGameSound("GAME_END");
		
			//设置界面
			if (IsLookonMode()==false)
			{
				GetPeerGameView().m_btStart.ShowWindow(true);
				SetGameTimer(GetMeChairID(),IDI_EXIT_GAME,30);
			}
		
			return true;
		}
		
		//出牌判断
		protected function VerdictOutCard():Boolean
		{
			var i:uint =0;
			//状态判断
			if (GetPeerGameView().m_btOutCard.IsWindowVisible()==false) return false;
		
			//获取扑克
			var bCardData:Array = new Array(13);
			var bShootCount:uint=GetPeerGameView().m_HandCardControl.GetShootCard(bCardData,CountArray(bCardData));
		
			//出牌判断
			if (bShootCount>0)
			{
				//排列扑克
				m_GameLogic.SortCardList(bCardData,bShootCount);
		
				//分析类型
				var bCardType:uint=m_GameLogic.GetCardType(bCardData,bShootCount);
		
				//类型判断
				if (bCardType==CT_ERROR) return false;
		
				//首出判断
				if ((m_wBankerUser==GetMeChairID())&&(m_bCardCount[m_wBankerUser]==13))
				{
					//方三判断
					for (i=0;i<bShootCount;i++)
					{
						if (bCardData[i]==0x03) break;
					}
					if (i==bShootCount) return false;
				}
		
				//跟牌判断
				if (m_bTurnCardCount==0) return true;
				return m_GameLogic.CompareCard4(bCardData,m_bTurnCardData,bShootCount,m_bTurnCardCount);
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
				GetPeerGameView().m_btOutPrompt.ShowWindow(false);
		
				//首出判断
				var cbOutCard:uint=m_bHandCardData[m_bHandCardCount-1];
				if ((m_wBankerUser==GetMeChairID())&&(m_bCardCount[m_wBankerUser]==13)) cbOutCard=0x03;
		
				//发送数据
				var OutCard:CMD_C_OutCard = new CMD_C_OutCard;
				OutCard.bCardCount=1;
				OutCard.bCardData[0]=cbOutCard;
				var sendData:ByteArray = OutCard.toByteArray();
				SendGameData(SUB_C_OUT_CART,
				sendData,
				CMD_C_OutCard.sizeof_CMD_C_OutCard-
				CMD_C_OutCard.sizeof_bCardData+OutCard.bCardCount);
		
				//预先处理
				PlayGameSound("OUT_CARD");
				GetPeerGameView().m_UserCardControl[2].SetCardData(OutCard.bCardData,OutCard.bCardCount);
		
				//预先删除
				var bSourceCount:uint=m_bHandCardCount;
				m_bHandCardCount-=OutCard.bCardCount;
				m_GameLogic.RemoveCard(OutCard.bCardData,OutCard.bCardCount,m_bHandCardData,bSourceCount);
				GetPeerGameView().m_HandCardControl.SetCardData(m_bHandCardData,m_bHandCardCount);
			}
			else OnPassCard(0,0);
		
			return true;
		}
		//定时器消息
		override protected function OnTimer( nIDEvent:int):Boolean
		{
			if ((nIDEvent==IDI_MOST_CARD)&&(m_wMostUser!=INVALID_CHAIR))
			{
				//变量定义
				var wCurrentUser:uint=m_wMostUser;
				m_wMostUser=INVALID_CHAIR;
		
				//删除定时器
				KillTimer(IDI_MOST_CARD);
		
				//设置界面
				GetPeerGameView().SetPassFlag(INVALID_CHAIR,false);
				for (var i:uint=0;i<GAME_PLAYER;i++) GetPeerGameView().m_UserCardControl[i].SetCardData(null,0);
		
				//玩家设置
				if ((IsLookonMode()==false)&&(wCurrentUser==GetMeChairID()))
				{
					ActiveGameFrame();
					GetPeerGameView().m_btOutCard.ShowWindow(true);
					GetPeerGameView().m_btPassCard.ShowWindow(true);
					GetPeerGameView().m_btOutPrompt.ShowWindow(true);
					GetPeerGameView().m_btPassCard.EnableWindow(false);
					GetPeerGameView().m_btOutCard.EnableWindow((VerdictOutCard()==true)?true:false);
				}
		
				//设置时间
				var wTimeCount:uint=30;
				if (GetPeerGameView().m_btPassCard.IsWindowEnabled()) wTimeCount=30;
				SetGameTimer(wCurrentUser,IDI_OUT_CARD,wTimeCount);
		
				
			}
			return true;
		}
		//开始按钮
		override protected function OnStart(wParam:uint, lParam:uint):uint
		{
			//设置变量
			m_wTimeOutCount=0;
			m_bHandCardCount=0;
			m_bTurnCardCount=0;
			m_bTurnOutType=CT_ERROR;
			m_wMostUser=INVALID_CHAIR;
			ZeroArray(m_bHandCardData);
			ZeroArray(m_bTurnCardData);
		
			//设置界面
			KillGameTimer(IDI_EXIT_GAME);
			GetPeerGameView().SetCardCount(INVALID_CHAIR,0);
			GetPeerGameView().SetPassFlag(INVALID_CHAIR,false);
		
			//隐藏控件
			GetPeerGameView().m_btStart.ShowWindow(false);
			GetPeerGameView().m_ScoreView.ShowWindow(false);
		
			//设置扑克
			GetPeerGameView().m_HandCardControl.SetCardData(null,0);
			GetPeerGameView().m_HandCardControl.SetPositively(false);
			GetPeerGameView().m_LeaveCardControl[0].SetCardData(null,0);
			GetPeerGameView().m_LeaveCardControl[1].SetCardData(null,0);
			GetPeerGameView().m_LeaveCardControl[2].SetCardData(null,0);
			for (var i:uint=0;i<GAME_PLAYER;i++) GetPeerGameView().m_UserCardControl[i].SetCardData(null,0);
		
			//发送消息
			SendUserReady(null,0);
		
			return 0;
		}
		
		//出牌消息
		protected function OnOutCard(wParam:uint, lParam:uint):uint
		{
			//状态判断
			if ((GetPeerGameView().m_btOutCard.IsWindowEnabled()==false)
				||(GetPeerGameView().m_btOutCard.IsWindowVisible()==false)) return 0;
		
			//设置界面
			KillGameTimer(IDI_OUT_CARD);
			GetPeerGameView().m_btOutCard.ShowWindow(false);
			GetPeerGameView().m_btOutCard.EnableWindow(false);
			GetPeerGameView().m_btPassCard.ShowWindow(false);
			GetPeerGameView().m_btPassCard.EnableWindow(false);
			GetPeerGameView().m_btOutPrompt.ShowWindow(false);
			
			//发送数据
			var OutCard:CMD_C_OutCard = new CMD_C_OutCard;
			OutCard.bCardCount=GetPeerGameView().m_HandCardControl.GetShootCard(OutCard.bCardData,CountArray(OutCard.bCardData));
			m_GameLogic.SortCardList(OutCard.bCardData,OutCard.bCardCount);
			var sendData:ByteArray = OutCard.toByteArray();
			SendGameData(SUB_C_OUT_CART,sendData,
			CMD_C_OutCard.sizeof_CMD_C_OutCard-
			CMD_C_OutCard.sizeof_bCardData+
			OutCard.bCardCount);
		
			//预先显示
			PlayGameSound("OUT_CARD");
			GetPeerGameView().m_UserCardControl[2].SetCardData(OutCard.bCardData,OutCard.bCardCount);
		
			//预先删除
			var bSourceCount:uint=m_bHandCardCount;
			m_bHandCardCount-=OutCard.bCardCount;
			m_GameLogic.RemoveCard(OutCard.bCardData,OutCard.bCardCount,m_bHandCardData,bSourceCount);
			GetPeerGameView().m_HandCardControl.SetCardData(m_bHandCardData,m_bHandCardCount);
		
			return 0;
		}
		
		//放弃出牌
		protected function OnPassCard(wParam:uint, lParam:uint):uint
		{
			//状态判断
			if (GetPeerGameView().m_btPassCard.IsWindowEnabled()==false) return 0;
		
			//设置界面
			KillGameTimer(IDI_OUT_CARD);
			GetPeerGameView().m_btOutCard.ShowWindow(false);
			GetPeerGameView().m_btOutCard.EnableWindow(false);
			GetPeerGameView().m_btPassCard.ShowWindow(false);
			GetPeerGameView().m_btPassCard.EnableWindow(false);
			GetPeerGameView().m_btOutPrompt.ShowWindow(false);
		
			//发送数据
			SendGameData(SUB_C_PASS_CARD);
		
			//预先显示
			GetPeerGameView().m_UserCardControl[2].SetCardData(null,0);
			GetPeerGameView().SetPassFlag(2,true);
			PlayGameSound("OUT_CARD");
			GetPeerGameView().m_HandCardControl.SetCardData(m_bHandCardData,m_bHandCardCount);
		
			return 0;
		}
		
		//出牌提示
		protected function OnOutPrompt(wParam:uint, lParam:uint):uint
		{
			GetPeerGameView().m_HandCardControl.ShootAllCard(false);
			
			var	cbCardCount:uint = 0;//扑克数目
			var	cbResultCard:Array = new Array(13);			//结果扑克
			ZeroArray(cbResultCard);
			//先出牌者
			if (m_bTurnCardCount==0)
			{		
				//首出判断
				var cbOutCard:uint=m_bHandCardData[m_bHandCardCount-1];
				if ((m_wBankerUser==GetMeChairID())&&(m_bCardCount[m_wBankerUser]==13))
					cbOutCard=0x03;
				//查找牌索引
				for(var i:uint = 0; i < m_bHandCardCount; i ++)
				{
					if(m_bHandCardData[i] == cbOutCard)
					{
						cbCardCount = 1;
						cbResultCard[0] = i;
						break;
					}
				}
				
			}
			else
			{
				cbCardCount = m_GameLogic.SearchOutCard(m_bHandCardData,
														m_bHandCardCount,
														m_bTurnCardData,
														m_bTurnCardCount,
														m_bTurnOutType,
														cbResultCard);
			}
			if(cbCardCount > 0)
			{
				GetPeerGameView().m_HandCardControl.SetShootCard(cbResultCard,
															 cbCardCount);
			
				//设置控件
				var bOutCard:Boolean=VerdictOutCard();
				GetPeerGameView().m_btOutCard.EnableWindow(bOutCard?true:false);
			}
			else
			{
				OnPassCard(0,0);
			}

			return 0;
		}
		
		//右键扑克
		protected function OnLeftHitCard(wParam:uint, lParam:uint):uint
		{
			//设置控件
			var bOutCard:Boolean=VerdictOutCard();
			GetPeerGameView().m_btOutCard.EnableWindow(bOutCard?true:false);
		
			return 0;
		}
		
		//左键扑克
		protected function OnRightHitCard(wParam:uint, lParam:uint):uint
		{
			//用户出牌
			OnOutCard(0,0);
		
			return 0;
		}
		//////////////////////////////////////
		//辅助函数
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
				case GameViewEvent.nGVM_PASS_CARD :
					{
						OnPassCard(e.m_nWParam,e.m_nLParam);
						return;



					};
				case GameViewEvent.nGVM_OUT_PROMPT :
					{
						OnOutPrompt(e.m_nWParam,e.m_nLParam);
						return;



					};
				case GameViewEvent.nGVM_LEFT_HIT_CARD:
					{
						OnLeftHitCard(e.m_nWParam,e.m_nLParam);
						return;



					};
				
			}
		}
		private function GenEmptyCardData(nCardCount:int):Array
		{
			var result:Array = new Array(nCardCount);
			ZeroArray(result, 0);
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
	}
}
include "../../Common/Memory.as"
include "../../Net/GlobalDef.as"
include "../../Net/GlobalFrame.as"
include "./Common/CMD_Hoe.as"
include "./Common/GameLogicDef.as"