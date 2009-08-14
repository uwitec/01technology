package org.god.Net
{
	import flash.utils.ByteArray;
	//帐号列表
	public class CMD_GP_LessUserList
	{
		public static const sizeof_Header_CMD_GP_LessUserList:uint = 4 + 2;
		
		public var dwParentUserID:uint;//上线帐号ID
		public var wUserCount:uint;//帐号人数
		public var UserData:Array  = new Array(0);
		
		public function CMD_GP_LessUserList()
		{
		}
		public static function readData(data:ByteArray):CMD_GP_LessUserList
		{
			var pos:int=data.position;
			var result:CMD_GP_LessUserList=new CMD_GP_LessUserList;
			result.dwParentUserID = data.readUnsignedInt();
			result.wUserCount = data.readUnsignedShort();
			if(result.wUserCount > 0)
			{
				result.UserData  = new Array(result.wUserCount);
				for(var i:uint = 0; i < result.wUserCount; i ++)
				{
					result.UserData[i] = tagUserData.readData(data);
					
					data.position += tagUserData.sizeof_tagUserData;
				}
			}
			data.position=pos;
			return result;
		}
	}
}
include "GLOBALDEF.as"
include "../Common/Memory.as"