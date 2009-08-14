package 
{
	import org.aswing.Component;
	import org.aswing.geom.IntRectangle;
	import org.aswing.geom.IntPoint;
	
	import org.god.Net.tagGameType;
	import org.god.Net.tagGameKind;
	import org.god.Net.tagGameServer;
	import org.god.IGameFrame.tagTableState;
	import org.god.IGameFrame.ITableFrameView;
	import org.god.IGameFrame.ITableFrameSink;
	import org.god.IGameFrame.ITableView;
	import org.god.IGameFrame.IServerItem;
	import org.god.IGameFrame.IUserItem;
	//游戏桌视图
	public class TableFrameView extends Component implements ITableFrameView
	{
		public static const rectDefaultPosition:IntRectangle = new IntRectangle(8, 182,1006, 564);
		public static const nDefaultTableBorder:uint = 6;
		
		private var m_bShowUser:Boolean;//是否显示
		private var m_wTableCount:uint;//游戏桌数
		private var m_wChairCount:uint;//椅子数目
		private var m_pTableViewArray:Array;
		private var m_pITableFrameSink:ITableFrameSink;
		private var m_pGameServer:IServerItem;
		private var m_pGameKind:IServerItem;
		private var m_nTableColCount:uint;
		private var m_pGamePane:GamePane;
		
		public function TableFrameView(pGamePane:GamePane)
		{
			super();
			m_pGamePane = pGamePane;
		}
		//配置函数
		public function InitTableFrame(wTableCount:uint,  
		   wPlayerCount:uint, 
		   pGameServer:IServerItem, 
		   pGameKind:IServerItem,
		   pIUnknownEx:ITableFrameSink):Boolean
		{
			m_wTableCount = wTableCount;
			m_wChairCount = wPlayerCount;
			m_pGameServer = pGameServer;
			m_pGameKind = pGameKind;
			m_pITableFrameSink = pIUnknownEx;
						
			m_pTableViewArray = new Array(m_wTableCount);
			for (var i:int = 0; i < m_wTableCount; i ++)
			{
				m_pTableViewArray[i] = new TableView(this);
				m_pTableViewArray[i].InitTableView(pGameServer,m_pGameKind,i, m_wChairCount,this);
				addChild(m_pTableViewArray[i]);
				
			}

			doLayout(rectDefaultPosition.clone());
			return true;
		}
		
		//销毁函数
		public function DestroyTableFrame():void
		{
			for (var i:int = 0; i < m_wTableCount; i ++)
			{
				m_pTableViewArray[i].DestroyTableView();
				removeChild(m_pTableViewArray[i]);
				m_pTableViewArray[i] = null;
			}
			m_pGameServer 	 = null;
			m_pGameKind		 = null;
			m_pITableFrameSink= null;
			m_pTableViewArray = null;
			m_pGamePane = null;
		}
		//更新桌子
		public function UpdateTableView( wTableID:uint,  bMustUpdate:Boolean):void
		{
		}
		//获取游戏桌指针
		public function GetTableArrayPtr( wTableID:uint):ITableView
		{
			if ((m_pTableViewArray==null)||(wTableID>=m_wTableCount))
			{
				return null;
			}
			return m_pTableViewArray[wTableID];
		}
		//设置显示
		public function ShowUserInfo( bShowUser:Boolean):Boolean
		{
			m_bShowUser = bShowUser;
			return true;
		}
		//设置视图桌子
		public function EnsureVisibleTable( wTableID:uint):Boolean
		{
			return true;
		}
		//闪动椅子
		public function FlashTable( wTableID:uint):Boolean
		{
			return true;
		}
		//闪动椅子
		public function FlashChair( wTableID:uint,  wChairID:uint):Boolean
		{
			return true;
		}
		//坐下
		public function SitChair(wTableID:uint,  wChairID:uint):Boolean
		{
			if(m_pGamePane)
				m_pGamePane.SitChair(wTableID, wChairID);
				
			return true;
		}
		//获取桌子数目
		public function GetTableCount():uint
		{
			return m_wTableCount;
		}
		//获取椅子数目
		public function GetChairCount():uint
		{
			return m_wChairCount;
		}
		//设置用户信息
		public function SetUserInfo( wTableID:uint,  wChairID:uint,  pIUserItem:IUserItem):Boolean
		{
			var pITableView:ITableView=GetTableArrayPtr(wTableID);

			if (pITableView==null)
			{
				return false;
			}
			return pITableView.SetUserInfo(wChairID,pIUserItem);

		}
		//设置游戏标志 
		public function SetPlayFlag( wTableID:uint,  bPlaying:Boolean):void
		{
			var pITableView:ITableView=GetTableArrayPtr(wTableID);

			if (pITableView!=null)
			{
				pITableView.SetPlayFlag(bPlaying);
			}
		}
		//设置密码标志
		public function SetPassFlag( wTableID:uint,  bPass:Boolean):void
		{
			var pITableView:ITableView=GetTableArrayPtr(wTableID);

			if (pITableView!=null)
			{
				pITableView.SetPassFlag(bPass);
			}

		}
		//获取用户信息
		public function GetUserInfo( wTableID:uint,  wChairID:uint):IUserItem
		{
			var pITableView:ITableView=GetTableArrayPtr(wTableID);

			if (pITableView==null)
			{
				return null;
			}
			return pITableView.GetUserInfo(wChairID);

		}
		//获取空椅子数
		public function GetNullChairCount( wTableID:uint,  wNullChairID:uint):IntPoint
		{
			var pITableView:ITableView=GetTableArrayPtr(wTableID);

			if (pITableView==null)
			{
				return new IntPoint(-1,-1);
			}
			return pITableView.GetNullChairCount(wNullChairID);
		}
		//查询游戏标志
		public function QueryPlayFlag( wTableID:uint):Boolean
		{
			var pITableView:ITableView=GetTableArrayPtr(wTableID);

			if (pITableView==null)
			{
				return false;
			}
			return pITableView.QueryPlayFlag();
		}
		//查询密码标志
		public function QueryPassFlag( wTableID:uint):Boolean
		{
			var pITableView:ITableView=GetTableArrayPtr(wTableID);

			if (pITableView==null)
			{
				return false;
			}
			return pITableView.QueryPassFlag();
		}
		//获取桌子属性
		public function GetTableAttrib( wTableID:uint):tagTableState
		{
			var pITableView:ITableView=GetTableArrayPtr(wTableID);

			if (pITableView==null)
			{
				return null;
			}
			return pITableView.GetTableAttrib();
		}
		private function doLayout(b:IntRectangle):Boolean
		{
			var GameServer:tagGameServer = m_pGameServer.getItemData();
			if(GameServer == null)
			{
				return false;
			}
			var GameKind:tagGameKind = m_pGameKind.getItemData();
			if(GameKind == null)
			{
				return false;
			}
			var rectTable:IntRectangle = TableResource.getTableViewRect(GameKind.wProcessType,
													m_wChairCount);
			m_nTableColCount = b.width / rectTable.width;
			var nXDistance:int = b.width / (m_nTableColCount * 2);

			var h:int = Math.ceil(Number(m_pTableViewArray.length) / Number(m_nTableColCount)) * (nDefaultTableBorder + rectTable.height) ;
			this.width = (rectTable.width)* m_nTableColCount;
			this.height = h;
			
			for(var i:int = 0; i < m_pTableViewArray.length; i ++)
			{
				var table:TableView = m_pTableViewArray[i] as TableView;
				if(table != null)
				{
					var row:int = i / m_nTableColCount;
					var col:int = i % m_nTableColCount;
					
					var xp:int = b.x + (col * 2 + 1) * nXDistance - rectTable.width/2;
					var yp:int = b.y + (row + 1) * (nDefaultTableBorder + rectTable.height) - rectTable.height;
					
					table.x = xp;
					table.y = yp;
					table.width = rectTable.width;
					table.height= rectTable.height;
				}
			}

			return true;
		}		
		override protected function paint(b:IntRectangle):void
		{
			super.paint(b);
			var cb:IntRectangle = b.clone();

			doLayout(cb);
		}
	}
}