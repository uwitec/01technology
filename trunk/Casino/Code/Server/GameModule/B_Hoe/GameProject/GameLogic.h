#ifndef GAME_LOGIC_HEAD_FILE
#define GAME_LOGIC_HEAD_FILE

#pragma once

//////////////////////////////////////////////////////////////////////////

//�˿�����
#define CT_ERROR					0									//��������
#define CT_SINGLE					1									//��������
#define CT_DOUBLE					2									//��������
#define CT_THREE_TIAO				3									//��������
#define	CT_SHUN_ZI					4									//˳������
#define CT_TONG_HUA					5									//ͬ������
#define CT_HU_LU					6									//��«����
#define CT_TIE_ZHI					7									//��֧����
#define CT_TONG_HUA_SHUN			8									//ͬ��˳��

//�궨��
#define MAX_COUNT					13									//�����Ŀ

//��ֵ����
#define	LOGIC_MASK_COLOR			0xF0								//��ɫ����
#define	LOGIC_MASK_VALUE			0x0F								//��ֵ����

//////////////////////////////////////////////////////////////////////////
class tagAnalyseResult
{
public:
	BYTE bFourCount;//������Ŀ
	BYTE bThreeCount;//������Ŀ
	BYTE bDoubleCount;//������Ŀ
	BYTE bSignedCount;//������Ŀ
	BYTE bFourLogicVolue[3];//�����б�
	BYTE bThreeLogicVolue[4];//�����б�
	BYTE bDoubleLogicVolue[6];//�����б�
	BYTE m_bSCardData[13];//�����˿�
	BYTE m_bDCardData[13];//�����˿�
	BYTE m_bTCardData[13];//�����˿�
	BYTE m_bFCardData[13];//�����˿�

	tagAnalyseResult()
	{
	}

	void init()
	{
		bFourCount=0;//������Ŀ
		bThreeCount=0;//������Ŀ
		bDoubleCount=0;//������Ŀ
		bSignedCount=0;//������Ŀ
		memset(bFourLogicVolue, 0, sizeof(bFourLogicVolue));
		memset(bThreeLogicVolue, 0, sizeof(bThreeLogicVolue));
		memset(bDoubleLogicVolue, 0, sizeof(bDoubleLogicVolue));
		memset(m_bSCardData, 0, sizeof(m_bSCardData));
		memset(m_bDCardData, 0, sizeof(m_bDCardData));
		memset(m_bTCardData, 0, sizeof(m_bTCardData));
		memset(m_bFCardData, 0, sizeof(m_bFCardData));
		
	}
};
//��Ϸ�߼���
class CGameLogic
{
	//��������
protected:
	static const BYTE				m_bCardListData[52];				//�˿�����

	//��������
public:
	//���캯��
	CGameLogic();
	//��������
	virtual ~CGameLogic();

	//���ͺ���
public:
	//��ȡ����
	BYTE GetCardType(BYTE bCardData[], BYTE bCardCount);
	//��ȡ��ֵ
	BYTE GetCardValue(BYTE bCardData) { return bCardData&LOGIC_MASK_VALUE; }
	//��ȡ��ɫ
	BYTE GetCardColor(BYTE bCardData) { return bCardData&LOGIC_MASK_COLOR; }

	//���ƺ���
public:
	//�����˿�
	void SortCardList(BYTE bCardData[], BYTE bCardCount);
	//�����˿�
	void RandCardList(BYTE bCardBuffer[], BYTE bBufferCount);
	//ɾ���˿�
	bool RemoveCard(const BYTE bRemoveCard[], BYTE bRemoveCount, BYTE bCardData[], BYTE bCardCount);

	//�߼�����
public:
	//��Ч�ж�
	bool IsValidCard(BYTE cbCardData);
	//�߼���ֵ
	BYTE GetCardLogicValue(BYTE cbCardData);


	//���ͺ���
public:
	//�Ƿ����
	bool IsDouble(BYTE bCardData[], BYTE bCardCount);
	//�Ƿ���֧
	bool IsTieZhi(BYTE bCardData[], BYTE bCardCount);
	//�Ƿ��«
	bool IsHuLu(BYTE bCardData[], BYTE bCardCount);
	//�Ƿ�ͬ��
	bool IsTongHua(BYTE bCardData[], BYTE bCardCount);
	//�Ƿ�˳��
	bool IsShunZhi(BYTE bCardData[], BYTE bCardCount);
	//�Ƿ�����
	bool IsSanTiao(BYTE bCardData[], BYTE bCardCount);
	//�Ƿ�ͬ��˳
	bool IsTongHuaShun(BYTE bCardData[], BYTE bCardCount);

	//��������
public:
	//����˳������
	int AnalyseCardData_ShunZhi(const BYTE CardData[],
		BYTE CardCount,BYTE AnalyseResult[][5]);
	//����ͬ������
	int AnalyseCardData_TongHua(const BYTE CardData[],
		BYTE CardCount,BYTE AnalyseResult[][5]);
	//����ͬ������
	int GetTongHuaCardData(BYTE HuaSe,
		const BYTE CardData[],
		BYTE CardCount,BYTE AnalyseResult[]);
	//������«����
	int AnalyseCardData_HuLu(const BYTE TCardData[],
		int TCardCount,
		const BYTE DCardData[],
		int DCardCount,
		BYTE AnalyseResult[][5]);
	//������֧����
	int AnalyseCardData_TieZhi(const BYTE FCardData[],
		int FCardCount,
		const BYTE SCardData[],
		int SCardCount,
		BYTE AnalyseResult[][5]);
	//ͬ��˳��
	int AnalyseCardData_TongHuaShun(const BYTE CardData[],
		int CardCount,
		BYTE AnalyseResult[][5]);
	//�����˿�
	void AnalysebCardData(const BYTE bCardData[], BYTE bCardCount, tagAnalyseResult & AnalyseResult);

	//���ܺ���
public:
	//�Ա��˿�
	bool CompareCard(BYTE bFirstCardData, BYTE bNextCardData);
	//�Ա��˿�
	bool CompareCard(BYTE bFirstList[], BYTE bNextList[], BYTE bFirstCount, BYTE bNextCount);
	//�����ж�
	BYTE SearchOutCard(BYTE bHandCardData[], BYTE bHandCardCount, BYTE bTurnCardData[], BYTE bTurnCardCount, BYTE bTurnOutType, BYTE cbOutCard[MAX_COUNT]);
};

//////////////////////////////////////////////////////////////////////////

#endif