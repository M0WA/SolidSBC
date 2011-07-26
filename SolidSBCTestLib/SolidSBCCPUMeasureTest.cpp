#include "StdAfx.h"
#include "SolidSBCCPUMeasureTest.h"
#include "SolidSBCCPUMeasureResult.h"
#include "SolidSBCCPUMeasureConfig.h"

#include <math.h>

#pragma optimize( "", off )

UINT SolidSBCTestThreadCPUMeasureFixMultipliers( double dCnt, UINT nStepCnt, ULONGLONG* pullMtpl, double dFixMultiplierThreshold, UINT nMaxSecs );

UINT SolidSBCCPUMeasureTest(LPVOID lpParam)
{
	PSSBC_TEST_THREAD_PARAM    pParam  = (PSSBC_TEST_THREAD_PARAM)lpParam;
	CSolidSBCCPUMeasureConfig* pConfig = (CSolidSBCCPUMeasureConfig*)pParam->pTestConfig;

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
		ullAddTotalSteps = ( ((ULONGLONG)pConfig->GetAdditionCnt()) * ullAdditionMtlp );
		ullDivTotalSteps = ( ((ULONGLONG)pConfig->GetDivisionCnt()) * ullDivisionMtlp );

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
		if ( pConfig->GetAdditionCnt() ) {
			for (ULONGLONG i = 0; i < ullAddTotalSteps; i++){
				lCounter++;}
		}
		dAddCnt = cnt.Stop();

		//measure division
		cnt.Start();
		if ( pConfig->GetDivisionCnt() ) {
			for (ULONGLONG i = 0; i < ullDivTotalSteps; i++){
				lCounter = lCounter/2;}
		}
		dDivCnt = cnt.Stop(); 

		//hold current multipliers
		ULONGLONG recentAddMultiplier = ullAdditionMtlp;
		ULONGLONG recentDivMultiplier = ullDivisionMtlp;

		//fix multipliers
		if ( pConfig->GetAutoFixMultiplier() ) {
			SolidSBCTestThreadCPUMeasureFixMultipliers(dAddCnt, pConfig->GetAdditionCnt(), &ullAdditionMtlp, pConfig->GetFixMultiplierTreshold(), pConfig->GetAutoFixMultiplier() );
			SolidSBCTestThreadCPUMeasureFixMultipliers(dDivCnt, pConfig->GetDivisionCnt(), &ullDivisionMtlp, pConfig->GetFixMultiplierTreshold(), pConfig->GetAutoFixMultiplier() );
		}
		dOverallCnt = cntOverAll.Stop();

		//send result 
		//TODO: !!!!!!!!!!!!!!! limit msg/seconds !!!!!!!!!!!!!!!! 
		//                maybe triggers DDOS ATTACK 
		if ( pConfig->GetTransmitData() ) {

			CSolidSBCCPUMeasureResult* pResult = new CSolidSBCCPUMeasureResult();
			pResult->SetAddDuration  (dAddCnt);
			pResult->SetAddMultiplier(recentAddMultiplier);
			pResult->SetDivDuration  (dDivCnt);
			pResult->SetDivMultiplier(recentDivMultiplier);
			pResult->SetOverallDuration(dOverallCnt);

			CSolidSBCTestThread::AddResult(pParam,(CSolidSBCTestResult*)pResult);
		}
		
		//exit thread?
		if ( CSolidSBCTestThread::ShallThreadEnd( pParam ) ) {
			break;}
	}

	delete pConfig;
	pConfig = NULL;
	return 0;
}

UINT SolidSBCTestThreadCPUMeasureFixMultipliers( double dCnt, UINT nStepCnt, ULONGLONG* pullMtpl, double dFixMultiplierThreshold, UINT nMaxSecs )
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

#pragma optimize( "", on )