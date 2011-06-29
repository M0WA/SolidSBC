// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SOLIDSBCNETLIB_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SOLIDSBCNETLIB_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SOLIDSBCNETLIB_EXPORTS
#define SOLIDSBCNETLIB_API __declspec(dllexport)
#else
#define SOLIDSBCNETLIB_API __declspec(dllimport)
#endif

// This class is exported from the SolidSBCNetLib.dll
class SOLIDSBCNETLIB_API CSolidSBCServerSocket {
public:
	CSolidSBCServerSocket(void);
	CSolidSBCServerSocket(const unsigned short& nListenPort);
	~CSolidSBCServerSocket(void);

	bool StartListen(const unsigned short& nListenPort);
	bool StartListen(const struct sockaddr_in& server);
	void Close(void);

protected:
	SOCKET m_hSocket;
	
	//do not call this functions beyond on your own
public:
	virtual bool OnAccept(SOCKET hNewClientSock)=0;
	bool WaitForClient(struct sockaddr_in* pClient);
};

// This class is exported from the SolidSBCNetLib.dll
class SOLIDSBCNETLIB_API CSolidSBCClientSocket {

public:
	CSolidSBCClientSocket(void);
	~CSolidSBCClientSocket(void);

	void Connect(const struct sockaddr_in& target, const int& nTimeoutMS = 5000);
	void Close(void);

	void GetNextPacket(void);

protected:	
	SOCKET m_hSocket;

private:
	int    m_nConnectTimeout;

	//do not call this functions beyond on your own
public:
	virtual bool OnConnect(bool bSuccess)=0;
	virtual bool OnRead()=0;
	bool WaitForConnect();
	bool WaitForPacket();
};