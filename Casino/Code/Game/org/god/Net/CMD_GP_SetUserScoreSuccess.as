package org.god.Net
{
	import flash.utils.ByteArray;

	//额度存取成功
	public class CMD_GP_SetUserScoreSuccess
	{
		public static const sizeof_CMD_GP_SetUserScoreSuccess:uint = 4 + 4 + 8 + 8;
		
		public var dwParentUserID:uint;//被存取用户上线ID
		public var dwUserID:uint;//被存取用户ID
		public var fScore:Number;//额度
		public var fScoreOffset:Number;//额度差
		
		public function CMD_GP_SetUserScoreSuccess()
		{

		}
		public static  function readData(data:ByteArray):CMD_GP_SetUserScoreSuccess
		{
			var pos:int=data.position;
			var result:CMD_GP_SetUserScoreSuccess=new CMD_GP_SetUserScoreSuccess  ;
			result.dwParentUserID=data.readUnsignedInt();
			result.dwUserID=data.readUnsignedInt();
			result.fScore=data.readDouble();
			result.fScoreOffset=data.readDouble();
			data.position=pos;
			return result;
		}
	}
}