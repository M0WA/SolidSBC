#include "stdafx.h"
#include <math.h>
#include "SolidSBCPerformanceCounter.h"
#include "SolidSBCTestThreadCPUMeasure.h"

#pragma optimize( "", off )

/*
ULONGLONG VMPerfTestThreadCPUMeasureCalculateGreatestCommonDivisor(ULONGLONG  a, ULONGLONG b);
UINT VMPerfTestThreadCPUMeasureFloatToFraction( double dFloat, ULONGLONG* pullNumerator, ULONGLONG* pullDenominator, BOOL bSimplifyFraction = TRUE, BOOL bUseSSE2 = TRUE );

ULONGLONG VMPerfTestThreadCPUMeasureCalculateGreatestCommonDivisor(ULONGLONG a, ULONGLONG b)
{
	//calculate greatest common divisor (euclides algorithm)
    if(b == 0)	return a;
    else		return VMPerfTestThreadCPUMeasureCalculateGreatestCommonDivisor(b, a % b);
}

UINT VMPerfTestThreadCPUMeasureFloatToFraction( double dFloat, ULONGLONG* pullNumerator, ULONGLONG* pullDenominator, BOOL bSimplifyFraction, BOOL bUseSSE2 )
{
	//enable sse2 support
	if ( bUseSSE2 )
		_set_SSE2_enable(1);

	int    nSignificantDigits = 8;
	double dPower		      = pow( (double)10.0f, nSignificantDigits );

	double dIntegerPart       = 0;
	double dFloatPart         = modf(dFloat,&dIntegerPart);
	double dPoweredFloatPart  = dFloatPart * dPower;

	//set numerator and denominator
	*pullNumerator   = static_cast<ULONGLONG>(dIntegerPart * dPower) + static_cast<ULONGLONG>(dPoweredFloatPart);
	*pullDenominator = static_cast<ULONGLONG>(dPower);

	if ( bSimplifyFraction ) 
	{
		//simplify fraction
		ULONGLONG ullGCD( VMPerfTestThreadCPUMeasureCalculateGreatestCommonDivisor(*pullNumerator, *pullDenominator) );
		*pullNumerator	 /= ullGCD;
		*pullDenominator /= ullGCD;
	}

	//disable sse2 support
	if ( bUseSSE2 )
		_set_SSE2_enable(0);

	return 0;
}
*/
UINT VMPerfTestThreadCPUMeasureFixMultipliers( double dCnt, UINT nStepCnt, ULONGLONG* pullMtpl, double dFixMultiplierThreshold, UINT nMaxSecs )
{
	// Zeit (soll)	   Multi(soll) 
	// ----------   =  -----------
	// Zeit (ist)      Multi(ist) 

	double dMaxSecs  = (double)nMaxSecs;

	if (  ( nMaxSecs != 0 )										//may and should not be zero 
	   && ( abs(dMaxSecs - dCnt) > dFixMultiplierThreshold )	//time out of threshold? 
	   ){
		double dNewMtlp  = ( dMaxSecs / dCnt ) * ((double)*pullMtpl);
		*pullMtpl = (ULONGLONG)dNewMtlp;
		if ( (*pullMtpl) == 0 )
			*pullMtpl = 1; //zero multiplier, no good, set at least to one
	}	
	return 0;
}

BOOL VMPerfTestThreadCPUMeasureShallEnd(CMutex* pcMutex, LPBOOL pbShallEnd)
{
	BOOL bReturn = TRUE;
	pcMutex->Lock();
	bReturn = *pbShallEnd;
	pcMutex->Unlock();

	return bReturn;
}

UINT VMPerfTestThreadCPUMeasure(LPVOID lParam)
{
	PVMPERF_TEST_BASE_THREAD_PARAM       pParam       = (PVMPERF_TEST_BASE_THREAD_PARAM)lParam;
	PVMPERF_TEST_CPUMEASURE_THREAD_PARAM pThreadParam = (PVMPERF_TEST_CPUMEASURE_THREAD_PARAM)pParam->threadParam;

	CPerformanceCounter cnt, cntOverAll;
	double              dAddCnt = 0, dDivCnt = 0, dOverallCnt, dAddLoop = 0, dDivLoop = 0;
	ULONGLONG			ullAdditionMtlp = 1, ullDivisionMtlp = 1;
	ULONGLONG			ullAddTotalSteps = 0,ullDivTotalSteps = 0;
	ULONGLONG			lCounter = 0; 

	while ( 1 ) {

		dAddCnt  = 0.0f; 
		dDivCnt  = 0.0f; 
		dAddLoop = 0.0f; 
		dDivLoop = 0.0f;

		//start overall counter
		cntOverAll.Start();

		//calculate needed steps
		ullAddTotalSteps = ( ((ULONGLONG)pThreadParam->nAdditionCnt) * ullAdditionMtlp );
		ullDivTotalSteps = ( ((ULONGLONG)pThreadParam->nDivisionCnt) * ullDivisionMtlp );

		/*
		//measure empty add loop
		cnt.Start();
		for (ULONG i = 0; i < ulAddTotalSteps; i++){
		}
		dAddLoop = cnt.Stop();

		//measure empty division loop
		cnt.Start();
		for (ULONG i = 0; i < ulDivTotalSteps; i++){
		}
		dDivLoop = cnt.Stop();
		*/

		//measure add
		cnt.Start();
		if ( pThreadParam->nAdditionCnt ) {
			for (ULONGLONG i = 0; i < ullAddTotalSteps; i++){
				lCounter++;}
		}
		dAddCnt = cnt.Stop();

		//measure division
		cnt.Start();
		if ( pThreadParam->nDivisionCnt ) {
			for (ULONGLONG i = 0; i < ullDivTotalSteps; i++){
				lCounter = lCounter/2;}
		}
		dDivCnt = cnt.Stop(); 

		//hold current multipliers
		ULONGLONG recentAddMultiplier = ullAdditionMtlp;
		ULONGLONG recentDivMultiplier = ullDivisionMtlp;

		//fix multipliers
		if ( pThreadParam->nAutoFixMultiplier ) {
			VMPerfTestThreadCPUMeasureFixMultipliers(dAddCnt, pThreadParam->nAdditionCnt, &ullAdditionMtlp, pThreadParam->dFixMultiplierTreshold, pThreadParam->nAutoFixMultiplier );
			VMPerfTestThreadCPUMeasureFixMultipliers(dDivCnt, pThreadParam->nDivisionCnt, &ullDivisionMtlp, pThreadParam->dFixMultiplierTreshold, pThreadParam->nAutoFixMultiplier );
		}

		dOverallCnt = cntOverAll.Stop();

		//send result 
		//TODO: !!!!!!!!!!!!!!! limit msg/seconds !!!!!!!!!!!!!!!! 
		//                maybe triggers DDOS ATTACK 
		if ( pThreadParam->bTransmitData ) {

			int nPacketSize = sizeof(SSBC_BASE_PACKET_HEADER) + sizeof(SSBC_CPUMEASURE_TESTRESULT_PACKET);
			PBYTE pPacket = new byte[nPacketSize];
			ZeroMemory(pPacket,nPacketSize);

			((PSSBC_BASE_PACKET_HEADER)pPacket)->type = SSBC_CPUMEASURE_TESTRESULT_PACKET_TYPE;
			((PSSBC_BASE_PACKET_HEADER)pPacket)->nPacketSize = nPacketSize;
			
			PSSBC_CPUMEASURE_TESTRESULT_PACKET pResult = ((PSSBC_CPUMEASURE_TESTRESULT_PACKET)&pPacket[sizeof(SSBC_BASE_PACKET_HEADER)]);
			pResult->dAddDuration     = dAddCnt;
			pResult->ullAddMultiplier = recentAddMultiplier;
			pResult->dDivDuration     = dDivCnt;
			pResult->ullDivMultiplier = recentDivMultiplier;
			pResult->dOverallDuration = dOverallCnt;

			g_cClientService.SendTestResult((PSSBC_BASE_PACKET_HEADER)pPacket);
		}
		
		//exit thread?
		if ( VMPerfTestThreadCPUMeasureShallEnd( pParam->pcStopMutex, pParam->pbShallStop ) ) {
			break;}
	}

	delete ((PVMPERF_TEST_CPUMEASURE_THREAD_PARAM)pParam->threadParam);
	pParam->threadParam = NULL;
	return 0;
}

#pragma optimize( "", on )