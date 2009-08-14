package org.god.IGameFrame
{
	//游戏桌子属性
	public class tagTableState
	{
		public static  const NOTE_LEN:uint =64;//桌子备注
		
		public var bPass:Boolean;//密码标志
		public var bPlaying:Boolean;//游戏标志
		public var wTableID:uint;//桌子号码
		public var wWatchCount:uint;//旁观数目
		public var wChairCount:uint;//玩家数目
		public var dwTableMasterID:uint;//台主玩家
		public var szTableNote:String = new String;//桌子备注
		public var pIUserItem:Array = new Array(MAX_CHAIR);//用户信息
		public function tagTableState()
		{
			//szTableNote.length = [NOTE_LEN]

			//pIUserItem IUserItem[MAX_CHAIR]
		}
	}
}
include "../Net/GLOBALDEF.as"
include "../Net/GLOBALFrame.as"
