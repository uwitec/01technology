#ifndef GLOBAL_FRAME_HEAD_FILE
#define GLOBAL_FRAME_HEAD_FILE

#pragma once

#include "GlobalDef.h"
#pragma pack(push)
#pragma pack(1)
//////////////////////////////////////////////////////////////////////////
//�궨��

//��Ϸ״̬
#define GS_FREE							0								//����״̬
#define GS_PLAYING						100								//��Ϸ״̬


//////////////////////////////////////////////////////////////////////////
//����������

#define MDM_GF_GAME						100								//��Ϸ��Ϣ
#define MDM_GF_FRAME					101								//�����Ϣ

#define SUB_GF_INFO						1								//��Ϸ��Ϣ
#define SUB_GF_USER_READY				2								//�û�ͬ��

#define SUB_GF_OPTION					100								//��Ϸ����
#define SUB_GF_SCENE					101								//������Ϣ

#define SUB_GF_USER_CHAT				200								//�û�����

#define SUB_GF_MESSAGE					300								//ϵͳ��Ϣ

//�汾��Ϣ
struct CMD_GF_Info
{
	BYTE								bAllowLookon;					//�Թ۱�־
};

//��Ϸ����
struct CMD_GF_Option
{
	BYTE								bGameStatus;					//��Ϸ״̬
	BYTE								bAllowLookon;					//�����Թ�
};

//����ṹ
struct CMD_GF_UserChat
{
	WORD								wChatLength;					//��Ϣ����
	DWORD								dwSendUserID;					//�����û�
	DWORD								dwTargetUserID;					//Ŀ���û�
	TCHAR								szChatMessage[MAX_CHAT_LEN];	//������Ϣ
};

//��Ϣ����
#define SMT_INFO						0x0001							//��Ϣ��Ϣ
#define SMT_EJECT						0x0002							//������Ϣ
#define SMT_GLOBAL						0x0004							//ȫ����Ϣ
#define SMT_CLOSE_GAME					0x1000							//�ر���Ϸ
#define SMT_INTERMIT_LINE				0x4000							//�ж�����

//��Ϣ���ݰ�
struct CMD_GF_Message
{
	WORD							wMessageType;						//��Ϣ����
	WORD							wErrorCode;							//������Ϣ����
	WORD							wMessageLength;						//��Ϣ����
	TCHAR							szContent[1024];					//��Ϣ����
};


//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)
#endif