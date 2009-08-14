package org.god.Net
{
	import org.aswing.util.HashMap;

	public class GlobalNetError
	{
		public function GlobalNetError()
		{
		}

		//CMD_GP_LogonError
		public static  const ErrCode_CMD_GP_LogonError_AccountNoExist=1;
		public static  const ErrCode_CMD_GP_LogonError_Password=ErrCode_CMD_GP_LogonError_AccountNoExist + 1;
		public static  const ErrCode_CMD_GP_LogonError_AccountFreeze=ErrCode_CMD_GP_LogonError_Password + 1;
		public static  const ErrCode_CMD_GP_LogonError_AlreadyLogon=ErrCode_CMD_GP_LogonError_AccountFreeze + 1;

		//CMD_GP_RegisterError
		public static  const ErrCode_CMD_GP_RegisterError_PasswdEmpty=ErrCode_CMD_GP_LogonError_AlreadyLogon + 1;
		public static  const ErrCode_CMD_GP_RegisterError_AccountEmpty=ErrCode_CMD_GP_RegisterError_PasswdEmpty + 1;
		public static  const ErrCode_CMD_GP_RegisterError_NameEmpty=ErrCode_CMD_GP_RegisterError_AccountEmpty + 1;
		public static  const ErrCode_CMD_GP_RegisterError_LevelInvalid=ErrCode_CMD_GP_RegisterError_NameEmpty + 1;
		public static  const ErrCode_CMD_GP_RegisterError_OccupancyRateInvalid=ErrCode_CMD_GP_RegisterError_LevelInvalid + 1;
		public static  const ErrCode_CMD_GP_RegisterError_LowOccupancyRate_MaxInvalid=ErrCode_CMD_GP_RegisterError_OccupancyRateInvalid + 1;
		public static  const ErrCode_CMD_GP_RegisterError_LowOccupancyRate_MinInvalid=ErrCode_CMD_GP_RegisterError_LowOccupancyRate_MaxInvalid + 1;
		public static  const ErrCode_CMD_GP_RegisterError_RollbackRateInvalid=ErrCode_CMD_GP_RegisterError_LowOccupancyRate_MinInvalid + 1;
		public static  const ErrCode_CMD_GP_RegisterError_UserScoreInvalid=ErrCode_CMD_GP_RegisterError_RollbackRateInvalid + 1;
		public static  const ErrCode_CMD_GP_RegisterError_AccoutExist=ErrCode_CMD_GP_RegisterError_UserScoreInvalid + 1;
		public static  const ErrCode_CMD_GP_RegisterError_ParentNotExist=ErrCode_CMD_GP_RegisterError_AccoutExist + 1;
		public static  const ErrCode_CMD_GP_RegisterError_ParentScoreNotEnough=ErrCode_CMD_GP_RegisterError_ParentNotExist + 1;
		public static  const ErrCode_CMD_GP_RegisterError_ORMTParentOR=ErrCode_CMD_GP_RegisterError_ParentScoreNotEnough + 1;
		public static  const ErrCode_CMD_GP_RegisterError_ORMTParentLORMax=ErrCode_CMD_GP_RegisterError_ORMTParentOR + 1;
		public static  const ErrCode_CMD_GP_RegisterError_ORLTParentLORMin=ErrCode_CMD_GP_RegisterError_ORMTParentLORMax + 1;
		public static  const ErrCode_CMD_GP_RegisterError_LORMaxMTParentLORMax=ErrCode_CMD_GP_RegisterError_ORLTParentLORMin + 1;
		public static  const ErrCode_CMD_GP_RegisterError_LORMinMTLORMax=ErrCode_CMD_GP_RegisterError_LORMaxMTParentLORMax + 1;
		public static  const ErrCode_CMD_GP_RegisterError_RollbackRateMTParentRollbackRate=ErrCode_CMD_GP_RegisterError_LORMinMTLORMax + 1;
		public static  const ErrCode_CMD_GP_RegisterError_Unknow=ErrCode_CMD_GP_RegisterError_RollbackRateMTParentRollbackRate + 1;

		//CMD_GP_SetUserScoreError
		public static  const ErrCode_CMD_GP_SetUserScoreError_ScoreNegative=ErrCode_CMD_GP_RegisterError_Unknow + 1;
		public static  const ErrCode_CMD_GP_SetUserScoreError_AccoutNotExist=ErrCode_CMD_GP_SetUserScoreError_ScoreNegative + 1;
		public static  const ErrCode_CMD_GP_SetUserScoreError_ParentNotExist=ErrCode_CMD_GP_SetUserScoreError_AccoutNotExist + 1;
		public static  const ErrCode_CMD_GP_SetUserScoreError_ParentScoreNotEnough=ErrCode_CMD_GP_SetUserScoreError_ParentNotExist + 1;
		public static  const ErrCode_CMD_GP_SetUserScoreError_AccountScoreNotEnough=ErrCode_CMD_GP_SetUserScoreError_ParentScoreNotEnough + 1;

		//CMD_GP_SetUserCongealStateError
		public static  const ErrCode_CMD_GP_SetUserCongealStateError_InvalidParam=ErrCode_CMD_GP_SetUserScoreError_AccountScoreNotEnough + 1;
		public static  const ErrCode_CMD_GP_SetUserCongealStateError_Unknow=ErrCode_CMD_GP_SetUserCongealStateError_InvalidParam + 1;

		//CMD_GP_SetBasicUserInfoError
		public static  const ErrCode_CMD_GP_SetBasicUserInfoError_PasswdEmpty=ErrCode_CMD_GP_SetUserCongealStateError_Unknow + 1;
		public static  const ErrCode_CMD_GP_SetBasicUserInfoError_NameEmpty=ErrCode_CMD_GP_SetBasicUserInfoError_PasswdEmpty + 1;

		//CMD_GP_SetDetailUserInfoError
		public static  const ErrCode_CMD_GP_SetDetailUserInfoError_OccupancyRateInvalid=ErrCode_CMD_GP_SetBasicUserInfoError_NameEmpty + 1;
		public static  const ErrCode_CMD_GP_SetDetailUserInfoError_LowOccupancyRate_MaxInvalid=ErrCode_CMD_GP_SetDetailUserInfoError_OccupancyRateInvalid + 1;
		public static  const ErrCode_CMD_GP_SetDetailUserInfoError_LowOccupancyRate_MinInvalid=ErrCode_CMD_GP_SetDetailUserInfoError_LowOccupancyRate_MaxInvalid + 1;
		public static  const ErrCode_CMD_GP_SetDetailUserInfoError_RollbackRateInvalid=ErrCode_CMD_GP_SetDetailUserInfoError_LowOccupancyRate_MinInvalid + 1;
		public static  const ErrCode_CMD_GP_SetDetailUserInfoError_AccoutNotExist=ErrCode_CMD_GP_SetDetailUserInfoError_RollbackRateInvalid + 1;
		public static  const ErrCode_CMD_GP_SetDetailUserInfoError_ParentNotExist=ErrCode_CMD_GP_SetDetailUserInfoError_AccoutNotExist + 1;
		public static  const ErrCode_CMD_GP_SetDetailUserInfoError_ORMTParentOR=ErrCode_CMD_GP_SetDetailUserInfoError_ParentNotExist + 1;
		public static  const ErrCode_CMD_GP_SetDetailUserInfoError_ORMTParentLORMax=ErrCode_CMD_GP_SetDetailUserInfoError_ORMTParentOR + 1;
		public static  const ErrCode_CMD_GP_SetDetailUserInfoError_ORLTParentLORMin=ErrCode_CMD_GP_SetDetailUserInfoError_ORMTParentLORMax + 1;
		public static  const ErrCode_CMD_GP_SetDetailUserInfoError_LORMaxMTParentLORMax=ErrCode_CMD_GP_SetDetailUserInfoError_ORLTParentLORMin + 1;
		public static  const ErrCode_CMD_GP_SetDetailUserInfoError_LORMinMTLORMax=ErrCode_CMD_GP_SetDetailUserInfoError_LORMaxMTParentLORMax + 1;
		public static  const ErrCode_CMD_GP_SetDetailUserInfoError_RollbackRateMTParentRollbackRate=ErrCode_CMD_GP_SetDetailUserInfoError_LORMinMTLORMax + 1;
		public static  const ErrCode_CMD_GP_SetDetailUserInfoError_Unknow=ErrCode_CMD_GP_SetDetailUserInfoError_RollbackRateMTParentRollbackRate + 1;

		//CMD_GP_SetUserRightError
		public static  const ErrCode_CMD_GP_SetUserRightError_AccoutNotExist=ErrCode_CMD_GP_SetDetailUserInfoError_Unknow + 1;
		public static  const ErrCode_CMD_GP_SetUserRightError_Unknow=ErrCode_CMD_GP_SetUserRightError_AccoutNotExist + 1;

		//CMD_GP_SetMyselfBasicUserInfoError
		public static  const ErrCode_CMD_GP_SetMyselfBasicUserInfoError_PasswdEmpty=ErrCode_CMD_GP_SetUserRightError_Unknow + 1;
		public static  const ErrCode_CMD_GP_SetMyselfBasicUserInfoError_NameEmpty=ErrCode_CMD_GP_SetMyselfBasicUserInfoError_PasswdEmpty + 1;
		public static  const ErrCode_CMD_GP_SetMyselfBasicUserInfoError_AccoutNotExist=ErrCode_CMD_GP_SetMyselfBasicUserInfoError_NameEmpty + 1;
		public static  const ErrCode_CMD_GP_SetMyselfBasicUserInfoError_PasswdNotMatch=ErrCode_CMD_GP_SetMyselfBasicUserInfoError_AccoutNotExist + 1;
		public static  const ErrCode_CMD_GP_SetMyselfBasicUserInfoError_Unknow=ErrCode_CMD_GP_SetMyselfBasicUserInfoError_PasswdNotMatch + 1;

		//CMD_GP_RegisterSubUserError
		public static  const ErrCode_CMD_GP_RegisterSubUserError_PasswdEmpty=ErrCode_CMD_GP_SetMyselfBasicUserInfoError_Unknow + 1;
		public static  const ErrCode_CMD_GP_RegisterSubUserError_AccountEmpty=ErrCode_CMD_GP_RegisterSubUserError_PasswdEmpty + 1;
		public static  const ErrCode_CMD_GP_RegisterSubUserError_NameEmpty=ErrCode_CMD_GP_RegisterSubUserError_AccountEmpty + 1;
		public static  const ErrCode_CMD_GP_RegisterSubUserError_AccountExist=ErrCode_CMD_GP_RegisterSubUserError_NameEmpty + 1;
		public static  const ErrCode_CMD_GP_RegisterSubUserError_ParentNotExist=ErrCode_CMD_GP_RegisterSubUserError_AccountExist + 1;

		//CMD_GP_SetSubUserCongealStateError
		public static  const ErrCode_CMD_GP_SetSubUserCongealStateError_AccoutNotExist=ErrCode_CMD_GP_RegisterSubUserError_ParentNotExist + 1;
		public static  const ErrCode_CMD_GP_SetSubUserCongealStateError_Unknow=ErrCode_CMD_GP_SetSubUserCongealStateError_AccoutNotExist + 1;

		//CMD_GP_SetBasicSubUserInfoError
		public static  const ErrCode_CMD_GP_SetBasicSubUserInfoError_PasswdEmpty=ErrCode_CMD_GP_SetSubUserCongealStateError_Unknow + 1;
		public static  const ErrCode_CMD_GP_SetBasicSubUserInfoError_NameEmpty=ErrCode_CMD_GP_SetBasicSubUserInfoError_PasswdEmpty + 1;
		public static  const ErrCode_CMD_GP_SetBasicSubUserInfoError_AccoutNotExist=ErrCode_CMD_GP_SetBasicSubUserInfoError_NameEmpty + 1;
		public static  const ErrCode_CMD_GP_SetBasicSubUserInfoError_Unknow=ErrCode_CMD_GP_SetBasicSubUserInfoError_AccoutNotExist + 1;

		//CMD_GP_OperationListError
		public static  const ErrCode_CMD_GP_OperationListError_AccountEmpty=ErrCode_CMD_GP_SetBasicSubUserInfoError_Unknow + 1;
		public static  const ErrCode_CMD_GP_OperationListError_InvalidTime=ErrCode_CMD_GP_OperationListError_AccountEmpty + 1;
		public static  const ErrCode_CMD_GP_OperationListError_InvalidPageSize=ErrCode_CMD_GP_OperationListError_InvalidTime + 1;

		//CMD_GP_ChartViewOneListError
		public static  const ErrCode_CMD_GP_ChartViewOneListError_AccountEmpty=ErrCode_CMD_GP_OperationListError_InvalidPageSize + 1;
		public static  const ErrCode_CMD_GP_ChartViewOneListError_InvalidTime=ErrCode_CMD_GP_ChartViewOneListError_AccountEmpty + 1;

		//CMD_GP_ChartViewTwoListError
		public static  const ErrCode_CMD_GP_ChartViewTwoListError_AccountEmpty=ErrCode_CMD_GP_ChartViewOneListError_InvalidTime + 1;
		public static  const ErrCode_CMD_GP_ChartViewTwoListError_InvalidTime=ErrCode_CMD_GP_ChartViewTwoListError_AccountEmpty + 1;
		public static  const ErrCode_CMD_GP_ChartViewTwoListError_InvalidPageSize=ErrCode_CMD_GP_ChartViewTwoListError_InvalidTime + 1;

		//CMD_GP_BetHistoryListError
		public static  const ErrCode_CMD_GP_BetHistoryListError_AccountEmpty=ErrCode_CMD_GP_ChartViewTwoListError_InvalidPageSize + 1;
		public static  const ErrCode_CMD_GP_BetHistoryListError_InvalidTime=ErrCode_CMD_GP_BetHistoryListError_AccountEmpty + 1;
		public static  const ErrCode_CMD_GP_BetHistoryListError_InvalidPageSize=ErrCode_CMD_GP_BetHistoryListError_InvalidTime + 1;

		//CMD_GR_SitFailed
		public static  const ErrCode_CMD_GR_SitFailed_NoFindTable=0xee00;
		public static  const ErrCode_CMD_GR_SitFailed_AlreadySit=ErrCode_CMD_GR_SitFailed_NoFindTable + 1;
		public static  const ErrCode_CMD_GR_SitFailed_NoEnoughRight=ErrCode_CMD_GR_SitFailed_AlreadySit + 1;
		public static  const ErrCode_CMD_GR_SitFailed_RoomShallClose=ErrCode_CMD_GR_SitFailed_NoEnoughRight + 1;
		public static  const ErrCode_CMD_GR_SitFailed_AllowEnterGame=ErrCode_CMD_GR_SitFailed_RoomShallClose + 1;
		public static  const ErrCode_CMD_GR_SitFailed_NoLookonUser=ErrCode_CMD_GR_SitFailed_AllowEnterGame + 1;
		public static  const ErrCode_CMD_GR_SitFailed_Password=ErrCode_CMD_GR_SitFailed_NoLookonUser + 1;
		public static  const ErrCode_CMD_GR_SitFailed_PlayingGame=ErrCode_CMD_GR_SitFailed_Password + 1;
		public static  const ErrCode_CMD_GR_SitFailed_OtherSitFirstly=ErrCode_CMD_GR_SitFailed_PlayingGame + 1;
		public static  const ErrCode_CMD_GR_SitFailed_NoEnoughScore=ErrCode_CMD_GR_SitFailed_OtherSitFirstly + 1;
		public static  const ErrCode_CMD_GR_SitFailed_TooManyScore=ErrCode_CMD_GR_SitFailed_NoEnoughScore + 1;
		public static  const ErrCode_CMD_GR_SitFailed_SameIP=ErrCode_CMD_GR_SitFailed_TooManyScore + 1;

		//message error code
		public static  const ErrCode_Message_ShallCloseRoom=0xf000;
		public static  const ErrCode_Message_AllowEnterRoom=ErrCode_Message_ShallCloseRoom + 1;
		public static  const ErrCode_Message_RoomFull=ErrCode_Message_AllowEnterRoom + 1;
		public static  const ErrCode_Message_AnotherUserLogon=ErrCode_Message_RoomFull + 1;
		public static  const ErrCode_Message_AllowRoomChat=ErrCode_Message_AnotherUserLogon + 1;
		public static  const ErrCode_Message_AllowWisper=ErrCode_Message_AllowRoomChat + 1;
		public static  const ErrCode_Message_OptionServer=ErrCode_Message_AllowWisper + 1;
		public static  const ErrCode_Message_CloseRoom=ErrCode_Message_OptionServer + 1;
		public static  const ErrCode_Message_CanntStandup=ErrCode_Message_CloseRoom + 1;
		public static  const ErrCode_Message_KickUser=ErrCode_Message_CanntStandup + 1;
		public static  const ErrCode_Message_ForceUserLogout=ErrCode_Message_KickUser+1;
		public static  const ErrCode_Message_LessScore=ErrCode_Message_ForceUserLogout + 1;

		//common
		public static  const ErrCode_AccountNotExist=0xfd00;
		public static  const ErrCode_LimitRight=0xfe00;
		public static  const ErrCode_Database=0xff00;

		private static  var m_ErrorMap:HashMap;
		private static  function Setup():void
		{
			m_ErrorMap=new HashMap  ;

			//CMD_GP_LogonError
			m_ErrorMap.put(ErrCode_CMD_GP_LogonError_AccountNoExist,"帐号不存在,请查证后再次尝试登入");
			m_ErrorMap.put(ErrCode_CMD_GP_LogonError_Password,"密码输入有误,请查证后再次尝试登入");
			m_ErrorMap.put(ErrCode_CMD_GP_LogonError_AccountFreeze,"帐户暂时处于停用状态,请联系客户服务中心了解详细情况");
			m_ErrorMap.put(ErrCode_CMD_GP_LogonError_AlreadyLogon,"帐户已经在别处登陆,如有疑问请联系客户服务中心了解详细情况");
		
			//CMD_GP_RegisterError
			m_ErrorMap.put(ErrCode_CMD_GP_RegisterError_PasswdEmpty,"密码不能为空,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_RegisterError_AccountEmpty,"帐号不能为空,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_RegisterError_NameEmpty,"名字不能为空,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_RegisterError_LevelInvalid,"用户级别无效,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_RegisterError_OccupancyRateInvalid,"占成比无效,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_RegisterError_LowOccupancyRate_MaxInvalid,"下线最大占成比无效,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_RegisterError_LowOccupancyRate_MinInvalid,"下线最小占成比无效,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_RegisterError_RollbackRateInvalid,"洗码比无效,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_RegisterError_UserScoreInvalid,"信用额度无效,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_RegisterError_AccoutExist,"该帐号已经存在,请尝试其他帐号");
			m_ErrorMap.put(ErrCode_CMD_GP_RegisterError_ParentNotExist,"上线帐号不存在,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_RegisterError_ParentScoreNotEnough,"上线帐号额度不够,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_RegisterError_ORMTParentOR,"占成比不能大于上线的占成比,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_RegisterError_ORMTParentLORMax,"占成比不能大于上线的占成比,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_RegisterError_ORLTParentLORMin,"占成比不能小于上线的下线最低占成比,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_RegisterError_LORMaxMTParentLORMax,"下线最高占成比不能大于上线的下线最高占成比,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_RegisterError_LORMinMTLORMax,"下线最低占成比不能大于下线最高占成比,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_RegisterError_RollbackRateMTParentRollbackRate,"洗码比不能大于上线的洗码比,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_RegisterError_Unknow,"新建帐号失败,发生未知错误,若需要帮助,请联系游戏客服咨询");
			//CMD_GP_SetUserScoreError
			m_ErrorMap.put(ErrCode_CMD_GP_SetUserScoreError_ScoreNegative,"输入的额度无效,若需要帮助,请联系游戏客服咨询");
			m_ErrorMap.put(ErrCode_CMD_GP_SetUserScoreError_AccoutNotExist,"该被存取额度的帐号不存在,请输入其他帐号");
			m_ErrorMap.put(ErrCode_CMD_GP_SetUserScoreError_ParentNotExist,"该被存取额度的帐号上线帐号不存在,请输入其他帐号");
			m_ErrorMap.put(ErrCode_CMD_GP_SetUserScoreError_ParentScoreNotEnough,"上线帐号额度不够,请输入正确的存入额度");
			m_ErrorMap.put(ErrCode_CMD_GP_SetUserScoreError_AccountScoreNotEnough,"该帐号额度不够,请输入正确的取出额度");
			//CMD_GP_SetUserCongealStateError
			m_ErrorMap.put(ErrCode_CMD_GP_SetUserCongealStateError_InvalidParam,"参数无效,若需要帮助,请联系游戏客服咨询");
			m_ErrorMap.put(ErrCode_CMD_GP_SetUserCongealStateError_Unknow,"操作失败,若需要帮助,请联系游戏客服咨询");
			//CMD_GP_SetBasicUserInfoError
			m_ErrorMap.put(ErrCode_CMD_GP_SetBasicUserInfoError_PasswdEmpty,"密码不能为空,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_SetBasicUserInfoError_NameEmpty,"名字不能为空,请查证后再次尝试");
			//CMD_GP_SetDetailUserInfoError
			m_ErrorMap.put(ErrCode_CMD_GP_SetDetailUserInfoError_OccupancyRateInvalid,"占成比无效,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_SetDetailUserInfoError_LowOccupancyRate_MaxInvalid,"下线最大占成比无效,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_SetDetailUserInfoError_LowOccupancyRate_MinInvalid,"下线最小占成比无效,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_SetDetailUserInfoError_RollbackRateInvalid,"洗码比无效,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_SetDetailUserInfoError_AccoutNotExist,"该帐号不存在,请尝试其他帐号");
			m_ErrorMap.put(ErrCode_CMD_GP_SetDetailUserInfoError_ParentNotExist,"上线帐号不存在,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_SetDetailUserInfoError_ORMTParentOR,"占成比不能大于上线的占成比,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_SetDetailUserInfoError_ORMTParentLORMax,"占成比不能大于上线的占成比,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_SetDetailUserInfoError_ORLTParentLORMin,"占成比不能小于上线的下线最低占成比,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_SetDetailUserInfoError_LORMaxMTParentLORMax,"下线最高占成比不能大于上线的下线最高占成比,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_SetDetailUserInfoError_LORMinMTLORMax,"下线最低占成比不能大于下线最高占成比,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_SetDetailUserInfoError_RollbackRateMTParentRollbackRate,"洗码比不能大于上线的洗码比,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_SetDetailUserInfoError_Unknow,"修改详细资料失败,发生未知错误,若需要帮助,请联系游戏客服咨询");
			//CMD_GP_SetUserRightError
			m_ErrorMap.put(ErrCode_CMD_GP_SetUserRightError_AccoutNotExist,"帐号不存在,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_SetUserRightError_Unknow,"修改权限失败,发生未知错误,若需要帮助,请联系游戏客服咨询");
			//CMD_GP_SetMyselfBasicUserInfoError
			m_ErrorMap.put(ErrCode_CMD_GP_SetMyselfBasicUserInfoError_PasswdEmpty,"密码不能为空,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_SetMyselfBasicUserInfoError_NameEmpty,"名字不能为空,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_SetMyselfBasicUserInfoError_AccoutNotExist,"该帐号不存在,请尝试其他帐号");
			m_ErrorMap.put(ErrCode_CMD_GP_SetMyselfBasicUserInfoError_PasswdNotMatch,"原密码错误,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_SetMyselfBasicUserInfoError_Unknow,"修改失败,发生未知错误,若需要帮助,请联系游戏客服咨询");

			//CMD_GP_RegisterSubUserError
			m_ErrorMap.put(ErrCode_CMD_GP_RegisterSubUserError_PasswdEmpty,"密码不能为空,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_RegisterSubUserError_AccountEmpty,"帐号不能为空,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_RegisterSubUserError_NameEmpty,"名字不能为空,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_RegisterSubUserError_AccountExist,"该帐号已经存在,请输入其他帐号");
			m_ErrorMap.put(ErrCode_CMD_GP_RegisterSubUserError_ParentNotExist,"父帐号不存在,请查证后再次尝试");
			//CMD_GP_SetSubUserCongealStateError
			m_ErrorMap.put(ErrCode_CMD_GP_SetSubUserCongealStateError_AccoutNotExist,"帐号不存在,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_SetSubUserCongealStateError_Unknow,"操作失败,若需要帮助,请联系游戏客服咨询");
			//CMD_GP_SetBasicSubUserInfoError
			m_ErrorMap.put(ErrCode_CMD_GP_SetBasicSubUserInfoError_PasswdEmpty,"密码不能为空,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_SetBasicSubUserInfoError_NameEmpty,"帐号不能为空,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_SetBasicSubUserInfoError_AccoutNotExist,"该帐号已经存在,请输入其他帐号");
			m_ErrorMap.put(ErrCode_CMD_GP_SetBasicSubUserInfoError_Unknow,"设定基本信息操作失败,若需要帮助,请联系游戏客服咨询");
			//CMD_GP_OperationListError
			m_ErrorMap.put(ErrCode_CMD_GP_OperationListError_AccountEmpty,"帐号不能为空,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_OperationListError_InvalidTime,"无效查询时间,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_OperationListError_InvalidPageSize,"无效查询页大小,请查证后再次尝试");
			//CMD_GP_ChartViewOneListError
			m_ErrorMap.put(ErrCode_CMD_GP_ChartViewOneListError_AccountEmpty,"帐号不能为空,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_ChartViewOneListError_InvalidTime,"无效查询时间,请查证后再次尝试");
			//CMD_GP_ChartViewTwoListError
			m_ErrorMap.put(ErrCode_CMD_GP_ChartViewTwoListError_AccountEmpty,"帐号不能为空,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_ChartViewTwoListError_InvalidTime,"无效查询时间,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_ChartViewTwoListError_InvalidPageSize,"无效查询页大小,请查证后再次尝试");
			//CMD_GP_BetHistoryListError
			m_ErrorMap.put(ErrCode_CMD_GP_BetHistoryListError_AccountEmpty,"帐号不能为空,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_BetHistoryListError_InvalidTime,"无效查询时间,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_CMD_GP_BetHistoryListError_InvalidPageSize,"无效查询页大小,请查证后再次尝试");

			//CMD_GR_SitFailed
			m_ErrorMap.put(ErrCode_CMD_GR_SitFailed_NoFindTable,"找不到空的游戏位置,请稍侯再试");
			m_ErrorMap.put(ErrCode_CMD_GR_SitFailed_AlreadySit,"您已经正在游戏中,不能更换位置");
			m_ErrorMap.put(ErrCode_CMD_GR_SitFailed_NoEnoughRight,"抱歉,你没有进行旁观游戏的权限,若需要帮助,请联系游戏客服咨询");
			m_ErrorMap.put(ErrCode_CMD_GR_SitFailed_RoomShallClose,"由于此游戏房间即将暂停服务,玩家不允许再进入游戏桌");
			m_ErrorMap.put(ErrCode_CMD_GR_SitFailed_AllowEnterGame,"抱歉,此游戏桌现在不允许玩家进入");
			m_ErrorMap.put(ErrCode_CMD_GR_SitFailed_NoLookonUser,"所请求旁观的位置已经没有玩家了,不能旁观");
			m_ErrorMap.put(ErrCode_CMD_GR_SitFailed_Password,"游戏桌携带密码错误,不能旁观游戏");
			m_ErrorMap.put(ErrCode_CMD_GR_SitFailed_PlayingGame,"游戏还没有开始,暂时不能旁观");
			m_ErrorMap.put(ErrCode_CMD_GR_SitFailed_OtherSitFirstly,"椅子已经被他人捷足先登了,下次动作要快点");
			m_ErrorMap.put(ErrCode_CMD_GR_SitFailed_NoEnoughScore,"您的额度不够,不能加入");
			m_ErrorMap.put(ErrCode_CMD_GR_SitFailed_TooManyScore,"您的额度太高,不能加入");
			m_ErrorMap.put(ErrCode_CMD_GR_SitFailed_SameIP,"此游戏桌存在与你 IP 地址相同的玩家,不能加入游戏");

			//Message
			m_ErrorMap.put(ErrCode_Message_ShallCloseRoom,"请注意，由于需要进行游戏房间维护更新工作，此游戏房间即将暂停服务");
			m_ErrorMap.put(ErrCode_Message_AllowEnterRoom,"抱歉，由于系统维护的原因，本游戏房间不允许任何玩家登录进入");
			m_ErrorMap.put(ErrCode_Message_RoomFull,"抱歉，本游戏房间已经满人");
			m_ErrorMap.put(ErrCode_Message_AnotherUserLogon,"请注意，你的帐号在另一地方进入此游戏房间了，你被迫离开");
			m_ErrorMap.put(ErrCode_Message_AllowRoomChat,"抱歉，本游戏房间不允许进行大厅公聊");
			m_ErrorMap.put(ErrCode_Message_AllowWisper,"抱歉，本游戏房间不能进行玩家私聊");
			m_ErrorMap.put(ErrCode_Message_OptionServer,"服务器房间配置参数选项设置成功");
			m_ErrorMap.put(ErrCode_Message_CloseRoom,"请注意，游戏房间即将关闭或者不允许玩家进入，请你离开游戏桌子");
			m_ErrorMap.put(ErrCode_Message_CanntStandup,"你正在游戏桌游戏，暂时不能离开位置");
			m_ErrorMap.put(ErrCode_Message_KickUser,"抱歉，您被管理员踢出房间，若有任何疑问，请联系游戏客服");
			m_ErrorMap.put(ErrCode_Message_ForceUserLogout,"抱歉，您被管理员强制登出，若有任何疑问，请联系游戏客服");
			m_ErrorMap.put(ErrCode_Message_LessScore,"你的游戏额度少于游戏入场卷，不能继续游戏");

			//common
			m_ErrorMap.put(ErrCode_AccountNotExist,"帐号不能为空,请查证后再次尝试");
			m_ErrorMap.put(ErrCode_LimitRight,"您的权限不够,若需要帮助,请联系游戏客服咨询");
			m_ErrorMap.put(ErrCode_Database,"由于数据库操作异常,若需要帮助,请联系游戏客服咨询");
		}
		public static  function GetErrorDescribe(wErrCode:uint):String
		{
			if (m_ErrorMap == null)
			{
				Setup();
			}
			var strError:String=m_ErrorMap.get(wErrCode);
			if (strError)
			{
				return strError;
			}
			else
			{
				return "发生操作异常，请联系客户服务中心了解详细情况";
			}
		}
	}
}