package org.god.Common.Operation
{
	import org.god.Common.FuncUtil;
	
	import org.god.Net.Operation.*;
	//操作通用辅助
	public class OperationUtil
	{
		public static function getOPTypeName(nOPerationType:uint):String
		{
			switch(nOPerationType)
			{
				case OPT_All:
					return "全部";
				case OPT_Logon:
					return "登陆";
				case OPT_NewAccount:
					return "新增下线帐号";
				case OPT_NewSubAccount:
					return "新增子帐号";
				case OPT_SetBasicInfo:
					return "设定密码及名称";
				case OPT_SetScore:
					return "存取额度";
				case OPT_SetCongealState:
					return "设定状态";
				case OPT_SetOccupancyRate:
					return "设定占成比";
				case OPT_SetRollbackRate:
					return "设定洗码比";
				case OPT_SetUserRight:
					return "设定权限";
				case OPT_KickUser:
					return "踢出游戏房间";
					
				case OPT_Notice:
					return "发布公告";
				case OPT_CancelBetHistory:
					return "取消游戏记录";
				case OPT_RestoreBetHistory:
					return "恢复游戏记录";
				case OPT_SetBetLimit:
					return "设定游戏上下限";
				case OPT_SetTax:
					return "设定抽水率";
				case OPT_SetRevenueSharing:
					return "设定限红";
				case OPT_DismissGame:
					return "解散游戏";
			}
			return "";
		}
		public static function GetOPTimeString(fDateTime:Number):String
		{
			return FuncUtil.fu_GetDateTimeString(fDateTime);
		}
		public static function GetOPAddressString(dwServerAddr:uint, bHalf:Boolean = true):String
		{
			var b0:uint = dwServerAddr & 0x000000ff;
			var b1:uint = (dwServerAddr >> 8) & 0x000000ff;
			var b2:uint = (dwServerAddr >> 16)& 0x000000ff;
			var b3:uint = (dwServerAddr >> 24)& 0x000000ff;
			var result:String;
			if(bHalf)
				result = b0 + "." + b1 + ".*.*";
			else
				result = b0 + "." + b1 + "." + b2 + "." + b3;
			return result;
		}
		public static function GetOPDetailString(Header:tagOperationHeader,
												Content:tagOperationContent):String
		{
			var szDetail:String = "";
			switch(Header.dwOPType)
			{
				case OPT_All:
					{
						break;
					}
				case OPT_Logon:
					{
						break;
					}
				case OPT_NewAccount:
					{
						var pOperationData_NewAccount:tagOperationData_NewAccount = 
							tagOperationData_NewAccount.readData(Content.data_Nothing);
						szDetail = IDS_NEW + FuncUtil.fu_GetPeerLevelName(pOperationData_NewAccount.cbLevel) + 
									"," + IDS_OCCUPANCYRATE + ":" + FuncUtil.fu_GetOccupancyRateName(pOperationData_NewAccount.fOccupancyRate,pOperationData_NewAccount.cbOccupancyRate_NoFlag) + 
									"," + IDS_ROLLBACKRATE + ":" + FuncUtil.fu_GetRollbackRate(pOperationData_NewAccount.fRollbackRate);
						break;
					}
				case OPT_NewSubAccount:
					{
						var pOperationData_NewSubAccount:tagOperationData_NewSubAccount=
						tagOperationData_NewSubAccount.readData(Content.data_Nothing);
						szDetail = IDS_NEW + IDS_SUBUSER + 
									"," + IDS_NAME + ":" + pOperationData_NewSubAccount.szName + 
									"," + IDS_PARENTUSER + ":" + pOperationData_NewSubAccount.szParentAccount;
						break;
					}
				case OPT_SetBasicInfo:
					{
						var pOperationData_SetBasicInfo:tagOperationData_SetBasicInfo=
						tagOperationData_SetBasicInfo.readData(Content.data_Nothing);
						szDetail = IDS_SET + " " + IDS_NAME + 
									":" + pOperationData_SetBasicInfo.szName + 
									"," + IDS_CHANGEPASSWD + ":*******";
						break;
					}
				case OPT_SetScore:
					{
						var pOperationData_SetScore:tagOperationData_SetScore=
						tagOperationData_SetScore.readData(Content.data_Nothing);
						var fBlanceScore:Number = pOperationData_SetScore.fSetScoreOffset + pOperationData_SetScore.fScoreResult;
						if(pOperationData_SetScore.fSetScoreOffset >= 0)
						{
							szDetail = IDS_SAVE + IDS_SCORE + ":" +
								pOperationData_SetScore.fSetScoreOffset.toFixed(2) + "," +
								IDS_BALANCE + ":" + fBlanceScore.toFixed(2);
						}
						else
						{
							szDetail = IDS_DRAW + IDS_SCORE + ":" +
								Number(-pOperationData_SetScore.fSetScoreOffset).toFixed(2) + "," +
								IDS_BALANCE + ":" + fBlanceScore.toFixed(2);
						}
						break;
					}
				case OPT_SetCongealState:
					{
						var pOperationData_SetCongealState:tagOperationData_SetCongealState=
						tagOperationData_SetCongealState.readData(Content.data_Nothing);
						if(pOperationData_SetCongealState.cbState == 0)
							szDetail = IDS_SET + " " + IDS_NORMAL + " " + IDS_STATE;
						else 
							szDetail = IDS_SET + " " + IDS_CONGEAL + " " + IDS_STATE;
						break;
					}
				case OPT_SetOccupancyRate:
					{
						var pOperationData_SetOccupancyRate:tagOperationData_SetOccupancyRate=
						tagOperationData_SetOccupancyRate.readData(Content.data_Nothing);
						
						szDetail = IDS_SET + " ";
						if(FuncUtil.fu_CMPOccupancyRate(pOperationData_SetOccupancyRate.fOccupancyRate,
											   pOperationData_SetOccupancyRate.cbOccupancyRate_NoFlag,
											   pOperationData_SetOccupancyRate.fOldOccupancyRate,
											   pOperationData_SetOccupancyRate.cbOldOccupancyRate_NoFlag) == false)
						{
							szDetail = szDetail + IDS_OCCUPANCYRATE + 
									":" + FuncUtil.fu_GetOccupancyRateName(pOperationData_SetOccupancyRate.fOldOccupancyRate,
											   pOperationData_SetOccupancyRate.cbOldOccupancyRate_NoFlag) +
									" --＞ " + FuncUtil.fu_GetOccupancyRateName(pOperationData_SetOccupancyRate.fOccupancyRate,
											   pOperationData_SetOccupancyRate.cbOccupancyRate_NoFlag);
						}
						
						if(FuncUtil.fu_CMPOccupancyRate(pOperationData_SetOccupancyRate.fLowOccupancyRate_Max,
											   pOperationData_SetOccupancyRate.cbLowOccupancyRate_Max_NoFlag,
											   pOperationData_SetOccupancyRate.fOldLowOccupancyRate_Max,
											   pOperationData_SetOccupancyRate.cbOldLowOccupancyRate_Max_NoFlag) == false)
						{
							szDetail = szDetail + IDS_LOWOCCUPANCYRATEMAX + 
									":" + FuncUtil.fu_GetOccupancyRateName(pOperationData_SetOccupancyRate.fOldLowOccupancyRate_Max,
											   pOperationData_SetOccupancyRate.cbOldLowOccupancyRate_Max_NoFlag) +
									" --＞ " + FuncUtil.fu_GetOccupancyRateName(pOperationData_SetOccupancyRate.fLowOccupancyRate_Max,
											   pOperationData_SetOccupancyRate.cbLowOccupancyRate_Max_NoFlag);
						}
						
						if(FuncUtil.fu_CMPOccupancyRate(pOperationData_SetOccupancyRate.fLowOccupancyRate_Min,
											   pOperationData_SetOccupancyRate.cbLowOccupancyRate_Min_NoFlag,
											   pOperationData_SetOccupancyRate.fOldLowOccupancyRate_Min,
											   pOperationData_SetOccupancyRate.cbOldLowOccupancyRate_Min_NoFlag) == false)
						{
							szDetail = szDetail + IDS_LOWOCCUPANCYRATEMIN + 
									":" + FuncUtil.fu_GetOccupancyRateName(pOperationData_SetOccupancyRate.fOldLowOccupancyRate_Min,
											   pOperationData_SetOccupancyRate.cbOldLowOccupancyRate_Min_NoFlag) +
									" --＞ " + FuncUtil.fu_GetOccupancyRateName(pOperationData_SetOccupancyRate.fLowOccupancyRate_Min,
											   pOperationData_SetOccupancyRate.cbLowOccupancyRate_Min_NoFlag);
						}
						if(FuncUtil.fu_CMPOccupancyRate(pOperationData_SetOccupancyRate.fTaxOccupancyRate,
											   0,
											   pOperationData_SetOccupancyRate.fOldTaxOccupancyRate,
											   0) == false)
						{
							szDetail = szDetail + IDS_TAXOCCUPANCYRATE + 
									":" + FuncUtil.fu_GetOccupancyRateName(pOperationData_SetOccupancyRate.fOldTaxOccupancyRate,
											   0) +
									" --＞ " + FuncUtil.fu_GetOccupancyRateName(pOperationData_SetOccupancyRate.fTaxOccupancyRate,
											   0);
						}
						
						break;
					}
				case OPT_SetRollbackRate:
					{
						var pOperationData_SetRollbackRate:tagOperationData_SetRollbackRate=
						tagOperationData_SetRollbackRate.readData(Content.data_Nothing);
						szDetail = IDS_SET + " " + IDS_ROLLBACKRATE + 
									":" + FuncUtil.fu_GetRollbackRate(pOperationData_SetRollbackRate.fOldRollbackRate) + 
									" --＞ " + FuncUtil.fu_GetRollbackRate(pOperationData_SetRollbackRate.fRollbackRate);
						break;
					}
				case OPT_SetUserRight:
					{
						var pOperationData_SetUserRight:tagOperationData_SetUserRight=
						tagOperationData_SetUserRight.readData(Content.data_Nothing);
						szDetail = IDS_SET + " " + IDS_RIGHT;
						break;
					}
				case OPT_KickUser:
					{
						var pOperationData_KickUser:tagOperationData_KickUser=
						tagOperationData_KickUser.readData(Content.data_Nothing);
						szDetail = IDS_TABLEPOSITION + ":" +
									pOperationData_KickUser.szTypeName + "-" + 
								   pOperationData_KickUser.szKindName + "-" +
								   pOperationData_KickUser.szServerName;
						
						break;
					}
			//////////////////////////////////////////////////////////////////
				case OPT_Notice:
					{
						break;
					}
				case OPT_CancelBetHistory:
					{
						break;
					}
				case OPT_RestoreBetHistory:
					{
						break;
					}
				case OPT_SetBetLimit:
					{
						break;
					}
				case OPT_SetTax:
					{
						break;
					}
				case OPT_SetRevenueSharing:
					{
						break;
					}
				case OPT_DismissGame:
					{
						break;
					}
			}
			return szDetail;
		}
	}
}
include "./StringTableDef.as"
include "./../StringTableDef.as"
include "../../Net/Operation/GlobalOperation.as"