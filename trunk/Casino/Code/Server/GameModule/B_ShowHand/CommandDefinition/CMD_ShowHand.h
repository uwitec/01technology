#ifndef CMD_SHOWHAND_HEAD_FILE
#define CMD_SHOWHAND_HEAD_FILE

#include "..\..\..\CommonDefinition\GlobalDef.h"
#pragma pack(push)
#pragma pack(1)
//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID							ID_B_SHOWHAND							//��Ϸ I D
#define GAME_PLAYER						4									//��Ϸ����
#define GAME_NAME						TEXT("�����Ϸ")					//��Ϸ����
#define GAME_GENRE						(GAME_GENRE_GOLD|GAME_GENRE_MATCH)	//��Ϸ����

//////////////////////////////////////////////////////////////////////////
//�����붨��

#define SUB_C_ADD_GOLD					1									//�û���ע
#define SUB_C_GIVE_UP					2									//������ע

#define SUB_S_GAME_START				100									//��Ϸ��ʼ
#define SUB_S_ADD_GOLD					101									//��ע���
#define SUB_S_GIVE_UP					102									//������ע
#define SUB_S_SEND_CARD					103									//������Ϣ
#define SUB_S_GAME_END					104									//��Ϸ����

//////////////////////////////////////////////////////////////////////////
//��Ϣ�ṹ��

//��Ϸ״̬
struct CMD_S_StatusFree
{
	DOUBLE								fBasicGold;						//�������
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	WORD				 				wCurrentUser;						//��ǰ���
	BYTE								bPlayStatus[GAME_PLAYER];			//��Ϸ״̬
	BYTE								bTableCardCount[GAME_PLAYER];		//�˿���Ŀ
	BYTE								bTableCardArray[GAME_PLAYER][5];	//�˿�����
	DOUBLE								fMaxGold;							//�����ע
	DOUBLE								fBasicGold;							//�������
	DOUBLE								fTurnMaxGold;						//�����ע
	DOUBLE								fTurnBasicGold;						//������ע
	DOUBLE								fTableGold[2*GAME_PLAYER];			//������
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	BYTE								bFundusCard;						//�����˿�
	WORD				 				wCurrentUser;						//��ǰ���
	DOUBLE								fMaxGold;							//�����ע
	DOUBLE								fTurnMaxGold;						//�����ע
	DOUBLE								fTurnBasicGold;						//������ע
	BYTE								bCardData[GAME_PLAYER];				//�û��˿�
};

//�û�����
struct CMD_S_GiveUp
{
	WORD								wUserChairID;						//�����û�
};

//�û���ע
struct CMD_C_AddGold
{
	double								fGold;								//��ע��Ŀ
};

//��ע���
struct CMD_S_AddGold
{
	WORD								wLastChairID;						//��һ�û�
	WORD								wCurrentUser;						//��ǰ�û�
	DOUBLE								fLastAddGold;						//��ע��Ŀ
	DOUBLE								fCurrentLessGold;					//���ټ�ע
};

//�������ݰ�
struct CMD_R_SendCard
{
	DOUBLE								fMaxGold;							//�����ע
	WORD								wCurrentUser;						//��ǰ�û�
	BYTE								bUserCard[4];						//�û��˿�
};

//��Ϸ����
struct CMD_S_GameEnd
{
	DOUBLE								fGameGold[4];						//��Ϸ�÷�
	BYTE								bUserCard[4];						//�û��˿�

	BYTE								bSendCardCount;
	WORD								wWinerUser;
	BYTE								bPlayStatus[GAME_PLAYER];			//��Ϸ״̬
	BYTE								bTableCardCount[GAME_PLAYER];		//�˿���Ŀ
	BYTE								bTableCardArray[GAME_PLAYER][5];	//�˿�����
	DOUBLE								fMaxGold;							//�����ע
	DOUBLE								fBasicGold;							//�������
	DOUBLE								fTurnMaxGold;						//�����ע
	DOUBLE								fTurnBasicGold;						//������ע
	DOUBLE								fTableGold[2*GAME_PLAYER];			//������

};

//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)
#endif