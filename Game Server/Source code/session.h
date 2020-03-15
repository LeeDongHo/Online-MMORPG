#pragma once

class MMOServer;

enum en_SESSION_MODE
{
	MODE_NONE = 0,
	MODE_AUTH, MODE_AUTH_TO_GAME, MODE_LOGOUT_IN_AUTH,
	MODE_GAME, MODE_LOGOUT_IN_GAME,
	WAIT_LOGOUT
};

class GameSession
{
public:
	SOCKET sock;
	SESSION_ID  clientID;
	unsigned int	arryIndex;				// ������ �迭������ �ε���


	en_SESSION_MODE	Mode;		// ���� ������ ���

	winBuffer		recvQ;
	lockFreeQueue<Sbuf*> completeRecvQ;
	
	lockFreeQueue<Sbuf*> sendQ;
	unsigned int sendCount;

	OVERLAPPED recvOver;
	OVERLAPPED sendOver;

	LONG sendFlag;
	LONG IOCount;

	bool logoutFlag;					// IOCP COUNT 0 �÷��� (�� �÷��װ� TRUE�̸� LOGOUT)
	bool authTOgame;				// AUTH -> GAME üũ �÷���
	LONG disconnectFlag;			// ������ ���� �÷���

	BYTE bufCode;
	BYTE bufKey1;
	BYTE bufKey2;

	// test
	MMOServer *server;

public:
	GameSession();
	~GameSession();

	void sendPacket(Sbuf* _buf, bool _type = false);
	void disconnect(void);

	virtual void onAuth_clientJoin(void) = 0;		
	virtual void onAuth_clientLeave(void) = 0;
	virtual void onAuth_Packet(Sbuf* _buf) = 0;

	virtual void onGame_clientJoin(void) = 0;
	virtual void onGame_clientLeave(void) = 0;
	virtual void onGame_Packet(Sbuf *_buf) = 0;
	
	virtual void onGame_Release(void) = 0;

	void set_bufCode(BYTE _bufCode, BYTE _bufKey1, BYTE _bufKey2);
	void set_session(SOCKET _sock);

	//test
	void set_server(MMOServer* _server);
};

