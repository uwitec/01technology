#ifndef CMD_LAND_HEAD_FILE
#define CMD_LAND_HEAD_FILE
#pragma pack(push)
#pragma pack(1)
//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID						ID_B_LAND								//��Ϸ I D
#define GAME_PLAYER					3									//��Ϸ����
#define GAME_NAME					TEXT("������")						//��Ϸ����
#define GAME_GENRE					(GAME_GENRE_SCORE|GAME_GENRE_GOLD)	//��Ϸ����

//��Ϸ״̬
#define GS_WK_FREE					GS_FREE								//�ȴ���ʼ
#define GS_WK_SCORE					GS_PLAYING							//�з�״̬
#define GS_WK_PLAYING				GS_PLAYING+1						//��Ϸ����

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_SEND_CARD				100									//��������
#define SUB_S_LAND_SCORE			101									//�з�����
#define SUB_S_GAME_START			102									//��Ϸ��ʼ
#define SUB_S_OUT_CARD				103									//�û�����
#define SUB_S_PASS_CARD				104									//��������
#define SUB_S_GAME_END				105									//��Ϸ����

//��Ϸ״̬
struct CMD_S_StatusFree
{
	DOUBLE							fBaseScore;							//�������
};

//��Ϸ״̬
struct CMD_S_StatusScore
{
	BYTE							bLandScore;							//��������
	DOUBLE							fBaseScore;							//�������
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bScoreInfo[3];						//�з���Ϣ
	BYTE							bCardData[17];						//�����˿�
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	WORD							wLandUser;							//�������
	WORD							wBombTime;							//ը������
	DOUBLE							fBaseScore;							//�������
	BYTE							bLandScore;							//��������
	WORD							wLastOutUser;						//���Ƶ���
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bBackCard[3];						//�����˿�
	BYTE							bCardData[20];						//�����˿�
	BYTE							bCardCount[3];						//�˿���Ŀ
	BYTE							bTurnCardCount;						//��������
	BYTE							bTurnCardData[20];					//�����б�
};

//�����˿�
struct CMD_S_SendCard
{
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bCardData[17];						//�˿��б�
};

//�û��з�
struct CMD_S_LandScore
{
	WORD							bLandUser;							//�з����
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bLandScore;							//�ϴνз�
	BYTE							bCurrentScore;						//��ǰ�з�
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	WORD				 			wLandUser;							//�������
	BYTE							bLandScore;							//��������
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bBackCard[3];						//�����˿�
};

//�û�����
struct CMD_S_OutCard
{
	BYTE							bCardCount;							//������Ŀ
	WORD				 			wCurrentUser;						//��ǰ���
	WORD							wOutCardUser;						//�������
	BYTE							bCardData[20];						//�˿��б�
};

//��������
struct CMD_S_PassCard
{
	BYTE							bNewTurn;							//һ�ֿ�ʼ
	WORD				 			wPassUser;							//�������
	WORD				 			wCurrentUser;						//��ǰ���
};

//��Ϸ����
struct CMD_S_GameEnd
{
	DOUBLE							fGameScore[3];						//��Ϸ���
	BYTE							bCardCount[3];						//�˿���Ŀ
	BYTE							bCardData[54];						//�˿��б� 
	BYTE							bBackCard[3];						//�����˿�
	WORD							wLandUser;							//�������
	WORD							wBombTime;							//ը������
	BYTE							bLandScore;							//��������
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_LAND_SCORE			1									//�û��з�
#define SUB_C_OUT_CART				2									//�û�����
#define SUB_C_PASS_CARD				3									//��������

//�û��з�
struct CMD_C_LandScore
{
	BYTE							bLandScore;							//��������
};

//�������ݰ�
struct CMD_C_OutCard
{
	BYTE							bCardCount;							//������Ŀ
	BYTE							bCardData[20];						//�˿��б�
};

//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)
#endif