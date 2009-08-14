package org.god.GameModule.B_Sparrow.Common
{
	import flash.utils.ByteArray;
	//操作命令
	public class CMD_C_OperateSparrow
	{
		public static const sizeof_CMD_C_OperateSparrow:uint = 1 + 1 ;
		
		public var cbOperateCode:uint;//操作代码
		public var cbOperateSparrow:uint;//操作扑克

		public function CMD_C_OperateSparrow()
		{
		}
		public function toByteArray():ByteArray
		{
			var result:ByteArray = newLitteEndianByteArray();
			result.writeByte(cbOperateCode);
			result.writeByte(cbOperateSparrow);
			return result;
		}
	}
}
include "../../../Common/Memory.as"