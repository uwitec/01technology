package 
{

	import org.aswing.Component;
	import org.aswing.geom.IntRectangle;
	import org.aswing.graphics.Pen;
	import org.aswing.graphics.SolidBrush;
	import org.aswing.graphics.Graphics2D;
	import org.aswing.ASColor;

	import org.god.Net.tagGameType;
	import org.god.Net.tagGameKind;
	import org.god.Net.tagGameServer;
	//服务类型标签容器
	public class ServerTypeTabContainer extends Component
	{
		private var m_mcParent:ServerItemTabPane;//服务项分页面板
		private var m_Tab:ServerTypeTab;//服务类型标签
		//创建首页标签
		static public function CreateServerTypeTabContainer_RootWeb(mcParent:ServerItemTabPane):ServerTypeTabContainer
		{
			var tab:ServerTypeTab = ServerTypeTab.CreateServerTypeTab_RootWeb(mcParent);
			return new ServerTypeTabContainer(tab,mcParent);
		}
		//创建现场标签
		static public function CreateServerTypeTabContainer_CasinoField(mcParent:ServerItemTabPane):ServerTypeTabContainer
		{
			var tab:ServerTypeTab = ServerTypeTab.CreateServerTypeTab_CasinoField(mcParent);
			return new ServerTypeTabContainer(tab,mcParent);
		}
		//创建游戏类型标签
		static public function CreateServerTypeTabContainer_GameKindGrid(mcParent:ServerItemTabPane,GameType:tagGameType):ServerTypeTabContainer
		{
			var tab:ServerTypeTab = ServerTypeTab.CreateServerTypeTab_GameKindGrid(mcParent,GameType);
			return new ServerTypeTabContainer(tab,mcParent);
		}
		//构造函数
		public function ServerTypeTabContainer(tab:ServerTypeTab,mcParent:ServerItemTabPane)
		{
			super();
			m_mcParent = mcParent;
			m_Tab = tab;
			addChild(m_Tab);
		}
		//销毁
		public function Destroy():void
		{
			if (m_Tab)
			{
				m_Tab.Destroy();
				removeChild(m_Tab);
				m_Tab = null;
			}
			m_mcParent  = null;
		}
		//完成登陆事件
		public function OnEventFinishLogon():void
		{
			if (m_Tab)
				m_Tab.OnEventFinishLogon();
		}
		//重画
		override protected function paint(b:IntRectangle):void
		{
			super.paint(b);
			var cb:IntRectangle = b.clone();

			if (m_Tab)
			{
				m_Tab.setComBounds(cb);
				m_Tab.doLayout();
			}
		}
		//获取标签
		public function getTab():ServerTypeTab
		{
			return m_Tab;
		}
	}
}
include "../../StringTableDef.as"
include "../../GlobalConst.as"