#pragma once

// CClientSockClient 명령 대상입니다.

class CClientSockClient : public CAsyncSocket
{
public:
	CClientSockClient();
	virtual ~CClientSockClient();
	void OnReceive(int nErrorCode);    //추가된 함수
};

