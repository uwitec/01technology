package
{
	import flash.display.Sprite;
	import flash.display.Bitmap;
	import flash.display.BitmapData;
	import flash.display.DisplayObjectContainer;
	import flash.events.MouseEvent;
	
	import org.god.IGameFrame.IMain;
	import org.god.IGameFrame.IRuleWindow;
	import org.god.SkinControl.ImageButton;
	//用户协议窗口
	public class RuleWindow extends Sprite implements IRuleWindow
	{
		protected var m_IMain:IMain;//主类接口
		protected var m_btAgree:ImageButton;//同意按钮
		protected var m_btReject:ImageButton;//拒绝按钮
		//构造函数
		public function RuleWindow(mcParent:IMain)
		{
			super();
			m_IMain = mcParent;
			m_btAgree=new ImageButton(IMAGE_BT_AGREE,425,37);
			m_btAgree.addEventListener(MouseEvent.CLICK,OnEventAgree);
			addChild(m_btAgree);	
			m_btReject=new ImageButton(IMAGE_BT_REJECT,425,37);
			m_btReject.addEventListener(MouseEvent.CLICK,OnEventReject);
			addChild(m_btReject);
			m_btAgree.x = 360;
			m_btAgree.y = 690;
			m_btReject.x = 578;
			m_btReject.y = 690;
		}
		//获取主类接口
		public function getIMain():IMain
		{
			return m_IMain;
		}
		//显示
		public function Show():void
		{
			this.visible = true;
		}
		//隐藏
		public function Hide():void
		{
			this.visible = false;
		}
		//获取活动显示容器
		public function getActiveDOC():DisplayObjectContainer
		{
			return this;
		}
		//复位
		public function resetAll():void
		{
		}
		//同意按钮事件
		protected function OnEventAgree(e:MouseEvent):void
		{
			if(m_IMain)
			{
				m_IMain.SwitchMainViewType(nMainViewType_PlazaWindow);
			}
		}
		//拒绝按钮事件
		protected function OnEventReject(e:MouseEvent):void
		{
			if(m_IMain)
			{
				m_IMain.resetAll();
				m_IMain.SwitchMainViewType(nMainViewType_LogonWindow);
			}
		}
	}
}
include "../../GlobalConst.as"
