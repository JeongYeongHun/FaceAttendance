#pragma once

// CClientSockServer 명령 대상입니다.

class CClientSockServer : public CAsyncSocket
{
public:
	CClientSockServer();
	virtual ~CClientSockServer();
	void OnReceive(int nErrorCode);   //추가된 함수
};

