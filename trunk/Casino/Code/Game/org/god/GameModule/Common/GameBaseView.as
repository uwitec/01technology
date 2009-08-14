package org.god.GameModule.Common
{
	import flash.display.MovieClip;
	import flash.display.Graphics;
	import flash.events.Event;
	import flash.geom.Point;
	import flash.geom.Matrix;
	import flash.display.BitmapData;
	import flash.display.GradientType;

	import org.god.Net.*;
	import org.god.IGameFrame.*;
	
	import org.aswing.graphics.*;
	import org.aswing.ASColor;

	public class GameBaseView extends MovieClip implements IGameView
	{
		public static var nGameUIWidth:uint = 1024;
		public static var nGameUIHeight:uint= 768;
		
		protected var m_bInitGameView:Boolean;
		protected var m_bFreeChildClassViewData:Boolean;
		
		protected var m_nXFace:int;//头像高度
		protected var m_nYFace:int;//头像宽度
		protected var m_nXReady:int;//准备宽
		protected var m_nYReady:int;//准备高
		protected var m_nXTimer:int;//定时器宽
		protected var m_nYTimer:int;//定时器高
		protected var m_nXBorder:int;//框架宽度
		protected var m_nYBorder:int;//框架高度

		protected var m_GameClass:GameBaseClass;
		//用户变量
		protected var m_wTimer:Array = new Array(MAX_CHAIR);//用户时间
		protected var m_pUserItem:Array = new Array(MAX_CHAIR);//用户信息
		
		protected var m_pUserFace:Array = new Array(MAX_CHAIR);//用户头像
		protected var m_ptFace:Array = new Array(MAX_CHAIR);//头像位置
		
		protected var m_pUserChairIDView:Array = new Array(MAX_CHAIR);//名字
		protected var m_ptName:Array = new Array(MAX_CHAIR);//名字位置
		
		protected var m_pClockView:Array = new Array(MAX_CHAIR);
		protected var m_ptTimer:Array = new Array(MAX_CHAIR);//时间位置
		
		protected var m_pUserReadyView:Array = new Array(MAX_CHAIR);
		protected var m_ptReady:Array = new Array(MAX_CHAIR);//准备位置
		
		protected var m_pNetSpeedView:INetSpeedView;
		protected var m_ptNetSpeedView:Point;
		protected var m_pUserInfoView:IUserInfoView;
		protected var m_ptUserInfoView:Point;
		
		protected var m_DrawBitmapData:BitmapData;
		protected var m_bWindowDrawMode:Boolean;
		
		public function GameBaseView(prarentClass:GameBaseClass)
		{
			m_GameClass = prarentClass;
			m_bWindowDrawMode = false;
			m_bInitGameView = false;
			m_bFreeChildClassViewData = false;
		}
		public function InitGameView():Boolean
		{
			if(m_bInitGameView)
			{
				return true;
			}
			for (var i:int = 0; i < MAX_CHAIR; i ++)
			{
				m_ptName[i] = new Point;
				m_ptFace[i] = new Point;
				m_ptTimer[i] = new Point;
				m_ptReady[i] = new Point;
			}
			
			m_ptNetSpeedView = new Point;
			m_pNetSpeedView = CreateNetSpeedView();
			if(m_pNetSpeedView)
				addChild(m_pNetSpeedView.getMovieClip());

			m_ptUserInfoView = new Point;
			m_pUserInfoView = CreateUserInfoView();
			if(m_pUserInfoView && m_pUserInfoView.createUserInfoView() == true)
				addChild(m_pUserInfoView.getMovieClip());
			else
			{
				if(m_pUserInfoView)
					m_pUserInfoView.destroy();
				m_pUserInfoView = null;
			}

			if(m_bWindowDrawMode)
			{
				m_DrawBitmapData = CreateDrawBitmapData();
				if(m_DrawBitmapData)
					stage.addEventListener(Event.RENDER, __render);
			}
			
			m_bInitGameView = true;
			return true;
		}
		public function DestroyGameView():void
		{
			if(m_bInitGameView == false)
				return ;
			ZeroArray(m_pUserFace,null,removeUserFace);
			m_pUserFace = null;
			ZeroArray(m_pUserChairIDView,null,removeUserChairIDView);
			m_pUserChairIDView = null;
			ZeroArray(m_pUserReadyView,null,removeUserReadyView);
			m_pUserReadyView = null;
			ZeroArray(m_pClockView,null,removeClockView);
			m_pClockView = null;
			ZeroArray(m_wTimer);
			m_wTimer = null;
			if(m_pNetSpeedView)
			{
				removeChild(m_pNetSpeedView.getMovieClip());
				m_pNetSpeedView = null;
				m_ptNetSpeedView = null;			
			}
			if(m_pUserInfoView)
			{
				m_pUserInfoView.destroy();
				removeChild(m_pUserInfoView.getMovieClip());
				m_pUserInfoView = null;
				m_ptUserInfoView = null;
			}
			ZeroArray(m_ptName,null);
			m_ptName = null;
			ZeroArray(m_ptFace,null);
			m_ptFace = null;
			ZeroArray(m_ptTimer,null);
			m_ptTimer = null;
			ZeroArray(m_ptReady,null);
			m_ptReady = null;
			ZeroArray(m_pUserItem,null);	
			m_pUserItem = null;

			if(stage)
				stage.removeEventListener(Event.RENDER, __render);
			if(m_DrawBitmapData)
			{
				m_DrawBitmapData.dispose();
				m_DrawBitmapData = null;
			}
			m_GameClass = null;
			m_bInitGameView = false;
		}
		

		public function RectifyGameView(nWidth:int,nHeight:int):void
		{
			if(m_pNetSpeedView)
			{
				m_pNetSpeedView.moveMovieClip(m_ptNetSpeedView.x, m_ptNetSpeedView.y);
			}
			if(m_pUserInfoView)
			{
				m_pUserInfoView.moveMovieClip(m_ptUserInfoView.x, m_ptUserInfoView.y);
			}
		}
	
		public function GetGameViewMC():MovieClip
		{
			return this;
		}
		
	
		//////////////////////////////////////
		protected function CreateUserFaceView(wChairID:uint,pUserItem:tagUserInfoHead):IFaceView
		{
			return null;
		}
		protected function CreateUserChairIDView(wChairID:uint,pUserItem:tagUserInfoHead):IChairIDView
		{
			return null;
		}
		protected function CreateUserReadyView(wChairID:uint,pUserItem:tagUserInfoHead):IUserReadyView
		{
			return null;
		}
		protected function CreateClockView(wChairID:uint,wTimer:uint):IClockView
		{
			return null;
		}
		protected function CreateNetSpeedView():INetSpeedView
		{
			return null;
		}
		protected function CreateUserInfoView():IUserInfoView
		{
			return null;
		}		
		protected function CreateDrawBitmapData():BitmapData
		{
			return null;
		}
		protected function GetUserFaceMCIndex(wChairID:uint):int
		{
			return -1;
		}

		protected function  DrawGameView(g:Graphics,pDC:BitmapData):void
		{
			
		}
		private function removeUserFace(face:IFaceView):void
		{
			if(face)
			{
				var faceMC:MovieClip = face.getMovieClip();
				if(faceMC)
				{
					removeChild(faceMC);
					faceMC = null;
				}
				face.destroy();
				face = null;
			}
		}
		private function removeUserChairIDView(view:IChairIDView):void
		{
			if(view)
			{
				var viewMC:MovieClip = view.getMovieClip();
				if(viewMC)
				{
					removeChild(viewMC);
					viewMC = null;
				}
				view.destroy();
				view = null;
			}
		}
		private function removeUserReadyView(view:IUserReadyView):void
		{
			if(view)
			{
				var viewMC:MovieClip = view.getMovieClip();
				if(viewMC)
				{
					removeChild(viewMC);
					viewMC = null;
				}
				view.destroy();
				view = null;
			}
		}	
		private function removeClockView(view:IClockView):void
		{
			if(view)
			{
				var viewMC:MovieClip = view.getMovieClip();
				if(viewMC)
				{
					removeChild(viewMC);
					viewMC = null;
				}
				view.destroy();
				view = null;
			}
		}	
		///////////////////////
		//功能函数
		//获取时间
		public function GetUserTimer(wChairID:uint):uint
		{
			if (wChairID>=MAX_CHAIR)
			{
				return 0;
			}
			return m_wTimer[wChairID];
		}
		//获取玩家
		public function GetUserInfo(wChairID:uint):tagUserInfoHead
		{
			if (wChairID>=MAX_CHAIR)
			{
				return null;
			}
			return m_pUserItem[wChairID];
		}
		//设置时间
		public function SetUserTimer(wChairID:uint,wTimer:uint):Boolean
		{
			if (wChairID>=MAX_CHAIR)
			{
				return false;
			}
			m_wTimer[wChairID]=wTimer;
			if(m_pClockView[wChairID] == null)
			{
				m_pClockView[wChairID] = CreateClockView(wChairID,wTimer);
				if(m_pClockView[wChairID])
				{
					var mc:MovieClip = m_pClockView[wChairID].getMovieClip();
					if(mc)
					{
						addChild(mc);
						m_pClockView[wChairID].moveMovieClip(m_ptTimer[wChairID].x,
													m_ptTimer[wChairID].y);
					}
				}
			}
			UpdateClockView(wChairID, wTimer);
			return true;
		}
		//设置用户
		public function SetUserInfo(wChairID:uint,pUserItem:tagUserInfoHead):Boolean
		{
			if (wChairID>=MAX_CHAIR)
			{
				return false;
			}
			
			UpdateUserFaceView(wChairID,pUserItem);
			UpdateChairIDView(wChairID,pUserItem);
			UpdateUserReadyView(wChairID,pUserItem);
			
			m_pUserItem[wChairID]=pUserItem;
			
			UpdateGameView();
			return true;
		}
		public function SetNetSpeed(wNetDelay:int):void
		{
			if(m_pNetSpeedView)
			{
				m_pNetSpeedView.setNetSpeed(wNetDelay);
			}
		}
		//更新界面
		public function  SetUserStatus(wChairID:uint,pUserData:tagUserInfoHead):void
		{
			UpdateUserReadyView(wChairID,pUserData);
			UpdateUserFaceView(wChairID,pUserData);
			UpdateChairIDView(wChairID,pUserData);
		}
		private function equalUserItem(pUserItem0:tagUserInfoHead,
											  pUserItem1:tagUserInfoHead):Boolean
		{
			if(pUserItem0 == pUserItem1)
			{
				return true;
			}
			if(pUserItem0 == null && pUserItem1 != null)
			{
				return false;
			}
			if(pUserItem0 != null && pUserItem1 == null)
			{
				return false;
			}
			if(pUserItem0 != null && pUserItem1 != null)
			{
				if(pUserItem0.dwUserID != pUserItem1.dwUserID)
				{
					return false;
				}
			}
			
			return true;
		}
		private function UpdateUserFaceView(wChairID:uint,pUserItem:tagUserInfoHead):void
		{
			if(equalUserItem(m_pUserItem[wChairID],pUserItem) == false)
			{
				if(m_pUserFace[wChairID])
				{
					removeUserFace(m_pUserFace[wChairID]);
					m_pUserFace[wChairID] = null;
				}
			}
			if(pUserItem != null && m_pUserFace[wChairID] == null)
			{
				m_pUserFace[wChairID] = CreateUserFaceView(wChairID,pUserItem);
				if(m_pUserFace[wChairID])
				{
					var mc:MovieClip = m_pUserFace[wChairID].getMovieClip();
					if(mc)
					{
						var nMCIndex:int = GetUserFaceMCIndex(wChairID);
						if(nMCIndex == -1)
							addChild(mc);
						else
							addChildAt(mc,nMCIndex);
						m_pUserFace[wChairID].moveMovieClip(m_ptFace[wChairID].x,
													m_ptFace[wChairID].y);
					}
				}
				
			}
			if(pUserItem && m_pUserFace[wChairID])
			{
				m_pUserFace[wChairID].setOffLine(pUserItem.UserStatus.cbUserStatus==US_OFFLINE);
			}
		}
		private function equalUserChairIDView(pUserItem0:tagUserInfoHead,
											  pUserItem1:tagUserInfoHead):Boolean
		{
			if(pUserItem0 == pUserItem1)
			{
				return true;
			}
			if(pUserItem0 == null && pUserItem1 != null)
			{
				return false;
			}
			if(pUserItem0 != null && pUserItem1 == null)
			{
				return false;
			}
			if(pUserItem0 != null && pUserItem1 != null)
			{
				if(pUserItem0.dwUserID != pUserItem1.dwUserID)
				{
					return false;
				}
			}
			
			return true;
		}
		private function UpdateChairIDView(wChairID:uint,pUserItem:tagUserInfoHead):void
		{
			if(equalUserChairIDView(m_pUserItem[wChairID],
										pUserItem) == false)
			{
				if(m_pUserChairIDView[wChairID])
				{
					removeUserChairIDView(m_pUserChairIDView[wChairID]);
					m_pUserChairIDView[wChairID] = null;
				}
			}
				
			if(pUserItem != null &&
			   m_pUserChairIDView[wChairID] == null)
			{
				m_pUserChairIDView[wChairID] = CreateUserChairIDView(wChairID,pUserItem);
				if(m_pUserChairIDView[wChairID])
				{
					var mc:MovieClip = m_pUserChairIDView[wChairID].getMovieClip();
					if(mc)
					{
						addChild(mc);
						m_pUserChairIDView[wChairID].moveMovieClip(m_ptName[wChairID].x,
													m_ptName[wChairID].y);
					}
				}
			}		
			if(pUserItem && m_pUserChairIDView[wChairID])
			{
				m_pUserChairIDView[wChairID].setOffLine(pUserItem.UserStatus.cbUserStatus==US_OFFLINE);
			}
		}
		
		private function equalUserReadyView(pUserItem0:tagUserInfoHead,
											  pUserItem1:tagUserInfoHead):Boolean
		{
			if(pUserItem0 == pUserItem1)
			{
				return true;
			}
			if(pUserItem0 == null && pUserItem1 != null)
			{
				return false;
			}
			if(pUserItem0 != null && pUserItem1 == null)
			{
				return false;
			}
			if(pUserItem0 != null && pUserItem1 != null)
			{
				if(pUserItem0.dwUserID != pUserItem1.dwUserID)
				{
					return false;
				}
				if(pUserItem0.UserStatus.cbUserStatus != pUserItem1.UserStatus.cbUserStatus)
				{
					return false;
				}
			}
			
			return true;
		}
		private function UpdateUserReadyView(wChairID:uint,
											 pUserItem:tagUserInfoHead):void
		{
			if(equalUserReadyView(m_pUserItem[wChairID],
										pUserItem) == false ||
			   (pUserItem != null &&
			   pUserItem.UserStatus.cbUserStatus!=US_READY))
			{
				if(m_pUserReadyView[wChairID])
				{
					removeUserReadyView(m_pUserReadyView[wChairID]);
					m_pUserReadyView[wChairID] = null;
				}
			}
			if(pUserItem != null &&
			   pUserItem.UserStatus.cbUserStatus==US_READY)
			{
				if(m_pUserReadyView[wChairID] == null)
				{
					m_pUserReadyView[wChairID] = CreateUserReadyView(wChairID,pUserItem);
					if(m_pUserReadyView[wChairID])
					{
						var mc:MovieClip = m_pUserReadyView[wChairID].getMovieClip();
						if(mc)
						{
							addChild(mc);
							m_pUserReadyView[wChairID].moveMovieClip(m_ptReady[wChairID].x,
															m_ptReady[wChairID].y);
						}
					}
				}
			}
		}
		private function  UpdateClockView(wChairID:uint, wTimer:uint):void
		{
			if(m_pClockView[wChairID])
			{
				m_pClockView[wChairID].setSecond(wTimer);
			}
		}
		private function __render(e:Event):void
		{
			if(m_bWindowDrawMode && m_DrawBitmapData)
			{
				this.graphics.clear();
			
				var g:Graphics2D = new Graphics2D(this.graphics);
				
				var pDC:BitmapData = m_DrawBitmapData.clone();
				DrawGameView(this.graphics, pDC);
				
 				var bBrush:BitmapBrush = new BitmapBrush(pDC, null, false, true);	
 				g.fillRectangle(bBrush,0,0,
								pDC.width,
								pDC.height);
				
				
				bBrush = null;
				g = null;
				pDC    = null;
			}
			
		}
		//更新游戏视图
		public function UpdateGameView():void
		{
			if(m_bWindowDrawMode && stage)
				stage.invalidate();
		}
		//更新游戏属性
		public function UpdateServerAttribute(attr:tagServerAttribute):void
		{
		}
	}
}
include "../../Common/Memory.as"
include "../../IGameFrame/ChannelMessageDef.as"
include "../../Net/GLOBALDEF.as"
include "../../Net/GLOBALFrame.as"
include "../../Net/CMD_Plaza.as"
include "../../Net/CMD_Game.as"