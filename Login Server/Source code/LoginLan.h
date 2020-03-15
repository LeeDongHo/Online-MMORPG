#pragma once

class LoginServer;

struct connectedServer
{
	unsigned __int64 Index = 0;
	short	serverType;
	short	serverPort;
	char	serverNumber;
	char	serverIp[16];
};

class loginLan : public LanServer
{
private:
	char IP[16];
	unsigned short Port;
	unsigned short workerCount;
	bool nagleOpt;
	unsigned int maxSessionCount;
	unsigned int serverCount;

	std::vector<connectedServer*> serverList;

	bool monitorThreadFlag;

	LoginServer *loginServer;

private:
	void loadConfig(const char *_configData);

	static unsigned __stdcall monitorThread(LPVOID _data);

	void OnClientJoin(unsigned __int64 _Index);
	void OnClientLeave(unsigned __int64 _Index);
	void OnRecv(unsigned __int64 _Index, Sbuf *_buf);		// ���� �Ϸ� ��
	void OnSend(unsigned __int64 _Index, int _sendSize);	// �۽� �Ϸ� ��

	void OnError(int _errorCode, WCHAR *_string);		// �����޼��� ����

	// process
	void proc_serverLogin(unsigned __int64 _Index, Sbuf *_buf);

	Sbuf* packet_userAuthToServer(unsigned __int64 _Index, char *_oid, unsigned __int64 _acNo);
	Sbuf* packet_serverLoginRes();
protected:
	void proc_userAuthFromServer(unsigned __int64 _Index, Sbuf *_buf);

public:
	loginLan(const char *_configData, LoginServer *_serverPtr);
	~loginLan();

	void proc_userAuthToServer(unsigned __int64 _Index, char *_oid, unsigned __int64 _acNo);
	void proc_userLogout(unsigned __int64 _Index, Sbuf *_buf);
	Sbuf* packet_userGameConnect(unsigned __int64 _Index);
};