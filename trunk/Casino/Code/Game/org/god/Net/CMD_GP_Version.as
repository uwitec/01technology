package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	//版本通知
	public class CMD_GP_Version
	{
		public var bNewVersion:uint;//更新版本
		public var bAllowConnect:uint;//允许连接
		public function CMD_GP_Version()
		{
		}
	}
}