package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	//用户积分信息
	public class tagUserScore
	{
		public static const sizeof_tagUserScore:int = 8;
		
		public var fScore:Number = 0;

		public function tagUserScore()
		{

		}
		public static function readData(data:ByteArray):tagUserScore
		{
			var pos:int=data.position;
			var result:tagUserScore=new tagUserScore;
			result.fScore = data.readDouble();

			data.position = pos;
			return result;
		}
		public function writeByteArray(data:ByteArray):void
		{
			data.writeDouble(fScore);

		}
		public function clone():tagUserScore
		{
			var result:tagUserScore=new tagUserScore;
			result.fScore = this.fScore;
			return result;
		}
	}
}
include "GLOBALDEF.as"
include "../Common/Memory.as"
