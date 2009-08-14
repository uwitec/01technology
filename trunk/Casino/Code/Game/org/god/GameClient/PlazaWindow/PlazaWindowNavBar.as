package
{
	import flash.display.MovieClip;
	import flash.utils.Timer;
	import flash.events.MouseEvent;
	import flash.events.TimerEvent;
 
	import org.god.IGameFrame.IPlazaWindowNavBar;
	import org.god.IGameFrame.IMain;
	import org.god.IGameFrame.GlobalMain;
	import org.god.IGameFrame.IPaneImpl;
	import org.god.Common.AppDisplayState;
	import org.god.Common.Media.SoundManager;
	import org.god.SkinControl.NavButton;
	
	import org.aswing.Container;
	import org.aswing.Icon;
	//大厅窗口导航条
	public class PlazaWindowNavBar extends MovieClip implements IPlazaWindowNavBar
	{
		private static const nHideStatusHeight:int = 8;
		private static const nShowStatusHeight:int = 50;
		private static const nBarWidth:int = 642;
		
		private var m_mcParent:PlazaWindow;//大厅窗口
		private var m_arrBtnGame:Array;//导航按钮数组
		private var m_nHideStatusXPos:int;//隐藏导航条时X坐标
		private var m_nHideStatusYPos:int;//隐藏导航条时Y坐标
		private var m_nShowStatusXPos:int;//显示导航条时X坐标
		private var m_nShowStatusYPos:int;//显示导航条时Y坐标
		private var m_nMotionShowDistance:Number;//移动显示距离
		private var m_timer:Timer;//定时器
		private var m_nDirectShow:Boolean;//显示方向
		//构造函数
		public function PlazaWindowNavBar(mcParent:PlazaWindow,
									nMaxPaneCount:int)
		{
			super();
			Init();
			
			m_mcParent	 = mcParent;
			m_arrBtnGame  = new Array(nMaxPaneCount);
			
			const nXPos:int = 40;
			const nYPos:int = 12;
			for (var i:int = 0; i < m_arrBtnGame.length; i ++)
			{
				var btn:NavButton = new NavButton(IDS_NOGAMEROOM, nXPos + 96 * i, nYPos);
				btn.setEnabled(false);
				btn.setIcon(GameIcon.loadGameIcon(253));
				m_arrBtnGame[i] = btn;
				addChild(btn);
				btn.addEventListener(MouseEvent.CLICK, OnEventNavPane);
			}
			m_btnFullScreen.addEventListener(MouseEvent.CLICK, OnEventFullScreen);
			m_btnDisplayNormal.addEventListener(MouseEvent.CLICK, OnEventDisplayNormal);
			m_btnQuiet.addEventListener(MouseEvent.CLICK, OnEventQuiet);
			m_btnSound.addEventListener(MouseEvent.CLICK, OnEventSound);
			
			m_btnDisplayNormal.visible = false;
			m_btnSound.visible = false;
			
			addEventListener(MouseEvent.ROLL_OVER, OnEventMouseRollOver);
			addEventListener(MouseEvent.ROLL_OUT, OnEventMouseRollOut);
			
			m_timer = new Timer(100);
			m_timer.addEventListener("timer", OnEventTimerHandler);
			
			this.x = m_nHideStatusXPos;
			this.y = m_nHideStatusYPos;
			//this.width = nBarWidth;
			//this.height = nShowStatusHeight;
			this.alpha = 1.0;
			m_mcParent.addChild(this);
			
		}
		//初始化
		private function Init():void
		{
			m_nHideStatusXPos = 0;
			m_nShowStatusXPos = 0;
			var main:IMain = GlobalMain.getMain();
			m_nHideStatusYPos = main.getStageHeight() - nHideStatusHeight;			
			m_nShowStatusYPos = main.getStageHeight() - nShowStatusHeight;
			m_nMotionShowDistance = (Number(m_nHideStatusYPos) - Number(m_nShowStatusYPos)) / 5;
		}
		
		////////////////////////////////////
		//导航按钮事件
		private function OnEventNavPane(e:MouseEvent):void
		{
			var nb:NavButton = e.currentTarget as NavButton;
			if (nb)
			{
				var pane:IPaneImpl = nb.GetBindData() as IPaneImpl;
				m_mcParent.activePane(pane);
			}
		}
		//全屏按钮事件
		private function OnEventFullScreen(e:MouseEvent):void
		{
			AppDisplayState.toggleFullScreen();
		}
		//正常显示按钮事件
		private function OnEventDisplayNormal(e:MouseEvent):void
		{
			AppDisplayState.toggleFullScreen();
		}
		//静音按钮事件
		private function OnEventQuiet(e:MouseEvent):void
		{
			SoundManager.GetGlobalSoundManager().SetQuiet(true);
		}
		//打开声音按钮事件
		private function OnEventSound(e:MouseEvent):void
		{
			SoundManager.GetGlobalSoundManager().SetQuiet(false);
		}
		//////////////////////////////
		//获取导航按钮数目
		public function getNavBtnCount():int
		{
			return m_arrBtnGame.length;
		}
		//设置导航按钮标题
		public function setBtnText(nIndex:int,strText:String):void
		{
			m_arrBtnGame[nIndex].setText(strText);
		}
		//获取导航按钮标题
		public function getBtnText(nIndex:int):String
		{
			return m_arrBtnGame[nIndex].getText();
		}
		//设置导航按钮图标
		public function setBtnIcon(nIndex:int,icon:Icon):void
		{
			m_arrBtnGame[nIndex].setIcon(icon);
		}
		//获取导航按钮图标
		public function getBtnIcon(nIndex:int):Icon
		{
			return m_arrBtnGame[nIndex].getIcon();
		}
		//设置导航按钮绑定数据
		public function setBtnBindData(nIndex:int,data:*):void
		{
			m_arrBtnGame[nIndex].SetBindData(data);
		}
		//获取导航按钮绑定数据
		public function getBtnBindData(nIndex:int):*
		{
			return m_arrBtnGame[nIndex].GetBindData();
		}
		//活动导航按钮
		public function ActiveBtn(nIndex:int):void
		{
			m_arrBtnGame[nIndex].Active();
		}
		//取消活动导航按钮
		public function DeactiveBtn(nIndex:int):void
		{
			m_arrBtnGame[nIndex].Deactive();
		}
		//切换全频事件
		public function OnEventToggleFullScreen(b:Boolean):void
		{
			if(b)
			{
				m_btnFullScreen.visible = false;
				m_btnDisplayNormal.visible = true;
			}
			else
			{
				m_btnFullScreen.visible = true;
				m_btnDisplayNormal.visible = false;
			}
		}
		//切换静音事件
		public function OnEventToggleSound(b:Boolean):void
		{
			if(b)
			{
				m_btnQuiet.visible = false;
				m_btnSound.visible = true;
			}
			else
			{
				m_btnQuiet.visible = true;
				m_btnSound.visible = false;
			}
		}
		///////////////////////
		//定时器事件
		private function OnEventTimerHandler(e:TimerEvent):void
		{
			if(m_nDirectShow)
			{
				if(this.y <= m_nShowStatusYPos)
				{
					this.y = m_nShowStatusYPos;
					m_timer.stop();
				}
				else
				{
					this.y -= m_nMotionShowDistance;
				}
			}
			else
			{
				if(this.y >= m_nHideStatusYPos)
				{
					this.y = m_nHideStatusYPos;
					m_timer.stop();
					this.alpha = 1.0;
				}
				else
				{
					this.y += m_nMotionShowDistance;
				}
			}
		}
		//开始显示导航条
		private function StartMotionShow():void
		{
			this.alpha = 0.80;
			m_nDirectShow = true;
			if(m_timer.running == false)
			{
				m_timer.start();
			}
		}
		//开始隐藏导航条
		private function StartMotionHide():void
		{
			m_nDirectShow = false;
			if(m_timer.running == false)
			{
				m_timer.start();
			}
		}
		//鼠标进入事件
		private function OnEventMouseRollOver(e:MouseEvent):void
		{
			//trace("mouseRollOver");
			StartMotionShow();
			//TransitionManager.start(this, {type:Fly,direction:Transition.IN,duration:3,easing:Elastic.easeOut, startPoint:7}); 
		}
		//鼠标离开事件
		private function OnEventMouseRollOut(e:MouseEvent):void
		{
			//trace("mouseRollOut");
			StartMotionHide();
			/*TransitionManager.start(this, 
			{type:Fly,
			direction:Transition.OUT,
			duration:3,
			easing:Elastic.easeOut, 
			startPoint:7});*/ 
		}
	}
}
include "../../StringTableDef.as"
