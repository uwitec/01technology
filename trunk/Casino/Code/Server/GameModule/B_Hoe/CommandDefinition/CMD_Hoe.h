#ifndef CMD_HOE_HEAD_FILE
#define CMD_HOE_HEAD_FILE
#pragma pack(push)
#pragma pack(1)
//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID						ID_B_HOE								//��Ϸ I D
#define GAME_PLAYER					4									//��Ϸ����
#define GAME_NAME					TEXT("�����")						//��Ϸ����
#define GAME_GENRE					(GAME_GENRE_SCORE|GAME_GENRE_GOLD)	//��Ϸ����

//��Ϸ״̬
#define GS_RF_FREE					GS_FREE								//�ȴ���ʼ
#define GS_RF_PLAYING				GS_PLAYING							//�з�״̬

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_START			101									//��Ϸ��ʼ
#define SUB_S_OUT_CARD				102									//�û�����
#define SUB_S_PASS_CARD				103									//��������
#define SUB_S_GAME_END				104									//��Ϸ����

//��Ϸ״̬
struct CMD_S_StatusFree
{
	DOUBLE							fBaseScore;							//�������
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	DOUBLE							fBaseScore;							//�������
	WORD							wBankerUser;						//ׯ���û�
	WORD							wLastOutUser;						//���Ƶ���
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							bCardData[13];						//�����˿�
	BYTE							bCardCount[4];						//�˿���Ŀ
	BYTE							bTurnCardCount;						//��������
	BYTE							bTurnCardData[13];					//�����б�
	DOUBLE                          fAllTurnScore[4];					//�ֵܾ÷�
	DOUBLE                          fLastTurnScore[4];					//�Ͼֵ÷�
};

//�����˿�
struct CMD_S_GameStart
{
	WORD							wBankerUser;						//ׯ���û�
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							cbCardData[13];						//�˿��б�
};

//�û�����
struct CMD_S_OutCard
{
	BYTE							bCardCount;							//�˿���Ŀ
	WORD				 			wCurrentUser;						//��ǰ���
	WORD							wOutCardUser;						//�������
	BYTE							bCardData[13];						//�˿��б�
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
	DOUBLE							fGameScore[4];						//��Ϸ���
	BYTE							bCardCount[4];						//�˿���Ŀ
	BYTE							bCardData[52];						//�˿��б� 
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_OUT_CART				2									//�û�����
#define SUB_C_PASS_CARD				3									//��������

//�������ݰ�
struct CMD_C_OutCard
{
	BYTE							bCardCount;							//������Ŀ
	BYTE							bCardData[13];						//�˿��б�
};

//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)
#endif