// ClientSockClient.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "eDeuProject.h"
#include "ClientSockClient.h"
#include "eDeuProjectDlg.h"


// CClientSockClient

CClientSockClient::CClientSockClient()
{
}

CClientSockClient::~CClientSockClient()
{
}


// CClientSockClient ��� �Լ�
void CClientSockClient::OnReceive(int nErrorCode)
//Client�� ���ú� �Լ�  Ŭ���̾�Ʈ�� �޽����� �ö����� �ڵ�����  ���̾�α��� //OnReceiveC ��  ȣ���Ѵ�.
{
	TRACE("CSockAll::OnReceiveC(nErrorCode = %d)\n", nErrorCode);
	//������ �ڵ�
	CeDeuProjectDlg* dlg = (CeDeuProjectDlg*)AfxGetApp()->m_pMainWnd;
	// ���̾�α� Ŭ������ �޾ƿ� 
	dlg->OnReceiveC();                                     // ���̾�α��� OnReceiveC�� ȣ����
	CAsyncSocket::OnReceive(nErrorCode);
}