#pragma once

// CServerSock 명령 대상입니다.

class CServerSock : public CAsyncSocket
{
public:
	CServerSock();
	virtual ~CServerSock();
	void OnAccept(int nErrorCode);   //추가된 함수
};

