package 
{
	import flash.display.MovieClip;
	import flash.utils.ByteArray;
	import flash.geom.Point;

	import org.god.IGameFrame.*;
	import org.god.GameModule.Common.*;
	import org.god.Net.*;
	import org.god.GameModule.B_Upgrade.Common.*;

	public class GameClass extends GameBaseClass
	{
		//时间标识
		protected static  var IDI_OUT_CARD:uint=200;//出牌定时器
		protected static  var IDI_CALL_CARD:uint=201;//叫牌定时器
		protected static  var IDI_EXIT_GAME:uint=202;//离开游戏定时器
		protected static  var IDI_SEND_CONCEAL:uint=203;//底牌定时器

		//时间范围
		protected static  var TIME_OUT_CARD:uint=30;//出牌时间
		protected static  var TIME_CALL_CARD:uint=10;//叫牌时间
		protected static  var TIME_START_GAME:uint=30;//开始时间
		protected static  var TIME_SEND_CONCEAL:uint=40;//留底时间

		//功能定时器
		protected static  var IDI_CONCEAL_CARD:uint=300;//底牌定时器
		protected static  var IDI_DISPATCH_CARD:uint=301;//发牌定时器
		protected static  var IDI_LAST_TURN_CARD:uint=302;//上轮定时器
		protected static  var IDI_THROW_CARD_RESULT:uint=303;//甩牌定时器

		//逻辑变量
		protected var m_cbPackCount:uint;//牌副数目
		protected var m_cbMainColor:uint;//主牌花色
		protected var m_cbMainValue:uint;//主牌数值

		//叫牌信息
		protected var m_cbCallCard:uint;//叫牌扑克
		protected var m_cbCallCount:uint;//叫牌数目
		protected var m_wCallCardUser:uint;//叫牌用户

		//状态变量
		protected var m_wBankerUser:uint;//庄家用户
		protected var m_wCurrentUser;//当前用户

		//得分信息
		protected var m_cbScoreCardCount:uint;//分牌数目
		protected var m_cbScoreCardData:Array = new Array(12*2);//得分扑克

		//甩牌变量
		protected var m_bThrowCard:Boolean;//甩牌标志
		protected var m_wThrowCardUser:uint;//甩牌用户
		protected var m_cbResultCardCount:uint;//还原扑克
		protected var m_cbResultCardData:Array = new Array(MAX_COUNT);//还原扑克

		//出牌变量
		protected var m_bLastTurn:Boolean;//上轮标志
		protected var m_wFirstOutUser:uint;//出牌用户
		protected var m_cbOutCardCount:Array = new Array(4);//出牌数目
		protected var m_cbOutCardData:Array = new Array(4);//出牌列表
		protected var m_bCardCount:Array=new Array(4);//扑克数目

		//底牌扑克
		protected var m_cbConcealCount:uint;//暗藏数目
		protected var m_cbConcealCard:Array = new Array(8);//暗藏扑克

		//用户扑克
		protected var m_cbHandCardCount:uint;//扑克数目
		protected var m_cbHandCardData:Array = new Array(MAX_COUNT);//手上扑克

		//配置变量
		protected var m_bStustee:Boolean;//托管标志
		protected var m_bDeasilOrder:Boolean;//出牌顺序
		protected var m_dwCardHSpace:uint;//扑克象素

		//辅助变量
		protected var m_cbRemnantCardCount:uint;//剩余数目
		protected var m_cbDispatchCardCount:uint;//派发数目
		protected var m_cbDispatchCardData:Array = new Array(MAX_COUNT);//派发扑克

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
			if (super.InitGameClass() == false)
			{
				return false;
			}
			for (var i:uint = 0; i < m_cbOutCardCount.length; i ++)
			{
				m_cbOutCardCount[i] = new Array(2);
			}
			for (i = 0; i < m_cbOutCardData.length; i ++)
			{
				m_cbOutCardData[i] = new Array(2);
				for (var k:uint = 0; k < m_cbOutCardData[i].length; k ++)
				{
					m_cbOutCardData[i][k] = new Array(MAX_COUNT);
				}
			}
			m_GameLogic = new GameLogic;
			
			//逻辑变量
			m_cbPackCount=0;
			m_cbMainColor=COLOR_ERROR;
			m_cbMainValue=VALUE_ERROR;

			//叫牌信息
			m_cbCallCard=0;
			m_cbCallCount=0;
			m_wCallCardUser=INVALID_CHAIR;

			//状态变量
			m_wBankerUser=INVALID_CHAIR;
			m_wCurrentUser=INVALID_CHAIR;
	
			//得分信息
			m_cbScoreCardCount=0;
			ZeroArray(m_cbScoreCardData);

			//甩牌变量
			m_bThrowCard=false;
			m_cbResultCardCount=0;
			m_wThrowCardUser=INVALID_CHAIR;
			ZeroArray(m_cbResultCardData);

			//出牌变量
			m_bLastTurn=false;
			m_wFirstOutUser=INVALID_CHAIR;
			ZeroThreeDimensionArray(m_cbOutCardData);
			ZeroTwoDimensionArray(m_cbOutCardCount);

			//底牌扑克
			m_cbConcealCount=0;
			ZeroArray(m_cbConcealCard);

			//用户扑克
			m_cbHandCardCount=0;
			ZeroArray(m_cbHandCardData);
			ZeroArray(m_bCardCount);

			//配置变量
			m_bStustee=false;

			//辅助变量
			m_cbRemnantCardCount=0;
			m_cbDispatchCardCount=0;
			ZeroArray(m_cbDispatchCardData);

			GetPeerGameView().addEventListener(GameViewEvent.strGameViewEvent, 
			   OnGameViewEevent);

			AddGameSound("GAME_END",SOUND_GAME_END);
			AddGameSound("GAME_LOST",SOUND_GAME_LOST);
			AddGameSound("GAME_START",SOUND_GAME_START);
			AddGameSound("GAME_WARN",SOUND_GAME_WARN);
			AddGameSound("GAME_WIN",SOUND_GAME_WIN);
			AddGameSound("OUT_CARD",SOUND_OUT_CARD);
			AddGameSound("CALL_CARD",SOUND_CALL_CARD);
			AddGameSound("THROW_RESULT",SOUND_THROW_RESULT);
			
			return true;
		}
		//重置状态
		override public function DestroyGameClass():void
		{
			GetPeerGameView().removeEventListener(GameViewEvent.strGameViewEvent,
			  OnGameViewEevent);

			m_cbScoreCardData = null;
			m_cbResultCardData= null;
			m_cbOutCardCount  = null;
			m_cbOutCardData   = null;
			m_cbConcealCard	  = null;
			m_cbHandCardData  = null;
			m_cbDispatchCardData = null;
			m_GameLogic = null;
			m_bCardCount= null;

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
					if ((nElapse==0)||((m_bStustee==true)&&(nElapse<(TIME_OUT_CARD))))
					{
						if(GetGameStatus() == GS_UG_PLAY)
						{
							if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) 
								AutomatismOutCard();
						}
						return true;
					}
		
					//播放声音
					if ((nElapse<=3)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound("GAME_WARN");
		
					return true;
				}
			case IDI_CALL_CARD:			//用户叫牌
				{
					//超时判断
					if ((nElapse==0)||((m_bStustee==true)&&(nElapse<(TIME_CALL_CARD))))
					{
						//隐藏控件
						GetPeerGameView().m_CallCardWnd.ShowWindow(false);
						GetPeerGameView().m_CallCardWnd.UpdateCallControl1(CallCardWnd.CALL_NONE);
		
						//发送消息
						if (IsLookonMode()==false && GetGameStatus() == GS_UG_CALL) SendGameData(SUB_C_CALL_FINISH);
		
						return true;
					}
		
					return true;
				}
			case IDI_EXIT_GAME:		//离开游戏
				{
					if ((nElapse==0)||((m_bStustee==true)&&(nElapse<(TIME_START_GAME))))
					{
						if ((IsLookonMode()==false)&&(wChairID==GetMeChairID())) OnExit(0,0);
						return true;
					}
					if ((nElapse<=3)&&(wChairID==GetMeChairID())&&(IsLookonMode()==false)) PlayGameSound("GAME_WARN");
		
					return true;
				}
			case IDI_SEND_CONCEAL:		//发送底牌
				{
					if ((nElapse==0)||((m_bStustee==true)&&(nElapse<(TIME_SEND_CONCEAL))))
					{
						//自动留底
						if ((IsLookonMode()==false)&&(wChairID==GetMeChairID()))
						{
							//删除定时器
							KillGameTimer(IDI_SEND_CONCEAL);
		
							//删除扑克
							m_cbHandCardCount-=m_cbConcealCount;
							m_GameLogic.RemoveCard(m_cbConcealCard,m_cbConcealCount,m_cbHandCardData,m_cbHandCardCount+m_cbConcealCount);
		
							//设置界面
							GetPeerGameView().m_btSendConceal.ShowWindow(false);
							GetPeerGameView().m_btSendConceal.EnableWindow(false);
							SetHandCardControl(m_cbHandCardData,m_cbHandCardCount);
		
							//构造数据
							var ConcealCard:CMD_C_ConcealCard = new CMD_C_ConcealCard;
							ConcealCard.cbConcealCount=m_cbConcealCount;
							CopyArray(ConcealCard.cbConcealCard,m_cbConcealCard,m_cbConcealCount);
		
							//发送数据
							var sendData:ByteArray = ConcealCard.toByteArray();
							SendGameData(SUB_C_CONCEAL_CARD,sendData,CMD_C_ConcealCard.sizeof_CMD_C_ConcealCard);
		
							//提示信息
							if (m_bStustee==false) InsertSystemString("限定的时间到了，系统为您自动埋底牌");
		
							return false;
						}
		
						return true;
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
			case SUB_S_SEND_CARD:		//发送扑克
				{
					return OnSubSendCard(pBuffer,wDataSize);
				}
			case SUB_S_CALL_CARD:		//用户叫牌
				{
					return OnSubCallCard(pBuffer,wDataSize);
				}
			case SUB_S_SEND_CONCEAL:	//发送底牌
				{
					return OnSubSendConceal(pBuffer,wDataSize);
				}
			case SUB_S_GAME_PLAY:		//游戏开始
				{
					return OnSubGamePlay(pBuffer,wDataSize);
				}
			case SUB_S_OUT_CARD:		//用户出牌
				{
					return OnSubOutCard(pBuffer,wDataSize);
				}
			case SUB_S_THROW_RESULT:	//甩牌结果
				{
					return OnSubThrowResult(pBuffer,wDataSize);
				}
			case SUB_S_TURN_BALANCE:	//一轮结算
				{
					return OnSubTurnBalance(pBuffer,wDataSize);
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
				case GS_UG_FREE:	//空闲状态
				{
					//效验数据
					if (wDataSize!=CMD_S_StatusFree.sizeof_CMD_S_StatusFree) return false;
					var pStatusFree:CMD_S_StatusFree=CMD_S_StatusFree.readData(pBuffer);
		
					//设置状态
					SetGameStatus(GS_UG_FREE);
					
					//设置控件
					if (IsLookonMode()==false)
					{
						GetPeerGameView().m_btStart.ShowWindow(true);
						GetPeerGameView().m_btStart.SetFocus();
					}
		
					//玩家设置
					if (IsLookonMode()==false)
					{
						GetPeerGameView().m_btStusteeControl.EnableWindow(true);
						GetPeerGameView().m_HandCardControl.SetDisplayFlag(true);
					}
		
					return true;
				}
			case GS_UG_CALL:	//叫牌状态
				{
					//效验数据
					if (wDataSize!=CMD_S_StatusCall.sizeof_CMD_S_StatusCall) return false;
					var pStatusCall:CMD_S_StatusCall=CMD_S_StatusCall.readData(pBuffer);
					
					//设置状态
					SetGameStatus(GS_UG_CALL);
			
					//逻辑变量
					m_cbPackCount=pStatusCall.cbPackCount;
					m_cbMainValue=pStatusCall.cbMainValue;
					m_wBankerUser=pStatusCall.wBankerUser;
		
					//叫牌信息
					m_cbCallCard=pStatusCall.cbCallCard;
					m_cbCallCount=pStatusCall.cbCallCount;
					m_wCallCardUser=pStatusCall.wCallCardUser;
		
					//设置扑克
					m_cbHandCardCount=pStatusCall.cbCardCount;
					CopyArray(m_cbHandCardData,pStatusCall.cbCardData,m_cbHandCardCount);
					CopyArray(m_bCardCount,pStatusCall.cbHandCardCount);
					FocusUpdateLeaveCardControl();
		
					//逻辑组件
					m_GameLogic.SetPackCount(m_cbPackCount);
					m_GameLogic.SetMainColor(m_cbMainColor);
					m_GameLogic.SetMainValue(m_cbMainValue);
		
					//主牌信息
					var wMeChairID:uint=GetMeChairID();
					var cbValueMySelf:uint=pStatusCall.cbValueOrder[wMeChairID%2];
					var cbValueOther:uint=pStatusCall.cbValueOrder[(wMeChairID+1)%2];
					GetPeerGameView().SetValueOrder(m_cbMainValue,cbValueMySelf,cbValueOther);
		
					//叫牌信息
					if (m_wCallCardUser!=INVALID_CHAIR)
					{
						var wViewChairID:uint=SwitchViewChairID(m_wCallCardUser);
						var cbCallColor:uint=m_GameLogic.GetCardColor(m_cbCallCard);
						GetPeerGameView().SetCallCardInfo(wViewChairID,cbCallColor,m_cbCallCount);
					}
		
					//叫牌界面
					var cbCallCard:Array = new Array(2);
					var wViewChairID:uint=SwitchViewChairID(m_wCallCardUser);
					for (var i:uint=0;i<m_cbCallCount;i++) cbCallCard[i]=m_cbCallCard;
					GetPeerGameView().m_UserCardControl[wViewChairID].SetCardData(cbCallCard,m_cbCallCount);
		
					//设置庄家
					if (m_wBankerUser!=INVALID_CHAIR) GetPeerGameView().SetBankerUser(SwitchViewChairID(m_wBankerUser));
		
					//玩家设置
					if (IsLookonMode()==false) 
					{
						GetPeerGameView().m_btStusteeControl.EnableWindow(true);
						GetPeerGameView().m_HandCardControl.SetDisplayFlag(true);
					}
		
					//扑克设置
					if (pStatusCall.cbComplete==false)
					{
						//控制设置
						GetPeerGameView().m_CallCardWnd.ShowWindow(true);
						GetPeerGameView().m_CallCardWnd.UpdateCallControl1(CallCardWnd.CALL_NONE);
		
						//派发扑克
						DispatchUserCard(m_cbHandCardData,m_cbHandCardCount);
					}
					else
					{
						//排列扑克
						m_GameLogic.SortCardList(m_cbHandCardData,m_cbHandCardCount);
		
						//设置扑克
						SetHandCardControl(m_cbHandCardData,m_cbHandCardCount);
					}
		
					return true;
				}
			case GS_UG_BACK:	//留底状态
				{
					//效验数据
					if (wDataSize!=CMD_S_StatusBack.sizeof_CMD_S_StatusBack) return false;
					var pStatusBack:CMD_S_StatusBack=CMD_S_StatusBack.readData(pBuffer);
		
					//设置状态
					SetGameStatus(GS_UG_BACK);
					
					//逻辑变量
					m_cbPackCount=pStatusBack.cbPackCount;
					m_cbMainColor=pStatusBack.cbMainColor;
					m_cbMainValue=pStatusBack.cbMainValue;
		
					//叫牌信息
					m_cbCallCard=pStatusBack.cbCallCard;
					m_cbCallCount=pStatusBack.cbCallCount;
					m_wCallCardUser=pStatusBack.wCallCardUser;
		
					//用户变量
					m_wBankerUser=pStatusBack.wBankerUser;
					m_wCurrentUser=pStatusBack.wCurrentUser;
					m_wCallCardUser=pStatusBack.wCallCardUser;
		
					//逻辑组件
					m_GameLogic.SetPackCount(m_cbPackCount);
					m_GameLogic.SetMainColor(m_cbMainColor);
					m_GameLogic.SetMainValue(m_cbMainValue);
		
					//设置底牌
					m_cbConcealCount=pStatusBack.cbConcealCount;
					CopyArray(m_cbConcealCard,pStatusBack.cbConcealCard,m_cbConcealCount);
		
					//设置扑克
					m_cbHandCardCount=pStatusBack.cbCardCount;
					CopyArray(m_cbHandCardData,pStatusBack.cbCardData,m_cbHandCardCount);
					CopyArray(m_bCardCount,pStatusBack.cbHandCardCount);
					FocusUpdateLeaveCardControl();
		
					//主牌信息
					var wMeChairID:uint=GetMeChairID();
					var cbValueMySelf:uint=pStatusBack.cbValueOrder[wMeChairID%2];
					var cbValueOther:uint=pStatusBack.cbValueOrder[(wMeChairID+1)%2];
					GetPeerGameView().SetValueOrder(m_cbMainValue,cbValueMySelf,cbValueOther);
		
					//叫牌信息
					if (m_wCallCardUser!=INVALID_CHAIR)
					{
						var wViewChairID:uint=SwitchViewChairID(m_wCallCardUser);
						var cbCallColor:uint=m_GameLogic.GetCardColor(m_cbCallCard);
						GetPeerGameView().SetCallCardInfo(wViewChairID,cbCallColor,m_cbCallCount);
					}
		
					//排列扑克
					m_GameLogic.SortCardList(m_cbHandCardData,m_cbHandCardCount);
		
					//界面设置
					SetHandCardControl(m_cbHandCardData,m_cbHandCardCount);
					GetPeerGameView().SetBankerUser(SwitchViewChairID(m_wBankerUser));
		
					//玩家设置
					if (IsLookonMode()==false) 
					{
						//扑克设置
						GetPeerGameView().m_btStusteeControl.EnableWindow(true);
						GetPeerGameView().m_HandCardControl.SetDisplayFlag(true);
		
						//留底设置
						if (m_wCurrentUser==GetMeChairID())
						{
							GetPeerGameView().m_btSendConceal.EnableWindow(true);
							GetPeerGameView().m_btSendConceal.ShowWindow(true);
							GetPeerGameView().m_HandCardControl.SetPositively(true);
							GetPeerGameView().m_HandCardControl.SetShootCard(m_cbConcealCard,m_cbConcealCount);
						}
						else GetPeerGameView().SetWaitConceal(true);
					}
					else GetPeerGameView().SetWaitConceal(true);
		
					//时间设置
					SetGameTimer(m_wCurrentUser,IDI_SEND_CONCEAL,TIME_SEND_CONCEAL);
		
					return true;
				}
			case GS_UG_PLAY:	//游戏状态
				{
					//效验数据
					if (wDataSize!=CMD_S_StatusPlay.sizeof_CMD_S_StatusPlay) return false;
					var pStatusPlay:CMD_S_StatusPlay=CMD_S_StatusPlay.readData(pBuffer);
		
					//设置状态
					SetGameStatus(GS_UG_PLAY);
					
					//逻辑变量
					m_cbPackCount=pStatusPlay.cbPackCount;
					m_cbMainColor=pStatusPlay.cbMainColor;
					m_cbMainValue=pStatusPlay.cbMainValue;
		
					//叫牌信息
					m_cbCallCard=pStatusPlay.cbCallCard;
					m_cbCallCount=pStatusPlay.cbCallCount;
					m_wCallCardUser=pStatusPlay.wCallCardUser;
		
					//用户变量
					m_wBankerUser=pStatusPlay.wBankerUser;
					m_wCurrentUser=pStatusPlay.wCurrentUser;
					m_wFirstOutUser=pStatusPlay.wFirstOutUser;
		
					//逻辑组件
					m_GameLogic.SetPackCount(m_cbPackCount);
					m_GameLogic.SetMainColor(m_cbMainColor);
					m_GameLogic.SetMainValue(m_cbMainValue);
		
					//设置底牌
					m_cbConcealCount=pStatusPlay.cbConcealCount;
					CopyArray(m_cbConcealCard,pStatusPlay.cbConcealCard,m_cbConcealCount);
		
					//设置扑克
					m_cbHandCardCount=pStatusPlay.cbCardCount;
					CopyArray(m_cbHandCardData,pStatusPlay.cbCardData,m_cbHandCardCount);
					CopyArray(m_bCardCount,pStatusPlay.cbHandCardCount);
					FocusUpdateLeaveCardControl();
		
					//出牌变量
					for (var i:uint=0;i<GAME_PLAYER;i++)
					{
						m_cbOutCardCount[i][0]=pStatusPlay.cbOutCardCount[i];
						CopyArray(m_cbOutCardData[i][0],pStatusPlay.cbOutCardData[i],m_cbOutCardCount[i][0]);
					}
		
					//得分信息
					m_cbScoreCardCount=pStatusPlay.cbScoreCardCount;
					CopyArray(m_cbScoreCardData,pStatusPlay.cbScoreCardData,m_cbScoreCardCount);
		
					//主牌信息
					var wMeChairID:uint=GetMeChairID();
					var cbValueMySelf:uint=pStatusPlay.cbValueOrder[wMeChairID%2];
					var cbValueOther:uint=pStatusPlay.cbValueOrder[(wMeChairID+1)%2];
					GetPeerGameView().SetValueOrder(m_cbMainValue,cbValueMySelf,cbValueOther);
		
					//叫牌信息
					if (m_wCallCardUser!=INVALID_CHAIR)
					{
						var wViewChairID:uint=SwitchViewChairID(m_wCallCardUser);
						var cbCallColor:uint=m_GameLogic.GetCardColor(m_cbCallCard);
						GetPeerGameView().SetCallCardInfo(wViewChairID,cbCallColor,m_cbCallCount);
					}
		
					//得分视图
					GetPeerGameView().m_CardScore.SetCardData(m_cbScoreCardData,m_cbScoreCardCount);
					GetPeerGameView().SetCardScore(m_GameLogic.GetCardScore(m_cbScoreCardData,m_cbScoreCardCount));
		
					//出牌界面
					for (var i:uint=0;i<GAME_PLAYER;i++)
					{
						var wViewChairID:uint=SwitchViewChairID(i);
						GetPeerGameView().m_UserCardControl[wViewChairID].SetCardData(m_cbOutCardData[i][0],m_cbOutCardCount[i][0]);
					}
		
					//排列扑克
					m_GameLogic.SortCardList(m_cbHandCardData,m_cbHandCardCount);
		
					//界面设置
					SetHandCardControl(m_cbHandCardData,m_cbHandCardCount);
					GetPeerGameView().SetBankerUser(SwitchViewChairID(m_wBankerUser));
		
					//玩家设置
					if (IsLookonMode()==false) 
					{
						GetPeerGameView().m_btStusteeControl.EnableWindow(true);
						GetPeerGameView().m_HandCardControl.SetPositively(true);
						GetPeerGameView().m_HandCardControl.SetDisplayFlag(true);
					}
		
					//旁观设置
					if (IsAllowLookon()==true) GetPeerGameView().m_HandCardControl.SetDisplayFlag(true);
		
					//出牌设置
					if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
					{
						GetPeerGameView().m_btOutCard.ShowWindow(true);
						GetPeerGameView().m_btOutPrompt.ShowWindow(true);
						GetPeerGameView().m_btOutCard.EnableWindow((VerdictOutCard()==true)?true:false);
					}
		
					//功能按钮
					GetPeerGameView().m_btConcealCard.EnableWindow(((m_cbConcealCount>0)&&((IsLookonMode()==false)||(IsAllowLookon()==true)))?true:false);
		
					//设置底牌
					GetPeerGameView().m_ConcealCardView.SetConcealCard(m_cbConcealCard,m_cbConcealCount);
					GetPeerGameView().m_btConcealCard.EnableWindow(((m_cbConcealCount!=0)&&((IsLookonMode()==false)||(IsAllowLookon()==true)))?true:false);

					//时间设置
					SetGameTimer(m_wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);
		
					return true;
				}
			}

			return false;
		}
		//旁观状态
		override protected function OnEventLookonChanged(bLookonUser:Boolean,pBuffer:ByteArray,wDataSize:uint):void
		{
			GetPeerGameView().m_HandCardControl.SetDisplayFlag((IsAllowLookon()==true));
			//功能按钮
			GetPeerGameView().m_btConcealCard.EnableWindow(((m_cbConcealCount>0)&&((IsLookonMode()==false)||IsAllowLookon()))?true:false);
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
		
			//设置状态
			SetGameStatus(GS_UG_CALL);
		
			//逻辑变量
			m_cbMainColor=COLOR_ERROR;
			m_cbMainValue=pGameStart.cbMainValue;
			m_cbPackCount=pGameStart.cbPackCount;
		
			//叫牌变量
			m_cbCallCard=0;
			m_cbCallCount=0;
			m_wCallCardUser=INVALID_CHAIR;
		
			//底牌变量
			m_cbConcealCount=0;
			ZeroArray(m_cbConcealCard);
		
			//状态变量
			m_wCurrentUser=INVALID_CHAIR;
			m_wBankerUser=pGameStart.wBankerUser;
		
			//得分信息
			m_cbScoreCardCount=0;
			ZeroArray(m_cbScoreCardData);
		
			//出牌变量
			m_wFirstOutUser=INVALID_CHAIR;
			ZeroThreeDimensionArray(m_cbOutCardData);
			ZeroTwoDimensionArray(m_cbOutCardCount);
		
			//逻辑组件
			m_GameLogic.SetPackCount(m_cbPackCount);
			m_GameLogic.SetMainColor(m_cbMainColor);
			m_GameLogic.SetMainValue(m_cbMainValue);
		
			//界面设置
			GetPeerGameView().SetCardScore(0xFFFF);
			GetPeerGameView().m_CardScore.SetCardData(null,0);
			GetPeerGameView().SetCallCardInfo(INVALID_CHAIR,COLOR_ERROR,0);
		
			//底牌控制
			GetPeerGameView().m_btConcealCard.EnableWindow(false);
			GetPeerGameView().m_ConcealCardView.ShowWindow(false);
			GetPeerGameView().m_ConcealCardView.SetConcealCard(null,0);
		
			//主牌信息
			var wMeChairID:uint=GetMeChairID();
			var cbValueMySelf:uint=pGameStart.cbValueOrder[wMeChairID%2];
			var cbValueOther:uint=pGameStart.cbValueOrder[(wMeChairID+1)%2];
			GetPeerGameView().SetValueOrder(m_cbMainValue,cbValueMySelf,cbValueOther);
		
			//设置庄家
			if (m_wBankerUser==INVALID_CHAIR) GetPeerGameView().SetBankerUser(INVALID_CHAIR);
			else GetPeerGameView().SetBankerUser(SwitchViewChairID(m_wBankerUser));
		
			//设置界面
			if (IsLookonMode()==true)
			{
				SetHandCardControl(null,0);
				GetPeerGameView().SetWaitConceal(false);
				GetPeerGameView().m_ScoreView.ShowWindow(false);
				GetPeerGameView().m_CardScore.SetCardData(null,0);
				GetPeerGameView().m_UserCardControl[0].SetCardData(null,0);
				GetPeerGameView().m_UserCardControl[1].SetCardData(null,0);
				GetPeerGameView().m_UserCardControl[2].SetCardData(null,0);
				GetPeerGameView().m_UserCardControl[3].SetCardData(null,0);
				
				GetPeerGameView().m_LeaveCardControl[0].SetCardData(null,0);
				GetPeerGameView().m_LeaveCardControl[1].SetCardData(null,0);
				GetPeerGameView().m_LeaveCardControl[2].SetCardData(null,0);
			}
		
			//控制设置
			var bPlayerMode:Boolean=(IsLookonMode()==false);
			GetPeerGameView().m_HandCardControl.SetDisplayFlag(bPlayerMode);
			if ((IsLookonMode()==false)&&(m_bStustee==false)) ActiveGameFrame();
		
			//环境设置
			PlayGameSound("GAME_START");
		
			return true;
		}
		
		//发送扑克
		 protected function OnSubSendCard(pBuffer:ByteArray,wDataSize:uint):Boolean
		{
			//变量定义
			var pSendCard:CMD_S_SendCard=CMD_S_SendCard.readData(pBuffer);
			var wFirstSize:uint=CMD_S_SendCard.sizeof_CMD_S_SendCard
			-CMD_S_SendCard.sizeof_cbCardData;
		
			//效验数据
			ASSERT(wDataSize>=wFirstSize);
			if (wDataSize<wFirstSize) return false;
			ASSERT(wDataSize==(wFirstSize+pSendCard.cbCardCount));
			if (wDataSize!=(wFirstSize+pSendCard.cbCardCount)) return false;
		
			//设置扑克
			m_cbHandCardCount=pSendCard.cbCardCount;
			CopyArray(m_cbHandCardData,pSendCard.cbCardData,m_cbHandCardCount);
			CopyArray(m_bCardCount,pSendCard.cbHandCardCount);
			FocusUpdateLeaveCardControl();
		
			//更新控制
			GetPeerGameView().m_CallCardWnd.ShowWindow(true);
			GetPeerGameView().m_CallCardWnd.UpdateCallControl1(CallCardWnd.CALL_NONE);
		
			//派发扑克
			DispatchUserCard(m_cbHandCardData,m_cbHandCardCount);
		
			//环境设置
			if ((IsLookonMode()==false)&&(m_bStustee==false)) ActiveGameFrame();
		
			return true;
		}
		
		//用户叫牌
		protected function OnSubCallCard(pBuffer:ByteArray,wDataSize:uint):Boolean
		{
			//效验数据
			ASSERT(wDataSize==CMD_S_CallCard.sizeof_CMD_S_CallCard);
			if (wDataSize!=CMD_S_CallCard.sizeof_CMD_S_CallCard) return false;
		
			//消息处理
			var pCallCard:CMD_S_CallCard=CMD_S_CallCard.readData(pBuffer);
		
			//清理叫牌
			if ((m_wCallCardUser!=INVALID_CHAIR)&&(m_wCallCardUser!=pCallCard.wCallCardUser))
			{
				var wViewChairID:uint=SwitchViewChairID(m_wCallCardUser);
				GetPeerGameView().m_UserCardControl[wViewChairID].SetCardData(null,0);
			}
		
			//设置变量
			m_cbCallCard=pCallCard.cbCallCard;
			m_cbCallCount=pCallCard.cbCallCount;
			m_wCallCardUser=pCallCard.wCallCardUser;
		
			//叫牌界面
			var cbCallCard:Array = new Array(2);
			var wViewChairID:uint=SwitchViewChairID(m_wCallCardUser);
			for (var i:uint=0;i<m_cbCallCount;i++) cbCallCard[i]=m_cbCallCard;
			GetPeerGameView().m_UserCardControl[wViewChairID].SetCardData(cbCallCard,m_cbCallCount);
		
			//视图信息
			var cbCallColor:uint=m_GameLogic.GetCardColor(m_cbCallCard);
			GetPeerGameView().SetCallCardInfo(wViewChairID,cbCallColor,m_cbCallCount);
		
			//播放声音
			PlayGameSound("CALL_CARD");
		
			//更新控制
			UpdateCallCardControl();
		
			return true;
		}
		
		//发送底牌
		protected function OnSubSendConceal(pBuffer:ByteArray,wDataSize:uint):Boolean
		{
			//效验数据
			ASSERT(wDataSize==CMD_S_SendConceal.sizeof_CMD_S_SendConceal);
			if (wDataSize!=CMD_S_SendConceal.sizeof_CMD_S_SendConceal) return false;
		
			//消息处理
			var pSendConceal:CMD_S_SendConceal=CMD_S_SendConceal.readData(pBuffer);
		
			//设置变量
			m_cbMainColor=pSendConceal.cbMainColor;
			m_wBankerUser=pSendConceal.wBankerUser;
			m_wCurrentUser=pSendConceal.wCurrentUser;
		
			//状态设置
			SetGameStatus(GS_UG_BACK);
			m_GameLogic.SetMainColor(m_cbMainColor);
			GetPeerGameView().SetBankerUser(SwitchViewChairID(m_wBankerUser));
		
			//叫牌清理
			if (m_wCallCardUser!=INVALID_CHAIR)
			{
				var wViewChairID:uint=SwitchViewChairID(m_wCallCardUser);
				GetPeerGameView().m_UserCardControl[wViewChairID].SetCardData(null,0);
			}
		
			//扑克设置
			if (m_wCurrentUser==GetMeChairID())
			{
				//设置底牌
				m_cbConcealCount=pSendConceal.cbConcealCount;
				CopyArray(m_cbConcealCard,pSendConceal.cbConcealCard,m_cbConcealCount);
		
				//设置扑克
				CopyArray(m_cbHandCardData,
					m_cbConcealCard,m_cbConcealCount,m_cbHandCardCount);
				m_cbHandCardCount+=m_cbConcealCount;
		
				//设置扑克
				m_GameLogic.SortCardList(m_cbHandCardData,m_cbHandCardCount);
				SetHandCardControl(m_cbHandCardData,m_cbHandCardCount);
		
				//设置控制
				if (IsLookonMode()==false) 
				{
					//激活框架
					if (m_bStustee==false) ActiveGameFrame();
		
					//设置控件
					GetPeerGameView().m_btSendConceal.EnableWindow(true);
					GetPeerGameView().m_btSendConceal.ShowWindow(true);
					GetPeerGameView().m_HandCardControl.SetPositively(true);
					GetPeerGameView().m_HandCardControl.SetShootCard(m_cbConcealCard,m_cbConcealCount);
				}
				else 
				{
					//设置提示
					GetPeerGameView().SetWaitConceal(true);
				}
			}
			else
			{
				//设置提示
				GetPeerGameView().SetWaitConceal(true);
		
				//设置扑克
				m_GameLogic.SortCardList(m_cbHandCardData,m_cbHandCardCount);
				SetHandCardControl(m_cbHandCardData,m_cbHandCardCount);
			}
		
			//环境设置
			SetGameTimer(m_wCurrentUser,IDI_SEND_CONCEAL,TIME_SEND_CONCEAL);
		
			return true;
		}
		
		//游戏开始
		protected function OnSubGamePlay(pBuffer:ByteArray,wDataSize:uint):Boolean
		{
			//效验数据
			ASSERT(wDataSize==CMD_S_GamePlay.sizeof_CMD_S_GamePlay);
			if (wDataSize!=CMD_S_GamePlay.sizeof_CMD_S_GamePlay) return false;
		
			//消息处理
			var pGamePlay:CMD_S_GamePlay=CMD_S_GamePlay.readData(pBuffer);
		
			//设置变量
			m_wCurrentUser=pGamePlay.wCurrentUser;
			m_wFirstOutUser=pGamePlay.wCurrentUser;
			m_cbConcealCount=pGamePlay.cbConcealCount;
			CopyArray(m_cbConcealCard,pGamePlay.cbConcealCard,m_cbConcealCount);
			
			GetPeerGameView().m_UserCardControl[0].SetCardData(null,0);
			GetPeerGameView().m_UserCardControl[1].SetCardData(null,0);
			GetPeerGameView().m_UserCardControl[2].SetCardData(null,0);
			GetPeerGameView().m_UserCardControl[3].SetCardData(null,0);

			//底牌设置
			if ((IsLookonMode()==true)&&(m_cbConcealCount!=0))
			{
				//删除扑克
				m_cbHandCardCount-=m_cbConcealCount;
				m_GameLogic.RemoveCard(m_cbConcealCard,m_cbConcealCount,m_cbHandCardData,m_cbHandCardCount+m_cbConcealCount);
		
				//设置扑克
				SetHandCardControl(m_cbHandCardData,m_cbHandCardCount);
			}
		
			//设置底牌
			GetPeerGameView().m_ConcealCardView.SetConcealCard(m_cbConcealCard,m_cbConcealCount);
			GetPeerGameView().m_btConcealCard.EnableWindow(((m_cbConcealCount!=0)&&((IsLookonMode()==false)||(IsAllowLookon()==true)))?true:false);
		
			//设置状态
			SetGameStatus(GS_UG_PLAY);
			GetPeerGameView().SetCardScore(0);
			GetPeerGameView().SetWaitConceal(false);
		
			//控制设置
			var bPlayerMode:Boolean=(IsLookonMode()==false);
			GetPeerGameView().m_HandCardControl.SetPositively(bPlayerMode);
		
			//当前玩家
			if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
			{
				if (m_bStustee==false) ActiveGameFrame();
				GetPeerGameView().m_btOutCard.ShowWindow(true);
				GetPeerGameView().m_btOutCard.EnableWindow(false);
				GetPeerGameView().m_btOutPrompt.ShowWindow(true);
			}
		
			//环境设置
			SetGameTimer(m_wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);
			PlayGameSound("GAME_START");
		
			return true;
		}
		
		//用户出牌
		protected function OnSubOutCard(pBuffer:ByteArray,wDataSize:uint):Boolean
		{
			//变量定义
			var pOutCard:CMD_S_OutCard=CMD_S_OutCard.readData(pBuffer);
			var wFirstSize:uint=CMD_S_OutCard.sizeof_CMD_S_OutCard-
			CMD_S_OutCard.sizeof_cbCardData;
		
			//效验数据
			ASSERT(wDataSize>=wFirstSize);
			if (wDataSize<wFirstSize) return false;
			ASSERT(wDataSize==(wFirstSize+pOutCard.cbCardCount));
			if (wDataSize!=(wFirstSize+pOutCard.cbCardCount)) return false;
		
			//变量定义
			var wMeChairID:uint=GetMeChairID();
			var wOutCardUser:uint=pOutCard.wOutCardUser;
			var wOutViewChairID:uint=SwitchViewChairID(wOutCardUser);
		
			//出牌处理
			SwitchToThrowResult();
			KillGameTimer(IDI_OUT_CARD);
		
			//扑克界面
			if ((IsLookonMode()==true)||(wMeChairID!=wOutCardUser))
			{
				//历史扑克
				if (wOutCardUser==m_wFirstOutUser)
				{
					for (var i:uint=0;i<GAME_PLAYER;i++) 
					{
						if (i==wOutCardUser) continue;
						GetPeerGameView().m_UserCardControl[SwitchViewChairID(i)].SetCardData(null,0);
					}
				}
		
				//控制界面
				GetPeerGameView().m_UserCardControl[wOutViewChairID].SetCardData(pOutCard.cbCardData,pOutCard.cbCardCount);
			}
			m_bCardCount[pOutCard.wOutCardUser]-=pOutCard.cbCardCount;
			FocusUpdateLeaveCardControl();
		
			//历史扑克
			if ((wOutCardUser==m_wFirstOutUser)&&(m_cbOutCardCount[m_wFirstOutUser][0]!=0))
			{
				//设置扑克
				for (var i:uint=0;i<GAME_PLAYER;i++) 
				{
					//保存扑克
					m_cbOutCardCount[i][1]=m_cbOutCardCount[i][0];
					CopyArray(m_cbOutCardData[i][1],m_cbOutCardData[i][0],m_cbOutCardCount[i][0]);
		
					//清理扑克
					m_cbOutCardCount[i][0]=0;
					ZeroArray(m_cbOutCardData[i][0]);
				}
		
				//控制按钮
				GetPeerGameView().m_btLastTurnCard.EnableWindow(true);
			}
		
			//出牌记录
			m_cbOutCardCount[wOutCardUser][0]=pOutCard.cbCardCount;
			CopyArray(m_cbOutCardData[wOutCardUser][0],pOutCard.cbCardData,pOutCard.cbCardCount);
		
			//当前界面
			SwitchToCurrentCard();
		
			//玩家扑克
			if ((IsLookonMode()==true)&&(wOutCardUser==wMeChairID))
			{
				//删除扑克 
				var cbSourceCount:uint=m_cbHandCardCount;
				m_cbHandCardCount-=pOutCard.cbCardCount;
				m_GameLogic.RemoveCard(pOutCard.cbCardData,pOutCard.cbCardCount,m_cbHandCardData,cbSourceCount);
		
				//设置扑克
				SetHandCardControl(m_cbHandCardData,m_cbHandCardCount);
			}
		
			//播放声音
			if ((IsLookonMode()==true)||(wMeChairID!=wOutCardUser)) PlayGameSound("OUT_CARD");
		
			//设置变量
			m_wCurrentUser=pOutCard.wCurrentUser;
		
			//玩家设置
			if ((IsLookonMode()==false)&&(m_wCurrentUser==wMeChairID))
			{
				if (m_bStustee==false) ActiveGameFrame();
				GetPeerGameView().m_btOutCard.ShowWindow(true);
				GetPeerGameView().m_btOutPrompt.ShowWindow(true);
				GetPeerGameView().m_btOutCard.EnableWindow((VerdictOutCard()==true)?true:false);
			}
		
			//设置时间
			if (m_wCurrentUser!=INVALID_CHAIR) SetGameTimer(m_wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);
		
			return true;
		}
		
		//甩牌结果
		protected function OnSubThrowResult(pBuffer:ByteArray,wDataSize:uint):Boolean
		{
			//变量定义
			var pThrowResult:CMD_S_ThrowResult=CMD_S_ThrowResult.readData(pBuffer);
			var wCardCount:uint=pThrowResult.cbThrowCardCount+pThrowResult.cbResultCardCount;
			var wFirstSize:uint=CMD_S_ThrowResult.sizeof_CMD_S_ThrowResult-CMD_S_ThrowResult.sizeof_cbCardDataArray;
		
			//效验数据
			ASSERT(wDataSize>=wFirstSize);
			if (wDataSize<wFirstSize) return false;
			ASSERT(wDataSize==(wFirstSize+wCardCount));
			if (wDataSize!=(wFirstSize+wCardCount)) return false;
		
			//变量定义
			var wMeChairID:uint=GetMeChairID();
			var wOutCardUser=pThrowResult.wOutCardUser;
			var wOutViewChairID:uint=SwitchViewChairID(wOutCardUser);
		
			//扑克变量
			var cbThrowCardCount:uint=pThrowResult.cbThrowCardCount;
			var cbResultCardCount:uint=pThrowResult.cbResultCardCount;
			var pcbThrowCardData:Array=NewArrayByCopy(pThrowResult.cbCardDataArray);
			var pcbResultCardData:Array=NewArrayByCopy(pThrowResult.cbCardDataArray,
			pThrowResult.cbCardDataArray.length - pThrowResult.cbThrowCardCount,pThrowResult.cbThrowCardCount);
			
			m_bCardCount[pThrowResult.wOutCardUser]-=pThrowResult.cbThrowCardCount;	
			FocusUpdateLeaveCardControl();
			
			//还原界面
			SwitchToCurrentCard();
		
			//出牌处理
			KillGameTimer(IDI_OUT_CARD);
			PlayGameSound("THROW_RESULT");
		
			//清理扑克
			for (var i:uint=0;i<GAME_PLAYER;i++) 
			{
				//设置扑克
				if (i!=wOutCardUser) GetPeerGameView().m_UserCardControl[SwitchViewChairID(i)].SetCardData(null,0);
			}
		
			//历史扑克
			if (m_cbOutCardCount[m_wFirstOutUser][0]!=0)
			{
				//设置扑克
				for (var i:uint=0;i<GAME_PLAYER;i++) 
				{
					//保存扑克
					m_cbOutCardCount[i][1]=m_cbOutCardCount[i][0];
					CopyArray(m_cbOutCardData[i][1],m_cbOutCardData[i][0],m_cbOutCardCount[i][0]);
		
					//清理扑克
					m_cbOutCardCount[i][0]=0;
					ZeroArray(m_cbOutCardData[i][0]);
				}
			}
		
			//出牌界面
			GetPeerGameView().m_UserCardControl[wOutViewChairID].SetCardData(pcbThrowCardData,cbThrowCardCount);
		
			//出牌记录
			m_cbOutCardCount[wOutCardUser][0]=cbResultCardCount;
			CopyArray(m_cbOutCardData[wOutCardUser][0],pcbResultCardData,cbResultCardCount);
		
			//玩家扑克
			if (wOutCardUser==wMeChairID)
			{
				//填补扑克
				if (IsLookonMode()==false)
				{
					//设置扑克
					var cbSourceCount:uint=m_cbHandCardCount;
					m_cbHandCardCount+=cbThrowCardCount;
					CopyArray(m_cbHandCardData,pcbThrowCardData,cbThrowCardCount,cbSourceCount);
		
					//排列扑克
					m_GameLogic.SortCardList(m_cbHandCardData,m_cbHandCardCount);
				}
		
				//删除扑克
				var cbSourceCount:uint=m_cbHandCardCount;
				m_cbHandCardCount-=cbResultCardCount;
				m_GameLogic.RemoveCard(pcbResultCardData,cbResultCardCount,m_cbHandCardData,cbSourceCount);
		
				//设置界面
				SetHandCardControl(m_cbHandCardData,m_cbHandCardCount);
			}
		
			//设置变量
			m_bThrowCard=true;
			m_cbResultCardCount=cbResultCardCount;
			m_wCurrentUser=pThrowResult.wCurrentUser;
			m_wThrowCardUser=pThrowResult.wOutCardUser;
			CopyArray(m_cbResultCardData,pcbResultCardData,cbResultCardCount);
		
			//设置时间
			SetTimer(IDI_THROW_CARD_RESULT,2000);
		
			//上轮设置
			GetPeerGameView().m_btLastTurnCard.EnableWindow(false);
		
			return true;
		}
		
		//一轮结算
		protected function OnSubTurnBalance(pBuffer:ByteArray,wDataSize:uint):Boolean
		{
			//效验数据
			ASSERT(wDataSize==CMD_TurnBalance.sizeof_CMD_TurnBalance);
			if (wDataSize!=CMD_TurnBalance.sizeof_CMD_TurnBalance) return false;
		
			//消息处理
			var pTurnBalance:CMD_TurnBalance=CMD_TurnBalance.readData(pBuffer);
		
			//计算得分
			var wWinnerUser:uint=pTurnBalance.wTurnWinner;
			if ((wWinnerUser!=m_wBankerUser)&&((wWinnerUser+2)%GAME_PLAYER!=m_wBankerUser))
			{
				//获取分牌
				for (var i:uint=0;i<GAME_PLAYER;i++)
				{
					var pcbScoreCard:Array=NewArrayByCopy(m_cbScoreCardData,
														  m_cbScoreCardData.length - m_cbScoreCardCount,
														  m_cbScoreCardCount);
					var cbMaxCount:uint=CountArray(m_cbScoreCardData)-m_cbScoreCardCount;
					var cbNewCount:uint =m_GameLogic.GetScoreCard(m_cbOutCardData[i][0],
												m_cbOutCardCount[i][0],
												pcbScoreCard,
												cbMaxCount);
					CopyArray(m_cbScoreCardData, pcbScoreCard, cbNewCount,m_cbScoreCardCount, 0);
					m_cbScoreCardCount += cbNewCount;
					
				}
		
				//设置界面
				GetPeerGameView().m_CardScore.SetCardData(m_cbScoreCardData,m_cbScoreCardCount);
				GetPeerGameView().SetCardScore(m_GameLogic.GetCardScore(m_cbScoreCardData,m_cbScoreCardCount));
			}
		
			//设置用户
			m_wCurrentUser=pTurnBalance.wCurrentUser;
			m_wFirstOutUser=pTurnBalance.wCurrentUser;
		
			//玩家设置
			if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
			{
				if (m_bStustee==false) ActiveGameFrame();
				GetPeerGameView().m_btOutCard.ShowWindow(true);
				GetPeerGameView().m_btOutPrompt.ShowWindow(true);
				GetPeerGameView().m_btOutCard.EnableWindow((VerdictOutCard()==true)?true:false);
			}
		
			//设置时间
			if (m_wCurrentUser!=INVALID_CHAIR) SetGameTimer(m_wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);
		
			return true;
		}
		
		//游戏结束
		protected function OnSubGameEnd(pBuffer:ByteArray,wDataSize:uint):Boolean
		{
			//效验数据
			ASSERT(wDataSize==CMD_S_GameEnd.sizeof_CMD_S_GameEnd);
			if (wDataSize!=CMD_S_GameEnd.sizeof_CMD_S_GameEnd) return false;
		
			//设置状态
			SetGameStatus(GS_UG_FREE);
			
			//消息处理
			var pGameEnd:CMD_S_GameEnd=CMD_S_GameEnd.readData(pBuffer);
		
			//游戏定时器
			KillGameTimer(IDI_OUT_CARD);
			KillGameTimer(IDI_CALL_CARD);
			KillGameTimer(IDI_SEND_CONCEAL);
		
			//功能定时器
			KillTimer(IDI_CONCEAL_CARD);
			KillTimer(IDI_DISPATCH_CARD);
			KillTimer(IDI_LAST_TURN_CARD);
			KillTimer(IDI_THROW_CARD_RESULT);
		
			//隐藏控件
			GetPeerGameView().m_btOutCard.ShowWindow(false);
			GetPeerGameView().m_CallCardWnd.ShowWindow(false);
			GetPeerGameView().m_btOutPrompt.ShowWindow(false);
			GetPeerGameView().m_btSendConceal.ShowWindow(false);
			GetPeerGameView().m_btSendConceal.EnableWindow(false);
			GetPeerGameView().m_CallCardWnd.ShowWindow(false);
		
			//控制界面
			GetPeerGameView().m_btConcealCard.EnableWindow(false);
			GetPeerGameView().m_btLastTurnCard.EnableWindow(false);
			GetPeerGameView().m_ConcealCardView.ShowWindow(false);
			GetPeerGameView().m_ConcealCardView.SetConcealCard(null,0);
		
			//设置控件
			SwitchToCurrentCard();
			SwitchToThrowResult();
			GetPeerGameView().SetWaitConceal(false);
			GetPeerGameView().m_btOutCard.EnableWindow(false);
		
			//设置结束
			GetPeerGameView().m_ScoreView.m_wGameScore=pGameEnd.wGameScore;
			GetPeerGameView().m_ScoreView.m_wConcealTime=pGameEnd.wConcealTime;
			GetPeerGameView().m_ScoreView.m_wConcealScore=pGameEnd.wConcealScore;
			GetPeerGameView().m_ScoreView.m_cbConcealCount=pGameEnd.cbConcealCount;
			CopyArray(GetPeerGameView().m_ScoreView.m_cbConcealCard,
					  pGameEnd.cbConcealCard,
					  pGameEnd.cbConcealCount);
			
			//设置积分
			for (var i:uint=0;i<GAME_PLAYER;i++)
			{
				GetPeerGameView().m_ScoreView.SetGameScore(i,pGameEnd.fScore[i]);
			}
			GetPeerGameView().m_ScoreView.SetMeChairID(GetMeChairID());
			//显示成绩
			GetPeerGameView().m_ScoreView.ShowWindow(true);
		
			//播放声音
			var wMeChairID:uint=GetMeChairID();
			var fMeScore:Number=pGameEnd.fScore[GetMeChairID()];
			if (fMeScore>0) PlayGameSound("GAME_WIN");
			else if (fMeScore<0) PlayGameSound("GAME_LOST");
			else PlayGameSound("GAME_END");
		
			//设置界面
			if (IsLookonMode()==false)
			{
				GetPeerGameView().m_btStart.ShowWindow(true);
				SetGameTimer(GetMeChairID(),IDI_EXIT_GAME,TIME_START_GAME);
			}
			if (m_bStustee==true)
			{
				m_bStustee=false;
				GetPeerGameView().m_btStusteeControl.SetButtonImage(IMAGE_START_TRUSTEE,125,25);
			}
	
		
			return true;
		}
		
		//出牌判断
		protected function VerdictOutCard():Boolean
		{
			//状态判断
			if (GetPeerGameView().m_btOutCard.IsWindowVisible()==false) return false;
		
			//获取扑克
			var cbCardData:Array = new Array(MAX_COUNT);
			var cbShootCount:uint=GetPeerGameView().m_HandCardControl.GetShootCard(cbCardData,CountArray(cbCardData));
		
			//出牌判断
			if (cbShootCount>0)
			{
				//排列扑克
				m_GameLogic.SortCardList(cbCardData,cbShootCount);
		
				//首出牌者
				if (m_wFirstOutUser==GetMeChairID())
				{
					//类型判断
					var cbCardType:uint=m_GameLogic.GetCardType(cbCardData,cbShootCount);
					if (cbCardType==CT_ERROR) return false;
		
					return true;
				}
		
				//跟随出牌
				var cbTurnCardCount:uint=m_cbOutCardCount[m_wFirstOutUser][0];
				var bLegality:Boolean=m_GameLogic.EfficacyOutCard(cbCardData,
						cbShootCount,
						m_cbOutCardData[m_wFirstOutUser][0],
						cbTurnCardCount,
						m_cbHandCardData,
						m_cbHandCardCount);
		
				return bLegality;
			}
		
			return false;
		}
		
		//自动出牌
		protected function AutomatismOutCard():Boolean
		{
			//变量定义
			var OutCardResult:tagOutCardResult = new tagOutCardResult;

		
			//搜索出牌
			var cbTurnCardCount:uint=(m_wCurrentUser==m_wFirstOutUser)?0:m_cbOutCardCount[m_wFirstOutUser][0];
			m_GameLogic.SearchOutCard(m_cbHandCardData,m_cbHandCardCount,m_cbOutCardData[m_wFirstOutUser][0],cbTurnCardCount,OutCardResult);
		
			//设置变量
			KillGameTimer(IDI_OUT_CARD);
			m_wCurrentUser=INVALID_CHAIR;
		
			//设置控件
			GetPeerGameView().m_btOutCard.ShowWindow(false);
			GetPeerGameView().m_btOutCard.EnableWindow(false);
			GetPeerGameView().m_btOutPrompt.ShowWindow(false);
		
			//设置扑克
			if (m_wFirstOutUser==GetMeChairID())
			{
				for (var i:uint=0;i<GAME_PLAYER;i++)
				{
					if (i!=2) GetPeerGameView().m_UserCardControl[i].SetCardData(null,0);
				}
			}
			
			//发送数据
			var OutCard:CMD_C_OutCard = new CMD_C_OutCard;
			OutCard.cbCardCount=OutCardResult.cbCardCount;
			CopyArray(OutCard.cbCardData,OutCardResult.cbResultCard,OutCard.cbCardCount);
			var sendData:ByteArray = OutCard.toByteArray();
			SendGameData(SUB_C_OUT_CARD,sendData,CMD_C_OutCard.sizeof_CMD_C_OutCard-
			CMD_C_OutCard.sizeof_cbCardData+OutCard.cbCardCount);
		
			//预先显示
			PlayGameSound("OUT_CARD");
			GetPeerGameView().m_UserCardControl[2].SetCardData(OutCard.cbCardData,OutCard.cbCardCount);
		
			//预先删除
			var cbSourceCount:uint=m_cbHandCardCount;
			m_cbHandCardCount-=OutCard.cbCardCount;
			m_GameLogic.RemoveCard(OutCard.cbCardData,OutCard.cbCardCount,m_cbHandCardData,cbSourceCount);
		
			//设置扑克
			SetHandCardControl(m_cbHandCardData,m_cbHandCardCount);
		
			return true;
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
				GetPeerGameView().m_UserCardControl[wViewChairID].SetCardData(m_cbOutCardData[i][0],m_cbOutCardCount[i][0]);
			}
		
			return true;
		}
		
		//甩牌结果
		protected function SwitchToThrowResult():Boolean
		{
			//处理判断
			if (m_bThrowCard==false) return false;
			if (m_wThrowCardUser==INVALID_CHAIR) return false;
		
			//时间设置
			KillTimer(IDI_THROW_CARD_RESULT);
		
			//设置界面
			var wViewChairID:uint=SwitchViewChairID(m_wThrowCardUser);
			GetPeerGameView().m_UserCardControl[wViewChairID].SetCardData(m_cbResultCardData,m_cbResultCardCount);
		
			//界面设置
			if (m_cbOutCardCount[0][1]!=0) GetPeerGameView().m_btLastTurnCard.EnableWindow(true);
		
			//设置变量
			m_bThrowCard=false;
			m_cbResultCardCount=0;
			m_wThrowCardUser=INVALID_CHAIR;
			ZeroArray(m_cbResultCardData);
		
			return true;
		}
		
		//更新叫牌
		protected function UpdateCallCardControl():Boolean
		{
			//状态过滤
			if (IsLookonMode()==true) return false;
			if (GetGameStatus()!=GS_UG_CALL) return false;
		
			//变量定义
			var cbCallMask:uint=CallCardWnd.CALL_NONE;
			var cbCallColor:uint=m_GameLogic.GetCardColor(m_cbCallCard);
		
			//辅助变量
			var cbCardCount:Array = new Array(0,0,0,0,0,0);
			var cbCallMaskArray:Array = new Array(CallCardWnd.CALL_FANG_KUAI,
												  CallCardWnd.CALL_MEI_HUA,
												  CallCardWnd.CALL_HONG_TAO,
												  CallCardWnd.CALL_HEI_TAO,
												  CallCardWnd.CALL_NT,
												  CallCardWnd.CALL_NT);
			var cbCallColorArray:Array = new Array(COLOR_FANG_KUAI,
												   COLOR_MEI_HUA,
												   COLOR_HONG_TAO,
												   COLOR_HEI_TAO,
												   COLOR_NT,
												   COLOR_NT);
		
			//更新调整
			if (m_wCallCardUser!=GetMeChairID())
			{
				//扑克统计
				for (var i:uint=0;i<m_cbDispatchCardCount;i++)
				{
					//获取属性
					var cbCardColor:uint=m_GameLogic.GetCardColor(m_cbDispatchCardData[i]);
					var cbCardValue:uint=m_GameLogic.GetCardValue(m_cbDispatchCardData[i]);
		
					//数据统计
					if (cbCardColor==COLOR_NT) cbCardCount[4+cbCardValue-0x0E]++;
					else if (cbCardValue==m_cbMainValue) cbCardCount[cbCardColor>>4]++;
				}
		
				//主牌控制
				for (var i:uint=0;i<CountArray(cbCardCount);i++)
				{
					//规则判断
					if ((cbCardCount[i]==0)||(cbCardCount[i]<m_cbCallCount)) continue;
		
					//数目判断
					if ((i<4)&&(cbCardCount[i]>m_cbCallCount))
					{
						cbCallMask|=cbCallMaskArray[i];
						continue;
					}
		
					//无主判断
					if ((i>=4)&&(cbCardCount[i]>m_cbCallCount)&&(cbCardCount[i]>=2))
					{
						cbCallMask|=cbCallMaskArray[i];
						continue;
					}
		
					//花色判断
					var cbPackCount:uint=m_GameLogic.GetPackCount();
					if ((cbCardCount[i]==cbPackCount)&&(cbCallColorArray[i]>cbCallColor))
					{
						cbCallMask|=cbCallMaskArray[i];
						continue;
					}
				}
			}
			else
			{
				//扑克统计
				var cbCallCardCount:uint=0;
				for (var i:uint=0;i<m_cbDispatchCardCount;i++)
				{
					if (m_cbDispatchCardData[i]==m_cbCallCard) cbCallCardCount++;
				}
		
				//叫牌控制
				if (cbCallCardCount>m_cbCallCount) cbCallMask=cbCallMaskArray[cbCallColor>>4];
			}
		
			//更新控制
			GetPeerGameView().m_CallCardWnd.UpdateCallControl1(cbCallMask);
		
			return true;
		}
		
		//派发扑克
		protected function DispatchUserCard(cbCardData:Array,  cbCardCount:uint):Boolean
		{
			//设置变量
			m_cbDispatchCardCount=0;
			m_cbRemnantCardCount=cbCardCount;
			CopyArray(m_cbDispatchCardData,cbCardData,cbCardCount);
		
			//设置定时器
			SetTimer(IDI_DISPATCH_CARD,400);
		
			return true;
		}
		
		//设置扑克
		protected function SetHandCardControl(cbCardData:Array,  cbCardCount:uint):Boolean
		{
			//设置过滤
			if (cbCardCount>0)
			{
				//变量定义
				var cbRectifyCount:uint=1;
				var cbRectifyCard:Array = NewArrayAndSetValue(MAX_COUNT+4,cbCardData[0]);
		
				//获取属性
				var cbLogicColor:uint=m_GameLogic.GetCardLogicColor1(cbCardData[0]);
		
				//变换扑克
				for (var i:uint=1;i<cbCardCount;i++)
				{
					//获取属性
					var cbCurrentColor:uint=m_GameLogic.GetCardLogicColor1(cbCardData[i]);
		
					//插入判断
					if (cbCurrentColor!=cbLogicColor)
					{
						cbLogicColor=cbCurrentColor;
						cbRectifyCard[cbRectifyCount++]=255;
						cbRectifyCard[cbRectifyCount++]=cbCardData[i];
					}
					else cbRectifyCard[cbRectifyCount++]=cbCardData[i];
				}
		
				//设置扑克
				GetPeerGameView().m_HandCardControl.SetCardData(cbRectifyCard,cbRectifyCount);
		
				return true;
			}
			else GetPeerGameView().m_HandCardControl.SetCardData(null,0);
		
			return true;
		}
		
		//定时器消息
		override protected function OnTimer( nIDEvent:int):Boolean
		{
			switch (nIDEvent)
			{
			case IDI_CONCEAL_CARD:		//查看底牌
				{
					//显示设置
					KillTimer(IDI_CONCEAL_CARD);
					GetPeerGameView().m_ConcealCardView.ShowWindow(false);
		
					return true;
				}
			case IDI_DISPATCH_CARD:		//派发扑克
				{
					//派发控制
					if (m_cbRemnantCardCount>0)
					{
						//获取属性
						var cbCardColor:uint=m_GameLogic.GetCardColor(m_cbDispatchCardData[m_cbDispatchCardCount]);
						var cbCardValue:uint=m_GameLogic.GetCardValue(m_cbDispatchCardData[m_cbDispatchCardCount]);
		
						//派发扑克
						m_cbRemnantCardCount--;
						m_cbDispatchCardCount++;
						m_GameLogic.SortCardList(m_cbDispatchCardData,m_cbDispatchCardCount);
						SetHandCardControl(m_cbDispatchCardData,m_cbDispatchCardCount);
		
						//叫牌控制
						if ((cbCardColor==COLOR_NT)||(cbCardValue==m_cbMainValue)) UpdateCallCardControl();
		
						//播放声音
						PlayGameSound("OUT_CARD");
					}
					
					//中止判断
					if (m_cbRemnantCardCount==0) 
					{
						//删除定时器
						KillTimer(IDI_DISPATCH_CARD);
		
						//环境设置
						SetGameTimer(GetMeChairID(),IDI_CALL_CARD,TIME_CALL_CARD);
						if ((IsLookonMode()==false)&&(m_bStustee==false)) ActiveGameFrame();
					}
		
					return true;
				}
			case IDI_LAST_TURN_CARD:	//上轮扑克
				{
					//还原界面
					SwitchToCurrentCard();
		
					return true;
				}
			case IDI_THROW_CARD_RESULT:	//甩牌结果
				{
					//还原界面
					SwitchToThrowResult();
		
					//当前玩家
					if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
					{
						if (m_bStustee==false) ActiveGameFrame();
						GetPeerGameView().m_btOutCard.ShowWindow(true);
						GetPeerGameView().m_btOutPrompt.ShowWindow(true);
						GetPeerGameView().m_btOutCard.EnableWindow((VerdictOutCard()==true)?true:false);
					}
		
					//设置定时器
					SetGameTimer(m_wCurrentUser,IDI_OUT_CARD,TIME_OUT_CARD);
		
					return true;
				}
			}
		
			return false;
		}
		
		//开始按钮
		override protected function OnStart(wParam:uint, lParam:uint):uint
		{
			//设置环境
			KillGameTimer(IDI_EXIT_GAME);
		
			//设置界面
			GetPeerGameView().SetCardScore(0xFFFF);
			GetPeerGameView().SetWaitConceal(false);
			GetPeerGameView().SetBankerUser(INVALID_CHAIR);
			GetPeerGameView().SetCallCardInfo(INVALID_CHAIR,COLOR_ERROR,0);
		
			//隐藏控件
			GetPeerGameView().m_btStart.ShowWindow(false);
			GetPeerGameView().m_ScoreView.ShowWindow(false);
		
			//底牌控制
			GetPeerGameView().m_btConcealCard.EnableWindow(false);
			GetPeerGameView().m_ConcealCardView.ShowWindow(false);
			GetPeerGameView().m_ConcealCardView.SetConcealCard(null,0);
		
			//设置扑克
			SetHandCardControl(null,0);
			GetPeerGameView().m_CardScore.SetCardData(null,0);
			GetPeerGameView().m_HandCardControl.SetPositively(false);
			for (var i:uint=0;i<GAME_PLAYER;i++) GetPeerGameView().m_UserCardControl[i].SetCardData(null,0);
			GetPeerGameView().m_LeaveCardControl[0].SetCardData(null,0);
			GetPeerGameView().m_LeaveCardControl[1].SetCardData(null,0);
			GetPeerGameView().m_LeaveCardControl[2].SetCardData(null,0);
	
			//发送消息
			SendUserReady(null,0);
		
			return 0;
		}
		
		//出牌消息
		protected function OnOutCard(wParam:uint, lParam:uint):uint
		{
			//状态判断
			if (GetPeerGameView().m_btOutCard.IsWindowEnabled()==false) return 0;
			if (GetPeerGameView().m_btOutCard.IsWindowVisible()==false) return 0;
		
			//设置变量
			KillGameTimer(IDI_OUT_CARD);
			m_wCurrentUser=INVALID_CHAIR;
		
			//设置控件
			GetPeerGameView().m_btOutCard.ShowWindow(false);
			GetPeerGameView().m_btOutCard.EnableWindow(false);
			GetPeerGameView().m_btOutPrompt.ShowWindow(false);
		
			//设置扑克
			if (m_wFirstOutUser==GetMeChairID())
			{
				for (var i:uint=0;i<GAME_PLAYER;i++)
				{
					if (i!=2) GetPeerGameView().m_UserCardControl[i].SetCardData(null,0);
				}
			}
			
			//发送数据
			var OutCard:CMD_C_OutCard = new CMD_C_OutCard;
			OutCard.cbCardCount=GetPeerGameView().m_HandCardControl.GetShootCard(OutCard.cbCardData,CountArray(OutCard.cbCardData));
			m_GameLogic.SortCardList(OutCard.cbCardData,OutCard.cbCardCount);
			var sendData:ByteArray = OutCard.toByteArray();
			SendGameData(SUB_C_OUT_CARD,sendData,
			CMD_C_OutCard.sizeof_CMD_C_OutCard-
			CMD_C_OutCard.sizeof_cbCardData+OutCard.cbCardCount);
		
			//预先显示
			PlayGameSound("OUT_CARD");
			GetPeerGameView().m_UserCardControl[2].SetCardData(OutCard.cbCardData,OutCard.cbCardCount);
		
			//预先删除
			var cbSourceCount:uint=m_cbHandCardCount;
			m_cbHandCardCount-=OutCard.cbCardCount;
			m_GameLogic.RemoveCard(OutCard.cbCardData,OutCard.cbCardCount,m_cbHandCardData,cbSourceCount);
		
			//设置扑克
			SetHandCardControl(m_cbHandCardData,m_cbHandCardCount);
		
			return 0;
		}
		
		//叫牌消息
		protected function OnCallCard(wParam:uint, lParam:uint):uint
		{
			//变量定义
			var cbCallCount:uint=0;
			var cbCallCard:uint=0x00;
			var cbCallColor:uint=lParam;
		
			//数据统计
			if (cbCallColor==COLOR_NT)
			{
				//数目统计
				var cbNTCount:Array = new Array(0,0);
				for (var i:uint=0;i<m_cbDispatchCardCount;i++)
				{
					var cbCardData:uint=m_cbDispatchCardData[i];
					if ((cbCardData==0x4E)||(cbCardData==0x4F)) cbNTCount[cbCardData-0x4E]++;
				}
		
				//叫牌数据
				if (cbNTCount[0]>=cbNTCount[1])
				{
					cbCallCard=0x4E;
					cbCallCount=cbNTCount[0];
				}
				else
				{
					cbCallCard=0x4F;
					cbCallCount=cbNTCount[1];
				}
			}
			else
			{
				//数目统计
				for (var i:uint=0;i<m_cbDispatchCardCount;i++)
				{
					//获取属性
					var cbCardData:uint=m_cbDispatchCardData[i];
					var cbCardColor:uint=m_GameLogic.GetCardColor(cbCardData);
					var cbCardValue:uint=m_GameLogic.GetCardValue(cbCardData);
		
					//数据统计
					if ((cbCardValue==m_cbMainValue)&&(cbCardColor==cbCallColor)) 
					{
						cbCallCount++;
						cbCallCard=cbCardData;
					}
				}
			}
		
			//更新界面
			GetPeerGameView().m_CallCardWnd.UpdateCallControl1(CallCardWnd.CALL_NONE);
		
			//构造数据
			var CallCard:CMD_C_CallCard = new CMD_C_CallCard;
			CallCard.cbCallCard=cbCallCard;
			CallCard.cbCallCount=cbCallCount;
			var sendData:ByteArray =CallCard.toByteArray();
		
			//发送数据
			SendGameData(SUB_C_CALL_CARD,sendData,CMD_C_CallCard.sizeof_CMD_C_CallCard);
		
			return 0;
		}
		
		//出牌提示
		protected function OnOutPrompt(wParam:uint, lParam:uint):uint
		{
			//变量定义
			var OutCardResult:tagOutCardResult = new tagOutCardResult;

		
			//搜索出牌
			var cbTurnCardCount:uint=(m_wCurrentUser==m_wFirstOutUser)?0:m_cbOutCardCount[m_wFirstOutUser][0];
			var bSuccess:Boolean=m_GameLogic.SearchOutCard(m_cbHandCardData,
														   m_cbHandCardCount,
														   m_cbOutCardData[m_wFirstOutUser][0],
														   cbTurnCardCount,
														   OutCardResult);
		
			//失败判断
			if (bSuccess==false)
			{
				InsertSystemString("自动提示出牌错误，请你把此状态信息截图并给我们提供，谢谢！");
				return 0;
			}
		
			//设置界面
			GetPeerGameView().m_btOutCard.EnableWindow(true);
			GetPeerGameView().m_HandCardControl.SetShootCard(OutCardResult.cbResultCard,OutCardResult.cbCardCount);
		
			return 0;
		}
		
		//留底消息
		protected function OnSendConceal(wParam:uint, lParam:uint):uint
		{
			//获取扑克
			var cbCardData:Array = new Array(MAX_COUNT);
			var cbCardCount:uint=GetPeerGameView().m_HandCardControl.GetShootCard(cbCardData,CountArray(cbCardData));
		
			//删除扑克
			m_cbHandCardCount-=cbCardCount;
			m_GameLogic.RemoveCard(cbCardData,cbCardCount,m_cbHandCardData,m_cbHandCardCount+cbCardCount);
		
			//设置界面
			KillGameTimer(IDI_SEND_CONCEAL);
			GetPeerGameView().m_btSendConceal.ShowWindow(false);
			GetPeerGameView().m_btSendConceal.EnableWindow(false);
			SetHandCardControl(m_cbHandCardData,m_cbHandCardCount);
		
			//构造数据
			var ConcealCard:CMD_C_ConcealCard = new CMD_C_ConcealCard;
			ConcealCard.cbConcealCount=cbCardCount;
			CopyArray(ConcealCard.cbConcealCard,cbCardData,cbCardCount);
			var sendData:ByteArray = ConcealCard.toByteArray();
			//发送数据
			SendGameData(SUB_C_CONCEAL_CARD,sendData,CMD_C_ConcealCard.sizeof_CMD_C_ConcealCard);
		
			return 0;
		}
		
		//查看底牌
		protected function OnConcealCard(wParam:uint, lParam:uint):uint
		{
			//查看设置
			if (GetPeerGameView().m_ConcealCardView.IsWindowVisible())
			{
				KillTimer(IDI_CONCEAL_CARD);
				GetPeerGameView().m_ConcealCardView.ShowWindow(false);
			}
			else
			{
				SetTimer(IDI_CONCEAL_CARD,5000);
				GetPeerGameView().m_ConcealCardView.ShowWindow(true);
			}
		
			return 0;
		}
		
		//请求离开
		protected function OnRuquestLeave(wParam:uint, lParam:uint):uint
		{
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
					GetPeerGameView().m_UserCardControl[wViewChairID].SetCardData(m_cbOutCardData[i][1],m_cbOutCardCount[i][1]);
				}
		
				//环境设置
				m_bLastTurn=true;
				GetPeerGameView().SetLastTurnCard(true);
				SetTimer(IDI_LAST_TURN_CARD,3000);
			}
			else SwitchToCurrentCard();
		
			return 0;
		}
		
		//拖管控制
		protected function OnStusteeControl(wParam:uint, lParam:uint):uint
		{
			if(GetGameStatus() != GS_UG_PLAY)
				return 0;
			//设置状态
			if (m_bStustee==true)
			{
				m_bStustee=false;
				GetPeerGameView().m_btStusteeControl.SetButtonImage(IMAGE_START_TRUSTEE,125,25);
			}
			else
			{
				m_bStustee=true;
				GetPeerGameView().m_btStusteeControl.SetButtonImage(IMAGE_STOP_TRUSTEE,125,25);
			}
		
			return 0;
		}
		
		//左键扑克
		protected function OnLeftHitCard(wParam:uint, lParam:uint):uint
		{
			//获取状态
			var cbGameStatue:uint=GetGameStatus();
		
			//留底状态
			switch (cbGameStatue)
			{
			case GS_UG_BACK:	//留底状态
				{
					//状态控制
					if ((IsLookonMode()==false)&&(m_wCurrentUser==GetMeChairID()))
					{
						//获取扑克
						var cbCardData:Array = new Array(MAX_COUNT);
						var cbCardCount:uint=GetPeerGameView().m_HandCardControl.GetShootCard(cbCardData,CountArray(cbCardData));
		
						//控制按钮
						GetPeerGameView().m_btSendConceal.EnableWindow((cbCardCount==m_cbConcealCount)?true:false);
					}
		
					return 0;
				}
			case GS_UG_PLAY:	//游戏状态
				{
					//设置控件
					var bOutCard:Boolean=VerdictOutCard();
					GetPeerGameView().m_btOutCard.EnableWindow(bOutCard?true:false);
		
					return 0;
				}

			}
		
			return 0;
		}
		
		//右键扑克
		protected function OnRightHitCard(wParam:uint, lParam:uint):uint
		{
			//用户出牌
			OnOutCard(0,0);
		
			return 0;
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
				case GameViewEvent.nGVM_CALL_CARD :
					{
						OnCallCard(e.m_nWParam,e.m_nLParam);
						return;


					};
				case GameViewEvent.nGVM_OUT_PROMPT :
					{
						OnOutPrompt(e.m_nWParam,e.m_nLParam);
						return;


					};
				case GameViewEvent.nGVM_SEND_CONCEAL :
					{
						OnSendConceal(e.m_nWParam,e.m_nLParam);
						return;


					};
				case GameViewEvent.nGVM_CONCEAL_CARD :
					{
						OnConcealCard(e.m_nWParam,e.m_nLParam);
						return;


					};
				case GameViewEvent.nGVM_REQUEST_LEAVE :
					{
						OnRuquestLeave(e.m_nWParam,e.m_nLParam);
						return;


				}
				case GameViewEvent.nGVM_LAST_TURN_CARD:
					{
						OnLastTurnCard(e.m_nWParam,e.m_nLParam);
						return;


				}
				case GameViewEvent.nGVM_TRUSTEE_CONTROL:
					{
						OnStusteeControl(e.m_nWParam,e.m_nLParam);
						return;


				}
				case GameViewEvent.nGVM_LEFT_HIT_CARD:
					{
						OnLeftHitCard(e.m_nWParam,e.m_nLParam);
						return;


				}
				
			}
		}

		//////////////////////////////////////
		//辅助函数
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
		private function FocusUpdateLeaveCardControl():void
		{
			var bCardData:Array = GenEmptyCardData(42);
			UpdateLeaveCardControl(bCardData);
		}
	}
}
include "../../Common/Memory.as"
include "./Common/GameLogicDef.as"
include "../../Net/GlobalDef.as"
include "../../Net/GlobalFrame.as"
include "./Common/CMD_Upgrade.as"