package org.god.Common
{
	//登陆站点单元
	public class LogonStationCell
	{
		protected var m_strName:String="";//登陆站点名称
		protected var m_strAddress:String="";//登陆站点地址
		//构造函数
		public function LogonStationCell(strName:String,strAddress:String)
		{
			m_strName = strName;
			m_strAddress = strAddress;
		}
		//获取登陆地址
		public function getAddress():String
		{
			return m_strAddress;
		}
		//获取登陆名称
		public function toString():String
		{
			return m_strName;
		}
	}
}