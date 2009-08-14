package org.god.Common
{
	//时间通用辅助
	public class TimeUtil
	{
		public static const millisecondsPerMinute:int = 1000 * 60;
		public static const millisecondsPerHour:int = 1000 * 60 * 60;
		public static const millisecondsPerDay:int = 1000 * 60 * 60 * 24;
		//获取当前时间的偏移时间
		public static function getTimeBeforeDayOffset(date:Date,nDayOffset:int = 3):Date
		{
			var result:Date = new Date(date);
			result.setTime(result.getTime() - millisecondsPerDay * nDayOffset + 1000);
			return result;
		}
		//获取查询开始时间
		public static function getQueryBeginTime(nDayOffset:int = 3):Date
		{
			var result:Date = new Date(getQueryEndTime());
			//result.setTime(result.getTime() - millisecondsPerDay * nDayOffset + 1000);
			return getTimeBeforeDayOffset(result, nDayOffset);
		}
		//获取查询结束时间
		public static function getQueryEndTime():Date
		{
			var now:Date = new Date;
			var date:Date = new Date(now.fullYear, 
								now.month,
								now.date,
								11,
								59,
								59,
								0);
			if(now.hours >= 12)
			{
				var result:Date = new Date();
				result.setTime(date.getTime() + millisecondsPerDay);
				return result;
			}
			else
			{
				return date;
			}
		}
		//获取时间差
		public static function getTimeOffsetDates(d0:Date,d1:Date):int
		{
			var f:Number = d1.getTime() - d0.getTime();
			return int(f / millisecondsPerDay);
		}
	}
}