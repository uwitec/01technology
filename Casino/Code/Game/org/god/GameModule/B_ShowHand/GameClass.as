package 
{
	import flash.display.MovieClip;
	import flash.utils.ByteArray;
	import flash.geom.Point;

	import org.god.IGameFrame.*;
	import org.god.GameModule.Common.*;
	import org.god.Net.*;
	import org.god.GameModule.B_ShowHand.Common.*;

	public class GameClass extends GameBaseClass
	{
		//定时器标识
		protected static  var IDI_EXIT_GAME:uint=200;//离开游戏定时器
		protected static  var IDI_GIVE_UP:uint=201;//放弃定时器

		//游戏变量
		protected var m_fTurnMaxGold:Number;//最大下注
		protected var m_fTurnBasicGold:Number;//跟注数目
		protected var m_bPlayStatus:Array=new Array(4);//游戏状态

		//辅助变量
		protected var m_bShowHand:Boolean;//是否梭哈
		protected var m_bMeAddGold:Boolean;//是否下注
		protected var m_fGoldShow:Number=0;//加注筹码
		protected var m_szName:Array=new Array(GAME_PLAYER);//玩家名字
		protected var b_UserFlag:Array=new Array(GAME_PLAYER);//玩家是否能加注


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
			for(var i:uint = 0; i < m_szName.length; i ++)
			{
				m_szName[i] = new String;
			}
			//游戏变量
			m_fTurnMaxGold=0;
			m_fTurnBasicGold=0;
			ZeroArray(m_bPlayStatus);

			//辅助变量
			m_fGoldShow=0;
			m_bShowHand=false;
			m_bMeAddGold=false;
			ZeroArray(m_szName, "");
			ZeroArray(b_UserFlag, false);
			GetPeerGameView().addEventListener(GameViewEvent.strGameViewEvent,OnGameViewEevent);

			AddGameSound("GAME_WIN",SOUND_GAME_WIN);
			AddGameSound("GAME_LOST",SOUND_GAME_LOST);
			AddGameSound("GAME_WARN",SOUND_GAME_WARN);
			AddGameSound("ADD_SCORE",SOUND_ADD_SCORE);
			AddGameSound("GAME_START",SOUND_GAME_START);
			AddGameSound("GAME_END",SOUND_GAME_END);
			AddGameSound("GIVE_UP",SOUND_GIVE_UP);

			return true;
		}
		//重置状态
		override public  function DestroyGameClass():void
		{
			GetPeerGameView().removeEventListener(GameViewEvent.strGameViewEvent,OnGameViewEevent);

			for(var i:uint = 0; i < m_szName.length; i ++)
			{
				m_szName[i] = null;
			}
			m_szName = null;
			m_bPlayStatus = null;
			b_UserFlag    = null;

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
				case IDI_EXIT_GAME:		//离开游戏定时器
				{
					if (nElapse==0)
					{
						OnExit(0,0);
						return false;
					}
					if (nElapse<=10) PlayGameSound("GAME_WARN");
		
					return true;
				}
			case IDI_GIVE_UP:			//放弃定时器
				{
					var wViewChairID:uint=SwitchViewChairID(wChairID);
					if (nElapse==0)
					{
						if ((IsLookonMode()==false)&&(wViewChairID==2)) OnGiveUp(1,1);
						return false;
					}
					if ((nElapse<=10)&&(wViewChairID==2)&&(IsLookonMode()==false)) PlayGameSound("GAME_WARN");
		
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
				case SUB_S_GAME_START:	//游戏开始
				{
					return OnSubGameStart(pBuffer,wDataSize);
				}
			case SUB_S_ADD_GOLD:	//用户下注
				{
					return OnSubAddGold(pBuffer,wDataSize);
				}
			case SUB_S_GIVE_UP:		//用户放弃
				{
					return OnSubGiveUp(pBuffer,wDataSize);
				}
			case SUB_S_SEND_CARD:	//发牌消息
				{
					return OnSubSendCard(pBuffer,wDataSize);
				}
			case SUB_S_GAME_END:	//游戏结束
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
				case GS_FREE:		//空闲状态
				{
					//效验数据
					if (wDataSize!=CMD_S_StatusFree.sizeof_CMD_S_StatusFree) return false;
					var pStatusFree:CMD_S_StatusFree=CMD_S_StatusFree.readData(pBuffer);
		
					//辅助变量
					m_bShowHand=false;
					m_bMeAddGold=false;
		
					//设置界面
					m_fGoldShow=0;
					m_fTurnBasicGold=0;
					GetPeerGameView().SetGoldTitleInfo(0,pStatusFree.fBasicGold);
		
					//设置控件
					if (IsLookonMode()==false)
					{
						GetPeerGameView().m_btStart.ShowWindow(true);
						GetPeerGameView().m_btStart.SetFocus();
					}
		
					return true;
				}
			case GS_PLAYING:	//游戏状态
				{
					//效验数据
					if (wDataSize!=CMD_S_StatusPlay.sizeof_CMD_S_StatusPlay) return false;
					var pStatusPlay:CMD_S_StatusPlay=CMD_S_StatusPlay.readData(pBuffer);
		
					//设置变量
					m_fTurnMaxGold=pStatusPlay.fTurnMaxGold;
					m_fTurnBasicGold=pStatusPlay.fTurnBasicGold;
					CopyArray(m_bPlayStatus,pStatusPlay.bPlayStatus);
		
					//设置界面
					for (var i:uint=0;i<GAME_PLAYER;i++)
					{
						var wViewStation:uint=SwitchViewChairID(i);
						var pUserData:tagUserInfoHead=GetUserInfo(i);
						if (pUserData!=null)
						{
							m_szName[i] = String(pUserData.szName);
							if (m_bPlayStatus[i]!=0) 
								GetPeerGameView().m_CardControl[wViewStation].SetCardData(pStatusPlay.bTableCardArray[i],pStatusPlay.bTableCardCount[i]);
						}
						GetPeerGameView().SetUserGoldInfo(wViewStation,false,pStatusPlay.fTableGold[2*i]);
						GetPeerGameView().SetUserGoldInfo(wViewStation,true,pStatusPlay.fTableGold[2*i+1]);
		
						//判断是否梭哈
						if (i==GetMeChairID())
						{
							m_bShowHand=((pStatusPlay.fMaxGold-pStatusPlay.fTableGold[2*i+1]-pStatusPlay.fTableGold[2*i])<=0);
						}
					}
					GetPeerGameView().SetGoldTitleInfo(pStatusPlay.fMaxGold,pStatusPlay.fBasicGold);
		
					//判断是否观看
					if ((IsLookonMode()==false)||(IsAllowLookon()==true)) GetPeerGameView().m_CardControl[2].AllowPositively(true);
		
					//判断是否自己下注
					if ((IsLookonMode()==false)&&(pStatusPlay.wCurrentUser==GetMeChairID()))
					{
						m_bMeAddGold=true;
						var wCardCount:uint=pStatusPlay.bTableCardCount[GetMeChairID()];
						GetPeerGameView().m_btFollow.ShowWindow(true);
						GetPeerGameView().m_btGiveUp.ShowWindow(true);
						GetPeerGameView().m_btAddGold.ShowWindow((m_bShowHand==false)?true:false);
						GetPeerGameView().m_btShowHand.ShowWindow(((wCardCount>4)&&(m_bShowHand==false))?true:false);
					}
					SetGameTimer(pStatusPlay.wCurrentUser,IDI_GIVE_UP,30);
		
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
			if (wDataSize!=CMD_S_GameStart.sizeof_CMD_S_GameStart) return false;
			var pGameStart:CMD_S_GameStart=CMD_S_GameStart.readData(pBuffer);
		
			//游戏变量
			m_fTurnMaxGold=pGameStart.fTurnMaxGold;
			m_fTurnBasicGold=pGameStart.fTurnBasicGold;
			ZeroArray(m_szName, "");
			ZeroArray(m_bPlayStatus);
		
			//辅助变量
			m_fGoldShow=0;
			m_bShowHand=false;
			m_bMeAddGold=false;
		
			//变量定义
			var wMeChairID:uint=GetMeChairID();
			var bLookonMode:Boolean=IsLookonMode();
			var fBaseGold:Number=GetPeerGameView().m_fBasicGold;
		
			//设置状态
			SetGameStatus(GS_PLAYING);
			GetPeerGameView().SetGoldTitleInfo(pGameStart.fMaxGold,fBaseGold);
		
			//设置界面
			var bCardData:Array = new Array(2);
			GetPeerGameView().m_ScoreView.ShowWindow(false);
			for (var i:uint=0;i<GAME_PLAYER;i++)
			{
				//变量定义
				var wViewChairID:uint=SwitchViewChairID(i);
				var pUserData:tagUserInfoHead=GetUserInfo(i);
		
				//设置界面
				if (pUserData!=null)
				{
					m_bPlayStatus[i]=1;
					bCardData[1]=pGameStart.bCardData[i];
					bCardData[0]=(wViewChairID==2)?pGameStart.bFundusCard:0;
					GetPeerGameView().m_CardControl[wViewChairID].SetCardData(bCardData,2);
					GetPeerGameView().SetUserGoldInfo(wViewChairID,false,0);
					GetPeerGameView().SetUserGoldInfo(wViewChairID,true,fBaseGold);
					m_szName[i] = String(pUserData.szName);
				}
				else
				{
					m_bPlayStatus[i]=0;
					GetPeerGameView().SetUserGoldInfo(wViewChairID,true,0);
					GetPeerGameView().SetUserGoldInfo(wViewChairID,false,0);
					GetPeerGameView().m_CardControl[wViewChairID].SetCardData(null,0);
				}
		
				//设置控件
				GetPeerGameView().m_CardControl[wViewChairID].ShowFirstCard(false);
			}
			if (bLookonMode==false) 
			{
				ActiveGameFrame();
				GetPeerGameView().m_CardControl[2].AllowPositively(true);
			}
		
			//设置控制
			if ((bLookonMode==false)&&(pGameStart.wCurrentUser==wMeChairID))
			{
				m_bMeAddGold=true;
				GetPeerGameView().m_btFollow.ShowWindow(true);
				GetPeerGameView().m_btGiveUp.ShowWindow(true);
				GetPeerGameView().m_btAddGold.ShowWindow(true);
			}
		
			//设置定时器
			SetGameTimer(pGameStart.wCurrentUser,IDI_GIVE_UP,30);
			PlayGameSound("GAME_START");
		
			return true;
		}
		
		//用户加注
		protected function OnSubAddGold(pBuffer:ByteArray,wDataSize:uint):Boolean
		{
			//效验数据
			if (wDataSize!=CMD_S_AddGold.sizeof_CMD_S_AddGold) return false;
			var pAddGold:CMD_S_AddGold=CMD_S_AddGold.readData(pBuffer);
		
			//变量定义
			var wMeChairID:uint=GetMeChairID();
			var wViewChairID:uint=SwitchViewChairID(pAddGold.wLastChairID);
		
			//处理数据
			m_fTurnBasicGold=pAddGold.fLastAddGold;
			
		
			GetPeerGameView().SetUserGoldInfo(wViewChairID,false,pAddGold.fLastAddGold);
			if ((IsLookonMode()==false)&&(pAddGold.wCurrentUser==wMeChairID))
			{
				//设置数据
				m_bMeAddGold=true;
				m_fGoldShow=GetPeerGameView().m_GoldView[4].GetGold();
				
		
				//获取扑克
				var wCardCount:uint=GetPeerGameView().m_CardControl[2].GetCardData(null,0);
		
				//自己下注
				ActiveGameFrame();
				GetPeerGameView().m_btFollow.ShowWindow(true);
				GetPeerGameView().m_btGiveUp.ShowWindow(true);
				
				GetPeerGameView().m_btAddGold.ShowWindow((m_bShowHand==false)?true:false);
				GetPeerGameView().m_btShowHand.ShowWindow((m_bShowHand==false)&&(wCardCount>4)?true:false);
			}
		
			//其他处理
			SetGameTimer(pAddGold.wCurrentUser,IDI_GIVE_UP,30);
			if (m_bPlayStatus[pAddGold.wCurrentUser]!=0) PlayGameSound("ADD_SCORE");
		
			return true;
		}
		
		//用户放弃
		protected function OnSubGiveUp(pBuffer:ByteArray,wDataSize:uint):Boolean
		{
			//效验数据
			if (wDataSize!=CMD_S_GiveUp.sizeof_CMD_S_GiveUp) return false;
			var pGiveUp:CMD_S_GiveUp=CMD_S_GiveUp.readData(pBuffer);
		
			//设置变量
			m_bPlayStatus[pGiveUp.wUserChairID]=0;
			if (pGiveUp.wUserChairID==GetMeChairID()) SetGameStatus(GS_FREE);
		
			//设置界面
			var bCard:Array = new Array(0,0,0,0,0);
			var wViewStation:uint=SwitchViewChairID(pGiveUp.wUserChairID);
			var bCount:uint=GetPeerGameView().m_CardControl[wViewStation].GetCardCount();
			GetPeerGameView().m_CardControl[wViewStation].AllowPositively(false);
			GetPeerGameView().m_CardControl[wViewStation].ShowFirstCard(false);
			GetPeerGameView().m_CardControl[wViewStation].SetCardData(bCard,bCount);
		
			//其他处理
			if (pGiveUp.wUserChairID==GetTimeChairID()) KillGameTimer(IDI_GIVE_UP);
			PlayGameSound("GIVE_UP");
		
			return true;
		}
		
		//发牌消息
		protected function OnSubSendCard(pBuffer:ByteArray,wDataSize:uint):Boolean
		{
			//效验数据
			if (wDataSize!=CMD_R_SendCard.sizeof_CMD_R_SendCard) return false;
			var pSendCard:CMD_R_SendCard=CMD_R_SendCard.readData(pBuffer);
		
			//设置变量
			m_fGoldShow=0;
			m_fTurnBasicGold=0;
			m_fTurnMaxGold=pSendCard.fMaxGold;
		
			//更新界面
			var bCardData:Array = new Array(5);
			var fUserTableGold:Number=0;
			for (var i:uint=0;i<GAME_PLAYER;i++)
			{
				//获取位置
				var wViewChairID:uint=SwitchViewChairID(i);
		
				//设置扑克
				if (m_bPlayStatus[i]!=0)
				{
					var wCardCount:uint=GetPeerGameView().m_CardControl[wViewChairID].GetCardData(bCardData,CountArray(bCardData));
					bCardData[wCardCount++]=pSendCard.bUserCard[i];
					GetPeerGameView().m_CardControl[wViewChairID].SetCardData(bCardData,wCardCount);
				}
		
				//设置金币
				fUserTableGold=GetPeerGameView().m_GoldView[wViewChairID*2].GetGold();
				fUserTableGold+=GetPeerGameView().m_GoldView[wViewChairID*2+1].GetGold();
				GetPeerGameView().SetUserGoldInfo(wViewChairID,false,0);
				GetPeerGameView().SetUserGoldInfo(wViewChairID,true,fUserTableGold);
			}
		
			//下注判断
			if ((IsLookonMode()==false)&&(pSendCard.wCurrentUser==GetMeChairID()))
			{
				ActiveGameFrame();
				m_bMeAddGold=true;
				GetPeerGameView().m_btFollow.ShowWindow(true);
				GetPeerGameView().m_btGiveUp.ShowWindow(true);
				GetPeerGameView().m_btAddGold.ShowWindow((m_bShowHand==false)?true:false);
				GetPeerGameView().m_btShowHand.ShowWindow(((GetPeerGameView().m_CardControl[2].GetCardData(null,0)>4)&&(m_bShowHand==false))?true:false);
			}
		
			//设置定时器
			SetGameTimer(pSendCard.wCurrentUser,IDI_GIVE_UP,30);
		
			//播放声音
			PlayGameSound("ADD_SCORE");
		
			return true;
		}
		
		//游戏结束
		protected function OnSubGameEnd(pBuffer:ByteArray,wDataSize:uint):Boolean
		{
			//效验参数
			if (wDataSize!=CMD_S_GameEnd.sizeof_CMD_S_GameEnd) return false;
			var pGameEnd:CMD_S_GameEnd=CMD_S_GameEnd.readData(pBuffer);
		
			//播放声音
			if (IsLookonMode()==false)
			{
				if (pGameEnd.fGameGold[GetMeChairID()]>0) PlayGameSound("GAME_WIN");
				else PlayGameSound("GAME_LOST");
			}
			else PlayGameSound("GAME_END");
		
			//游戏正常结束
			var bCardData:Array = new Array(5);
			GetPeerGameView().m_ScoreView.ResetScore();
			for (var i:uint=0;i<GAME_PLAYER;i++)
			{
				//设置信息
				if (pGameEnd.fGameGold[i]!=0)
				{
					GetPeerGameView().m_ScoreView.SetGameScore(i,pGameEnd.fGameGold[i]);
					if (pGameEnd.bUserCard[i]!=0)
					{
						var wViewStation:uint=SwitchViewChairID(i);
						var wCardCount:uint=GetPeerGameView().m_CardControl[wViewStation].GetCardData(bCardData,CountArray(bCardData));
						bCardData[0]=pGameEnd.bUserCard[i];
						GetPeerGameView().m_CardControl[wViewStation].SetCardData(bCardData,wCardCount);
						GetPeerGameView().m_CardControl[wViewStation].SortCardData();
						GetPeerGameView().m_CardControl[wViewStation].ShowFirstCard(true);
					}
				}
			}
			GetPeerGameView().m_ScoreView.SetMeChairID(GetMeChairID());
			GetPeerGameView().m_ScoreView.ShowWindow(true);
		
			//设置控件
			KillGameTimer(IDI_GIVE_UP);
			GetPeerGameView().m_btGiveUp.ShowWindow(false);
			GetPeerGameView().m_btFollow.ShowWindow(false);
			GetPeerGameView().m_btAddGold.ShowWindow(false);
			GetPeerGameView().m_btShowHand.ShowWindow(false);
			GetPeerGameView().m_GoldControl.ShowWindow(false);
		
			//设置状态
			SetGameStatus(GS_FREE);
		
			//游戏变量
			m_fTurnMaxGold=0;
			m_fTurnBasicGold=0;
			ZeroArray(m_bPlayStatus);
		
			//辅助变量
			m_fGoldShow=0;
			m_bShowHand=false;
			m_bMeAddGold=false;
			ZeroArray(m_szName, "");
		
			//设置界面
			if (IsLookonMode()==false)
			{
				GetPeerGameView().m_btStart.ShowWindow(true);
				GetPeerGameView().m_btStart.SetFocus();
				SetGameTimer(GetMeChairID(),IDI_EXIT_GAME,30);
			}
		
			return true;
		}
		
		//开始按钮
		override protected function OnStart(wParam:uint, lParam:uint):uint
		{
			//设置界面
			KillGameTimer(IDI_EXIT_GAME);
			GetPeerGameView().m_btStart.ShowWindow(false);
		
			//发送消息
			SendUserReady(null,0);
		
			return 0;
		}
		
		//加注按钮 
		protected function OnAddGold(wParam:uint, lParam:uint):uint
		{
			if ((m_bMeAddGold==true)&&(GetPeerGameView().m_GoldControl.IsWindowVisible()==false))
			{
				var fGold:Number=m_fTurnMaxGold;
				GetPeerGameView().SetUserGoldInfo(2,false,m_fTurnBasicGold);
				fGold-=GetPeerGameView().m_GoldView[4].GetGold();
				fGold-=GetPeerGameView().m_GoldView[5].GetGold();
				if(fGold < 0)
					fGold = 0;
				GetPeerGameView().m_GoldControl.SetMaxGold(fGold);
				GetPeerGameView().m_GoldControl.ShowWindow(true);
			}
		
			return 0;
		}
		
		//放弃按钮
		protected function OnGiveUp(wParam:uint, lParam:uint):uint
		{
			/*if ((wParam==0)&&(GetPeerGameView().m_GoldControl.IsWindowVisible()==true))
			{
				//放弃跟注
				GetPeerGameView().m_GoldControl.ShowWindow(false);
				GetPeerGameView().m_GoldControl.SetMaxGold(0);
				GetPeerGameView().SetUserGoldInfo(2,false,m_fGoldShow);
			}
			else*/
			{
				//放弃游戏
				m_bMeAddGold=false;
				KillGameTimer(IDI_GIVE_UP);
				GetPeerGameView().m_GoldControl.ShowWindow(false);
				GetPeerGameView().m_btAddGold.ShowWindow(false);
				GetPeerGameView().m_btFollow.ShowWindow(false);
				GetPeerGameView().m_btGiveUp.ShowWindow(false);
				GetPeerGameView().m_btShowHand.ShowWindow(false);
				GetPeerGameView().SetUserGoldInfo(2,false,m_fGoldShow);
		
				//发送消息
				SendGameData(SUB_C_GIVE_UP);
			}
			return 0;
		}
		
		//跟注按钮
		protected function OnFollow(wParam:uint, lParam:uint):uint
		{
			//获取金币
			var fGold:Number=GetPeerGameView().m_GoldView[4].GetGold()+GetPeerGameView().m_GoldControl.GetGold();
			fGold=__max(fGold,m_fTurnBasicGold);
		
			//设置变量
			m_bMeAddGold=false;
			if ((GetPeerGameView().m_fMaxGold-GetPeerGameView().m_GoldView[5].GetGold()-fGold)==0) m_bShowHand=true;
		
			//设置界面
			GetPeerGameView().m_GoldControl.SetMaxGold(0);
			GetPeerGameView().m_GoldControl.ShowWindow(false);
			GetPeerGameView().m_btFollow.ShowWindow(false);
			GetPeerGameView().m_btGiveUp.ShowWindow(false);
			GetPeerGameView().m_btAddGold.ShowWindow(false);
			GetPeerGameView().m_btShowHand.ShowWindow(false);
			GetPeerGameView().SetUserGoldInfo(2,false,fGold);
		
			//删除定时器
			KillGameTimer(IDI_GIVE_UP);
			
			//发送数据
			var AddPoint:CMD_C_AddGold = new CMD_C_AddGold;
			AddPoint.fGold=fGold;
			var sendData:ByteArray = AddPoint.toByteArray();
			SendGameData(SUB_C_ADD_GOLD,sendData,CMD_C_AddGold.sizeof_CMD_C_AddGold);
		
			return 0;
		}
		
		//梭哈按钮
		protected function OnShowHand(wParam:uint, lParam:uint):uint
		{
			var fGold:Number=m_fTurnMaxGold-GetPeerGameView().m_GoldView[5].GetGold();
			var dwMoney4:Number=GetPeerGameView().m_GoldView[4].GetGold();
			if ((fGold-dwMoney4)>0)
			{
				//CString strMessage;
				//strMessage.Format(TEXT("梭哈的下注是 %ld 筹码，确实要梭哈吗？"),fGold-dwMoney4);
				//if (IDYES!=AfxMessageBox(strMessage,MB_YESNO|MB_DEFBUTTON2|MB_ICONINFORMATION)) return 0;
			}
			if (m_bMeAddGold==true)
			{
				GetPeerGameView().SetUserGoldInfo(2,false,fGold);
				OnFollow(0,0);
			}
		
			return 0;
		}
		//////////////////////////////////////
		//辅助函数
		private function OnGameViewEevent(e:GameViewEvent):void
		{
			switch(e.m_nMsg)
			{
				case GameViewEvent.nGVM_GAME_START:
				{
					OnStart(e.m_nWParam,e.m_nLParam);
					return;
				}
				case GameViewEvent.nGVM_FOLLOW:
				{
					OnFollow(e.m_nWParam,e.m_nLParam);
					return;
				}
				case GameViewEvent.nGVM_GIVE_UP:
				{
					OnGiveUp(e.m_nWParam,e.m_nLParam);
					return;
				}
				case GameViewEvent.nGVM_ADD_GOLD:
				{
					OnAddGold(e.m_nWParam,e.m_nLParam);
					return;
				}
				case GameViewEvent.nGVM_SHOW_HAND:
				{
					OnShowHand(e.m_nWParam,e.m_nLParam);
					return;
				}
				
			}
		}
	}
}
include "../../Common/Memory.as"
include "../../Net/GlobalDef.as"
include "../../Net/GlobalFrame.as"
include "./Common/CMD_ShowHand.as"