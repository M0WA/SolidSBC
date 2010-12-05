#include "stdafx.h"
#include "SolidSBCPerformanceCounter.h"

CPerformanceCounter::CPerformanceCounter(void)
{
	QueryPerformanceFrequency( &m_liFrequency );
}

CPerformanceCounter::~CPerformanceCounter(void)
{
}

double CPerformanceCounter::Stop(void) 
{ 
	LARGE_INTEGER liPerfEnd;
	QueryPerformanceCounter( &liPerfEnd ); 
	double dTimeDiff = (((double)(liPerfEnd.QuadPart-m_liStart.QuadPart)) / ((double)m_liFrequency.QuadPart)); 
	return dTimeDiff;
}

void CPerformanceCounter::Start(void) 
{ 
	QueryPerformanceCounter( &m_liStart ); 
}
