package org.god.Common
{
	//管理权限辅助
	public class MasterRight
	{
		public static const MR_CAN_ACCOUNT_NEW:uint 	= 0x00000001;//下线帐号新建
		public static const MR_CAN_ACCOUNT_SET:uint 	= 0x00000002;//下线帐号设定
		public static const MR_CAN_SUBACCOUNT_NEW:uint  = 0x00000004;//子帐号新建
		public static const MR_CAN_SUBACCOUNT_SET:uint  = 0x00000008;//子帐号设定
		public static const MR_CAN_CHART_VIEW:uint 		= 0x00000010;//游戏报表查看
		public static const MR_CAN_BETHISTORY_VIEW:uint = 0x00000020;//游戏记录查看
		public static const MR_CAN_BETMONITOR_VIEW:uint = 0x00000040;//现场投注查看
		public static const MR_CAN_OPERATION_VIEW:uint  = 0x00000080;//操作记录查看
		public static const MR_CAN_MYSELFDATA_SET:uint  = 0x00000100;//自身帐号设定
		
		public static const USERTYPE_SUB:uint			=1;	//子帐号
		//构造函数
		public function MasterRight()
		{
		}
		public static function SimpleCheck_UserRight():Boolean
		{
			if(GlobalCommonMain.getGlobalCommonMain().GetUserType() == USERTYPE_SUB &&
				GlobalCommonMain.getGlobalCommonMain().GetUserLevel() != 0)
				return false;
			return true;
		}
		public static function CanAccountNew(dwMasterRight:uint):Boolean//下线帐号新建
		{
			return (dwMasterRight & MR_CAN_ACCOUNT_NEW)!=0 ? false : true;
		}
		public static function CanAccountSet(dwMasterRight:uint):Boolean//下线帐号设定
		{
			return (dwMasterRight & MR_CAN_ACCOUNT_SET)!=0 ? false : true;
		}
		public static function CanSubAccountNew(dwMasterRight:uint):Boolean//子帐号新建
		{
			return (dwMasterRight & MR_CAN_SUBACCOUNT_NEW)!=0 ? false : true;
		}
		public static function CanSubAccountSet(dwMasterRight:uint):Boolean//子帐号设定
		{
			return (dwMasterRight & MR_CAN_SUBACCOUNT_SET)!=0 ? false : true;
		}
		public static function CanChartView(dwMasterRight:uint):Boolean//游戏报表查看
		{
			return (dwMasterRight & MR_CAN_CHART_VIEW)!=0 ? false : true;
		}
		public static function CanBetHistoryView(dwMasterRight:uint):Boolean//游戏记录查看
		{
			return (dwMasterRight & MR_CAN_BETHISTORY_VIEW)!=0 ? false : true;
		}
		public static function CanBetMonitorView(dwMasterRight:uint):Boolean//现场投注查看
		{
			return (dwMasterRight & MR_CAN_BETMONITOR_VIEW)!=0 ? false : true;
		}
		public static function CanOperationView(dwMasterRight:uint):Boolean//操作记录查看
		{
			return (dwMasterRight & MR_CAN_OPERATION_VIEW)!=0 ? false : true;
		}
		public static function CanMyselfDataSet(dwMasterRight:uint):Boolean//自身帐号设定
		{
			return (dwMasterRight & MR_CAN_MYSELFDATA_SET)!=0 ? false : true;
		}
		///////////////////////////////////////////////////////////////////////////
		public static function AllowAccountNew(dwMasterRight:uint):uint//下线帐号新建
		{
			return (dwMasterRight & ~MR_CAN_ACCOUNT_NEW);
		}
		public static function AllowAccountSet(dwMasterRight:uint):uint//下线帐号设定
		{
			return (dwMasterRight & ~MR_CAN_ACCOUNT_SET);
		}
		public static function AllowSubAccountNew(dwMasterRight:uint):uint//子帐号新建
		{
			return (dwMasterRight & ~MR_CAN_SUBACCOUNT_NEW);
		}
		public static function AllowSubAccountSet(dwMasterRight:uint):uint//子帐号设定
		{
			return (dwMasterRight & ~MR_CAN_SUBACCOUNT_SET);
		}
		public static function AllowChartView(dwMasterRight:uint):uint//游戏报表查看
		{
			return (dwMasterRight & ~MR_CAN_CHART_VIEW);
		}
		public static function AllowBetHistoryView(dwMasterRight:uint):uint//游戏记录查看
		{
			return (dwMasterRight & ~MR_CAN_BETHISTORY_VIEW);
		}
		public static function AllowBetMonitorView(dwMasterRight:uint):uint//现场投注查看
		{
			return (dwMasterRight & ~MR_CAN_BETMONITOR_VIEW);
		}
		public static function AllowOperationView(dwMasterRight:uint):uint//操作记录查看
		{
			return (dwMasterRight & ~MR_CAN_OPERATION_VIEW);
		}
		public static function AllowMyselfDataSet(dwMasterRight:uint):uint//自身帐号设定
		{
			return (dwMasterRight & ~MR_CAN_MYSELFDATA_SET);
		}
		///////////////////////////////////////////////////////////////////////////
		public static function LimitAccountNew(dwMasterRight:uint):uint//下线帐号新建
		{
			return (dwMasterRight | MR_CAN_ACCOUNT_NEW);
		}
		public static function LimitAccountSet(dwMasterRight:uint):uint//下线帐号设定
		{
			return (dwMasterRight | MR_CAN_ACCOUNT_SET);
		}
		public static function LimitSubAccountNew(dwMasterRight:uint):uint//子帐号新建
		{
			return (dwMasterRight | MR_CAN_SUBACCOUNT_NEW);
		}
		public static function LimitSubAccountSet(dwMasterRight:uint):uint//子帐号设定
		{
			return (dwMasterRight | MR_CAN_SUBACCOUNT_SET);
		}
		public static function LimitChartView(dwMasterRight:uint):uint//游戏报表查看
		{
			return (dwMasterRight | MR_CAN_CHART_VIEW);
		}
		public static function LimitBetHistoryView(dwMasterRight:uint):uint//游戏记录查看
		{
			return (dwMasterRight | MR_CAN_BETHISTORY_VIEW);
		}
		public static function LimitBetMonitorView(dwMasterRight:uint):uint//现场投注查看
		{
			return (dwMasterRight | MR_CAN_BETMONITOR_VIEW);
		}
		public static function LimitOperationView(dwMasterRight:uint):uint//操作记录查看
		{
			return (dwMasterRight | MR_CAN_OPERATION_VIEW);
		}
		public static function LimitMyselfDataSet(dwMasterRight:uint):uint//自身帐号设定
		{
			return (dwMasterRight | MR_CAN_MYSELFDATA_SET);
		}
	}
}