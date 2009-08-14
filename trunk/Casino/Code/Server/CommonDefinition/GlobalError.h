#pragma	  once
//CMD_GP_LogonError
const WORD ErrCode_CMD_GP_LogonError_AccountNoExist		=   1;
const WORD ErrCode_CMD_GP_LogonError_Password  			=	ErrCode_CMD_GP_LogonError_AccountNoExist + 1;
const WORD ErrCode_CMD_GP_LogonError_AccountFreeze		=	ErrCode_CMD_GP_LogonError_Password + 1;
const WORD ErrCode_CMD_GP_LogonError_AlreadyLogon		=	ErrCode_CMD_GP_LogonError_AccountFreeze + 1;

//CMD_GP_RegisterError
const WORD ErrCode_CMD_GP_RegisterError_PasswdEmpty		=	ErrCode_CMD_GP_LogonError_AlreadyLogon + 1;
const WORD ErrCode_CMD_GP_RegisterError_AccountEmpty		=	ErrCode_CMD_GP_RegisterError_PasswdEmpty + 1;
const WORD ErrCode_CMD_GP_RegisterError_NameEmpty		=	ErrCode_CMD_GP_RegisterError_AccountEmpty + 1;
const WORD ErrCode_CMD_GP_RegisterError_LevelInvalid		=	ErrCode_CMD_GP_RegisterError_NameEmpty + 1;
const WORD ErrCode_CMD_GP_RegisterError_OccupancyRateInvalid		=	ErrCode_CMD_GP_RegisterError_LevelInvalid + 1;
const WORD ErrCode_CMD_GP_RegisterError_LowOccupancyRate_MaxInvalid=	ErrCode_CMD_GP_RegisterError_OccupancyRateInvalid + 1;
const WORD ErrCode_CMD_GP_RegisterError_LowOccupancyRate_MinInvalid=	ErrCode_CMD_GP_RegisterError_LowOccupancyRate_MaxInvalid + 1;
const WORD ErrCode_CMD_GP_RegisterError_RollbackRateInvalid		=	ErrCode_CMD_GP_RegisterError_LowOccupancyRate_MinInvalid + 1;
const WORD ErrCode_CMD_GP_RegisterError_UserScoreInvalid		=	ErrCode_CMD_GP_RegisterError_RollbackRateInvalid + 1;
const WORD ErrCode_CMD_GP_RegisterError_AccoutExist			=	ErrCode_CMD_GP_RegisterError_UserScoreInvalid + 1;
const WORD ErrCode_CMD_GP_RegisterError_ParentNotExist			=	ErrCode_CMD_GP_RegisterError_AccoutExist + 1;
const WORD ErrCode_CMD_GP_RegisterError_ParentScoreNotEnough	=	ErrCode_CMD_GP_RegisterError_ParentNotExist + 1;
const WORD ErrCode_CMD_GP_RegisterError_ORMTParentOR		=	ErrCode_CMD_GP_RegisterError_ParentScoreNotEnough + 1;
const WORD ErrCode_CMD_GP_RegisterError_ORMTParentLORMax	=	ErrCode_CMD_GP_RegisterError_ORMTParentOR + 1;
const WORD ErrCode_CMD_GP_RegisterError_ORLTParentLORMin	=	ErrCode_CMD_GP_RegisterError_ORMTParentLORMax + 1;
const WORD ErrCode_CMD_GP_RegisterError_LORMaxMTParentLORMax	=	ErrCode_CMD_GP_RegisterError_ORLTParentLORMin + 1;
const WORD ErrCode_CMD_GP_RegisterError_LORMinMTLORMax		=	ErrCode_CMD_GP_RegisterError_LORMaxMTParentLORMax + 1;
const WORD ErrCode_CMD_GP_RegisterError_RollbackRateMTParentRollbackRate	=	ErrCode_CMD_GP_RegisterError_LORMinMTLORMax + 1;
const WORD ErrCode_CMD_GP_RegisterError_Unknow				=	ErrCode_CMD_GP_RegisterError_RollbackRateMTParentRollbackRate + 1;

//CMD_GP_SetUserScoreError
const WORD ErrCode_CMD_GP_SetUserScoreError_ScoreNegative		= ErrCode_CMD_GP_RegisterError_Unknow + 1;
const WORD ErrCode_CMD_GP_SetUserScoreError_AccoutNotExist		= ErrCode_CMD_GP_SetUserScoreError_ScoreNegative + 1;
const WORD ErrCode_CMD_GP_SetUserScoreError_ParentNotExist		= ErrCode_CMD_GP_SetUserScoreError_AccoutNotExist + 1;
const WORD ErrCode_CMD_GP_SetUserScoreError_ParentScoreNotEnough	= ErrCode_CMD_GP_SetUserScoreError_ParentNotExist + 1;
const WORD ErrCode_CMD_GP_SetUserScoreError_AccountScoreNotEnough= ErrCode_CMD_GP_SetUserScoreError_ParentScoreNotEnough + 1;

//CMD_GP_SetUserCongealStateError
const WORD ErrCode_CMD_GP_SetUserCongealStateError_InvalidParam	= ErrCode_CMD_GP_SetUserScoreError_AccountScoreNotEnough + 1;
const WORD ErrCode_CMD_GP_SetUserCongealStateError_Unknow		= ErrCode_CMD_GP_SetUserCongealStateError_InvalidParam + 1;

//CMD_GP_SetBasicUserInfoError
const WORD ErrCode_CMD_GP_SetBasicUserInfoError_PasswdEmpty	= ErrCode_CMD_GP_SetUserCongealStateError_Unknow + 1;
const WORD ErrCode_CMD_GP_SetBasicUserInfoError_NameEmpty	= ErrCode_CMD_GP_SetBasicUserInfoError_PasswdEmpty + 1;

//CMD_GP_SetDetailUserInfoError
const WORD ErrCode_CMD_GP_SetDetailUserInfoError_OccupancyRateInvalid		=	ErrCode_CMD_GP_SetBasicUserInfoError_NameEmpty + 1;
const WORD ErrCode_CMD_GP_SetDetailUserInfoError_LowOccupancyRate_MaxInvalid =	ErrCode_CMD_GP_SetDetailUserInfoError_OccupancyRateInvalid + 1;
const WORD ErrCode_CMD_GP_SetDetailUserInfoError_LowOccupancyRate_MinInvalid =	ErrCode_CMD_GP_SetDetailUserInfoError_LowOccupancyRate_MaxInvalid + 1;
const WORD ErrCode_CMD_GP_SetDetailUserInfoError_RollbackRateInvalid		=	ErrCode_CMD_GP_SetDetailUserInfoError_LowOccupancyRate_MinInvalid + 1;
const WORD ErrCode_CMD_GP_SetDetailUserInfoError_AccoutNotExist			=	ErrCode_CMD_GP_SetDetailUserInfoError_RollbackRateInvalid + 1;
const WORD ErrCode_CMD_GP_SetDetailUserInfoError_ParentNotExist			=	ErrCode_CMD_GP_SetDetailUserInfoError_AccoutNotExist + 1;
const WORD ErrCode_CMD_GP_SetDetailUserInfoError_ORMTParentOR		=	ErrCode_CMD_GP_SetDetailUserInfoError_ParentNotExist + 1;
const WORD ErrCode_CMD_GP_SetDetailUserInfoError_ORMTParentLORMax	=	ErrCode_CMD_GP_SetDetailUserInfoError_ORMTParentOR + 1;
const WORD ErrCode_CMD_GP_SetDetailUserInfoError_ORLTParentLORMin	=	ErrCode_CMD_GP_SetDetailUserInfoError_ORMTParentLORMax + 1;
const WORD ErrCode_CMD_GP_SetDetailUserInfoError_LORMaxMTParentLORMax	=	ErrCode_CMD_GP_SetDetailUserInfoError_ORLTParentLORMin + 1;
const WORD ErrCode_CMD_GP_SetDetailUserInfoError_LORMinMTLORMax		=	ErrCode_CMD_GP_SetDetailUserInfoError_LORMaxMTParentLORMax + 1;
const WORD ErrCode_CMD_GP_SetDetailUserInfoError_RollbackRateMTParentRollbackRate	=	ErrCode_CMD_GP_SetDetailUserInfoError_LORMinMTLORMax + 1;
const WORD ErrCode_CMD_GP_SetDetailUserInfoError_Unknow				=	ErrCode_CMD_GP_SetDetailUserInfoError_RollbackRateMTParentRollbackRate + 1;

//CMD_GP_SetUserRightError
const WORD ErrCode_CMD_GP_SetUserRightError_AccoutNotExist		=	ErrCode_CMD_GP_SetDetailUserInfoError_Unknow + 1;
const WORD ErrCode_CMD_GP_SetUserRightError_Unknow				=	ErrCode_CMD_GP_SetUserRightError_AccoutNotExist + 1;

//CMD_GP_SetMyselfBasicUserInfoError
const WORD ErrCode_CMD_GP_SetMyselfBasicUserInfoError_PasswdEmpty	= ErrCode_CMD_GP_SetUserRightError_Unknow + 1;
const WORD ErrCode_CMD_GP_SetMyselfBasicUserInfoError_NameEmpty	= ErrCode_CMD_GP_SetMyselfBasicUserInfoError_PasswdEmpty + 1;
const WORD ErrCode_CMD_GP_SetMyselfBasicUserInfoError_AccoutNotExist	= ErrCode_CMD_GP_SetMyselfBasicUserInfoError_NameEmpty + 1;
const WORD ErrCode_CMD_GP_SetMyselfBasicUserInfoError_PasswdNotMatch		= ErrCode_CMD_GP_SetMyselfBasicUserInfoError_AccoutNotExist + 1;
const WORD ErrCode_CMD_GP_SetMyselfBasicUserInfoError_Unknow				=	ErrCode_CMD_GP_SetMyselfBasicUserInfoError_PasswdNotMatch + 1;

//CMD_GP_RegisterSubUserError
const WORD ErrCode_CMD_GP_RegisterSubUserError_PasswdEmpty		=	ErrCode_CMD_GP_SetMyselfBasicUserInfoError_Unknow + 1;
const WORD ErrCode_CMD_GP_RegisterSubUserError_AccountEmpty		=	ErrCode_CMD_GP_RegisterSubUserError_PasswdEmpty + 1;
const WORD ErrCode_CMD_GP_RegisterSubUserError_NameEmpty			=	ErrCode_CMD_GP_RegisterSubUserError_AccountEmpty + 1;
const WORD ErrCode_CMD_GP_RegisterSubUserError_AccountExist		=	ErrCode_CMD_GP_RegisterSubUserError_NameEmpty + 1;
const WORD ErrCode_CMD_GP_RegisterSubUserError_ParentNotExist	=	ErrCode_CMD_GP_RegisterSubUserError_AccountExist + 1;

//CMD_GP_SetSubUserCongealStateError
const WORD ErrCode_CMD_GP_SetSubUserCongealStateError_AccoutNotExist	= ErrCode_CMD_GP_RegisterSubUserError_ParentNotExist + 1;
const WORD ErrCode_CMD_GP_SetSubUserCongealStateError_Unknow			= ErrCode_CMD_GP_SetSubUserCongealStateError_AccoutNotExist + 1;

//CMD_GP_SetBasicSubUserInfoError
const WORD ErrCode_CMD_GP_SetBasicSubUserInfoError_PasswdEmpty		= ErrCode_CMD_GP_SetSubUserCongealStateError_Unknow + 1;
const WORD ErrCode_CMD_GP_SetBasicSubUserInfoError_NameEmpty		= ErrCode_CMD_GP_SetBasicSubUserInfoError_PasswdEmpty + 1;
const WORD ErrCode_CMD_GP_SetBasicSubUserInfoError_AccoutNotExist	= ErrCode_CMD_GP_SetBasicSubUserInfoError_NameEmpty + 1;
const WORD ErrCode_CMD_GP_SetBasicSubUserInfoError_Unknow			= ErrCode_CMD_GP_SetBasicSubUserInfoError_AccoutNotExist + 1;

//CMD_GP_OperationListError
const WORD ErrCode_CMD_GP_OperationListError_AccountEmpty			= ErrCode_CMD_GP_SetBasicSubUserInfoError_Unknow + 1;
const WORD ErrCode_CMD_GP_OperationListError_InvalidTime			= ErrCode_CMD_GP_OperationListError_AccountEmpty + 1;
const WORD ErrCode_CMD_GP_OperationListError_InvalidPageSize	    = ErrCode_CMD_GP_OperationListError_InvalidTime + 1;

//CMD_GP_ChartViewOneListError
const WORD ErrCode_CMD_GP_ChartViewOneListError_AccountEmpty		= ErrCode_CMD_GP_OperationListError_InvalidPageSize + 1;
const WORD ErrCode_CMD_GP_ChartViewOneListError_InvalidTime			= ErrCode_CMD_GP_ChartViewOneListError_AccountEmpty + 1;

//CMD_GP_ChartViewTwoListError
const WORD ErrCode_CMD_GP_ChartViewTwoListError_AccountEmpty		= ErrCode_CMD_GP_ChartViewOneListError_InvalidTime + 1;
const WORD ErrCode_CMD_GP_ChartViewTwoListError_InvalidTime			= ErrCode_CMD_GP_ChartViewTwoListError_AccountEmpty + 1;
const WORD ErrCode_CMD_GP_ChartViewTwoListError_InvalidPageSize	    = ErrCode_CMD_GP_ChartViewTwoListError_InvalidTime + 1;

//CMD_GP_BetHistoryListError
const WORD ErrCode_CMD_GP_BetHistoryListError_AccountEmpty			= ErrCode_CMD_GP_ChartViewTwoListError_InvalidPageSize + 1;
const WORD ErrCode_CMD_GP_BetHistoryListError_InvalidTime			= ErrCode_CMD_GP_BetHistoryListError_AccountEmpty + 1;
const WORD ErrCode_CMD_GP_BetHistoryListError_InvalidPageSize	    = ErrCode_CMD_GP_BetHistoryListError_InvalidTime + 1;

//CMD_GR_SitFailed
const WORD ErrCode_CMD_GR_SitFailed_NoFindTable				=	0xee00;
const WORD ErrCode_CMD_GR_SitFailed_AlreadySit			    =	ErrCode_CMD_GR_SitFailed_NoFindTable + 1;
const WORD ErrCode_CMD_GR_SitFailed_NoEnoughRight			=	ErrCode_CMD_GR_SitFailed_AlreadySit + 1;
const WORD ErrCode_CMD_GR_SitFailed_RoomShallClose  			=	ErrCode_CMD_GR_SitFailed_NoEnoughRight + 1;
const WORD ErrCode_CMD_GR_SitFailed_AllowEnterGame  			=	ErrCode_CMD_GR_SitFailed_RoomShallClose + 1;
const WORD ErrCode_CMD_GR_SitFailed_NoLookonUser    			=	ErrCode_CMD_GR_SitFailed_AllowEnterGame + 1;
const WORD ErrCode_CMD_GR_SitFailed_Password	      			=	ErrCode_CMD_GR_SitFailed_NoLookonUser + 1;
const WORD ErrCode_CMD_GR_SitFailed_PlayingGame     			=	ErrCode_CMD_GR_SitFailed_Password + 1;
const WORD ErrCode_CMD_GR_SitFailed_OtherSitFirstly			=	ErrCode_CMD_GR_SitFailed_PlayingGame + 1;
const WORD ErrCode_CMD_GR_SitFailed_NoEnoughScore  			=	ErrCode_CMD_GR_SitFailed_OtherSitFirstly + 1;
const WORD ErrCode_CMD_GR_SitFailed_TooManyScore  			=	ErrCode_CMD_GR_SitFailed_NoEnoughScore + 1;
const WORD ErrCode_CMD_GR_SitFailed_SameIP		    		=	ErrCode_CMD_GR_SitFailed_TooManyScore + 1;

//message error code
const WORD ErrCode_Message_ShallCloseRoom					= 0xf000;
const WORD ErrCode_Message_AllowEnterRoom					= ErrCode_Message_ShallCloseRoom + 1;
const WORD ErrCode_Message_RoomFull							= ErrCode_Message_AllowEnterRoom + 1;
const WORD ErrCode_Message_AnotherUserLogon					= ErrCode_Message_RoomFull + 1;
const WORD ErrCode_Message_AllowRoomChat					= ErrCode_Message_AnotherUserLogon + 1;
const WORD ErrCode_Message_AllowWisper						= ErrCode_Message_AllowRoomChat + 1;
const WORD ErrCode_Message_OptionServer						= ErrCode_Message_AllowWisper + 1;
const WORD ErrCode_Message_CloseRoom						= ErrCode_Message_OptionServer + 1;
const WORD ErrCode_Message_CanntStandup						= ErrCode_Message_CloseRoom + 1;
const WORD ErrCode_Message_KickUser							= ErrCode_Message_CanntStandup + 1;
const WORD ErrCode_Message_ForceUserLogout					= ErrCode_Message_KickUser + 1;
const WORD ErrCode_Message_LessScore						= ErrCode_Message_ForceUserLogout + 1;

//common
const WORD ErrCode_Nothing									=	0xfc00;
const WORD ErrCode_AccountNotExist							=	0xfd00;
const WORD ErrCode_LimitRight								=	0xfe00;
const WORD ErrCode_Database									=	0xff00;

struct tagErrorPair
{
	WORD 	nCode;
	char*	szDescribe;
};

static const  tagErrorPair g_ErrorPair[] =
{
	//CMD_GP_LogonError
	{ErrCode_CMD_GP_LogonError_AccountNoExist, "�ʺŲ�����,���֤���ٴγ��Ե���"},
	{ErrCode_CMD_GP_LogonError_Password, "������������,���֤���ٴγ��Ե���"},
	{ErrCode_CMD_GP_LogonError_AccountFreeze, "�ʻ���ʱ����ͣ��״̬,����ϵ�ͻ����������˽���ϸ���"},
	{ErrCode_CMD_GP_LogonError_AlreadyLogon, "�ʻ��Ѿ��ڱ𴦵�½,������������ϵ�ͻ����������˽���ϸ���"},
	//CMD_GP_RegisterError
	{ErrCode_CMD_GP_RegisterError_PasswdEmpty, "���벻��Ϊ��,���֤���ٴγ���"},
	{ErrCode_CMD_GP_RegisterError_AccountEmpty, "�ʺŲ���Ϊ��,���֤���ٴγ���"},
	{ErrCode_CMD_GP_RegisterError_NameEmpty, "���ֲ���Ϊ��,���֤���ٴγ���"},
	{ErrCode_CMD_GP_RegisterError_LevelInvalid, "�û�������Ч,���֤���ٴγ���"},
	{ErrCode_CMD_GP_RegisterError_OccupancyRateInvalid, "ռ�ɱ���Ч,���֤���ٴγ���"},
	{ErrCode_CMD_GP_RegisterError_LowOccupancyRate_MaxInvalid, "�������ռ�ɱ���Ч,���֤���ٴγ���"},
	{ErrCode_CMD_GP_RegisterError_LowOccupancyRate_MinInvalid, "������Сռ�ɱ���Ч,���֤���ٴγ���"},
	{ErrCode_CMD_GP_RegisterError_RollbackRateInvalid, "ϴ�����Ч,���֤���ٴγ���"},
	{ErrCode_CMD_GP_RegisterError_UserScoreInvalid, "���ö����Ч,���֤���ٴγ���"},
	{ErrCode_CMD_GP_RegisterError_AccoutExist, "���ʺ��Ѿ�����,�볢�������ʺ�"},
	{ErrCode_CMD_GP_RegisterError_ParentNotExist, "�����ʺŲ�����,���֤���ٴγ���"},
	{ErrCode_CMD_GP_RegisterError_ParentScoreNotEnough, "�����ʺŶ�Ȳ���,���֤���ٴγ���"},
	{ErrCode_CMD_GP_RegisterError_ORMTParentOR, "ռ�ɱȲ��ܴ������ߵ�ռ�ɱ�,���֤���ٴγ���"},
	{ErrCode_CMD_GP_RegisterError_ORMTParentLORMax, "ռ�ɱȲ��ܴ������ߵ�ռ�ɱ�,���֤���ٴγ���"},
	{ErrCode_CMD_GP_RegisterError_ORLTParentLORMin, "ռ�ɱȲ���С�����ߵ��������ռ�ɱ�,���֤���ٴγ���"},
	{ErrCode_CMD_GP_RegisterError_LORMaxMTParentLORMax, "�������ռ�ɱȲ��ܴ������ߵ��������ռ�ɱ�,���֤���ٴγ���"},
	{ErrCode_CMD_GP_RegisterError_LORMinMTLORMax, "�������ռ�ɱȲ��ܴ����������ռ�ɱ�,���֤���ٴγ���"},
	{ErrCode_CMD_GP_RegisterError_RollbackRateMTParentRollbackRate, "ϴ��Ȳ��ܴ������ߵ�ϴ���,���֤���ٴγ���"},
	{ErrCode_CMD_GP_RegisterError_Unknow, "�½��ʺ�ʧ��,����δ֪����,����Ҫ����,����ϵ��Ϸ�ͷ���ѯ"},
	//CMD_GP_SetUserScoreError
	{ErrCode_CMD_GP_SetUserScoreError_ScoreNegative, "����Ķ����Ч,����Ҫ����,����ϵ��Ϸ�ͷ���ѯ"},
	{ErrCode_CMD_GP_SetUserScoreError_AccoutNotExist, "�ñ���ȡ��ȵ��ʺŲ�����,�����������ʺ�"},
	{ErrCode_CMD_GP_SetUserScoreError_ParentNotExist, "�ñ���ȡ��ȵ��ʺ������ʺŲ�����,�����������ʺ�"},
	{ErrCode_CMD_GP_SetUserScoreError_ParentScoreNotEnough, "�����ʺŶ�Ȳ���,��������ȷ�Ĵ�����"},
	{ErrCode_CMD_GP_SetUserScoreError_AccountScoreNotEnough, "���ʺŶ�Ȳ���,��������ȷ��ȡ�����"},
	//CMD_GP_SetUserCongealStateError
	{ErrCode_CMD_GP_SetUserCongealStateError_InvalidParam,"������Ч,����Ҫ����,����ϵ��Ϸ�ͷ���ѯ"},
	{ErrCode_CMD_GP_SetUserCongealStateError_Unknow,"����ʧ��,����Ҫ����,����ϵ��Ϸ�ͷ���ѯ"},
	//CMD_GP_SetBasicUserInfoError
	{ErrCode_CMD_GP_SetBasicUserInfoError_PasswdEmpty,"���벻��Ϊ��,���֤���ٴγ���"},
	{ErrCode_CMD_GP_SetBasicUserInfoError_NameEmpty,"���ֲ���Ϊ��,���֤���ٴγ���"},
	//CMD_GP_SetDetailUserInfoError
	{ErrCode_CMD_GP_SetDetailUserInfoError_OccupancyRateInvalid, "ռ�ɱ���Ч,���֤���ٴγ���"},
	{ErrCode_CMD_GP_SetDetailUserInfoError_LowOccupancyRate_MaxInvalid, "�������ռ�ɱ���Ч,���֤���ٴγ���"},
	{ErrCode_CMD_GP_SetDetailUserInfoError_LowOccupancyRate_MinInvalid, "������Сռ�ɱ���Ч,���֤���ٴγ���"},
	{ErrCode_CMD_GP_SetDetailUserInfoError_RollbackRateInvalid, "ϴ�����Ч,���֤���ٴγ���"},
	{ErrCode_CMD_GP_SetDetailUserInfoError_AccoutNotExist, "���ʺŲ�����,�볢�������ʺ�"},
	{ErrCode_CMD_GP_SetDetailUserInfoError_ParentNotExist, "�����ʺŲ�����,���֤���ٴγ���"},
	{ErrCode_CMD_GP_SetDetailUserInfoError_ORMTParentOR, "ռ�ɱȲ��ܴ������ߵ�ռ�ɱ�,���֤���ٴγ���"},
	{ErrCode_CMD_GP_SetDetailUserInfoError_ORMTParentLORMax, "ռ�ɱȲ��ܴ������ߵ�ռ�ɱ�,���֤���ٴγ���"},
	{ErrCode_CMD_GP_SetDetailUserInfoError_ORLTParentLORMin, "ռ�ɱȲ���С�����ߵ��������ռ�ɱ�,���֤���ٴγ���"},
	{ErrCode_CMD_GP_SetDetailUserInfoError_LORMaxMTParentLORMax, "�������ռ�ɱȲ��ܴ������ߵ��������ռ�ɱ�,���֤���ٴγ���"},
	{ErrCode_CMD_GP_SetDetailUserInfoError_LORMinMTLORMax, "�������ռ�ɱȲ��ܴ����������ռ�ɱ�,���֤���ٴγ���"},
	{ErrCode_CMD_GP_SetDetailUserInfoError_RollbackRateMTParentRollbackRate, "ϴ��Ȳ��ܴ������ߵ�ϴ���,���֤���ٴγ���"},
	{ErrCode_CMD_GP_SetDetailUserInfoError_Unknow, "�޸���ϸ����ʧ��,����δ֪����,����Ҫ����,����ϵ��Ϸ�ͷ���ѯ"},
	//CMD_GP_SetUserRightError
	{ErrCode_CMD_GP_SetUserRightError_AccoutNotExist, "�ʺŲ�����,���֤���ٴγ���"},
	{ErrCode_CMD_GP_SetUserRightError_Unknow, "�޸�Ȩ��ʧ��,����δ֪����,����Ҫ����,����ϵ��Ϸ�ͷ���ѯ"},
	//CMD_GP_SetMyselfBasicUserInfoError
	{ErrCode_CMD_GP_SetMyselfBasicUserInfoError_PasswdEmpty,"���벻��Ϊ��,���֤���ٴγ���"},
	{ErrCode_CMD_GP_SetMyselfBasicUserInfoError_NameEmpty,"���ֲ���Ϊ��,���֤���ٴγ���"},
	{ErrCode_CMD_GP_SetMyselfBasicUserInfoError_AccoutNotExist,"���ʺŲ�����,�볢�������ʺ�"},
	{ErrCode_CMD_GP_SetMyselfBasicUserInfoError_PasswdNotMatch,"ԭ�������,���֤���ٴγ���"},
	{ErrCode_CMD_GP_SetMyselfBasicUserInfoError_Unknow, "�޸�ʧ��,����δ֪����,����Ҫ����,����ϵ��Ϸ�ͷ���ѯ"},

	//CMD_GP_RegisterSubUserError
	{ErrCode_CMD_GP_RegisterSubUserError_PasswdEmpty, "���벻��Ϊ��,���֤���ٴγ���"},
	{ErrCode_CMD_GP_RegisterSubUserError_AccountEmpty, "�ʺŲ���Ϊ��,���֤���ٴγ���"},
	{ErrCode_CMD_GP_RegisterSubUserError_NameEmpty, "���ֲ���Ϊ��,���֤���ٴγ���"},
	{ErrCode_CMD_GP_RegisterSubUserError_AccountExist, "���ʺ��Ѿ�����,�����������ʺ�"},
	{ErrCode_CMD_GP_RegisterSubUserError_ParentNotExist, "���ʺŲ�����,���֤���ٴγ���"},
	//CMD_GP_SetSubUserCongealStateError
	{ErrCode_CMD_GP_SetSubUserCongealStateError_AccoutNotExist,"�ʺŲ�����,���֤���ٴγ���"},
	{ErrCode_CMD_GP_SetSubUserCongealStateError_Unknow,"����ʧ��,����Ҫ����,����ϵ��Ϸ�ͷ���ѯ"},
	//CMD_GP_SetBasicSubUserInfoError
	{ErrCode_CMD_GP_SetBasicSubUserInfoError_PasswdEmpty, "���벻��Ϊ��,���֤���ٴγ���"},
	{ErrCode_CMD_GP_SetBasicSubUserInfoError_NameEmpty, "�ʺŲ���Ϊ��,���֤���ٴγ���"},
	{ErrCode_CMD_GP_SetBasicSubUserInfoError_AccoutNotExist, "���ʺ��Ѿ�����,�����������ʺ�"},
	{ErrCode_CMD_GP_SetBasicSubUserInfoError_Unknow, "�趨������Ϣ����ʧ��,����Ҫ����,����ϵ��Ϸ�ͷ���ѯ"},
	//CMD_GP_OperationListError
	{ErrCode_CMD_GP_OperationListError_AccountEmpty, "�ʺŲ���Ϊ��,���֤���ٴγ���"},
	{ErrCode_CMD_GP_OperationListError_InvalidTime, "��Ч��ѯʱ��,���֤���ٴγ���"},
	{ErrCode_CMD_GP_OperationListError_InvalidPageSize, "��Ч��ѯҳ��С,���֤���ٴγ���"},
	//CMD_GP_ChartViewOneListError
	{ErrCode_CMD_GP_ChartViewOneListError_AccountEmpty, "�ʺŲ���Ϊ��,���֤���ٴγ���"},
	{ErrCode_CMD_GP_ChartViewOneListError_InvalidTime, "��Ч��ѯʱ��,���֤���ٴγ���"},
	//CMD_GP_ChartViewTwoListError
	{ErrCode_CMD_GP_ChartViewTwoListError_AccountEmpty, "�ʺŲ���Ϊ��,���֤���ٴγ���"},
	{ErrCode_CMD_GP_ChartViewTwoListError_InvalidTime,  "��Ч��ѯʱ��,���֤���ٴγ���"},
	{ErrCode_CMD_GP_ChartViewTwoListError_InvalidPageSize, "��Ч��ѯҳ��С,���֤���ٴγ���"},
	//CMD_GP_BetHistoryListError
	{ErrCode_CMD_GP_BetHistoryListError_AccountEmpty, "�ʺŲ���Ϊ��,���֤���ٴγ���"},
	{ErrCode_CMD_GP_BetHistoryListError_InvalidTime,  "��Ч��ѯʱ��,���֤���ٴγ���"},
	{ErrCode_CMD_GP_BetHistoryListError_InvalidPageSize, "��Ч��ѯҳ��С,���֤���ٴγ���"},

	//CMD_GR_SitFailed
	{ErrCode_CMD_GR_SitFailed_NoFindTable, "�Ҳ����յ���Ϸλ��,���Ժ�����"},
	{ErrCode_CMD_GR_SitFailed_AlreadySit, "���Ѿ�������Ϸ��,���ܸ���λ��"},
	{ErrCode_CMD_GR_SitFailed_NoEnoughRight, "��Ǹ,��û�н����Թ���Ϸ��Ȩ��,����Ҫ����,����ϵ��Ϸ�ͷ���ѯ"},
	{ErrCode_CMD_GR_SitFailed_RoomShallClose, "���ڴ���Ϸ���伴����ͣ����,��Ҳ������ٽ�����Ϸ��"},
	{ErrCode_CMD_GR_SitFailed_AllowEnterGame, "��Ǹ,����Ϸ�����ڲ�������ҽ���"},
	{ErrCode_CMD_GR_SitFailed_NoLookonUser, "�������Թ۵�λ���Ѿ�û�������,�����Թ�"},
	{ErrCode_CMD_GR_SitFailed_Password, "��Ϸ��Я���������,�����Թ���Ϸ"},
	{ErrCode_CMD_GR_SitFailed_PlayingGame, "��Ϸ��û�п�ʼ,��ʱ�����Թ�"},
	{ErrCode_CMD_GR_SitFailed_OtherSitFirstly, "�����Ѿ������˽����ȵ���,�´ζ���Ҫ���"},
	{ErrCode_CMD_GR_SitFailed_NoEnoughScore, "���Ķ�Ȳ���,���ܼ���"},
	{ErrCode_CMD_GR_SitFailed_TooManyScore, "���Ķ��̫��,���ܼ���"},
	{ErrCode_CMD_GR_SitFailed_SameIP, "����Ϸ���������� IP ��ַ��ͬ�����,���ܼ�����Ϸ"},

	//Message
	{ErrCode_Message_ShallCloseRoom, "��ע�⣬������Ҫ������Ϸ����ά�����¹���������Ϸ���伴����ͣ����"},
	{ErrCode_Message_AllowEnterRoom, "��Ǹ������ϵͳά����ԭ�򣬱���Ϸ���䲻�����κ���ҵ�¼����"},
	{ErrCode_Message_RoomFull, "��Ǹ������Ϸ�����Ѿ�����"},
	{ErrCode_Message_AnotherUserLogon, "��ע�⣬����ʺ�����һ�ط��������Ϸ�����ˣ��㱻���뿪"},
	{ErrCode_Message_AllowRoomChat, "��Ǹ������Ϸ���䲻������д�������"},
	{ErrCode_Message_AllowWisper, "��Ǹ������Ϸ���䲻�ܽ������˽��"},
	{ErrCode_Message_OptionServer, "�������������ò���ѡ�����óɹ�"},
	{ErrCode_Message_CloseRoom, "��ע�⣬��Ϸ���伴���رջ��߲�������ҽ��룬�����뿪��Ϸ����"},
	{ErrCode_Message_CanntStandup, "��������Ϸ����Ϸ����ʱ�����뿪λ��"},
	{ErrCode_Message_KickUser, "��Ǹ����������Ա�߳����䣬�����κ����ʣ�����ϵ��Ϸ�ͷ�"},
	{ErrCode_Message_ForceUserLogout, "��Ǹ����������Աǿ�Ƶǳ��������κ����ʣ�����ϵ��Ϸ�ͷ�"},
	{ErrCode_Message_LessScore, "�����Ϸ���������Ϸ�볡�����ܼ�����Ϸ"},

	//common
	{ErrCode_AccountNotExist, "�ʺŲ���Ϊ��,���֤���ٴγ���"},
	{ErrCode_LimitRight, "����Ȩ�޲���,����Ҫ����,����ϵ��Ϸ�ͷ���ѯ"},
	{ErrCode_Database, "�������ݿ�����쳣,����Ҫ����,����ϵ��Ϸ�ͷ���ѯ"},
};