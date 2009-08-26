﻿package 
{
	import flash.display.MovieClip;
	import flash.system.*;
	import flash.events.Event;
	
	import org.god.IGameFrame.IGameClass;
	import org.god.IGameFrame.IGameModule;
	
	public class ModuleMain extends MovieClip implements IGameModule
	{
		//构造函数
		public function ModuleMain()
		{
			Security.allowDomain("*");
			System.useCodePage=true;
			//Security.loadPolicyFile("http://"+strWebMainServer+"/crossdomain.xml");
		}
		//获取游戏类接口
		public function getIGameClass():IGameClass
		{
			var game:GameClass;
			var igame:IGameClass;
			try
			{
				game = new GameClass;
				igame = game.QueryIGameClass();
				return igame;
			}
			catch(e:Event)
			{
				System.setClipboard(e.toString());
			}
			game = null;
			igame = null;
			return null;
		}
	}
}
include "../../GlobalConst.as"