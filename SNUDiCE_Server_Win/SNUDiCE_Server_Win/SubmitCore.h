#pragma once
#include "network.h"


class gSubmitCore
{
public:
	static gSubmitCore *GetIF();

public:
	bool			m_isValidSubmitSubject[MAXROOM];
	BYTE			m_submitSubject[MAXROOM][CLASSNUM][CLASSSEAT];
	BYTE			m_submitSubjectPlayer[MAXROOM][ROOMMAXPLAYER][6];
	bool			m_isFinishSubmitSubject[MAXROOM][ROOMMAXPLAYER];

public:
	bool			SetUp();
	void			init(int nRoomindex);
	void			Release();

	void			pk_gamestart_rep(int nRoomIndex);
	void			pk_submit_ask(PK_DEFAULT *pk, SOCKET sock);
	void			pk_submitready_ask(PK_DEFAULT *pk,SOCKET sock);
	

	// packet

	//void			pk_message_ask(PK_DEFAULT *pk, SOCKET sock);

	// �߿�! put�� push �� �����Ǿ����.
	// ����, ȣ���Ҷ��� **Subject�� ȣ���ؾ� �ϸ� ToPlayer�� ȣ������ ����.
	// m_submitSubject�� m_submitSubjectPlayer����..
	// ���� �̷� ���� �Ѱɱ�...

private:
	bool			putSubject(int nRoomIndex,int nInRoomIndex,int nSubjectIndex);
	void			putSubjectToPlayer(int nRoomIndex, int nInRoomIndex, int nSubjectIndex);
	bool			pushSubject(int nRoomIndex,int nInRoomIndex,int nSubjectIndex);
	void			pushSubjectToPlayer(int nRoomIndex,int nInRoomIndex,int nSubjectIndex);
	bool			isSubjectFull(int nRoomIndex,int nInRoomIndex);
	bool			isSubjectExisted(int nRoomIndex,int nInRoomIndex,int nSubjectIndex);
	bool			isFinishAllSubmit(int nRoomIndex);
};