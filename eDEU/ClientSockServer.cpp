// ClientSockServer.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "eDeuProject.h"
#include "ClientSockServer.h"
#include "eDeuProjectDlg.h"


// CClientSockServer

CClientSockServer::CClientSockServer()
{
}

CClientSockServer::~CClientSockServer()
{
}


// CClientSockServer ��� �Լ�
void CClientSockServer::OnReceive(int nErrorCode)    //������  �޽����� ���ö����� �ڵ����� ȣ���
{
	TRACE("CeDeuProject::OnReceiveS(nErrorCode = %d)\n", nErrorCode);
	//������ �ڵ�



	CeDeuProjectDlg* dlg = (CeDeuProjectDlg*)AfxGetApp()->m_pMainWnd;
	// ���̾�α� Ŭ������ �޾ƿ�
	dlg->OnReceiveS(this);                                                                // OnReceiveS�� ȣ��  �ϴµ� Why? this??

	CAsyncSocket::OnReceive(nErrorCode);
}