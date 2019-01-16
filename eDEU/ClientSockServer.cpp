// ClientSockServer.cpp : 구현 파일입니다.
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


// CClientSockServer 멤버 함수
void CClientSockServer::OnReceive(int nErrorCode)    //서버에  메시지가 들어올때마다 자동으로 호출됨
{
	TRACE("CeDeuProject::OnReceiveS(nErrorCode = %d)\n", nErrorCode);
	//디버깅용 코드



	CeDeuProjectDlg* dlg = (CeDeuProjectDlg*)AfxGetApp()->m_pMainWnd;
	// 다이얼로그 클래스를 받아옴
	dlg->OnReceiveS(this);                                                                // OnReceiveS를 호출  하는데 Why? this??

	CAsyncSocket::OnReceive(nErrorCode);
}