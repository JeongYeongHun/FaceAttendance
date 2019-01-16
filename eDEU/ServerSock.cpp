// ServerSock.cpp : 구현 파일입니다.
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


// CServerSock 멤버 함수
void CServerSock::OnAccept(int nErrorCode)
// 다이얼로그에서 OnAccept를 호출할때 불러들여져서 서버를 만든다.
{
	TRACE("CeDeuProject::OnAccept(nErrorCode = %d)\n", nErrorCode);  //디버깅용 코드 
	CeDeuProjectDlg* dlg = (CeDeuProjectDlg*)AfxGetApp()->m_pMainWnd;
	//다이얼로그 클래스를 받아옴
	dlg->OnAccept();      // 다이얼로그 OnAccept 호출 
	CAsyncSocket::OnAccept(nErrorCode);
}