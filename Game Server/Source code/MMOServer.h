#pragma once

struct AUTH_DATA
{
	SOCKET sock;
	SOCKADDR_IN sockAddr;
};

class MMOServer
{
private:
	// CONFIG VALUE
	bool nagleOpt;
	WCHAR ip[16];
	unsigned short port;
	unsigned char workerCount;

	// SOCKET
	SOCKET listenSock;
	tcp_keepalive tcpKeep;

	// HANDLE
	HANDLE *workerHandle;
	HANDLE IOCPHandle;

	bool threadFlag;

	LONG acceptTPS;

	// Monitoring
	LONG recvTPS, sendTPS;				// TPS
	LONG authCount, gameCount;			// ������
	LONG sendCount;
	LONG authToGameCount,logoutCount;
	LONG authFrame, gameFrame, sendFrame;	// �����Ӽ�

protected:
	BYTE bufCode;
	BYTE bufKey1;
	BYTE bufKey2;

	unsigned int maxSession;
	GameSession	 **sessionArry;
	lockFreeStack<unsigned int> indexStack;

	lockFreeQueue<AUTH_DATA*> AUTHQ;

	LONG	 sessionCount;
	ULONG64 acceptTotal;

	LONG	pAcceptTPS, pRecvTPS, pSendTPS, pSendCount;
	LONG	pAuth, pGame, pAuthToGame, pLogout;
	LONG pAuthFrame, pGameFrame, pSendFrame;
	ULONGLONG pProcPacket;
public:
	LONG comQ, pcomQ;
	LONG sdQ, psdQ;

private:
	void loadConfig(const char* _configData);

	bool connectRequest(SOCKADDR_IN _sockAddr);
	void recvPost(GameSession *_ss);
	void sendPost(GameSession *_ss);
	void completeSend(DWORD _trans, GameSession *_ss);
	void completeRecv(DWORD _trans, GameSession *_ss);

public:
	MMOServer() {};
	~MMOServer() {};

	void setSessionArry(player *_array, unsigned int _maxSession);
	// ������
	static unsigned __stdcall acceptThread(LPVOID _data);

	static unsigned __stdcall AUTHThread(LPVOID _data);
	void			acceptProcess(void);
	void			checkProcess(void);
	void			AUTHMODE(void);

	static unsigned __stdcall GameThread(LPVOID _data);
	void			GAMEMODE(void);
	void			gamePacket(void);
	void			logoutProcess(void);
	void			Release(void);

	
	static unsigned __stdcall SendThread(LPVOID _data);

	static unsigned __stdcall WorkerThread(LPVOID _data);


	// �Լ�
	bool Start(const char *_buffer);
	void Stop(void);

	void monitoring(void);

	// �����Լ�
	virtual void onAuth_Update(void) = 0;
	virtual void onGame_Update(void) = 0;

	friend class player;

};

