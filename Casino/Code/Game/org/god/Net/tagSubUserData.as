package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	//子用户信息结构
	public class tagSubUserData
	{
		public static const sizeof_tagSubUserData:int = 28/*dwUserID-dwHighUserID4*/ + 
NAME_LEN + 1 + NAME_LEN/*szName*/ + 1 + 4 + 4 + 8;

		public var dwUserID:uint;
		public var dwParentUserID:uint;
		public var dwHighUserID0:uint;
		public var dwHighUserID1:uint;
		public var dwHighUserID2:uint;
		public var dwHighUserID3:uint;
		public var dwHighUserID4:uint;
		public var szAccount:String =  String("");
		public var cbState:uint;
		public var szName:String =  String("");
		public var cbStateCongealFlag:uint;
		public var dwUserRight:uint;
		public var dwMasterRight:uint;
		public var fRegisterTime:Number = Number(0);

		public function tagSubUserData()
		{
			//szAccount.length = NAME_LEN;
			//szName.length = NAME_LEN; 
		}
		public static function readData(data:ByteArray):tagSubUserData
		{
			var pos:int=data.position;
			var result:tagSubUserData=new tagSubUserData;

			result.dwUserID = data.readUnsignedInt();
			result.dwParentUserID= data.readUnsignedInt();
			result.dwHighUserID0= data.readUnsignedInt();
			result.dwHighUserID1= data.readUnsignedInt();
			result.dwHighUserID2= data.readUnsignedInt();
			result.dwHighUserID3= data.readUnsignedInt();
			result.dwHighUserID4= data.readUnsignedInt();
			result.szAccount = readStringByByteArray(data, NAME_LEN);
			result.cbState= data.readByte();
			result.szName = readStringByByteArray(data, NAME_LEN);
			result.cbStateCongealFlag  = data.readByte();
			result.dwUserRight= data.readUnsignedInt();
			result.dwMasterRight= data.readUnsignedInt();
			result.fRegisterTime= data.readDouble();


			data.position=pos;
			return result;
		}
		public function writeByteArray(data:ByteArray):void
		{
			data.writeUnsignedInt(dwUserID);
			data.writeUnsignedInt(dwParentUserID);
			data.writeUnsignedInt(dwHighUserID0);
			data.writeUnsignedInt(dwHighUserID1);
			data.writeUnsignedInt(dwHighUserID2);
			data.writeUnsignedInt(dwHighUserID3);
			data.writeUnsignedInt(dwHighUserID4);
			writeStringToByteArray(data,szAccount, NAME_LEN);
			data.writeByte(cbState);
			writeStringToByteArray(data, szName, NAME_LEN);
			data.writeByte(cbStateCongealFlag);
			data.writeUnsignedInt(dwUserRight);
			data.writeUnsignedInt(dwMasterRight);
			data.writeDouble(fRegisterTime);

		}
	}

}
include "GLOBALDEF.as"
include "../Common/Memory.as"
