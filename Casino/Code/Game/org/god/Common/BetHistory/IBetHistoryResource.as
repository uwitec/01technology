package org.god.Common.BetHistory
{
	import org.god.SkinControl.SkinImage;
	//游戏记录资源接口
	public interface IBetHistoryResource
	{
		function GetGameEndViewImage(wProcessType:uint):SkinImage;
		function GetResource(strResName:String):SkinImage;
		function GetResourcePackage(strResName:String):*;
	}
}