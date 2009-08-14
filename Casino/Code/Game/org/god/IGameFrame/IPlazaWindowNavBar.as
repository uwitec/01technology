package org.god.IGameFrame
{
	import org.aswing.Icon;
	//大厅窗口导航条接口
	public interface IPlazaWindowNavBar
	{
		//获取导航按钮数目
		function getNavBtnCount():int;
		//设置导航按钮标题
		function setBtnText(nIndex:int,strText:String):void;
		//获取导航按钮标题
		function getBtnText(nIndex:int):String;
		//设置导航按钮图标
		function setBtnIcon(nIndex:int,icon:Icon):void;
		//获取导航按钮图标
		function getBtnIcon(nIndex:int):Icon;
		//设置导航按钮数据
		function setBtnBindData(nIndex:int,data:*):void;
		//获取导航按钮数据
		function getBtnBindData(nIndex:int):*;
		//活动导航按钮
		function ActiveBtn(nIndex:int):void;
		//取消活动导航按钮
		function DeactiveBtn(nIndex:int):void;
		//切换全频事件
		function OnEventToggleFullScreen(b:Boolean):void;
		//切换静音事件
		function OnEventToggleSound(b:Boolean):void;
	}
}