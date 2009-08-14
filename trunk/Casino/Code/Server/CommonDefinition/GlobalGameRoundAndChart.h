#pragma	  once
#pragma pack(push)
#pragma pack(1)
//����
static const int SORTTYPE_TimeDesc = 1;//ʱ�併��
static const int SORTTYPE_TimeAsc = SORTTYPE_TimeDesc + 1;//ʱ������
static const int SORTTYPE_BetScoreDesc = SORTTYPE_TimeAsc + 1;//Ͷע����
static const int SORTTYPE_BetScoreAsc  = SORTTYPE_BetScoreDesc + 1;//Ͷע����
static const int SORTTYPE_WinScoreDesc  = SORTTYPE_BetScoreAsc + 1;//�ɲʽ���
static const int SORTTYPE_WinScoreAsc  = SORTTYPE_WinScoreDesc + 1;//�ɲ�����
static const int SORTTYPE_ValidScoreDesc  = SORTTYPE_WinScoreAsc + 1;//ʵ��������
static const int SORTTYPE_ValidScoreAsc  = SORTTYPE_ValidScoreDesc + 1;//ʵ��������
static const int SORTTYPE_RollbackDesc  = SORTTYPE_ValidScoreAsc + 1;//ϴ�뽵��
static const int SORTTYPE_RollbackAsc  = SORTTYPE_RollbackDesc + 1;//ϴ������
static const int SORTTYPE_RoundCountDesc  = SORTTYPE_RollbackAsc + 1;//��Ϸ����Ŀ����
static const int SORTTYPE_RoundCountAsc  = SORTTYPE_RoundCountDesc + 1;//��Ϸ����Ŀ����

//�����¼�鿴��ʽ-1
struct tagChartViewOneData
{
	TCHAR	szAccount[NAME_LEN];	//�ʺ�
	//Ͷע
	DWORD	dwGameRoundCount;		//����
	DOUBLE	fTotalBetScore;			//Ͷע�ܶ�
	DOUBLE	fTotalWinScore;			//���ɲ�
	DOUBLE	fTotalTaxScore;			//�ܳ�ˮ

	//�ɲ�ռ��
	DOUBLE	fWinScore_High;			//�ɲ�ռ��-����
	DOUBLE	fWinScore_Self;			//�ɲ�ռ��-����
	DOUBLE	fWinScore_Less;			//�ɲ�ռ��-����

	//��ˮռ��
	DOUBLE	fTaxScore_High;			//��ˮռ��-����
	DOUBLE	fTaxScore_Self;			//��ˮռ��-����
	DOUBLE	fTaxScore_Less;			//��ˮռ��-����
	
	//ϴ��
	DOUBLE	fValidScore_Total;		//ϴ��-�����ܶ�
	DOUBLE	fValidScore_High;		//ϴ��-���߻���
	DOUBLE	fValidScore_High_Rollback;	//ϴ��-����ϴ��
	DOUBLE	fValidScore_Less;		//ϴ��-���߻���
	DOUBLE	fValidScore_Less_Rollback;	//ϴ��-����ϴ��

	//���ն��
	DOUBLE	fPaidScore_High;		//���ն��-���߽���
	DOUBLE	fPaidScore_Less;		//���ն��-���߽���
};

//�����¼�鿴��ʽ-2
struct tagChartViewTwoData
{
	TCHAR	szAccount[NAME_LEN];	//�ʺ�
	//Ͷע
	DWORD	dwGameRoundCount;		//����
	DOUBLE	fTotalBetScore;			//Ͷע�ܶ�
	DOUBLE	fTotalWinScore;			//���ɲ�
	DOUBLE	fTotalTaxScore;			//�ܳ�ˮ

	//ϴ��
	DOUBLE	fValidScore_Total;		//ϴ��-�����ܶ�
	DOUBLE	fValidScore_Less_Rollback;	//ϴ��-����ϴ��

};

//��Ϸ�ּ�¼
struct tagBetHistoryData
{
	DOUBLE		fUCID;							//�����¼ID

	DOUBLE		fGameRoundID;					//��Ϸ�ּ�¼ID
	DOUBLE		fStartTime;						//��Ϸ��ʼʱ��
	WORD		wProcessType;					//��Ϸ����

	TCHAR		szTypeName[TYPE_LEN];			//��������
	TCHAR		szKindName[KIND_LEN];			//��Ϸ����
	TCHAR		szServerName[SERVER_LEN];		//��������
	WORD		wTableID;						//��ID

	DOUBLE		fTotalBetScore;					//Ͷע�ܶ�
	DOUBLE		fTotalWinScore;					//���ɲ�
	DOUBLE		fTotalTaxScore;					//�ܳ�ˮ
	DOUBLE		fValidScore_Less_Rollback;		//ϴ��-����ϴ��

	BYTE		cbEndReason;					//��Ϸ����ԭ��
	WORD		wEndDataSize;					//��Ϸ������Ϣ��С
	BYTE		cbEndData[1536];				//��Ϸ������Ϣ
};
#pragma pack(pop)