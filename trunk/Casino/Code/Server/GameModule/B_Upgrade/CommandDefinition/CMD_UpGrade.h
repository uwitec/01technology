#ifndef CMD_UPGRADE_HEAD_FILE
#define CMD_UPGRADE_HEAD_FILE
#pragma pack(push)
#pragma pack(1)
//////////////////////////////////////////////////////////////////////////
//�����궨��

#define KIND_ID						ID_B_UPGRADE							//��Ϸ I D
#define GAME_PLAYER					4									//��Ϸ����
#define GAME_NAME					TEXT("������Ϸ")					//��Ϸ����
#define GAME_GENRE					(GAME_GENRE_GOLD|GAME_GENRE_MATCH)	//��Ϸ����

//��Ϸ״̬
#define GS_UG_FREE					GS_FREE								//����״̬
#define GS_UG_CALL					(GS_PLAYING+1)						//����״̬
#define GS_UG_BACK					(GS_PLAYING+2)						//����״̬
#define GS_UG_PLAY					(GS_PLAYING+3)						//��Ϸ״̬
#define GS_UG_WAIT					(GS_PLAYING+4)						//�ȴ�״̬

//////////////////////////////////////////////////////////////////////////
//����������ṹ

#define SUB_S_GAME_START			100									//��Ϸ��ʼ
#define SUB_S_SEND_CARD				101									//�����˿�
#define SUB_S_CALL_CARD				102									//�û�����
#define SUB_S_SEND_CONCEAL			103									//�����˿�
#define SUB_S_GAME_PLAY				104									//��Ϸ��ʼ
#define SUB_S_OUT_CARD				105									//�û�����
#define SUB_S_THROW_RESULT			106									//˦�ƽ��
#define SUB_S_TURN_BALANCE			107									//һ��ͳ��
#define SUB_S_GAME_END				108									//��Ϸ����

//��Ϸ״̬
struct CMD_S_StatusFree
{
	DOUBLE							fBaseScore;							//�������
};

//����״̬
struct CMD_S_StatusCall
{
	//��Ϸ����
	WORD							wBankerUser;						//ׯ���û�
	BYTE							cbPackCount;						//������Ŀ
	BYTE							cbMainValue;						//������ֵ
	BYTE							cbValueOrder[2];					//������ֵ

	//���Ʊ���
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbCardData[52];						//�˿��б�
	BYTE							cbHandCardCount[4];					//�˿���Ŀ

	//������Ϣ
	BYTE							cbComplete;							//��ɱ�־
	BYTE							cbCallCard;							//�����˿�
	BYTE							cbCallCount;						//������Ŀ
	WORD							wCallCardUser;						//�����û�
};

//����״̬
struct CMD_S_StatusBack
{
	//��Ϸ����
	BYTE							cbPackCount;						//������Ŀ
	BYTE							cbMainValue;						//������ֵ
	BYTE							cbMainColor;						//������ֵ
	BYTE							cbValueOrder[2];					//������ֵ

	//������Ϣ
	BYTE							cbCallCard;							//�����˿�
	BYTE							cbCallCount;						//������Ŀ
	WORD							wCallCardUser;						//�����û�

	//�û�����
	WORD							wBankerUser;						//ׯ���û�
	WORD							wCurrentUser;						//��ǰ�û�

	//�˿˱���
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbCardData[60];						//�˿��б�
	BYTE							cbHandCardCount[4];					//�˿���Ŀ

	//������Ϣ
	BYTE							cbConcealCount;						//������Ŀ
	BYTE							cbConcealCard[8];					//�����˿�
};

//��Ϸ״̬
struct CMD_S_StatusPlay
{
	//��Ϸ����
	BYTE							cbPackCount;						//������Ŀ
	BYTE							cbMainValue;						//������ֵ
	BYTE							cbMainColor;						//������ֵ
	BYTE							cbValueOrder[2];					//������ֵ

	//�û�����
	WORD							wBankerUser;						//ׯ���û�
	WORD							wCurrentUser;						//��ǰ�û�
	WORD							wFirstOutUser;						//�����û�

	//������Ϣ
	BYTE							cbCallCard;							//�����˿�
	BYTE							cbCallCount;						//������Ŀ
	WORD							wCallCardUser;						//�����û�

	//�˿˱���
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbCardData[52];						//�˿��б�
	BYTE							cbHandCardCount[4];					//�˿���Ŀ

	//������Ϣ
	BYTE							cbConcealCount;						//������Ŀ
	BYTE							cbConcealCard[8];					//�����˿�

	//���Ʊ���
	BYTE							cbOutCardCount[4];					//������Ŀ
	BYTE							cbOutCardData[4][52];				//�����б�

	//�÷ֱ���
	BYTE							cbScoreCardCount;					//�˿���Ŀ
	BYTE							cbScoreCardData[48];				//�÷��˿�
};

//�ȴ�״̬
struct CMD_S_StatusWait
{
	//��Ϸ����
	BYTE							cbPackCount;						//������Ŀ
	BYTE							cbMainValue;						//������ֵ
	BYTE							cbValueOrder[2];					//������ֵ

	//�û�����
	WORD							wBankerUser;						//ׯ���û�
};

//��Ϸ��ʼ
struct CMD_S_GameStart
{
	WORD							wBankerUser;						//ׯ���û�
	BYTE							cbPackCount;						//������Ŀ
	BYTE							cbMainValue;						//������ֵ
	BYTE							cbValueOrder[2];					//������ֵ
};

//�����˿�
struct CMD_S_SendCard
{
	BYTE							cbHandCardCount[4];					//�˿���Ŀ
	BYTE							cbCardCount;						//�˿���Ŀ
	BYTE							cbCardData[52];						//�˿��б�
};

//�û�����
struct CMD_S_CallCard
{
	BYTE							cbCallCard;							//�����˿�
	BYTE							cbCallCount;						//������Ŀ
	WORD							wCallCardUser;						//�����û�
};

//�����˿�
struct CMD_S_SendConceal
{
	BYTE							cbMainColor;						//������ֵ
	WORD							wBankerUser;						//ׯ���û�
	WORD							wCurrentUser;						//��ǰ�û�
	BYTE							cbConcealCount;						//������Ŀ
	BYTE							cbConcealCard[8];					//�����˿�
};

//��Ϸ��ʼ
struct CMD_S_GamePlay
{
	WORD							wCurrentUser;						//��ǰ�û�
	BYTE							cbConcealCount;						//������Ŀ
	BYTE							cbConcealCard[8];					//�����˿�
};

//�û�����
struct CMD_S_OutCard
{
	BYTE							cbCardCount;						//�˿���Ŀ
	WORD							wOutCardUser;						//�������
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							cbCardData[52];						//�˿��б�
};

//˦�ƽ��
struct CMD_S_ThrowResult
{
	WORD							wOutCardUser;						//�������
	WORD				 			wCurrentUser;						//��ǰ���
	BYTE							cbThrowCardCount;					//�˿���Ŀ
	BYTE							cbResultCardCount;					//�˿���Ŀ
	BYTE							cbCardDataArray[104];				//�˿�����
};

//һ��ͳ��
struct CMD_TurnBalance
{
	WORD							wTurnWinner;						//һ��ʤ��
	WORD				 			wCurrentUser;						//��ǰ���
};

//��Ϸ����
struct CMD_S_GameEnd
{
	DOUBLE							fScore[4];							//�û��÷�
	WORD							wGameScore;							//��Ϸ�÷�
	WORD							wConcealTime;						//�۵ױ���
	WORD							wConcealScore;						//���Ƶ÷�
	BYTE							cbConcealCount;						//������Ŀ
	BYTE							cbConcealCard[8];					//�����˿�

	//�û�����
	WORD							wBankerUser;						//ׯ���û�

	//������Ϣ
	BYTE							cbCallCard;							//�����˿�
	BYTE							cbCallCount;						//������Ŀ
	WORD							wCallCardUser;						//�����û�

	//�÷ֱ���
	BYTE							cbScoreCardCount;					//�˿���Ŀ
	BYTE							cbScoreCardData[48];				//�÷��˿�
};

//////////////////////////////////////////////////////////////////////////
//�ͻ�������ṹ

#define SUB_C_CALL_CARD				1									//�û�����
#define SUB_C_CALL_FINISH			2									//�������
#define SUB_C_CONCEAL_CARD			3									//�����˿�
#define SUB_C_OUT_CARD				4									//�û�����

//�û�����
struct CMD_C_CallCard
{
	BYTE							cbCallCard;							//�����˿�
	BYTE							cbCallCount;						//������Ŀ
};

//�����˿�
struct CMD_C_ConcealCard
{
	BYTE							cbConcealCount;						//������Ŀ
	BYTE							cbConcealCard[8];					//�����˿�
};

//�û�����
struct CMD_C_OutCard
{
	BYTE							cbCardCount;						//������Ŀ
	BYTE							cbCardData[52];						//�˿��б�
};

//////////////////////////////////////////////////////////////////////////
#pragma pack(pop)
#endif