package 
{
	import flash.display.Sprite;
	import flash.display.Shape;
	import flash.display.SimpleButton;
	import flash.events.MouseEvent;
	
	import org.god.Net.tagGameServer;
	import org.god.Net.tagGameKind;
	import org.god.IGameFrame.ITableView;
	import org.god.IGameFrame.ITableFrameView;
	import org.god.IGameFrame.IServerItem;
	import org.god.IGameFrame.IUserItem;
	import org.god.IGameFrame.tagTableState;
	
	import org.aswing.Component;
	import org.aswing.JLabel;
	import org.aswing.geom.IntPoint;
	import org.aswing.geom.IntRectangle;
	import org.aswing.ASFont;
	import org.aswing.ASColor;
	
	//游戏桌视图
	public class TableView extends Component implements ITableView
	{
		private var m_tableMC:Sprite;//桌子
		private var m_labelTableID:JLabel;//游戏桌
		private var m_bMouseDown;//是否按下
		private var m_bFocusFrame;//框架标志
		private var m_wHoverChairID;//盘旋椅子
		private var m_TableState:tagTableState;//桌子属性
		private var m_pITableFrameView:ITableFrameView;//框架接口//桌子属性
		private var m_UserFaceMC:Array;//头像
		private var m_GameServer:tagGameServer;//服务器属性
		private var m_GameKind:tagGameKind;//游戏类型
		private var m_UserFaceShape:Array;//头像位置简单形状
		private var m_pTableFrameView:TableFrameView;
		
		public function TableView(pTableFrameView:TableFrameView)
		{
			super();
			m_pTableFrameView = pTableFrameView;
			m_TableState = new tagTableState;
			m_UserFaceMC = new Array(m_TableState.pIUserItem.length);
		}
		//初始化函数
		public function InitTableView(pGameServer:IServerItem,
									  pGameKind:IServerItem,
		  wTableID:uint,  
		  wChairCount:uint, 
		  pITableFrameView:ITableFrameView):void
		{
			m_TableState.wTableID=wTableID;
			m_TableState.wChairCount=wChairCount;

			m_pITableFrameView = pITableFrameView;

			m_GameServer = pGameServer.getItemData();
			m_GameKind   = pGameKind.getItemData();
			if (m_GameServer != null && m_GameKind!= null)
			{
				m_tableMC = TableResource.getTableViewMC(m_GameKind.wProcessType,
				wChairCount);
				if (m_tableMC)
				{
					var rect:IntRectangle = TableResource.getTableViewRect(m_GameKind.wProcessType,
					wChairCount);
					m_tableMC.x= rect.x;
					m_tableMC.y= rect.y;
					m_tableMC.width = rect.width;
					m_tableMC.height= rect.height;
					addChild(m_tableMC);

					var strTableID:String = "--" + wTableID + "--";
					m_labelTableID = new JLabel(strTableID,null,JLabel.CENTER);
					addChild(m_labelTableID);
					m_labelTableID.x = rect.x;
					m_labelTableID.width = rect.width;
					m_labelTableID.height= 12;
					var nYOffer:int = -2;
					m_labelTableID.y = (rect.y + rect.height) - m_labelTableID.height + nYOffer;
					var font:ASFont = new ASFont;
					font = font.changeSize(12);
					font = font.changeBold(true);
					m_labelTableID.setFont(font);
					m_labelTableID.setForeground(new ASColor(0x0a0a0a));
					
					m_UserFaceShape = new Array(wChairCount); 
					for(var i:int = 0; i < wChairCount; i ++)
					{
						var rect:IntRectangle = TableResource.getTableViewPeopleRect(m_GameKind.wProcessType,
										m_TableState.wChairCount,
										i);
						if(rect)
						{
							var s:Shape = new Shape;
							s.graphics.beginFill(0xf0f0f0, 0.1);
							s.graphics.drawRect(0,0,rect.width,rect.height);
           					s.graphics.lineStyle(0.8, 0xf0f0f0);
            				s.graphics.moveTo(0,0);
							s.graphics.lineTo(rect.width,0);
							s.graphics.moveTo(rect.width,0);
							s.graphics.lineTo(rect.width,rect.height);
							s.graphics.moveTo(rect.width,rect.height);
							s.graphics.lineTo(0,rect.height);
							s.graphics.moveTo(0,rect.height);
							s.graphics.lineTo(0,0);
							
							s.graphics.endFill();
							
							m_UserFaceShape[i] = new  SimpleButton(s, s, s, s);
							
							m_UserFaceShape[i].x = rect.x;
							m_UserFaceShape[i].y = rect.y;
							m_UserFaceShape[i].width = rect.width;
							m_UserFaceShape[i].height= rect.height;
							m_UserFaceShape[i].alpha = 0;
							addChild(m_UserFaceShape[i]);
							m_UserFaceShape[i].useHandCursor= true;
							m_UserFaceShape[i].enabled= true;
							m_UserFaceShape[i].addEventListener(MouseEvent.MOUSE_OVER,
																OnEventUserFaceShapeMouseOver);
							m_UserFaceShape[i].addEventListener(MouseEvent.MOUSE_OUT,
																OnEventUserFaceShapeMouseOut);
							m_UserFaceShape[i].addEventListener(MouseEvent.MOUSE_DOWN,
																OnEventUserFaceShapeMouseDown);
						}
					}
				}
			}
		}
		public function DestroyTableView():void
		{
			if(m_UserFaceMC)
			{
				for(var i:uint = 0; i < m_UserFaceMC.length; i ++)
				{
					if(m_UserFaceMC[i] != null)
					{
						removeChild(m_UserFaceMC[i]);
						m_UserFaceMC[i] = null;
					}
				}
				m_UserFaceMC = null;
			}
			if(m_UserFaceShape)
			{
				for(var i:uint = 0; i < m_TableState.wChairCount; i ++)
				{
					m_UserFaceShape[i].removeEventListener(MouseEvent.MOUSE_OVER,
											OnEventUserFaceShapeMouseOver);
					m_UserFaceShape[i].removeEventListener(MouseEvent.MOUSE_OUT,
											OnEventUserFaceShapeMouseOut);
					m_UserFaceShape[i].removeEventListener(MouseEvent.MOUSE_DOWN,
																OnEventUserFaceShapeMouseDown);
					removeChild(m_UserFaceShape[i]);
					m_UserFaceShape[i] = null;
				}
				m_UserFaceShape = null;
			}
			if (m_tableMC)
			{
				removeChild(m_tableMC);
				m_tableMC = null;
			}
			if (m_labelTableID)
			{
				removeChild(m_labelTableID);
				m_labelTableID = null;
			}
			m_pITableFrameView = null;
			m_TableState = null;
			m_GameServer = null;
			m_pTableFrameView = null;
		}
		//设置用户信息
		public function SetUserInfo( wChairID:uint, pIUserItem:IUserItem):Boolean
		{
			if (wChairID>=m_TableState.pIUserItem.length)
			{
				return false;
			}
			if(pIUserItem != null)
			{
				if(m_TableState.pIUserItem[wChairID]!=null)
				{
					if(m_TableState.pIUserItem[wChairID].GetUserID() != 
							pIUserItem.GetUserID())
					{
						if(m_UserFaceMC[wChairID] != null)
						{
							removeChild(m_UserFaceMC[wChairID]);;
							m_UserFaceMC[wChairID] = null;
						}			
						showUserFace(wChairID,pIUserItem);
					}
				}
				else
				{
					showUserFace(wChairID,pIUserItem);
				}
			}
			else
			{
				if(m_UserFaceMC[wChairID] != null)
				{
					removeChild(m_UserFaceMC[wChairID]);
					m_UserFaceMC[wChairID] = null;
				}
			}
			m_TableState.pIUserItem[wChairID]=pIUserItem;
			m_pITableFrameView.UpdateTableView(m_TableState.wTableID,false);
			return true;

		}
		//设置鼠标按下
		public function SetMouseDown( bMouseDown:Boolean):void
		{
			if (m_bMouseDown!=bMouseDown)
			{
				m_bMouseDown=bMouseDown;
				m_pITableFrameView.UpdateTableView(m_TableState.wTableID,true);
			}
		}
		//设置焦点框架
		public function SetFocusFrame( bFocusFrame:Boolean):void
		{
			if (m_bFocusFrame!=bFocusFrame)
			{
				m_bFocusFrame=bFocusFrame;
				m_pITableFrameView.UpdateTableView(m_TableState.wTableID,false);
			}
		}
		//设置盘旋位置
		public function SetHoverChairID( wHoverChairID:uint):void
		{
			if (m_wHoverChairID!=wHoverChairID)
			{
				m_wHoverChairID=wHoverChairID;
				m_pITableFrameView.UpdateTableView(m_TableState.wTableID,true);
			}
		}
		//设置游戏标志 
		public function SetPlayFlag( bPlaying:Boolean):void
		{
			if (m_TableState.bPlaying!=bPlaying)
			{
				m_TableState.bPlaying=bPlaying;
				m_pITableFrameView.UpdateTableView(m_TableState.wTableID,false);
			}
		}
		//设置密码标志
		public function SetPassFlag( bPass:Boolean):void
		{
			if (m_TableState.bPass!=bPass)
			{
				m_TableState.bPass=bPass;
				m_pITableFrameView.UpdateTableView(m_TableState.wTableID,false);
			}
		}
		//设置备注
		public function SetTableNote(pszNote:String):void
		{
			m_TableState.szTableNote = String(pszNote);
			m_pITableFrameView.UpdateTableView(m_TableState.wTableID,true);
		}
		//获取用户信息
		public function GetUserInfo(wChairID:uint):IUserItem
		{
			if (wChairID>=m_TableState.pIUserItem.length)
			{
				return null;
			}
			return m_TableState.pIUserItem[wChairID];
		}
		//获取空椅子数
		public function GetNullChairCount(wNullChairID:uint):IntPoint
		{
			var wNullCount:uint=0;
			wNullChairID=INVALID_CHAIR;
			for (var i:uint=0; i<m_TableState.wChairCount; i++)
			{
				if (m_TableState.pIUserItem[i]==null)
				{
					++wNullCount;
					wNullChairID=i;
				}
			}
			return new IntPoint(wNullCount,wNullChairID);
		}
		//查询焦点框架
		public function QueryFocusFrame():Boolean
		{
			return m_bFocusFrame;
		}
		//查询盘旋位置
		public function QueryHoverChairID():uint
		{
			return m_wHoverChairID;
		}
		//查询游戏标志
		public function QueryPlayFlag():Boolean
		{
			return m_TableState.bPlaying;
		}
		//查询密码标志
		public function QueryPassFlag():Boolean
		{
			return m_TableState.bPass;
		}
		//获取桌子属性
		public function GetTableAttrib():tagTableState
		{
			return m_TableState;
		}
		//显示用户头像
		private function showUserFace( wChairID:uint, pIUserItem:IUserItem):void
		{
			var UserFace:Sprite = TableResource.getTableViewPeopleMC(m_GameKind.wProcessType,
										m_TableState.wChairCount,
										wChairID,
										pIUserItem.GetUserFaceID());
			if(UserFace != null)
			{
				addChild(UserFace);
				var rect:IntRectangle = TableResource.getTableViewPeopleRect(m_GameKind.wProcessType,
										m_TableState.wChairCount,
										wChairID);
				if(rect)
				{
					UserFace.x = rect.x;
					UserFace.y = rect.y;
					UserFace.width  = rect.width;
					UserFace.height = rect.height;
				}
				m_UserFaceMC[wChairID] = UserFace;
			}			
		}
		private function OnEventUserFaceShapeMouseOver(e:MouseEvent):void
		{
			trace("OnEventUserFaceShapeMouseOver" + e.target);
			if(e.target)
				e.target.alpha = 1.0;
		}
		private function OnEventUserFaceShapeMouseOut(e:MouseEvent):void
		{
			trace("OnEventUserFaceShapeMouseOver" + e.target);
			if(e.target)
				e.target.alpha = 0.0;
		}
		private function OnEventUserFaceShapeMouseDown(e:MouseEvent):void
		{
			for(var i:uint = 0; i < m_TableState.wChairCount; i ++)
			{
				if(m_UserFaceShape[i] == e.target)
				{
					m_pTableFrameView.SitChair(m_TableState.wTableID, i);
					break;
				}
			}
		}
	}
}
include "../../../Net/GLOBALDEF.as"
