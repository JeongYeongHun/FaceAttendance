// ClientSockClient.cpp : 구현 파일입니다.
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


// CClientSockClient 멤버 함수
void CClientSockClient::OnReceive(int nErrorCode)
//Client의 리시브 함수  클라이언트에 메시지가 올때마다 자동으로  다이얼로그의 //OnReceiveC 를  호출한다.
{
	TRACE("CSockAll::OnReceiveC(nErrorCode = %d)\n", nErrorCode);
	//디버깅용 코드
	CeDeuProjectDlg* dlg = (CeDeuProjectDlg*)AfxGetApp()->m_pMainWnd;
	// 다이얼로그 클래스를 받아옴 
	dlg->OnReceiveC();                                     // 다이얼로그의 OnReceiveC를 호출함
	CAsyncSocket::OnReceive(nErrorCode);
}