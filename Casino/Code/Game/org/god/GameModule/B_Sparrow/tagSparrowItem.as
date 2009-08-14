package 
{
	public class tagSparrowItem
	{
		public var bShoot:Boolean;//弹起标志
		public var cbSparrowData:uint;//扑克数据
		public function tagSparrowItem()
		{
			
		}
		public function clone():tagSparrowItem
		{
			var result:tagSparrowItem = new tagSparrowItem;
			result.bShoot 	 = this.bShoot;
			result.cbSparrowData = this.cbSparrowData;
			return result;
		}
	}
}