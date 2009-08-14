package 
{
	import flash.display.MovieClip;
	import flash.utils.ByteArray;
	import flash.geom.Point;

	import org.god.IGameFrame.*;
	import org.god.GameModule.Common.*;
	import org.god.Net.*;
	import org.god.GameModule.B_GoldFlower.Common.*;
	//游戏类
	public class GameClass extends GameBaseClass
	{
		//定时器标识
		protected static  var IDI_EXIT_GAME:uint=200;//离开游戏定时器
		protected static  var IDI_GIVE_UP:uint=201;//放弃定时器

		//游戏变量
		protected var m_fMaxGold:Number;
		protected var m_fTurnMaxGold:Number;//最大下注
		protected var m_fTurnBasicGold:Number;//跟注数目
		protected var m_bPlayStatus:Array=new Array(GAME_PLAYER);//游戏状态
		protected var m_bKanPaiFlag:Array = new Array(GAME_PLAYER+1);        //是否看牌状态 ADD

		//辅助变量
		protected var m_wCurrentUser:uint;

		protected var m_bShowHand:Boolean;//是否梭哈
		protected var m_bMeAddGold:Boolean;//是否下注
		protected var m_fGoldShow:Number=0;//加注筹码
		protected var m_szName:Array=new Array(GAME_PLAYER);//玩家名字
		
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
				throw Error("GetPeerGameView GetPeerGameView() == null");
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
				m_szName[i] = new String;
			}
			//游戏变量
			m_wCurrentUser=0;
			m_fTurnMaxGold=0;
			m_fMaxGold = 0;
			m_fTurnBasicGold=0;
			ZeroArray(m_bPlayStatus);
			ZeroArray(m_bKanPaiFlag, false);//add

			//辅助变量
			m_fGoldShow=0;
			m_bShowHand=false;
			m_bMeAddGold=false;
			ZeroArray(m_szName, "");
			
			GetPeerGameView().addEventListener(GameViewEvent.strGameViewEvent,OnGameViewEevent);
				
			for (i=0;i<GAME_PLAYER;i++)
			{
			   GetPeerGameView().m_CardControl[i].SetCardData(null,0);
			}

			AddGameSound("GAME_START",SOUND_GAME_START);
			AddGameSound("GAME_WARN",SOUND_GAME_WARN);
			AddGameSound("ADD_SCORE",SOUND_ADD_SCORE);
			AddGameSound("GAME_END",SOUND_GAME_END);
			AddGameSound("GAME_WIN",SOUND_GAME_WIN);
			AddGameSound("GAME_LOST",SOUND_GAME_LOST);
			AddGameSound("GIVE_UP",SOUND_GIVE_UP);

			return true;
		}
		//销毁游戏类
		override public  function DestroyGameClass():void
		{
			GetPeerGameView().removeEventListener(GameViewEvent.strGameViewEvent,OnGameViewEevent);

			for(var i:uint = 0; i < m_szName.length; i ++)
			{
				m_szName[i] = null;
			}
			m_szName = null;
			m_bPlayStatus = null;


			super.DestroyGameClass();
		}
		////////////////////////////////
		//驱动接口IGameClassSink
		//获取窗口
		public function GetFrameMC():MovieClip
		{
			return this;
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
						if ((IsLookonMode()==false)&&(wViewChairID==3)) OnGiveUp(1,1);
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
			case SUB_S_KANPAI:     //看牌
				{
					return OnSubKanPai(pBuffer, wDataSize);
				}
			case SUB_S_NEWTURN:		//新局
				{
					return OnSubNewTurn(pBuffer, wDataSize);
				}
			default:
			break;
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
					m_fMaxGold = pStatusPlay.fMaxGold;
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
						var j:uint=0;														                   //add
						m_bMeAddGold=true;
						var wCardCount:uint=pStatusPlay.bTableCardCount[GetMeChairID()];
						GetPeerGameView().m_btFollow.ShowWindow(true); 
						//GetPeerGameView().m_btFollow.EnableWindow(false);                                   //add
						GetPeerGameView().m_btKanPai.ShowWindow(true);                                   //add
						
		///////////////////////////////////////////////////////////////////////////////// begin add
						for(var i:uint=0;i<GAME_PLAYER;i++)
						{
							if (m_bPlayStatus[i]!=0) j++;
						}
						if (j==2) 
						{
		           			 GetPeerGameView().m_btKaiPai.ShowWindow(true); 
							GetPeerGameView().m_btKaiPai.EnableWindow(true);
						}
						else
						{
							GetPeerGameView().m_btKaiPai.ShowWindow(true);
							GetPeerGameView().m_btKaiPai.EnableWindow(false);				
						}
		/////////////////////////////////////////////////////////////////////////////////// end add 
						GetPeerGameView().m_btGiveUp.ShowWindow(true);
						GetPeerGameView().m_btAddGold.ShowWindow((m_bShowHand==false)?true:false);
						GetPeerGameView().m_btGoldFlower.ShowWindow(((wCardCount>=4)&&(m_bShowHand==false))?true:false);
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
			m_fMaxGold	 = pGameStart.fMaxGold;
			m_fTurnMaxGold=pGameStart.fTurnMaxGold;
			m_fTurnBasicGold=pGameStart.fTurnBasicGold;
			ZeroArray(m_szName, "");
			ZeroArray(m_bPlayStatus);
			ZeroArray(m_bKanPaiFlag,false);                     //add
		
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
					GetPeerGameView().m_CardControl[SwitchViewChairID(i)].SetCardData(pGameStart.bCardData[i],3);              
					GetPeerGameView().SetUserGoldInfo(SwitchViewChairID(i),false,0);
					GetPeerGameView().SetUserGoldInfo(SwitchViewChairID(i),true,fBaseGold);
					m_szName[i] = String(pUserData.szName);
				}
				else
				{
					m_bPlayStatus[i]=0;
					GetPeerGameView().SetUserGoldInfo(SwitchViewChairID(i),true,0);
					GetPeerGameView().SetUserGoldInfo(SwitchViewChairID(i),false,0);
					GetPeerGameView().m_CardControl[SwitchViewChairID(i)].SetCardData(null,0);
				}
		
				
			}
			var mIndexBanker:uint;
			for(var i:uint=GAME_PLAYER-1;i>=0;i--)                   
			{
				mIndexBanker=(pGameStart.wCurrentUser+i)%GAME_PLAYER;
				if (m_bPlayStatus[mIndexBanker]!=0) break;
			}
			GetPeerGameView().GetCurrentUser(SwitchViewChairID(mIndexBanker)); 
			
			if (bLookonMode==false) 
			{
				ActiveGameFrame();
				GetPeerGameView().m_CardControl[2].AllowPositively(true);
			}
		
			//设置控制
			if ((bLookonMode==false)&&(pGameStart.wCurrentUser==wMeChairID))
			{
				var j:uint=0;                                                                                   //add
				m_bMeAddGold=true;
				GetPeerGameView().m_btFollow.ShowWindow(true); 
				//GetPeerGameView().m_btFollow.EnableWindow(false);										   //add
				GetPeerGameView().m_btGiveUp.ShowWindow(true);
				GetPeerGameView().m_btKanPai.ShowWindow(true);                                           //add
		
///////////////////////////////////////////////////////////////////////////////// begin add
				for(var i:uint=0;i<GAME_PLAYER;i++)
				{
					if (m_bPlayStatus[i]!=0) j++;
				}
				if (j==2) 
				{
           		 	GetPeerGameView().m_btKaiPai.ShowWindow(true); 
					GetPeerGameView().m_btKaiPai.EnableWindow(true);
				}
				else
				{
					GetPeerGameView().m_btKaiPai.ShowWindow(true);
					GetPeerGameView().m_btKaiPai.EnableWindow(false);				
				}
/////////////////////////////////////////////////////////////////////////////////// end add
				GetPeerGameView().m_btAddGold.ShowWindow(true);
			}
		
			//设置定时器
			SetGameTimer(pGameStart.wCurrentUser,IDI_GIVE_UP,30);
			PlayGameSound("GAME_START");
			
			for( i=0;i<GAME_PLAYER;i++)															       //add
	   			 GetPeerGameView().m_CardControl[i].SetShowCardFlag(false);                                  //add
			 m_wCurrentUser=pGameStart.wCurrentUser;          //add	
		
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
			m_fTurnBasicGold=pAddGold.fCurrentLessGold;
	    	GetPeerGameView().SetUserGoldInfo(wViewChairID,false,pAddGold.fLastAddGold);

			if ((IsLookonMode()==false)&&(pAddGold.wCurrentUser==wMeChairID))
			{
				//设置数据		
				m_bMeAddGold=true;
				m_fGoldShow=GetPeerGameView().m_GoldView[4].GetGold();

				//自己下注
				ActiveGameFrame();
				GetPeerGameView().m_btFollow.ShowWindow(true); 
				//GetPeerGameView().m_btFollow.EnableWindow(false);                                          //add

				GetPeerGameView().m_btGiveUp.ShowWindow(true);
				GetPeerGameView().m_btKanPai.ShowWindow(true);                                            //add			
				GetPeerGameView().m_btAddGold.ShowWindow((m_bShowHand==false)?true:false);
				///////////////////////////////////////////////////////////////////////// begin add
				var j:uint=0;
				for(var i:uint=0;i<GAME_PLAYER;i++)
				{
					if (m_bPlayStatus[i]!=0) j++;
				}
				if (j==2) 
				{
            		GetPeerGameView().m_btKaiPai.ShowWindow(true); 
					GetPeerGameView().m_btKaiPai.EnableWindow(true);
				}
				else
				{
					GetPeerGameView().m_btKaiPai.ShowWindow(true);
					GetPeerGameView().m_btKaiPai.EnableWindow(false);				
				}
			}
		
			//其他处理
			SetGameTimer(pAddGold.wCurrentUser,IDI_GIVE_UP,30);
			if (m_bPlayStatus[pAddGold.wCurrentUser]!=0) PlayGameSound("ADD_SCORE");
			m_wCurrentUser=pAddGold.wCurrentUser;     //add
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
			GetPeerGameView().m_CardControl[wViewStation].SetCardData(null,0);
		
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
			
			var fUserTableGold:Number=0;
			for (var i:uint=0;i<GAME_PLAYER;i++)
			{
				//获取位置
				var wViewChairID:uint=SwitchViewChairID(i);
		
				
				//设置金币
				fUserTableGold=GetPeerGameView().m_GoldView[wViewChairID*2].GetGold();
				fUserTableGold+=GetPeerGameView().m_GoldView[wViewChairID*2+1].GetGold();
				GetPeerGameView().SetUserGoldInfo(wViewChairID,false,0);
				GetPeerGameView().SetUserGoldInfo(wViewChairID,true,fUserTableGold);
			}
		
			//下注判断
			if ((IsLookonMode()==false)&&(pSendCard.wCurrentUser==GetMeChairID()))
			{
				var j:uint=0;															                       //add
				ActiveGameFrame();
				m_bMeAddGold=true;
				GetPeerGameView().m_btFollow.ShowWindow(true);
				GetPeerGameView().m_btGiveUp.ShowWindow(true);
				GetPeerGameView().m_btKanPai.ShowWindow(true);                                           //add
		
///////////////////////////////////////////////////////////////////////////////// begin add
				for( i=0;i<GAME_PLAYER;i++)
				{
					if (m_bPlayStatus[i]!=0) j++;
				}
				if (j==2) 
				{
          		   GetPeerGameView().m_btKaiPai.ShowWindow(true); 
					GetPeerGameView().m_btKaiPai.EnableWindow(true);
				}
				else
				{
					GetPeerGameView().m_btKaiPai.ShowWindow(true);
					GetPeerGameView().m_btKaiPai.EnableWindow(false);				
				}
/////////////////////////////////////////////////////////////////////////////////// end add
				GetPeerGameView().m_btAddGold.ShowWindow((m_bShowHand==false)?true:false);
				GetPeerGameView().m_btGoldFlower.ShowWindow(((GetPeerGameView().m_CardControl[2].GetCardData(null,0)>=4)&&(m_bShowHand==false))?true:false);

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
			var bCardData:Array = new Array(3);
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
			GetPeerGameView().m_btKanPai.ShowWindow(false);                                               //add
			GetPeerGameView().m_btKaiPai.ShowWindow(false); 
			GetPeerGameView().m_btGiveUp.ShowWindow(false);
			GetPeerGameView().m_btFollow.ShowWindow(false);
			GetPeerGameView().m_btAddGold.ShowWindow(false);
			GetPeerGameView().m_btGoldFlower.ShowWindow(false);
			GetPeerGameView().m_GoldControl.ShowWindow(false);
		
			//设置状态
			SetGameStatus(GS_FREE);
		
			//游戏变量
			m_fTurnMaxGold=0;
			m_fTurnBasicGold=0;
			ZeroArray(m_bPlayStatus);
			ZeroArray(m_bKanPaiFlag,false);             //add
		
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
			for(var i:uint=0;i<GAME_PLAYER;i++)
			{
	   			 GetPeerGameView().m_CardControl[i].SetShowCardFlag(true);
				m_bKanPaiFlag[i]=false;
			}
			GetPeerGameView().m_btKanPai.EnableWindow(true);
			m_bKanPaiFlag[GAME_PLAYER]=true;

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
				var wViewChairID:uint=this.SwitchViewChairID(GetMeChairID());
				GetPeerGameView().SetUserGoldInfo(wViewChairID,false,m_fTurnBasicGold);
				
				var fGold:Number=m_fTurnMaxGold-m_fTurnBasicGold;
				//剩余
				var wViewChairID:uint=this.SwitchViewChairID(GetMeChairID());
				var fUserTableGold:Number=GetPeerGameView().m_GoldView[wViewChairID*2].GetGold();
				fUserTableGold+=GetPeerGameView().m_GoldView[wViewChairID*2+1].GetGold();
				var fLeaveGold:Number = m_fMaxGold - fUserTableGold;
				if(fGold > fLeaveGold)
					fGold = fLeaveGold;
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
			//放弃游戏
			m_bMeAddGold=false;
			KillGameTimer(IDI_GIVE_UP);
			GetPeerGameView().m_GoldControl.ShowWindow(false);
			GetPeerGameView().m_btAddGold.ShowWindow(false);
			GetPeerGameView().m_btFollow.ShowWindow(false);
			GetPeerGameView().m_btGiveUp.ShowWindow(false);
			GetPeerGameView().m_btKanPai.ShowWindow(false);                                           //add
			GetPeerGameView().m_btKaiPai.ShowWindow(false);                                           //add
			GetPeerGameView().m_btGoldFlower.ShowWindow(false);
			GetPeerGameView().SetUserGoldInfo(2,false,m_fGoldShow);
		
			//发送消息
			SendGameData(SUB_C_GIVE_UP);
			
			return 0;
		}
		
		//跟注按钮
		protected function OnFollow(wParam:uint, lParam:uint):uint
		{
			//获取金币
			var wViewChairID:uint=this.SwitchViewChairID(GetMeChairID());
			var fGold:Number=GetPeerGameView().m_GoldView[wViewChairID * 2].GetGold() + 
							GetPeerGameView().m_GoldControl.GetGold();     //add

			fGold=__max(fGold,m_fTurnBasicGold);
			fGold=__min(fGold,m_fTurnMaxGold);
		
			//剩余
			var wViewChairID:uint=this.SwitchViewChairID(GetMeChairID());
			var fUserTableGold:Number=GetPeerGameView().m_GoldView[wViewChairID*2+1].GetGold();
			var fLeaveGold:Number = m_fMaxGold - fUserTableGold;
			if(fGold > fLeaveGold)
				fGold = fLeaveGold;
				
			//设置变量
			m_bMeAddGold=false;
			if ((GetPeerGameView().m_fMaxGold-GetPeerGameView().m_GoldView[5].GetGold()-fGold)==0) m_bShowHand=true;
			//设置界面
			
			GetPeerGameView().m_GoldControl.ShowWindow(false);
			GetPeerGameView().m_btFollow.ShowWindow(false);
			GetPeerGameView().m_btKanPai.ShowWindow(false);		          	                           //add
			GetPeerGameView().m_btGiveUp.ShowWindow(false);                                               //add
			GetPeerGameView().m_btKaiPai.ShowWindow(false);                               
			GetPeerGameView().m_btAddGold.ShowWindow(false);
			GetPeerGameView().m_btGoldFlower.ShowWindow(false);	       

		
			//删除定时器
			KillGameTimer(IDI_GIVE_UP);
			
			//发送数据
			var AddPoint:CMD_C_AddGold = new CMD_C_AddGold;
			AddPoint.fGold=fGold;
         
			var sendData:ByteArray = AddPoint.toByteArray();
			SendGameData(SUB_C_ADD_GOLD,sendData,CMD_C_AddGold.sizeof_CMD_C_AddGold);

			GetPeerGameView().m_GoldControl.SetGold(0);
			
			if(m_bKanPaiFlag[m_wCurrentUser])
				SendGameData(SUB_C_KANPAI);
			else 
				SendGameData(SUB_C_NOKANPAI);
				
			return 0;
		}
		
		//梭哈按钮
		protected function OnGoldFlower(wParam:uint, lParam:uint):uint
		{
			var fGold:Number=m_fTurnMaxGold-GetPeerGameView().m_GoldView[5].GetGold();
			var dwMoney4:Number=GetPeerGameView().m_GoldView[4].GetGold();
			if ((fGold-dwMoney4)>0)
			{
			}
			if (m_bMeAddGold==true)
			{
				var wViewChairID:uint=this.SwitchViewChairID(GetMeChairID());
				GetPeerGameView().SetUserGoldInfo(wViewChairID,false,fGold);
				OnFollow(0,0);
			}
		
			return 0;
		}
			
		public function OnKaiPai(wParam:uint, lParam:uint):uint                     //add 开牌按钮
		{
			var j:int = -1;
			for(var i:int=0;i<GAME_PLAYER;i++)
			{
				if (m_bPlayStatus[i]!=0 &&
						i != GetMeChairID())
				{
					j = i;
					break;
				}
			}
			if(j != -1)
			{
				var wViewChairID:uint=this.SwitchViewChairID(GetMeChairID());
				var fUserTableGold:Number=GetPeerGameView().m_GoldView[wViewChairID*2].GetGold();
				fUserTableGold+=GetPeerGameView().m_GoldView[wViewChairID*2+1].GetGold();
		
				wViewChairID=this.SwitchViewChairID(j);
				var fAnotherUserTableGold:Number=GetPeerGameView().m_GoldView[wViewChairID*2].GetGold();
				fAnotherUserTableGold+=GetPeerGameView().m_GoldView[wViewChairID*2+1].GetGold();

				var fTurnBasicGold:Number = fAnotherUserTableGold - fUserTableGold;
				fTurnBasicGold = __max(fTurnBasicGold, 0);
				GetPeerGameView().m_GoldControl.SetGold(fTurnBasicGold);
				OnFollow(wParam,lParam);
			}
			//发送消息
			SendGameData(SUB_C_KAIPAI);
		
			return 0;
		}
		//跟注
		public function ShowFollow(wParam:uint, lParam:uint):uint                 //add
		{
			GetPeerGameView().m_btFollow.EnableWindow(true);
			var j:uint=0;
			for(var i:uint=0;i<GAME_PLAYER;i++)
			{
				if (m_bPlayStatus[i]!=0) j++;
			}
			if (j==2) 
			{
		        GetPeerGameView().m_btKaiPai.ShowWindow(true); 
				GetPeerGameView().m_btKaiPai.EnableWindow(true);
			}
			
			return 0;
		}
		//看牌
		public function OnKanPai(wParam:uint, lParam:uint)	:uint				//add
		{
			m_bKanPaiFlag[m_wCurrentUser]=true;
			GetPeerGameView().m_btKanPai.EnableWindow(false);
			return 0;
		}
		//看牌消息
		public function OnSubKanPai(pBuffer:ByteArray,wDataSize:uint):Boolean         //add
		{
			if (wDataSize!=CMD_S_KANPAI.sizeof_CMD_S_KANPAI) return false;
			var pKanPai:CMD_S_KANPAI = CMD_S_KANPAI.readData(pBuffer);
			CopyArray(m_bKanPaiFlag,pKanPai.bKanPaiFlag);
			return true;
		}
		//新局
		public function OnSubNewTurn(pBuffer:ByteArray,wDataSize:uint):Boolean
		{
			if (wDataSize!=CMD_S_NewTurn.sizeof_CMD_S_NewTurn) return false;
			var pNewTurn:CMD_S_NewTurn = CMD_S_NewTurn.readData(pBuffer);
			//设置变量
			m_fMaxGold = pNewTurn.fMaxGold;
			m_fTurnMaxGold=pNewTurn.fTurnMaxGold;
			m_fTurnBasicGold=pNewTurn.fTurnBasicGold;

			//设置界面
			for (var i:uint=0;i<GAME_PLAYER;i++)
			{
				var wViewStation:uint=SwitchViewChairID(i);
			
				GetPeerGameView().SetUserGoldInfo(wViewStation,false,pNewTurn.fTableGold[2*i]);
				GetPeerGameView().SetUserGoldInfo(wViewStation,true,pNewTurn.fTableGold[2*i+1]);
		
				//判断是否梭哈
				if (i==GetMeChairID())
				{
					m_bShowHand=((pNewTurn.fMaxGold-pNewTurn.fTableGold[2*i+1]-pNewTurn.fTableGold[2*i])<=0);
				}
			}
			GetPeerGameView().SetGoldTitleInfo(pNewTurn.fMaxGold,pNewTurn.fBasicGold);

			return true;
		}
		//////////////////////////////////////
		//辅助函数
		private function OnGameViewEevent(e:GameViewEvent):void
		{
			switch(e.m_nMsg)
			{
				case GameViewEvent.nGVM_START://开始按钮
				{
					OnStart(e.m_nWParam,e.m_nLParam);
					return;
				}
				case GameViewEvent.nGVM_FOLLOW://跟注按钮
				{
					OnFollow(e.m_nWParam,e.m_nLParam);
					return;
				}
				case GameViewEvent.nGVM_GIVE_UP://放弃按钮
				{
					OnGiveUp(e.m_nWParam,e.m_nLParam);
					return;
				}
				case GameViewEvent.nGVM_ADD_GOLD://下注按钮
				{
					OnAddGold(e.m_nWParam,e.m_nLParam);
					return;
				}
				case GameViewEvent.nGVM_GOLDFLOWER://押全注
				{
					OnGoldFlower(e.m_nWParam,e.m_nLParam);
					return;
				}
				case GameViewEvent.nGVM_KAIPAI://开牌按钮
				{
					OnKaiPai(e.m_nWParam,e.m_nLParam);
					return;
				}
				case GameViewEvent.nGVM_ONFOLLOW://跟注
				{
					ShowFollow(e.m_nWParam,e.m_nLParam);
					return;
				}
				case GameViewEvent.nGVM_KANPAI://看牌
				{
					OnKanPai(e.m_nWParam,e.m_nLParam);
					return;
				}
			}
		}
	}
}
include "../../Common/Memory.as"
include "../../Net/GlobalDef.as"
include "../../Net/GlobalFrame.as"
include "./Common/CMD_GoldFlower.as"