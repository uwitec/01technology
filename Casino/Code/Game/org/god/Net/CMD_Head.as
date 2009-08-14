package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;

	//数据包传递包头
	public class CMD_Head
	{
		public static const sizeof_CMD_Head:int=CMD_Info.sizeof_CMD_Info + CMD_Command.sizeof_CMD_Command;
		
		public var CmdInfo:CMD_Info=new CMD_Info;//基础结构
		public var CommandInfo:CMD_Command=new CMD_Command;//命令信息

		public static function readData(data:ByteArray):CMD_Head
		{
			var pos:int=data.position;
			var result:CMD_Head=new CMD_Head;

			result.CmdInfo.wDataSize=data.readUnsignedShort();
			result.CmdInfo.cbCheckCode=data.readByte();
			result.CmdInfo.cbMessageVer=data.readByte();

			result.CommandInfo.wMainCmdID=data.readUnsignedShort();
			result.CommandInfo.wSubCmdID=data.readUnsignedShort();
			data.position=pos;
			return result;
		}
		public function toByteArray():ByteArray
		{
			var result:ByteArray = newLitteEndianByteArray();

			result.writeBytes(CmdInfo.toByteArray());
			result.writeBytes(CommandInfo.toByteArray());

			return result;
		}
	}

}
include "GLOBALDEF.as"
include "../Common/Memory.as"
