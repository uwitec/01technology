package 
{
	import flash.display.Sprite;
	import flash.system.System;
	import flash.system.Security;
	import flash.events.Event;
	
	import org.god.IGameFrame.IGameClass;
	import org.god.IGameFrame.IGameModule;
	
	public class ModuleMain extends Sprite implements IGameModule
	{
		public function ModuleMain()
		{
			Security.allowDomain("*");
			System.useCodePage=true;
		}
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
