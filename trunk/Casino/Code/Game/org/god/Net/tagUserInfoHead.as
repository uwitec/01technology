package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	//用户基本信息结构
	public class tagUserInfoHead
	{
		public static const sizeof_tagUserInfoHead = 2 + 4 + NAME_LEN +
		tagUserStatus.sizeof_tagUserStatus + tagUserScore.sizeof_tagUserScore;
		//用户属性
		public var wFaceID:uint;//头像索引
		public var dwUserID:uint;//用户 I D
		public var szName:String = new String;

		public var UserScore:tagUserScore = new tagUserScore;
		//用户状态
		public var UserStatus:tagUserStatus = new tagUserStatus;

		public function tagUserInfoHead()
		{
			//szName.length = NAME_LEN;
		}
		public static function readData(data:ByteArray):tagUserInfoHead
		{
			var pos:int=data.position;
			var result:tagUserInfoHead=new tagUserInfoHead;
			result.wFaceID = data.readUnsignedShort();
			result.dwUserID= data.readUnsignedInt();
			result.szName= readStringByByteArray(data, NAME_LEN);
			
			result.UserScore  = tagUserScore.readData(data);
			data.position += tagUserScore.sizeof_tagUserScore;
			result.UserStatus  = tagUserStatus.readData(data);

			data.position = pos;
			return result;
		}
		public function clone():tagUserInfoHead
		{
			var result:tagUserInfoHead=new tagUserInfoHead;
			result.wFaceID = this.wFaceID;
			result.dwUserID= this.dwUserID;
			result.szName  = String(this.szName);
			result.UserScore  = this.UserScore.clone();
			result.UserStatus  = this.UserStatus.clone();
			return result;
		}
	}
}
include "GLOBALDEF.as"
include "../Common/Memory.as"
