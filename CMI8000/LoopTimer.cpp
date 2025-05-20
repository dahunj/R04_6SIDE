// LoopTimer.cpp : 구현 파일
//
#include "StdAfx.h"
#include "LoopTimer.h"

#include "LogFile.h"
#include "DataManager.h"

CLoopTimer::CLoopTimer()
{
	m_dwLoopStart = 0;
	m_dwLoopTerm = 0;

	m_bWaitingRun = FALSE;
	m_dwWaitingStart = 0;
}

CLoopTimer::~CLoopTimer()
{
}

void CLoopTimer::Set_LoopTime(DWORD msec)
{
	m_dwLoopStart = GetTickCount();
	m_dwLoopTerm = msec;
}

BOOL CLoopTimer::Over_LoopTime()
{
//	return FALSE;	// No Timeout Error

	DWORD dwTerm = GetTickCount() - m_dwLoopStart;
	if (dwTerm >= m_dwLoopTerm)
		return TRUE;
	else
		return FALSE;
}

BOOL CLoopTimer::Delay_LoopTime(DWORD msec)
{
	DWORD dwTerm = GetTickCount() - m_dwLoopStart;
	if (dwTerm >= msec) return TRUE;
	else return FALSE;
}

BOOL CLoopTimer::Waiting_Time(DWORD msec)
{
	if (!m_bWaitingRun) {
		m_bWaitingRun = TRUE;
		m_dwWaitingStart = GetTickCount();
	}

	DWORD dwTerm = GetTickCount() - m_dwWaitingStart;
	if (dwTerm < msec) return FALSE;
	
	m_bWaitingRun = FALSE;
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////

void CLoopTimer::Takt_Start()
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (!pEquipData->bJobListTaktLog) return;

	m_dwTakt = GetTickCount();
}

void CLoopTimer::Takt_Save(int nFun, int nId)
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (!pEquipData->bJobListTaktLog) return;

	CString strFun, strLog, strMsg;
	switch (nFun) {
	case 1:		// LoadTray
		strFun = "LoadTray";
		switch (nId) {
		case  1: strMsg = "Z Axis Move to Support Up Position"; break;
		case  2: strMsg = "Port Support Out"; break;
		case  3: strMsg = "Z Axis Move to Support Down Position"; break;
		case  4: strMsg = "Port Support In"; break;
		case  5: strMsg = "Z Axis Move to Ready Down Position"; break;
		case  6: strMsg = "Master/Slave In"; break;
		case  7: strMsg = "X Axis Move to Unload Position"; break;
		case  8: strMsg = "Z Axis Move to Unload Up Position"; break;
		case  9: strMsg = "Z Axis Move to Ready Down Position"; break;
		case 10: strMsg = "X Axis Move to Load Position"; break;
		}
		break;
	case 2:		// LoadPicker
		strFun = "LoadPicker";
		switch (nId) {
		case 1: strMsg = "Z Cylinder Down"; break;
		case 2: strMsg = "Load Tray Master/Slave Out"; break;
		case 3: strMsg = "Load Picker Master/Slave In"; break;
		case 4: strMsg = "Z Cylinder Up"; break;
		case 5: strMsg = "Y Axis Move to Angle Port Position"; break;
		case 6: strMsg = "Z Cylinder Down"; break;
		case 7: strMsg = "Load Picker Master/Slave Out"; break;
		case 8: strMsg = "Z Cylinder Up"; break;
		case 9: strMsg = "Y Axis Move to Load Tray Position"; break;
		}
		break;
	case 3:		// VisionAngle
		strFun = "VisionAngle";
		switch (nId) {
		case  1: strMsg = "Port Master/Slave In"; break;
		case  2: strMsg = "Align Z Axis Move to Down Position"; break;
		case  3: strMsg = "Align Scan Complete"; break;
		case  4: strMsg = "Align Z Axis Move to Ready Position"; break;
		case  5: strMsg = "Angle Unit Move to Inspection Start Position"; break;
		case  6: strMsg = "Angle Unit Move to Inspection Position"; break;
		case  7: strMsg = "Angle Scan Complete"; break;
		case  8: strMsg = "Port Master/Slave Out"; break;
		case  9: strMsg = "Angle Unit Move to Ready Position"; break;
		case 10: strMsg = "Z Axis Focus Move"; break;
		}
		break;
	case 4:		// AngleTray1
		strFun = "AngleTray1";
		switch (nId) {
		case  1: strMsg = "Z Axis Move to Support Up Position"; break;
		case  2: strMsg = "Angle Port Support Out, Stage Vacuum On"; break;
		case  3: strMsg = "Z Axis Move to Ready Down Position"; break;
		case  4: strMsg = "Angle Port Support In"; break;
		case  5: strMsg = "Y Axis Move to Ready Position"; break;
		case  6: strMsg = "Z Axis Move to Pick Up Position"; break;
		case  7: strMsg = "Y Axis Move to Picker L1 Position"; break;
		case  8: strMsg = "Y Axis Move to Tray Unload Position"; break;
		case  9: strMsg = "Z Axis Move to Ready Down Position"; break;
		case 10: strMsg = "Y Axis Move to Tray Load Position"; break;
		case 11: strMsg = "Z Axis Move to Tray Pick Up Position"; break;
		}
		break;
	case 5:		// AngleTray2
		strFun = "AngleTray2";
		switch (nId) {
		case  1: strMsg = "Z Axis Move to Support Up Position"; break;
		case  2: strMsg = "Angle Port Support Out, Stage Vacuum On"; break;
		case  3: strMsg = "Z Axis Move to Ready Down Position"; break;
		case  4: strMsg = "Angle Port Support In"; break;
		case  5: strMsg = "Y Axis Move to Ready Position"; break;
		case  6: strMsg = "Z Axis Move to Pick Up Position"; break;
		case  7: strMsg = "Y Axis Move to Picker L1 Position"; break;
		case  8: strMsg = "Y Axis Move to Tray Unload Position"; break;
		case  9: strMsg = "Z Axis Move to Ready Down Position"; break;
		case 10: strMsg = "Y Axis Move to Tray Load Position"; break;
		case 11: strMsg = "Z Axis Move to Tray Pick Up Position"; break;
		}
		break;
	case 6:		// Btm1Picker
		strFun = "Btm1Picker";
		switch (nId) {
		case  1: strMsg = "X, Angle Y Axis Move to Loading Position"; break;
		case  2: strMsg = "Module Loading (Down + VacuumOn + Up) In Angle Tray"; break;
		case  3: strMsg = "X Axis Move to Btm1 Inspection Position Pitch"; break;
		case  4: strMsg = "Z Axis Focus Move"; break;
		case  5: strMsg = "Z Axis Ready Up"; break;
		case  6: strMsg = "X Axis Move to Unloading Position"; break;
		case  7: strMsg = "Module Unloading (Down + VacuumOff + Up) In Inspect Stage"; break;
		case  8: strMsg = "X Axis Move to Loading Position"; break;
		case  9: strMsg = "Shift Y Axis Move"; break;
		case 10: strMsg = "Z Axis Btm1 Inspect Down, Shift Y Axis Move to Ready Position (Set_LoadComplete)"; break;
		case 11: strMsg = "X Axis Move to Btm1 Inspection Position"; break;
		}
		break;
	case 7:		// Inspection1
		strFun = "Inspection1";
		switch (nId) {
		case  1: strMsg = "Move to Module Align Position"; break;
		case  2: strMsg = "Module Align Complete"; break;
		case  3: strMsg = "Move to Top1 Position (Set_LoadComplete)"; break;
		case  4: strMsg = "Top1 Z Axis Focus Move"; break;
		case  5: strMsg = "Top1 Mirror Z Axis Move"; break;
		case  6: strMsg = "Move to Top2 Position (Set_LoadComplete)"; break;
		case  7: strMsg = "Top2 Z Axis Focus Move"; break;
		case  8: strMsg = "Top2 Shift Move"; break;
		case  9: strMsg = "Move to Unloading Position"; break;
		case 10: strMsg = "Stage Down"; break;
		case 11: strMsg = "Move to Loading Position"; break;
		case 12: strMsg = "Stage Up"; break;
		case 13: strMsg = "Move to Top2 Position"; break;
		case 14: strMsg = "Move to Top1 Position"; break;
		}
		break;
	case 8:		// Inspection2
		strFun = "Inspection2";
		switch (nId) {
		case  1: strMsg = "Move to Module Align Position"; break;
		case  2: strMsg = "Module Align Complete"; break;
		case  3: strMsg = "Move to Top1 Position (Set_LoadComplete)"; break;
		case  4: strMsg = "Top1 Z Axis Focus Move"; break;
		case  5: strMsg = "Top1 Mirror Z Axis Move"; break;
		case  6: strMsg = "Move to Top2 Position (Set_LoadComplete)"; break;
		case  7: strMsg = "Top2 Z Axis Focus Move"; break;
		case  8: strMsg = "Top2 Shift Move"; break;
		case  9: strMsg = "Move to Unloading Position"; break;
		case 10: strMsg = "Stage Backward & Down"; break;
		case 11: strMsg = "Move to Loading Position"; break;
		case 12: strMsg = "Stage Forward & Up"; break;
		case 13: strMsg = "Move to Top2 Position"; break;
		case 14: strMsg = "Move to Top1 Position"; break;
		}
		break;
	case 9:		// Inspection3
		strFun = "Inspection3";
		switch (nId) {
		case  1: strMsg = "Move to Module Align Position"; break;
		case  2: strMsg = "Module Align Complete"; break;
		case  3: strMsg = "Move to Top1 Position (Set_LoadComplete)"; break;
		case  4: strMsg = "Top1 Z Axis Focus Move"; break;
		case  5: strMsg = "Top1 Mirror Z Axis Move"; break;
		case  6: strMsg = "Move to Top2 Position (Set_LoadComplete)"; break;
		case  7: strMsg = "Top2 Z Axis Focus Move"; break;
		case  8: strMsg = "Top2 Shift Move"; break;
		case  9: strMsg = "Move to Unloading Position"; break;
		case 10: strMsg = "Stage Backward & Down"; break;
		case 11: strMsg = "Move to Loading Position"; break;
		case 12: strMsg = "Stage Forward & Up"; break;
		case 13: strMsg = "Move to Top2 Position"; break;
		case 14: strMsg = "Move to Top1 Position"; break;
		}
		break;
	case 10:	// Btm2Picker
		strFun = "Btm2Picker";
		switch (nId) {
		case  1: strMsg = "Module Loading (Down + VacuumOn + Up) In Inspect Stage"; break;
		case  2: strMsg = "X Axis Move to Btm2 Inspection Position Pitch"; break;
		case  3: strMsg = "Z Axis Move to Btm2 Inspection Down (Set_LoadComplete)"; break;
		case  4: strMsg = "Z Axis Focus Move"; break;
		case  5: strMsg = "Z Axis Ready Up"; break;
		case  6: strMsg = "X, Buffer Y Axis Move to Unloading Position"; break;
		case  7: strMsg = "Module Unloading (Down + VacuumOff + Up) In Buffer Tray"; break;
		case  8: strMsg = "X Axis Move to Loading Position"; break;
		case  9: strMsg = "X Axis Move to Btm2 Inspection Position"; break;
		case 10: strMsg = "Btm2 Picker Turn Complete"; break;
		}
		break;
	case 11:	// BufferTray1
		strFun = "BufferStage1";
		switch (nId) {
		case 1: strMsg = "Satge1 Y Axis Move to Sort Picker1 Position"; break;
		case 2: strMsg = "Stage1 Down"; break;
		case 3: strMsg = "Stage1 Y Axis Move to Btm2 Picker L1 Position"; break;
		case 4: strMsg = "Stage1 Up"; break;
		}
		break;
	case 12:	// BufferTray2
		strFun = "BufferStage2";
		switch (nId) {
		case 1: strMsg = "Satge2 Y Axis Move to Sort Picker1 Position"; break;
		case 2: strMsg = "Stage2 Down"; break;
		case 3: strMsg = "Stage2 Y Axis Move to Btm2 Picker L1 Position"; break;
		case 4: strMsg = "Stage2 Up"; break;
		}
		break;
	case 13:	// SortPicker1
		strFun = "SortPicker1";
		switch (nId) {
		case  1: strMsg = "Move to Module Pick Position"; break;
		case  2: strMsg = "Module Loading (Down + VacuumOn + Up) In Buffer Tray"; break;
		case  3: strMsg = "Move to NG Unloading Position"; break;
		case  4: strMsg = "NG Unloading (Down + VacuumOff + Up) In NG Tray"; break;
		case  5: strMsg = "Z Axis Ready Up & Next Waiting"; break;
		case  6: strMsg = "Move to Good Unloading Position"; break;
		case  7: strMsg = "Good Unloading (Down + VacuumOff + Up) In Good Tray"; break;
		case  8: strMsg = "Z Axis Ready Up"; break;
		case  9: strMsg = "Move to Buffer Position"; break;
		case 10: strMsg = "Move to NG Buffer Position"; break;
		case 11: strMsg = "Z Axis Ready Up"; break;
		case 12: strMsg = "Module Loading (Down + VacuumOn + Up) In NG Buffer"; break;
		}
		break;
	case 14:	// SortPicker2
		strFun = "SortPicker2";
		switch (nId) {
		case  1: strMsg = "Move to Module Pick Position"; break;
		case  2: strMsg = "Module Loading (Down + VacuumOn + Up) In Buffer Tray"; break;
		case  3: strMsg = "Move to NG Unloading Position"; break;
		case  4: strMsg = "NG Unloading (Down + VacuumOff + Up) In NG Tray"; break;
		case  5: strMsg = "Z Axis Ready Up & Next Waiting"; break;
		case  6: strMsg = "Move to Good Unloading Position"; break;
		case  7: strMsg = "Good Unloading (Down + VacuumOff + Up) In Good Tray"; break;
		case  8: strMsg = "Z Axis Ready Up"; break;
		case  9: strMsg = "Move to Buffer Position"; break;
		case 10: strMsg = "Move to NG Buffer Position"; break;
		case 11: strMsg = "Z Axis Ready Up"; break;
		case 12: strMsg = "Module Loading (Down + VacuumOn + Up) In NG Buffer"; break;
		}
		break;
	case 15:	// GoodTray1
		strFun = "GoodTray1";
		switch (nId) {
		case  1: strMsg = "Good Tray1 Master/Slave In"; break;
		case  2: strMsg = "Z Axis Move to Moving Up Position"; break;
		case  3: strMsg = "Y Axis Move to Sort Picker1 Position"; break;
		case  4: strMsg = "Z Axis Move to Load Up Position"; break;
		case  5: strMsg = "Z Axis Move to Moving Down Position"; break;
		case  6: strMsg = "Y Axis Move to Unload Position"; break;
		case  7: strMsg = "Z Axis Move to Unload Up Position"; break;
		case  8: strMsg = "Good Tray1 Master/Slave Out"; break;
		case  9: strMsg = "Z Axis Move to Moving Down Position"; break;
		case 10: strMsg = "Y Axis Move to Load Position"; break;
		case 11: strMsg = "Z Axis Move to Moving Up Position"; break;
		}
		break;
	case 16:	// GoodTray2
		strFun = "GoodTray2";
		switch (nId) {
		case  1: strMsg = "Good Tray2 Master/Slave In"; break;
		case  2: strMsg = "Z Axis Move to Moving Up Position"; break;
		case  3: strMsg = "Y Axis Move to Sort Picker1 Position"; break;
		case  4: strMsg = "Z Axis Move to Load Up Position"; break;
		case  5: strMsg = "Z Axis Move to Moving Down Position"; break;
		case  6: strMsg = "Y Axis Move to Unload Position"; break;
		case  7: strMsg = "Z Axis Move to Unload Up Position"; break;
		case  8: strMsg = "Good Tray2 Master/Slave Out"; break;
		case  9: strMsg = "Z Axis Move to Moving Down Position"; break;
		case 10: strMsg = "Y Axis Move to Load Position"; break;
		case 11: strMsg = "Z Axis Move to Moving Up Position"; break;
		}
		break;
	case 17:	// NgTray
		strFun = "NgTray";
		switch (nId) {
		case 1: strMsg = "Y Axis Move to Picker2 Tray1 Position"; break;
		case 2: strMsg = "Y Axis Move to Ready Position"; break;
		}
		break;
	case 18:	// EmptyTrayX
		strFun = "EmptyTrayX";
		switch (nId) {
		case 1: strMsg = "Cylinder Down"; break;
		case 2: strMsg = "Angle Stage Vacuum Off"; break;
		case 3: strMsg = "Master/Slave In"; break;
		case 4: strMsg = "Cylinder Up"; break;
		case 5: strMsg = "Move to Empty Port Position"; break;
		case 6: strMsg = "Cylinder Down"; break;
		case 7: strMsg = "Master/Slave Out"; break;
		case 8: strMsg = "Cylinder Up"; break;
		case 9: strMsg = "Move to Angle Stage Position"; break;
		}
		break;
	case 19:	// EmptyTrayElevator
		strFun = "EmptyTrayElevator";
		switch (nId) {
		case 1: strMsg = "Slow Up"; break;
		case 2: strMsg = "Slow down"; break;
		case 3: strMsg = "Move to Base Down Position"; break;
		}
		break;
	case 20:	// EmptyTrayY
		strFun = "EmptyTrayY";
		switch (nId) {
		case 1: strMsg = "Move to Empty Port Position"; break;
		case 2: strMsg = "Cylinder Down"; break;
		case 3: strMsg = "Master/Slave In"; break;
		case 4: strMsg = "Cylinder Up"; break;
		case 5: strMsg = "Move to Good Stage Position"; break;
		case 6: strMsg = "Cylinder Down"; break;
		case 7: strMsg = "Master/Slave Out"; break;
		case 8: strMsg = "Cylinder Up"; break;
		}
		break;
	}

	if (strMsg != "") {
		gLot.dAverageCycle[nFun-1][0][nId-1]++;
		gLot.dAverageCycle[nFun-1][1][nId-1] += (GetTickCount() - m_dwTakt) / 1000.0;
	}

	strLog.Format("MCC,(%02d) %s,(%02d) %s,%0.3lf", nFun, strFun, nId, strMsg, (GetTickCount() - m_dwTakt) / 1000.0);
	//g_objLogFile.Save_JobListLog(strLog);
	g_objLogFile.Save_HandlerLog(strLog);

	strLog.Format("%s,%d,%s,%0.3lf", strFun, nId, strMsg,(GetTickCount() - m_dwTakt) / 1000.0);
	g_objLogFile.Save_JobListExcel(strLog);
}

///////////////////////////////////////////////////////////////////////////////
