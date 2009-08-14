package org.god.Common
{
	import flash.display.Stage;
	import flash.display.StageDisplayState;
	import flash.display.DisplayObject;
	//全频显示
	public class AppDisplayState
	{
		private static var m_mcMain:DisplayObject;//主影片
		private static var m_bEnableFullScreen:Boolean;//是否全屏
		//构造函数
		public function AppDisplayState()
		{
		}
		//设置信息
		static public function setMain(mcMain:DisplayObject, bEnableFullScreen:Boolean = true):void
		{
			m_mcMain = mcMain;
			m_bEnableFullScreen = bEnableFullScreen;
		}
		//切换全屏
		static public function toggleFullScreen():void
		{
			if(m_bEnableFullScreen == false)
				return;
			try
			{
				switch (m_mcMain.stage.displayState)
				{
					case StageDisplayState.NORMAL:
						{
							//全屏显示
							if(DEBUG == 0)
								m_mcMain.stage.displayState = StageDisplayState.FULL_SCREEN;
						}
						break;
					case StageDisplayState.FULL_SCREEN:
					default :
						//普通显示
						m_mcMain.stage.displayState = StageDisplayState.NORMAL;
						break;
				}
			}
			catch (e:Error)
			{

				trace(e);
			}
		}
		//判断全屏
		static public function IsFullScreen():Boolean
		{
			return m_mcMain.stage.displayState == StageDisplayState.FULL_SCREEN;
		}
		//强制全屏或者普通显示方式
		static public function FocusFullScreen(bFullScreen:Boolean):void
		{
			if(m_bEnableFullScreen == false)
				return;
			try
			{
				if (bFullScreen)
				{
					//强制全屏显示
					if (m_mcMain.stage.displayState != StageDisplayState.FULL_SCREEN)
					{
						if(DEBUG == 0)
							m_mcMain.stage.displayState = StageDisplayState.FULL_SCREEN;
					}
				}
				else
				{
					//强制普通显示
					if (m_mcMain.stage.displayState != StageDisplayState.NORMAL)
					{
						m_mcMain.stage.displayState = StageDisplayState.NORMAL;
					}
				}
			}
			catch (e:Error)
			{

				trace(e);
			}
		}
	}
}
include "../GlobalConst.as"