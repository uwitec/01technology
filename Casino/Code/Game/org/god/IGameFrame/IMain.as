package org.god.IGameFrame
{
	import flash.display.DisplayObjectContainer;
	import flash.display.DisplayObject;
	import flash.utils.ByteArray;

	import org.god.Net.tagUserData;
	import org.god.Net.CMD_GP_GameInfo;
	import org.god.Net.CMD_GP_Notice;
	import org.god.Net.IClientSocket;
	//主类接口
	public interface IMain
	{
		function getILogonWindow():ILogonWindow;//获取登陆窗口接口
		function getIPlazaWindow():IPlazaWindow;//获取大厅窗口接口
		function getStageWidth():int;//获取舞台宽度
		function getStageHeight():int;//获取舞台高度
		
		function SwitchMainViewType(nDM:int):void;//切换显示类型
		function getActiveDOC():DisplayObjectContainer;//获取当前活动显示容器
		function ShowStatusMessage(strMsg:String,bModal:Boolean=true):void;//显示状态消息框
		function HideStatusMessage():void;//隐藏状态消息框
		function ShowMessageBox(strMsg:String,bSplitMsg:Boolean = true,bModal:Boolean=true):void;//显示消息框
		function ShowErrorMessageBox(wErrCode:uint,bSplitMsg:Boolean = true,bModal:Boolean=true):void;//显示错误消息框
		function addChildDO(d:DisplayObject,
								   bModal:Boolean):void;//添加子显示对象
		function removeChildDO(d:DisplayObject):void;//删除子显示对象
		
		function resetAll():void;//复位
		
		function isLogonPlaza():Boolean;//是否登陆大厅
		function setLogonPlazaOK(bOK:Boolean):void;//设置登陆成功
		function setUserData(UserData:*):void;//设置用户信息
		function getUserData():tagUserData;//获取用户信息
		function setUserType(wUserType:uint):void;//设置用户类型
		function getUserType():uint;//获取用户类型
		function setRealUserID(wUserType:uint):void;//设置用户真实id
		function getRealUserID():uint;//获取用户真实id
		function setUserAccount(strAccount:String):void;//设置用户帐号
		function getUserAccount():String;//获取用户帐号
		function setUserPasswd(strPasswd:String):void;//设置用户密码
		function getUserPasswd():String;//获取用户密码
		function setGameInfo(GameInfo:CMD_GP_GameInfo):void;//设置游戏信息
		function getGameInfo():CMD_GP_GameInfo;//获取游戏信息
		function setNotice(Notice:CMD_GP_Notice):void;//设置公告
		function getNotice():CMD_GP_Notice;//获取公告
		
		function getClientSocket():IClientSocket;//获取网络接口
		function OnSocketRead(wMainCmdID:uint,wSubCmdID:uint,pBuffer:ByteArray,wDataSize:int,pIClientSocket:IClientSocket):Boolean;//处理网络消息
	
		function addUserDataChangeEventListener(listener:Function):void;//添加用户消息改变事件监听
		function removeUserDataChangeEventListener(listener:Function):void;//删除用户消息改变事件监听
		
		function GetErrorDescribe(wErrCode:uint):String;//获取错误信息描述
		function getDefClass(strClassName:String,strModuleName:String):Class;//获取定义类
	}
}