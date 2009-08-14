package org.god.Net
{
	public interface IUnknownEx
	{

		//是否有效
		function IsValid():Boolean;
		//释放对象
		function Release():Boolean;
		//接口查询
		function QueryInterface(Guid:String,dwQueryVer:uint):*;
	}
}
