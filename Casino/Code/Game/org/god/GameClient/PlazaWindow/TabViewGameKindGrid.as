package 
{
	import org.god.Net.tagGameType;
	import org.god.Net.tagGameKind;
	import org.god.Net.tagGameServer;
	import org.god.Common.ServerItem;
	
	import org.aswing.Component;
	import org.aswing.geom.IntRectangle;
	//游戏类型标签
	public class TabViewGameKindGrid extends Component
	{
		private static const nCellColCount:int  = 3;
		private static const nDefaultCellBorder:int = 5;
		
		private var m_mcParent:ServerTypeTab;//父亲服务类型页
		private var m_GameItem:ServerItem;//GameType
		private var m_arrGameKindCell:Array;//GameKind
		//创建
		static public function CreateTabViewGameKindGrid(GameItem:ServerItem,
		mcParent:ServerTypeTab)
		{
			return new TabViewGameKindGrid(GameItem,mcParent);
		}
		//构造函数
		public function TabViewGameKindGrid(GameItem:ServerItem,mcParent:ServerTypeTab)
		{
			super();
			m_mcParent = mcParent;
			m_GameItem = GameItem;
			m_arrGameKindCell = new Array;
		}
		//销毁
		public function Destroy():void
		{
			for(var i:int = 0; i < m_arrGameKindCell.length; i ++)
			{
				var cell:GameKindGridCell = m_arrGameKindCell[i] as GameKindGridCell;
				if(cell != null)
				{
					cell.Destroy();
				}
				 m_arrGameKindCell[i] = null;
			}
			m_arrGameKindCell = null;
			m_mcParent = null;
			m_GameItem = null;
		}
		//完成登陆事件
		public function OnEventFinishLogon():void
		{
			for(var i:int = 0; i < m_arrGameKindCell.length; i ++)
			{
				var cell:GameKindGridCell = m_arrGameKindCell[i] as GameKindGridCell;
				if(cell != null)
				{
					cell.OnEventFinishLogon();
				}
			}
		}
		//更新游戏在线人数
		public function UpdateGameKindOnLine(wKindID:uint,dwOnLineCount:uint):void
		{
			var cell:GameKindGridCell = findKindItem(wKindID);
			if(cell == null)
			{
				return ;
			}
			cell.UpdateGameKindOnLine(dwOnLineCount);
		}
		//重画
		override protected function paint(b:IntRectangle):void
		{
			super.paint(b);
			var cb:IntRectangle = b.clone();
			try
			{
				doLayout(cb);
			}
			catch(e:Error)
			{
			}
		}
		//设置位置
		override public function setComBounds(b:IntRectangle):void
		{
			var h:int =Math.ceil(Number(m_arrGameKindCell.length) / Number(nCellColCount)) * (nDefaultCellBorder + GameKindGridCell.nDefaultHeight) ;

			if(b.height < h)
			{
				b.width -= 18;
				b.height = h;
			}
			super.setComBounds(b);

		}
		//插入游戏类型项
		public function insertKindItem(item:tagGameKind):Boolean
		{
			if(findKindItem(item.wKindID) != null)
			{
				return false;
			}
			var itemKind:ServerItem = new ServerItem(item, SUB_GP_LIST_KIND);
			var cell:GameKindGridCell = GameKindGridCell.CreateGameKindGridCell(itemKind, this);
			m_arrGameKindCell.push(cell);
			addChild(cell);

			var h:int = Math.ceil(Number(m_arrGameKindCell.length) / Number(nCellColCount)) * (nDefaultCellBorder + GameKindGridCell.nDefaultHeight) ;
			if(this.height < h)
			{
				this.width -= 18;
				this.height = h;
			}
			
			return true;
		}
		//插入游戏服务项
		public function insertServerItem(item:tagGameServer):Boolean
		{
			var cell:GameKindGridCell = findKindItem(item.wKindID);
			if(cell == null)
			{
				return false;
			}
			return cell.insertServerItem(item);
		}
		//查找游戏类型项
		public function findKindItem(wKindID:uint):GameKindGridCell
		{
			for(var i:int = 0; i < m_arrGameKindCell.length; i ++)
			{
				var cell:GameKindGridCell = m_arrGameKindCell[i] as GameKindGridCell;
				if(cell != null)
				{
					var item:ServerItem = cell.getServerItem();
					if(item != null && item.getItemData() != null)
					{
						var GameKind:tagGameKind = item.getItemData();
						if(GameKind.wKindID == wKindID)
						{
							return cell;
						}
					}
				}
			}
			return null;
		}
		//获取游戏种类项
		public function getGameTypeItem():ServerItem
		{
			return m_GameItem;
		}
		//布局
		private function doLayout(b:IntRectangle):Boolean
		{
	
			var nXDistance:int = b.width / (nCellColCount * 2);
			
			for(var i:int = 0; i < m_arrGameKindCell.length; i ++)
			{
				var cell:GameKindGridCell = m_arrGameKindCell[i] as GameKindGridCell;
				if(cell != null)
				{
					var row:int = i / nCellColCount;
					var col:int = i % nCellColCount;
					
					var xp:int = b.x + (col * 2 + 1) * nXDistance - GameKindGridCell.nDefaultWidth/2;
					var yp:int = b.y + (row + 1) * (nDefaultCellBorder + GameKindGridCell.nDefaultHeight) - GameKindGridCell.nDefaultHeight;
					
					cell.setComBounds(new IntRectangle(xp, yp,GameKindGridCell.nDefaultWidth, GameKindGridCell.nDefaultHeight));
				}
			}
			return true;
		}
	}
}
include "../../StringTableDef.as"
include "../../GlobalConst.as"
include "../../Net/GLOBALDEF.as"
include "../../Net/CMD_Plaza.as"
