#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "PlazaService.h"
#include "RequestSocket.h"

//////////////////////////////////////////////////////////////////////////
class CUpdateEventSink;

//���öԻ���
class CSystemOptionDlg : public CDialog
{
	//���캯��
public:
	//�������
	CSystemOptionDlg();
	//��������
	virtual ~CSystemOptionDlg();

	//���غ���
protected:
	//��ʼ������
	virtual BOOL OnInitDialog();
	//�ؼ����໯
	virtual void DoDataExchange(CDataExchange * pDX);
	//ȷ������
	virtual void OnOK();

	//��Ϣӳ��
	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////

//���Ի���
class CPlazaServerDlg : public CDialog
{
	//��������
protected:
	bool							m_bAskQuit;							//�˳�ѯ��

	//�������
public:
	CPlazaService					m_LogonService;						//��������
	CEventServiceHelper				m_EventService;						//�¼�����
	CRequestSocket					m_RequestSocket;					//����Socket
	CUpdateEventSink				*m_pUpdateEventSink;					//�����¼�����

	//�ؼ�����
public:
	CRichEditCtrl					m_RichEditCtrl;						//�¼���¼

	//���캯��
public:
	//�������
	CPlazaServerDlg();
	//��������
	virtual ~CPlazaServerDlg();
	//��������
protected:
	//��������
	void UpdateUserCount(WORD wLogonUserCount,WORD wGameUserCount);

	//���غ���
protected:
	//�ؼ����໯
	virtual void DoDataExchange(CDataExchange * pDX);
	//��ʼ������
	virtual BOOL OnInitDialog();
	//ȷ����Ϣ
	virtual void OnOK();
	//ȡ������
	virtual void OnCancel();
	//��Ϣ����
	virtual BOOL PreTranslateMessage(MSG * pMsg);
	//��Ϣ����
protected:
	//��������
	afx_msg void OnBnClickedStartService();
	//ֹͣ����
	afx_msg void OnBnClickedStopService();
	//���÷���
	afx_msg void OnBnClickedInitService();
	//�����û�
	afx_msg void OnBnClickedEnumConnectUser();
	//������Ϣ�ɹ�
	afx_msg LRESULT OnRequestInfoOk(WPARAM w, LPARAM l);
	//��������
	afx_msg LRESULT OnShowUserCountNotify(WPARAM w, LPARAM l);
	//��ʱ
	afx_msg void OnTimer(UINT nIDEvent);

	DECLARE_MESSAGE_MAP()

};

//////////////////////////////////////////////////////////////////////////
