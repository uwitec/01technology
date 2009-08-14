package  org.god.IGameFrame
{
	//服务属性
	public class tagServerAttribute
	{
		public var wKindID:uint;//类型标识
		public var wServerID:uint;//房间标识
		public var wGameGenre:uint;//游戏类型
		public var wChairCount:uint;//椅子数目
		public var szKindName:String = new String;//类型名字
		public var szServerName:String = new String;//房间名称
		public var fCellScore:Number;//单位额度
		public var fHighScore:Number;//最高额度
		public var fLessScore:Number;//最低额度
		
		public function tagServerAttribute()
		{
			//szKindName.length = [KIND_LEN];
			//szServerName.length = [SERVER_LEN]
		}
	}
}