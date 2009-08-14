package org.god.Net
{
	import flash.utils.ByteArray;
	//帐号列表
	public class CMD_GP_SubUserList
	{
		public static const sizeof_Header_CMD_GP_SubUserList:uint = 4 + 2;
		
		public var dwParentUserID:uint;//上线帐号ID
		public var wSubUserCount:uint;//子帐号人数
		public var UserData:Array  = new Array(0);
		
		public function CMD_GP_SubUserList()
		{
		}
		public static function readData(data:ByteArray):CMD_GP_SubUserList
		{
			var pos:int=data.position;
			var result:CMD_GP_SubUserList=new CMD_GP_SubUserList;
			result.dwParentUserID = data.readUnsignedInt();
			result.wSubUserCount = data.readUnsignedShort();
			if(result.wSubUserCount > 0)
			{
				result.UserData  = new Array(result.wSubUserCount);
				for(var i:uint = 0; i < result.wSubUserCount; i ++)
				{
					result.UserData[i] = tagSubUserData.readData(data);
					
					data.position += tagSubUserData.sizeof_tagSubUserData;
				}
			}
			data.position=pos;
			return result;
		}
	}
}
include "GLOBALDEF.as"
include "../Common/Memory.as"