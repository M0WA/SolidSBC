#pragma once

class CSolidSBCSocket
{
protected:
	CSolidSBCSocket(void);

public:
	~CSolidSBCSocket(void);
	virtual void Close(void);

protected:
	SOCKET m_hSocket;
};

