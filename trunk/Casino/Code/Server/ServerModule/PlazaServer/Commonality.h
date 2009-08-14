#ifndef COMMONALITY_HEAD_FILE
#define COMMONALITY_HEAD_FILE

#pragma once

#include "Stdafx.h"

//////////////////////////////////////////////////////////////////////////
//����ͷ�ļ�

//�������ļ�
#include "..\..\CommandDefinition\CMD_Plaza.h"
#include "..\..\CommandDefinition\CMD_Center.h"

//ȫ��ͷ�ļ�
#include "..\..\CommonDefinition\GlobalField.h"
#include "..\..\CommonDefinition\GlobalService.h"
#include "..\..\CommonDefinition\GlobalOperation.h"
#include "..\..\CommonDefinition\GlobalGameRoundAndChart.h"
//////////////////////////////////////////////////////////////////////////
//������
struct tagOperationUserInfo
{
	DWORD			dwOperationUserID;					//�����û�ID
	BYTE			cbOperationUserType;				//�����û�����
	TCHAR			szOperationAccount[NAME_LEN];		//�����ʺ�
	DWORD			dwIPAddress;						//������ַ
};
//////////////////////////////////////////////////////////////////////////
//���ݿ������
static const DWORD dwDBRequstOffset= 5000;

#define	DBR_GP_LOGON_BY_ACCOUNT						10		//�ʺŵ�½

//���ݿ������ʶ
#define DBR_GP_LOGON_SUCCESS						dwDBRequstOffset + 10//��½�ɹ�
#define DBR_GP_LOGON_ERROR							dwDBRequstOffset + 11//��½ʧ��
//�ʺŵ�½
struct DBR_GP_LogonByAccount
{
	DWORD							dwClientIP;							//���ӵ�ַ
	TCHAR							szAccounts[NAME_LEN];				//��¼�ʺ�
	TCHAR							szPassWord[PASS_LEN];				//��¼����
	TCHAR							szRealityPass[PASS_LEN];			//��ʵ����
	BYTE							cbClientType;						//�ͻ�������
};


//��½�ɹ�
typedef CMD_GP_LogonSuccess DBR_GP_LogonSuccess;

//��½ʧ��
typedef CMD_GP_Error DBR_GP_LogonError;

//////////////////////////////////////////////////////////////////////////

#define DBR_GP_REGISTER_USER						20					//ע���ʺ�
//�ʺ�ע��
struct DBR_GP_RegisterUser
{
	tagOperationUserInfo			Operator;							//������

	DWORD							dwOperationUserID;					//�����û�ID
	DWORD							dwClientIP;							//���ӵ�ַ
	tagUserData						UserData;
	TCHAR							szPassWord[PASS_LEN];				//��¼����
};

//////////////////////////////////////////////////////////////////////////

#define DBR_GP_GET_LOWUSERLIST						30					//��ȡ�����ʺ�
#define DBR_GP_GET_SUBUSERLIST						31					//��ȡ���ʺ�
//���ݿ������ʶ
#define DBR_GP_GET_LOWUSERLISTCOMPLETE				dwDBRequstOffset + 30//���������ʺ����

typedef CMD_GP_GetLessUserList DBR_GP_GetLessUserList;
typedef CMD_GP_GetLessUserList DBR_GP_GetSubUserList;

//////////////////////////////////////////////////////////////////////////

#define DBR_GP_SET_SETUSERSCORE						40					//��ȴ�ȡ

struct DBR_GP_SetUserScore 
{
	tagOperationUserInfo			Operator;							//������
	CMD_GP_SetUserScore				RequestData;						//��������
};

//////////////////////////////////////////////////////////////////////////

#define DBR_GP_SET_USERCONGEALSTATE					50					//״̬�趨
struct DBR_GP_SetUserCongealState 
{
	tagOperationUserInfo			Operator;							//������
	CMD_GP_SetUserCongealState		RequestData;						//��������
};

//////////////////////////////////////////////////////////////////////////

#define DBR_GP_SET_BASICUSERINFO					60					//������Ϣ�趨
struct DBR_GP_SetBasicUserInfo 
{
	tagOperationUserInfo			Operator;							//������
	CMD_GP_BasicUserInfo		RequestData;							//��������
};

//////////////////////////////////////////////////////////////////////////

#define DBR_GP_SET_DETAILUSERINFO					70					//��ϸ��Ϣ�趨
struct DBR_GP_SetDetailUserInfo 
{
	tagOperationUserInfo			Operator;							//������
	CMD_GP_DetailUserInfo		RequestData;							//��������
};

//////////////////////////////////////////////////////////////////////////

#define DBR_GP_SET_USERRIGHT						80					//Ȩ���趨
struct DBR_GP_SetUserRight 
{
	tagOperationUserInfo			Operator;							//������
	CMD_GP_SetUserRight		RequestData;								//��������
};

//////////////////////////////////////////////////////////////////////////

#define DBR_GP_SET_MYSELFBASICUSERINFO				90					//���������Ϣ�趨
struct DBR_GP_SetMySelfBasicUserInfo 
{
	tagOperationUserInfo			Operator;							//������
	CMD_GP_MySelfBasicUserInfo		RequestData;						//��������
};



//////////////////////////////////////////////////////////////////////////

#define DBR_GP_SET_SUBUSERCONGEALSTATE				100					//���ʺ�״̬�趨
struct DBR_GP_SetSubUserCongealState 
{
	tagOperationUserInfo			Operator;							//������
	CMD_GP_SetSubUserCongealState		RequestData;					//��������
};

//////////////////////////////////////////////////////////////////////////

#define DBR_GP_REGISTER_SUBUSER						110					//ע�����ʺ�
struct DBR_GP_RegisterSubUserInfo 
{
	tagOperationUserInfo			Operator;							//������
	CMD_GP_RegisterSubUser		RequestData;							//��������
};

//////////////////////////////////////////////////////////////////////////

#define DBR_GP_SET_BASICSUBUSERINFO					120					//���ʺŻ�����Ϣ�趨
struct DBR_GP_SetBasicSubUserInfo 
{
	tagOperationUserInfo			Operator;							//������
	CMD_GP_BasicSubUserInfo		RequestData;							//��������
};

//////////////////////////////////////////////////////////////////////////
#define DBR_GP_GET_OPERATION						130					//��ȡ������¼
struct DBR_GP_GetOperation 
{
	tagOperationUserInfo			Operator;							//������
	CMD_GP_GetOperation				RequestData;						//��������
};

//////////////////////////////////////////////////////////////////////////
#define DBR_GP_GET_CHARTVIEWONE						140					//��ȡ��Ϸ�����¼��ʽ-1
struct DBR_GP_GetChartViewOne
{
	tagOperationUserInfo			Operator;							//������
	CMD_GP_GetChartViewOne			RequestData;						//��������
};
//////////////////////////////////////////////////////////////////////////
#define DBR_GP_GET_CHARTVIEWTWO						150					//��ȡ��Ϸ�����¼��ʽ-2
struct DBR_GP_GetChartViewTwo
{
	tagOperationUserInfo			Operator;							//������
	CMD_GP_GetChartViewTwo			RequestData;						//��������
};
//////////////////////////////////////////////////////////////////////////
#define DBR_GP_GET_BETHISTORY						160					//��ȡ��Ϸ�ּ�¼
struct DBR_GP_GetBetHistory
{
	tagOperationUserInfo			Operator;							//������
	CMD_GP_GetBetHistory			RequestData;						//��������
};
//////////////////////////////////////////////////////////////////////////
#define DBR_GP_GET_DETAILBETSCORE					170					//��ȡ��Ϸ�ּ�¼��ϸͶע��Ϣ
struct DBR_GP_GetDetailBetScore
{
	tagOperationUserInfo			Operator;							//������
	CMD_GP_GetDetailBetScore		RequestData;						//��������
};
//////////////////////////////////////////////////////////////////////////
#define DBR_GP_GET_CHARTVIEWTHREE					180					//��ȡ��Ϸ�����¼��ʽ-3
struct DBR_GP_GetChartViewThree
{
	tagOperationUserInfo			Operator;							//������
	CMD_GP_GetChartViewThree		RequestData;						//��������
};
///////////////////////////////////////////////////////////////////////////
#define DBR_UPDATE_GAMEMAININFO						1000				//������Ϸ����Ϣ
#define DBR_UPDATE_GAMEMAININFO_SUCCESS				1001				//������Ϸ����Ϣ
struct DBR_Update_GameMainInfo
{
	CMD_GP_GameInfo					GameInfo;							//��Ϸ��Ϣ
	CMD_GP_Notice					ManagementNotice;					//������
	CMD_GP_Notice					GameNotice;							//��Ϸ����

};

//////////////////////////////////////////////////////////////////////////
#define	DBR_GP_LOGOUT_USER							1010				//ע���ʺŵ�½
struct DBR_GP_LogoutUser
{
	DWORD							dwRealUserID;						//�û��ʺ�
	BYTE							cbUserType;							//�û�����
};

//////////////////////////////////////////////////////////////////////////
//��������
#define	MR_EVENT_SEND_USER_MESSAGE					2000				//�����û���Ϣ
struct 	MR_EVENT_SendUserMessage
{
	DWORD							dwRealUserID;						//�û��ʺ�
	BYTE							cbUserType;							//�û�����
	BOOL							bKick;								//�ǳ����
	TCHAR							szMessage[1024];					//�û���Ϣ
};

#endif