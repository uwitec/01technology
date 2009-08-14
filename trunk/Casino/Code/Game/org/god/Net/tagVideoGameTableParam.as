package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	//视频游戏桌参数结构
	public class tagVideoGameTableParam
	{
		public var fMaxUserRevenueSharing:Number = new Number;
		public var wStateKernelServerPort:uint;
		public var dwStateKernelServerAddr:uint;
		public function tagVideoGameTableParam()
		{

		}
	}
}
include "GLOBALDEF.as"
include "../Common/Memory.as"
