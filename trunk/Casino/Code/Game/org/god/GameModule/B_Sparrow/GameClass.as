package 
{
	import flash.display.MovieClip;
	import flash.utils.ByteArray;
	import flash.geom.Point;

	import org.god.IGameFrame.*;
	import org.god.GameModule.Common.*;
	import org.god.Net.*;
	import org.god.GameModule.B_Sparrow.Common.*;

	public class GameClass extends GameBaseClass
	{
		//游戏定时器
		protected static  const IDI_EXIT_GAME:uint=200;//离开游戏定时器
		protected static  const IDI_OPERATE_SPARROW:uint=202;//操作定时器

		//游戏定时器
		protected static  const TIME_START_GAME:uint=30;//开始定时器
		protected static  const TIME_HEAR_STATUS:uint=3;//出牌定时器
		protected static  const TIME_OPERATE_SPARROW:uint=30;//操作定时器

		//用户变量
		protected var m_wBankerUser:uint;//庄家用户
		protected var m_wCurrentUser:uint;//当前用户

		//状态变量
		protected var m_bHearStatus:Boolean;//听牌状态

		//堆立变量
		protected var m_wHeapHand:uint;//堆立头部
		protected var m_wHeapTail:uint;//堆立尾部
		protected var m_cbHeapSparrowInfo:Array=new Array(4);//堆牌信息

		//麻将变量
		protected var m_cbLeftSparrowCount;//剩余数目
		protected var m_cbSparrowIndex:Array=new Array(MAX_INDEX);//手中麻将

		//组合麻将
		protected var m_cbWeaveCount:Array=new Array(GAME_PLAYER);//组合数目
		protected var m_WeaveItemArray:Array=new Array(GAME_PLAYER);//组合麻将

		//出牌信息
		protected var m_wOutSparrowUser:uint;//出牌用户
		protected var m_cbOutSparrowData:uint;//出牌麻将
		protected var m_cbDiscardCount:Array=new Array(GAME_PLAYER);//丢弃数目
		protected var m_cbDiscardSparrow:Array=new Array(GAME_PLAYER);//丢弃记录

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
			if(super.InitGameClass() == false)
				return false;
			
			for (var i:uint=0; i < m_cbHeapSparrowInfo.length; i++)
			{
				m_cbHeapSparrowInfo[i]=new Array(2);
			}
			for (i=0; i < m_WeaveItemArray.length; i++)
			{
				m_WeaveItemArray[i]=new Array(4);
				for(var k:uint = 0; k < m_WeaveItemArray[i].length; k ++)
				{
					m_WeaveItemArray[i][k] = new tagWeaveItem;
				}
			}
			for (i=0; i < m_cbDiscardSparrow.length; i++)
			{
				m_cbDiscardSparrow[i]=new Array(55);
			}
			m_GameLogic=new GameLogic  ;

			//游戏变量
			m_wBankerUser=INVALID_CHAIR;
			m_wCurrentUser=INVALID_CHAIR;

			//状态变量
			m_bHearStatus=false;

			//堆立变量
			m_wHeapHand=0;
			m_wHeapTail=0;
			ZeroTwoDimensionArray(m_cbHeapSparrowInfo);

			//出牌信息
			m_cbOutSparrowData=0;
			m_wOutSparrowUser=INVALID_CHAIR;
			ZeroTwoDimensionArray(m_cbDiscardSparrow);
			ZeroArray(m_cbDiscardCount);

			//组合麻将
			ZeroArray(m_cbWeaveCount);
			EachTwoDimensionArray(m_WeaveItemArray,tagWeaveItem.ZeroMemory);

			//麻将变量
			m_cbLeftSparrowCount=0;
			ZeroArray(m_cbSparrowIndex);
			
			GetPeerGameView().addEventListener(GameViewEvent.strGameViewEvent, 
											   OnGameViewEevent);
					AddGameSound("OUT_CARD",SOUND_OUT_CARD);
			AddGameSound("GAME_WIN",SOUND_GAME_WIN);
			AddGameSound("GAME_WARN ",SOUND_GAME_WARN);
			AddGameSound("GAME_START",SOUND_GAME_START);
			AddGameSound("GAME_LOST",SOUND_GAME_LOST);
			AddGameSound("GAME_END",SOUND_GAME_END);
			AddGameSound("A_TING",SOUND_A_TING);
			AddGameSound("A_SHANG",SOUND_A_SHANG);
			AddGameSound("A_PENG",SOUND_A_PENG);
			AddGameSound("CHI_HU",SOUND_A_CHI_HU);
			AddGameSound("A_GANG",SOUND_A_GANG);
			AddGameSound("W_9",SOUND_W_9);
			AddGameSound("W_8",SOUND_W_8);
			AddGameSound("W_7",SOUND_W_7);
			AddGameSound("W_6",SOUND_W_6);
			AddGameSound("W_5",SOUND_W_5);
			AddGameSound("W_4",SOUND_W_4);
			AddGameSound("W_3",SOUND_W_3);
			AddGameSound("W_2",SOUND_W_2);
			AddGameSound("W_1",SOUND_W_1);
			AddGameSound("T_9",SOUND_T_9);
			AddGameSound("T_8",SOUND_T_8);
			AddGameSound("T_7",SOUND_T_7);
			AddGameSound("T_6",SOUND_T_6);
			AddGameSound("T_5",SOUND_T_5);
			AddGameSound("T_4",SOUND_T_4);
			AddGameSound("T_3",SOUND_T_3);
			AddGameSound("T_2",SOUND_T_2);
			AddGameSound("T_1",SOUND_T_1);
			AddGameSound("S_9",SOUND_S_9);
			AddGameSound("S_8",SOUND_S_8);
			AddGameSound("S_7",SOUND_S_7);
			AddGameSound("S_6",SOUND_S_6);
			AddGameSound("S_5",SOUND_S_5);
			AddGameSound("S_4",SOUND_S_4);
			AddGameSound("S_3",SOUND_S_3);
			AddGameSound("S_2",SOUND_S_2);
			AddGameSound("S_1",SOUND_S_1);
			AddGameSound("A_FILL",SOUND_A_FILL);
			
			return true;
		}
		
		//重置状态
		override public function DestroyGameClass():void
		{
			GetPeerGameView().removeEventListener(GameViewEvent.strGameViewEvent,
												  OnGameViewEevent);
			
			//游戏变量
			m_wBankerUser=INVALID_CHAIR;
			m_wCurrentUser=INVALID_CHAIR;

			//状态变量
			m_bHearStatus=false;

			//堆立变量
			m_wHeapHand=0;
			m_wHeapTail=0;
			FreeTwoDimensionArray(m_cbHeapSparrowInfo, 0);
			m_cbHeapSparrowInfo = null;

			//出牌信息
			m_cbOutSparrowData=0;
			m_wOutSparrowUser=INVALID_CHAIR;
			FreeTwoDimensionArray(m_cbDiscardSparrow);
			m_cbDiscardSparrow = null;
			ZeroArray(m_cbDiscardCount);
			m_cbDiscardCount = null;

			//组合麻将
			ZeroArray(m_cbWeaveCount);
			m_cbWeaveCount = null;
			FreeTwoDimensionArray(m_WeaveItemArray);
			m_WeaveItemArray = null;
			
			//麻将变量
			m_cbLeftSparrowCount=0;
			ZeroArray(m_cbSparrowIndex);
			m_cbSparrowIndex = null;
			
			FreeTwoDimensionArray(m_WeaveItemArray);
			m_WeaveItemArray = null;
			
			m_GameLogic=null;//游戏逻辑
			
			super.DestroyGameClass();
		}
		
		////////////////////////////////
		private function GenEmptySparrowData(nSparrowCount:int):Array
		{
			var result:Array=new Array(nSparrowCount);
			ZeroArray(result,0x43);
			return result;
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
				case IDI_EXIT_GAME ://离开游戏游戏
					{
						if (nElapse == 0)
						{
							OnExit(0,0);
							return false;
						}
						if (nElapse <= 5)
						{
							PlayGameSound("GAME_WARN");
						}

						return true;


					};
				case IDI_OPERATE_SPARROW ://操作定时器
					{
						//超时判断
						if (nElapse == 0 && IsLookonMode() == false)
						{
							//获取位置
							var wMeChairID:uint=GetMeChairID();

							//动作处理
							if (wChairID == wMeChairID)
							{
								if (m_wCurrentUser == wMeChairID)
								{
									var cbSparrowData:uint=GetPeerGameView().m_HandSparrowControl.GetCurrentSparrow();
									OnOutSparrow(cbSparrowData,cbSparrowData);
								}
								else
								{
									OnSparrowOperate(WIK_NULL,0);
								}

							}
							return false;
						}

						//播放声音
						if (nElapse <= 3 && wChairID == GetMeChairID() && IsLookonMode() == false)
						{
							PlayGameSound("GAME_WARN");
						}

						return true;


				}
			};

			return false;

		}
		//网络消息
		override protected function OnGameMessage(wSubCmdID:uint,pBuffer:ByteArray,wDataSize:uint):Boolean
		{
			switch (wSubCmdID)
			{
				case SUB_S_GAME_START ://游戏开始
					{
						return OnSubGameStart(pBuffer,wDataSize);

					};
				case SUB_S_OUT_SPARROW ://用户出牌
					{
						return OnSubOutSparrow(pBuffer,wDataSize);

					};
				case SUB_S_SEND_SPARROW ://发牌消息
					{
						return OnSubSendSparrow(pBuffer,wDataSize);

					};
				case SUB_S_OPERATE_NOTIFY ://操作提示
					{
						return OnSubOperateNotify(pBuffer,wDataSize);

					};
				case SUB_S_OPERATE_RESULT ://操作结果
					{
						return OnSubOperateResult(pBuffer,wDataSize);

					};
				case SUB_S_GAME_END ://游戏结束
					{
						return OnSubGameEnd(pBuffer,wDataSize);

				}
			};

			return true;
		}
		override protected function OnGameSceneMessage(cbGameStation:uint,bLookonOther:Boolean,pBuffer:ByteArray,wDataSize:uint):Boolean
		{
			switch (cbGameStation)
			{
			case GS_MJ_FREE:	//空闲状态
				{
					//效验数据
					if (wDataSize!=CMD_S_StatusFree.sizeof_CMD_S_StatusFree) return false;
					var pStatusFree:CMD_S_StatusFree=CMD_S_StatusFree.readData(pBuffer);
		
					//设置数据
					m_wBankerUser=pStatusFree.wBankerUser;
					GetPeerGameView().SetCellScore(pStatusFree.fCellScore);
					GetPeerGameView().m_HandSparrowControl.SetDisplayItem(true);
		
					//设置界面
					for (var i:uint=0;i<GAME_PLAYER;i++)
					{
						m_cbHeapSparrowInfo[i][0]=0;
						var v:uint = 26;
						if(i==m_wBankerUser)
							v = 28;
						else if((i+2)%4==m_wBankerUser)
							v = 28;
						
						m_cbHeapSparrowInfo[i][1]=HEAP_FULL_COUNT-v;
						GetPeerGameView().m_HeapSparrow[SwitchViewChairID(i)].SetSparrowData(m_cbHeapSparrowInfo[i][0],m_cbHeapSparrowInfo[i][1],HEAP_FULL_COUNT);
					}
		
					//设置控件
					if (IsLookonMode()==false)
					{
						GetPeerGameView().m_btStart.visible = true;
						//GetPeerGameView().m_btStart.SetFocus();
					}
		
					//更新界面
					GetPeerGameView().UpdateGameView();
		
					return true;
				}
			case GS_MJ_PLAY:	//游戏状态
				{
					//效验数据
					if (wDataSize!=CMD_S_StatusPlay.sizeof_CMD_S_StatusPlay) return false;
					var pStatusPlay:CMD_S_StatusPlay=CMD_S_StatusPlay.readData(pBuffer);
		
					//设置变量
					m_wBankerUser=pStatusPlay.wBankerUser;
					m_wCurrentUser=pStatusPlay.wCurrentUser;
					m_cbLeftSparrowCount=pStatusPlay.cbLeftSparrowCount;
		
					//历史变量
					m_wOutSparrowUser=pStatusPlay.wOutSparrowUser;
					m_cbOutSparrowData=pStatusPlay.cbOutSparrowData;
					CopyTwoDimensionArray(m_cbDiscardSparrow,pStatusPlay.cbDiscardSparrow,CountArray(m_cbDiscardSparrow));
					CopyArray(m_cbDiscardCount,pStatusPlay.cbDiscardCount,CountArray(m_cbDiscardCount));
		
					//麻将变量
					CopyArray(m_cbWeaveCount,pStatusPlay.cbWeaveCount,CountArray(m_cbWeaveCount));
					for(var i:uint = 0; i < m_WeaveItemArray.length; i ++)
					{
						for(var k:uint = 0; k < m_WeaveItemArray[i].length; k ++)
						{
							m_WeaveItemArray[i][k].cbWeaveKind = pStatusPlay.WeaveItemArray[i][k].cbWeaveKind;
							m_WeaveItemArray[i][k].cbCenterSparrow = pStatusPlay.WeaveItemArray[i][k].cbCenterSparrow;
							m_WeaveItemArray[i][k].cbPublicSparrow = pStatusPlay.WeaveItemArray[i][k].cbPublicSparrow;
							m_WeaveItemArray[i][k].wProvideUser = pStatusPlay.WeaveItemArray[i][k].wProvideUser;
						}
					}
					
					m_GameLogic.SwitchToSparrowIndex3(pStatusPlay.cbSparrowData,pStatusPlay.cbSparrowCount,m_cbSparrowIndex);
		
					//辅助变量
					var wViewChairID:Array = new Array(0,0,0,0);
					for (var i:uint=0;i<GAME_PLAYER;i++) wViewChairID[i]=SwitchViewChairID(i);
		
					//界面设置
					GetPeerGameView().SetCellScore(pStatusPlay.fCellScore);
					GetPeerGameView().SetBankerUser(wViewChairID[m_wBankerUser]);
		
					//组合麻将
					var cbWeaveSparrow:Array=new Array(0,0,0,0);
					for (var i:uint=0;i<GAME_PLAYER;i++)
					{
						for (var j:uint=0;j<m_cbWeaveCount[i];j++)
						{
							var cbWeaveKind:uint=m_WeaveItemArray[i][j].cbWeaveKind;
							var cbCenterSparrow:uint=m_WeaveItemArray[i][j].cbCenterSparrow;
							var cbWeaveSparrowCount:uint=m_GameLogic.GetWeaveSparrow(cbWeaveKind,cbCenterSparrow,cbWeaveSparrow);
							GetPeerGameView().m_WeaveSparrow[wViewChairID[i]][j].SetSparrowData(cbWeaveSparrow,cbWeaveSparrowCount);
							if (cbWeaveKind&(WIK_GANG|WIK_FILL))
							{
								if (m_WeaveItemArray[i][j].wProvideUser==i) GetPeerGameView().m_WeaveSparrow[wViewChairID[i]][j].SetDisplayItem(false);
							}
						}
					}
		
					//用户麻将
					if (m_wCurrentUser==GetMeChairID())
					{
						var cbSparrowCount:uint=pStatusPlay.cbSparrowCount;
						GetPeerGameView().m_HandSparrowControl.SetSparrowData(pStatusPlay.cbSparrowData,cbSparrowCount-1,pStatusPlay.cbSparrowData[cbSparrowCount-cbSparrowCount]);
					}
					else GetPeerGameView().m_HandSparrowControl.SetSparrowData(pStatusPlay.cbSparrowData,pStatusPlay.cbSparrowCount,0); 
		
					//麻将设置
					for (var i:uint=0;i<GAME_PLAYER;i++)
					{
						//用户麻将
						if (i!=GetMeChairID())
						{
							var cbSparrowCount:uint=13-m_cbWeaveCount[i]*3;
							var wUserSparrowIndex:uint=(wViewChairID[i]<2)?wViewChairID[i]:2;
							GetPeerGameView().m_UserSparrow[wUserSparrowIndex].SetSparrowData(cbSparrowCount,(m_wCurrentUser==i));
						}
		
						//丢弃麻将
						var wViewChairID1:uint=SwitchViewChairID(i);
						GetPeerGameView().m_DiscardSparrow[wViewChairID1].SetSparrowData(m_cbDiscardSparrow[i],m_cbDiscardCount[i]);
					}
		
					//控制设置
					if (IsLookonMode()==false) 
					{
						GetPeerGameView().m_HandSparrowControl.SetPositively(true);
						GetPeerGameView().m_HandSparrowControl.SetDisplayItem(true);
					}
		
					//堆立麻将
					for (var i:uint=0;i<GAME_PLAYER;i++)
					{
						m_cbHeapSparrowInfo[i][0]=0;
						var v:uint = 26;
						if(i==m_wBankerUser)
							v = 28;
						else if((i+2)%4==m_wBankerUser)
							v = 28;
						m_cbHeapSparrowInfo[i][1]=HEAP_FULL_COUNT-v;
					}
		
					//分发麻将
					var cbTakeCount:uint=108-m_cbLeftSparrowCount;
					var cbSiceFirst:uint=HIBYTE(pStatusPlay.wSiceCount);
					var cbSiceSecond:uint=LOBYTE(pStatusPlay.wSiceCount);
					var wTakeChairID:uint=(m_wBankerUser+7-cbSiceFirst)%GAME_PLAYER;
					for (var i:uint=0;i<GAME_PLAYER;i++)
					{
						//计算数目
						var cbValidCount:uint=HEAP_FULL_COUNT-m_cbHeapSparrowInfo[wTakeChairID][1]-((i==0)?(cbSiceSecond-1)*2:0);
						var cbRemoveCount:uint=__min(cbValidCount,cbTakeCount);
		
						//提取麻将
						cbTakeCount-=cbRemoveCount;
						m_cbHeapSparrowInfo[wTakeChairID][(i==0)?1:0]+=cbRemoveCount;
		
						//完成判断
						if (cbTakeCount==0)
						{
							m_wHeapHand=wTakeChairID;
							m_wHeapTail=(m_wBankerUser+7-cbSiceFirst)%GAME_PLAYER;
							break;
						}
		
						//切换索引
						wTakeChairID=(wTakeChairID+1)%GAME_PLAYER;
					}
		
					//堆立界面
					for (var i:uint=0;i<GAME_PLAYER;i++)
					{
						GetPeerGameView().m_HeapSparrow[wViewChairID[i]].SetSparrowData(m_cbHeapSparrowInfo[i][0],m_cbHeapSparrowInfo[i][1],HEAP_FULL_COUNT);
					}
		
					//历史麻将
					if (m_wOutSparrowUser!=INVALID_CHAIR)
					{
						var  wOutChairID:uint=SwitchViewChairID(m_wOutSparrowUser);
						GetPeerGameView().SetOutSparrowInfo(wOutChairID,m_cbOutSparrowData);
					}
		
					//出牌设置
					if (cbGameStation==GS_MJ_PLAY)
					{
						//操作界面
						if ((IsLookonMode()==false)&&(pStatusPlay.cbActionMask!=WIK_NULL))
						{
							//获取变量
							var cbActionMask:uint=pStatusPlay.cbActionMask;
							var cbActionSparrow:uint=pStatusPlay.cbActionSparrow;
		
							//变量定义
							var GangSparrowResult:tagGangSparrowResult = new tagGangSparrowResult;
							
		
							//杠牌判断
							if ((cbActionMask&(WIK_GANG|WIK_FILL))!=0)
							{
								//桌面杆牌
								if ((m_wCurrentUser==INVALID_CHAIR)&&(cbActionSparrow!=0))
								{
									GangSparrowResult.cbSparrowCount=1;
									GangSparrowResult.cbSparrowData[0]=cbActionSparrow;
								}
		
								//自己杆牌
								if ((m_wCurrentUser==GetMeChairID())||(cbActionSparrow==0))
								{
									var wMeChairID:uint=GetMeChairID();
									m_GameLogic.AnalyseGangSparrow(m_cbSparrowIndex,m_WeaveItemArray[wMeChairID],m_cbWeaveCount[wMeChairID],GangSparrowResult);
								}
							}
		
							//设置界面
							GetPeerGameView().m_ControlMC.SetControlInfo(cbActionSparrow,cbActionMask,GangSparrowResult);
							if (m_wCurrentUser==INVALID_CHAIR) SetGameTimer(GetMeChairID(),IDI_OPERATE_SPARROW,TIME_OPERATE_SPARROW);
						}
		
						//设置时间
						if (m_wCurrentUser!=INVALID_CHAIR)
						{
							//计算时间
							var wTimeCount:uint=TIME_OPERATE_SPARROW;
							if ((m_bHearStatus==true)&&(m_wCurrentUser==GetMeChairID())) wTimeCount=TIME_HEAR_STATUS;
		
							//设置时间
							SetGameTimer(m_wCurrentUser,TIME_OPERATE_SPARROW,wTimeCount);
						}
					}
		
					//更新界面
					GetPeerGameView().UpdateGameView();
		
					return true;
				}
			}
		
			return false;
		}

		//游戏事件
		//游戏开始
		protected function OnSubGameStart(pBuffer:ByteArray, wDataSize:uint):Boolean
		{
			//效验数据
			ASSERT(wDataSize==CMD_S_GameStart.sizeof_CMD_S_GameStart);
			if (wDataSize!=CMD_S_GameStart.sizeof_CMD_S_GameStart) return false;
		
			//变量定义
			var pGameStart:CMD_S_GameStart=CMD_S_GameStart.readData(pBuffer);
		
			//设置状态
			SetGameStatus(GS_MJ_PLAY);
		
			//设置变量
			m_bHearStatus=false;
			m_wBankerUser=pGameStart.wBankerUser;
			m_wCurrentUser=pGameStart.wCurrentUser;
			m_cbLeftSparrowCount=MAX_REPERTORY-GAME_PLAYER*(MAX_COUNT-1)-1;
		
			//出牌信息
			m_cbOutSparrowData=0;
			m_wOutSparrowUser=INVALID_CHAIR;
			ZeroArray(m_cbDiscardSparrow);
			ZeroArray(m_cbDiscardCount);
		
			//组合麻将
			ZeroArray(m_cbWeaveCount);
			EachTwoDimensionArray(m_WeaveItemArray,tagWeaveItem.ZeroMemory);
		
			//设置麻将
			var cbSparrowCount:uint=(GetMeChairID()==m_wBankerUser)?MAX_COUNT:(MAX_COUNT-1);
			m_GameLogic.SwitchToSparrowIndex3(pGameStart.cbSparrowData,cbSparrowCount,m_cbSparrowIndex);
		
			//设置界面
			var bPlayerMode:Boolean=(IsLookonMode()==false);
			GetPeerGameView().m_ScoreView.ShowWindow(false);
			GetPeerGameView().m_HandSparrowControl.SetPositively(bPlayerMode);
			GetPeerGameView().SetBankerUser(SwitchViewChairID(m_wBankerUser));
		
			//旁观界面
			if (bPlayerMode==false)
			{
				GetPeerGameView().SetHuangZhuang(false);
				GetPeerGameView().SetStatusFlag(false,false);
				GetPeerGameView().SetUserAction(INVALID_CHAIR,0);
				GetPeerGameView().SetOutSparrowInfo(INVALID_CHAIR,0);
				GetPeerGameView().m_ScoreView.ShowWindow(false);
			}
		
			//堆立麻将
			for (var i:uint=0;i<GAME_PLAYER;i++)
			{
				m_cbHeapSparrowInfo[i][0]=0;
				var v:uint = 26;
				if(i==m_wBankerUser)
					v = 28;
				else if((i+2)%4==m_wBankerUser)
					v = 28;
				m_cbHeapSparrowInfo[i][1]=HEAP_FULL_COUNT-v;
			}
		
			//分发麻将
			var cbTakeCount:uint=(MAX_COUNT-1)*GAME_PLAYER+1;
			var cbSiceFirst:uint=HIBYTE(pGameStart.wSiceCount);
			var cbSiceSecond:uint=LOBYTE(pGameStart.wSiceCount);
			var wTakeChairID:uint=(m_wBankerUser+7-cbSiceFirst)%GAME_PLAYER;
			for (var i:uint=0;i<GAME_PLAYER;i++)
			{
				//计算数目
				var cbValidCount:uint=HEAP_FULL_COUNT-m_cbHeapSparrowInfo[wTakeChairID][1]-((i==0)?(cbSiceSecond-1)*2:0);
				var cbRemoveCount:uint=__min(cbValidCount,cbTakeCount);
		
				//提取麻将
				cbTakeCount-=cbRemoveCount;
				m_cbHeapSparrowInfo[wTakeChairID][(i==0)?1:0]+=cbRemoveCount;
		
				//完成判断
				if (cbTakeCount==0)
				{
					m_wHeapHand=wTakeChairID;
					m_wHeapTail=(m_wBankerUser+7-cbSiceFirst)%GAME_PLAYER;
					break;
				}
		
				//切换索引
				wTakeChairID=(wTakeChairID+1)%GAME_PLAYER;
			}
		
			//麻将设置
			for (var i:uint=0;i<GAME_PLAYER;i++)
			{
				//变量定义
				var  wViewChairID:uint=SwitchViewChairID(i);
		
				//组合界面
				GetPeerGameView().m_WeaveSparrow[i][0].SetDisplayItem(true);
				GetPeerGameView().m_WeaveSparrow[i][1].SetDisplayItem(true);
				GetPeerGameView().m_WeaveSparrow[i][2].SetDisplayItem(true);
				GetPeerGameView().m_WeaveSparrow[i][3].SetDisplayItem(true);
				GetPeerGameView().m_HeapSparrow[wViewChairID].SetSparrowData(m_cbHeapSparrowInfo[i][0],m_cbHeapSparrowInfo[i][1],HEAP_FULL_COUNT);
		
				//用户麻将
				if (wViewChairID!=2)
				{
					var wIndex:uint=(wViewChairID>=3)?2:wViewChairID;
					GetPeerGameView().m_UserSparrow[wIndex].SetSparrowData(CountArray(pGameStart.cbSparrowData),(i==m_wBankerUser));
				}
				else
				{
					var cbBankerSparrow:uint=(i==m_wBankerUser)?pGameStart.cbSparrowData[MAX_COUNT-1]:0;
					GetPeerGameView().m_HandSparrowControl.SetSparrowData(pGameStart.cbSparrowData,MAX_COUNT-1,cbBankerSparrow);
				}
		
				//旁观界面
				if (bPlayerMode==false)
				{
					GetPeerGameView().m_TableSparrow[wViewChairID].SetSparrowData(null,0);
					GetPeerGameView().m_DiscardSparrow[wViewChairID].SetSparrowData(null,0);
					GetPeerGameView().m_WeaveSparrow[wViewChairID][0].SetSparrowData(null,0);
					GetPeerGameView().m_WeaveSparrow[wViewChairID][1].SetSparrowData(null,0);
					GetPeerGameView().m_WeaveSparrow[wViewChairID][2].SetSparrowData(null,0);
					GetPeerGameView().m_WeaveSparrow[wViewChairID][3].SetSparrowData(null,0);
				}
			}
		
			//出牌提示
			if ((bPlayerMode==true)&&(m_wCurrentUser!=INVALID_CHAIR))
			{
				var wMeChairID:uint=GetMeChairID();
				if (m_wCurrentUser==wMeChairID) GetPeerGameView().SetStatusFlag(true,false);
			}
		
			//动作处理
			if ((bPlayerMode==true)&&(pGameStart.cbUserAction!=WIK_NULL))
			{
				ShowOperateControl(pGameStart.cbUserAction,0);
				SetGameTimer(GetMeChairID(),IDI_OPERATE_SPARROW,TIME_OPERATE_SPARROW);
			}
		
			//更新界面
			GetPeerGameView().UpdateGameView();
		
			//激活框架
			if (bPlayerMode==true) ActiveGameFrame();
		
			//环境处理
			PlayGameSound("GAME_START");
		
			//设置时间
			if (m_wCurrentUser!=INVALID_CHAIR) SetGameTimer(m_wCurrentUser,IDI_OPERATE_SPARROW,TIME_OPERATE_SPARROW);
		
			return true;
		}
		//用户出牌
		protected function OnSubOutSparrow(pBuffer:ByteArray, wDataSize:uint):Boolean
		{
			//效验消息
			ASSERT(wDataSize==CMD_S_OutSparrow.sizeof_CMD_S_OutSparrow);
			if (wDataSize!=CMD_S_OutSparrow.sizeof_CMD_S_OutSparrow) return false;
		
			//消息处理
			var pOutSparrow:CMD_S_OutSparrow=CMD_S_OutSparrow.readData(pBuffer);
		
			//变量定义
			var wMeChairID:uint=GetMeChairID();
			var wOutViewChairID:uint=SwitchViewChairID(pOutSparrow.wOutSparrowUser);
		
			//设置变量
			m_wCurrentUser=INVALID_CHAIR;
			m_wOutSparrowUser=pOutSparrow.wOutSparrowUser;
			m_cbOutSparrowData=pOutSparrow.cbOutSparrowData;
		
			//其他用户
			if ((IsLookonMode()==true)||(pOutSparrow.wOutSparrowUser!=wMeChairID))
			{
				//环境设置
				KillGameTimer(IDI_OPERATE_SPARROW); 
				PlaySparrowSound(pOutSparrow.cbOutSparrowData);
		
				//出牌界面
				GetPeerGameView().SetUserAction(INVALID_CHAIR,0);
				GetPeerGameView().SetOutSparrowInfo(wOutViewChairID,pOutSparrow.cbOutSparrowData);
		
				//设置麻将
				if (wOutViewChairID==2)
				{
					//删除麻将
					var cbSparrowData:Array = new Array(MAX_COUNT);
					m_GameLogic.RemoveSparrow2(m_cbSparrowIndex,pOutSparrow.cbOutSparrowData);
		
					//设置麻将
					var cbSparrowCount:uint=m_GameLogic.SwitchToSparrowData2(m_cbSparrowIndex,cbSparrowData);
					GetPeerGameView().m_HandSparrowControl.SetSparrowData(cbSparrowData,cbSparrowCount,0);
				}
				else
				{
					var wUserIndex:uint=(wOutViewChairID>2)?2:wOutViewChairID;
					GetPeerGameView().m_UserSparrow[wUserIndex].SetCurrentSparrow(false);
				}
			}
		
			return true;
		}
		//发牌消息
		protected function OnSubSendSparrow(pBuffer:ByteArray, wDataSize:uint):Boolean
		{
			//效验数据
			ASSERT(wDataSize==CMD_S_SendSparrow.sizeof_CMD_S_SendSparrow);
			if (wDataSize!=CMD_S_SendSparrow.sizeof_CMD_S_SendSparrow) return false;
		
			//变量定义
			var pSendSparrow=CMD_S_SendSparrow.readData(pBuffer);
		
			//设置变量
			var wMeChairID:uint=GetMeChairID();
			m_wCurrentUser=pSendSparrow.wCurrentUser;
		
			//丢弃麻将
			if ((m_wOutSparrowUser!=INVALID_CHAIR)&&(m_cbOutSparrowData!=0))
			{
				//丢弃麻将
				var wOutViewChairID:uint=SwitchViewChairID(m_wOutSparrowUser);
				GetPeerGameView().m_DiscardSparrow[wOutViewChairID].AddSparrowItem(m_cbOutSparrowData);
		
				//设置变量
				m_cbOutSparrowData=0;
				m_wOutSparrowUser=INVALID_CHAIR;
			}
		
			//发牌处理
			if (pSendSparrow.cbSparrowData!=0)
			{
				//取牌界面
				var  wViewChairID:uint=SwitchViewChairID(m_wCurrentUser);
				if (wViewChairID!=2)
				{
					var wUserIndex:uint=(wViewChairID>2)?2:wViewChairID;
					GetPeerGameView().m_UserSparrow[wUserIndex].SetCurrentSparrow(true);
				}
				else
				{
					m_cbSparrowIndex[m_GameLogic.SwitchToSparrowIndex1(pSendSparrow.cbSparrowData)]++;
					GetPeerGameView().m_HandSparrowControl.SetCurrentSparrowData(pSendSparrow.cbSparrowData);
				}
				
				//扣除麻将
				DeductionTableSparrow(true);
			}
		
			//当前用户
			if ((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID))
			{
				//激活框架
				ActiveGameFrame();
		
				//动作处理
				if (pSendSparrow.cbActionMask!=WIK_NULL)
				{
					//获取变量
					var cbActionSparrow:uint=pSendSparrow.cbSparrowData;
					var cbActionMask:uint=pSendSparrow.cbActionMask;
		
					//变量定义
					var GangSparrowResult:tagGangSparrowResult = new tagGangSparrowResult;
					
					//杠牌判断
					if ((cbActionMask&(WIK_GANG|WIK_FILL))!=0)
					{
						var wMeChairID:uint=GetMeChairID();
						m_GameLogic.AnalyseGangSparrow(m_cbSparrowIndex,m_WeaveItemArray[wMeChairID],m_cbWeaveCount[wMeChairID],GangSparrowResult);
					}
		
					//设置界面
					GetPeerGameView().m_ControlMC.SetControlInfo(cbActionSparrow,cbActionMask,GangSparrowResult);
				}
			}
		
			//出牌提示
			GetPeerGameView().SetStatusFlag((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID),false);
		
			//更新界面
			GetPeerGameView().UpdateGameView();
		
			//计算时间
			var wTimeCount:uint=TIME_OPERATE_SPARROW;
			if ((m_bHearStatus==true)&&(m_wCurrentUser==wMeChairID)) wTimeCount=TIME_HEAR_STATUS;
		
			//设置时间
			SetGameTimer(m_wCurrentUser,IDI_OPERATE_SPARROW,wTimeCount);
		
			return true;
		}
		//操作提示
		protected function OnSubOperateNotify(pBuffer:ByteArray, wDataSize:uint):Boolean
		{
			//效验数据
			ASSERT(wDataSize==CMD_S_OperateNotify.sizeof_CMD_S_OperateNotify);
			if (wDataSize!=CMD_S_OperateNotify.sizeof_CMD_S_OperateNotify) return false;
		
			//变量定义
			var pOperateNotify:CMD_S_OperateNotify=CMD_S_OperateNotify.readData(pBuffer);
		
			//用户界面
			if ((IsLookonMode()==false)&&(pOperateNotify.cbActionMask!=WIK_NULL))
			{
				//获取变量
				var cbActionMask:uint=pOperateNotify.cbActionMask;
				var cbActionSparrow:uint=pOperateNotify.cbActionSparrow;
		
				//变量定义
				var GangSparrowResult:tagGangSparrowResult = new tagGangSparrowResult;
				
				//杠牌判断
				if ((cbActionMask&(WIK_GANG|WIK_FILL))!=0)
				{
					//桌面杆牌
					if ((m_wCurrentUser==INVALID_CHAIR)&&(cbActionSparrow!=0))
					{
						GangSparrowResult.cbSparrowCount=1;
						GangSparrowResult.cbSparrowData[0]=cbActionSparrow;
					}
		
					//自己杆牌
					if ((m_wCurrentUser==GetMeChairID())||(cbActionSparrow==0))
					{
						var wMeChairID:uint=GetMeChairID();
						m_GameLogic.AnalyseGangSparrow(m_cbSparrowIndex,m_WeaveItemArray[wMeChairID],m_cbWeaveCount[wMeChairID],GangSparrowResult);
					}
				}
		
				//设置界面
				ActiveGameFrame();
				SetGameTimer(GetMeChairID(),IDI_OPERATE_SPARROW,TIME_OPERATE_SPARROW);
				GetPeerGameView().m_ControlMC.SetControlInfo(cbActionSparrow,cbActionMask,GangSparrowResult);
			}
		
			return true;
		}
		//操作结果
		protected function OnSubOperateResult(pBuffer:ByteArray, wDataSize:uint):Boolean
		{
			//效验消息
			ASSERT(wDataSize==CMD_S_OperateResult.sizeof_CMD_S_OperateResult);
			if (wDataSize!=CMD_S_OperateResult.sizeof_CMD_S_OperateResult) return false;
		
			//消息处理
			var pOperateResult:CMD_S_OperateResult=CMD_S_OperateResult.readData(pBuffer);
		
			//变量定义
			var cbPublicSparrow:Boolean=true;
			var wOperateUser:uint=pOperateResult.wOperateUser;
			var cbOperateSparrow:uint=pOperateResult.cbOperateSparrow;
			var wOperateViewID:uint=SwitchViewChairID(wOperateUser);
		
			//出牌变量
			if (pOperateResult.cbOperateCode!=WIK_NULL)
			{
				m_cbOutSparrowData=0;
				m_wOutSparrowUser=INVALID_CHAIR;
			}
		
			//设置组合
			if ((pOperateResult.cbOperateCode&(WIK_GANG|WIK_FILL))!=0)
			{
				//设置变量
				m_wCurrentUser=INVALID_CHAIR;
		
				//组合麻将
				var cbWeaveIndex:uint=0xFF;
				for (var i:uint=0;i<m_cbWeaveCount[wOperateUser];i++)
				{
					var cbWeaveKind:uint=m_WeaveItemArray[wOperateUser][i].cbWeaveKind;
					var cbCenterSparrow:uint=m_WeaveItemArray[wOperateUser][i].cbCenterSparrow;
					if ((cbCenterSparrow==cbOperateSparrow)&&(cbWeaveKind==WIK_PENG))
					{
						cbWeaveIndex=i;
						m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicSparrow=true;
						m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult.cbOperateCode;
						m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult.wProvideUser;
						break;
					}
				}
		
				//组合麻将
				if (cbWeaveIndex==0xFF)
				{
					//暗杠判断
					cbPublicSparrow=(pOperateResult.wProvideUser==wOperateUser)?false:true;
		
					//设置麻将
					cbWeaveIndex=m_cbWeaveCount[wOperateUser]++;
					m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicSparrow=cbPublicSparrow;
					m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCenterSparrow=cbOperateSparrow;
					m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult.cbOperateCode;
					m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult.wProvideUser;
				}
		
				//组合界面
				var cbWeaveSparrow:Array = new Array(0,0,0,0);
				var cbWeaveKind:uint=pOperateResult.cbOperateCode;
				var cbWeaveSparrowCount:uint=m_GameLogic.GetWeaveSparrow(cbWeaveKind,cbOperateSparrow,cbWeaveSparrow);
				GetPeerGameView().m_WeaveSparrow[wOperateViewID][cbWeaveIndex].SetSparrowData(cbWeaveSparrow,cbWeaveSparrowCount);
				GetPeerGameView().m_WeaveSparrow[wOperateViewID][cbWeaveIndex].SetDisplayItem((cbPublicSparrow==true)?true:false);
		
				//麻将设置
				if (GetMeChairID()==wOperateUser)
				{
					m_cbSparrowIndex[m_GameLogic.SwitchToSparrowIndex1(pOperateResult.cbOperateSparrow)]=0;
				}
		
				//设置麻将
				if (GetMeChairID()==wOperateUser)
				{
					var cbSparrowData:Array = new Array(MAX_COUNT);
					var cbSparrowCount:uint=m_GameLogic.SwitchToSparrowData2(m_cbSparrowIndex,cbSparrowData);
					GetPeerGameView().m_HandSparrowControl.SetSparrowData(cbSparrowData,cbSparrowCount,0);
				}
				else
				{
					var wUserIndex:uint=(wOperateViewID>=3)?2:wOperateViewID;
					var cbSparrowCount:uint=MAX_COUNT-m_cbWeaveCount[wOperateUser]*3;
					GetPeerGameView().m_UserSparrow[wUserIndex].SetSparrowData(cbSparrowCount-1,false);
				}
			}
			else if (pOperateResult.cbOperateCode!=WIK_NULL)
			{
				//设置变量
				m_wCurrentUser=pOperateResult.wOperateUser;
		
				//设置组合
				var cbWeaveIndex:uint=m_cbWeaveCount[wOperateUser]++;
				m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbPublicSparrow=true;
				m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbCenterSparrow=cbOperateSparrow;
				m_WeaveItemArray[wOperateUser][cbWeaveIndex].cbWeaveKind=pOperateResult.cbOperateCode;
				m_WeaveItemArray[wOperateUser][cbWeaveIndex].wProvideUser=pOperateResult.wProvideUser;
		
				//组合界面
				var cbWeaveSparrow:Array=new Array(0,0,0,0);
				var cbWeaveKind:uint=pOperateResult.cbOperateCode;
				var cbWeaveSparrowCount:uint=m_GameLogic.GetWeaveSparrow(cbWeaveKind,cbOperateSparrow,cbWeaveSparrow);
				GetPeerGameView().m_WeaveSparrow[wOperateViewID][cbWeaveIndex].SetSparrowData(cbWeaveSparrow,cbWeaveSparrowCount);
		
				//删除麻将
				if (GetMeChairID()==wOperateUser)
				{
					var a:Array = new Array(1);
					a[0] = cbOperateSparrow;
					m_GameLogic.RemoveSparrow4(cbWeaveSparrow,cbWeaveSparrowCount,a,1);
					m_GameLogic.RemoveSparrow3(m_cbSparrowIndex,cbWeaveSparrow,cbWeaveSparrowCount-1);
				}
		
				//设置麻将
				if (GetMeChairID()==wOperateUser)
				{
					var cbSparrowData:Array = new Array(MAX_COUNT);
					var cbSparrowCount:uint=m_GameLogic.SwitchToSparrowData2(m_cbSparrowIndex,cbSparrowData);
					GetPeerGameView().m_HandSparrowControl.SetSparrowData(cbSparrowData,cbSparrowCount-1,cbSparrowData[cbSparrowCount-1]);
				}
				else
				{
					var wUserIndex:uint=(wOperateViewID>=3)?2:wOperateViewID;
					var cbSparrowCount:uint=MAX_COUNT-m_cbWeaveCount[wOperateUser]*3;
					GetPeerGameView().m_UserSparrow[wUserIndex].SetSparrowData(cbSparrowCount-1,true);
				}
			}
		
			//设置界面
			GetPeerGameView().SetOutSparrowInfo(INVALID_CHAIR,0);
			GetPeerGameView().m_ControlMC.ShowWindow(false);
			GetPeerGameView().SetUserAction(wOperateViewID,pOperateResult.cbOperateCode);
			GetPeerGameView().SetStatusFlag((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()),false);
		
			//更新界面
			GetPeerGameView().UpdateGameView();
		
			//环境设置
			PlayActionSound(pOperateResult.cbOperateCode);
		
			//设置时间
			if (m_wCurrentUser!=INVALID_CHAIR)
			{
				//计算时间
				var wTimeCount:uint=TIME_OPERATE_SPARROW;
				if ((m_bHearStatus==true)&&(m_wCurrentUser==GetMeChairID())) wTimeCount=TIME_HEAR_STATUS;
		
				//设置时间
				SetGameTimer(m_wCurrentUser,IDI_OPERATE_SPARROW,wTimeCount);
			}
		
			return true;
		}
		//游戏结束
		protected function OnSubGameEnd(pBuffer:ByteArray, wDataSize:uint):Boolean
		{
			//效验数据
			ASSERT(wDataSize==CMD_S_GameEnd.sizeof_CMD_S_GameEnd);
			if (wDataSize!=CMD_S_GameEnd.sizeof_CMD_S_GameEnd) return false;
		
			//消息处理
			var pGameEnd:CMD_S_GameEnd=CMD_S_GameEnd.readData(pBuffer);
		
			//设置状态
			SetGameStatus(GS_MJ_FREE);
			GetPeerGameView().SetStatusFlag(false,false);
		
			//删除定时器
			KillGameTimer(IDI_OPERATE_SPARROW);
		
			//设置控件
			GetPeerGameView().SetStatusFlag(false,false);
			GetPeerGameView().m_ControlMC.ShowWindow(false);
			GetPeerGameView().m_HandSparrowControl.SetPositively(false);
		
			//结束设置
			if (pGameEnd.cbChiHuSparrow!=0)
			{
				GetPeerGameView().m_ScoreView.SetBankerUser(m_wBankerUser);
				GetPeerGameView().m_ScoreView.SetChiHuSparrow(pGameEnd.cbChiHuSparrow);
				GetPeerGameView().m_ScoreView.SetProvideUser(pGameEnd.wProvideUser);
				for (var i:uint=0;i<GAME_PLAYER;i++)
				{
					GetPeerGameView().m_ScoreView.SetGameScore(i,pGameEnd.fGameScore[i]);
					GetPeerGameView().m_ScoreView.SetSparrowType(i,pGameEnd.wChiHuKind[i]);
				}
				
				GetPeerGameView().m_ScoreView.SetMeChairID(GetMeChairID());
				GetPeerGameView().m_ScoreView.ShowWindow(true);
			}
			else 
			{
				DeductionTableSparrow(true);
				GetPeerGameView().SetHuangZhuang(true);
			}
		
			//用户麻将
			for (var i:uint=0;i<GAME_PLAYER;i++)
			{
				var  wViewChairID:uint=SwitchViewChairID(i);
				if (pGameEnd.wChiHuKind[i]!=CHK_NULL) GetPeerGameView().SetUserAction(wViewChairID,WIK_CHI_HU);
				GetPeerGameView().m_TableSparrow[wViewChairID].SetSparrowData(pGameEnd.cbSparrowData[i],pGameEnd.cbSparrowCount[i]);
			}
		
			//设置麻将
			GetPeerGameView().m_UserSparrow[0].SetSparrowData(0,false);
			GetPeerGameView().m_UserSparrow[1].SetSparrowData(0,false);
			GetPeerGameView().m_UserSparrow[2].SetSparrowData(0,false);
			GetPeerGameView().m_HandSparrowControl.SetSparrowData(null,0,0);
		
			//播放声音
			var fScore:Number=pGameEnd.fGameScore[GetMeChairID()];
			if (fScore>0) PlayGameSound("GAME_WIN");
			else if (fScore<0) PlayGameSound("GAME_LOST");
			else PlayGameSound("GAME_END");
		
			//设置界面
			if (IsLookonMode()==false)
			{
				GetPeerGameView().m_btStart.visible = true;
				SetGameTimer(GetMeChairID(),IDI_EXIT_GAME,TIME_START_GAME);
			}
		
			//更新界面
			GetPeerGameView().UpdateGameView();
		
			return true;
		}
		
		/////////////////////////////////////////////////////////////
		//消息处理
		//播放声音
		protected function PlaySparrowSound(cbSparrowData:uint):void
		{
			//变量定义
			
			var szKind:Array=new Array("W","S","T","F");
		
			//构造名字
			var cbValue:uint=(cbSparrowData&MASK_VALUE);
			var cbColor:uint=(cbSparrowData&MASK_COLOR)>>4;
		
			var szSoundName:String = szKind[cbColor] + "_" + cbValue;
			//播放声音
			PlayGameSound(szSoundName);
		
			return;
		}
		
		//播放声音
		protected function PlayActionSound( cbAction:uint):void
		{
			switch (cbAction)
			{
			case WIK_NULL:		//取消
				{
					PlayGameSound("OUT_SPARROW");
					break;
				}
			case WIK_LEFT:
			case WIK_CENTER:
			case WIK_RIGHT:		//上牌
				{
					PlayGameSound("A_SHANG");
					break;
				}
			case WIK_PENG:		//碰牌
				{
					PlayGameSound("A_PENG");
					break;
				}
			case WIK_FILL:		//补牌
				{
					PlayGameSound("A_FILL");
					break;
				}
			case WIK_GANG:		//杠牌
				{
					PlayGameSound("A_GANG");
					break;
				}
			case WIK_CHI_HU:	//吃胡
				{
					PlayGameSound("A_CHI_HU");
					break;
				}
			}
		
			return;
		}
		
		//扣除麻将
		protected function DeductionTableSparrow( bHeadSparrow:Boolean):void
		{
			if (bHeadSparrow==true)
			{
				//切换索引
				var cbHeapCount:uint=m_cbHeapSparrowInfo[m_wHeapHand][0]+m_cbHeapSparrowInfo[m_wHeapHand][1];
				if (cbHeapCount==HEAP_FULL_COUNT) m_wHeapHand=(m_wHeapHand+1)%CountArray(m_cbHeapSparrowInfo);
		
				//减少麻将
				m_cbLeftSparrowCount--;
				m_cbHeapSparrowInfo[m_wHeapHand][0]++;
		
				//堆立麻将
				var wHeapViewID:uint=SwitchViewChairID(m_wHeapHand);
				var wMinusHeadCount:uint=m_cbHeapSparrowInfo[m_wHeapHand][0];
				var wMinusLastCount:uint=m_cbHeapSparrowInfo[m_wHeapHand][1];
				GetPeerGameView().m_HeapSparrow[wHeapViewID].SetSparrowData(wMinusHeadCount,wMinusLastCount,HEAP_FULL_COUNT);
			}
			else
			{
				//切换索引
				var cbHeapCount:uint=m_cbHeapSparrowInfo[m_wHeapTail][0]+m_cbHeapSparrowInfo[m_wHeapTail][1];
				if (cbHeapCount==HEAP_FULL_COUNT) m_wHeapTail=(m_wHeapTail+3)%CountArray(m_cbHeapSparrowInfo);
		
				//减少麻将
				m_cbLeftSparrowCount--;
				m_cbHeapSparrowInfo[m_wHeapTail][1]++;
		
				//堆立麻将
				var wHeapViewID:uint=SwitchViewChairID(m_wHeapTail);
				var wMinusHeadCount:uint=m_cbHeapSparrowInfo[m_wHeapTail][0];
				var wMinusLastCount:uint=m_cbHeapSparrowInfo[m_wHeapTail][1];
				GetPeerGameView().m_HeapSparrow[wHeapViewID].SetSparrowData(wMinusHeadCount,wMinusLastCount,HEAP_FULL_COUNT);
			}
		
			return;
		}
		
		//显示控制
		protected function ShowOperateControl(cbUserAction:uint, cbActionSparrow:uint):Boolean
		{
			//变量定义
			var GangSparrowResult:tagGangSparrowResult= new tagGangSparrowResult;
		
			//杠牌判断
			if ((cbUserAction&(WIK_GANG|WIK_FILL))!=0)
			{
				//桌面杆牌
				if (cbActionSparrow!=0)
				{
					GangSparrowResult.cbSparrowCount=1;
					GangSparrowResult.cbSparrowData[0]=cbActionSparrow;
				}
		
				//自己杆牌
				if (cbActionSparrow==0)
				{
					var wMeChairID:uint=GetMeChairID();
					m_GameLogic.AnalyseGangSparrow(m_cbSparrowIndex,m_WeaveItemArray[wMeChairID],m_cbWeaveCount[wMeChairID],GangSparrowResult);
				}
			}
		
			//显示界面
			GetPeerGameView().m_ControlMC.SetControlInfo(cbActionSparrow,cbUserAction,GangSparrowResult);
		
			return true;
		}
		
		//开始按钮
		override protected function OnStart(wParam:uint, lParam:uint):uint
		{
			//环境设置
			KillGameTimer(IDI_EXIT_GAME);
			GetPeerGameView().m_btStart.visible = false;
			GetPeerGameView().m_ControlMC.ShowWindow(false);
			GetPeerGameView().m_ScoreView.ShowWindow(false);
		
			//设置界面
			GetPeerGameView().SetHuangZhuang(false);
			GetPeerGameView().SetStatusFlag(false,false);
			GetPeerGameView().SetBankerUser(INVALID_CHAIR);
			GetPeerGameView().SetUserAction(INVALID_CHAIR,0);
			GetPeerGameView().SetOutSparrowInfo(INVALID_CHAIR,0);
		
			//麻将设置
			GetPeerGameView().m_UserSparrow[0].SetSparrowData(0,false);
			GetPeerGameView().m_UserSparrow[1].SetSparrowData(0,false);
			GetPeerGameView().m_UserSparrow[2].SetSparrowData(0,false);
			GetPeerGameView().m_HandSparrowControl.SetSparrowData(null,0,0);
		
			//麻将设置
			for (var i:uint=0;i<GAME_PLAYER;i++)
			{
				GetPeerGameView().m_TableSparrow[i].SetSparrowData(null,0);
				GetPeerGameView().m_DiscardSparrow[i].SetSparrowData(null,0);
				GetPeerGameView().m_WeaveSparrow[i][0].SetSparrowData(null,0);
				GetPeerGameView().m_WeaveSparrow[i][1].SetSparrowData(null,0);
				GetPeerGameView().m_WeaveSparrow[i][2].SetSparrowData(null,0);
				GetPeerGameView().m_WeaveSparrow[i][3].SetSparrowData(null,0);
			}
		
			//堆立麻将
			for (var i:uint=0;i<GAME_PLAYER;i++)
			{
				m_cbHeapSparrowInfo[i][0]=0;
				var v:uint = 26;
				if(i==m_wBankerUser)
					v = 28;
				else if((i+2)%4==m_wBankerUser)
					v = 28;
				m_cbHeapSparrowInfo[i][1]=HEAP_FULL_COUNT-v;
				GetPeerGameView().m_HeapSparrow[SwitchViewChairID(i)].SetSparrowData(m_cbHeapSparrowInfo[i][0],m_cbHeapSparrowInfo[i][1],HEAP_FULL_COUNT);
			}
		
			//状态变量
			m_bHearStatus=false;
		
			//游戏变量
			m_wCurrentUser=INVALID_CHAIR;
		
			//出牌信息
			m_cbOutSparrowData=0;
			m_wOutSparrowUser=INVALID_CHAIR;
			ZeroTwoDimensionArray(m_cbDiscardSparrow);
			ZeroArray(m_cbDiscardCount);
		
			//组合麻将
			ZeroArray(m_cbWeaveCount);
			EachTwoDimensionArray(m_WeaveItemArray,tagWeaveItem.ZeroMemory);
		
			//堆立麻将
			m_wHeapHand=0;
			m_wHeapTail=0;
			ZeroTwoDimensionArray(m_cbHeapSparrowInfo);
		
			//麻将变量
			m_cbLeftSparrowCount=0;
			ZeroArray(m_cbSparrowIndex);
		
			//发送消息
			SendUserReady(null,0);
		
			return 0;
		}
		
		//出牌操作
		protected function OnOutSparrow(wParam:uint, lParam:uint):uint
		{
			//出牌判断
			if ((IsLookonMode()==true)||(m_wCurrentUser!=GetMeChairID())) return 0;
		
			//听牌判断
			if ((m_bHearStatus==true)&&(GetPeerGameView().m_HandSparrowControl.GetCurrentSparrow()!=wParam)) return 0;
		
			//设置变量
			m_wCurrentUser=INVALID_CHAIR;
			var cbOutSparrowData:uint=wParam;
			m_GameLogic.RemoveSparrow2(m_cbSparrowIndex,cbOutSparrowData);
		
			//设置麻将
			var cbSparrowData:Array = new Array(MAX_COUNT);
			var cbSparrowCount:uint=m_GameLogic.SwitchToSparrowData2(m_cbSparrowIndex,cbSparrowData);
			GetPeerGameView().m_HandSparrowControl.SetSparrowData(cbSparrowData,cbSparrowCount,0);
		
			//设置界面
			KillGameTimer(IDI_OPERATE_SPARROW);
			GetPeerGameView().UpdateGameView();
			GetPeerGameView().SetStatusFlag(false,false);
			GetPeerGameView().SetUserAction(INVALID_CHAIR,0);
			GetPeerGameView().SetOutSparrowInfo(2,cbOutSparrowData);
			GetPeerGameView().m_ControlMC.ShowWindow(false);
		
			//播放声音
			PlaySparrowSound(cbOutSparrowData);
		
			//发送数据
			var OutSparrow:CMD_C_OutSparrow = new CMD_C_OutSparrow;
			OutSparrow.cbSparrowData=cbOutSparrowData;
			var sendData:ByteArray =  OutSparrow.toByteArray();
			SendGameData(SUB_C_OUT_SPARROW,sendData,CMD_C_OutSparrow.sizeof_CMD_C_OutSparrow);
		
			return 0;
		}
		
		//麻将操作
		protected function OnSparrowOperate(wParam:uint, lParam:uint):uint
		{
			//变量定义
			var cbOperateCode:uint=LOBYTE(wParam);
			var cbOperateSparrow:uint=LOBYTE(lParam);
		
			//状态判断
			if ((m_wCurrentUser==GetMeChairID())&&(cbOperateCode==WIK_NULL))
			{
				GetPeerGameView().m_ControlMC.ShowWindow(false);
				return 0;
			};
		
			//环境设置
			KillGameTimer(IDI_OPERATE_SPARROW);
			GetPeerGameView().SetStatusFlag(false,true);
			GetPeerGameView().m_ControlMC.ShowWindow(false);
		
			//发送命令
			var OperateSparrow:CMD_C_OperateSparrow = new CMD_C_OperateSparrow;
			OperateSparrow.cbOperateCode=cbOperateCode;
			OperateSparrow.cbOperateSparrow=cbOperateSparrow;
			var sendData:ByteArray =  OperateSparrow.toByteArray();
			SendGameData(SUB_C_OPERATE_SPARROW,sendData,CMD_C_OperateSparrow.sizeof_CMD_C_OperateSparrow);
		
			return 0;
		}
		///////////////////////
		protected function OnGameViewEevent(e:GameViewEvent):void
		{
			switch(e.m_nMsg)
			{
				case GameViewEvent.nGVM_START:
				{
					OnStart(e.m_nWParam,e.m_nLParam);
					return;
				}
				case GameViewEvent.nGVM_OUT_SPARROW:
				{
					OnOutSparrow(e.m_nWParam,e.m_nLParam);
					return;
				}
				case GameViewEvent.nGVM_SPARROW_OPERATE:
				{
					OnSparrowOperate(e.m_nWParam,e.m_nLParam);
					return;
				}
			}
		}

	}
}
include "./Common/GameLogicDef.as"
include "./Common/CMD_SPARROW.as"
include "./SparrowDef.as"
include "../../Common/Memory.as"
include "../../Net/GlobalDef.as"
include "../../Net/GlobalFrame.as"