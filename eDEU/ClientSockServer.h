#pragma once

// CClientSockServer ��� ����Դϴ�.

class CClientSockServer : public CAsyncSocket
{
public:
	CClientSockServer();
	virtual ~CClientSockServer();
	void OnReceive(int nErrorCode);   //�߰��� �Լ�
};

