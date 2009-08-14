#pragma once
#include "InitParamter.h"
#include "RequestSocket.h"

class CGameServerDlg;
//��Ϸ��������
class CGameServiceHelperContainer
{
public:
	CGameServiceHelperContainer(void);
	~CGameServiceHelperContainer(void);
	//��ʼ��
	void Init(CGameServerDlg*	pServiceLoaderDlg,
				int nGameServiceHelperIndex,
				IEventService * pIEventService,
				TCHAR*	szCenterServerAddr,
				TCHAR*	szLoaderServerPass,
				WORD wServerID,
				WORD wGameNetPort,
				TCHAR *szGameNetAddr);
	//����
	BOOL Start();
	//ֹͣ
	void Stop();
	//�ж�����
	BOOL IsRunning();
	//���ò���
	void SetParam(TCHAR*	szCenterServerAddr,
		TCHAR*	szLoaderServerPass,
		WORD wServerID,
		WORD wGameNetPort,
		TCHAR *szGameNetAddr);
	//��ȡ���ò���
	CInitParamter& GetInitParamter(){return m_InitParamter;}
	//��������
	void RequstStart(BOOL b);
	//�ж���������
	BOOL IsRequstStart();
	//������Ϣ�ɹ�
	int  OnRequestInfoOk(WPARAM w, LPARAM l);
	
protected:
	//��ӡ������Ϣ
	void ShowErrorMessasge (LPCTSTR pszString, enTraceLevel TraceLevel);
	//��ӡ������Ϣ
	void ShowDescribeMessage(LPCTSTR pszString);
	//֪ͨ����״̬
	void NotifyServiceStatus(bool bRunning);
public:
	CGameServiceHelper				m_GameService;						//GameService
	IEventService *					m_pIEventService;					//�¼�����
	tagGameServiceParameter			m_GameServiceParameter;				//��Ϸ�������

	CInitParamter					m_InitParamter;						//���ò���
	CRequestSocket					m_RequestSocket;					//������Ϣ�������

	CGameServerDlg*					m_pServiceLoaderDlg;				//������
	int								m_nGameServiceHelperIndex;			//�����������
	
	BOOL							m_bRequstStart;						//����ʼ���
};
