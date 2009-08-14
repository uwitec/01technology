package org.god.Common
{
	//通用辅助函数
	public class FuncUtil
	{
		//获取用户等级名称
		public static function fu_GetPeerLevelName(cbLevel:uint):String
		{
			switch (cbLevel)
			{
				case 0 :
					return IDS_COMPANY;
				case 1 :
					return IDS_BIGSTOCKHOLDER;
				case 2 :
					return IDS_STOCKHOLDER;
				case 3 :
					return IDS_GENERALAGENT;
				case 4 :
					return IDS_AGENT;
				case 5 :
					return IDS_GAMEUSER;
				default :
					break;
			}
			return "??????";
		}
		//获取等级用户帐号前缀
		public static function fu_GetAcountPrefix(cbLevel:uint):String
		{
			switch (cbLevel)
			{
				case 0 :
					return "a";
				case 1 :
					return "u";
				case 2 :
					return "v";
				case 3 :
					return "x";
				case 4 :
					return "y";
				case 5 :
					return "p";
				default :
					break;
			}
			return "?";
		}
		//获取子用户帐号前缀
		public static function fu_GetSubAcountPrefix():String
		{
			return "s";
		}
		//获取详细用户状态文本
		public static function fu_GetStateCongealName(cbLevel:uint,cbStateCongealFlag:uint):String
		{
			switch (cbLevel)
			{
				case 0 :
					if (cbStateCongealFlag & 0x1)
					{
						return IDS_CONGEAL;
					}
				case 1 :
					if (cbStateCongealFlag & 0x2)
					{
						return IDS_CONGEAL;
					}
				case 2 :
					if (cbStateCongealFlag & 0x4)
					{
						return IDS_CONGEAL;
					}
				case 3 :
					if (cbStateCongealFlag & 0x8)
					{
						return IDS_CONGEAL;
					}
				case 4 :
					if (cbStateCongealFlag & 0x10)
					{
						return IDS_CONGEAL;
					}
				case 5 :
					if (cbStateCongealFlag & 0x20)
					{
						return IDS_CONGEAL;
					}
				default :
					break;
			}
			if (cbStateCongealFlag != 0)
			{
				return IDS_CONGEAL + "(" + IDS_CONGEALRELATION + ")";
			}

			return IDS_NORMAL;
		}
		//获取用户状态文本
		public static function fu_GetSimpleStateCongealName(cbLevel:uint,cbStateCongealFlag:uint):String
		{
			switch (cbLevel)
			{
				case 0 :
					if (cbStateCongealFlag & 0x1)
					{
						return IDS_CONGEAL;
					}
				case 1 :
					if (cbStateCongealFlag & 0x2)
					{
						return IDS_CONGEAL;
					}
				case 2 :
					if (cbStateCongealFlag & 0x4)
					{
						return IDS_CONGEAL;
					}
				case 3 :
					if (cbStateCongealFlag & 0x8)
					{
						return IDS_CONGEAL;
					}
				case 4 :
					if (cbStateCongealFlag & 0x10)
					{
						return IDS_CONGEAL;
					}
				case 5 :
					if (cbStateCongealFlag & 0x20)
					{
						return IDS_CONGEAL;
					}
				default :
					break;
			}
			return IDS_NORMAL;
		}
		//获取子用户状态文本
		public static function fu_GetSimpleSubUserStateCongealName(cbStateCongealFlag:uint):String
		{
			if (cbStateCongealFlag & 0x40)
			{
				return IDS_CONGEAL;
			}

			return IDS_NORMAL;
		}
		//判断用户停用状态
		public static function fu_IsCongeal(cbStateCongealFlag:uint):Boolean
		{

			if (cbStateCongealFlag != 0)
			{
				return true;
			}
			return false;
		}
		//获取详细占成比文本
		public static function fu_GetOccupancyRateName(fOccupancyRate:Number,cbOccupancyRate_NoFlag:uint):String
		{
			if (cbOccupancyRate_NoFlag)
			{
				return IDS_NOOCCUPANCYRATE;
			}
			return Number(100.0 * fOccupancyRate).toFixed(0) + "%";
		}
		//获取占成比文本
		public static function fu_GetSimpleOccupancyRateName(fOccupancyRate:Number):String
		{
			return  Number(100.0 * fOccupancyRate).toFixed(0) + "%";
		}
		//获取洗码比文本
		public static function fu_GetRollbackRate(fRollbackRate:Number):String
		{
			return Number(100.0 * fRollbackRate).toFixed(2) + "%";
		}
		//获取时间文本
		public static function fu_GetDateTimeString(fDateTime:Number):String
		{
			var date:Date=new Date  ;
			date.setTime(fDateTime * 1000.0);

			return date.fullYear + IDS_YEAR + (date.month + 1) + IDS_MONTH + date.date + IDS_DATE + " " + date.hours + ":" + date.minutes + ":" + date.seconds;
		}
		//比较占成比
		public static function fu_CMPOccupancyRate(fOccupancyRate0:Number,cbOccupancyRate_NoFlag0:uint,
												   fOccupancyRate1:Number,cbOccupancyRate_NoFlag1:uint):Boolean
		{
			if(cbOccupancyRate_NoFlag0 != cbOccupancyRate_NoFlag1)
				return false;
			var o1:uint = uint((fOccupancyRate0 + 0.000005) * 100000)
			var o2:uint = uint((fOccupancyRate1 + 0.000005) * 100000)
			if(o1 == o2)
				return true;
			else
				return false;
		}
		//获取IP地址
		public static function GetIPAddressString(dwServerAddr:uint, bHalf:Boolean = true):String
		{
			var b0:uint = dwServerAddr & 0x000000ff;
			var b1:uint = (dwServerAddr >> 8) & 0x000000ff;
			var b2:uint = (dwServerAddr >> 16)& 0x000000ff;
			var b3:uint = (dwServerAddr >> 24)& 0x000000ff;
			var result:String;
			if(bHalf)
				result = b0 + "." + b1 + ".*.*";
			else
				result = b0 + "." + b1 + "." + b2 + "." + b3;
			return result;
		}
	}
}
include "./StringTableDef.as"