package
{
	import flash.display.DisplayObject;
	
	import org.aswing.graphics.Graphics2D;
	import org.aswing.Icon;
	import org.aswing.Component;
	import org.aswing.plaf.UIResource;
	import org.aswing.error.ImpMissError;
	import org.aswing.util.HashMap;
	//游戏图标
	public class GameIcon implements Icon,UIResource
	{

		protected var m_image:DisplayObject;//图标显示对象
		protected var m_loaded:Boolean;//是否装载
		protected static  var m_mapIcons:HashMap;//图标对应表
		//构造函数
		public function GameIcon(image:DisplayObject)
		{
			m_image = image;
			if (m_image != null)
			{
				m_loaded = true;
			}
			else
			{
				m_loaded = false;
			}
		}
		//检测装载
		static protected function checkIconsLoad():void
		{
			if (m_mapIcons == null)
			{
				m_mapIcons = new HashMap;
	
				m_mapIcons.put(0, ICON0_VideoGameType);
				m_mapIcons.put(1, ICON1_BattleGameType);
				m_mapIcons.put(2, ICON2_PokieGameType);
				m_mapIcons.put(3, ICON3_LotteryGameType);
				
				m_mapIcons.put(11,ICON11_Hoe);
				m_mapIcons.put(12,ICON12_Land);
				m_mapIcons.put(13,ICON13_UncoverPig);
				m_mapIcons.put(14,ICON14_BlackJack);
				m_mapIcons.put(15,ICON15_UpGrade);
				m_mapIcons.put(16,ICON16_Sparrow);
				m_mapIcons.put(17,ICON17_ShowHand);
				m_mapIcons.put(18,ICON18_GoldFlower);
				
				m_mapIcons.put(21,ICON21_DragonTiger);
				m_mapIcons.put(22,ICON22_Baccarat);
				m_mapIcons.put(23,ICON23_Super6);
				m_mapIcons.put(24,ICON24_Sicbo);
				m_mapIcons.put(25,ICON25_Roulette);
				
				m_mapIcons.put(253,ICON253_NoRoom);
				m_mapIcons.put(254,ICON254_Room);
				m_mapIcons.put(255,ICON255_Plaza);				
			}	
		}
		//检测装载
		protected function checkLoad(c:Component):void
		{
			if (!m_loaded)
			{
				throw new ImpMissError  ;
			}
		}
		//更新图标
		public function updateIcon(c:Component, g:Graphics2D, x:int, y:int):void
		{
			checkLoad(c);
			if (m_image)
			{
				m_image.x = x;
				m_image.y = y;
			}
		}
		//获取图标高度
		public function getIconHeight(c:Component):int
		{
			checkLoad(c);
			return m_image?m_image.height:0;
		}
		//获取图标宽度
		public function getIconWidth(c:Component):int
		{
			checkLoad(c);
			return m_image?m_image.width:0;
		}
		//获取图标显示对象
		public function getDisplay(c:Component):DisplayObject
		{
			checkLoad(c);
			return m_image;
		}
		//装载图标
		static public function loadGameIcon(nbImageID:uint):GameIcon
		{
			checkIconsLoad();
			var o:Object = m_mapIcons.get(nbImageID);
			if(o != null)
			{
				var c:Class = o as Class;
				return new GameIcon(new c());
			}
			else
			{
				return null;
			}
		}
	}
}