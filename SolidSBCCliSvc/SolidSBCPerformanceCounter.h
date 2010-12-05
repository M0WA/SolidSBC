#pragma once

class CPerformanceCounter
{
public:
	CPerformanceCounter(void);
	~CPerformanceCounter(void);
	void   Start(void);
	double Stop(void);
	
private:
	LARGE_INTEGER m_liStart;
	LARGE_INTEGER m_liFrequency;
};
