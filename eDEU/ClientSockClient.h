#pragma once

// CClientSockClient ��� ����Դϴ�.

class CClientSockClient : public CAsyncSocket
{
public:
	CClientSockClient();
	virtual ~CClientSockClient();
	void OnReceive(int nErrorCode);    //�߰��� �Լ�
};

