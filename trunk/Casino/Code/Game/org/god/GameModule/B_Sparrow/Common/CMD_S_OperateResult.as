package org.god.GameModule.B_Sparrow.Common
{
	import flash.utils.ByteArray;
	//操作命令
	public class CMD_S_OperateResult
	{
		public static const sizeof_CMD_S_OperateResult:uint = 2 + 2 + 1 + 1 ;
		
		public var wOperateUser:uint;//操作用户
		public var wProvideUser:uint;//供应用户
		public var cbOperateCode:uint;//操作代码
		public var cbOperateSparrow:uint;//操作扑克

		public function CMD_S_OperateResult()
		{
		}
		public static function readData(data:ByteArray):CMD_S_OperateResult
		{
			var pos:int=data.position;
			var result:CMD_S_OperateResult=new CMD_S_OperateResult;
			result.wOperateUser = data.readUnsignedShort();
			result.wProvideUser = data.readUnsignedShort();
			result.cbOperateCode = data.readUnsignedByte();
			result.cbOperateSparrow = data.readUnsignedByte();
			
			data.position=pos;
			return result;
		}
	}
}
include "../../../Common/Memory.as"