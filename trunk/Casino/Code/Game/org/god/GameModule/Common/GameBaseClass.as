package org.god.GameModule.Common
{
	import flash.display.MovieClip;
	import flash.utils.ByteArray;
	import flash.utils.Timer;
	import flash.events.TimerEvent;

	import org.god.Common.TimerMap;
	import org.god.Common.Media.SoundPlayer;
	import org.god.Common.Media.SoundManager;
	import org.god.Common.GlobalCommonMain;
	import org.god.Net.*;
	import org.god.IGameFrame.*;
	import org.aswing.util.*;

	public class GameBaseClass extends MovieClip implements IGameClass
	{
		private var m_clientContainer:IGameClassContainer;
		protected var m_GameClientView:GameBaseView;
		protected var m_IMain:* = null;

		//房间信息
		protected var m_wTableID:uint;//桌子号码
		protected var m_wChairID:uint;//椅子号码

		//固有信息
		protected var m_dwUserID:uint;//用户 I D
		protected var m_ServerAttribute:tagServerAttribute;//房间属性

		//游戏信息
		protected var m_bInquire:Boolean;//关闭询问
		protected var m_bLookonMode:Boolean;//旁观模式
		protected var m_bGameStatus:uint;//游戏状态
		
		//配置信息
		protected var m_bAllowLookon:Boolean;

		//用户信息
		protected var m_pMeUserItem:tagUserInfoHead;//自己信息
		protected var m_pUserItem:Array;//玩家信息
		protected var m_UserItemLookon:LinkedList;//旁观数组
		protected var m_UserItemFactory:LinkedList;

		//定时器信息
		protected var m_nTimerID:uint;//定时器 ID
		protected var m_nElapseCount:uint;//定时器计数
		protected var m_wTimerChairID:uint;//定期器位置
		protected var m_TimerWork:TimerMap;//定时器
		
		//声音信息
		private var m_SoundResource:HashMap;

		public function GameBaseClass()
		{
		}
		//////////////////////////////////////////////
		public function CreateGameClient(clientContainer:IGameClassContainer):Boolean
		{
			do
			{
				if (clientContainer == null ||
					m_clientContainer != null)
				{
					break;
				}
				m_clientContainer = clientContainer;
				
				if (InitGameClass() == false)
				{
					break;
				}
				
				return SendIPCChannelMessage(IPC_MIAN_IPC_KERNEL,IPC_SUB_IPC_CLIENT_CONNECT,null,0);
	
			}while(false);
			DestroyGameClient();
			return false;
		}
		public function DestroyGameClient():void
		{
			if(m_clientContainer != null)
			{
				DestroyGameClass();
				m_clientContainer = null;
			}
		}
		public function GetMovieClip():MovieClip
		{
			return this;
		}
		public function GetGameView():IGameView
		{
			if (m_GameClientView == null)
			{
				return null;
			}
			else
			{
				return m_GameClientView  as  IGameView;
			}
		}
		protected function CreateGameView():GameBaseView
		{
			return null;
		}
		public function SendEventExitGameClient(bAutoSitChairAgain:Boolean = false):void
		{
			if(GetExitFlags() == false)
			{
				if(DEBUG == 0)
				{
					ShowMessageBox(IDS_PLAYINGANDCANNTEXITGAME);
					return;
				}
			}
			if (m_clientContainer)
			{
				m_clientContainer.DestroyGameClass(this, bAutoSitChairAgain);
			}
		}
		///////////////////////////////////////////////////////
		public function InitGameClass():Boolean
		{
			do
			{
				m_IMain = GlobalCommonMain.getGlobalCommonMain().GetIMain();
				
				m_SoundResource = new HashMap;
				
				//定时器信息
				m_nTimerID=0;
				m_nElapseCount=0;
				m_wTimerChairID=INVALID_CHAIR;
				m_TimerWork = new TimerMap;

				m_dwUserID=0;
				m_wTableID=INVALID_TABLE;
				m_wChairID=INVALID_CHAIR;
				m_bGameStatus=GS_FREE;
				
				m_bInquire = true;
			
				m_ServerAttribute = new tagServerAttribute;
				m_pUserItem = new Array(MAX_CHAIR);
				m_UserItemLookon = new LinkedList;
				m_UserItemFactory = new LinkedList;

				if (m_GameClientView == null)
				{
					m_GameClientView = CreateGameView();
					if (m_GameClientView == null)
					{
						break;
					}
					m_GameClientView.x = 0;
					m_GameClientView.y = 0;
					
					addChild(m_GameClientView);
					if(m_GameClientView.InitGameView() == false)
					{
						break;
					}
					m_GameClientView.RectifyGameView(GameBaseView.nGameUIWidth,
													 GameBaseView.nGameUIHeight);
				}
				return true;
			}while(false);
			return false;
		}
		public function DestroyGameClass():void
		{
			if (m_GameClientView)
			{
				m_GameClientView.DestroyGameView();
				removeChild(m_GameClientView);
				m_GameClientView = null;
			}			
			if(m_SoundResource)
			{
				var sa:Array = m_SoundResource.values();
				for (var i:uint=0; i<sa.length; i ++)
				{
					sa[i].Unload();
					sa[i] = null;
				}
				m_SoundResource.clear();
				m_SoundResource = null;
			}
			//删除定时器
			//if (m_nTimerID!=0 && m_TimerWork)
			if (m_TimerWork)
			{
				//m_TimerWork.KillTimer(m_nTimerID);
				m_TimerWork.Destroy();
				m_TimerWork = null;
				m_nTimerID=0;
				m_nElapseCount=0;
				m_wTimerChairID=INVALID_CHAIR;
			}
			//旁观用户
			if (m_UserItemLookon)
			{
				m_UserItemLookon.clear();
				m_UserItemLookon = null;
			}

			//游戏用户
			if (m_ServerAttribute && m_UserItemFactory)
			{
				for (var i:uint=0; i<m_ServerAttribute.wChairCount; i++)
				{
					if (m_pUserItem[i]!=null)
					{
						m_UserItemFactory.remove(m_pUserItem[i]);
						m_pUserItem[i]=null;
					}
				}
				m_UserItemFactory.clear();
				m_UserItemFactory = null;
				m_ServerAttribute = null;
			}

			m_clientContainer = null;
			m_pUserItem.length = 0;
			m_pUserItem 	  = null;
			
			m_bLookonMode=true;
			m_pMeUserItem=null;
			m_bGameStatus=US_FREE;
			m_wTableID=INVALID_TABLE;
			m_wChairID=INVALID_CHAIR;
			
			m_IMain = null;
		}
		/////////////////////////////////////////////////////
		//信息接口
		//离开状态
		public function GetExitFlags():Boolean
		{
			if (m_bLookonMode==true)
			{
				return true;
			}
			if (m_pMeUserItem==null)
			{
				return true;
			}
			if (m_pMeUserItem.UserStatus.cbUserStatus!=US_PLAY)
			{
				return true;
			}
			return m_bInquire == true?false:true;
		}
		//旁观状态
		public function IsLookonMode():Boolean
		{
			return m_bLookonMode;
		}
		//允许旁观
		public function IsAllowLookon():Boolean
		{
			return m_bAllowLookon;
		}
		//房间属性
		public function GetServerAttribute():tagServerAttribute
		{
			return m_ServerAttribute;
		}
		public function InsertSystemString(str:String):void
		{
		}
		//状态接口

		//游戏状态
		public function GetGameStatus():uint
		{
			return m_bGameStatus;
		}
		//游戏状态
		public function SetGameStatus( bGameStatus:uint):void
		{
			m_bGameStatus=bGameStatus;
		}
		//用户接口
		//自己位置
		public function GetMeChairID():uint
		{
			return m_wChairID;
		}
		//获取自己
		public function GetMeUserInfo():tagUserInfoHead
		{
			return m_pMeUserItem;
		}
		//获取自己
		public function GetGlobalMeUserInfo():tagUserData
		{
			return GlobalCommonMain.getGlobalCommonMain().GetUserData();
		}
		//获取玩家
		public function GetUserInfo(wChairID:uint):tagUserInfoHead
		{
			if (wChairID>=MAX_CHAIR || m_ServerAttribute == null)
			{
				return null;
			}
			if (wChairID>=m_ServerAttribute.wChairCount)
			{
				return null;
			}
			return m_pUserItem[wChairID];
		}
		public function ShowMessageBox(pszMessage:String):void
		{
			if(m_IMain)
			{
				m_IMain.ShowMessageBox(pszMessage);
			}
		}
		//功能接口
		//发送准备
		public function SendUserReady(pBuffer:ByteArray, wDataSize:uint):Boolean
		{
			return SendSocketChannelMessage(MDM_GF_FRAME,SUB_GF_USER_READY,pBuffer,wDataSize);
		}
		public function SendGameData(wSubCmdID:uint,
		 pBuffer:ByteArray=null, wDataSize:uint=0):Boolean
		{
			return SendSocketChannelMessage(MDM_GF_GAME,wSubCmdID,pBuffer,wDataSize);
		}
		//发送聊天
		public function SendChatMessage( dwTargetUserID:uint,
		pszChatMessage:String, 
		crFontColor:uint)
		{
			return true;
		}
		//定时器接口
		//定时器位置
		public function GetTimeChairID():uint
		{
			return m_wTimerChairID;
		}
		protected function SetTimer(wTimerID:int, dwElapse:uint):void
		{
			if (m_TimerWork)
			{
				m_TimerWork.SetTimer(wTimerID,dwElapse, timerHandler);
			}
		}
		protected function KillTimer(wTimerID:int):void
		{
			if (m_TimerWork)
			{
				m_TimerWork.KillTimer(wTimerID);
			}
		}
		protected function OnTimer(nIDEvent:int):Boolean
		{
			return false;
		}
		//设置定时器
		public function SetGameTimer(wChairID:uint, nTimerID:uint, nElapse:uint):Boolean
		{
			if (m_ServerAttribute == null ||
				m_TimerWork == null)
			{
				return false;
			}
			//逻辑处理
			if (m_nTimerID!=0)
			{
				KillGameTimer(m_nTimerID);
			}
			if ((wChairID<m_ServerAttribute.wChairCount)&&(nElapse>0))
			{
				//设置变量
				m_nTimerID=nTimerID;
				m_nElapseCount=nElapse;
				m_wTimerChairID=wChairID;

				//设置界面
				m_TimerWork.SetTimer(nTimerID,1000, timerHandler);

				return OnEventTimer(m_wTimerChairID,nElapse,nTimerID);

			}

			return false;
		}
		//删除定时器
		public function KillGameTimer(nTimerID:uint):Boolean
		{
			//逻辑处理
			if ((m_nTimerID!=0)&&
			((m_nTimerID==nTimerID)||(nTimerID==0)))
			{
				//设置界面
				if (m_TimerWork)
				{
					m_TimerWork.KillTimer(m_nTimerID);
				}
				if (m_wTimerChairID!=INVALID_CHAIR)
				{
					OnEventTimerKilled(m_wTimerChairID,
					  m_nTimerID);
				
				}
				//设置变量
				m_nTimerID=0;
				m_nElapseCount=0;
				m_wTimerChairID=INVALID_CHAIR;

				return true;
			}

			return false;
		}
		public function AddGameSound(strName:String,sound:Class):void
		{
			if(m_SoundResource == null || strName.length == 0 || sound == null)
				return;
			var player:SoundPlayer = new SoundPlayer;
			if(player.LoadLocalSoundData(sound))
				m_SoundResource.put(strName, player);
		}
		public function PlayGameSound(strName:String):void
		{
			if(m_SoundResource == null)
				return;
			var player:SoundPlayer = m_SoundResource.get(strName);
			if(player == null)
			{
				trace("PlayGameSound no find " + strName);
				return;
			}
			SoundManager.GetGlobalSoundManager().PlaySound(player);
		}
		public function ActiveGameFrame():void
		{
			if(m_clientContainer)
				m_clientContainer.ActiveGameClass(this);
		}
		//切换椅子
		public function SwitchViewChairID( wChairID:uint):uint
		{
			var pMeUserData:tagUserInfoHead = GetMeUserInfo();
			var pServerAttribute:tagServerAttribute = GetServerAttribute();
			if(pMeUserData == null || pServerAttribute == null)
			{
				return 0;
			}
			//转换椅子
			var wViewChairID:uint=(wChairID+pServerAttribute.wChairCount-pMeUserData.UserStatus.wChairID);
			switch (pServerAttribute.wChairCount)
			{
				case 2 :
					{
						wViewChairID+=1;
						break;





					};
				case 3 :
					{
						wViewChairID+=1;
						break;





					};
				case 4 :
					{
						wViewChairID+=2;
						break;





					};
				case 5 :
					{
						wViewChairID+=2;
						break;





					};
				case 6 :
					{
						wViewChairID+=3;
						break;





					};
				case 7 :
					{
						wViewChairID+=3;
						break;





					};
				case 8 :
					{
						wViewChairID+=4;
						break;





				}
			};
			return wViewChairID % pServerAttribute.wChairCount;
		}
		private function timerHandler(nIDEvent:int):void
		{
			//时间处理
			if ((m_nTimerID==nIDEvent) && (m_wTimerChairID!=INVALID_CHAIR))
			{
				//消息处理
				if (m_nElapseCount>0)
				{
					m_nElapseCount--;
				}
				
				var bSuccess:Boolean=OnEventTimer(m_wTimerChairID,
				 m_nElapseCount,
				 m_nTimerID);
				if ((m_nElapseCount==0)||(bSuccess==false))
				{
					KillGameTimer(m_nTimerID);
				}
				
				return;
			}
			OnTimer(nIDEvent);
		}
		/////////////////////////////////////

		//////////////////
		//信道数据处理
		public function OnIPCChannelMessage(wMainCmdID:uint,
		  wSubCmdID:uint,
		  pIPCBuffer:*, 
		  wDataSize:uint):Boolean
		{
			if (OnEventProcess(wMainCmdID,wSubCmdID,pIPCBuffer,wDataSize))
			{
				return true;
			}
			//默认处理
			switch (wMainCmdID)
			{
				case IPC_MAIN_CONFIG ://配置信息
					{
						return OnIPCConfig(wMainCmdID,wSubCmdID,pIPCBuffer,wDataSize);
					};
				case IPC_MAIN_USER ://用户信息
					{
						return OnIPCUser(wMainCmdID,wSubCmdID,pIPCBuffer,wDataSize);
					};
				case IPC_MAIN_CONCTROL ://控制消息
					{
						return OnIPCControl(wMainCmdID,wSubCmdID,pIPCBuffer,wDataSize);
				}
			};

			return false;
		}
		public function OnIPCSocketMessage(wMainCmdID:uint,
		wSubCmdID:uint,
		pBuffer:ByteArray,
		wDataSize:int):Boolean
		{
			var bSuccess:Boolean=OnIPCSocket(wMainCmdID,wSubCmdID,pBuffer,wDataSize);
			if (bSuccess==false)
			{
				m_bInquire=false;	
				SendEventExitGameClient();
				return false;
			}
			return true;
		}
		//配置信息
		public function OnIPCConfig(wMainCmdID:uint,
		  wSubCmdID:uint,
		  pIPCBuffer:*, 
		  wDataSize:uint):Boolean
		{
			switch (wSubCmdID)
			{
				case IPC_SUB_SERVER_INFO ://房间信息
					{
						//效验参数
						if (wDataSize<IPC_GF_ServerInfo.sizeof_IPC_GF_ServerInfo)
						{
							return false;
						}

						//处理数据
						var pServerInfo:IPC_GF_ServerInfo=pIPCBuffer as IPC_GF_ServerInfo;
						m_wTableID=pServerInfo.wTableID;
						m_wChairID=pServerInfo.wChairID;
						m_dwUserID=pServerInfo.dwUserID;
						if (m_ServerAttribute)
						{
							m_ServerAttribute.wKindID=pServerInfo.wKindID;
							m_ServerAttribute.wServerID=pServerInfo.wServerID;
							m_ServerAttribute.wGameGenre=pServerInfo.wGameGenre;
							m_ServerAttribute.wChairCount=pServerInfo.wChairCount;
							m_ServerAttribute.szKindName = String(pServerInfo.szKindName);
							m_ServerAttribute.szServerName = String(pServerInfo.szServerName);
							m_ServerAttribute.fCellScore = Number(pServerInfo.fCellScore);
							m_ServerAttribute.fHighScore = Number(pServerInfo.fHighScore);
							m_ServerAttribute.fLessScore = Number(pServerInfo.fLessScore);
							
							//更新游戏属性
							if(m_GameClientView)
								m_GameClientView.UpdateServerAttribute(m_ServerAttribute);
						}
						
						return true;

				}
			};
			return false;
		}
		//用户信息
		public function OnIPCUser(wMainCmdID:uint,
		  wSubCmdID:uint,
		  pIPCBuffer:*, 
		  wDataSize:uint):Boolean
		{
			switch (wSubCmdID)
			{
				case IPC_SUB_USER_COME ://用户消息
					{
						if (wDataSize<tagUserInfoHead.sizeof_tagUserInfoHead)
						{
							return false;
						}
						var UserData:tagUserInfoHead = pIPCBuffer as tagUserInfoHead;
						ActiveUserItem(UserData.clone());
						return true;






					};
				case IPC_SUB_USER_STATUS ://用户状态
					{
						if (wDataSize<IPC_UserStatus.sizeof_IPC_UserStatus)
						{
							return false;
						}
						//消息处理
						var pUserStatus:IPC_UserStatus=pIPCBuffer as IPC_UserStatus;
						if (pUserStatus.cbUserStatus<US_SIT)
						{
							if (pUserStatus.dwUserID==m_dwUserID)
							{
								DestroyGameClass();
								return true;
							}
							else
							{
								DeleteUserItem(pUserStatus.dwUserID);
							}
						}
						else
						{
							UpdateUserItem(pUserStatus.dwUserID,
							   pUserStatus.cbUserStatus,
							   pUserStatus.wNetDelay);
						}
						return true;





					};
				case IPC_SUB_USER_SCORE ://用户积分
					{
						if (wDataSize<IPC_UserScore.sizeof_IPC_UserScore)
						{
							return false;
						}

						//消息处理
						var pUserScore:IPC_UserScore=pIPCBuffer as IPC_UserScore;
						UpdateUserScore(pUserScore.dwUserID,pUserScore.UserScore);
						return true;





					};
				case IPC_SUB_GAME_START ://游戏开始
					{
						//设置用户
						if (m_ServerAttribute)
						{
							for (var i:uint=0; i<m_ServerAttribute.wChairCount; i++)
							{
								if (m_pUserItem[i]!=null)
								{
									m_pUserItem[i].UserStatus.cbUserStatus=US_PLAY;
									OnEventUserStatus(m_pUserItem[i],
									m_pUserItem[i].UserStatus.wChairID,
									false);
								}
							}

							return true;
						}



					};
				case IPC_SUB_GAME_FINISH ://游戏结束
					{
						if (m_ServerAttribute)
						{
							//设置用户
							for (var i:uint=0; i<m_ServerAttribute.wChairCount; i++)
							{
								if (m_pUserItem[i]!=null)
								{
									m_pUserItem[i].UserStatus.cbUserStatus=US_SIT;
									OnEventUserStatus(m_pUserItem[i],
									m_pUserItem[i].UserStatus.wChairID,
									false);
								}
							}

							return true;
						}




				}
			};
			return false;
		}
		//控制消息
		public function OnIPCControl(wMainCmdID:uint,
		  wSubCmdID:uint,
		  pIPCBuffer:*, 
		  wDataSize:uint):Boolean
		{
			switch (wSubCmdID)
			{
				case IPC_SUB_START_FINISH ://启动完成
					{
						//获取场景
						var Info:CMD_GF_Info = new CMD_GF_Info;
						Info.bAllowLookon=m_bAllowLookon?1:0;
						var pBuffer:ByteArray = Info.toByteArray();
						//发送消息
						SendSocketChannelMessage(MDM_GF_FRAME,SUB_GF_INFO,pBuffer,CMD_GF_Info.sizeof_CMD_GF_Info);
						
						return true;


					};
				case IPC_SUB_CLOSE_FRAME ://关闭框架
					{
						m_bInquire=false;
						CloseChannelMessage(true,true);
						
						return true;

					}
			};
			return true;
		}
		public function OnIPCSocket(wMainCmdID:uint,
		wSubCmdID:uint,
		pBuffer:ByteArray,
		wDataSize:int):Boolean
		{
			//特殊处理
			var bHandle:Boolean=false;

			bHandle=OnEventSocket(wMainCmdID,
			 wSubCmdID,
			 pBuffer,
			 wDataSize);
			//默认处理
			if ((bHandle==false)&&(wMainCmdID==MDM_GF_FRAME))
			{
				switch (wSubCmdID)
				{
					case SUB_GF_OPTION ://游戏配置
						{
							var pOption:CMD_GF_Option=CMD_GF_Option.readData(pBuffer);
							m_bGameStatus=pOption.bGameStatus;
							m_bAllowLookon=pOption.bAllowLookon?true:false;
							return true;
						}
					case SUB_GF_SCENE ://游戏场景
						{
							return OnGameSceneMessage(m_bGameStatus,m_bLookonMode,pBuffer,wDataSize);
						}

					case SUB_GF_USER_CHAT ://聊天信息
						{
							return true;
						}
					case SUB_GF_MESSAGE ://系统消息
						{
							return true;
					}
				};
				return true;
			}
			return bHandle;
		}

		///////////////////////////////////////////////
		protected function SendIPCChannelMessage(wMainCmdID:uint,
		  wSubCmdID:uint,
		  pIPCBuffer:* = null, 
		  wDataSize:uint = 0):Boolean
		{
			if (m_clientContainer)
			{
				return m_clientContainer.OnIPCChannelMessage(wMainCmdID,wSubCmdID,pIPCBuffer,wDataSize);
			}
			else
			{
				return false;
			}
		}
		protected function SendSocketChannelMessage(wMainCmdID:uint,
		  wSubCmdID:uint,
		  pIPCBuffer:ByteArray = null, 
		  wDataSize:uint = 0):Boolean
		{
			if (m_clientContainer)
			{
				return m_clientContainer.OnIPCSocketMessage(wMainCmdID,wSubCmdID,pIPCBuffer,wDataSize);
			}
			else
			{
				return false;
			}
		}
		private function CloseChannelMessage( bNotifyServer:Boolean, 
		 bNotifySink:Boolean):void
		{
			//发送关闭消息
			if (bNotifyServer==true)
			{
				SendIPCChannelMessage(IPC_MIAN_IPC_KERNEL,
				  IPC_SUB_IPC_CLIENT_CLOSE);
			}
		}
		//增加用户
		private function ActiveUserItem(pActiveUserData:tagUserInfoHead):Boolean
		{
			if (pActiveUserData.UserStatus.wTableID==INVALID_TABLE ||
			m_ServerAttribute == null ||
			m_UserItemFactory == null)
			{
				return false;
			}
			if (pActiveUserData.UserStatus.wChairID>=m_ServerAttribute.wChairCount)
			{
				return false;
			}
			m_UserItemFactory.append(pActiveUserData);
			//设置变量
			var bLookonMode:Boolean=
			(pActiveUserData.UserStatus.cbUserStatus==US_LOOKON);
			if (bLookonMode==false)
			{
				m_pUserItem[pActiveUserData.UserStatus.wChairID]=pActiveUserData;
			}
			else
			{
				if (m_UserItemLookon)
				{
					m_UserItemLookon.append(pActiveUserData);
				}
			}

			//判断自己
			if (m_dwUserID==pActiveUserData.dwUserID)
			{
				m_bLookonMode=bLookonMode;
				m_pMeUserItem=pActiveUserData;
				m_wTableID=m_pMeUserItem.UserStatus.wTableID;
				m_wChairID=m_pMeUserItem.UserStatus.wChairID;
			}

			//通知改变
			OnEventUserEnter(pActiveUserData,
				 pActiveUserData.UserStatus.wChairID,
				bLookonMode);

			return true;
		}
		//删除用户
		private function DeleteUserItem(dwUserID:uint):Boolean
		{
			if (m_ServerAttribute == null ||
			m_UserItemFactory == null)
			{
				return false;
			}
			//游戏用户
			var pUserData:tagUserInfoHead=null;
			for (var i:uint=0; i<m_ServerAttribute.wChairCount; i++)
			{
				pUserData=m_pUserItem[i];
				if ((pUserData!=null)&&(pUserData.dwUserID==dwUserID))
				{
					//设置变量
					m_pUserItem[i]=null;
					m_UserItemFactory.remove(pUserData);

					//通知改变
					OnEventUserLeft(pUserData,
						   pUserData.UserStatus.wChairID,
						   false);

					return true;
				}
			}

			//旁观用户
			if (m_UserItemLookon && m_UserItemFactory)
			{
				for (var i:uint=0; i<m_UserItemLookon.size(); i++)
				{
					pUserData=m_UserItemLookon.get(i);
					if (pUserData.dwUserID==dwUserID)
					{
						//设置变量
						m_UserItemLookon.removeAt(i);
						m_UserItemFactory.remove(pUserData);

						//判断自己
						if (m_dwUserID==dwUserID)
						{
							m_pMeUserItem=null;
							m_wTableID=INVALID_TABLE;
							m_wChairID=INVALID_CHAIR;
						}
						
						//通知改变
						OnEventUserLeft(pUserData,
							  pUserData.UserStatus.wChairID,true);
						

						return true;
					}
				}
			}

			return false;
		}
		//更新用户
		private function UpdateUserScore(dwUserID:uint, pUserScore:tagUserScore):Boolean
		{
			//寻找用户
			var pUserData:tagUserInfoHead=SearchUserItem(dwUserID);
			if (pUserData==null)
			{
				return false;
			}

			//设置数据

			//通知改变
			var bLookonUser:Boolean=(pUserData.UserStatus.cbUserStatus==US_LOOKON);
		
			OnEventUserScore(pUserData,
				pUserData.UserStatus.wChairID,
				bLookonUser);
			return true;
		}
		private function UpdateUserItem(dwUserID:uint, 
		cbUserStatus:uint,
		wNetDelay:uint)
		{
			//寻找用户
			var pUserData:tagUserInfoHead=SearchUserItem(dwUserID);
			if (pUserData==null)
			{
				return false;
			}

			//设置数据
			pUserData.UserStatus.wNetDelay=wNetDelay;
			pUserData.UserStatus.cbUserStatus=cbUserStatus;
			
			//通知改变
			var bLookonUser:Boolean=(cbUserStatus==US_LOOKON);
			OnEventUserStatus(pUserData,
				  pUserData.UserStatus.wChairID,
				  bLookonUser);
			return true;
		}
		//查找用户
		private function SearchUserItem(dwUserID:uint):tagUserInfoHead
		{
			if (m_UserItemFactory == null)
			{
				return null;
			}//变量定义
			var wIndex:uint=0;
			var pUserData:tagUserInfoHead=null;

			//寻找用户
			do
			{
				pUserData=m_UserItemFactory.get(wIndex++);
				if (pUserData==null)
				{
					break;
				}
				if (pUserData.dwUserID==dwUserID)
				{
					return pUserData;
				}
			} while (true);

			return null;
		}
		//////////////////////////////////////
		//时间消息
		protected function  OnEventTimer(wChairID:uint,  nElapse:uint,  nTimerID:uint):Boolean
		{
			return false;
		}
		//网络消息
		public function OnEventSocket(wMainCmdID:uint,
		wSubCmdID:uint,
		pBuffer:ByteArray,
		wDataSize:int):Boolean
		{
			
			switch (wMainCmdID)
			{
				case MDM_GF_FRAME ://框架消息
				{
					return OnFrameMessage(wSubCmdID,pBuffer,wDataSize);
				}
				case MDM_GF_GAME ://游戏消息
				{
					return OnGameMessage(wSubCmdID,pBuffer,wDataSize);
				}
			}

			return false;
		}
		//进程消息
		protected function  OnEventProcess(wMainCmdID:uint,
								wSubCmdID:uint,
								pBuffer:*,
								wDataSize:int):Boolean
		{
			return false;
		}

		//游戏事件
		//游戏状态
		//网络消息
		protected function OnGameMessage(wSubCmdID:uint, pBuffer:ByteArray, wDataSize:uint):Boolean
		{
			return false;
		}
		//游戏场景
		protected function OnFrameMessage(wSubCmdID:uint, pBuffer:ByteArray,  wDataSize:uint):Boolean
		{
			return false;
		}

		protected function OnGameSceneMessage(cbGameStation:uint,  bLookonOther:Boolean, pBuffer:ByteArray,  wDataSize:uint):Boolean
		{
			return false;
		}
		
		//聊天信息
		protected function  OnUserChatMessage(pSendUserData:tagUserInfoHead, pRecvUserData:tagUserInfoHead,  
									pszChatMessage:String,  crColor:uint):Boolean
		{
			return true;
		}
		//用户进入
		protected function  OnEventUserEnter(pUserData:tagUserInfoHead,  wChairID:uint,  bLookonUser:Boolean):void
		{
			if (bLookonUser == false)
			{
				var wViewChairID:uint=this.SwitchViewChairID(wChairID);
				m_GameClientView.SetUserInfo(wViewChairID,pUserData);
				if(wChairID == GetMeChairID())
					m_GameClientView.SetNetSpeed(pUserData.UserStatus.wNetDelay);
			}
		}
		//用户离开
		protected function  OnEventUserLeft(pUserData:tagUserInfoHead,  wChairID:uint,  bLookonUser:Boolean):void
		{
			if (bLookonUser == false)
			{
				var wViewChairID:uint=this.SwitchViewChairID(wChairID);
				m_GameClientView.SetUserInfo(wViewChairID,null);
			}
		}
		//用户积分
		protected function  OnEventUserScore(pUserData:tagUserInfoHead,  wChairID:uint,  bLookonUser:Boolean):void
		{
		}
		//用户状态
		protected function  OnEventUserStatus(pUserData:tagUserInfoHead,  wChairID:uint,  bLookonUser:Boolean):void
		{
			if (bLookonUser == false)
			{
				var wViewChairID:uint=this.SwitchViewChairID(wChairID);
				m_GameClientView.SetUserStatus(wViewChairID,pUserData);
				if(wChairID == GetMeChairID())
					m_GameClientView.SetNetSpeed(pUserData.UserStatus.wNetDelay);
			}
		}
		//时间更新
		protected function  OnEventTimerKilled(wChairID:uint,  nTimerID:uint):void
		{
			var wViewChairID:uint=this.SwitchViewChairID(wChairID);
			m_GameClientView.SetUserTimer(wViewChairID,0);
		}
		//旁观状态
		protected function  OnEventLookonChanged( bLookonUser:Boolean, pBuffer:ByteArray,  wDataSize:uint):void
		{
		}
		///////////////////////////////
		protected function OnStart(wParam:uint, lParam:uint):uint
		{
			return 0;
		}
		protected function OnExit(wParam:uint, lParam:uint = 0):uint
		{
			SendEventExitGameClient(lParam > 0 ? true : false);
			return 0;
		}
	}
}
include "./StringTableDef.as"
include "../../GlobalConst.as"
include "../../IGameFrame/ChannelMessageDef.as"
include "../../Net/GLOBALDEF.as"
include "../../Net/GLOBALFrame.as"
include "../../Net/CMD_Plaza.as"
include "../../Net/CMD_Game.as"