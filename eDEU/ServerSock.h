#pragma once

// CServerSock ��� ����Դϴ�.

class CServerSock : public CAsyncSocket
{
public:
	CServerSock();
	virtual ~CServerSock();
	void OnAccept(int nErrorCode);   //�߰��� �Լ�
};

