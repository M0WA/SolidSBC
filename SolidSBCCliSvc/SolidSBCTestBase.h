#pragma once


class CVMPerfTestBase
{
public:
	CVMPerfTestBase(void);
	~CVMPerfTestBase(void);
	int  Start( AFX_THREADPROC threadFunc, HWND hWnd, LPVOID threadParam );
	int  Stop(void);

protected:
	CWinThread* m_pThread;
	CMutex      m_cStopMutex;
	BOOL        m_bShallStop;

private:
	VMPERF_TEST_BASE_THREAD_PARAM m_baseThreadParam;
};
