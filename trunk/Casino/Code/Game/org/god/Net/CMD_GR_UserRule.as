package org.god.Net
{
	import flash.utils.ByteArray;
	import flash.utils.Endian;
	//用户规则
	public class CMD_GR_UserRule
	{
		public var bPassword:uint;//设置密码
		public var bLimitWin:uint;//限制胜率
		public var bLimitFlee:uint;//限制断线
		public var bLimitScore:uint;//限制分数
		public var bLimitDelay:uint;//限制延时
		public var bCheckSameIP:uint;//效验地址
		public var wWinRate:uint;//限制胜率
		public var wFleeRate:uint;//限制逃跑
		public var wNetDelay:uint;//网络延时
		public var lMaxScore:int;//最高分数 
		public var lLessScore:int;//最低分数
		public var szPassword:String=new String  ;//桌子密码
		public function CMD_GR_UserRule()
		{
			//szPassword.length = PASS_LEN;
		}
	}
}
include "GLOBALDEF.as";
