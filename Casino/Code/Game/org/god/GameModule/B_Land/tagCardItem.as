package 
{
	public class tagCardItem
	{
		public var bShoot:Boolean;//弹起标志
		public var bCardData:uint;//扑克数据
		public function tagCardItem()
		{
			bCardData = 0xffffffff;
		}
		public function clone():tagCardItem
		{
			var result:tagCardItem = new tagCardItem;
			result.bShoot 	 = this.bShoot;
			result.bCardData = this.bCardData;
			return result;
		}
	}
}