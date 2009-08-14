package org.god.Common
{
	//房间服务状态
	public class enServiceStatus
	{
		public static  const ServiceStatus_Null:int=0;//没有状态
		public static  const ServiceStatus_Connecting:int=1;//连接状态
		public static  const ServiceStatus_EfficacyUser:int=2;//效验用户
		public static  const ServiceStatus_RecvConfigInfo:int=3;//接收配置
		public static  const ServiceStatus_RecvRoomInfo:int=4;//接收信息
		public static  const ServiceStatus_Serviceing:int=5;//服务状态
		public static  const ServiceStatus_NetShutDown:int=6;//网络中断

	}
}