#pragma once


struct user {
	unsigned __int64 ssIndex;		// index ID
	__int64 accountNo;	// DB�� accountNo

	bool gameConnect = false;
	bool chatConnect = false;

	charArr *character = NULL;
	bool loginFlag = false;
};

class LoginServer : protected IOCPServer
{
private:

	bool printFlag;			// ��½�����
	bool whiteFlag;		// ȭ��Ʈ ������ �ɼ�
	bool nagleOpt;			// NAGLE OPTION

	char				ip[16];// ip�ּ�
	unsigned short port;	// ��Ʈ

	unsigned int maxClient;
	unsigned int threadCount;

	std::list <user*> userList;	// ���� ���� ����Ʈ
	
	memoryPool<user> *userPool;
	memoryPool<charArr> *loadPool;

	SRWLOCK userLock;

	int loginWait;
	int loginSuccess;

	// Encryption Code Key
	BYTE Code, Key1, Key2;

public:
	loginDB* DB;
	loginLan *lanServer;

private:
	// config
	void loadConfig(const char* _config);

	// thread
	static unsigned __stdcall printThread(LPVOID _data);
	
	// �����Լ�
	virtual void OnClinetJoin(unsigned __int64 _index);	// accept -> ����ó�� �Ϸ� �� ȣ��
	virtual void OnClientLeave(unsigned __int64 _index);		// disconnect �� ȣ��
	virtual bool OnConnectionRequest(char *_ip, unsigned int _port); // accept �� [false : Ŭ���̾�Ʈ �ź� / true : ���� ���]
	virtual void OnRecv(unsigned __int64 _index, Sbuf *_buf);		// ���� �Ϸ� ��
	virtual void OnError(int _errorCode, WCHAR *_string);		// �����޼��� ����

	// �����Լ�
	user* getUser(unsigned __int64 _index);

	bool searchOverlapped(__int64 _accountNo);

	void proc_signUp(unsigned __int64 _index, Sbuf *_buf);			// Ŭ���̾�Ʈ ȸ������ 
	void proc_withDrawal(unsigned __int64 _Index, Sbuf *_buf);		// Ŭ���̾�Ʈ ȸ�� Ż��

	void proc_createChar(unsigned __int64 _Index, Sbuf *_buf);		// ĳ���� ����
	void proc_deleteChar(unsigned __int64 _Index, Sbuf *_buf);		// ĳ���� ����
	void proc_selectChar(unsigned __int64 _Index, Sbuf *_buf);		// ĳ���� ����
	void proc_charList(unsigned __int64 _Index, Sbuf *_buf);			// ĳ���� ���� ��û
	char* getOID(WCHAR *_name, charArr *_data);

	void proc_userLogin(unsigned __int64 _index, Sbuf* _buf);		// Ŭ���̾�Ʈ �α��� ó��
	void proc_goodbyeUser(unsigned __int64 _index);

	Sbuf* packet_sendResult(short _Type, char _Result);	// ����� ���� ��
	Sbuf* packet_charList(short _Type, unsigned char _Result, charArr *_data);	// ĳ���� ���� ���� ��  ��
	Sbuf* packet_serverAuth(int _acNo, unsigned __int64 _sessionKey, char* _charNo);

public:
	LoginServer(const char *_config);
	~LoginServer();

	void terminateServer(void);

	void renewVariable(int *_wait, int *_success);

	void proc_userAuthFromServer(unsigned __int64 _Index, int _serverType);

	friend class loginDB;
};