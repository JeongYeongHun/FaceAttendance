// ServerSock.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "eDeuProject.h"
#include "ServerSock.h"
#include "eDeuProjectDlg.h"

// CServerSock

CServerSock::CServerSock()
{
}

CServerSock::~CServerSock()
{
}


// CServerSock ��� �Լ�
void CServerSock::OnAccept(int nErrorCode)
// ���̾�α׿��� OnAccept�� ȣ���Ҷ� �ҷ��鿩���� ������ �����.
{
	TRACE("CeDeuProject::OnAccept(nErrorCode = %d)\n", nErrorCode);  //������ �ڵ� 
	CeDeuProjectDlg* dlg = (CeDeuProjectDlg*)AfxGetApp()->m_pMainWnd;
	//���̾�α� Ŭ������ �޾ƿ�
	dlg->OnAccept();      // ���̾�α� OnAccept ȣ�� 
	CAsyncSocket::OnAccept(nErrorCode);
}