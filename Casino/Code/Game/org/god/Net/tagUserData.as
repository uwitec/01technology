package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	
	//用户信息结构
	public class tagUserData
	{
		public static const sizeof_tagUserData:int = 28/*dwUserID-dwHighUserID4*/ + 
NAME_LEN + 1 + 1 + NAME_LEN/*szName*/ + 1 + 1 + 4 + 4 + 8 + 8/*fLowScore*/ +
8 + 1 + 8 + 1 + 8 + 1 + 8+ 8/*fRollbackRate*/ +
4 + 8 +  2 + tagUserScore.sizeof_tagUserScore;

		public var dwUserID:uint;//用户ID
		public var dwParentUserID:uint;//用户父ID-上一级
		public var dwHighUserID0:uint;//用户父ID-公司
		public var dwHighUserID1:uint;//用户父ID-大股东
		public var dwHighUserID2:uint;//用户父ID-股东
		public var dwHighUserID3:uint;//用户父ID-总代理
		public var dwHighUserID4:uint;//用户父ID-代理
		public var szAccount:String =  String("");//用户帐号
		public var cbLevel:uint;//用户级别
		public var cbState:uint;//用户状态
		public var szName:String =  String("");//用户名称
		public var cbFaceID:uint;//用户头像
		public var cbStateCongealFlag:uint;//用户状态 
		public var dwUserRight:uint;//用户游戏权限
		public var dwMasterRight:uint;//用户管理权限
		public var fMidScore:Number = Number(0);//用户体系额度
		public var fLowScore:Number = Number(0);//会员额度
		public var fOccupancyRate:Number = Number(0);//输赢占成比
		public var cbOccupancyRate_NoFlag:uint;//输赢不占成标记
		public var fLowOccupancyRate_Max:Number = Number(0);//输赢下线最高占成比
		public var cbLowOccupancyRate_Max_NoFlag:uint;//输赢下线最高不占成比标记
		public var fLowOccupancyRate_Min:Number = Number(0);//输赢下线最低占成比
		public var cbLowOccupancyRate_Min_NoFlag:uint;//输赢下线最低占成比
		public var fTaxOccupancyRate:Number = Number(0);//抽水占成比
		public var fRollbackRate:Number = Number(0);//洗码比
		public var dwBetLimit:uint;//游戏上下限
		public var fRegisterTime:Number = Number(0);//注册时间
		public var wLessUserCount:uint;//下线人数

		public var UserScore:tagUserScore = new tagUserScore;//信用额度

		
		public function tagUserInfoHead()
		{
			//szAccount.length = NAME_LEN;
			//szName.length = NAME_LEN; 
		}
		public static function readData(data:ByteArray):tagUserData
		{
			var pos:int=data.position;
			var result:tagUserData=new tagUserData;

			result.dwUserID = data.readUnsignedInt();
			result.dwParentUserID= data.readUnsignedInt();
			result.dwHighUserID0= data.readUnsignedInt();
			result.dwHighUserID1= data.readUnsignedInt();
			result.dwHighUserID2= data.readUnsignedInt();
			result.dwHighUserID3= data.readUnsignedInt();
			result.dwHighUserID4= data.readUnsignedInt();
			result.szAccount = readStringByByteArray(data, NAME_LEN);
			result.cbLevel= data.readByte();
			result.cbState= data.readByte();
			result.szName = readStringByByteArray(data, NAME_LEN);
			result.cbFaceID= data.readByte();
			result.cbStateCongealFlag  = data.readByte();
			result.dwUserRight= data.readUnsignedInt();
			result.dwMasterRight= data.readUnsignedInt();
			result.fMidScore= data.readDouble();
			result.fLowScore= data.readDouble();
			result.fOccupancyRate= data.readDouble();
			result.cbOccupancyRate_NoFlag= data.readByte();
			result.fLowOccupancyRate_Max= data.readDouble();
			result.cbLowOccupancyRate_Max_NoFlag = data.readByte();
			result.fLowOccupancyRate_Min= data.readDouble();
			result.cbLowOccupancyRate_Min_NoFlag = data.readByte();
			result.fTaxOccupancyRate= data.readDouble();
			result.fRollbackRate= data.readDouble();
			result.dwBetLimit= data.readUnsignedInt();
			result.fRegisterTime= data.readDouble();
			result.wLessUserCount =data.readUnsignedShort();

			result.UserScore= tagUserScore.readData(data);

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
			data.writeByte(cbLevel);
			data.writeByte(cbState);
			writeStringToByteArray(data, szName, NAME_LEN);
			data.writeByte(cbFaceID);
			data.writeByte(cbStateCongealFlag);
			data.writeUnsignedInt(dwUserRight);
			data.writeUnsignedInt(dwMasterRight);
			data.writeDouble(fMidScore);
			data.writeDouble(fLowScore);
			data.writeDouble(fOccupancyRate);
			data.writeByte(cbOccupancyRate_NoFlag);
			data.writeDouble(fLowOccupancyRate_Max);
			data.writeByte(cbLowOccupancyRate_Max_NoFlag);
			data.writeDouble(fLowOccupancyRate_Min);
			data.writeByte(cbLowOccupancyRate_Min_NoFlag);
			data.writeDouble(fTaxOccupancyRate);
			data.writeDouble(fRollbackRate);
			data.writeUnsignedInt(dwBetLimit);
			data.writeDouble(fRegisterTime);
			data.writeShort(wLessUserCount);


			UserScore.writeByteArray(data);
		}
	}

}
include "GLOBALDEF.as"
include "../Common/Memory.as"
