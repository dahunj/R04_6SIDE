// SequenceMain.cpp : 구현 파일
//
#include "stdafx.h"
#include "CMI8000.h"
#include "CMI8000Dlg.h"
#include "SequenceMain.h"
#include <math.h>

#include "LogFile.h"
#include "Common.h"
#include "Inspector.h"
#include "MESInterface.h"
#include "Dispatcher.h"
#include "WorkDlg.h"

CSequenceMain g_objSequenceMain;

///////////////////////////////////////////////////////////////////////////////

CSequenceMain::CSequenceMain()
{
	for (int i = 0; i < AXIS_COUNT; i++) m_pStatus[i] = g_objAJinAXL.Get_pStatus(i);
	for (int i = 0; i < AXIS_COUNT; i++) m_pParam[i] = g_objAJinAXL.Get_pParam(i);

	m_pDX00 = g_objAJinAXL.Get_pDX00();
	m_pDX01 = g_objAJinAXL.Get_pDX01();
	m_pDX02 = g_objAJinAXL.Get_pDX02();
	m_pDX03 = g_objAJinAXL.Get_pDX03();
	m_pDX04 = g_objAJinAXL.Get_pDX04();
	m_pDX05 = g_objAJinAXL.Get_pDX05();
	m_pDX06 = g_objAJinAXL.Get_pDX06();
	m_pDX07 = g_objAJinAXL.Get_pDX07();
	m_pDX08 = g_objAJinAXL.Get_pDX08();
	m_pDX09 = g_objAJinAXL.Get_pDX09();
	m_pDX10 = g_objAJinAXL.Get_pDX10();
	m_pDX11 = g_objAJinAXL.Get_pDX11();
	m_pDX12 = g_objAJinAXL.Get_pDX12();
	m_pDX13 = g_objAJinAXL.Get_pDX13();
	m_pDX14 = g_objAJinAXL.Get_pDX14();
	m_pDX15 = g_objAJinAXL.Get_pDX15();

	m_pDY00 = g_objAJinAXL.Get_pDY00();
	m_pDY01 = g_objAJinAXL.Get_pDY01();
	m_pDY02 = g_objAJinAXL.Get_pDY02();
	m_pDY03 = g_objAJinAXL.Get_pDY03();
	m_pDY04 = g_objAJinAXL.Get_pDY04();
	m_pDY05 = g_objAJinAXL.Get_pDY05();
	m_pDY06 = g_objAJinAXL.Get_pDY06();
	m_pDY07 = g_objAJinAXL.Get_pDY07();
	m_pDY08 = g_objAJinAXL.Get_pDY08();
	m_pDY09 = g_objAJinAXL.Get_pDY09();
	m_pDY10 = g_objAJinAXL.Get_pDY10();
	m_pDY11 = g_objAJinAXL.Get_pDY11();
	m_pDY12 = g_objAJinAXL.Get_pDY12();
	m_pDY13 = g_objAJinAXL.Get_pDY13();
	m_pDY14 = g_objAJinAXL.Get_pDY14();
	m_pDY15 = g_objAJinAXL.Get_pDY15();

	m_pEquipData = g_objDataManager.Get_pEquipData();
	m_pMoveData = g_objDataManager.Get_pMoveData();
	m_pRosData = g_objDataManager.Get_pRosData();

	m_strLog = "";

	m_bThreadMainRun = FALSE;
	m_pThreadMainRun = NULL;

	m_nBtm2PickMultiCnt = 0;
	m_nSortPick1MultiCnt = 0;
	m_nSortPick2MultiCnt = 0;
	
	iGoodTrayBufferCount = 0;

	Reset_MainRunCase();
}

CSequenceMain::~CSequenceMain()
{
	if (m_pThreadMainRun) {
		m_bThreadMainRun = FALSE;
		WaitForSingleObject(m_pThreadMainRun->m_hThread, INFINITE);
	}
}

void CSequenceMain::Reset_MainRunCase()
{
	m_nLoadTrayCase			= 0;		//  1. (Error : 3100)
	m_nLoadPickCase			= 0;		//  2. (Error : 3200)
	m_nVisAngleCase			= 0;		//  3. (Error : 3300)
	m_nAngleTray1Case		= 0;		//  4. (Error : 3400)
	m_nAngleTray2Case		= 50;		//  5. (Error : 3500)
	m_nBtm1PickCase			= 0;		//  6. (Error : 3600)
	m_nInspect1Case			= 0;		//  7. (Error : 3700) Center
	m_nInspect2Case			= 70;		//  8. (Error : 3800) Front
	m_nInspect3Case			= 50;		//  9. (Error : 3900) Rear
	m_nBtm2PickCase			= 0;		// 10. (Error : 4000)
	m_nBuffTray1Case		= 0;		// 11. (Error : 4100)
	m_nBuffTray2Case		= 50;		// 12. (Error : 4200)
	m_nSortPick1Case		= 0;		// 13. (Error : 4300) Rear
	m_nSortPick2Case		= 0;		// 14. (Error : 4400) Front
	m_nGoodTray1Case		= 0;		// 15. (Error : 4500)	
	m_nGoodTray2Case		= 50;		// 16. (Error : 4600) 
	m_nNgTrayCase			= 0;		// 17. (Error : 4700) 
	m_nEmptyTrayXCase		= 0;		// 18. (Error : 4800)
	m_nEmptyTrayElCase		= 0;		// 19. (Error : 4900)
	m_nEmptyTrayYCase		= 0;		// 20. (Error : 5000)
}

int *CSequenceMain::Get_pMainRunCase()
{
	static int nCase[20] = { 0 };

	nCase[0]  = m_nLoadTrayCase;		//  1. (Error : 3100)
	nCase[1]  = m_nLoadPickCase;		//  2. (Error : 3200)
	nCase[2]  = m_nVisAngleCase;		//  3. (Error : 3300)
	nCase[3]  = m_nAngleTray1Case;		//  4. (Error : 3400)
	nCase[4]  = m_nAngleTray2Case;		//  5. (Error : 3500)
	nCase[5]  = m_nBtm1PickCase;		//  6. (Error : 3600)
	nCase[6]  = m_nInspect1Case;		//  7. (Error : 3700)
	nCase[7]  = m_nInspect2Case;		//  8. (Error : 3800)
	nCase[8]  = m_nInspect3Case;		//  9. (Error : 3900)
	nCase[9]  = m_nBtm2PickCase;		// 10. (Error : 4000)
	nCase[10] = m_nBuffTray1Case;		// 11. (Error : 4100)
	nCase[11] = m_nBuffTray2Case;		// 12. (Error : 4200)
	nCase[12] = m_nSortPick1Case;		// 13. (Error : 4300)
	nCase[13] = m_nSortPick2Case;		// 14. (Error : 4400)
	nCase[14] = m_nGoodTray1Case;		// 15. (Error : 4500)
	nCase[15] = m_nGoodTray2Case;		// 16. (Error : 4600)
	nCase[16] = m_nNgTrayCase;			// 17. (Error : 4700)
	nCase[17] = m_nEmptyTrayXCase;		// 18. (Error : 4800)
	nCase[18] = m_nEmptyTrayElCase;		// 19. (Error : 4900)
	nCase[19] = m_nEmptyTrayYCase;		// 20. (Error : 5000)

	return nCase;
}

int CSequenceMain::Get_MainRunCase(int nRun)
{
	if		(nRun == AUTO_LOAD_TRAY)		return m_nLoadTrayCase;			//  1. (Error : 3100)
	else if (nRun == AUTO_LOAD_PICKER)		return m_nLoadPickCase;			//  2. (Error : 3200)
	else if (nRun == AUTO_VISION_ANGLE)		return m_nVisAngleCase;			//  3. (Error : 3300)
	else if (nRun == AUTO_ANGLE_TRAY1)		return m_nAngleTray1Case;		//  4. (Error : 3400)
	else if (nRun == AUTO_ANGLE_TRAY2)		return m_nAngleTray2Case;		//  5. (Error : 3500)
	else if (nRun == AUTO_VISION_BTM1)		return m_nBtm1PickCase;			//  6. (Error : 3600)
	else if (nRun == AUTO_INSPECTION1)		return m_nInspect1Case;			//  7. (Error : 3700)
	else if (nRun == AUTO_INSPECTION2)		return m_nInspect2Case;			//  8. (Error : 3800)
	else if (nRun == AUTO_INSPECTION3)		return m_nInspect3Case;			//  9. (Error : 3900)
	else if (nRun == AUTO_VISION_BTM2)		return m_nBtm2PickCase;			// 10. (Error : 4000)
	else if (nRun == AUTO_BUFFER1)			return m_nBuffTray1Case;		// 11. (Error : 4100)
	else if (nRun == AUTO_BUFFER2)			return m_nBuffTray2Case;		// 12. (Error : 4200)
	else if (nRun == AUTO_SORT_PICKER1)		return m_nSortPick1Case;		// 13. (Error : 4300)
	else if (nRun == AUTO_SORT_PICKER2)		return m_nSortPick2Case;		// 14. (Error : 4400)
	else if (nRun == AUTO_GOOD_TRAY1)		return m_nGoodTray1Case;		// 15. (Error : 4500)
	else if (nRun == AUTO_GOOD_TRAY2)		return m_nGoodTray2Case;		// 16. (Error : 4600)
	else if (nRun == AUTO_NG_TRAY)			return m_nNgTrayCase;			// 17. (Error : 4700)
	else if (nRun == AUTO_EMPTY_TRAY_X)		return m_nEmptyTrayXCase;		// 18. (Error : 4800)
	else if (nRun == AUTO_EMPTY_TRAY_EL)	return m_nEmptyTrayElCase;		// 19. (Error : 4900)
	else if (nRun == AUTO_EMPTY_TRAY_Y)		return m_nEmptyTrayYCase;		// 20. (Error : 5000)

	return 0;
}

void CSequenceMain::Set_MainRunCase(int nRun, int nCase)
{
	if		(nRun == AUTO_LOAD_TRAY)		m_nLoadTrayCase			= nCase;	//  1. (Error : 3100)
	else if (nRun == AUTO_LOAD_PICKER)		m_nLoadPickCase			= nCase;	//  2. (Error : 3200)
	else if (nRun == AUTO_VISION_ANGLE)		m_nVisAngleCase			= nCase;	//  3. (Error : 3300)
	else if (nRun == AUTO_ANGLE_TRAY1)		m_nAngleTray1Case		= nCase;	//  4. (Error : 3400)
	else if (nRun == AUTO_ANGLE_TRAY2)		m_nAngleTray2Case		= nCase;	//  5. (Error : 3500)
	else if (nRun == AUTO_VISION_BTM1)		m_nBtm1PickCase			= nCase;	//  6. (Error : 3600)
	else if (nRun == AUTO_INSPECTION1)		m_nInspect1Case			= nCase;	//  7. (Error : 3700)
	else if (nRun == AUTO_INSPECTION2)		m_nInspect2Case			= nCase;	//  8. (Error : 3800)
	else if (nRun == AUTO_INSPECTION3)		m_nInspect3Case			= nCase;	//  9. (Error : 3900)
	else if (nRun == AUTO_VISION_BTM2)		m_nBtm2PickCase			= nCase;	// 10. (Error : 4000)
	else if (nRun == AUTO_BUFFER1)			m_nBuffTray1Case		= nCase;	// 11. (Error : 4100)
	else if (nRun == AUTO_BUFFER2)			m_nBuffTray2Case		= nCase;	// 12. (Error : 4200)
	else if (nRun == AUTO_SORT_PICKER1)		m_nSortPick1Case		= nCase;	// 13. (Error : 4300)
	else if (nRun == AUTO_SORT_PICKER2)		m_nSortPick2Case		= nCase;	// 14. (Error : 4400)
	else if (nRun == AUTO_GOOD_TRAY1)		m_nGoodTray1Case		= nCase;	// 15. (Error : 4500)
	else if (nRun == AUTO_GOOD_TRAY2)		m_nGoodTray2Case		= nCase;	// 16. (Error : 4600)
	else if (nRun == AUTO_NG_TRAY)			m_nNgTrayCase			= nCase;	// 17. (Error : 4700)
	else if (nRun == AUTO_EMPTY_TRAY_X)		m_nEmptyTrayXCase		= nCase;	// 18. (Error : 4800)
	else if (nRun == AUTO_EMPTY_TRAY_EL)	m_nEmptyTrayElCase		= nCase;	// 19. (Error : 4900)
	else if (nRun == AUTO_EMPTY_TRAY_Y)		m_nEmptyTrayYCase		= nCase;	// 20. (Error : 5000)	
}

void CSequenceMain::Set_MainRunLoop(int nRun, int nLoop)
{
	if		(nRun == AUTO_LOAD_TRAY)		m_tLoadTrayLoop.Set_LoopTime(nLoop);	//  1. (Error : 3100)
	else if (nRun == AUTO_LOAD_PICKER)		m_tLoadPickLoop.Set_LoopTime(nLoop);	//  2. (Error : 3200)
	else if (nRun == AUTO_VISION_ANGLE)		m_tVisAngleLoop.Set_LoopTime(nLoop);	//  3. (Error : 3300)
	else if (nRun == AUTO_ANGLE_TRAY1)		m_tAngleTray1Loop.Set_LoopTime(nLoop);	//  4. (Error : 3400)
	else if (nRun == AUTO_ANGLE_TRAY2)		m_tAngleTray2Loop.Set_LoopTime(nLoop);	//  5. (Error : 3500)
	else if (nRun == AUTO_VISION_BTM1)		m_tBtm1PickLoop.Set_LoopTime(nLoop);	//  6. (Error : 3600)
	else if (nRun == AUTO_INSPECTION1)		m_tInspect1Loop.Set_LoopTime(nLoop);	//  7. (Error : 3700)
	else if (nRun == AUTO_INSPECTION2)		m_tInspect2Loop.Set_LoopTime(nLoop);	//  8. (Error : 3800)
	else if (nRun == AUTO_INSPECTION3)		m_tInspect3Loop.Set_LoopTime(nLoop);	//  9. (Error : 3900)
	else if (nRun == AUTO_VISION_BTM2)		m_tBtm2PickLoop.Set_LoopTime(nLoop);	// 10. (Error : 4000)
	else if (nRun == AUTO_BUFFER1)			m_tBuffTray1Loop.Set_LoopTime(nLoop);	// 11. (Error : 4100)
	else if (nRun == AUTO_BUFFER2)			m_tBuffTray2Loop.Set_LoopTime(nLoop);	// 12. (Error : 4200)
	else if (nRun == AUTO_SORT_PICKER1)		m_tSortPick1Loop.Set_LoopTime(nLoop);	// 13. (Error : 4300)
	else if (nRun == AUTO_SORT_PICKER2)		m_tSortPick2Loop.Set_LoopTime(nLoop);	// 14. (Error : 4400)
	else if (nRun == AUTO_GOOD_TRAY1)		m_tGoodTray1Loop.Set_LoopTime(nLoop);	// 15. (Error : 4500)
	else if (nRun == AUTO_GOOD_TRAY2)		m_tGoodTray2Loop.Set_LoopTime(nLoop);	// 16. (Error : 4600)
	else if (nRun == AUTO_NG_TRAY)			m_tNgTrayLoop.Set_LoopTime(nLoop);		// 17. (Error : 4700)
	else if (nRun == AUTO_EMPTY_TRAY_X)		m_tEmptyTrayXLoop.Set_LoopTime(nLoop);	// 18. (Error : 4800)
	else if (nRun == AUTO_EMPTY_TRAY_EL)	m_tEmptyTrayElLoop.Set_LoopTime(nLoop);	// 19. (Error : 4900)
	else if (nRun == AUTO_EMPTY_TRAY_Y)		m_tEmptyTrayYLoop.Set_LoopTime(nLoop);	// 20. (Error : 5000)
}

BOOL CSequenceMain::Get_IsAutoRun()
{
	int *pCase = Get_pMainRunCase();
	for (int i = 0; i < AUTO_COUNT; i++) {
		if (i == AUTO_ANGLE_TRAY1 && m_nAngleTray1Case == 50) continue;
		if (i == AUTO_ANGLE_TRAY2 && m_nAngleTray2Case == 50) continue;
		if (i == AUTO_INSPECTION1 && (m_nInspect1Case == 50 || m_nInspect1Case == 70)) continue;	// Center
		if (i == AUTO_INSPECTION2 && (m_nInspect2Case == 50 || m_nInspect2Case == 70)) continue;	// Front
		if (i == AUTO_INSPECTION3 && (m_nInspect3Case == 50 || m_nInspect3Case == 70)) continue;	// Rear
		if (i == AUTO_BUFFER1 && m_nBuffTray1Case == 50) continue;
		if (i == AUTO_BUFFER2 && m_nBuffTray2Case == 50) continue;
		if (i == AUTO_GOOD_TRAY1 && m_nGoodTray1Case == 50) continue;
		if (i == AUTO_GOOD_TRAY2 && m_nGoodTray2Case == 50) continue;
		if (*(pCase + i) > 0) return TRUE;
	}

	if (!Check_Btm1PickerEmpty()) return TRUE;
	if (!Check_InspetionEmpty(1)) return TRUE;
	if (!Check_InspetionEmpty(2)) return TRUE;
	if (!Check_InspetionEmpty(3)) return TRUE;
	if (!Check_Btm2PickerEmpty()) return TRUE;
	if (!Check_SortPickerEmpty(1)) return TRUE;
	if (!Check_SortPickerEmpty(2)) return TRUE;

	return FALSE;
}

void CSequenceMain::Begin_MainRunThread()
{
	Run_Replay();

	if (m_nLoadTrayCase == 0)	m_nLoadTrayCase = 1;

	if (m_nBuffTray1Case == 0)	m_nBuffTray1Case = 1;
	if (m_nBuffTray2Case == 0)	m_nBuffTray2Case = 1;

	if (m_nGoodTray1Case == 0)	m_nGoodTray1Case = 1;
	if (m_nGoodTray2Case == 0)	m_nGoodTray2Case = 1;

	if (m_nNgTrayCase == 0)		m_nNgTrayCase = 1;

	if (m_nEmptyTrayXCase == 0)		m_nEmptyTrayXCase = 1;
	if (m_nEmptyTrayYCase == 0)		m_nEmptyTrayYCase = 1;
	if (m_nEmptyTrayElCase == 0)	m_nEmptyTrayElCase = 1;

	g_objInspector.Set_StatusUpdate(INSPECTOR_ALL, 1);
	g_objDispatcher.Set_StatusUpdate(1);

	if (m_pThreadMainRun) End_MainRunThread();
	m_bThreadMainRun = TRUE;
	m_pThreadMainRun = AfxBeginThread(Thread_MainRun, NULL);
}

void CSequenceMain::End_MainRunThread()
{
	if (m_pThreadMainRun) {
		m_bThreadMainRun = FALSE;
		WaitForSingleObject(m_pThreadMainRun->m_hThread, INFINITE);
	}
}

void CSequenceMain::Run_Replay()
{
	for (int i = 0; i < AUTO_COUNT; i++) Set_MainRunLoop(i, 5000);

	if (m_nVisAngleCase == 15) m_tVisAngleLoop.Set_LoopTime(30000);
	if (m_nBtm1PickCase == 10) m_tBtm1PickLoop.Set_LoopTime(90000);
	if (m_nInspect1Case == 10 || m_nInspect1Case == 20)  m_tInspect1Loop.Set_LoopTime(90000);
	if (m_nInspect2Case == 10 || m_nInspect2Case == 20)  m_tInspect2Loop.Set_LoopTime(90000);
	if (m_nInspect3Case == 10 || m_nInspect3Case == 20)  m_tInspect3Loop.Set_LoopTime(90000);
	if (m_nBtm2PickCase == 10)  m_tBtm2PickLoop.Set_LoopTime(90000);

	// Empty Port Z축
	if (m_nEmptyTrayElCase == 3) {	// Slow Up
// #ifdef EDITION_2ND
// 		if (!gData.bEmptyZTopCheckRepeat && !m_pDX01->iEmptyPortTopCheck) {
// 			g_objAJinAXL.Move_Absolute(AX_EMPTY_PORT_Z, m_pMoveData->dEmptyPortZ[2], 0.2);
// 			m_tEmptyTrayElLoop.Set_LoopTime(10000);	// 10초
// 		}
// #else
// 		if (!gData.bEmptyZTopCheckRepeat && !m_pDX07->iEmptyPortTopCheck) {
// 			g_objAJinAXL.Move_Absolute(AX_EMPTY_PORT_Z, m_pMoveData->dEmptyPortZ[2], 0.2);
// 			m_tEmptyTrayElLoop.Set_LoopTime(10000);	// 10초
// 		}
// #endif
	} else if (m_nEmptyTrayElCase == 6) {	// Slow Down
		g_objAJinAXL.Move_Absolute(AX_EMPTY_PORT_Z, m_pMoveData->dEmptyPortZ[0], 0.2);
		m_tEmptyTrayElLoop.Set_LoopTime(10000);	// 10초
	} else if (m_nEmptyTrayElCase == 21) {
		g_objCommon.Move_Position(AX_EMPTY_PORT_Z, 1);
		m_tEmptyTrayElLoop.Set_LoopTime(10000);	// 10초
	} else if (m_nEmptyTrayElCase == 26) {
		g_objCommon.Move_Position(AX_EMPTY_PORT_Z, 0);
		m_tEmptyTrayElLoop.Set_LoopTime(10000);	// 10초
	} else if (m_nEmptyTrayElCase == 52) {	// Ready Down
		g_objCommon.Move_Position(AX_EMPTY_PORT_Z, 0);
		m_tEmptyTrayElLoop.Set_LoopTime(10000);	// 10초
	}

	// Empty Trans1 X축
	if (m_nEmptyTrayXCase == 9) {
		g_objCommon.Move_Position(AX_EMPTY_TRANS1_X, 3);
		m_tEmptyTrayXLoop.Set_LoopTime(90000);	// 속도 100일때 50초
	} else if (m_nEmptyTrayXCase == 12) {
		g_objCommon.Move_Position(AX_EMPTY_TRANS1_X, 1);
		m_tEmptyTrayXLoop.Set_LoopTime(90000);	// 속도 100일때 50초
	} else if (m_nEmptyTrayXCase == 21) {
		g_objCommon.Move_Position(AX_EMPTY_TRANS1_X, 0);
		m_tEmptyTrayXLoop.Set_LoopTime(90000);	// 속도 100일때 50초
	} else if (m_nEmptyTrayXCase == 13) {
		m_tEmptyTrayXLoop.Set_LoopTime(30000);	// Tray Full Alarm 상황일 수 있어 30초
	}

	// Empty Trans2 Y축
	if (m_nEmptyTrayYCase == 2) {
		m_tEmptyTrayYLoop.Set_LoopTime(60000);	// Tray 가져가기전 대기 시간 1분.

	} else if (m_nEmptyTrayYCase == 5) {
		m_tEmptyTrayYLoop.Set_LoopTime(30000);
	}
}

/////////////////////////////////////////////////////////////////////////////
// Main Thread Function 
UINT CSequenceMain::Thread_MainRun(LPVOID lpVoid)
{
	while (g_objSequenceMain.m_bThreadMainRun) {
		if (!g_objCommon.Check_MainEmgAir()) break;
		if (!g_objCommon.Check_MainDoor(TRUE)) break;
		//if (!g_objCommon.Check_PortArea(TRUE)) break;
		//if (!g_objCommon.Check_TrayFull()) break;
		if (!g_objCommon.Check_ServoOn()) break;
		if (!g_objCommon.Check_DirveAlarm()) break;
		if (!g_objCommon.Check_EndLimit()) break;
		if (!g_objCommon.Check_HomeDone()) break;

		if (!g_objSequenceMain.LoadTray_Run()) break;			//  1. (Error : 3100)
		if (!g_objSequenceMain.LoadPicker_Run()) break;			//  2. (Error : 3200)
		if (!g_objSequenceMain.VisionAngle_Run()) break;		//  3. (Error : 3300)
		if (!g_objSequenceMain.AngleTray1_Run()) break;			//  4. (Error : 3400)
		if (!g_objSequenceMain.AngleTray2_Run()) break;			//  5. (Error : 3500)
		if (!g_objSequenceMain.Btm1Picker_Run()) break;			//  6. (Error : 3600)
		if (!g_objSequenceMain.Inspection1_Run()) break;		//  7. (Error : 3700) Center
		if (!g_objSequenceMain.Inspection2_Run()) break;		//  8. (Error : 3800) Front
		if (!g_objSequenceMain.Inspection3_Run()) break;		//  9. (Error : 3900) Rear
		if (!g_objSequenceMain.Btm2Picker_Run()) break;			// 10. (Error : 4000)
		if (!g_objSequenceMain.BufferTray1_Run()) break;		// 11. (Error : 4100)
		if (!g_objSequenceMain.BufferTray2_Run()) break;		// 12. (Error : 4200)
		if (!g_objSequenceMain.SortPicker1_Run()) break;		// 13. (Error : 4300) Rear
		if (!g_objSequenceMain.SortPicker2_Run()) break;		// 14. (Error : 4400) Front
		if (!g_objSequenceMain.GoodTray1_Run()) break;			// 15. (Error : 4500)
		if (!g_objSequenceMain.GoodTray2_Run()) break;			// 16. (Error : 4600)
		if (!g_objSequenceMain.NgTray_Run()) break;				// 17. (Error : 4700)
		if (!g_objSequenceMain.EmptyTrayX_Run()) break;			// 18. (Error : 4800)
		if (!g_objSequenceMain.EmptyTrayElevator_Run()) break;	// 19. (Error : 4900)
		if (!g_objSequenceMain.EmptyTrayY_Run()) break;			// 20. (Error : 5000)		

		if (!g_objSequenceMain.Run_Simulation_04A()) break;		//  99.
		if (!g_objSequenceMain.Run_Simulation_04B()) break;		//  99.
		if (!g_objSequenceMain.Run_Simulation_04C()) break;		//  99.
		if (g_objSequenceMain.LotEnd_Run()) break;


		if (gData.bLotEndBeep) {
			if (g_objSequenceMain.m_pThreadBeep == NULL && g_objSequenceMain.m_pThreadNgFullBeep == NULL) {
				g_objSequenceMain.m_pThreadBeep = AfxBeginThread(Thread_Beep, (LPVOID)(2000));
				gData.bLotEndBeep = FALSE;
			}
		}

		Sleep(5);
	}

	// Stop 버튼을 누른후에 마지막 프로세스 Case가 모션 이동 명령 일수 있어 이곳에도 Stop 추가. 
	g_objAJinAXL.Stop_Motion(AX_EMPTY_PORT_Z);		// 강제 STOP

	g_objInspector.Set_StatusUpdate(INSPECTOR_ALL, 0);
	g_objDispatcher.Set_StatusUpdate(0);

	g_objSequenceMain.m_bThreadMainRun = FALSE;
	g_objSequenceMain.m_pThreadMainRun = NULL;

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// Picker Vacuum Thread Function

UINT CSequenceMain::Thread_Vacuum_B1p(LPVOID lpVoid)
{
	g_objSequenceMain.Vacuum_Btm1Picker((int)lpVoid);
	g_objSequenceMain.m_pThreadVacuumB1p = NULL;
	return 0;
}

UINT CSequenceMain::Thread_Vacuum_B2p(LPVOID lpVoid)
{
	g_objSequenceMain.Vacuum_Btm2Picker((int)lpVoid);
	g_objSequenceMain.m_pThreadVacuumB2p = NULL;
	return 0;
}

UINT CSequenceMain::Thread_Vacuum_Sp1(LPVOID lpVoid)
{
	g_objSequenceMain.Vacuum_SortPicker1((int)lpVoid);
	g_objSequenceMain.m_pThreadVacuumSp1 = NULL;
	return 0;
}

UINT CSequenceMain::Thread_Vacuum_Sp2(LPVOID lpVoid)
{
	g_objSequenceMain.Vacuum_SortPicker2((int)lpVoid);
	g_objSequenceMain.m_pThreadVacuumSp2 = NULL;
	return 0;
}

UINT CSequenceMain::Thread_Vacuum_B2p_Multi(LPVOID lpVoid)
{
	g_objSequenceMain.Vacuum_Btm2PickerMulti((int)lpVoid);
	g_objSequenceMain.m_pThreadVacuumB2pMulti = NULL;
	return 0;
}

UINT CSequenceMain::Thread_Vacuum_Sp1_Multi(LPVOID lpVoid)
{
	g_objSequenceMain.Vacuum_SortPicker1Multi((int)lpVoid);
	g_objSequenceMain.m_pThreadVacuumSp1Multi = NULL;
	return 0;
}

UINT CSequenceMain::Thread_Vacuum_Sp2_Multi(LPVOID lpVoid)
{
	g_objSequenceMain.Vacuum_SortPicker2Multi((int)lpVoid);
	g_objSequenceMain.m_pThreadVacuumSp2Multi = NULL;
	return 0;
}

UINT CSequenceMain::Thread_Beep(LPVOID lpVoid)
{
	g_objSequenceMain.Beep_Post(0, (int)lpVoid);
	g_objSequenceMain.m_pThreadBeep = NULL;
	return 0;
}

UINT CSequenceMain::Thread_NgFullBeep(LPVOID lpVoid)
{
	g_objSequenceMain.Beep_Post(1, (int)lpVoid);
	g_objSequenceMain.m_pThreadNgFullBeep = NULL;
	return 0;
}

///////////////////////////////////////////////////////////////////////////////

void CSequenceMain::Vacuum_Btm1Picker(int nSub)
{
	int nCount = 0;
	while (TRUE) {
		g_objCommon.Set_Btm1PickerVacOff(nSub);
		Sleep(m_pEquipData->nVacOffDelay[0]);
		
		g_objCommon.Set_Btm1PickerAirOff(nSub);
		if (++nCount >= m_pEquipData->nVacOffRepeat[0]) return;
		Sleep(m_pEquipData->nVacOffDelay[0]);
	}
}

void CSequenceMain::Vacuum_Btm2Picker(int nSub)
{
	int nCount = 0;
	while (TRUE) {
		g_objCommon.Set_Btm2PickerVacOff(nSub);
		Sleep(m_pEquipData->nVacOffDelay[1]);

		g_objCommon.Set_Btm2PickerAirOff(nSub);
		if (++nCount >= m_pEquipData->nVacOffRepeat[1]) return;
		Sleep(m_pEquipData->nVacOffDelay[1]);
	}
}

void CSequenceMain::Vacuum_SortPicker1(int nSub)
{
	int nCount = 0;
	while (TRUE) {
		g_objCommon.Set_SortPicker1VacOff(nSub);
		Sleep(m_pEquipData->nVacOffDelay[2]);

		g_objCommon.Set_SortPicker1AirOff(nSub);
		if (++nCount >= m_pEquipData->nVacOffRepeat[2]) return;
		Sleep(m_pEquipData->nVacOffDelay[2]);
	}
}

void CSequenceMain::Vacuum_SortPicker2(int nSub)
{
	int nCount = 0;
	while (TRUE) {
		g_objCommon.Set_SortPicker2VacOff(nSub);
		Sleep(m_pEquipData->nVacOffDelay[3]);

		g_objCommon.Set_SortPicker2AirOff(nSub);
		if (++nCount >= m_pEquipData->nVacOffRepeat[3]) return;
		Sleep(m_pEquipData->nVacOffDelay[3]);
	}
}

void CSequenceMain::Vacuum_Btm2PickerMulti(int nSub)
{
	int nCount = 0;
	while (TRUE) {
		g_objCommon.Set_Btm2PickerVacOffMulti(nSub, m_nBtm2PickMultiCnt, m_nBtm2PickRow);
		Sleep(m_pEquipData->nVacOffDelay[1]);

		g_objCommon.Set_Btm2PickerAirOffMulti(nSub, m_nBtm2PickMultiCnt, m_nBtm2PickRow);
		if (++nCount >= m_pEquipData->nVacOffRepeat[1]) return;
		Sleep(m_pEquipData->nVacOffDelay[1]);
	}
}

void CSequenceMain::Vacuum_SortPicker1Multi(int nSub)
{
	int nCount = 0;
	while (TRUE) {
		g_objCommon.Set_SortPicker1VacOffMulti(nSub, m_nSortPick1MultiCnt);
		Sleep(m_pEquipData->nVacOffDelay[3]);

		g_objCommon.Set_SortPicker1AirOffMulti(nSub, m_nSortPick1MultiCnt);
		if (++nCount >= m_pEquipData->nVacOffRepeat[2]) return;
		Sleep(m_pEquipData->nVacOffDelay[3]);
	}
}

void CSequenceMain::Vacuum_SortPicker2Multi(int nSub)
{
	int nCount = 0;
	while (TRUE) {
		g_objCommon.Set_SortPicker2VacOffMulti(nSub, m_nSortPick2MultiCnt);
		Sleep(m_pEquipData->nVacOffDelay[3]);

		g_objCommon.Set_SortPicker2AirOffMulti(nSub, m_nSortPick2MultiCnt);
		if (++nCount >= m_pEquipData->nVacOffRepeat[3]) return;
		Sleep(m_pEquipData->nVacOffDelay[3]);
	}
}

///////////////////////////////////////////////////////////////////////////////

BOOL CSequenceMain::LotEnd_Run()
{
//	if (!m_bLoadLotEnd) return FALSE;
	if (!m_bUnloadLotEnd) return FALSE;

	if (Get_IsAutoRun()) return FALSE;

	int nPNo = gData.nULPNo - 1;	// 맨마지막 공정인 Good Tray Port No 활용.
	if (nPNo < 0) nPNo = 0;
//	Job_LotEnd(gData.nULPNo);
// 	Beep_Post(1000);
	if (gData.bCycleStop) g_dlgWork.PostMessage(UM_RESET_CYCLE_STOP, NULL, NULL);

	gMes.nLotStatus = 0;
	g_objMES.Set_Status(3);	//Idle Set
	gData.bMesFirstLot = FALSE;
	gData.bMesContinueLot = FALSE;
	g_objMES.m_nMESSequence = 0;
	g_objMES.m_bMesErr = FALSE;
	gData.nMesPortNo = 0;

	gData.bFirstLotStart = FALSE;

	gData.bNGTrayWait = FALSE;
	gData.bGoodTrayWait = FALSE;
	gData.bEmptyZWait = FALSE;
	gData.bLoadLampOn[0] = FALSE;
	gData.bLoadLampOn[1] = FALSE;

	CCMI8000Dlg *pMainDlg = (CCMI8000Dlg*)AfxGetApp()->GetMainWnd();
	pMainDlg->Set_LampFlicker_Load1(FALSE);
	pMainDlg->Set_LampFlicker_Load2(FALSE);
	pMainDlg->Set_LampFlicker_Ng(FALSE);
	pMainDlg->Set_LampFlicker_Good(FALSE);
	pMainDlg->Set_LampFlicker_Empty(FALSE);

	CString strMsg;

	int nSum = gLot.nGoodCount[nPNo] + gLot.nNgCount[nPNo];
	double dRate = (nSum == 0 ? 0.0 : gLot.nGoodCount[nPNo] * 100.0 / nSum);
	strMsg.Format("Lot End ==> ID(%s), Total(%d), Good(%d), NG(%d), Rate(%0.1lf)", gLot.sLotID[nPNo], nSum, gLot.nGoodCount[nPNo], gLot.nNgCount[nPNo], dRate);
	g_objLogFile.Save_HandlerLog(strMsg);

	strMsg.Format("Lot End.\n\nSpecial NG Count\n(N1:%d, N2:%d, ROS:%d)",
		gLot.nSNgCount[nPNo][1], gLot.nSNgCount[nPNo][2], gLot.nRosNg[nPNo]);

	g_objCommon.Show_Alarm(strMsg);

// 	// Lot Result [날짜,Lot Start,Lot End,Term,호기,Lot ID,투입수,양품수,불량수,불량률,HREN(H),HREP(P),Spider(S),BA(B),Chip(C),MES(M)];
// 	CString strStart, strEnd, strTerm, strEquip;
// 
// 	strStart.Format("%s:%s:%s", gLot.sStartTime[nPNo].Mid(9, 2), gLot.sStartTime[nPNo].Mid(11, 2), gLot.sStartTime[nPNo].Mid(13, 2));
// 	strEnd.Format("%s:%s:%s", gLot.sEndTime[nPNo].Mid(9, 2), gLot.sEndTime[nPNo].Mid(11, 2), gLot.sEndTime[nPNo].Mid(13, 2));
// 	int nSec = (int)(gLot.dwLotEnd[nPNo] - gLot.dwLotStart[nPNo]) / 1000;
// 	int nH = nSec / 60 / 60;
// 	int nM = (nSec - nH * 60 * 60) / 60;
// 	int nS = (nSec - nH * 60 * 60) % 60;
// 	strTerm.Format("%02d:%02d:%02d", nH, nM, nS);
// 	strEquip = m_pEquipData->sEquipName.Right(2);
// 	double dNgRate = (nSum == 0 ? 0.0 : gLot.nNgCount[nPNo] * 100.0 / nSum);
// 
// 	strMsg.Format("%s,%s,%s,%s,%s,%d,%d,%d,%0.2lf%%,%d,%d,%d,%d,%d,%d",
// 		strStart, strEnd, strTerm, strEquip, gLot.sLotID[nPNo], nSum, gLot.nGoodCount[nPNo], gLot.nNgCount[nPNo], dNgRate,
// 		gLot.nSNgCount[nPNo][1], gLot.nSNgCount[nPNo][2], gLot.nSNgCount[nPNo][0]);
// 
// 	g_objLogFile.Save_LotResult(nPNo, strMsg);
// 
// 	//Time, Lot_ID, Total, Good, NG, RosRequest, RosGood, RosNg, RosRepair, Filtering1, Filtering2, RateAvi, Final, Tack
// 	double dFiltering1 = (gLot.nRosRequest[nPNo] == 0 ? 0.0 : (gLot.nRosGood[nPNo] * 100.0) / gLot.nRosRequest[nPNo]);
// 	double dFiltering2 = (nSum == 0 ? 0.0 : (gLot.nRosGood[nPNo] * 100.0) / nSum);
// 	double dRateAvi = (nSum == 0 ? 0.0 : ((gLot.nGoodCount[nPNo] - gLot.nRosGood[nPNo]) * 100.0) / nSum);
// 
// 	strMsg.Format("\t%s \t%d \t%d \t%d \t%d \t%d \t%d \t%d \t%d \t%0.2lf \t%0.2lf \t%0.2lf \t%0.2lf \t%0.3lf",
// 		gLot.sLotID[nPNo], nSum, gLot.nGoodCount[nPNo], gLot.nNgCount[nPNo], gLot.nRosRequest[nPNo], gLot.nRosGood[nPNo], gLot.nRosNg[nPNo], gLot.nRosRepair[nPNo], gLot.nRosTimeOut[nPNo],
// 		dFiltering1, dFiltering2, dRateAvi, dRate, gLot.dTackTime);
// 	g_objLogFile.Save_DailyLot(strMsg);
// 
// 	strMsg.Format(",%s,%s,%s,%s,%0.3lf,%d,%0.3lf,%0.3lf,%0.3lf,%d,%0.2lf,%0.3lf,%0.3lf,%0.2lf",
// 		m_pEquipData->sEquipName, gLot.sLotID[nPNo], strStart, strEnd, gLot.dTackTime, nSum, (double)(gLot.dwRunTime) / 1000,
// 		(double)(gLot.dwStopTime) / 1000, (double)(gLot.dwErrorTime) / 1000, gLot.nErrorCount, dRate, 3600 / gLot.dTackTime ,(double)(gLot.dwRunTime + gLot.dwErrorTime + gLot.dwStopTime) / gLot.nErrorCount);
// 	g_objLogFile.Save_OperatingRatio(strMsg);
// 
// 	g_objLogFile.Save_AverageCycle(nPNo);

	m_pEquipData->bResultTestUse = FALSE;	// LOT 끝나면 Reset
	gData.bNgTrayEnd = FALSE;

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////

void CSequenceMain::Set_ClearRunData(int nType)
{
	memset(gData.bReload, 0x00, sizeof(BOOL) * 5);
	memset(gData.bScanDone, 0x00, sizeof(BOOL) * 5);
	memset(gData.byInspectDone, 0x00, sizeof(BYTE) * 2 * 30 * 40);
	memset(gData.nInspectInfo, 0x00, sizeof(int) * 2 * 30 * 40);
	memset(gData.cJudgeCode1, 0x00, sizeof(char) *  30 * 40 * 7);
	memset(gData.cJudgeCode2, 0x00, sizeof(char) *  30 * 40 * 7);
	memset(gData.bRosDone, 0x00, sizeof(BOOL) * 2 * 30 * 40);

	memset(gData.InfoAnglePortTray, 0x00, sizeof(int) * 8 * 5);
	memset(gData.InfoAngleTray, 0x00, sizeof(int) * 2 * 8 * 5);
	memset(gData.InfoBuffTray, 0x00, sizeof(int) * 2 * 8 * 5);
	memset(gData.InfoGoodTray, 0x00, sizeof(int) * 8 * 5);
	memset(gData.InfoNgTray, 0x00, sizeof(int) * 6 * 8 * 5);

	g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 0, NULL);

	memset(gData.InfoBtm1Pick, 0x00, sizeof(int) * 2 * 5);
	memset(gData.InfoInspect, 0x00, sizeof(int) * 3 * 2 * 5);
	memset(gData.InfoBtm2Pick, 0x00, sizeof(int) * 2 * 5);
	memset(gData.InfoSortPick, 0x00, sizeof(int) * 2 * 5);
	memset(gData.InfoNgBuffer, 0x00, sizeof(int) * 2 * 5);

	gData.nPNoTrayPick = gData.nPNoBtm1Pick = gData.nPNoBtm2Pick = 0;
	gData.nPNoNgTray = gData.nPNoGoodTray = 0;
	gData.nLPNo = gData.nULPNo = 0;
	memset(gData.nPNoAnglePort, 0x00, sizeof(int) * 2);
	memset(gData.nPNoAngleTray, 0x00, sizeof(int) * 2);
	memset(gData.nPNoInspect, 0x00, sizeof(int) * 3 );
	memset(gData.nPNoBuffTray, 0x00, sizeof(int) * 2);
	memset(gData.nPNoSortPick, 0x00, sizeof(int) * 2);
	memset(gData.nPNoNgBuffer, 0x00, sizeof(int) * 2);

	gData.nTNoTrayPick = 0;
	memset(gData.nTNoAnglePort, 0x00, sizeof(int) * 2);
	memset(gData.nTNoAngleTray, 0x00, sizeof(int) * 2);
	memset(gData.nTNoBtm1Pick, 0x00, sizeof(int) * 2 * 5);
	memset(gData.nTNoInspect, 0x00, sizeof(int) * 3 * 2 * 5);
	memset(gData.nTNoBtm2Pick, 0x00, sizeof(int) * 2 * 5);
	memset(gData.nTNoBuffTray, 0x00, sizeof(int) * 2 * 5 * 8);
	memset(gData.nTNoSortPick, 0x00, sizeof(int) * 2 * 5);
	memset(gData.nTNoNgBuffer, 0x00, sizeof(int) * 2 * 5);

	memset(gData.nCNoBtm1Pick, 0x00, sizeof(int) * 2 * 5);
	memset(gData.nCNoInspect, 0x00, sizeof(int) * 3 * 2 * 5);
	memset(gData.nCNoBtm2Pick, 0x00, sizeof(int) * 2 * 5);
	memset(gData.nCNoBuffTray, 0x00, sizeof(int) * 2 * 5 * 8);
	memset(gData.nCNoSortPick, 0x00, sizeof(int) * 2 * 5);
	memset(gData.nCNoNgBuffer, 0x00, sizeof(int) * 2 * 5);

	memset(gLot.dAverageCycle, 0x00, sizeof(double) * 28 * 2 * 14);

	g_objInspector.Set_InspectJudgeClear();

	// Process 변수 초기화
	m_bLoadLotEnd = FALSE;
	m_bUnloadLotEnd = FALSE;

	m_nAnglePortTrayCnt = 0;
	m_bAngleInspComplete = FALSE;

	//AHN
	m_dwFirstLoad = 0;
	m_dwLastUnLoad = 0;
	m_dwULCycleTime = 0;
	gLot.nErrorCount = 0;
	gLot.dwRunTime = gLot.dwErrorTime = gLot.dwStopTime = 0;
	gLot.bLotEndComplete[0] = FALSE;
	gLot.bLotEndComplete[1] = FALSE;
	gData.bFirstLotStart = FALSE;

	gData.nLoadTrayCount[0] = 0; gData.nLoadTrayCount[1] = 0;
	gData.nEmptyTrayCount = (m_nEmptyTrayElCase == 20 ? 1 : 0);
	gData.nGoodTrayCount = 0;
// 	gData.nNNgTrayCount = 0;
// 	gData.nSNgTrayCount = 0;

	gData.nTrayUseCount[0] = gData.nTrayUseCount[1] = 0;
	gData.nCmUseCount[0] = gData.nCmUseCount[1] = 0;

	gData.bNGTrayWait = FALSE;
	gData.bContinueLotEnd = FALSE;
	gData.bGoodTrayWait = FALSE;
	gData.bGoodTrayLotEnd[0] = FALSE;
	gData.bGoodTrayLotEnd[1] = FALSE;
	gData.bEmptyZWait = FALSE;

	gData.bLoadLampOn[0] = FALSE;
	gData.bLoadLampOn[1] = FALSE;

	gData.bVisionVerErr = FALSE;

	gData.bInspetBWD = FALSE;
	gData.bBuffStageMove = FALSE;

	// Mirror Down Error 변수 초기화
	gData.bTop1MirrorErr[0] = FALSE;
	gData.bTop1MirrorErr[1] = FALSE;
	gData.bTop1MirrorErr[2] = FALSE;

	for (int p = 0; p < 2; p++) for (int i = 0; i < 6; i++) gLot.nSNgCount[p][i] = 0;	// Special NG
	gLot.nGoodCount[0] = gLot.nGoodCount[1] = gLot.nNgCount[0] = gLot.nNgCount[1] = 0;
	for (int p = 0; p < 2; p++) { gLot.nRosRequest[p] = gLot.nRosGood[p] = gLot.nRosNg[p] = gLot.nRosRepair[p] = gLot.nRosTimeOut[p] = 0; }
	g_dlgWork.PostMessage(UM_VISION_RESULT, NULL, NULL);

	for (int p = 0; p <  2; p++) for (int i = 0; i < 30; i++) for (int j = 0; j < 40; j++) gMes.sBarID[p][i][j] = gMes.sJudge[p][i][j] = gMes.sNGCode[p][i][j] = "";
	for (int p = 0; p <  2; p++) for (int i = 0; i < 30; i++) for (int j = 0; j < 40; j++) for (int k = 0; k < 5; k++) gData.sNGData[p][i][j][k] = "";
	memset(gMes.nCmResult, 0x00, sizeof(int) * 30 * 40);	// Result Clear

	m_bLotLoadEnable[0] = FALSE;
	m_bLotLoadEnable[1] = FALSE;

	g_dlgWork.Enable_UserInput(1, TRUE);
	g_dlgWork.Enable_UserInput(2, TRUE);

	gData.bMesContinueLot = FALSE;

	CCMI8000Dlg *pMainDlg = (CCMI8000Dlg*)AfxGetApp()->GetMainWnd();
	pMainDlg->Set_LampFlicker_Load1(FALSE);
	pMainDlg->Set_LampFlicker_Load2(FALSE);
	pMainDlg->Set_LampFlicker_Ng(FALSE);
	pMainDlg->Set_LampFlicker_Good(FALSE);
	pMainDlg->Set_LampFlicker_Empty(FALSE);

	if (nType == 0) m_pEquipData->bResultTestUse = FALSE;	// LOT 끝나면 Reset

	gLot.nNGT = gLot.nNGC = gLot.nGDT = gLot.nG1DC = 0;

	g_objDispatcher.Reset();

	g_dlgWork.PostMessage(UM_UPDATE_MODEL, NULL, NULL);
}

///////////////////////////////////////////////////////////////////////////////
void CSequenceMain::Init_AnglePortTray(int nPNo)
{
	memset(gData.InfoAnglePortTray, 0x00, sizeof(int) * 8 * 5);	// Empty

	int nNo = nPNo-1;
	if (nNo < 0) nNo = 0;

	int nCmMax = gData.nCmMaxCount;
	int nTrayCnt = gData.nTNoAnglePort[0];
	int nTrayUse = gData.nTrayUseCount[nNo];
	int nCmUse   = gData.nCmUseCount[nNo];

	int nCmCnt = (nTrayCnt < nTrayUse ? nCmMax : (nCmUse - (nTrayCnt - 1) * nCmMax));
	if (gData.bR04A || gData.bR13A) {
		for (int i = 0; i < nCmCnt; i++) gData.InfoAnglePortTray[i % gData.nTrayY][i / gData.nTrayY] = 9;	// Not Use

	} else {
 		for (int i = 0; i < nCmCnt; i++) gData.InfoAnglePortTray[i / gData.nTrayX][i % gData.nTrayX] = 9;	// Not Use
	}

	g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 1, NULL);
}

void CSequenceMain::Init_AngleTray(int nNo)
{
	for (int y = 0; y < gData.nTrayY; y++) {
		for (int x = 0; x < gData.nTrayX; x++) {
			if (gData.InfoAngleTray[nNo][y][x] == 1)	gData.InfoAngleTray[nNo][y][x] = 9;
			else										gData.InfoAngleTray[nNo][y][x] = 0;
		}
	}

	g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 2, nNo);
}

void CSequenceMain::Init_BufferTray()
{
	memset(gData.InfoBuffTray, 0x00, sizeof(int) * 2 * 8 * 5);	// Empty
	// 테스트 (시간 단축하기 위해 10줄 채운 상태로 진행)
	//for (int i = 0; i < 10; i++) for (int j = 0; j < 3; j++) gData.InfoGoodTray[i][j] = 1;
	g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 3, NULL);
}

void CSequenceMain::Init_GoodTray()
{
	memset(gData.InfoGoodTray, 0x00, sizeof(int) * 8 * 5);	// Empty
	// 테스트 (시간 단축하기 위해 10줄 채운 상태로 진행)
	//for (int i = 0; i < 10; i++) for (int j = 0; j < 3; j++) gData.InfoGoodTray[i][j] = 1;
	g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 4, NULL);
}

void CSequenceMain::Init_NgTray(int nNo)
{
//	memset(gData.InfoNgTray, 0x00, sizeof(int) * 6 * 8 * 5);	// Empty
	int nNo1, nNo2;
	if (nNo == 0) { nNo1 = 0; nNo2 = 2; }
	if (nNo == 1) { nNo1 = 0; nNo2 = 2; }
	if (nNo == 2) { nNo1 = 1; nNo2 = 3; }

	for (int y = 0; y < gData.nTrayY; y++) {
		for (int x = 0; x < gData.nTrayX; x++) {
			gData.InfoNgTray[nNo1][y][x] = 0;
			if (nNo2 != -1) gData.InfoNgTray[nNo2][y][x] = 0;
		}
	}
	g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 5, NULL);
}

///////////////////////////////////////////////////////////////////////////////
BOOL CSequenceMain::Check_AnglePortTrayEmpty()
{
	for (int y = 0; y < gData.nTrayY; y++) {
		for (int x = 0; x < gData.nTrayX; x++) {
			if (gData.InfoAnglePortTray[y][x] > 0) return FALSE;
		}
	}
	return TRUE;
}

BOOL CSequenceMain::Check_AngleTrayEmpty(int nNo)
{
	int nSNo,nENo = 0;
	if (nNo == 0) { nSNo = 0; nENo = 2; }
	else		  { nSNo = nNo-1; nENo = nNo; }

	for (int s = nSNo; s < nENo; s++) {
		for (int y = 0; y < gData.nTrayY; y++) {
			for (int x = 0; x < gData.nTrayX; x++) {
				if (gData.InfoAngleTray[s][y][x] > 0) return FALSE;
			}
		}
	}
	return TRUE;
}

BOOL CSequenceMain::Check_AngleTrayLineEmpty(int nNo, int nLine)
{	
	for (int x = 0; x < gData.nTrayX; x++) {
		if (gData.InfoAngleTray[nNo-1][nLine-1][x] > 0) return FALSE;
	}
	return TRUE;
}

BOOL CSequenceMain::Check_Btm1PickerEmpty()
{
	for (int i = 0; i < 10; i++) { 
		if (gData.InfoBtm1Pick[i] > 0) return FALSE; 
	}
	return TRUE;
}

BOOL CSequenceMain::Check_InspetionEmpty(int nNo)
{
	for (int i = 0; i < 10; i++) {
		if (gData.InfoInspect[nNo-1][i] > 0) return FALSE;
	}
	return TRUE;
}

BOOL CSequenceMain::Check_Btm2PickerEmpty()
{
	for (int i = 0; i < 10; i++) { 
		if (gData.InfoBtm2Pick[i] > 0) return FALSE; 
	}
	return TRUE;
}

BOOL CSequenceMain::Check_SortPickerEmpty(int nNo)
{
	for (int i = 0; i < 5; i++) { if (gData.InfoSortPick[nNo-1][i] > 0) return FALSE; }
	return TRUE;
}

BOOL CSequenceMain::Check_BufferEmpty(int nNo)
{
	int nSNo,nENo = 0;
	if (nNo == 0) { nSNo = 0; nENo = 2; }
	else		  { nSNo = nNo-1; nENo = nNo; }

	for (int s = nSNo; s < nENo; s++) {
		for (int y = 0; y < gData.nTrayY; y++) {
			for (int x = 0; x < gData.nTrayX; x++) {
				if (gData.InfoBuffTray[s][y][x] > 0) return FALSE;
			}
		}
	}
	return TRUE;
}

BOOL CSequenceMain::Check_BufferTrayLineEmpty(int nNo, int nLine)
{	
	for (int x = 0; x < gData.nTrayX; x++) {
		if (gData.InfoBuffTray[nNo-1][nLine-1][x] > 0) return FALSE;
	}
	return TRUE;
}

BOOL CSequenceMain::Check_GoodTrayEmpty()
{
	for (int y = 0; y < gData.nTrayY; y++) {
		for (int x = 0; x < gData.nTrayX; x++) {
			if (gData.InfoGoodTray[y][x] > 0) return FALSE;
		}
	}
	return TRUE;
}

BOOL CSequenceMain::Check_Btm1PickerFull()
{
	// A모델은 90도 회전하기때문에 Full Picker가 안될수있다.
	// L2(5~10번)에 모듈이 있으면 Btm1 검사위치로 이동한다.
	for (int i = 5; i < 10; i++) { 
		if (gData.InfoBtm1Pick[i] > 0) return TRUE; 
	}
	return FALSE;
}

BOOL CSequenceMain::Check_BufferFull(int nNo)
{
	if (gData.bR04B || gData.bR13B_4 || gData.bR13B_5) {	// 10개씩 넘어가게 해준다.
		if (gData.InfoBuffTray[nNo][gData.nTrayY - 2][gData.nTrayX - 1] > 0) return TRUE;

	} else {
		if (gData.InfoBuffTray[nNo][gData.nTrayY - 1][gData.nTrayX - 1] > 0) return TRUE;
 	}
	
	return FALSE;
}

BOOL CSequenceMain::Check_GoodTrayFull()
{
	if (gData.InfoGoodTray[gData.nTrayY - 1][gData.nTrayX - 1] > 0) return TRUE;
	return FALSE;
}

BOOL CSequenceMain::Check_NgTrayFull()
{
// 	if (gData.InfoNgTray[2][gData.nTrayY - 1][gData.nTrayX - 1] > 0) return TRUE;	// Normal NG
	if (gData.InfoNgTray[2][gData.nTrayY - 1][gData.nTrayX - 1] > 0) return TRUE;	// N1
	if (gData.InfoNgTray[3][gData.nTrayY - 1][gData.nTrayX - 1] > 0) return TRUE;	// N2

	return FALSE;
}

BOOL CSequenceMain::Check_NgBufferFull(int nNo)
{	
	if (nNo == 1) { if (gData.InfoNgBuffer[1][gData.nSortPickQt-1] > 0) return TRUE; }
	if (nNo == 2) { if (gData.InfoNgBuffer[0][gData.nSortPickQt-1] > 0) return TRUE; }

	return FALSE;
}

BOOL CSequenceMain::Check_NgBufferEmpty(int nNo)
{
	int nBNo = 0;
	if (nNo == 1) nBNo = 1;
	if (nNo == 2) nBNo = 0;

	for (int i = 0; i < gData.nSortPickQt; i++) {
		if (gData.InfoNgBuffer[nBNo][i] > 0) return FALSE; 
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
BOOL CSequenceMain::Select_AngleScanPos(int &nAnglePosX, int &nAnglePosY)
{
	// Module만 Scan
	nAnglePosX = nAnglePosY = 0;
	for(int j=(gData.nTrayY-1); j>=0; j--) {
		if (j==1 || j==3 || j==5 || j==7 || j==9 || j==11) {
			for(int i=(gData.nTrayX-1); i>=0; i--) {
				if (gData.InfoAnglePortTray[j][i] == 9) {
					nAnglePosY = j + 1;
					nAnglePosX = i + 1;
					break;
				}
			}
		} else {
			for(int i=0; i<gData.nTrayX; i++) {
				if (gData.InfoAnglePortTray[j][i] == 9) {
					nAnglePosY = j + 1;
					nAnglePosX = i + 1;
					break;
				}
			}
		}
		if (nAnglePosY > 0) break;
	}

/*
	// Tray All Scan
	if (nAnglePosY == 0) nAnglePosY = 1;
	if (nAnglePosY==1 || nAnglePosY==3 || nAnglePosY==5 || nAnglePosY==7 || nAnglePosY==9 || nAnglePosY==11) {
		nAnglePosX++;
		if (nAnglePosX > TRAY_X) {
			nAnglePosX = TRAY_X;	nAnglePosY++;
		}
	} else {
		nAnglePosX--;
		if (nAnglePosX == 0) {
			nAnglePosY++; nAnglePosX = 1;
		}
	}
*/
	if (nAnglePosY > gData.nTrayY) return FALSE;
	if (nAnglePosY == 0 || nAnglePosX ==0) return FALSE;
	return TRUE;
}

BOOL CSequenceMain::Select_BtmScanPos(int nPos, int nScanCnt)
{
	int *pInfo = NULL;
	if (nPos == AUTO_VISION_BTM1) pInfo = gData.InfoBtm1Pick;
	if (nPos == AUTO_VISION_BTM2) pInfo = gData.InfoBtm2Pick;

	if (pInfo == NULL) return FALSE;

	if (gData.bR04C) 
	{	// Btm Picker 4개만 사용.
		if (nScanCnt == 0)
		{
			if( pInfo[3] == 9 || pInfo[8] == 9) 
			{ 
				return TRUE; 
			}
		}
		if (nScanCnt == 1) {
			if( pInfo[2] == 9 || pInfo[7] == 9 ) { return TRUE; }
		}
		if (nScanCnt == 2) {
			if( pInfo[1] == 9 || pInfo[6] == 9) { return TRUE; }
		}

		if (nScanCnt == 3) {
			if( pInfo[0] == 9 || pInfo[5] == 9 ) { return TRUE; }
		}

	} else {
		if (nScanCnt == 0) {
			if( pInfo[4] == 9 ||pInfo[9] == 9 ) { return TRUE; }
		}
		if (nScanCnt == 1) {
			if( pInfo[3] == 9 ||pInfo[8] == 9 ) { return TRUE; }
		}
		if (nScanCnt == 2) {
			if( pInfo[2] == 9 || pInfo[7] == 9) { return TRUE; }
		}
		if (nScanCnt == 3) {
			if( pInfo[1] == 9 || pInfo[6] == 9 ) { return TRUE; }
		}
		if (nScanCnt == 4) {
			if( pInfo[0] == 9 || pInfo[5] == 9) { return TRUE; }
		}
	}
	return FALSE;
}

BOOL CSequenceMain::Select_Top1ScanPos(int nPos, int nScanCnt)
{
	int *pInfo = NULL;
	if (nPos == AUTO_INSPECTION1) pInfo = gData.InfoInspect[0];
	if (nPos == AUTO_INSPECTION2) pInfo = gData.InfoInspect[1];
	if (nPos == AUTO_INSPECTION3) pInfo = gData.InfoInspect[2];

	if (pInfo == NULL) return FALSE;

	if (gData.bR04C) {	// Btm Picker 4개만 사용.

		if (nScanCnt == 0) {
			if( pInfo[3] == 9 || pInfo[8] == 9) { return TRUE; }
		}
		if (nScanCnt == 1) {
			if( pInfo[2] == 9 || pInfo[7] == 9 ) { return TRUE; }
		}
		if (nScanCnt == 2) {
			if( pInfo[1] == 9 || pInfo[6] == 9) { return TRUE; }
		}

		if (nScanCnt == 3) {
			if( pInfo[0] == 9 || pInfo[5] == 9 ) { return TRUE; }
		}
	} else {
		if (nScanCnt == 0) {
			if( pInfo[4] == 9 ||pInfo[9] == 9 ) { return TRUE; }
		}
		if (nScanCnt == 1) {
			if( pInfo[3] == 9 ||pInfo[8] == 9 ) { return TRUE; }
		}
		if (nScanCnt == 2) {
			if( pInfo[2] == 9 || pInfo[7] == 9) { return TRUE; }
		}
		if (nScanCnt == 3) {
			if( pInfo[1] == 9 || pInfo[6] == 9 ) { return TRUE; }
		}
		if (nScanCnt == 4) {
			if( pInfo[0] == 9 || pInfo[5] == 9) { return TRUE; }
		}
	}
	return FALSE;
}

BOOL CSequenceMain::Select_Top2ScanPos(int nPos, int nScanCnt)
{
	int *pInfo = NULL;
	if (nPos == AUTO_INSPECTION1) pInfo = gData.InfoInspect[0];
	if (nPos == AUTO_INSPECTION2) pInfo = gData.InfoInspect[1];
	if (nPos == AUTO_INSPECTION3) pInfo = gData.InfoInspect[2];

	if (pInfo == NULL) return FALSE;

	if (gData.bR04C) {	// Btm Picker 4개만 사용.
		if (nScanCnt == 0) {
			if( pInfo[3] == 9 || pInfo[8] == 9) { return TRUE; }
		}
		if (nScanCnt == 1) {
			if( pInfo[2] == 9 || pInfo[7] == 9) { return TRUE; }
		}
		if (nScanCnt == 2) {
			if( pInfo[1] == 9 || pInfo[6] == 9) { return TRUE; }
		}
		if (nScanCnt == 3) {
			if( pInfo[0] == 9 || pInfo[5] == 9) { return TRUE; }
		}

	} else {
		if (nScanCnt == 0) {
			if( pInfo[4] == 9 || pInfo[9] == 9) { return TRUE; }
		}
		if (nScanCnt == 1) {
			if( pInfo[3] == 9 || pInfo[8] == 9) { return TRUE; }
		}
		if (nScanCnt == 2) {
			if( pInfo[2] == 9 || pInfo[7] == 9) { return TRUE; }
		}
		if (nScanCnt == 3) {
			if( pInfo[1] == 9 || pInfo[6] == 9) { return TRUE; }
		}
		if (nScanCnt == 4) {
			if( pInfo[0] == 9 || pInfo[5] == 9) { return TRUE; }
		}
	}
	return FALSE;
}


BOOL CSequenceMain::Select_AngleTrayPos(int nNo, int &nTrayPosY, int &nRow)
{
	nTrayPosY = nRow = -1;
	if (Check_AngleTrayEmpty()) return FALSE;

	if (gData.bR04A || gData.bR13A) {
		for (int y = 0; y < gData.nTrayX; y++) {
			if (gData.InfoAngleTray[nNo-1][0][y] > 0) { nTrayPosY = y; nRow = y%2; break;}
		}
	} else {
		if (gData.bR04C) {
			for (int y = 0; y <gData.nTrayY; y++) {
				if (gData.InfoAngleTray[nNo-1][y][0] > 0) { nTrayPosY = y; nRow = 0; break;}
				if (gData.InfoAngleTray[nNo-1][y][4] > 0) { nTrayPosY = y; nRow = 1; break;}
			}
		} else {
			for (int y = 0; y <gData.nTrayY; y++) {
				if (gData.InfoAngleTray[nNo-1][y][0] > 0) { nTrayPosY = y; nRow = y%2; break;}
			}
		}
	}
	if (nTrayPosY == -1 || nRow == -1) return FALSE;

	return TRUE;
}

BOOL CSequenceMain::Select_Btm1PickPos(int &nPos)
{
	if (gData.bR04C) {
		if (gData.InfoBtm1Pick[0] == 0) { nPos = 0; return TRUE; }
		if (gData.InfoBtm1Pick[5] == 0) { nPos = 5; return TRUE; }
	} else {
		for (int i = 0; i < 10; i++) { if (gData.InfoBtm1Pick[i] == 0) { nPos = i; return TRUE; } }	// 1,2,3,4,5 순서로...
	}
	return FALSE;
}

BOOL CSequenceMain::Select_Btm2PickPos(int &nPos)
{
	if (gData.bR04C) {
		if (gData.InfoBtm2Pick[0] > 0) { nPos = 0; return TRUE; }
		if (gData.InfoBtm2Pick[5] > 0) { nPos = 5; return TRUE; }
	} else {
		for (int i = 0; i < 10; i++) { if (gData.InfoBtm2Pick[i] > 0) { nPos = i; return TRUE; } }	// 1,2,3,4,5 순서로...
	}	
	return FALSE;
}

BOOL CSequenceMain::Select_SortPickGoodPos(int nNo, int &nPos, int &nCnt)
{
	nPos = -1; nCnt =  0;
	
	for (int i = 0; i < 5; i++) { 
		if (gData.InfoSortPick[nNo-1][i] == 1) { nPos = i; break; } 
	}

	for (int j = nPos; j < 5; j++) { 
		if (gData.InfoSortPick[nNo-1][j] == 1)	nCnt++;
		else									break;
	}
	if (nPos == -1 || nCnt == 0) return FALSE;

	return TRUE;
}

BOOL CSequenceMain::Select_SortPickNgPos(int nNo, int &nPos, int &nCnt, BOOL bBuffer)
{
	nPos = -1; nCnt = 0;

	if (bBuffer) {	// Ng Buffer 전용
		for (int i = 0; i < 5; i++) { if (gData.InfoSortPick[nNo-1][i] >= 2) { nPos = i; break; } }
		for (int i = nPos; i < 5; i++) { if (gData.InfoSortPick[nNo-1][i] < 2) break; nCnt++; }

	} else {
		int *pInfo = gData.InfoSortPick[nNo-1];

		for (int i = 0; i < 5; i++) { if (pInfo[i] == 2 || pInfo[i] == 4 || pInfo[i] == 6 || pInfo[i] == 7) { nPos = i; break; } }	// Tray1,3

		if (nPos == -1) {
			for (int i = 0; i < 5; i++) { if (pInfo[i] == 5) { nPos = i; break; } }	// Tray4
			if (nPos == -1) {
				for (int i = 0; i < 5; i++) { if (pInfo[i] == 8) { nPos = i; break; } }	// Tray2
			}
		}
		if (nPos == -1) return FALSE;

		for (int i = nPos; i < 5; i++) { if (pInfo[i] != pInfo[nPos]) break; nCnt++; }
	}
	if (nPos == -1 || nCnt == 0) return FALSE;

	return TRUE;
}

BOOL CSequenceMain::Select_BufferTrayDownPos(int nNo, int &nTrayPosY, int &nRow)
{
	nTrayPosY = nRow = -1;
	if (Check_BufferFull(nNo-1)) return FALSE;

	if (gData.bR04A || gData.bR13A) {
		for (int y = 0; y < gData.nTrayX; y++) {
			if (gData.InfoBuffTray[nNo-1][0][y] == 0) { nTrayPosY = y; nRow = y%2; return TRUE; }
		}
	} else {
		if (gData.bR04C) {
			for (int y = 0; y < gData.nTrayY; y++) {
				if (gData.InfoBuffTray[nNo-1][y][0] == 0) { nTrayPosY = y; nRow = 0; return TRUE; }
				if (gData.InfoBuffTray[nNo-1][y][4] == 0) { nTrayPosY = y; nRow = 1; return TRUE; }
			}
		} else {
			for (int y = 0; y < gData.nTrayY; y++) {
				if (gData.InfoBuffTray[nNo-1][y][0] == 0) { nTrayPosY = y; nRow = y%2; return TRUE; }
			}
		}
	}
	
	return FALSE;
}

BOOL CSequenceMain::Select_BufferTrayUpPos(int nNo, int &nTrayPosX, int &nTrayPosY)
{
	nTrayPosX = -1; nTrayPosY = -1;
	if (Check_BufferEmpty(nNo)) return FALSE;

	for (int y = 0; y < gData.nTrayY; y++) {
		for (int x = 0; x < gData.nTrayX; x++) {
			if (gData.InfoBuffTray[nNo-1][y][x] > 0) { nTrayPosX = x; nTrayPosY = y; return TRUE; }
		}
	}
	return FALSE;
}

BOOL CSequenceMain::Select_GoodTrayPos(int &nTrayPosX, int &nTrayPosY)
{
	nTrayPosX = nTrayPosY = -1;
	if (Check_GoodTrayFull()) return FALSE;

	for (int y = 0; y < gData.nTrayY; y++) {
		for (int x = 0; x < gData.nTrayX; x++) {
			if (gData.InfoGoodTray[y][x] == 0) { nTrayPosX = x; nTrayPosY = y; return TRUE; }
		}
	}
	return FALSE;
}

BOOL CSequenceMain::Select_NgTrayPos(int nPickNo, int &nTrayNo, int &nTrayPosX, int &nTrayPosY)
{
	nTrayNo = nTrayPosX = nTrayPosY = -1;
	if (Check_NgTrayFull()) return FALSE;

	int *pInfo = gData.InfoSortPick[nPickNo-1];
	int nTray1 = -1, nTray2 = -1, nInfo = 0;	// Default
	
	if (!m_pEquipData->bUseSNgMix) {
		for (int i = 0; i < 5; i++) { 
			if (pInfo[i] == 2 || pInfo[i] == 4 || pInfo[i] == 6 || pInfo[i] == 7) { nInfo = pInfo[i]; break; }	// Tray1,3
		}
		if (nInfo == 0) {
			for (int i = 0; i < 5; i++) {
				if (pInfo[i] == 5) { nInfo = pInfo[i]; break; }	// Tray4
			}
			if (nInfo == 0) {
				for (int i = 0; i < 5; i++) {
					if (pInfo[i] == 8) { nInfo = pInfo[i]; break; }	// Tray2
				}
			}
		}
		if (nInfo == 0) return FALSE;

		if (nInfo == 2 || nInfo == 4 || nInfo == 6 || nInfo == 7) { nTray1 = 0; nTray2 = 2; }	// N,N1,N3,B
		if (nInfo == 5) nTray1 = 3;		// N2
		if (nInfo == 8) nTray1 = 1;		// N4
	}

	for (int t = 3; t >= 0; t--) {
		if (t != nTray1 && t != nTray2) continue;
		for (int y = 0; y < gData.nTrayY; y++) {
			for (int x = 0; x < gData.nTrayX; x++) {
				if (gData.InfoNgTray[t][y][x] == 0) { nTrayNo = t; nTrayPosX = x; nTrayPosY = y; return TRUE; }
			}
		}
	}

	return FALSE;
}

BOOL CSequenceMain::Select_NgBufferPos(int nPickNo, int &nTrayPosX)
{
	int nBNo = 0;
	nTrayPosX = -1;
	if (Check_NgBufferFull(nPickNo)) return FALSE;

	if (nPickNo == 1) nBNo = 1;
	if (nPickNo == 2) nBNo = 0;

	for (int x = 0; x < gData.nSortPickQt; x++) {
		if (gData.InfoNgBuffer[nBNo][x] == 0) { nTrayPosX = x; return TRUE; }
	}
	return FALSE;
}

BOOL CSequenceMain::Select_SortPickGoodMulti(int nNo, int &nPos, int &nCnt)
{
	nPos = -1, nCnt = 0;
	for (int i = 0; i < 5; i++) { if (gData.InfoSortPick[nNo-1][i] == 1) { nPos = i; nCnt = 1; break; } }
	if (nPos < 0) return FALSE;
	for (int i = nPos + 1; i < 5; i++) { if (gData.InfoSortPick[nNo-1][i] != 1) break; nCnt++; }
	return TRUE;
}

BOOL CSequenceMain::Select_SortPickNgMulti(int nNo, int &nPos, int &nCnt)
{
	nPos = -1, nCnt = 0;
	for (int i = 0; i < 5; i++) { if (gData.InfoSortPick[nNo-1][i] >= 2) { nPos = i; nCnt = 1; break; } }
	if (nPos < 0) return FALSE;
	if (m_pEquipData->bUseSNgMix) {
		for (int i = nPos + 1; i < 5; i++) { if (gData.InfoSortPick[nNo-1][i] >= 2) break; nCnt++; }
	} else {
		for (int i = nPos + 1; i < 5; i++) { if (gData.InfoSortPick[nNo-1][i] != gData.InfoSortPick[nNo-1][nPos]) break; nCnt++; }
	}
	return TRUE;
}

void CSequenceMain::Set_InspectJigNo(int nType, int nScanCnt, int &nINo1, int &nINo2, int &nINo3, int &nINo4)
{
	if (gData.bR04C) 
	{
		if (nType == 0) {	// BTM1
			if (nScanCnt == 0) { nINo1 = 9; nINo2 = 4; nINo3 = 0; nINo4 = 0;}
			if (nScanCnt == 1) { nINo1 = 8; nINo2 = 3; nINo3 = 0; nINo4 = 0;}
			if (nScanCnt == 2) { nINo1 = 7; nINo2 = 2; nINo3 = 0; nINo4 = 0;}
			if (nScanCnt == 3) { nINo1 = 6; nINo2 = 1; nINo3 = 0; nINo4 = 0;}
		}
		if (nType == 1) {	// TOP1
			if (nScanCnt == 0) { nINo1 = 4; nINo2 = 9; nINo3 = 0; nINo4 = 0;}
			if (nScanCnt == 1) { nINo1 = 3; nINo2 = 8; nINo3 = 0; nINo4 = 0;}
			if (nScanCnt == 2) { nINo1 = 2; nINo2 = 7; nINo3 = 0; nINo4 = 0;}
			if (nScanCnt == 3) { nINo1 = 1; nINo2 = 6; nINo3 = 0; nINo4 = 0;}
		}
		if (nType == 2) {	// TOP2
			if (nScanCnt == 0) { nINo1 = 9; nINo2 = 4; nINo3 = 0; nINo4 = 0;}
			if (nScanCnt == 1) { nINo1 = 8; nINo2 = 3; nINo3 = 0; nINo4 = 0;}
			if (nScanCnt == 2) { nINo1 = 7; nINo2 = 2; nINo3 = 0; nINo4 = 0;}
			if (nScanCnt == 3) { nINo1 = 6; nINo2 = 1; nINo3 = 0; nINo4 = 0;}
		}
		if (nType == 3) {	// BTM2
			if (nScanCnt == 0) { nINo1 = 9; nINo2 = 4; nINo3 = 0; nINo4 = 0; }
			if (nScanCnt == 1) { nINo1 = 8; nINo2 = 3; nINo3 = 0; nINo4 = 0; }
			if (nScanCnt == 2) { nINo1 = 7; nINo2 = 2; nINo3 = 0; nINo4 = 0; }
			if (nScanCnt == 3) { nINo1 = 6; nINo2 = 1; nINo3 = 0; nINo4 = 0; }
		}
	} 
	else
	{
		if (nType == 0) 
		{	// BTM1
			if (nScanCnt == 0) { nINo1 = 10; nINo2 = 5; nINo3 = 0; nINo4 = 0;}
			if (nScanCnt == 1) { nINo1 = 9; nINo2 = 4; nINo3 = 0; nINo4 = 0;}
			if (nScanCnt == 2) { nINo1 = 8; nINo2 = 3; nINo3 = 0; nINo4 = 0;}
			if (nScanCnt == 3) { nINo1 = 7; nINo2 = 2; nINo3 = 0; nINo4 = 0;}
			if (nScanCnt == 4) { nINo1 = 6; nINo2 = 1; nINo3 = 0; nINo4 = 0;}

		}
		if (nType == 1) {	// TOP1
			if (nScanCnt == 0) { nINo1 = 5; nINo2 = 10; nINo3 = 0; nINo4 = 0;}
			if (nScanCnt == 1) { nINo1 = 4; nINo2 = 9; nINo3 = 0; nINo4 = 0;}
			if (nScanCnt == 2) { nINo1 = 3; nINo2 = 8; nINo3 = 0; nINo4 = 0;}
			if (nScanCnt == 3) { nINo1 = 2; nINo2 = 7; nINo3 = 0; nINo4 = 0;}
			if (nScanCnt == 4) { nINo1 = 1; nINo2 = 6; nINo3 = 0; nINo4 = 0;}

		}
		if (nType == 2) {	// TOP2
	// 		if (nScanCnt == 0) { nINo1 = 5; nINo2 = 10; nINo3 = 0; nINo4 = 0;}
	// 		if (nScanCnt == 1) { nINo1 = 4; nINo2 =  9; nINo3 = 0; nINo4 = 0;}
	// 		if (nScanCnt == 2) { nINo1 = 3; nINo2 =  8; nINo3 = 0; nINo4 = 0;}
	// 		if (nScanCnt == 3) { nINo1 = 2; nINo2 =  7; nINo3 = 0; nINo4 = 0;}
	// 		if (nScanCnt == 4) { nINo1 = 1; nINo2 =  6; nINo3 = 0; nINo4 = 0;}
			if (nScanCnt == 0) { nINo1 = 10; nINo2 = 5; nINo3 = 0; nINo4 = 0;}
			if (nScanCnt == 1) { nINo1 =  9; nINo2 = 4; nINo3 = 0; nINo4 = 0;}
			if (nScanCnt == 2) { nINo1 =  8; nINo2 = 3; nINo3 = 0; nINo4 = 0;}
			if (nScanCnt == 3) { nINo1 =  7; nINo2 = 2; nINo3 = 0; nINo4 = 0;}
			if (nScanCnt == 4) { nINo1 =  6; nINo2 = 1; nINo3 = 0; nINo4 = 0;}
		}
		if (nType == 3) {	// BTM2
			if (nScanCnt == 0) { nINo1 = 10; nINo2 = 5; nINo3 = 0; nINo4 = 0;}
			if (nScanCnt == 1) { nINo1 =  9; nINo2 = 4; nINo3 = 0; nINo4 = 0;}
			if (nScanCnt == 2) { nINo1 =  8; nINo2 = 3; nINo3 = 0; nINo4 = 0;}
			if (nScanCnt == 3) { nINo1 =  7; nINo2 = 2; nINo3 = 0; nINo4 = 0;}
			if (nScanCnt == 4) { nINo1 =  6; nINo2 = 1; nINo3 = 0; nINo4 = 0;}
		}
	}
}
///////////////////////////////////////////////////////////////////////////////
BOOL CSequenceMain::Check_InspectDone(int nPortNo, int nTrayNo, int nCmNo, int &nInfo, int nSortNo)
{
	if (nPortNo == 0 || nTrayNo == 0 || nCmNo == 0 || nInfo == 0) return TRUE;
	if (((gData.byInspectDone[nPortNo-1][nTrayNo-1][nCmNo-1] >> 7) & 1) == 1) return TRUE;	// 판정 완료 (2번 판정하지 않기 위해)

	
#ifdef DRY_RUN_TEST
	int nRand = g_objCommon.Get_Random(0, 99);
	int nNg1 = m_pEquipData->nResultTestN1;
	int nNg2 = m_pEquipData->nResultTestN2 + nNg1;
	int nNg3 = m_pEquipData->nResultTestN3 + nNg2;
	int nNg4 = m_pEquipData->nResultTestN4 + nNg3;

	int nJudge = (nRand < nNNg ? 2 : (nRand < nSNg ? 4 : (nRand < nS2Ng ? 5 : 1)));
	nInfo = gData.nInspectInfo[nPortNo-1][nTrayNo-1][nCmNo-1] = nJudge;
	if (nInfo == 9) { nInfo = gData.nInspectInfo[nPortNo-1][nTrayNo-1][nCmNo-1] = 1; }
#else
	
	EQUIP_DATA *m_pEquipData = g_objDataManager.Get_pEquipData();

	BOOL bRet = TRUE;
	if (m_pEquipData->bUseInspectAngle && ((gData.byInspectDone[nPortNo-1][nTrayNo-1][nCmNo-1] >> 0) & 1) == 0) bRet = FALSE; //return FALSE;	// Angle
	if (m_pEquipData->bUseInspectBtm1  && ((gData.byInspectDone[nPortNo-1][nTrayNo-1][nCmNo-1] >> 1) & 1) == 0) bRet = FALSE; //return FALSE;	// Btm1
	if (m_pEquipData->bUseInspectTop1  && ((gData.byInspectDone[nPortNo-1][nTrayNo-1][nCmNo-1] >> 2) & 1) == 0) bRet = FALSE; //return FALSE;	// Top1	
	if (m_pEquipData->bUseInspectTop2  && ((gData.byInspectDone[nPortNo-1][nTrayNo-1][nCmNo-1] >> 3) & 1) == 0) bRet = FALSE; //return FALSE;	// Top2
	if (m_pEquipData->bUseInspectBtm2  && ((gData.byInspectDone[nPortNo-1][nTrayNo-1][nCmNo-1] >> 4) & 1) == 0) bRet = FALSE; //return FALSE;	// Btm2

	int tempTick = 0;
	if(nSortNo == 1) tempTick = gData.nSp1Timer;
	else if(nSortNo == 2) tempTick = gData.nSp2Timer;

	ROS_DATA *pRosData = g_objDataManager.Get_pRosData();
	if(!bRet)
	{
		if (m_pEquipData->bUseInspectSkip || (GetTickCount() - tempTick > pRosData->nJudgeTimeOver) ) 
		{
			// SortPicker에서 검사 완료 체크할때 검사결과가 안날라왔으면 1차로 빼준다.
			gData.nInspectInfo[nPortNo-1][nTrayNo-1][nCmNo-1] = 4;
			gMes.sJudge[nPortNo-1][nTrayNo-1][nCmNo-1] = "N1";
			CString strLog;
			//strLog.Format("Judge Time Over Sort Picker (%d msec), PortNo (%d),TrayNo (%d), CmNo (%d)", pRosData->nJudgeTimeOver, nPortNo, nTrayNo, nCmNo);
			strLog.Format("Judge Time Over Sort Picker, PortNo (%d),TrayNo (%d), CmNo (%d)", nPortNo, nTrayNo, nCmNo);
			g_objLogFile.Save_HandlerLog(strLog);

		} 
		else 
		{
			return FALSE;
		}
	}
	
	CString strTemp;

	int nWaitTime = 0;
	if		(nSortNo == 1)	nWaitTime = GetTickCount() - gData.nSp1Timer;
	else if (nSortNo == 2)  nWaitTime = GetTickCount() - gData.nSp2Timer;
	strTemp.Format("%d,%d,%d,%s,%d", nPortNo, nTrayNo, nCmNo, gData.sSortWaitStartTime[nSortNo-1], nWaitTime);
	g_objLogFile.Save_InspectWaitLog(nPortNo, strTemp);

	// 에러처리 하지 않음 (2018.04.10)
	//if (m_pEquipData->bUseMesNg && gMes.sBarID[nTrayNo-1][nCmNo-1].GetLength() > 10 && gMes.nCmResult[nTrayNo-1][nCmNo-1] == 0)  return FALSE;	// 성능불량
	//CString strTemp;

	
	if (m_pEquipData->bResultTestUse) {	// TEST
		int nRand = g_objCommon.Get_Random(0, 99);
		int nNg1 = m_pEquipData->nResultTestN1;
		int nNg2 = m_pEquipData->nResultTestN2 + nNg1;
		int nNg3 = m_pEquipData->nResultTestN3 + nNg2;
		int nNg4 = m_pEquipData->nResultTestN4 + nNg3;
		
		int nJudge = (nRand < nNg1 ? 2 : (nRand < nNg2 ? 4 : (nRand < nNg3 ? 5 : (nRand < nNg4 ? 2 : 1))));
		nInfo = gData.nInspectInfo[nPortNo - 1][nTrayNo - 1][nCmNo -1] = nJudge;
		if (nInfo == 9) { nInfo = gData.nInspectInfo[nPortNo - 1][nTrayNo - 1][nCmNo -1] = 1; }
		strTemp.Format("ResultTest_Use : %d,%d,%d",nInfo, nTrayNo , nCmNo);
		g_objLogFile.Save_TestLog(strTemp);
	} 
	else
	{
		if (gData.bCycleStop && !Get_VisionInspectUse()) 
		{
			nInfo = gData.nInspectInfo[nPortNo-1][nTrayNo-1][nCmNo-1] = 1;	//Good
		}
		else if (Get_VisionInspectUse())  
		{
			nInfo = gData.nInspectInfo[nPortNo-1][nTrayNo-1][nCmNo-1];
		}


		if (nInfo == 9) { nInfo = gData.nInspectInfo[nPortNo-1][nTrayNo-1][nCmNo-1] = 1; }
	}
#endif


#ifdef VISION_REPEAT
	nInfo = gData.nInspectInfo[nPortNo-1][nTrayNo-1][nCmNo-1] = 1;		// 반복성 검증일때 모두 양품
#endif

	gData.byInspectDone[nPortNo-1][nTrayNo-1][nCmNo-1] |= (1 << 7);	// 판정 완료 (2번 판정하지 않기 위해)
	return TRUE;	// All Inspect Done
}



void CSequenceMain::Set_InspectError(int nPortNo, int nTrayNo, int nCmNo)
{
	// byInspectDone[2][40][45] : 검사완료 (5bit 사용 ==> 0:Angle, 1:Btm1, 2:top1, 3:Top2, 4:Btm2)
	CString strErrMsg = "";
	if		(m_pEquipData->bUseInspectAngle && ((gData.byInspectDone[nPortNo-1][nTrayNo-1][nCmNo-1] >> 0) & 1) == 0) strErrMsg.Format(" ==> Tray:%d. CM:%d. Angle", nTrayNo, nCmNo);	// Angle
	else if	(m_pEquipData->bUseInspectBtm1  && ((gData.byInspectDone[nPortNo-1][nTrayNo-1][nCmNo-1] >> 1) & 1) == 0) strErrMsg.Format(" ==> Tray:%d. CM:%d. Btm1",  nTrayNo, nCmNo);	// Btm1
	else if (m_pEquipData->bUseInspectTop1  && ((gData.byInspectDone[nPortNo-1][nTrayNo-1][nCmNo-1] >> 2) & 1) == 0) strErrMsg.Format(" ==> Tray:%d. CM:%d. Top1",  nTrayNo, nCmNo);	// Top1
	else if (m_pEquipData->bUseInspectTop2  && ((gData.byInspectDone[nPortNo-1][nTrayNo-1][nCmNo-1] >> 3) & 1) == 0) strErrMsg.Format(" ==> Tray:%d. CM:%d. Top2",  nTrayNo, nCmNo);	// Top2
	else if (m_pEquipData->bUseInspectBtm2  && ((gData.byInspectDone[nPortNo-1][nTrayNo-1][nCmNo-1] >> 4) & 1) == 0) strErrMsg.Format(" ==> Tray:%d. CM:%d. Btm2",  nTrayNo, nCmNo);	// Btm2

	//else if (m_pEquipData->bUseDispatcher && !g_objDispatcher.Is_JudgeDone(nPortNo, nTrayNo, nCmNo)) strErrMsg.Format(" ==> Tray:%d. CM:%d. MDJ(Timeover %d)", nTrayNo, nCmNo, m_pRosData->nJudgeTimeOver);	// Dispatcher

	// 에러처리 하지 않음 (2018.04.10)
	//else if (m_pEquipData->bUseMesNg && gMes.nCmResult[nTrayNo-1][nCmNo-1] == 0) {
	//	strErrMsg.Format(" ==> MES 성능불량 미수신 Tray:%d, CM:%d LotID:%s, ModuleID:%s", nTrayNo, nCmNo, gLot.sLotID, gMes.sBarID[nTrayNo-1][nCmNo-1]);
	//	gMes.nCmResult[nTrayNo-1][nCmNo-1] = 2;
	//}

	if (strErrMsg != "") g_objCommon.Set_ErrorSubMessage(strErrMsg);
}

void CSequenceMain::Set_ScanError(int nErrNo, int nTrayNo1, int nTrayNo2, int nTrayNo3, int nTrayNo4, int nCmNo1, int nCmNo2, int nCmNo3, int nCmNo4)
{
	CString strErrMsg = "";
	if		(nErrNo == 3315) strErrMsg.Format(" ==> Tray:%d. CM:%d. Angle", nTrayNo1, nCmNo1);	// Angle
	else if	(nErrNo == 3610) strErrMsg.Format(" ==> Tray:%d. %d. %d. %d. CM:%d. %d. %d. %d. Btm1", nTrayNo1, nTrayNo2, nTrayNo3, nTrayNo4, nCmNo1, nCmNo2, nCmNo3, nCmNo4);	// Btm1
	else if (nErrNo == 3710 || nErrNo == 3810 || nErrNo == 3910) strErrMsg.Format(" ==> Tray:%d. %d. %d. %d. CM:%d. %d. %d. %d. Top1",  nTrayNo1, nTrayNo2, nTrayNo3, nTrayNo4, nCmNo1, nCmNo2, nCmNo3, nCmNo4);	// Top1
	else if (nErrNo == 3720 || nErrNo == 3820 || nErrNo == 3920) strErrMsg.Format(" ==> Tray:%d. %d. CM:%d. %d Top2",  nTrayNo1, nTrayNo2, nCmNo1, nCmNo2);	// Top2
	else if (nErrNo == 4010) strErrMsg.Format(" ==> Tray:%d. %d. %d. %d. CM:%d. %d. %d. %d. Btm2",  nTrayNo1, nTrayNo2, nTrayNo3, nTrayNo4, nCmNo1, nCmNo2, nCmNo3, nCmNo4);	// Btm2

	if (strErrMsg != "") g_objCommon.Set_ErrorSubMessage(strErrMsg);
}

void CSequenceMain::Write_LotJudge(int nPNo, int nTrayNo, int nCmNo, int nInfo)
{
	CString strTemp;
	
	int nPx = nPNo - 1;
	int nTx = nTrayNo - 1;
	int nCx = nCmNo - 1;

	if (nPx < 0 || nPx > 1 || nTx < 0 || nTx > 29 || nCx < 0 || nCx > 39 || nInfo == 0) {
		strTemp.Format("Exception Write Judge(1) : %d,%d,%d,%d", nPx+1, nTx+1, nCx+1, nInfo);
		g_objLogFile.Save_TestLog(strTemp);
		return;
	}

	char chCode[7];	// 판정코드 (0:AG, 1:B1, 2:T1, 3:T2, 4:B2)
	if(nPNo == 0){
		memcpy(chCode, gData.cJudgeCode1[nTrayNo][nCmNo], sizeof(char) * 7);
	}
	if(nPNo == 1){
		memcpy(chCode, gData.cJudgeCode2[nTrayNo][nCmNo], sizeof(char) * 7);
	}
	

	for (int i = 0; i < 5; i++)
	{ 
		if(i == 0 && m_pEquipData->bUseInspectAngle && chCode[i] == 0)
		{
			chCode[i] = 0x6E; //  비전 결과가 안오면 공란으로 두지말고 E으로 기록해 달라. (From kjw request)
			strTemp.Format("Exception Write Judge(2) : %d,%d,%d,%d,%d", nPx+1, nTx+1, nCx+1, nInfo, i);
			g_objLogFile.Save_TestLog(strTemp);
		}
		else if(i == 1 && m_pEquipData->bUseInspectBtm1 && chCode[i] == 0)
		{
			chCode[i] = 0x6E; //  비전 결과가 안오면 공란으로 두지말고 E으로 기록해 달라. (From kjw request)
			strTemp.Format("Exception Write Judge(2) : %d,%d,%d,%d,%d", nPx+1, nTx+1, nCx+1, nInfo, i);
			g_objLogFile.Save_TestLog(strTemp);
		}
		else if(i == 2 && m_pEquipData->bUseInspectTop1 && chCode[i] == 0)
		{
			chCode[i] = 0x6E; //  비전 결과가 안오면 공란으로 두지말고 E으로 기록해 달라. (From kjw request)
			strTemp.Format("Exception Write Judge(2) : %d,%d,%d,%d,%d", nPx+1, nTx+1, nCx+1, nInfo, i);
			g_objLogFile.Save_TestLog(strTemp);
		}
		else if(i == 3 && m_pEquipData->bUseInspectTop2 && chCode[i] == 0)
		{
			chCode[i] = 0x6E; //  비전 결과가 안오면 공란으로 두지말고 E으로 기록해 달라. (From kjw request)
			strTemp.Format("Exception Write Judge(2) : %d,%d,%d,%d,%d", nPx+1, nTx+1, nCx+1, nInfo, i);
			g_objLogFile.Save_TestLog(strTemp);
		}
		else if(i == 4 && m_pEquipData->bUseInspectBtm2 && chCode[i] == 0)
		{
			chCode[i] = 0x6E; //  비전 결과가 안오면 공란으로 두지말고 E으로 기록해 달라. (From kjw request)
			strTemp.Format("Exception Write Judge(2) : %d,%d,%d,%d,%d", nPx+1, nTx+1, nCx+1, nInfo, i);
			g_objLogFile.Save_TestLog(strTemp);
		}
		else if (chCode[i] == 0) {
			chCode[i] = 0x45; //  비전 결과가 안오면 공란으로 두지말고 E으로 기록해 달라. (From kjw request)
			strTemp.Format("Exception Write Judge(2) : %d,%d,%d,%d,%d", nPx+1, nTx+1, nCx+1, nInfo, i);
			g_objLogFile.Save_TestLog(strTemp);
		}
	
	}

	CString strCode;	// Log 순서 : AG, B1, T1, T2, B2
	strCode.Format("%c,%c,%c,%c,%c,%d", chCode[0], chCode[1], chCode[2], chCode[3], chCode[4], nInfo);
	CString strResult = (nInfo == 1 ? "Pass" : "Fail");
//	int nJigNo = gData.nIndexJigNo[nTrayNo-1][nCmNo-1];
	for (int i = 0; i < 5; i++)
	{
		if(chCode[i] == 0x6E) strResult = "Fail";
	}

 	m_strLog.Format("%s,%s,%s,%d,%d,%s,%s,%s", m_pEquipData->sEquipName, MAIN_VERSION, gData.sLotID[nPNo-1], nTrayNo, nCmNo, gMes.sBarID[nPNo-1][nTrayNo-1][nCmNo-1], strCode, strResult);
	g_objLogFile.Save_LotTime(nPNo, m_strLog);
}

// 한번에 2Lot 이상 들어가면 안된다.
BOOL CSequenceMain::Check_LoadTrayLoading(int nPNo)
{
	if (gData.nPNoAnglePort[0] == nPNo) return FALSE;
	if (gData.nPNoBtm1Pick == nPNo) return FALSE;
	if (gData.nPNoInspect[0] == nPNo) return FALSE;
	if (gData.nPNoInspect[1] == nPNo) return FALSE;
	if (gData.nPNoInspect[2] == nPNo) return FALSE;
	if (gData.nPNoBtm1Pick == nPNo) return FALSE;
	if (gData.nPNoBuffTray[0] == nPNo) return FALSE;
	if (gData.nPNoBuffTray[1] == nPNo) return FALSE;
	if (gData.nPNoGoodTray == nPNo) return FALSE;
	if (gData.nPNoNgTray == nPNo) return FALSE;

	return TRUE;
}

BOOL CSequenceMain::Check_LoadLotEnd(int nPNo, int nMode)
{
	// 로드가 다끝나고 앞공정에 모듈이 없으면 TRUE
	if (!gData.bCycleStop && (gData.nLoadTrayCount[nPNo-1] < gData.nTrayUseCount[nPNo-1])) return FALSE;
	if (gData.nPNoAnglePort[0] == nPNo) return FALSE;
	if (gData.nPNoAngleTray[0] == nPNo || gData.nPNoAngleTray[1] == nPNo) return FALSE;
// 	if (!Check_AngleTrayEmpty()) return FALSE;
	if (nMode == 0 && !Check_Btm1PickerEmpty()) return FALSE;

	return TRUE;
}

BOOL CSequenceMain::Check_UnloadLotEnd(int nMode)
{
// 	if (!m_bLoadLotEnd) return FALSE;
	if (m_nAngleTray1Case == 30 && m_pDX02->iAngleStage1Exist) return FALSE;	// 빈트레이 언로딩 해야한다.
	if (m_nAngleTray2Case == 30 && m_pDX02->iAngleStage2Exist) return FALSE;	// 빈트레이 언로딩 해야한다.
	if (!Check_AngleTrayEmpty()) return FALSE;
	if (!Check_Btm1PickerEmpty()) return FALSE;
	if (!Check_InspetionEmpty(1)) return FALSE;
	if (!Check_InspetionEmpty(2)) return FALSE;
	if (!Check_InspetionEmpty(3)) return FALSE;
	if (!Check_Btm2PickerEmpty()) return FALSE;
	if (!Check_BufferEmpty()) return FALSE;
	if (nMode == 0 && !Check_NgBufferEmpty(1)) return FALSE;
	if (nMode == 0 && !Check_NgBufferEmpty(2)) return FALSE;
	if (nMode == 0 && !Check_SortPickerEmpty(1)) return FALSE;
	if (nMode == 0 && !Check_SortPickerEmpty(2)) return FALSE;

	return TRUE;
}

BOOL CSequenceMain::Check_Btm2LoadEnd(int nPNo)
{
	if (gData.nPNoAnglePort[0] == nPNo) return FALSE;
	if (gData.nPNoAngleTray[0] == nPNo) return FALSE;
	if (gData.nPNoAngleTray[1] == nPNo) return FALSE;
	if (gData.nPNoBtm1Pick == nPNo) return FALSE;
	if (gData.nPNoInspect[0] == nPNo) return FALSE;
	if (gData.nPNoInspect[1] == nPNo) return FALSE;
	if (gData.nPNoInspect[2] == nPNo) return FALSE;
	if (gData.nPNoBtm2Pick == nPNo) return FALSE;
	return TRUE;
}

BOOL CSequenceMain::Check_InspectLotEnd(int nPNo, int nSortPickNo)
{
// 	if (nSortPickNo == 1 && (gData.nPNoSortPick[1] == nPNo)) return FALSE;
// 	if (nSortPickNo == 2 && (gData.nPNoSortPick[0] == nPNo)) return FALSE;
// 	if (gData.nPNoNgBuffer[0] == nPNo) return FALSE;
// 	if (gData.nPNoNgBuffer[1] == nPNo) return FALSE;
	if (gData.nPNoSortPick[0] == nPNo) return FALSE;
	if (gData.nPNoSortPick[1] == nPNo) return FALSE;
	if (gData.nPNoBuffTray[0] == nPNo) return FALSE;
	if (gData.nPNoBuffTray[1] == nPNo) return FALSE;
	if (gData.nPNoBtm2Pick == nPNo) return FALSE;
	if (gData.nPNoInspect[0] == nPNo) return FALSE;
	if (gData.nPNoInspect[1] == nPNo) return FALSE;
	if (gData.nPNoInspect[2] == nPNo) return FALSE;
	if (gData.nPNoBtm1Pick == nPNo) return FALSE;
	if (gData.nPNoAngleTray[0] == nPNo) return FALSE;
	if (gData.nPNoAngleTray[1] == nPNo) return FALSE;
	if (gData.nPNoAnglePort[0] == nPNo) return FALSE;
	if (gData.nPNoTrayPick == nPNo) return FALSE;

	return TRUE;
}

BOOL CSequenceMain::Check_NgBufferLotEnd(int nPNo)
{
// 	if (Check_NgBufferEmpty(nPNo)) return TRUE;

	if (gData.nPNoNgBuffer[0] == nPNo) return FALSE;
	if (gData.nPNoNgBuffer[1] == nPNo) return FALSE;

	return TRUE;
}

BOOL CSequenceMain::Get_VisionInspectUse()
{
	BOOL bUse = (m_pEquipData->bUseInspectAngle || m_pEquipData->bUseInspectBtm1 || m_pEquipData->bUseInspectTop1 || m_pEquipData->bUseInspectTop2 || m_pEquipData->bUseInspectBtm2);
	return bUse;
}

///////////////////////////////////////////////////////////////////////////////

void CSequenceMain::Job_LotStart(int nPortNo)
{
	int nLPNo = nPortNo - 1;
	g_objInspector.Set_LotStart(INSPECTOR_ALL, gData.sLotID[nLPNo], nPortNo, m_pEquipData->sVisionProgVer, m_pEquipData->sVisionParaVer);
	gLot.bLotEndComplete[nLPNo] = FALSE;
	if (!gData.bVisionVerErr) {
		g_objDispatcher.Set_LotStart(nPortNo);

		SYSTEMTIME time;
		GetLocalTime(&time);

		gLot.sLotID[nLPNo] = gData.sLotID[nLPNo];
		gLot.sStartTime[nLPNo].Format("%04d%02d%02d_%02d%02d%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
		m_dwFirstLoad = gLot.dwLotStart[nLPNo] = GetTickCount();
		gLot.nTrayCount[nLPNo] = gData.nTrayUseCount[nLPNo];
		gLot.nCmCount[nLPNo] = gData.nCmUseCount[nLPNo];

 		gLot.nErrorCount = 0;
 		if (gData.bFirstLotStart == TRUE) {
			gData.bFirstLotStart = FALSE;
			gLot.dwRunTime = gLot.dwErrorTime = gLot.dwStopTime = 0;
		}

		g_objLogFile.Save_Print(0, 0, 0, 0, 0, 0);	//Lot_Start

		g_dlgWork.PostMessage(UM_LOT_START_END, (WPARAM)1, nPortNo);	// LotStart
	}
}

void CSequenceMain::Job_LotEnd(int nPortNo)
{
	int nLPNo = nPortNo - 1;
	if (gLot.nCmCount[nLPNo] < 1) return;	// Error
	if (gLot.bLotEndComplete[nLPNo] == TRUE) return;

	gLot.bLotEndComplete[nLPNo] = TRUE;

	SYSTEMTIME time;
	GetLocalTime(&time);
	gLot.dwLotEnd[nLPNo] = GetTickCount();
	gLot.sEndTime[nLPNo].Format("%04d%02d%02d_%02d%02d%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

	g_objInspector.Set_LotEnd(INSPECTOR_ALL, gData.sLotID[nLPNo], nPortNo);
	g_objDispatcher.Set_LotEnd(nPortNo);
	if (gData.bCycleStop == FALSE) g_objMES.Set_LotEnd(gLot.sLotID[nLPNo], gLot.nCmCount[nLPNo], gData.sOperID, gLot.nGoodCount[nLPNo], gLot.nNgCount[nLPNo], nLPNo);	// 2020.9.15+
	gData.bMesRegistered[nLPNo] = FALSE;

// 	//해당포트 판정 완료, 결과 초기화
// 	memset(gData.byInspectDone[nLPNo], 0x00, sizeof(BYTE) * 30 * 40);
// 	memset(gData.nInspectInfo[nLPNo], 0x00, sizeof(int) * 30 * 40);
// 	gData.nPNoNgTray = gData.nPNoGoodTray = 0;
// 	gData.nGoodTrayCount = 0;

//	if (m_dwULCycleTime != 0) m_dwULCycleTime = GetTickCount() - m_dwULPickUpTime;
// 	gLot.dwLotEnd[nLPNo] = GetTickCount();
// 	gLot.sEndTime[nLPNo].Format("%04d%02d%02d_%02d%02d%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

	DWORD dwTime = gLot.dwLotEnd[nLPNo] - gLot.dwLotStart[nLPNo];
	gLot.dTackTime = dwTime / 1000.0 / gLot.nCmCount[nLPNo];	// Floating-point inexact result
	m_dwLastUnLoad = gLot.dwLotEnd[nLPNo] - m_dwLastUnLoad;

	m_strLog.Format("LotID,%s,Start_Time,%s,End_Time,%s,Time,%d,Tray_Count,%02d,CM_Count,%04d,Tack,%0.7lf,%d,%d",
		gLot.sLotID[nLPNo], gLot.sStartTime[nLPNo], gLot.sEndTime[nLPNo], dwTime, gLot.nTrayCount[nLPNo], gLot.nCmCount[nLPNo], gLot.dTackTime, gLot.nGoodCount[nLPNo], gLot.nNgCount[nLPNo]);
	//g_objLogFile.Save_JobListLog(m_strLog);
	g_objLogFile.Save_HandlerLog(m_strLog);

	double dOne = (double)(dwTime - m_dwFirstLoad - m_dwLastUnLoad) / (gLot.nCmCount[nLPNo] - 1);
	m_strLog.Format("LotID(%s), Load(%0.3lf), Unload(%0.3lf), Takt as 600 EA Run(%0.6lf)", gLot.sLotID[nLPNo], (double)m_dwFirstLoad / 1000, (double)m_dwLastUnLoad / 1000, (dOne * 599 + m_dwFirstLoad + m_dwLastUnLoad) / 600000.0);
	//g_objLogFile.Save_JobListLog(m_strLog);
	g_objLogFile.Save_HandlerLog(m_strLog);

	
	m_strLog.Format("%s,%s,%s,%d,%02d,%04d,%0.7lf,%d,%d,%d,%d",
		gLot.sLotID[nLPNo], gLot.sStartTime[nLPNo], gLot.sEndTime[nLPNo], dwTime, gLot.nTrayCount[nLPNo], gLot.nCmCount[nLPNo], gLot.dTackTime, gLot.nGoodCount[nLPNo], gLot.nNgCount[nLPNo], gLot.nSNgCount[nLPNo][1], gLot.nSNgCount[nLPNo][2]);
	g_objLogFile.Save_JobListLog(m_strLog, TRUE);

	g_objLogFile.Save_Print(9, 0, 0, 0, 0, 0);	//Lot_End(검사결과 합계정보 저장)

	g_dlgWork.PostMessage(UM_LOT_START_END, (WPARAM)2, nPortNo);	// LotEnd

	// UPH & 생산량
	//if (gLot.sLotID.GetLength() < 12) return;		// Lot ID

	gUph.dTaktTime = gLot.dTackTime;

	if (time.wHour >= 7 && time.wHour < 19) gUph.nCmCount[0] += gLot.nCmCount[nLPNo];
	else gUph.nCmCount[1] += gLot.nCmCount[nLPNo];

	int i = (int)time.wHour;
	int j = gUph.nLotCount[i];
	if (j > 49) return;		// 1시간에 LOT 수량 MAX 50개

	gUph.dTakt[i][j] = gLot.dTackTime;
	gUph.nLotCount[i] = j + 1;

	int nSum = gLot.nGoodCount[nLPNo] + gLot.nNgCount[nLPNo];
	double dRate = (nSum == 0 ? 0.0 : gLot.nGoodCount[nLPNo] * 100.0 / nSum);
	// Lot Result [날짜,Lot Start,Lot End,Term,호기,Lot ID,투입수,양품수,불량수,불량률,HREN(H),HREP(P),Spider(S),BA(B),Chip(C),MES(M)];
	CString strMsg, strStart, strEnd, strTerm, strEquip;

	strStart.Format("%s:%s:%s", gLot.sStartTime[nLPNo].Mid(9, 2), gLot.sStartTime[nLPNo].Mid(11, 2), gLot.sStartTime[nLPNo].Mid(13, 2));
	strEnd.Format("%s:%s:%s", gLot.sEndTime[nLPNo].Mid(9, 2), gLot.sEndTime[nLPNo].Mid(11, 2), gLot.sEndTime[nLPNo].Mid(13, 2));
	int nSec = (int)(gLot.dwLotEnd[nLPNo] - gLot.dwLotStart[nLPNo]) / 1000;
	int nH = nSec / 60 / 60;
	int nM = (nSec - nH * 60 * 60) / 60;
	int nS = (nSec - nH * 60 * 60) % 60;
	strTerm.Format("%02d:%02d:%02d", nH, nM, nS);
	strEquip = m_pEquipData->sEquipName.Right(2);
	double dNgRate = (nSum == 0 ? 0.0 : gLot.nNgCount[nLPNo] * 100.0 / nSum);

	strMsg.Format("%s,%s,%s,%s,%s,%d,%d,%d,%0.2lf%%,%d,%d,%d",
		strStart, strEnd, strTerm, strEquip, gLot.sLotID[nLPNo], nSum, gLot.nGoodCount[nLPNo], gLot.nNgCount[nLPNo], dNgRate,
		gLot.nSNgCount[nLPNo][1], gLot.nSNgCount[nLPNo][2], gLot.nSNgCount[nLPNo][0]);

	g_objLogFile.Save_LotResult(nLPNo, strMsg);

	//Time, Lot_ID, Total, Good, NG, RosRequest, RosGood, RosNg, RosRepair, Filtering1, Filtering2, RateAvi, Final, Tack
	double dFiltering1 = (gLot.nRosRequest[nLPNo] == 0 ? 0.0 : (gLot.nRosGood[nLPNo] * 100.0) / gLot.nRosRequest[nLPNo]);
	double dFiltering2 = (nSum == 0 ? 0.0 : (gLot.nRosGood[nLPNo] * 100.0) / nSum);
	double dRateAvi = (nSum == 0 ? 0.0 : ((gLot.nGoodCount[nLPNo] - gLot.nRosGood[nLPNo]) * 100.0) / nSum);

	strMsg.Format("\t%s \t%d \t%d \t%d \t%d \t%d \t%d \t%d \t%d \t%0.2lf \t%0.2lf \t%0.2lf \t%0.2lf \t%0.3lf",
		gLot.sLotID[nLPNo], nSum, gLot.nGoodCount[nLPNo], gLot.nNgCount[nLPNo], gLot.nRosRequest[nLPNo], gLot.nRosGood[nLPNo], gLot.nRosNg[nLPNo], gLot.nRosRepair[nLPNo], gLot.nRosTimeOut[nLPNo],
		dFiltering1, dFiltering2, dRateAvi, dRate, gLot.dTackTime);
	g_objLogFile.Save_DailyLot(strMsg);

	CCMI8000Dlg *pMainDlg = (CCMI8000Dlg*)AfxGetApp()->GetMainWnd();
	pMainDlg->Set_LotStateTime();

	strStart.Format("%s-%s-%s %s:%s:%s", gLot.sStartTime[nLPNo].Mid(0, 4), gLot.sStartTime[nLPNo].Mid(4, 2), gLot.sStartTime[nLPNo].Mid(6, 2), gLot.sStartTime[nLPNo].Mid(9, 2), gLot.sStartTime[nLPNo].Mid(11, 2), gLot.sStartTime[nLPNo].Mid(13, 2));
	strEnd.Format("%s-%s-%s %s:%s:%s", gLot.sEndTime[nLPNo].Mid(0, 4), gLot.sEndTime[nLPNo].Mid(4, 2), gLot.sEndTime[nLPNo].Mid(6, 2), gLot.sEndTime[nLPNo].Mid(9, 2), gLot.sEndTime[nLPNo].Mid(11, 2), gLot.sEndTime[nLPNo].Mid(13, 2));
	strMsg.Format("%s,%s,%s,%0.3lf,%d,%0.3lf,%0.3lf,%0.3lf,%d,%0.2lf,%0.3lf,%0.3lf,%0.2lf",
		gLot.sLotID[nLPNo], strStart, strEnd, gLot.dTackTime, nSum, (double)(gLot.dwRunTime) / 1000,
		(double)(gLot.dwStopTime) / 1000, (double)(gLot.dwErrorTime) / 1000, gLot.nErrorCount, dRate, 3600 / gLot.dTackTime ,(double)(gLot.dwRunTime + gLot.dwErrorTime + gLot.dwStopTime) / gLot.nErrorCount);
	g_objLogFile.Save_OperatingRatio(strMsg);
	gLot.dwRunTime = gLot.dwErrorTime = gLot.dwStopTime = 0;

	g_objLogFile.Save_AverageCycle(nLPNo);
	g_objLogFile.Save_LotLog(nPortNo);	//gjcs

	// 해당포트 판정 완료, 결과 초기화
	g_objDispatcher.PortReset(nLPNo);	// 2020.10.28+
	memset(gData.byInspectDone[nLPNo], 0x00, sizeof(BYTE) * 30 * 40);
	memset(gData.nInspectInfo[nLPNo], 0x00, sizeof(int) * 30 * 40);
	memset(gData.bRosDone[nLPNo], 0x00, sizeof(BOOL) * 30 * 40);
	gData.nPNoNgTray = gData.nPNoGoodTray = 0;
	gData.nGoodTrayCount = 0;

	gLot.nGoodCount[nLPNo] = gLot.nNgCount[nLPNo] = 0;
	gLot.nSNgCount[nLPNo][1] = gLot.nSNgCount[nLPNo][2] = 0; 
	gLot.nRosRequest[nLPNo] = gLot.nRosGood[nLPNo] = gLot.nRosNg[nLPNo] = gLot.nRosRepair[nLPNo] = gLot.nRosTimeOut[nLPNo] = 0;

	gData.sLotID[nLPNo] = "";
	gData.nCmUseCount[nLPNo] = 0;
	gData.nTrayUseCount[nLPNo] = 0;	

	gData.bLoadLampOn[nLPNo] = TRUE;

// 	if (nLPNo == 0) {
// 		g_dlgWork.m_stcLotIdR.SetWindowText("");
// 		g_dlgWork.m_stcCmCountR.SetWindowText("0");
// 		g_dlgWork.m_stcTrayCountR.SetWindowText("0");
// 	} else {
// 		g_dlgWork.m_stcLotIdS.SetWindowText("");
// 		g_dlgWork.m_stcCmCountS.SetWindowText("0");
// 		g_dlgWork.m_stcTrayCountS.SetWindowText("0");
// 	}
// 	g_dlgWork.Enable_UserInput(nPortNo, TRUE);

	g_dlgWork.PostMessage(UM_LOT_INFO_CLEAR, nLPNo, NULL);
	g_dlgWork.PostMessage(UM_UPDATE_UPH, NULL, NULL);

//	Beep_Post(1000);
}

void CSequenceMain::Beep_Post(int nState, int nTime)
{
	if (nTime < 500) nTime = 500;

	if (nState == 0) {	// Lot End
		m_pDY14->oBuzzerBit0 = m_pEquipData->bBuzzer[0][0];
		m_pDY14->oBuzzerBit1 = m_pEquipData->bBuzzer[0][1];
		m_pDY14->oBuzzerBit2 = m_pEquipData->bBuzzer[0][2];
		m_pDY14->oBuzzerBit3 = m_pEquipData->bBuzzer[0][3];
		m_pDY14->oBuzzerBit4 = m_pEquipData->bBuzzer[0][4];

	} else if (nState == 1) {	// NG Full
		m_pDY14->oBuzzerBit0 = TRUE;	//m_pEquipData->bBuzzer[1][0];
		m_pDY14->oBuzzerBit1 = FALSE;	//m_pEquipData->bBuzzer[1][1];
		m_pDY14->oBuzzerBit2 = FALSE;	//m_pEquipData->bBuzzer[1][2];
		m_pDY14->oBuzzerBit3 = FALSE;	//m_pEquipData->bBuzzer[1][3];
		m_pDY14->oBuzzerBit4 = FALSE;	//m_pEquipData->bBuzzer[1][4];
	}
	g_objAJinAXL.Write_Output(14);
	theApp.uSleep(nTime);
	m_pDY14->oBuzzerBit0 = FALSE;
	m_pDY14->oBuzzerBit1 = FALSE;
	m_pDY14->oBuzzerBit2 = FALSE;
	m_pDY14->oBuzzerBit3 = FALSE;
	m_pDY14->oBuzzerBit4 = FALSE;
	g_objAJinAXL.Write_Output(14);
}

///////////////////////////////////////////////////////////////////////////////
// 1. (Error : 3100)
BOOL CSequenceMain::LoadTray_Run()
{
	static int nLtWorkPort = 0;		// 작업중인 Port (1 ~ 2)

	// LJH 1번 투입시 2번에 Tray 있으면 가져가야한다...
	// Load Tray Lot 투입시 도어락, 버튼 확인하여 Tray Loading 작업 못하게 막아준다.
	if (((m_bLotLoadEnable[0] == TRUE && (m_pEquipData->bUseDoorLock && m_pDX15->iDoor02Unlock) && (nLtWorkPort == 1 && m_nLoadTrayCase > 3 && m_nLoadTrayCase < 10)) ||
		(m_bLotLoadEnable[1] == TRUE && (m_pEquipData->bUseDoorLock && m_pDX15->iDoor03Unlock) && (nLtWorkPort == 2 && m_nLoadTrayCase > 3 && m_nLoadTrayCase < 10)))) return TRUE;
//		&& (m_nLoadTrayCase > 3 && m_nLoadTrayCase < 10)) return TRUE;

	switch (m_nLoadTrayCase) {
 	case 0:		// Start 시 1로 진행
		nLtWorkPort = 0;
 		return TRUE;
 
	case 1:		// Load Tray, Port 확인.
		if (m_bUnloadLotEnd || gData.bCycleStop) {
			nLtWorkPort = 0;
			m_nLoadTrayCase = 0;	// 사이클스탑이면 종료.

		} else if ((m_pDX00->iLoadPort1Bottom || m_pDX00->iLoadPort2Bottom) && !gData.bCycleStop) {
			// 맨처음 시작할때 2개 모두 적재 되어있으면 1번부터 가져간다.
			// 작업할 포트에 트레이 유무와 입력수량확인.
			if ((nLtWorkPort == 0 || nLtWorkPort == 2) && m_pDX00->iLoadPort1Bottom && (gData.nCmUseCount[0] > 0)) nLtWorkPort = 1;
			else if ((nLtWorkPort == 0 || nLtWorkPort == 1) && m_pDX00->iLoadPort2Bottom && (gData.nCmUseCount[1] > 0)) nLtWorkPort = 2;

			if (!Check_LoadTrayLoading(nLtWorkPort)) return TRUE;	// 같은 Port Lot을 동시에 검사하지 않는다.
			m_nLoadTrayCase++; m_tLoadTrayLoop.Set_LoopTime(5000);

		}
		break;
	case 2:		// Tray X Move to Load Position
		if (gData.bCycleStop) {
			nLtWorkPort = 0;
			m_nLoadTrayCase = 0;	// 사이클스탑이면 종료.
		}
		if (gData.nLoadTrayCount[nLtWorkPort-1] < gData.nTrayUseCount[nLtWorkPort-1] && !gData.bCycleStop) {
			if ((nLtWorkPort == 1 && !m_pDX00->iLoadPort1Bottom) || (nLtWorkPort == 2 && !m_pDX00->iLoadPort2Bottom)) break;

			if (nLtWorkPort == 1 && !g_objCommon.Check_Position(AX_LOAD_STAGE_X, 0)) g_objCommon.Move_Position(AX_LOAD_STAGE_X, 0);	// Port1
			if (nLtWorkPort == 2 && !g_objCommon.Check_Position(AX_LOAD_STAGE_X, 1)) g_objCommon.Move_Position(AX_LOAD_STAGE_X, 1);	// Port2
			m_nLoadTrayCase++; m_tLoadTrayLoop.Set_LoopTime(5000);
		} 
		break;
	case 3:		// Check Position, Lot Start
		if ((nLtWorkPort == 1 && g_objCommon.Check_Position(AX_LOAD_STAGE_X, 0)) || 
			(nLtWorkPort == 2 && g_objCommon.Check_Position(AX_LOAD_STAGE_X, 1))) {
			if (gData.nLoadTrayCount[nLtWorkPort-1] == 0) {
// 				gData.nLPNo = nLtWorkPort;
//				g_dlgWork.Enable_UserInput(nLtWorkPort, FALSE);
//				Job_LotStart(nLtWorkPort);
				if (g_objMES.m_bMESUse && !gData.bMesFirstLot && !gData.bMesRegistered[nLtWorkPort-1]) {
					// 연속 랏 관련하여 MES Lot Start 부분 수정.
					gData.bMesContinueLot = TRUE;	// 연속랏 중이다.
					g_objMES.m_bStart = TRUE;
					g_objMES.m_bMesErr = FALSE;
					if (g_objMES.m_nMESSequence == 3)	g_objMES.m_nMESSequence = 0;
					g_dlgWork.Get_LotInfo(nLtWorkPort);	// 입력된 Lot 정보를 다시 얻는다.
					g_objMES.Set_LotStart(gData.sLotID[nLtWorkPort-1], gData.nCmUseCount[nLtWorkPort-1], gData.sOperID, nLtWorkPort);	//2020.9.16+
				} else if (g_objMES.m_bMESUse && gData.bMesFirstLot) {
					gData.bMesFirstLot = FALSE;
				}
			}
			m_dwLoadTray = GetTickCount();
			m_nLoadTrayCase++; m_tLoadTrayLoop.Set_LoopTime(30000);
		} 
		break;
	case 4:		// Tray Z Move to Support Up Position (Load)
		if (g_objMES.m_bMESUse==FALSE || g_objMES.m_nMESSequence==3) {
			if (g_objCommon.Get_LoadTrayMasterSlaveOut() && !m_pDX01->iLoadStageExist) {
				m_tLoadTrayLoop.Takt_Start();
				g_dlgWork.Enable_UserInput(nLtWorkPort, FALSE);
				if (gData.nLoadTrayCount[nLtWorkPort-1] == 0) { Job_LotStart(nLtWorkPort); gData.bMesRegistered[nLtWorkPort-1] = TRUE; }
				g_objCommon.Move_Position(AX_LOAD_STAGE_Z, 1);	// Support Up
				m_nLoadTrayCase++; m_tLoadTrayLoop.Set_LoopTime(10000);
			}

		} else if (g_objMES.m_bMESUse && g_objMES.m_bMesErr == TRUE) {
			if (g_objMES.m_nMESSequence == 2) { m_nLoadTrayCase = 3; m_tLoadTrayLoop.Set_LoopTime(10000); }
			return FALSE;
		}
		break;
	case 5:		// Port Support Out
		if (g_objCommon.Check_Position(AX_LOAD_STAGE_Z, 1)) {
			m_tLoadTrayLoop.Takt_Save(1, 1);
			m_tLoadTrayLoop.Takt_Start();
			g_objCommon.Set_LoadPortSupportOut(nLtWorkPort);
			m_nLoadTrayCase++; m_tLoadTrayLoop.Set_LoopTime(5000);
		}
		break;
	case 6:		// Tray Z Move to Support Down Position
		if (g_objCommon.Get_LoadPortSupportOut(nLtWorkPort)) {
			m_tLoadTrayLoop.Takt_Save(1, 2);
			m_tLoadTrayLoop.Takt_Start();
			g_objCommon.Move_Position(AX_LOAD_STAGE_Z, 2);	// Support Down
			m_nLoadTrayCase++; m_tLoadTrayLoop.Set_LoopTime(10000);
		}
		break;
	case 7:		// Port Support In
		if (g_objCommon.Check_Position(AX_LOAD_STAGE_Z, 2)) {
			m_tLoadTrayLoop.Takt_Save(1, 3);
			m_tLoadTrayLoop.Takt_Start();
			g_objCommon.Set_LoadPortSupportIn(nLtWorkPort);
			m_nLoadTrayCase++; m_tLoadTrayLoop.Set_LoopTime(5000);
		}
		break;
 	case 8:		// Tray Z Move to Ready Down Position
		if (g_objCommon.Get_LoadPortSupportIn(nLtWorkPort)) {
			m_tLoadTrayLoop.Takt_Save(1, 4);
			m_tLoadTrayLoop.Takt_Start();
			g_objCommon.Move_Position(AX_LOAD_STAGE_Z, 0);
			m_nLoadTrayCase++; m_tLoadTrayLoop.Set_LoopTime(10000);
		}
		break;
	case 9:		// Tray Master In
		if (g_objCommon.Check_Position(AX_LOAD_STAGE_Z, 0)) {
			m_tLoadTrayLoop.Takt_Save(1, 5);
			m_tLoadTrayLoop.Takt_Start();
			m_pDY01->oLoadStageMasterIn = TRUE;
			g_objAJinAXL.Write_Output(1);
			m_nLoadTrayCase++; m_tLoadTrayLoop.Set_LoopTime(5000);
		}
		break;
	case 10:	// Tray Slave In
		if (m_pDX01->iLoadStageMasterIn && !m_pDX01->iLoadStageMasterOut) {
			m_pDY01->oLoadStageSlaveIn = TRUE;
			g_objAJinAXL.Write_Output(1);
			m_nLoadTrayCase++; m_tLoadTrayLoop.Set_LoopTime(5000);
		}
		break;
	case 11:	// Check Tray Master/Slave In
		if (g_objCommon.Get_LoadTrayMasterSlaveIn() && m_pDX01->iLoadStageExist) {
			m_tLoadTrayLoop.Takt_Save(1, 6);
			m_nLoadTrayCase = 15; m_tLoadTrayLoop.Set_LoopTime(5000);
		}
		break;

	case 15:	// Tray X Move to Unload Position
		if (g_objCommon.Check_Position(AX_LOAD_STAGE_Z, 0)) {
			m_tLoadTrayLoop.Takt_Start();
			g_objCommon.Move_Position(AX_LOAD_STAGE_X, 2);	// Unload
			m_nLoadTrayCase++; m_tLoadTrayLoop.Set_LoopTime(20000);
		}
		break;
	case 16:	// Tray Z Move to Unload Up Position
		if (g_objCommon.Check_Position(AX_LOAD_STAGE_X, 2)) {
			m_tLoadTrayLoop.Takt_Save(1, 7);
			m_tLoadTrayLoop.Takt_Start();
			g_objCommon.Move_Position(AX_LOAD_STAGE_Z, 3);	// Unload Up
			m_nLoadTrayCase++; m_tLoadTrayLoop.Set_LoopTime(10000);
		}
		break;
	case 17:	// Load Tray Count 증가 (Start)
		if (g_objCommon.Check_Position(AX_LOAD_STAGE_Z, 3) && m_pDX01->iLoadStageExist) {
			m_tLoadTrayLoop.Takt_Save(1, 8);
			gData.nLoadTrayCount[nLtWorkPort-1]++;
			if (gData.nLoadTrayCount[nLtWorkPort-1] == 1) {
				gData.nLPNo = nLtWorkPort;	// 첫번째 트레이 로딩할때
				if (gData.bVisionVerErr) {
					Job_LotStart(nLtWorkPort);
					gData.bVisionVerErr = FALSE;
				}
				m_nLoadTrayCase++; m_tLoadTrayLoop.Set_LoopTime(30000);	// 10초 (Wait for Vision Ready)
			} else m_nLoadTrayCase = 20;
		}
		break;
	case 18:	// Check Lot Ready
		if (g_objInspector.Check_LotReady()) {
			m_nLoadTrayCase = 20; m_tLoadTrayLoop.Set_LoopTime(5000);
		}
		break;

	case 20:	// Tray X Unload Position, Tray Pick Up Waiting
		return TRUE;

	// return
	case 21:	// Tray Z Move to Ready Down Position
		if (g_objCommon.Get_LoadTrayMasterSlaveOut()) {	// 작업이 끝나면 Out 상태이다.
			if (!g_objCommon.Check_Position(AX_LOAD_STAGE_X, 2)) break;	// 인터락
			m_tLoadTrayLoop.Takt_Start();
			g_objCommon.Move_Position(AX_LOAD_STAGE_Z, 0);	// Ready Down
			m_nLoadTrayCase++; m_tLoadTrayLoop.Set_LoopTime(10000);
		}
		break;
	case 22:	// Port Check
		if (g_objCommon.Check_Position(AX_LOAD_STAGE_Z, 0)) {
			m_tLoadTrayLoop.Takt_Save(1, 9);
			m_nLoadTrayCase++; m_tLoadTrayLoop.Set_LoopTime(20000);
		}
		break;
	case 23:	// Tray X Move to Load Position
		if (g_objCommon.Check_Position(AX_LOAD_STAGE_Z, 0)) {
			m_tLoadTrayLoop.Takt_Start();
			if (nLtWorkPort == 1) g_objCommon.Move_Position(AX_LOAD_STAGE_X, 0);	// Port1
			if (nLtWorkPort == 2) g_objCommon.Move_Position(AX_LOAD_STAGE_X, 1);	// Port2
			m_nLoadTrayCase++; m_tLoadTrayLoop.Set_LoopTime(10000);
		}
		break;
	case 24:	// Check End or Go
		if ((nLtWorkPort == 1 && g_objCommon.Check_Position(AX_LOAD_STAGE_X, 0)) || 
			(nLtWorkPort == 2 && g_objCommon.Check_Position(AX_LOAD_STAGE_X, 1))) {
			m_tLoadTrayLoop.Takt_Save(1, 10);

			if (gData.nLoadTrayCount[nLtWorkPort-1] >= gData.nTrayUseCount[nLtWorkPort-1]) {
				m_bLotLoadEnable[nLtWorkPort-1] = TRUE;	// Port에서 트레이 로드 완료

				if (nLtWorkPort == 1) { 
					m_pDY00->oLoadPort1SlideLock = FALSE; m_pDY00->oLoadPort1SlideUnlock = TRUE; 
					//m_pDY15->oDoor02Unlock = TRUE; 
					nLtWorkPort = 2;

				} else if (nLtWorkPort == 2) {
					m_pDY00->oLoadPort2SlideLock = FALSE; m_pDY00->oLoadPort2SlideUnlock = TRUE; 
					//m_pDY15->oDoor03Unlock = TRUE; 
					nLtWorkPort = 1;
				}
				g_objAJinAXL.Write_Output(0);
				g_objAJinAXL.Write_Output(14);
				gData.nLoadTrayCount[nLtWorkPort-1] = 0;
				m_nLoadTrayCase = 30; m_tLoadTrayLoop.Set_LoopTime(10000);
			
			} else {
				m_nLoadTrayCase = 2; m_tLoadTrayLoop.Set_LoopTime(10000);
			}		

			m_strLog.Format("LoadTray, %d", GetTickCount() - m_dwLoadTray);
			g_objLogFile.Save_TestLog(m_strLog);
		}
		break;

	case 30:	// Wait, Port Select
		if (!Check_LoadTrayLoading(nLtWorkPort)) return TRUE;	// 같은 Port에서 연속으로 2Lot은 검사할 수 없다.

		// 원래 순서대로 트레이가 적재 되어있을때
		// 2번 차례인데 1번에만 트레이가 있을때는 다시 1번에서 작업하도록 한다.
		if ((nLtWorkPort == 1 && m_pDX00->iLoadPort1Bottom && m_bLotLoadEnable[0] == FALSE) ||
			(nLtWorkPort == 2 && m_pDX00->iLoadPort2Bottom && m_bLotLoadEnable[1] == FALSE)) {

			g_dlgWork.Get_LotInfo(nLtWorkPort);
			m_nLoadTrayCase = 2; m_tLoadTrayLoop.Set_LoopTime(10000);

			m_strLog.Format("LoadTray case 30,No,%d,PNo,%d,LoadEnable1,%d,LoadEnable2,%d,1Bottom,%d,2Bottom,%d", 0, nLtWorkPort, m_bLotLoadEnable[0], m_bLotLoadEnable[1], m_pDX00->iLoadPort1Bottom, m_pDX00->iLoadPort2Bottom);
			g_objLogFile.Save_HandlerLog(m_strLog);

		} else if (nLtWorkPort == 1 && !m_pDX00->iLoadPort1Bottom &&
					m_pDX00->iLoadPort2Bottom && m_bLotLoadEnable[1] == FALSE) {

			g_dlgWork.Get_LotInfo(2);
			if (gData.sLotID[1].GetLength() > 0 && gData.nCmUseCount[1] > 0) {
				nLtWorkPort = 2; g_objCommon.Move_Position(AX_LOAD_STAGE_X, 1);	// Port2
				m_nLoadTrayCase = 2; m_tLoadTrayLoop.Set_LoopTime(10000);

				m_strLog.Format("LoadTray case 30,No,%d,PNo,%d,LoadEnable1,%d,LoadEnable2,%d,1Bottom,%d,2Bottom,%d", 1, nLtWorkPort, m_bLotLoadEnable[0], m_bLotLoadEnable[1], m_pDX00->iLoadPort1Bottom, m_pDX00->iLoadPort2Bottom);
				g_objLogFile.Save_HandlerLog(m_strLog);
			}
		} else if (nLtWorkPort == 2 && !m_pDX00->iLoadPort2Bottom &&
					m_pDX00->iLoadPort1Bottom && m_bLotLoadEnable[0] == FALSE) {

			g_dlgWork.Get_LotInfo(1);
			if (gData.sLotID[0].GetLength() > 0 && gData.nCmUseCount[0] > 0) {
				nLtWorkPort = 1; g_objCommon.Move_Position(AX_LOAD_STAGE_X, 0);	// Port1
				m_nLoadTrayCase = 2; m_tLoadTrayLoop.Set_LoopTime(10000);

				m_strLog.Format("LoadTray case 30,No,%d,PNo,%d,LoadEnable1,%d,LoadEnable2,%d,1Bottom,%d,2Bottom,%d", 2, nLtWorkPort, m_bLotLoadEnable[0], m_bLotLoadEnable[1], m_pDX00->iLoadPort1Bottom, m_pDX00->iLoadPort2Bottom);
				g_objLogFile.Save_HandlerLog(m_strLog);
			}
		} else {
			if (m_bUnloadLotEnd) {
				m_nLoadTrayCase = 0; m_tLoadTrayLoop.Set_LoopTime(10000);	// 작업 종료
			}
		}		
		return TRUE;
	}

	// 1. (Error : 3100)
	if (m_tLoadTrayLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(3100 + m_nLoadTrayCase);
		return FALSE;
 	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 2. (Error : 3200)
BOOL CSequenceMain::LoadPicker_Run()
{
	switch (m_nLoadPickCase) {
	case 0:		// Wait for Tray Ready
		if (m_nLoadTrayCase == 20) {
			m_nLoadPickCase++; m_tLoadPickLoop.Set_LoopTime(5000);
		}
		return TRUE;

	case 1:		// Picker Z Cylinder Down
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_Y, 0)) {
			if (!m_pDX01->iLoadPickerExist) {
				if (m_nLoadTrayCase != 20) return TRUE;		// Wait for Tray Loading
				m_dwTrayPick = GetTickCount();
				m_tLoadPickLoop.Takt_Start();
			
				g_objCommon.Set_LoadPickerDown();
				m_nLoadPickCase++; m_tLoadPickLoop.Set_LoopTime(10000);
			}
		}
		break;
	case 2:		// Load Tray Slave Out
		if (g_objCommon.Get_LoadPickerDown()) {
			m_tLoadPickLoop.Takt_Save(2, 1);
			m_tLoadPickLoop.Takt_Start();
			m_pDY01->oLoadStageSlaveIn = FALSE;
			g_objAJinAXL.Write_Output(1);
			m_nLoadPickCase++; m_tLoadPickLoop.Set_LoopTime(10000);
		}
		break;
	case 3:		// Load Tray Master Out
		if (!m_pDX01->iLoadStageSlaveIn && m_pDX01->iLoadStageSlaveOut) {
			m_pDY01->oLoadStageMasterIn = FALSE;
			g_objAJinAXL.Write_Output(1);
			m_nLoadPickCase++; m_tLoadPickLoop.Set_LoopTime(10000);
		}
		break;
	case 4:		// Picker Master In
		if (g_objCommon.Get_LoadTrayMasterSlaveOut()) {
			m_tLoadPickLoop.Takt_Save(2, 2);
			m_tLoadPickLoop.Takt_Start();
			m_pDY01->oLoadPickerMasterIn = TRUE; m_pDY01->oLoadPickerMasterOut = FALSE;
			g_objAJinAXL.Write_Output(1);
			m_nLoadPickCase++; m_tLoadPickLoop.Set_LoopTime(10000);
		}
		break;
	case 5:		// Picker Slave In
		if (m_pDX01->iLoadPickerMasterIn && !m_pDX01->iLoadPickerMasterOut) {
			m_pDY01->oLoadPickerSlaveIn = TRUE; m_pDY01->oLoadPickerSlaveOut = FALSE;
			g_objAJinAXL.Write_Output(1);
			m_nLoadPickCase++; m_tLoadPickLoop.Set_LoopTime(10000);
		}
		break;
	case 6:		// 정보전달. // Picker Z Cylinder Up
		if (g_objCommon.Get_LoadPickerMasterSlaveIn()) {
			m_tLoadPickLoop.Takt_Save(2, 3);
			m_tLoadPickLoop.Takt_Start();

			gData.nPNoTrayPick = gData.nLPNo;
			gData.nTNoTrayPick = gData.nLoadTrayCount[gData.nLPNo-1];

			g_objCommon.Set_LoadPickerUp();
			m_nLoadPickCase++; m_tLoadPickLoop.Set_LoopTime(10000);
		}
		break;
	case 7:		// Tray Check
		if (g_objCommon.Get_LoadPickerUp()) {
			if (m_pDX01->iLoadPickerExist) {
				m_tLoadPickLoop.Takt_Save(2, 4);
				if (m_nLoadTrayCase == 20) m_nLoadTrayCase = 21;
				m_nLoadPickCase = 10; m_tLoadPickLoop.Set_LoopTime(10000);
			}
		}
		break;

	case 10:	// Wait for Align Port Tray Empty
		// Port에 Tray가 있는데 검사 완료가 안되었다면 대기.	// Tray 검사 끝나고 다음 Tray 미리 올려놓을때 필요. 
		//if (m_nVisAngleCase == 0 && m_nAnglePortTrayCnt == 1 && !m_bAngleInspComplete) return TRUE;

		if (m_nVisAngleCase == 0 && !m_pDX02->iAnglePortExist && !m_bAngleInspComplete) {	// Angle Port가 작업안하고 트레이가 없다.
			if (g_objCommon.Check_Position(AX_ANGLE_UNIT_X, 0) && g_objCommon.Check_Position(AX_ANGLE_UNIT_Y, 0) && g_objCommon.Check_Position(AX_ANGLE_UNIT_R, 0)) {	// Inter lock 간섭 있다.
				m_nLoadPickCase++; m_tLoadPickLoop.Set_LoopTime(10000);
			}
		}
		return TRUE;

	case 11:	// Picker Y Move to Angle Port Unload Position
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_Y, 0)) {
			m_tLoadPickLoop.Takt_Start();
			g_objCommon.Move_Position(AX_LOAD_PICKER_Y, 1);
			m_nLoadPickCase++; m_tLoadPickLoop.Set_LoopTime(10000);
		}
		break;
	case 12:	// Angle Port 안전확인, Picker Down
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_Y, 1)) {
			if (g_objCommon.Get_AnglePortSupportIn() && g_objCommon.Get_AnglePortMasterSlaveOut()) {	// Port 확인
				m_tLoadPickLoop.Takt_Save(2, 5);
				m_tLoadPickLoop.Takt_Start();
				g_objCommon.Set_LoadPickerDown();
				m_nLoadPickCase++; m_tLoadPickLoop.Set_LoopTime(10000);
			}			
		}
		break;
	case 13:	// Picker Slave Out
		if (g_objCommon.Get_LoadPickerDown()) {
			m_tLoadPickLoop.Takt_Save(2, 6);
			m_tLoadPickLoop.Takt_Start();
			m_pDY01->oLoadPickerSlaveIn = FALSE; m_pDY01->oLoadPickerSlaveOut = TRUE;
			m_pDY01->oLoadPickerMasterIn = FALSE; m_pDY01->oLoadPickerMasterOut = TRUE;
			g_objAJinAXL.Write_Output(1);
			m_nLoadPickCase++; m_tLoadPickLoop.Set_LoopTime(10000);
		}
		break;
	case 14:	// Picker Master Out
		if (g_objCommon.Get_LoadPickerMasterSlaveOut()) {
			m_tLoadPickLoop.Takt_Save(2, 7);
			m_nLoadPickCase++; m_tLoadPickLoop.Set_LoopTime(10000);
		}
		break;
	case 15:	// 정보전달, Picker Up
		if (g_objCommon.Get_LoadPickerMasterSlaveOut()) {
			m_tLoadPickLoop.Takt_Start();

			gData.nPNoAnglePort[m_nAnglePortTrayCnt] = gData.nPNoTrayPick; gData.nPNoTrayPick = 0;
			gData.nTNoAnglePort[m_nAnglePortTrayCnt] = gData.nTNoTrayPick;
			m_nAnglePortTrayCnt++;

			g_objCommon.Set_LoadPickerUp();
			m_nLoadPickCase++; m_tLoadPickLoop.Set_LoopTime(10000);
		}
		break;
	case 16:	// Y Axis Move to Load Tray Position
		if (g_objCommon.Get_LoadPickerUp()) {
			m_tLoadPickLoop.Takt_Save(2, 8);
			m_tLoadPickLoop.Takt_Start();
			g_objCommon.Move_Position(AX_LOAD_PICKER_Y, 0);
// 			m_pDY02->oAnglePortMasterIn = TRUE; m_pDY02->oAnglePortMasterOut = FALSE; 
// 			m_pDY02->oAnglePortSlaveIn = TRUE; m_pDY02->oAnglePortSlaveOut = FALSE; 
// 			g_objAJinAXL.Write_Output(2);
			m_nLoadPickCase++; m_tLoadPickLoop.Set_LoopTime(10000);
		}
		break;
	case 17:	// Position Check
		if (g_objCommon.Check_Position(AX_LOAD_PICKER_Y, 0)) {
			m_tLoadPickLoop.Takt_Save(2, 9);
			m_nLoadPickCase = 0; m_tLoadPickLoop.Set_LoopTime(10000);

			m_strLog.Format("LoadTrayPick, %d", GetTickCount() - m_dwTrayPick);
			g_objLogFile.Save_TestLog(m_strLog);
		}
		break;
	}

	// 2. (Error : 3200)
	if (m_tLoadPickLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(3200 + m_nLoadPickCase);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 3. (Error : 3300)
BOOL CSequenceMain::VisionAngle_Run()
{
	static int		nAngleTrayXPos, nAngleTrayYPos = 0;
	static double	dAngleUnitX, dAngleUnitY, dAngleUnitR = 0.0;
	static BOOL		bAlignRetry = FALSE;
 
	switch (m_nVisAngleCase) {
	case 0:		// Tray 수량, 센서 감지 및 검사 유무 확인 후 진행
		if (m_nAnglePortTrayCnt == 1 && m_pDX02->iAnglePortExist && !m_bAngleInspComplete) {
			if (g_objCommon.Check_Position(AX_LOAD_PICKER_Y, 0)) {	// Inter lock 간섭이 있다.
				if (!m_pEquipData->bUseVisionAlign && !m_pEquipData->bUseInspectAngle) {	// Angle Skip
					Init_AnglePortTray(gData.nPNoAnglePort[0]);
					for (int i = 0; i < 5; i++) {
						for (int j = 0; j < 8; j++) {
							if (gData.InfoAnglePortTray[i][j] == 9) gData.InfoAnglePortTray[i][j] = 1;	//Scan Done
						}
					}
					m_nVisAngleCase = 21; m_tVisAngleLoop.Set_LoopTime(5000);
				} else {
					m_nVisAngleCase = 1; m_tVisAngleLoop.Set_LoopTime(5000);
				}
			}
		}
		return TRUE;

	case 1:		// Master In
		m_tVisAngleLoop.Takt_Start();
		m_dwVisAngle = GetTickCount();
// 		m_pDY02->oAnglePortMasterIn = TRUE; m_pDY02->oAnglePortMasterOut = FALSE; 
// 		g_objAJinAXL.Write_Output(2);
		m_nVisAngleCase++; m_tVisAngleLoop.Set_LoopTime(5000);
		break;
	case 2:		// Slave In & Align Z Move to Down Position 
//		if (m_pDX02->iAnglePortMasterIn && !m_pDX02->iAnglePortMasterOut) {
		if (!m_pDX02->iAnglePortMasterIn && m_pDX02->iAnglePortMasterOut) {
// 			m_pDY02->oAnglePortSlaveIn = TRUE; m_pDY02->oAnglePortSlaveOut = FALSE; 
// 			g_objAJinAXL.Write_Output(2);
			g_objCommon.Move_Position(AX_VISION_ALIGN_Z, 1);
			m_nVisAngleCase++; m_tVisAngleLoop.Set_LoopTime(5000);
		}
		break;
	case 3:		// Master/Slave Check 
//		if (g_objCommon.Get_AnglePortMasterSlaveIn()) {
		if (g_objCommon.Get_AnglePortMasterSlaveOut()) {
			m_tVisAngleLoop.Takt_Save(3, 1);
			m_tVisAngleLoop.Takt_Start();

			Init_AnglePortTray(gData.nPNoAnglePort[0]);
			//g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 2, NULL);
			gData.bAlignReply = FALSE;
			bAlignRetry = FALSE;

			if (m_pEquipData->bUseVisionAlign)	m_nVisAngleCase++;
			else								m_nVisAngleCase = 8;
			m_tVisAngleLoop.Set_LoopTime(5000);
		}
		break;
	case 4:		// Align Load Complete
		if (g_objCommon.Check_Position(AX_VISION_ALIGN_Z, 1)) {
			m_tVisAngleLoop.Takt_Save(3, 2);
			m_tVisAngleLoop.Takt_Start();
// 			g_objInspector.Set_LoadComplete(INSPECTOR_PC1, gLot.sLotID[gData.nPNoAnglePort[0]], "AL", gData.nPNoAnglePort[0], 0, 0, 0, 0, 0, 0, 0, 0);
			g_objInspector.Set_AlignRequest(INSPECTOR_PC1, gLot.sLotID[gData.nPNoAnglePort[0]-1], gData.nPNoAnglePort[0], gData.nTNoAnglePort[0], 1);	//gData.nLoadTrayCount[gData.nPNoAnglePort[0]-1], 1);
			m_nVisAngleCase++; m_tVisAngleLoop.Set_LoopTime(30000);
		}
		break;

	case 5:		// Wait for Vision Align Complete (30sec)
		if (!m_pEquipData->bUseVisionAlign) {
			m_nVisAngleCase = 8; m_tVisAngleLoop.Set_LoopTime(5000);
		}
		break;

// 	case 6:
// 		if (g_objAJinAXL.Is_Done(AX_VISION_ALIGN_Z)) {
// 			m_tVisAngleLoop.Takt_Start();
// 			g_objAJinAXL.Move_Absolute(AX_VISION_ALIGN_Z, m_dAlignZ);
// 			m_nVisAngleCase++; m_tVisAngleLoop.Set_LoopTime(5000);
// 		}
// 		break;
// 	case 7:	// Send Move Complete
// 		if (g_objAJinAXL.Is_Done(AX_VISION_ALIGN_Z)) {
// 			m_tVisAngleLoop.Takt_Save(3, 10);
// 			g_objInspector.Set_MoveComplete(INSPECTOR_PC1, "AL");
// 			m_nVisAngleCase = 5; m_tVisAngleLoop.Set_LoopTime(30000);	// 30초
// 		}
// 		break;

	case 8:		// Align Z Move to Ready Up Position
		if (g_objAJinAXL.Is_Done(AX_VISION_ALIGN_Z)) {
			if (!m_pEquipData->bUseVisionAlign) { if (!m_tVisAngleLoop.Waiting_Time(80)) break; }	// Dealy Time

			m_tVisAngleLoop.Takt_Save(3, 3);
			m_tVisAngleLoop.Takt_Start();
			g_objCommon.Move_Position(AX_VISION_ALIGN_Z, 0);
			m_nVisAngleCase = 10; m_tVisAngleLoop.Set_LoopTime(30000);
		}
		break;

	case 10:	// Angle X, Y, R Move to Inspection Position
		if (g_objCommon.Check_Position(AX_VISION_ALIGN_Z, 0)) {
			if (g_objCommon.Check_Position(AX_LOAD_PICKER_Y, 0)) {	// Inter lock
				m_tVisAngleLoop.Takt_Save(3, 4);
				m_tVisAngleLoop.Takt_Start();
// 				g_objCommon.Move_Position(AX_ANGLE_UNIT_X, 1);
// 				g_objCommon.Move_Position(AX_ANGLE_UNIT_Y, 1);
// 				g_objCommon.Move_Position(AX_ANGLE_UNIT_R, 1);
				g_objCommon.Move_Position(AX_VISION_ANGLE_Z, 0);
				m_nVisAngleCase++; m_tVisAngleLoop.Set_LoopTime(5000);
			}
		}
		break;
	case 11:	// Position Check & Align Inspect Complete Check
// 		if (g_objCommon.Check_Position(AX_ANGLE_UNIT_X, 1) && g_objCommon.Check_Position(AX_ANGLE_UNIT_Y, 1) &&
// 			g_objCommon.Check_Position(AX_ANGLE_UNIT_R, 1) && g_objCommon.Check_Position(AX_VISION_ANGLE_Z, 0)) {
		if (g_objCommon.Check_Position(AX_VISION_ANGLE_Z, 0)) {
			m_tVisAngleLoop.Takt_Save(3, 5);
			CString strLog;
			double dPos = m_pMoveData->dVisionAngleZ[0];
			double dEnc = g_objAJinAXL.Get_Position(AX_VISION_ANGLE_Z);
			strLog.Format("%0.3lf,%0.3lf", dPos, dEnc);
			g_objLogFile.Save_FocusAxisLog(AX_VISION_ANGLE_Z, strLog);

			m_nVisAngleCase++; m_tVisAngleLoop.Set_LoopTime(5000);
		}
		break;
	case 12:	// Move to Module Inspection Position 
		g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 1, NULL);
		if (Select_AngleScanPos(nAngleTrayXPos, nAngleTrayYPos)) {
			m_tVisAngleLoop.Takt_Start();

			int nIndex =  (nAngleTrayYPos - 1) * gData.nTrayX + nAngleTrayXPos;
			dAngleUnitY = m_pMoveData->dAngleUnitY[1] + (m_pEquipData->dTrayPitchY * (nAngleTrayYPos-1) );// + g_objInspector.Get_AlignY(nIndex);
			dAngleUnitX = m_pMoveData->dAngleUnitX[1] - (m_pEquipData->dTrayPitchX * (nAngleTrayXPos-1) );// + g_objInspector.Get_AlignX(nIndex);
			dAngleUnitR = m_pMoveData->dAngleUnitR[1];// + g_objInspector.Get_AlignQ(nIndex);
// 			dAngleUnitY = m_pMoveData->dAngleUnitY[1] - (m_pEquipData->dTrayPitchY * (nAngleTrayYPos-1) ) + m_dAngleY;
// 			dAngleUnitX = m_pMoveData->dAngleUnitX[1] - (m_pEquipData->dTrayPitchX * (nAngleTrayXPos-1) ) + m_dAngleX;
// 			dAngleUnitR = m_pMoveData->dAngleUnitR[1] + m_dAngleR;

			g_objAJinAXL.Move_Absolute(AX_ANGLE_UNIT_Y, dAngleUnitY);	
			g_objAJinAXL.Move_Absolute(AX_ANGLE_UNIT_X, dAngleUnitX);	
			g_objAJinAXL.Move_Absolute(AX_ANGLE_UNIT_R, dAngleUnitR);	

			m_nVisAngleCase++; m_tVisAngleLoop.Set_LoopTime(10000);
		} else {
			// Scan End
			m_nVisAngleCase = 20; m_tVisAngleLoop.Set_LoopTime(10000);
		}
		break;
	case 13:	// Set Load Complete
		if (g_objAJinAXL.Is_MoveDone(AX_ANGLE_UNIT_Y, dAngleUnitY) &&
			g_objAJinAXL.Is_MoveDone(AX_ANGLE_UNIT_X, dAngleUnitX) &&
			g_objAJinAXL.Is_MoveDone(AX_ANGLE_UNIT_R, dAngleUnitR)) {
			m_tVisAngleLoop.Takt_Save(3, 6);
			m_tVisAngleLoop.Takt_Start();
			if (!m_pEquipData->bUseInspectAngle) {
				if (gData.InfoAnglePortTray[nAngleTrayYPos-1][nAngleTrayXPos-1] == 9) gData.InfoAnglePortTray[nAngleTrayYPos-1][nAngleTrayXPos-1] = 1;	//Scan Done
				m_nVisAngleCase = 19; m_tVisAngleLoop.Set_LoopTime(30000);
			} else {
				int nCmNo = (nAngleTrayYPos-1) * gData.nTrayX +  nAngleTrayXPos;
				g_objInspector.Set_LoadComplete(INSPECTOR_PC1, "AG", gLot.sLotID[gData.nPNoAnglePort[0]-1], gData.nPNoAnglePort[0], gData.nTNoAnglePort[0],0, 0, 0, nCmNo, 0, 0, 0);
				m_nVisAngleCase = 15; m_tVisAngleLoop.Set_LoopTime(30000);
			}
		}
		break;

	case 15:	// Wait for Angle inspection(30sec)
		if (!m_pEquipData->bUseInspectAngle) {
			m_nVisAngleCase = 19; m_tVisAngleLoop.Set_LoopTime(5000);
		}
		break;
	case 16:
		if (g_objAJinAXL.Is_Done(AX_VISION_ANGLE_Z)) {
			m_tVisAngleLoop.Takt_Start();
			g_objAJinAXL.Move_Absolute(AX_VISION_ANGLE_Z, m_dAngleZ);
			m_nVisAngleCase++; m_tVisAngleLoop.Set_LoopTime(5000);
		}
		break;
	case 17:	// Send Move Complete
		if (g_objAJinAXL.Is_Done(AX_VISION_ANGLE_Z)) {
			m_tVisAngleLoop.Takt_Save(3, 10);
			CString strLog;
			double dEnc = g_objAJinAXL.Get_Position(AX_VISION_ANGLE_Z);
			strLog.Format("%0.3lf,%0.3lf", m_dAngleZ, dEnc);
			g_objLogFile.Save_FocusAxisLog(AX_VISION_ANGLE_Z, strLog);

			g_objInspector.Set_MoveComplete(INSPECTOR_PC1, "AG");
			m_nVisAngleCase = 15; m_tVisAngleLoop.Set_LoopTime(30000);	// 30초
		}
		break;

	case 19:	// Align Z Move to Ready Up Position
//		if (!m_pEquipData->bUseInspectAngle) { if (!m_tVisAngleLoop.Waiting_Time(50)) break; }	// Dealy Time
		m_tVisAngleLoop.Takt_Save(3, 7);
		m_nVisAngleCase = 12; m_tVisAngleLoop.Set_LoopTime(30000);
		break;

	case 20:	// Angle Unit Move to Ready Position
		if (g_objAJinAXL.Is_Done(AX_ANGLE_UNIT_Y) && g_objAJinAXL.Is_Done(AX_ANGLE_UNIT_X) && g_objAJinAXL.Is_Done(AX_ANGLE_UNIT_R)) {
			m_tVisAngleLoop.Takt_Start();
			g_objCommon.Move_Position(AX_ANGLE_UNIT_X, 0);
			g_objCommon.Move_Position(AX_ANGLE_UNIT_Y, 0);
			g_objCommon.Move_Position(AX_ANGLE_UNIT_R, 0);
			g_objCommon.Move_Position(AX_VISION_ANGLE_Z, 0);
			m_nVisAngleCase++; m_tVisAngleLoop.Set_LoopTime(5000);
		}
		break;

	case 21:	// Slave Out
		m_pDY02->oAnglePortSlaveIn = FALSE; m_pDY02->oAnglePortSlaveOut = TRUE; 
		g_objAJinAXL.Write_Output(2);
//		g_objCommon.Move_Position(AX_VISION_ALIGN_Z, 1);
		m_nVisAngleCase++; m_tVisAngleLoop.Set_LoopTime(5000);
		break;
	case 22:	// Master Out
		if (!m_pDX02->iAnglePortSlaveIn && m_pDX02->iAnglePortSlaveOut) {
			m_pDY02->oAnglePortMasterIn = FALSE; m_pDY02->oAnglePortMasterOut = TRUE; 
			g_objAJinAXL.Write_Output(2);
			m_nVisAngleCase++; m_tVisAngleLoop.Set_LoopTime(5000);
		}
		break;
	case 23:	// Master/Slave Check 
		if (g_objCommon.Get_AnglePortMasterSlaveOut()) {
			m_tVisAngleLoop.Takt_Save(3, 8);
			m_bAngleInspComplete = TRUE;	// 검사 완료.
			m_nVisAngleCase++; m_tVisAngleLoop.Set_LoopTime(5000);
		}
		break;
	case 24:	// Angle Unit Position Check
		if (g_objCommon.Check_Position(AX_ANGLE_UNIT_X, 0) && g_objCommon.Check_Position(AX_ANGLE_UNIT_Y, 0) &&
			g_objCommon.Check_Position(AX_ANGLE_UNIT_R, 0) && g_objCommon.Check_Position(AX_VISION_ANGLE_Z, 0)) {
				m_tVisAngleLoop.Takt_Save(3, 9);
				m_nVisAngleCase = 0; m_tVisAngleLoop.Set_LoopTime(5000);

				m_strLog.Format("VisionAngle, %d", GetTickCount() - m_dwVisAngle);
				g_objLogFile.Save_TestLog(m_strLog);
		}
		break;
	}

	// 3. (Error : 3300)
	if (m_tVisAngleLoop.Over_LoopTime()) {
		if (m_nVisAngleCase == 5) {
			if (!gData.bAlignReply && !bAlignRetry) { bAlignRetry = TRUE; m_nVisAngleCase = 4; m_tVisAngleLoop.Set_LoopTime(5000); return TRUE;}
		}
		if (m_nVisAngleCase == 15) {
			int nCmNo = (nAngleTrayYPos-1) * gData.nTrayX +  nAngleTrayXPos;
			Set_ScanError(3315, gData.nTNoAnglePort[0], 0, 0, 0, nCmNo);
		}
		g_objCommon.Show_Error(3300 + m_nVisAngleCase);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 4. (Error : 3400)
BOOL CSequenceMain::AngleTray1_Run()
{
	static double	dStage1Z = 0.0;

	switch (m_nAngleTray1Case) {
	case 0:		// Z Axis Move To Support Ready Position
		if (m_bUnloadLotEnd) {
			return TRUE;
		} else {
			// Load Position Check
			if (g_objCommon.Check_Position(AX_ANGLE_STAGE1_Y, 0) && !m_pDX02->iAngleStage1Exist &&
				!m_pDX02->iAngleStage1AlignIn && m_pDX02->iAngleStage1AlignOut)
			{	
				g_objCommon.Move_Position(AX_ANGLE_STAGE1_Z, 3);	// Support Ready Position
				m_nAngleTray1Case++; m_tAngleTray1Loop.Set_LoopTime(5000);
			}
		}
		break;

	case 1:		// 안전 확인. 검사 중이면 Z Axis Move to Support Wait Position
		if (m_nVisAngleCase > 8 && m_pDX02->iAnglePortExist) {	// 검사 진행 중이면
			if (m_pDX02->iAngleStage1Normal && !m_pDX02->iAngleStage1Rotate &&
				g_objCommon.Check_Position(AX_ANGLE_STAGE1_Y, 0) && g_objCommon.Check_Position(AX_ANGLE_STAGE1_Z, 3)) {
					g_objCommon.Move_Position(AX_ANGLE_STAGE1_Z, 5);
					m_nAngleTray1Case++; m_tAngleTray1Loop.Set_LoopTime(5000);
			}
			break;

		} else if ((m_nVisAngleCase == 0 || m_nVisAngleCase == 24) && m_pDX02->iAnglePortExist && m_bAngleInspComplete) { // 작업 끝났고 검사 완료
			if (m_pDX02->iAngleStage1Normal && !m_pDX02->iAngleStage1Rotate && g_objCommon.Get_AnglePortMasterSlaveOut() &&
				g_objCommon.Check_Position(AX_ANGLE_STAGE1_Y, 0) && g_objCommon.Check_Position(AX_ANGLE_STAGE1_Z, 3)) {
					m_tAngleTray1Loop.Takt_Start();
					m_dwAngleTray1 = GetTickCount();
					g_objCommon.Move_Position(AX_ANGLE_STAGE1_Z, 1);
					m_nAngleTray1Case = 3; m_tAngleTray1Loop.Set_LoopTime(5000);
			}
			break;

		}
		if (m_bUnloadLotEnd) {
			g_objCommon.Move_Position(AX_ANGLE_STAGE1_Z, 0);	// Ready Down Position
			m_nAngleTray1Case = 0; m_tAngleTray1Loop.Set_LoopTime(5000);
		}
		return TRUE;
	case 2:		// 안전 확인. Z Axis Move to Support Up Position
		if ((m_nVisAngleCase == 0 || m_nVisAngleCase == 24) && m_pDX02->iAnglePortExist && m_bAngleInspComplete) {	// 작업 끝났고 검사 완료했으면 진행.

			if (m_pDX02->iAngleStage1Normal && !m_pDX02->iAngleStage1Rotate && g_objCommon.Get_AnglePortMasterSlaveOut() &&
				g_objCommon.Check_Position(AX_ANGLE_STAGE1_Y, 0) && g_objCommon.Check_Position(AX_ANGLE_STAGE1_Z, 5)) {
				m_tAngleTray1Loop.Takt_Start();
				m_dwAngleTray1 = GetTickCount();
				g_objCommon.Move_Position(AX_ANGLE_STAGE1_Z, 1);
				m_nAngleTray1Case++; m_tAngleTray1Loop.Set_LoopTime(5000);
			}
			break;
		}
		if (m_bUnloadLotEnd) {
			g_objCommon.Move_Position(AX_ANGLE_STAGE1_Z, 0);	// Ready Down Position
			m_nAngleTray1Case = 0; m_tAngleTray1Loop.Set_LoopTime(5000);
		}
		return TRUE;
	case 3:		// Angle Port Support Out, Stage Align In
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE1_Z, 1)) {
			m_tAngleTray1Loop.Takt_Save(4, 1);
			m_tAngleTray1Loop.Takt_Start();
			g_objCommon.Set_AnglePortSupportOut();
			m_pDY02->oAngleStage1AlignIn = TRUE;
			g_objAJinAXL.Write_Output(2);
			m_nAngleTray1Case++; m_tAngleTray1Loop.Set_LoopTime(5000);
		}
		break;
	case 4:		// 정보전달, Z Axis Move to Ready Down Position
		if (g_objCommon.Get_AnglePortSupportOut() && m_pDX02->iAngleStage1Exist &&
			m_pDX02->iAngleStage1AlignIn && !m_pDX02->iAngleStage1AlignOut)
		{
			m_tAngleTray1Loop.Takt_Save(4, 2);
			m_tAngleTray1Loop.Takt_Start();

			memmove(gData.InfoAngleTray[0], gData.InfoAnglePortTray, sizeof(int) * 8 * 5);
			memset(gData.InfoAnglePortTray, 0x00, sizeof(int) * 8 * 5);
			gData.nPNoAngleTray[0] = gData.nPNoAnglePort[0]; gData.nPNoAnglePort[0] = 0;
			gData.nTNoAngleTray[0] = gData.nTNoAnglePort[0]; gData.nTNoAnglePort[0] = 0;
			m_nAnglePortTrayCnt--;
			//g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 2, 0);
			g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 1, 0);

			//g_objCommon.Move_Position(AX_ANGLE_STAGE1_Z, 0);
			m_bAngleInspComplete = FALSE;
			g_objCommon.Move_Position(AX_ANGLE_STAGE1_Z, 3);	// Support Ready Position
			m_nAngleTray1Case++; m_tAngleTray1Loop.Set_LoopTime(5000);
		}
		break;
	case 5:		// Angle Port Support In
		//if (g_objCommon.Check_Position(AX_ANGLE_STAGE1_Z, 0)) {
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE1_Z, 3)) {
			m_tAngleTray1Loop.Takt_Save(4, 3);
			m_tAngleTray1Loop.Takt_Start();
//			m_bAngleInspComplete = FALSE;
			g_objCommon.Set_AnglePortSupportIn();
			m_nAngleTray1Case++; m_tAngleTray1Loop.Set_LoopTime(5000);
		}
		break;
	case 6:		// Support Check
		if (g_objCommon.Get_AnglePortSupportIn()) {
			m_tAngleTray1Loop.Takt_Save(4, 4);			
			m_nAngleTray1Case++; m_tAngleTray1Loop.Set_LoopTime(5000);
		}
	case 7:		// 안전 확인.
		if (m_nAngleTray2Case >= 20) {	// Btm2 Picker CM Loading
			m_nAngleTray1Case++; m_tAngleTray1Loop.Set_LoopTime(5000);
		}
		return TRUE;
	case 8:		// Y Axis Move to Ready Position, R04A만 회전을 한다.
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE1_Z, 3)) {
			m_tAngleTray1Loop.Takt_Start();
			if (gData.bR04A || gData.bR13A) {
				m_pDY02->oAngleStage1Rotate = TRUE;
				g_objAJinAXL.Write_Output(2);
			}
			g_objCommon.Move_Position(AX_ANGLE_STAGE1_Y, 1); // Ready Position
			m_nAngleTray1Case++; m_tAngleTray1Loop.Set_LoopTime(5000);
		}
		break;
	case 9:		// Z Axis Move to Pick Up Position, R04A Model Rotate
		if ((( (gData.bR04A || gData.bR13A) && !m_pDX02->iAngleStage1Normal &&  m_pDX02->iAngleStage1Rotate) || 
			 ((!gData.bR04A && !gData.bR13A) &&  m_pDX02->iAngleStage1Normal && !m_pDX02->iAngleStage1Rotate)) &&
			g_objCommon.Check_Position(AX_ANGLE_STAGE1_Y, 1)) {
			m_tAngleTray1Loop.Takt_Save(4, 5);
			m_tAngleTray1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_ANGLE_STAGE1_Z, 2);
			m_nAngleTray1Case++; m_tAngleTray1Loop.Set_LoopTime(5000);
		}
		break;
	case 10:
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE1_Z, 2)) {
			m_tAngleTray1Loop.Takt_Save(4, 6);
			m_nAngleTray1Case = 15; m_tAngleTray1Loop.Set_LoopTime(5000);
		}
		break;
	case 15:	// Wait for Stage2
		// Btm1 Picker CM Loading
		if (m_nAngleTray2Case > 23)	m_nAngleTray1Case++;
		return TRUE;

	case 16:	// Y Axis Move to Btm1 Picker L1 Position
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE1_Z, 2)) {
			m_tAngleTray1Loop.Takt_Start();
			if ((gData.bR04B || gData.bR13B_5 || gData.bR13A) && m_bBtm1ModulePick) {
				g_objCommon.Move_Position(AX_ANGLE_STAGE1_Y, 3); // Btm1 Picker L2 Position

			} else {
				g_objCommon.Move_Position(AX_ANGLE_STAGE1_Y, 2); // Btm1 Picker L1 Position
			}
			
			m_nAngleTray1Case++; m_tAngleTray1Loop.Set_LoopTime(5000);
		}
		break;
	case 17:	// Position Check
		if (((gData.bR04B || gData.bR13B_5 || gData.bR13A) && m_bBtm1ModulePick && g_objCommon.Check_Position(AX_ANGLE_STAGE1_Y, 3)) ||
			g_objCommon.Check_Position(AX_ANGLE_STAGE1_Y, 2)) {

			m_tAngleTray1Loop.Takt_Save(4, 7);
			Init_AngleTray(0);
			m_bBtm1ModulePick = FALSE;
			g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 2, 0);
			m_nAngleTray1Case = 20; m_tAngleTray1Loop.Set_LoopTime(5000);
		}
		break;

	case 20:	// Module Load Working
		return TRUE;

	case 21:	// 스테이지 20mm 다운 후 이동
		if (m_nAngleTray2Case <= 20) {
			dStage1Z = m_pMoveData->dAngleStage1Z[2] - 20.0;	// CM Pick Up Position - 5mm
			g_objAJinAXL.Move_Absolute(AX_ANGLE_STAGE1_Z, dStage1Z); 
			m_nAngleTray1Case++; m_tAngleTray1Loop.Set_LoopTime(5000);
		} else { return TRUE; }
		break;
	case 22:	// Down Check
		if (g_objAJinAXL.Is_MoveDone(AX_ANGLE_STAGE1_Z, dStage1Z)) {
			m_nAngleTray1Case++; m_tAngleTray1Loop.Set_LoopTime(5000);
		}
		break;

	case 23:	// Y Axis Empty Tray Unload Position
		if (m_nAngleTray2Case == 30) return TRUE;	// Empty Tray Unloading
		if (m_nAngleTray2Case > 52 || m_nAngleTray2Case < 21) {	// Inter lock
			m_tAngleTray1Loop.Takt_Start();
			if (gData.bR04A || gData.bR13A) {
				m_pDY02->oAngleStage1Rotate = FALSE;
				g_objAJinAXL.Write_Output(2);
			}
			g_objCommon.Move_Position(AX_ANGLE_STAGE1_Y, 4); // Unload Position
			m_nAngleTray1Case++; m_tAngleTray1Loop.Set_LoopTime(5000);
		}
		return TRUE;
	case 24:	// Position Check & ZAxis Move To Tray Pick Up Position
		if (m_pDX02->iAngleStage1Normal && !m_pDX02->iAngleStage1Rotate &&
			g_objCommon.Check_Position(AX_ANGLE_STAGE1_Y, 4)) {
			m_tAngleTray1Loop.Takt_Save(4, 8);
			m_tAngleTray1Loop.Takt_Start();

			g_objCommon.Move_Position(AX_ANGLE_STAGE1_Z, 4); // Tray Pick Up Position
			//m_nAngleTray1Case = 30; m_tAngleTray1Loop.Set_LoopTime(5000);
			m_nAngleTray1Case++; m_tAngleTray1Loop.Set_LoopTime(5000);
		}
		break;
	case 25:	// Position Check
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE1_Z, 4)) {
			m_tAngleTray1Loop.Takt_Save(4, 11);
			m_nAngleTray1Case = 30; m_tAngleTray1Loop.Set_LoopTime(5000);
		}
		break;
	case 30:	// Empty Tray Unload Working
		return TRUE;

	case 31:	// Z Axis Move to Ready Down Position
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE1_Y, 4) && g_objCommon.Check_Position(AX_ANGLE_STAGE1_Z, 4) && !m_pDX02->iAngleStage1Exist) {
			m_tAngleTray1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_ANGLE_STAGE1_Z, 2); // Picker Up Position 안전한 위치.
			m_nAngleTray1Case++; m_tAngleTray1Loop.Set_LoopTime(30000);
		}
		break;
	case 32:	//
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE1_Y, 4) && g_objCommon.Check_Position(AX_ANGLE_STAGE1_Z, 2)) {
			m_tAngleTray1Loop.Takt_Save(4, 6);
			m_strLog.Format("AngleStage1, %d", GetTickCount() - m_dwAngleTray1);
			g_objLogFile.Save_TestLog(m_strLog);
			m_nAngleTray1Case = 50; m_tAngleTray1Loop.Set_LoopTime(30000);
		}
		break;

	case 50:	// Wait for Loading
		if (m_nAngleTray2Case > 9 && m_nAngleTray2Case < 50) m_nAngleTray1Case++; 
		return TRUE;

	case 51:	// Y Axis Move to Load Position
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE1_Y, 4) && g_objCommon.Check_Position(AX_ANGLE_STAGE1_Z, 2)) {
			m_tAngleTray1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_ANGLE_STAGE1_Z, 0);
			m_nAngleTray1Case++; m_tAngleTray1Loop.Set_LoopTime(30000);
		}
		break;
	case 52:	// Check Down Position
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE1_Z, 0) && g_objCommon.Check_Position(AX_ANGLE_STAGE2_Z, 2)) {
			m_tAngleTray1Loop.Takt_Save(4, 9);
			g_objCommon.Move_Position(AX_ANGLE_STAGE1_Y, 0);
			m_nAngleTray1Case++; m_tAngleTray1Loop.Set_LoopTime(30000);
		}
		break;
	case 53:	// Check Position
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE1_Y, 0)) {
			m_tAngleTray1Loop.Takt_Save(4, 10);

			// Tray Align Out
			m_pDY02->oAngleStage1AlignIn = FALSE;
			g_objAJinAXL.Write_Output(2);
			m_nAngleTray1Case = 0; m_tAngleTray1Loop.Set_LoopTime(5000);
		}
		break;
	}

	// 4. (Error : 3400)
	if (m_tAngleTray1Loop.Over_LoopTime()) {
		g_objCommon.Show_Error(3400 + m_nAngleTray1Case);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 5. (Error : 3500)
BOOL CSequenceMain::AngleTray2_Run()
{
	static double	dStage2Z = 0.0;

	switch (m_nAngleTray2Case) {
	case 0:		// Wait for Angle Vision Done
		if (m_bUnloadLotEnd) {
			return TRUE;
		} else {
			// Load Position Check
			if (g_objCommon.Check_Position(AX_ANGLE_STAGE2_Y, 0) && !m_pDX02->iAngleStage2Exist &&
				!m_pDX02->iAngleStage2AlignIn && m_pDX02->iAngleStage2AlignOut)
			{	
				g_objCommon.Move_Position(AX_ANGLE_STAGE2_Z, 3);	// Support Ready Position
				m_nAngleTray2Case++; m_tAngleTray2Loop.Set_LoopTime(5000);
			}
		}
		break;

	case 1:
		if (m_nVisAngleCase > 8 && m_pDX02->iAnglePortExist) {	// 검사 진행 중이면
			if (m_pDX02->iAngleStage2Normal && !m_pDX02->iAngleStage2Rotate &&
				g_objCommon.Check_Position(AX_ANGLE_STAGE2_Y, 0) && g_objCommon.Check_Position(AX_ANGLE_STAGE2_Z, 3)) {

				g_objCommon.Move_Position(AX_ANGLE_STAGE2_Z, 5);	// Wait Position
				m_nAngleTray2Case++; m_tAngleTray2Loop.Set_LoopTime(5000);
			}
			break;

		} else if ((m_nVisAngleCase == 0 || m_nVisAngleCase == 24) && m_pDX02->iAnglePortExist && m_bAngleInspComplete) {	// 작업 끝났고 검사 완료했으면 진행.
			if (m_pDX02->iAngleStage2Normal && !m_pDX02->iAngleStage2Rotate && g_objCommon.Get_AnglePortMasterSlaveOut() &&
				g_objCommon.Check_Position(AX_ANGLE_STAGE2_Y, 0) && g_objCommon.Check_Position(AX_ANGLE_STAGE2_Z, 3)) {

				m_tAngleTray2Loop.Takt_Start();
				m_dwAngleTray2 = GetTickCount();
				g_objCommon.Move_Position(AX_ANGLE_STAGE2_Z, 1);
				m_nAngleTray2Case = 3; m_tAngleTray2Loop.Set_LoopTime(5000);
			}
			break;

		}
		if (m_bUnloadLotEnd) {
			g_objCommon.Move_Position(AX_ANGLE_STAGE2_Z, 0);	// Ready Down Position
			m_nAngleTray2Case = 0; m_tAngleTray2Loop.Set_LoopTime(5000);
		}
		return TRUE;
	case 2:		// 안전 확인. Z Axis Move to Support Up Position
		if ((m_nVisAngleCase == 0 || m_nVisAngleCase == 24) && m_pDX02->iAnglePortExist && m_bAngleInspComplete) {	// 작업 끝났고 검사 완료했으면 진행.

			if (m_pDX02->iAngleStage2Normal && !m_pDX02->iAngleStage2Rotate && g_objCommon.Get_AnglePortMasterSlaveOut() &&
				g_objCommon.Check_Position(AX_ANGLE_STAGE2_Y, 0) && g_objCommon.Check_Position(AX_ANGLE_STAGE2_Z, 5)) {
				m_tAngleTray2Loop.Takt_Start();
				m_dwAngleTray2 = GetTickCount();
				g_objCommon.Move_Position(AX_ANGLE_STAGE2_Z, 1);
				m_nAngleTray2Case++; m_tAngleTray2Loop.Set_LoopTime(5000);
			}
			break;
		}
		if (m_bUnloadLotEnd) {
			g_objCommon.Move_Position(AX_ANGLE_STAGE2_Z, 0);	// Ready Down Position
			m_nAngleTray2Case = 0; m_tAngleTray2Loop.Set_LoopTime(5000);
		}
		return TRUE;
	case 3:		// Angle Port Support Out, Stage Vacuum On
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE2_Z, 1)) {
			m_tAngleTray2Loop.Takt_Save(5, 1);
			m_tAngleTray2Loop.Takt_Start();
			g_objCommon.Set_AnglePortSupportOut();
			m_pDY02->oAngleStage2AlignIn = TRUE;
			g_objAJinAXL.Write_Output(2);
			m_nAngleTray2Case++; m_tAngleTray2Loop.Set_LoopTime(5000);
		}
		break;
	case 4:		// 정보전달, Z Axis Move to Ready Down Position
		if (g_objCommon.Get_AnglePortSupportOut() && m_pDX02->iAngleStage2Exist &&
			m_pDX02->iAngleStage2AlignIn && !m_pDX02->iAngleStage2AlignOut)
		{
			m_tAngleTray2Loop.Takt_Save(5, 2);
			m_tAngleTray2Loop.Takt_Start();

			memmove(gData.InfoAngleTray[1], gData.InfoAnglePortTray, sizeof(int) * 8 * 5);
			memset(gData.InfoAnglePortTray, 0x00, sizeof(int) * 8 * 5);
			gData.nPNoAngleTray[1] = gData.nPNoAnglePort[0]; gData.nPNoAnglePort[0] = 0;
			gData.nTNoAngleTray[1] = gData.nTNoAnglePort[0]; gData.nTNoAnglePort[0] = 0;
			m_nAnglePortTrayCnt--;
			//g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 2, 1);
			g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 1, 0);

			//g_objCommon.Move_Position(AX_ANGLE_STAGE2_Z, 0);
			m_bAngleInspComplete = FALSE;
			g_objCommon.Move_Position(AX_ANGLE_STAGE2_Z, 3);	// Support Ready Position
			m_nAngleTray2Case++; m_tAngleTray2Loop.Set_LoopTime(5000);
		}
		break;
	case 5:		// Angle Port Support In
		//if (g_objCommon.Check_Position(AX_ANGLE_STAGE2_Z, 0)) {
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE2_Z, 3)) {
			m_tAngleTray2Loop.Takt_Save(5, 3);
			m_tAngleTray2Loop.Takt_Start();
//			m_bAngleInspComplete = FALSE;
			g_objCommon.Set_AnglePortSupportIn();
			m_nAngleTray2Case++; m_tAngleTray2Loop.Set_LoopTime(5000);
		}
		break;
	case 6:		// Support Check
		if (g_objCommon.Get_AnglePortSupportIn()) {
			m_tAngleTray2Loop.Takt_Save(5, 4);			
			m_nAngleTray2Case++; m_tAngleTray2Loop.Set_LoopTime(5000);
		}
	case 7:		// 안전 확인.
		if (m_nAngleTray1Case >= 20) {	// Btm1 Picker CM Loading
			m_nAngleTray2Case++; m_tAngleTray2Loop.Set_LoopTime(5000);
		}
		return TRUE;
	case 8:		// Y Axis Move to Ready Position
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE2_Z, 3)) {
			m_tAngleTray2Loop.Takt_Start();
			if (gData.bR04A || gData.bR13A) {
				m_pDY02->oAngleStage2Rotate = TRUE;
				g_objAJinAXL.Write_Output(2);
			}
			g_objCommon.Move_Position(AX_ANGLE_STAGE2_Y, 1); // Ready Position
			m_nAngleTray2Case++; m_tAngleTray2Loop.Set_LoopTime(5000);
		}
		break;
	case 9:		// Y Axis Move to Ready Position
		if ((( (gData.bR04A || gData.bR13A) && !m_pDX02->iAngleStage2Normal &&  m_pDX02->iAngleStage2Rotate) || 
			 ((!gData.bR04A && !gData.bR13A) &&  m_pDX02->iAngleStage2Normal && !m_pDX02->iAngleStage2Rotate)) &&
			g_objCommon.Check_Position(AX_ANGLE_STAGE2_Y, 1)) {
			m_tAngleTray2Loop.Takt_Save(5, 5);
			m_tAngleTray2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_ANGLE_STAGE2_Z, 2);
			m_nAngleTray2Case = 10; m_tAngleTray2Loop.Set_LoopTime(5000);
		}
		break;
	case 10:
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE2_Z, 2)) {
			m_tAngleTray2Loop.Takt_Save(5, 6);
			m_nAngleTray2Case = 15; m_tAngleTray2Loop.Set_LoopTime(5000);
		}
		break;
	case 15:	// Wait for Stage1
		// Btm1 Picker CM Loading
		if (m_nAngleTray1Case > 23)	m_nAngleTray2Case++;
		return TRUE;

	case 16:	// Y Axis Move to Btm1 Picker L1 Position
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE2_Z, 2)) {
			m_tAngleTray2Loop.Takt_Start();
			if ((gData.bR04B || gData.bR13B_5 || gData.bR13A) && m_bBtm1ModulePick) {
				g_objCommon.Move_Position(AX_ANGLE_STAGE2_Y, 3); // Btm1 Picker L2 Position

			} else {
				g_objCommon.Move_Position(AX_ANGLE_STAGE2_Y, 2); // Btm1 Picker L1 Position
			}
			m_nAngleTray2Case++; m_tAngleTray2Loop.Set_LoopTime(5000);
		}
		break;
	case 17:	// Position Check
		if (((gData.bR04B || gData.bR13B_5 || gData.bR13A) && m_bBtm1ModulePick && g_objCommon.Check_Position(AX_ANGLE_STAGE2_Y, 3)) ||
			g_objCommon.Check_Position(AX_ANGLE_STAGE2_Y, 2)) {

			m_tAngleTray2Loop.Takt_Save(5, 7);
			Init_AngleTray(1);
			m_bBtm1ModulePick = FALSE;
			g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 2, 1);
			m_nAngleTray2Case = 20; m_tAngleTray2Loop.Set_LoopTime(5000);
		}
		break;

	case 20:	// Module Load Working
		return TRUE;

	case 21:	// 스테이지를 20mm 다운 한다.
		if (m_nAngleTray1Case <=20) {
			dStage2Z = m_pMoveData->dAngleStage2Z[2] - 20.0;	// CM Pick Up Position - 20mm
			g_objAJinAXL.Move_Absolute(AX_ANGLE_STAGE2_Z, dStage2Z); 
			m_nAngleTray2Case++; m_tAngleTray2Loop.Set_LoopTime(5000);
		} else { return TRUE; }
		break;
	case 22:	// Down Check
		if (g_objAJinAXL.Is_MoveDone(AX_ANGLE_STAGE2_Z, dStage2Z)) {
			m_nAngleTray2Case++; m_tAngleTray2Loop.Set_LoopTime(5000);
		}
		break;

	case 23:	// Y Axis Empty Tray Unload Position
		if (m_nAngleTray1Case == 30) return TRUE;	// Empty Tray Unloading
		if (m_nAngleTray1Case > 52 || m_nAngleTray1Case < 21) {	// Inter lock
			m_tAngleTray2Loop.Takt_Start();
			if (gData.bR04A || gData.bR13A) {
				m_pDY02->oAngleStage2Rotate = FALSE;
				g_objAJinAXL.Write_Output(2);
			}
			g_objCommon.Move_Position(AX_ANGLE_STAGE2_Y, 4); // Unload Position
			m_nAngleTray2Case++; m_tAngleTray2Loop.Set_LoopTime(5000);
		}
		return TRUE;
	case 24:	// Position Check & Z Axis Move To Tray Pick Up Position
		if (m_pDX02->iAngleStage2Normal && !m_pDX02->iAngleStage2Rotate &&
			g_objCommon.Check_Position(AX_ANGLE_STAGE2_Y, 4)) {
			m_tAngleTray2Loop.Takt_Save(5, 8);
			m_tAngleTray2Loop.Takt_Start();

			g_objCommon.Move_Position(AX_ANGLE_STAGE2_Z, 4);
			m_nAngleTray2Case++; m_tAngleTray2Loop.Set_LoopTime(5000);
		}
		break;
	case 25:	// Position Check
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE2_Z, 4)) {
			m_tAngleTray2Loop.Takt_Save(5, 11);
			m_nAngleTray2Case = 30; m_tAngleTray2Loop.Set_LoopTime(5000);
		}
		break;
	case 30:	// Empty Tray Unload Working
		return TRUE;

	case 31:	// Z Axis Move to Ready Down Position
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE2_Y, 4) && g_objCommon.Check_Position(AX_ANGLE_STAGE2_Z, 4) && !m_pDX02->iAngleStage2Exist) {
			m_tAngleTray2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_ANGLE_STAGE2_Z, 2);
			m_nAngleTray2Case++; m_tAngleTray2Loop.Set_LoopTime(30000);
		}
		break;
	case 32:	// Z Axis Move to Ready Down Position
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE2_Y, 4) && g_objCommon.Check_Position(AX_ANGLE_STAGE2_Z, 2)) {
			m_tAngleTray2Loop.Takt_Save(5, 6);
			m_strLog.Format("AngleStage2, %d", GetTickCount() - m_dwAngleTray2);
			g_objLogFile.Save_TestLog(m_strLog);
			m_nAngleTray2Case = 50; m_tAngleTray2Loop.Set_LoopTime(30000);
		}
		break;

	case 50:	// Wait for Loading
		if (m_nAngleTray1Case > 9 && m_nAngleTray1Case < 50) m_nAngleTray2Case++; 
		return TRUE;

	case 51:	// Y Axis Move to Load Position
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE2_Y, 4) && g_objCommon.Check_Position(AX_ANGLE_STAGE2_Z, 2)) {
			m_tAngleTray2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_ANGLE_STAGE2_Z, 0);
			m_nAngleTray2Case++; m_tAngleTray2Loop.Set_LoopTime(30000);
		}
		break;
	case 52:	// Check Down Position
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE2_Z, 0) && g_objCommon.Check_Position(AX_ANGLE_STAGE1_Z, 2)) {
			m_tAngleTray2Loop.Takt_Save(5, 9);
			g_objCommon.Move_Position(AX_ANGLE_STAGE2_Y, 0);
			m_nAngleTray2Case++; m_tAngleTray2Loop.Set_LoopTime(30000);
		}
		break;
	case 53:	// Check Position
		if (g_objCommon.Check_Position(AX_ANGLE_STAGE2_Y, 0)) {
			m_tAngleTray2Loop.Takt_Save(5, 10);

			// Tray Align Out
			m_pDY02->oAngleStage2AlignIn = FALSE;
			g_objAJinAXL.Write_Output(2);
			m_nAngleTray2Case = 0; m_tAngleTray2Loop.Set_LoopTime(5000);
		}
		break;
	}

	// 5. (Error : 3500)
	if (m_tAngleTray2Loop.Over_LoopTime()) {
		g_objCommon.Show_Error(3500 + m_nAngleTray2Case);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 6. (Error : 3600)
BOOL CSequenceMain::Btm1Picker_Run()
{
	static int nB1pWorkTray = 0;				// 작업중인 Tray (1 ~ 2)
	static int nB1PStartNo = 0, nB1PickNo = 0;		// Btm1 Picker 시작 번호(0~9), 열별 순번(0~4)
	static int nB1pTrayPosY = 0;				// 작업중인 Tray 위치 Y (R04A:0 ~ 3, R04B:0 ~ 2, R04C:0 ~ 3)
	static double dB1pTrayY = 0.0, dB1pX = 0.0;		// Y, X, Z 이동위치
	static int	nB1pRow = 0, nB1pScanNo = 0, nB1pInspStageNo = 0;
	static int	nB1INo1 = 0, nB1INo2 = 0, nB1INo3 = 0, nB1INo4 = 0;	// 검사할때 피커 번호
	static int	nErrRetry = 0;

	switch (m_nBtm1PickCase) 
	{
	case 0:		// Wait for Angle Tray L1 Position
		if (m_nAngleTray1Case == 20 || m_nAngleTray2Case == 20) m_nBtm1PickCase++;
		m_tBtm1PickLoop.Set_LoopTime(10000);
		return TRUE;

	case 1:		// Picker Z Move to Buffer Down Position
		if ((g_objCommon.Check_Position(AX_BTM1_PICKER_X, 0) || g_objCommon.Check_Position(AX_BTM1_PICKER_X, 1) ||
			g_objCommon.Check_Position(AX_BTM1_PICKER_X, 2) || g_objCommon.Check_Position(AX_BTM1_PICKER_X, 3)) && 
			g_objCommon.Check_Position(AX_BTM1_PICKER_P1, 0) && g_objCommon.Check_Position(AX_BTM1_PICKER_P2, 0) &&
			((gData.bR04A && g_objCommon.Get_Btm1PickerRotate()) || (!gData.bR04A && g_objCommon.Get_Btm1PickerNormal()))) 
		{
			if (!m_tBtm1PickLoop.Waiting_Time(2000)) break;
			if (m_nAngleTray1Case != 20 && m_nAngleTray2Case != 20) break;	// 인터락

			m_dwBtm1Pick = GetTickCount();
			if (m_nAngleTray1Case == 20) nB1pWorkTray = 1;
			if (m_nAngleTray2Case == 20) nB1pWorkTray = 2;

			if (Check_AngleTrayEmpty(nB1pWorkTray))
			{	// Tray Change
				if (nB1pWorkTray == 1) m_nAngleTray1Case = 21;
				if (nB1pWorkTray == 2) m_nAngleTray2Case = 21;
				m_nBtm1PickCase = 0; m_tBtm1PickLoop.Set_LoopTime(10000);
			} 
			else 
			{
				m_nBtm1PickCase++; m_tBtm1PickLoop.Set_LoopTime(10000);
			}
		}
		break;

	case 2:		// Angle Tray Y, Btm1 Picker X Move to Tray Down Position
		if ((nB1pWorkTray == 1 && m_nAngleTray2Case >= 30 && m_nAngleTray2Case < 53 && (((gData.bR04B || gData.bR13B_5 || gData.bR13A) && nB1pRow == 0 && nB1pTrayPosY > 1) || (gData.bR04A && nB1pTrayPosY > 1))) ||
			(nB1pWorkTray == 2 && m_nAngleTray1Case >= 30 && m_nAngleTray1Case < 53 && (((gData.bR04B || gData.bR13B_5 || gData.bR13A) && nB1pRow == 0 && nB1pTrayPosY > 1) || (gData.bR04A && nB1pTrayPosY > 1)))) return TRUE;	// Interlock Empty Tray 작업 중에는 픽업을 하지 않는다.
		// Pick Position
		if (Select_Btm1PickPos(nB1PStartNo)) 
		{
			m_tBtm1PickLoop.Takt_Start();
			Select_AngleTrayPos(nB1pWorkTray, nB1pTrayPosY, nB1pRow);

			if ((gData.bR04B || gData.bR13B_5 || gData.bR13A)) 
			{
				if (nB1PStartNo > 4) nB1pRow = 1;
				else				 nB1pRow = 0;
			}
			
			// 0: Stage1 L1, 1:Stage1 L2, 2:Stage2 L1, 3:Stage2 L2
			// R04C는 Tray 8x4이다
			if (gData.bR04C) {
				if (nB1pWorkTray == 1) {
					if (nB1pRow == 0)	dB1pX = m_pMoveData->dBtm1PickerX[0];
					else				dB1pX = m_pMoveData->dBtm1PickerX[1] + m_pEquipData->dTrayPitchX * 4;
				}
				if (nB1pWorkTray == 2) {
					if (nB1pRow == 0)	dB1pX = m_pMoveData->dBtm1PickerX[2];
					else				dB1pX = m_pMoveData->dBtm1PickerX[3] + m_pEquipData->dTrayPitchX * 4;
				}

			} else {
				if (nB1pWorkTray == 1) {
					if (nB1pRow == 0) dB1pX = m_pMoveData->dBtm1PickerX[0];	//Stage1 L1 Position
					if (nB1pRow == 1) dB1pX = m_pMoveData->dBtm1PickerX[1];	//Stage1 L2 Position
				}
				if (nB1pWorkTray == 2) {
					if (nB1pRow == 0) dB1pX = m_pMoveData->dBtm1PickerX[2];	//Stage2 L1 Position
					if (nB1pRow == 1) dB1pX = m_pMoveData->dBtm1PickerX[3];	//Stage2 L2 Position
				}
			}
			g_objAJinAXL.Move_Absolute(AX_BTM1_PICKER_X, dB1pX);

			if (nB1pWorkTray == 1 && m_nAngleTray1Case == 20) {
				if (gData.bR04A || gData.bR13A)	dB1pTrayY = m_pMoveData->dAngleStage1Y[2+nB1pRow] + nB1pTrayPosY * m_pEquipData->dTrayPitchX;
				else		dB1pTrayY = m_pMoveData->dAngleStage1Y[2+nB1pRow] + nB1pTrayPosY * m_pEquipData->dTrayPitchY;
				g_objAJinAXL.Move_Absolute(AX_ANGLE_STAGE1_Y, dB1pTrayY);

// 				if (m_nAngleTray2Case == 15) {
// 					if (nB1pRow == 0 && nB1pTrayPosY == 2) {
// 						m_bBtm1ModulePick = TRUE;
// 						m_nAngleTray2Case = 16;
// 					}
// 				}
			}
			if (nB1pWorkTray == 2 && m_nAngleTray2Case == 20) {
				if (gData.bR04A || gData.bR13A)	dB1pTrayY = m_pMoveData->dAngleStage2Y[2+nB1pRow] + nB1pTrayPosY * m_pEquipData->dTrayPitchX;
				else		dB1pTrayY = m_pMoveData->dAngleStage2Y[2+nB1pRow] + nB1pTrayPosY * m_pEquipData->dTrayPitchY;
				g_objAJinAXL.Move_Absolute(AX_ANGLE_STAGE2_Y, dB1pTrayY);

// 				if (m_nAngleTray1Case == 15) {
// 					if (nB1pRow == 0 && nB1pTrayPosY == 2) {
// 						m_bBtm1ModulePick = TRUE;
// 						m_nAngleTray1Case = 16;
// 					}
// 				}
			}
			
			m_nBtm1PickCase++; m_tBtm1PickLoop.Set_LoopTime(10000);
		}			
		break;
	case 3:		// Btm1 Picker Down
		if (g_objAJinAXL.Is_MoveDone(AX_BTM1_PICKER_X, dB1pX)) {
			if ((nB1pWorkTray == 1 && g_objAJinAXL.Is_MoveDone(AX_ANGLE_STAGE1_Y, dB1pTrayY)) ||
				(nB1pWorkTray == 2 && g_objAJinAXL.Is_MoveDone(AX_ANGLE_STAGE2_Y, dB1pTrayY))) {

				if ((nB1pWorkTray == 1 && m_nAngleTray2Case > 15 && m_nAngleTray2Case <= 20 && !g_objAJinAXL.Is_Done(AX_ANGLE_STAGE2_Y)) ||
					(nB1pWorkTray == 2 && m_nAngleTray1Case > 15 && m_nAngleTray1Case <= 20 && !g_objAJinAXL.Is_Done(AX_ANGLE_STAGE1_Y))) break;

				m_tBtm1PickLoop.Takt_Save(6, 1);
				m_tBtm1PickLoop.Takt_Start();
				g_objCommon.Move_Position(AX_BTM1_PICKER_Z, 1);	// Load Down
				g_objCommon.Set_InfoBtm1PickerVacOn(INFO_STAGE, nB1pRow, nB1pWorkTray, nB1pTrayPosY);
				//g_objCommon.Set_InfoBtm1PickerDown(INFO_STAGE, nB1pRow, nB1pWorkTray, nB1pTrayPosY);	// Picker 고정으로 사용.
				g_objCommon.Set_Btm1PickerDown();

				m_nBtm1PickCase++; m_tBtm1PickLoop.Set_LoopTime(5000);
			}
		}
		break;
	case 4:		// 정보전달(Tray->Picker) & Btm1 Vacuum On
		if (g_objCommon.Check_Position(AX_BTM1_PICKER_Z, 1) && g_objCommon.Get_Btm1PickerDown(0)) {
			int nTrayX = 0, nBtmX = 0;
			nBtmX = 5 * nB1pRow;

			for (int i = 0; i < gData.nBtmPickQt; i++) {
				if (gData.bR04A) {	// 90도 회전되어 있다.
					gData.InfoBtm1Pick[nBtmX+i] = gData.InfoAngleTray[nB1pWorkTray-1][4-i][nB1pTrayPosY]; 
					gData.InfoAngleTray[nB1pWorkTray-1][4-i][nB1pTrayPosY] = 0;
					gData.nTNoBtm1Pick[nBtmX+i] = gData.nTNoAngleTray[nB1pWorkTray-1];

					gData.nCNoBtm1Pick[nBtmX+i] = (gData.InfoBtm1Pick[nBtmX+i] > 0 ? ((17 + nB1pTrayPosY) - (4 * i)) : 0);

				} else if (gData.bR13A) {
					gData.InfoBtm1Pick[nBtmX+i] = gData.InfoAngleTray[nB1pWorkTray-1][4-i][nB1pTrayPosY]; 
					gData.InfoAngleTray[nB1pWorkTray-1][4-i][nB1pTrayPosY] = 0;
					gData.nTNoBtm1Pick[nBtmX+i] = gData.nTNoAngleTray[nB1pWorkTray-1];

					gData.nCNoBtm1Pick[nBtmX+i] = (gData.InfoBtm1Pick[nBtmX+i] > 0 ? ((13 + nB1pTrayPosY) - (3 * i)) : 0);

				} else {
					if (gData.bR04C) nTrayX = 4 * nB1pRow;

					gData.InfoBtm1Pick[nBtmX+i] = gData.InfoAngleTray[nB1pWorkTray-1][nB1pTrayPosY][nTrayX+i]; 
					gData.InfoAngleTray[nB1pWorkTray-1][nB1pTrayPosY][nTrayX+i] = 0;
					gData.nTNoBtm1Pick[nBtmX+i] = gData.nTNoAngleTray[nB1pWorkTray-1];

					gData.nCNoBtm1Pick[nBtmX+i] = (gData.InfoBtm1Pick[nBtmX+i] > 0 ? (nB1pTrayPosY * gData.nTrayX + nTrayX + i + 1) : 0);
				}
			}
			gData.nPNoBtm1Pick = gData.nPNoAngleTray[nB1pWorkTray-1]; 
			if (Check_AngleTrayEmpty(nB1pWorkTray)) {	// 트레이가 다 비었을때 초기화 해준다.
				gData.nPNoAngleTray[nB1pWorkTray-1] = 0;
				gData.nTNoAngleTray[nB1pWorkTray-1] = 0;
			}
			g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 2, nB1pWorkTray-1);
			g_objCommon.Set_InfoBtm1PickerVacOn(INFO_PICK);

			//R04C는 한라인에서 4개씩 2번 집는다.
			m_nBtm1PickCase++; m_tBtm1PickLoop.Set_LoopTime(5000);
		}
		break;
	case 5:		// Btm1 Picker Up
		//if (g_objCommon.Get_InfoBtm1PickerVacOn()) {	// Delay Time 만 사용한다.
			if (!m_tBtm1PickLoop.Waiting_Time(m_pEquipData->nDelayAdd[0])) break;	// Btm1 Delay
			//g_objCommon.Set_Btm1PickerUp(0);	// Picker 고정으로 사용.
			g_objCommon.Move_Position(AX_BTM1_PICKER_Z, 0);	// Ready up
			m_bBtm1ModulePick = FALSE;
			m_nBtm1PickCase++; m_tBtm1PickLoop.Set_LoopTime(5000);
		//}
		break;
	case 6:		// Btm1 Picker Full, Tray Empty check & Tray Change
		if (g_objCommon.Check_Position(AX_BTM1_PICKER_Z, 0) && g_objCommon.Get_InfoBtm1PickerVacOn()) {
			m_tBtm1PickLoop.Takt_Save(6, 2);
			if (Check_Btm1PickerFull() || Check_AngleTrayEmpty(nB1pWorkTray)) {
				if (Check_AngleTrayEmpty(nB1pWorkTray) || ((gData.bR04A || gData.bR13A) && (nB1pTrayPosY + 1 >= gData.nTrayX)) || ((!gData.bR04A && !gData.bR13A) && (nB1pTrayPosY + 1 >= gData.nTrayY))) {	// Tray Change
					nB1pTrayPosY = 0;
					if (nB1pWorkTray == 1) { nB1pWorkTray = 0; m_nAngleTray1Case = 21; }
					if (nB1pWorkTray == 2) { nB1pWorkTray = 0; m_nAngleTray2Case = 21; }
				}
				//g_objCommon.Move_Position(AX_BTM1_PICKER_Z, 0);	// Ready up

				// R04B 모델은 마지막 라인을 집고 다음 트레이에서 나머지 한줄(Picker L2) 집을 준비를 한다. Tray 5x3
				if ((gData.bR04B || gData.bR13B_5 || gData.bR13A)) {
					if (Check_Btm1PickerFull() || Check_LoadLotEnd(gData.nPNoBtm1Pick, 1)) { nB1pTrayPosY++; m_nBtm1PickCase++; m_tBtm1PickLoop.Set_LoopTime(5000); }
					else { m_bBtm1ModulePick = TRUE; m_nBtm1PickCase = 1; m_tBtm1PickLoop.Set_LoopTime(20000); }
				} else {
					m_nBtm1PickCase++; m_tBtm1PickLoop.Set_LoopTime(5000);
				}
				
			} else {
				if ((gData.bR04B || gData.bR13B_5 || gData.bR13A) && Select_Btm1PickPos(nB1PStartNo)) {
					if (nB1PStartNo > 4) nB1pRow = 1;
					else				 nB1pRow = 0;
				}
				nB1pTrayPosY++;
				m_nBtm1PickCase = 2; m_tBtm1PickLoop.Set_LoopTime(5000);
			}
		}
		break;
	case 7:		// X Axis Move To Btm1 Inspection Position
		if (g_objCommon.Check_Position(AX_BTM1_PICKER_Z, 0) && g_objCommon.Get_InfoBtm1PickerVacOn()) {
			m_tBtm1PickLoop.Takt_Start();

			dB1pX = m_pMoveData->dBtm1PickerX[4];
			g_objAJinAXL.Move_Absolute(AX_BTM1_PICKER_X, dB1pX);
			g_objCommon.Move_Position(AX_BTM1_PICKER_Z, 2);	// Btm1 Down
			if (gData.bR04C) {	// 이미지에 모듈이 6개 들어가기 때문에 벌려줘야한다.
				g_objCommon.Move_Position(AX_BTM1_PICKER_P1, 1);	// Inspection Position
				g_objCommon.Move_Position(AX_BTM1_PICKER_P2, 1);	// Inspection Position
			}
			nB1pScanNo = 0;
			////////////////////////////////////////////////////////////////////////////
			int nLastT = gData.nTrayUseCount[gData.nPNoBtm1Pick-1];
			int nLastC = gData.nCmUseCount[gData.nPNoBtm1Pick-1] % gData.nCmMaxCount;
			if (nLastC == 0) nLastC = gData.nCmMaxCount;
			for (int i = 0; i < 10; i++) {
				if (gData.nTNoBtm1Pick[i] == 1 && gData.nCNoBtm1Pick[i] == 1) m_dwFirstLoad = GetTickCount() - m_dwFirstLoad;	// First Module
				if (gData.nTNoBtm1Pick[i] == nLastT && gData.nCNoBtm1Pick[i] == nLastC) m_dwLastUnLoad = GetTickCount();		// Last Module
			}
			////////////////////////////////////////////////////////////////////////////
			//m_nBtm1PickCase++; m_tBtm1PickLoop.Set_LoopTime(10000);
			m_nBtm1PickCase = 15; m_tBtm1PickLoop.Set_LoopTime(10000);			
		}
		break;
	case 8:		// Z Axis Move to Btm1 Down
		if (g_objAJinAXL.Is_MoveDone(AX_BTM1_PICKER_X, dB1pX) && 
			((!gData.bR04C && g_objCommon.Check_Position(AX_BTM1_PICKER_P1, 0) && g_objCommon.Check_Position(AX_BTM1_PICKER_P2, 0)) ||
			(gData.bR04C && g_objCommon.Check_Position(AX_BTM1_PICKER_P1, 1) && g_objCommon.Check_Position(AX_BTM1_PICKER_P2, 1)))) {
			m_tBtm1PickLoop.Takt_Save(6, 3);
			m_tBtm1PickLoop.Takt_Start();
			g_objCommon.Move_Position(AX_BTM1_PICKER_Z, 2);	// Btm1 Down
			m_nBtm1PickCase++; m_tBtm1PickLoop.Set_LoopTime(10000);			
		}
		break;
	case 9:		// Set Load Complete
		if (g_objAJinAXL.Is_MoveDone(AX_BTM1_PICKER_X, dB1pX)  &&
			g_objCommon.Check_Position(AX_BTM1_PICKER_Z, 2) ) {

			m_tBtm1PickLoop.Takt_Save(6, 10);
			if (m_pEquipData->bUseInspectBtm1) {				
				Set_InspectJigNo(0, nB1pScanNo, nB1INo1, nB1INo2, nB1INo3, nB1INo4);	// 검사하는 피커 번호를 설정해준다.

				g_objInspector.Set_LoadComplete(INSPECTOR_PC1, "B1SP", gLot.sLotID[gData.nPNoBtm1Pick-1], gData.nPNoBtm1Pick, 
					gData.nTNoBtm1Pick[nB1INo1-1], gData.nTNoBtm1Pick[nB1INo2-1], 0, 0,
					gData.nCNoBtm1Pick[nB1INo1-1], gData.nCNoBtm1Pick[nB1INo2-1], 0, 0);

				m_nBtm1PickCase = 10; m_tBtm1PickLoop.Set_LoopTime(90000);
			} else {
				m_nBtm1PickCase = 15; m_tBtm1PickLoop.Set_LoopTime(5000);
			}
			nB1pScanNo++;
		}
		break;
	
	case 10:	// Wait
		if (!m_pEquipData->bUseInspectBtm1) {
			m_nBtm1PickCase = 15; m_tBtm1PickLoop.Set_LoopTime(5000);
		}
		break;

	case 11:	// Btm1 Picker Z Focus Move
		if (g_objAJinAXL.Is_Done(AX_BTM1_PICKER_Z)) {
			m_tBtm1PickLoop.Takt_Start();
			g_objAJinAXL.Move_Absolute(AX_BTM1_PICKER_Z, m_dBtm1Z);
			m_nBtm1PickCase++; m_tBtm1PickLoop.Set_LoopTime(5000);
		}
		break;
	case 12:	// Send Move Complete
		if (g_objAJinAXL.Is_Done(AX_BTM1_PICKER_Z)) {
			m_tBtm1PickLoop.Takt_Save(6, 4);
			CString strLog;
			double dEnc = g_objAJinAXL.Get_Position(AX_BTM1_PICKER_Z);
			strLog.Format("%0.3lf,%0.3lf", m_dBtm1Z, dEnc);
			g_objLogFile.Save_FocusAxisLog(AX_BTM1_PICKER_Z, strLog);
			g_objInspector.Set_MoveComplete(INSPECTOR_PC1, "B1SP");
			m_nBtm1PickCase = 10; m_tBtm1PickLoop.Set_LoopTime(90000);	// 90초
		}
		break;

	case 13:	// Btm1 Shift Y Move
		if ( g_objAJinAXL.Is_Done(AX_BTM1_PICKER_X) && g_objAJinAXL.Is_Done(AX_BTM1_PICKER_Z)) {
			m_tBtm1PickLoop.Takt_Start();
			
			dB1pX = dB1pX + m_pEquipData->dInspectBTM1ShiftX;
			
			g_objAJinAXL.Move_Absolute(AX_BTM1_PICKER_X, dB1pX);
			g_objAJinAXL.Move_Absolute(AX_BTM1_PICKER_Z, m_dBtm1Z);
			m_nBtm1PickCase++; m_tBtm1PickLoop.Set_LoopTime(5000);
		}
		break;
	case 14:	// Send Shift Complete
		if ( g_objAJinAXL.Is_MoveDone(AX_BTM1_PICKER_X, dB1pX) && g_objAJinAXL.Is_Done(AX_BTM1_PICKER_Z)) {
			m_tBtm1PickLoop.Takt_Save(6, 9);
			g_objInspector.Set_SMoveComplete(INSPECTOR_PC1, "B1SP");
			m_nBtm1PickCase = 10; m_tBtm1PickLoop.Set_LoopTime(90000);	// 90초
		}
		break;
		
	case 15:	// X Move to Pitch & Inspection End Check
		if (!m_pEquipData->bUseInspectBtm1) { if(!m_tBtm1PickLoop.Waiting_Time(500)) break; }

		if (g_objAJinAXL.Is_MoveDone(AX_BTM1_PICKER_X, dB1pX) &&
			((!gData.bR04C && g_objCommon.Check_Position(AX_BTM1_PICKER_P1, 0) && g_objCommon.Check_Position(AX_BTM1_PICKER_P2, 0)) ||
			(gData.bR04C && g_objCommon.Check_Position(AX_BTM1_PICKER_P1, 1) && g_objCommon.Check_Position(AX_BTM1_PICKER_P2, 1)))) {
			//nB1pScanNo++;
			if (gData.bReload[1]) {
				nB1pScanNo--; gData.bReload[1] = FALSE;
			}
			if ((nB1pScanNo > (gData.nBtmPickQt -1) && !gData.bR04C) || (nB1pScanNo > (gData.nBtmPickQt -1) && gData.bR04C)) { 
				nB1pScanNo = 0;
				m_tBtm1PickLoop.Takt_Start();
				// LJH A모델만 Picker Trun을 하니 A모델만 Pitch1/2 -2.5으로 보내면 될듯...
				if (gData.bR04A) {
					g_objAJinAXL.Move_Absolute(AX_BTM1_PICKER_P1, -2.5);
					g_objAJinAXL.Move_Absolute(AX_BTM1_PICKER_P2, -2.5);

				} else {
					g_objCommon.Move_Position(AX_BTM1_PICKER_P1, 1);	// Inspection Position
					g_objCommon.Move_Position(AX_BTM1_PICKER_P2, 1);	// Inspection Position
				}				
				g_objCommon.Move_Position(AX_BTM1_PICKER_Z, 0);
				

				if (gData.bR04A)
				{
					g_objCommon.Set_Btm1PickerUp24();
					g_objCommon.Set_Btm1PickerUp79();
				}
				
				m_bReqShiftB1Y = FALSE;
				m_nBtm1PickCase = 20; m_tBtm1PickLoop.Set_LoopTime(10000);

			} else {
				if (Select_BtmScanPos(AUTO_VISION_BTM1, nB1pScanNo)) {
					m_tBtm1PickLoop.Takt_Save(6, 11);
					m_tBtm1PickLoop.Takt_Start();
					if (gData.bR04A || gData.bR13A)		dB1pX = m_pMoveData->dBtm1PickerX[4] + nB1pScanNo * m_pEquipData->dTrayPitchY;	// Btm1 Inspect
					else if (gData.bR04C)				dB1pX = m_pMoveData->dBtm1PickerX[4] + nB1pScanNo * m_pEquipData->dInspectPitchX;	// Btm1 Inspect
					else								dB1pX = m_pMoveData->dBtm1PickerX[4] + nB1pScanNo * m_pEquipData->dTrayPitchX;	// Btm1 Inspect

					g_objAJinAXL.Move_Absolute(AX_BTM1_PICKER_X, dB1pX);			
					g_objCommon.Move_Position(AX_BTM1_PICKER_Z, 2);	// Btm1 Down
					
					m_nBtm1PickCase = 8; m_tBtm1PickLoop.Set_LoopTime(10000);
				} 
				else 
				{
					nB1pScanNo++;
				}
			}
		}
		break;

	case 20:	// X, P1, P2 Move to Inspection Stage
		if (g_objAJinAXL.Is_MoveDone(AX_BTM1_PICKER_X, dB1pX) &&
			(!gData.bR04A || g_objCommon.Get_Btm1PickerUp24()) && 
			(!gData.bR04A || g_objCommon.Get_Btm1PickerUp79()) && 
			(!gData.bR04A || (g_objAJinAXL.Is_MoveDone(AX_BTM1_PICKER_P1, -2.5) && g_objAJinAXL.Is_MoveDone(AX_BTM1_PICKER_P2, -2.5))))
		{
// 		if (g_objAJinAXL.Is_MoveDone(AX_BTM1_PICKER_X, dB1pX) && 
// 			(!gData.bR04A || g_objCommon.Get_Btm1PickerUp24()) && 
// 			(!gData.bR04A || g_objCommon.Get_Btm1PickerUp79()) && 
// 			(!gData.bR04A || (g_objCommon.Check_Position(AX_BTM1_PICKER_P1, 1) && g_objCommon.Check_Position(AX_BTM1_PICKER_P2, 1)))) {

			m_tBtm1PickLoop.Takt_Save(6, 5);
			m_tBtm1PickLoop.Takt_Start();
			g_objCommon.Move_Position(AX_BTM1_PICKER_X, 5);	// Inspecting Stage Pos
			if (gData.bR04A) {
#ifdef EDITION_2ND
				m_pDY03->oBtm1PickerReadyL1 = TRUE; m_pDY03->oBtm1PickerTurnL1 = FALSE;
				m_pDY03->oBtm1PickerReadyL2 = TRUE; m_pDY03->oBtm1PickerTurnL2 = FALSE;
				m_pDY03->oBtm1PickerReadyL3 = TRUE; m_pDY03->oBtm1PickerTurnL3 = FALSE;
				m_pDY03->oBtm1PickerReadyL4 = TRUE; m_pDY03->oBtm1PickerTurnL4 = FALSE;
				m_pDY03->oBtm1PickerReadyL5 = TRUE; m_pDY03->oBtm1PickerTurnL5 = FALSE;
				m_pDY03->oBtm1PickerReadyL6 = TRUE; m_pDY03->oBtm1PickerTurnL6 = FALSE;
				m_pDY03->oBtm1PickerReadyL7 = TRUE; m_pDY03->oBtm1PickerTurnL7 = FALSE;
				m_pDY03->oBtm1PickerReadyL8 = TRUE; m_pDY03->oBtm1PickerTurnL8 = FALSE;
				m_pDY03->oBtm1PickerReadyL9 = TRUE; m_pDY03->oBtm1PickerTurnL9 = FALSE;
				m_pDY03->oBtm1PickerReadyL10 = TRUE; m_pDY03->oBtm1PickerTurnL10 = FALSE;
#else
				m_pDY03->oBtm1PickerTurn01 = FALSE;	m_pDY03->oBtm1PickerTurn06 = FALSE;
				m_pDY03->oBtm1PickerTurn23 = FALSE;	m_pDY03->oBtm1PickerTurn78 = FALSE;
				m_pDY03->oBtm1PickerTurn45 = FALSE;	m_pDY03->oBtm1PickerTurn910 = FALSE;
#endif
				g_objAJinAXL.Write_Output(3);
			}
			m_nBtm1PickCase++; m_tBtm1PickLoop.Set_LoopTime(10000);
		}
		break;
	case 21:	// Z Move to Unload Down
		if (g_objCommon.Check_Position(AX_BTM1_PICKER_X, 5) && g_objCommon.Get_Btm1PickerNormal() &&
			g_objCommon.Check_Position(AX_BTM1_PICKER_Z, 0) && g_objCommon.Get_InfoBtm1PickerVacOn() &&
			((gData.bR04A && g_objAJinAXL.Is_MoveDone(AX_BTM1_PICKER_P1, -2.5) && g_objAJinAXL.Is_MoveDone(AX_BTM1_PICKER_P2, -2.5)) ||
			(!gData.bR04A && g_objCommon.Check_Position(AX_BTM1_PICKER_P1, 1) && g_objCommon.Check_Position(AX_BTM1_PICKER_P2, 1)))) 
		{
			if (!m_tBtm1PickLoop.Waiting_Time(2000)) break;

			if (m_nInspect1Case == 0 || m_nInspect2Case == 0 || m_nInspect3Case == 0) {
				if (m_nInspect1Case == 0) nB1pInspStageNo = 1;
				if (m_nInspect2Case == 0) nB1pInspStageNo = 2;
				if (m_nInspect3Case == 0) nB1pInspStageNo = 3;
			
				nErrRetry = 0;	//Retry Clear

				m_tBtm1PickLoop.Takt_Save(6, 6);
				m_tBtm1PickLoop.Takt_Start();

				g_objCommon.Move_Position(AX_BTM1_PICKER_P1, 1);	// Inspection Position
 				g_objCommon.Move_Position(AX_BTM1_PICKER_P2, 1);	// Inspection Position
				m_nBtm1PickCase++; m_tBtm1PickLoop.Set_LoopTime(5000);

			} else { m_tBtm1PickLoop.Set_LoopTime(5000); return TRUE; }	// 앞공정이 밀려있어서 Stage가 없으면 대기.
		}
		break;
	case 22:	// 안전 확인 후 Picker Down, Inspection Stage Vac On
		if (g_objCommon.Check_Position(AX_BTM1_PICKER_P1, 1) && g_objCommon.Check_Position(AX_BTM1_PICKER_P2, 1)) {
			
			if ((nB1pInspStageNo == 1 && m_pDX05->iInspectStage1Up && !m_pDX05->iInspectStage1Down) ||
				(nB1pInspStageNo == 2 && m_pDX05->iInspectStage2Up && !m_pDX05->iInspectStage2Down && m_pDX05->iInspectStage2Fwd && !m_pDX05->iInspectStage2Bwd) ||
				(nB1pInspStageNo == 3 && m_pDX05->iInspectStage3Up && !m_pDX05->iInspectStage3Down && m_pDX05->iInspectStage3Fwd && !m_pDX05->iInspectStage3Bwd)) {

				//g_objCommon.Set_InfoBtm1PickerDown(INFO_PICK);	// Picker 고정으로 사용.
				g_objCommon.Set_Btm1PickerDown();

				g_objCommon.Move_Position(AX_BTM1_PICKER_Z, 3);	//Unload Down
				g_objCommon.Set_InfoInspectVacOn(nB1pInspStageNo);
				m_nBtm1PickCase++; m_tBtm1PickLoop.Set_LoopTime(5000);
			}
		}
		break;
	case 23:	// 정보전달, Btm1 Picker Vac Off
		if (g_objCommon.Check_Position(AX_BTM1_PICKER_Z, 3) && g_objCommon.Get_Btm1PickerDown(0)) {
			for (int i = 0; i < 10; i++) 
			{
				gData.InfoInspect[nB1pInspStageNo-1][i] = gData.InfoBtm1Pick[i]; gData.InfoBtm1Pick[i] = 0;

				gData.nTNoInspect[nB1pInspStageNo-1][i] = gData.nTNoBtm1Pick[i];
				gData.nCNoInspect[nB1pInspStageNo-1][i] = gData.nCNoBtm1Pick[i];
				gData.nTNoBtm1Pick[i] = gData.nCNoBtm1Pick[i] = 0;
			}
			gData.nPNoInspect[nB1pInspStageNo-1] = gData.nPNoBtm1Pick; gData.nPNoBtm1Pick = 0;
			//g_objCommon.Set_InfoInspectVacOn(nB1pInspStageNo);
			m_pThreadVacuumB1p = AfxBeginThread(Thread_Vacuum_B1p, (LPVOID)0);
			m_nBtm1PickCase++; m_tBtm1PickLoop.Set_LoopTime(5000);
		}
		break;
	case 24:	// Picker Up
		if (!m_pThreadVacuumB1p /*&& g_objCommon.Get_InfoInspectVacOn(nB1pInspStageNo) && g_objCommon.Get_Btm1PickerVacOff(0)*/) {

			//g_objCommon.Set_Btm1PickerUp(0);	// Picker 고정으로 사용.
			g_objCommon.Move_Position(AX_BTM1_PICKER_Z, 0);	// Ready Up			
			m_nBtm1PickCase++; m_tBtm1PickLoop.Set_LoopTime(5000);
		}
		break;
	case 25:	// X, P1, P2 Move to Load Position
		if (g_objCommon.Check_Position(AX_BTM1_PICKER_Z, 0) && g_objCommon.Get_Btm1PickerVacOff(0)/*g_objCommon.Get_Btm1PickerUp(0) && g_objCommon.Get_InfoInspectVacOn(nB1pInspStageNo)*/) {
			// Inspection Stage Start
			if (nB1pInspStageNo = 1 && m_nInspect1Case == 0) m_nInspect1Case = 1;
			if (nB1pInspStageNo = 2 && m_nInspect2Case == 0) m_nInspect2Case = 1;
			if (nB1pInspStageNo = 3 && m_nInspect3Case == 0) m_nInspect3Case = 1;

			m_tBtm1PickLoop.Takt_Save(6, 7);
			m_tBtm1PickLoop.Takt_Start();
			if (nB1pWorkTray == 0) {
				dB1pX = m_pMoveData->dBtm1PickerX[0];
				g_objAJinAXL.Move_Absolute(AX_BTM1_PICKER_X, dB1pX);
				
				if (gData.bR04A) {
#ifdef EDITION_2ND
					m_pDY03->oBtm1PickerReadyL1 = FALSE; m_pDY03->oBtm1PickerTurnL1 = TRUE;
					m_pDY03->oBtm1PickerReadyL2 = FALSE; m_pDY03->oBtm1PickerTurnL2 = TRUE;
					m_pDY03->oBtm1PickerReadyL3 = FALSE; m_pDY03->oBtm1PickerTurnL3 = TRUE;
					m_pDY03->oBtm1PickerReadyL4 = FALSE; m_pDY03->oBtm1PickerTurnL4 = TRUE;
					m_pDY03->oBtm1PickerReadyL5 = FALSE; m_pDY03->oBtm1PickerTurnL5 = TRUE;
					m_pDY03->oBtm1PickerReadyL6 = FALSE; m_pDY03->oBtm1PickerTurnL6 = TRUE;
					m_pDY03->oBtm1PickerReadyL7 = FALSE; m_pDY03->oBtm1PickerTurnL7 = TRUE;
					m_pDY03->oBtm1PickerReadyL8 = FALSE; m_pDY03->oBtm1PickerTurnL8 = TRUE;
					m_pDY03->oBtm1PickerReadyL9 = FALSE; m_pDY03->oBtm1PickerTurnL9 = TRUE;
					m_pDY03->oBtm1PickerReadyL10 = FALSE; m_pDY03->oBtm1PickerTurnL10 = TRUE;
#else
					m_pDY03->oBtm1PickerTurn01 = TRUE;	m_pDY03->oBtm1PickerTurn06 = TRUE;
					m_pDY03->oBtm1PickerTurn23 = TRUE;	m_pDY03->oBtm1PickerTurn78 = TRUE;
					m_pDY03->oBtm1PickerTurn45 = TRUE;	m_pDY03->oBtm1PickerTurn910 = TRUE;
#endif
					g_objAJinAXL.Write_Output(3);
				} 
				else {	// LJH A모델이 아니면 복귀하면서 Tray Pitch로 변환.
					g_objCommon.Move_Position(AX_BTM1_PICKER_P1, 0);
					g_objCommon.Move_Position(AX_BTM1_PICKER_P2, 0);
				}
				m_nBtm1PickCase++; m_tBtm1PickLoop.Set_LoopTime(5000);
			} else {
				// Pick Position
				if (Select_Btm1PickPos(nB1PStartNo)) {	// Tray
					m_tBtm1PickLoop.Takt_Start();
					Select_AngleTrayPos(nB1pWorkTray, nB1pTrayPosY, nB1pRow);
					if (nB1PStartNo > 4) { nB1pRow = 1; }
					else				 { nB1pRow = 0; }

					// 0: Stage1 L1, 1:Stage1 L2, 2:Stage2 L1, 3:Stage2 L2
					// R04C는 Tray 8x4이다
					if (gData.bR04C) {
						if (nB1pWorkTray == 1) {
							if (nB1pRow == 0)	dB1pX = m_pMoveData->dBtm1PickerX[0];
							else				dB1pX = m_pMoveData->dBtm1PickerX[1] + m_pEquipData->dTrayPitchX * 4;
						}
						if (nB1pWorkTray == 2) {
							if (nB1pRow == 0)	dB1pX = m_pMoveData->dBtm1PickerX[2];
							else				dB1pX = m_pMoveData->dBtm1PickerX[3] + m_pEquipData->dTrayPitchX * 4;
						}

					} else {
						if (nB1pWorkTray == 1) {
							if (nB1pRow == 0) dB1pX = m_pMoveData->dBtm1PickerX[0];	//Stage1 L1 Position
							if (nB1pRow == 1) dB1pX = m_pMoveData->dBtm1PickerX[1];	//Stage1 L2 Position
						}
						if (nB1pWorkTray == 2) {
							if (nB1pRow == 0) dB1pX = m_pMoveData->dBtm1PickerX[2];	//Stage2 L1 Position
							if (nB1pRow == 1) dB1pX = m_pMoveData->dBtm1PickerX[3];	//Stage2 L2 Position
						}
					}
					g_objAJinAXL.Move_Absolute(AX_BTM1_PICKER_X, dB1pX);

					if (nB1pWorkTray == 1 && m_nAngleTray1Case == 20) {
						if (gData.bR04A || gData.bR13A)	dB1pTrayY = m_pMoveData->dAngleStage1Y[2+nB1pRow] + nB1pTrayPosY * m_pEquipData->dTrayPitchX;
						else		dB1pTrayY = m_pMoveData->dAngleStage1Y[2+nB1pRow] + nB1pTrayPosY * m_pEquipData->dTrayPitchY;
						g_objAJinAXL.Move_Absolute(AX_ANGLE_STAGE1_Y, dB1pTrayY);

						if ((gData.bR04B || gData.bR13B_5 || gData.bR13A) && m_nAngleTray2Case == 15) {
							if (nB1pRow == 0 && nB1pTrayPosY == 2) {
								m_bBtm1ModulePick = TRUE;
								m_nAngleTray2Case = 16;
							}
						}
					}
					if (nB1pWorkTray == 2 && m_nAngleTray2Case == 20) {
						if (gData.bR04A || gData.bR13A)	dB1pTrayY = m_pMoveData->dAngleStage2Y[2+nB1pRow] + nB1pTrayPosY * m_pEquipData->dTrayPitchX;
						else		dB1pTrayY = m_pMoveData->dAngleStage2Y[2+nB1pRow] + nB1pTrayPosY * m_pEquipData->dTrayPitchY;
						g_objAJinAXL.Move_Absolute(AX_ANGLE_STAGE2_Y, dB1pTrayY);

						if ((gData.bR04B || gData.bR13B_5 || gData.bR13A) && m_nAngleTray1Case == 15) {
							if (nB1pRow == 0 && nB1pTrayPosY == 2) {
								m_bBtm1ModulePick = TRUE;
								m_nAngleTray1Case = 16;
							}
						}
					}

					if (gData.bR04A) {
#ifdef EDITION_2ND
						m_pDY03->oBtm1PickerReadyL1 = FALSE; m_pDY03->oBtm1PickerTurnL1 = TRUE;
						m_pDY03->oBtm1PickerReadyL2 = FALSE; m_pDY03->oBtm1PickerTurnL2 = TRUE;
						m_pDY03->oBtm1PickerReadyL3 = FALSE; m_pDY03->oBtm1PickerTurnL3 = TRUE;
						m_pDY03->oBtm1PickerReadyL4 = FALSE; m_pDY03->oBtm1PickerTurnL4 = TRUE;
						m_pDY03->oBtm1PickerReadyL5 = FALSE; m_pDY03->oBtm1PickerTurnL5 = TRUE;
						m_pDY03->oBtm1PickerReadyL6 = FALSE; m_pDY03->oBtm1PickerTurnL6 = TRUE;
						m_pDY03->oBtm1PickerReadyL7 = FALSE; m_pDY03->oBtm1PickerTurnL7 = TRUE;
						m_pDY03->oBtm1PickerReadyL8 = FALSE; m_pDY03->oBtm1PickerTurnL8 = TRUE;
						m_pDY03->oBtm1PickerReadyL9 = FALSE; m_pDY03->oBtm1PickerTurnL9 = TRUE;
						m_pDY03->oBtm1PickerReadyL10 = FALSE; m_pDY03->oBtm1PickerTurnL10 = TRUE;
#else
						m_pDY03->oBtm1PickerTurn01 = TRUE;	m_pDY03->oBtm1PickerTurn06 = TRUE;
						m_pDY03->oBtm1PickerTurn23 = TRUE;	m_pDY03->oBtm1PickerTurn78 = TRUE;
						m_pDY03->oBtm1PickerTurn45 = TRUE;	m_pDY03->oBtm1PickerTurn910 = TRUE;
#endif
						g_objAJinAXL.Write_Output(3);
					}
					else {	// LJH A모델이 아니면 복귀하면서 Tray Pitch로 변환.
						g_objCommon.Move_Position(AX_BTM1_PICKER_P1, 0);
						g_objCommon.Move_Position(AX_BTM1_PICKER_P2, 0);
					}
					m_nBtm1PickCase++; m_tBtm1PickLoop.Set_LoopTime(10000);
				}
			}
		}
		break;
	case 26:	// Position Check
//		if (g_objAJinAXL.Is_MoveDone(AX_BTM1_PICKER_X, dB1pX) && g_objCommon.Get_Btm1PickerRotate()) {
		if (g_objCommon.Get_Btm1PickerRotate()) {
			if (gData.bR04A) {
				g_objCommon.Move_Position(AX_BTM1_PICKER_P1, 0);
				g_objCommon.Move_Position(AX_BTM1_PICKER_P2, 0);
			}
			m_nBtm1PickCase++; m_tBtm1PickLoop.Set_LoopTime(5000);
		}

	case 27:
		if (g_objAJinAXL.Is_MoveDone(AX_BTM1_PICKER_X, dB1pX) && g_objCommon.Check_Position(AX_BTM1_PICKER_P1, 0) && g_objCommon.Check_Position(AX_BTM1_PICKER_P2, 0)) {

			m_tBtm1PickLoop.Takt_Save(6, 8);
			m_strLog.Format("Btm1 Picker, %d", GetTickCount() - m_dwBtm1Pick);
			g_objLogFile.Save_TestLog(m_strLog);

			if (nB1pWorkTray == 0) { m_nBtm1PickCase = 0; m_tBtm1PickLoop.Set_LoopTime(5000); }
			else				   { m_nBtm1PickCase = 2; m_tBtm1PickLoop.Set_LoopTime(5000); }
		}
		break;


	case 80:	// Picker Turn (Rotate)
		if (gData.bR04A && g_objAJinAXL.Is_MoveDone(AX_BTM1_PICKER_P1, -2.5) && g_objAJinAXL.Is_MoveDone(AX_BTM1_PICKER_P2, -2.5)) {
#ifdef EDITION_2ND
			m_pDY03->oBtm1PickerReadyL1 = FALSE; m_pDY03->oBtm1PickerTurnL1 = TRUE;
			m_pDY03->oBtm1PickerReadyL2 = FALSE; m_pDY03->oBtm1PickerTurnL2 = TRUE;
			m_pDY03->oBtm1PickerReadyL3 = FALSE; m_pDY03->oBtm1PickerTurnL3 = TRUE;
			m_pDY03->oBtm1PickerReadyL4 = FALSE; m_pDY03->oBtm1PickerTurnL4 = TRUE;
			m_pDY03->oBtm1PickerReadyL5 = FALSE; m_pDY03->oBtm1PickerTurnL5 = TRUE;
			m_pDY03->oBtm1PickerReadyL6 = FALSE; m_pDY03->oBtm1PickerTurnL6 = TRUE;
			m_pDY03->oBtm1PickerReadyL7 = FALSE; m_pDY03->oBtm1PickerTurnL7 = TRUE;
			m_pDY03->oBtm1PickerReadyL8 = FALSE; m_pDY03->oBtm1PickerTurnL8 = TRUE;
			m_pDY03->oBtm1PickerReadyL9 = FALSE; m_pDY03->oBtm1PickerTurnL9 = TRUE;
			m_pDY03->oBtm1PickerReadyL10 = FALSE; m_pDY03->oBtm1PickerTurnL10 = TRUE;
#else
			m_pDY03->oBtm1PickerTurn01 = TRUE;	m_pDY03->oBtm1PickerTurn06 = TRUE;
			m_pDY03->oBtm1PickerTurn23 = TRUE;	m_pDY03->oBtm1PickerTurn78 = TRUE;
			m_pDY03->oBtm1PickerTurn45 = TRUE;	m_pDY03->oBtm1PickerTurn910 = TRUE;
#endif
			g_objAJinAXL.Write_Output(3);
			m_nBtm1PickCase++; m_tBtm1PickLoop.Set_LoopTime(5000);
		}
		break;
	case 81:	// Picker Turn (Ready)
		if (g_objCommon.Get_Btm1PickerRotate()) {
			if (!m_tBtm1PickLoop.Waiting_Time(300)) break;

#ifdef EDITION_2ND
			m_pDY03->oBtm1PickerReadyL1 = TRUE; m_pDY03->oBtm1PickerTurnL1 = FALSE;
			m_pDY03->oBtm1PickerReadyL2 = TRUE; m_pDY03->oBtm1PickerTurnL2 = FALSE;
			m_pDY03->oBtm1PickerReadyL3 = TRUE; m_pDY03->oBtm1PickerTurnL3 = FALSE;
			m_pDY03->oBtm1PickerReadyL4 = TRUE; m_pDY03->oBtm1PickerTurnL4 = FALSE;
			m_pDY03->oBtm1PickerReadyL5 = TRUE; m_pDY03->oBtm1PickerTurnL5 = FALSE;
			m_pDY03->oBtm1PickerReadyL6 = TRUE; m_pDY03->oBtm1PickerTurnL6 = FALSE;
			m_pDY03->oBtm1PickerReadyL7 = TRUE; m_pDY03->oBtm1PickerTurnL7 = FALSE;
			m_pDY03->oBtm1PickerReadyL8 = TRUE; m_pDY03->oBtm1PickerTurnL8 = FALSE;
			m_pDY03->oBtm1PickerReadyL9 = TRUE; m_pDY03->oBtm1PickerTurnL9 = FALSE;
			m_pDY03->oBtm1PickerReadyL10 = TRUE; m_pDY03->oBtm1PickerTurnL10 = FALSE;
#else
			m_pDY03->oBtm1PickerTurn01 = FALSE;	m_pDY03->oBtm1PickerTurn06 = FALSE;
			m_pDY03->oBtm1PickerTurn23 = FALSE;	m_pDY03->oBtm1PickerTurn78 = FALSE;
			m_pDY03->oBtm1PickerTurn45 = FALSE;	m_pDY03->oBtm1PickerTurn910 = FALSE;
#endif
			g_objAJinAXL.Write_Output(3);
			m_nBtm1PickCase = 21; m_tBtm1PickLoop.Set_LoopTime(5000);
		}
		break;
	}

	// 6. (Error : 3600)
	if (m_tBtm1PickLoop.Over_LoopTime()) {
		if (m_nBtm1PickCase == 6) { gData.nErrAngleStageNo = nB1pWorkTray; }

		if (m_nBtm1PickCase == 10) {
			if (!gData.bR04C && nB1pScanNo > 2) {
				Set_ScanError(3610, gData.nTNoBtm1Pick[nB1INo3-1], gData.nTNoBtm1Pick[nB1INo4-1], 0, 0, gData.nCNoBtm1Pick[nB1INo3-1], gData.nCNoBtm1Pick[nB1INo4-1]);
			}
			else {
				Set_ScanError(3610, gData.nTNoBtm1Pick[nB1INo1-1], gData.nTNoBtm1Pick[nB1INo2-1], gData.nTNoBtm1Pick[nB1INo3-1], gData.nTNoBtm1Pick[nB1INo4-1],
					                gData.nCNoBtm1Pick[nB1INo1-1], gData.nCNoBtm1Pick[nB1INo2-1], gData.nCNoBtm1Pick[nB1INo3-1], gData.nCNoBtm1Pick[nB1INo4-1]);
			}

		} else if (m_nBtm1PickCase == 21 && gData.bR04A && nErrRetry == 0) {	// R04A에서만 Retry 해야한다.
			nErrRetry = 1;
			m_nBtm1PickCase = 80; m_tBtm1PickLoop.Set_LoopTime(5000);
			return TRUE;
		}
		g_objCommon.Show_Error(3600 + m_nBtm1PickCase);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 7. (Error : 3700)
BOOL CSequenceMain::Inspection1_Run()
{
	static int		nIs1ScanNo = 0;
	static double	dIs1PosX = 0.0;
	static int		nI1JNo1, nI1JNo2, nI1JNo3, nI1JNo4 = 0;	// 검사할때 Jig 번호
	static int		nAlignCnt = 0;
	static BOOL		bTaktSave1 = FALSE;
	static BOOL		bStopOne1 = FALSE;


	

	switch (m_nInspect1Case) {
	case 0:		// Wait for Module Loading
		return TRUE;

	case 1:		// 안전확인 & Move to Align Position
		{
			BOOL bInspStart = FALSE;
			BOOL bInspWaitStart = FALSE;
			// 모두 다운
			if (!m_pDX05->iInspectStage2Up && m_pDX05->iInspectStage2Down && !m_pDX05->iInspectStage2Fwd && m_pDX05->iInspectStage2Bwd &&
				!m_pDX05->iInspectStage3Up && m_pDX05->iInspectStage3Down && !m_pDX05->iInspectStage3Fwd && m_pDX05->iInspectStage3Bwd) { bInspStart = TRUE; }

			// R04A/B T1 마지막 검사 ~ Unloading, Inspect2번이 언로드 위치면 Inspect3번이 Top2 첫번째 검사 시작위치 진입. R04C T2 ~ Unloading
// 			if ((!m_pDX05->iInspectStage2Up && m_pDX05->iInspectStage2Down && !m_pDX05->iInspectStage2Fwd && m_pDX05->iInspectStage2Bwd ||
// 				 (!gData.bR04C && g_objCommon.Check_Position(AX_INSPECT_STAGE2_X, 4))) && m_nInspect3Case > 7 && m_nInspect3Case <= 31) {
			if (m_nInspect3Case > 7 && m_nInspect3Case <= 31) {	

				double dCurPos = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE3_X);
				double dT2Pos = m_pMoveData->dInspectStage3X[3];	//T2 Pos
				if (!gData.bR04C) dT2Pos = m_pMoveData->dInspectStage3X[2] + m_pEquipData->dInspectPitchX * 4;	// R04A/B T1 Pos + Inspection Stage Pitch * 4
				double dUnPos = m_pMoveData->dInspectStage3X[4];	//Unload Pos
				if (dCurPos >= dT2Pos && dCurPos <= dUnPos) {	//위치 확인
					bInspStart = TRUE;
				} else {
					if (!gData.bR04C) dT2Pos = m_pMoveData->dInspectStage3X[2] + m_pEquipData->dInspectPitchX * 2 - 1;	// R04A/B T1 Pos + Inspection Stage Pitch * 4
					if (dCurPos >= dT2Pos && dCurPos <= dUnPos) {	//위치 확인
						bInspWaitStart = TRUE;
					}
				}
			}

			if (bInspStart) {
				// Interlock Alarm
				if (g_objCommon.Check_Position(AX_INSPECT_STAGE1_X, 0) && g_objCommon.Get_InfoInspectVacOn(1) &&
					m_pDX05->iInspectStage1Up && !m_pDX05->iInspectStage1Down) {

// 					if (!bStopOne1) { bStopOne1 = TRUE; return FALSE; }
// 					else bStopOne1 = FALSE;

					m_dwInspect1 = GetTickCount();
					m_tInspect1Loop.Takt_Start();
					g_objCommon.Move_Position(AX_INSPECT_STAGE1_X, 1);
					m_nInspect1Case++; m_tInspect1Loop.Set_LoopTime(5000);
				} else {
					g_objCommon.Show_Error(3700 + m_nInspect1Case);
					return FALSE;
				}				
			}
			if (bInspWaitStart) {
				// Interlock Alarm
				if (g_objCommon.Check_Position(AX_INSPECT_STAGE1_X, 0) && g_objCommon.Get_InfoInspectVacOn(1) &&
					m_pDX05->iInspectStage1Up && !m_pDX05->iInspectStage1Down) {

// 					if (!bStopOne1) { bStopOne1 = TRUE; return FALSE; }
// 					else bStopOne1 = FALSE;

					m_dwInspect1 = GetTickCount();
					m_tInspect1Loop.Takt_Start();
					g_objCommon.Move_Position(AX_INSPECT_STAGE1_X, 5);
					m_nInspect1Case = 85; m_tInspect1Loop.Set_LoopTime(5000);
				} else {
					g_objCommon.Show_Error(3700 + m_nInspect1Case);
					return FALSE;
				}				
			}
		}
		return TRUE;

	case 2:		// Align Z Axis Down
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE1_X, 1) && 
			g_objCommon.Check_Position(AX_MODULE_ALIGN_Y, 0) && g_objCommon.Check_Position(AX_MODULE_ALIGN_Z, 0)) {

			m_tInspect1Loop.Takt_Save(7, 1);
			m_tInspect1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_MODULE_ALIGN_Z, 1);
			m_nInspect1Case++; m_tInspect1Loop.Set_LoopTime(5000); 
		}
		break;
	case 3:		// Inspect Stage Vacuum Off
		if (g_objCommon.Check_Position(AX_MODULE_ALIGN_Z, 1)) {
			//g_objCommon.Set_InspectStage1VacOff(0);
			//dIs1PosX = m_pMoveData->dInspectStage1X[1] + m_pEquipData->dInspectALOffset;
			//g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE1_X, dIs1PosX, 0.25);	// X방향 얼라인. 속도는 1/4
			//m_nInspect1Case++; m_tInspect1Loop.Set_LoopTime(5000); 			
			m_nInspect1Case = 80; m_tInspect1Loop.Set_LoopTime(5000);	// Backward Offset
		}
		break;
	case 4:		// Align Y Axis Move to Side2 Position
		if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE1_X, dIs1PosX) && g_objCommon.Get_InfoInspectVacOn(1)) {
			if (nAlignCnt == 0) g_objCommon.Move_Position(AX_MODULE_ALIGN_Y, 1);	// Side 1
			if (nAlignCnt == 1) g_objCommon.Move_Position(AX_MODULE_ALIGN_Y, 2);	// Side 2
			m_nInspect1Case++; m_tInspect1Loop.Set_LoopTime(5000); 
		}
		break;
	case 5:		// Align Y Axis Move to Center Position
		if ((nAlignCnt == 0 && g_objCommon.Check_Position(AX_MODULE_ALIGN_Y, 1)) ||
			(nAlignCnt == 1 && g_objCommon.Check_Position(AX_MODULE_ALIGN_Y, 2))) {
			if (nAlignCnt == 0) {
				nAlignCnt++;
				m_nInspect1Case = 4; m_tInspect1Loop.Set_LoopTime(5000); 

			} else { 
				nAlignCnt = 0; 
				g_objCommon.Move_Position(AX_MODULE_ALIGN_Y, 0);
				m_nInspect1Case++; m_tInspect1Loop.Set_LoopTime(5000); 
			}
		}
		break;
	case 6:		// Align Z Axis Move to Ready Up Position
		if (g_objCommon.Check_Position(AX_MODULE_ALIGN_Y, 0)) {
			g_objCommon.Move_Position(AX_MODULE_ALIGN_Z, 0);
			m_nInspect1Case++; m_tInspect1Loop.Set_LoopTime(10000); 
		}
		break;		
	case 7:		// 안전확인 & Move to Top1 Position	// Mirror 실린더 확인
		if (g_objCommon.Check_Position(AX_MODULE_ALIGN_Z, 0) && g_objCommon.Check_Position(AX_TOP1_MIRROR_Z, 0) && g_objCommon.Get_InfoInspectVacOn(1)) {
			if (!bTaktSave1) { bTaktSave1 = TRUE; m_tInspect1Loop.Takt_Save(7, 2); }
		
#ifdef TYPE_R13	// Mirror 확인안해도 된다.
			if (1)
#else

			if(gData.bR04A && !m_pDX06->iTop1Mirror1Up && m_pDX06->iTop1Mirror1Down && !m_pDX06->iTop1Mirror2Up && m_pDX06->iTop1Mirror2Down )
			{
				m_pDY06->oTop1Mirror12Up = TRUE;
				m_pDY06->oTop1Mirror12Down = FALSE;
				g_objAJinAXL.Write_Output(6);
			}
			else if(!gData.bR04A && m_pDX06->iTop1Mirror1Up && !m_pDX06->iTop1Mirror1Down && m_pDX06->iTop1Mirror2Up && !m_pDX06->iTop1Mirror2Down)
			{
				m_pDY06->oTop1Mirror12Up = FALSE;
				m_pDY06->oTop1Mirror12Down = TRUE;
				g_objAJinAXL.Write_Output(6);
			}

			if ((gData.bR04A && m_pDX06->iTop1Mirror1Up && !m_pDX06->iTop1Mirror1Down && m_pDX06->iTop1Mirror2Up && !m_pDX06->iTop1Mirror2Down) ||
				(!gData.bR04A && !m_pDX06->iTop1Mirror1Up && m_pDX06->iTop1Mirror1Down && !m_pDX06->iTop1Mirror2Up && m_pDX06->iTop1Mirror2Down))
#endif
			{
				// 다운 상태이거나 Top2, Unload 위치에 있으면 이동
// 				if ((!m_pDX05->iInspectStage3Up && m_pDX05->iInspectStage3Down && !m_pDX05->iInspectStage3Fwd && m_pDX05->iInspectStage3Bwd) ||
// 					(m_nInspect3Case >= 30 && m_nInspect3Case < 50 && g_objCommon.Check_Position(AX_INSPECT_STAGE3_X, 4))) {

				if (!m_pDX05->iInspectStage3Up && m_pDX05->iInspectStage3Down && !m_pDX05->iInspectStage3Fwd && m_pDX05->iInspectStage3Bwd) 
				{	// 다운 상태
					m_tInspect1Loop.Takt_Start();
					dIs1PosX = m_pMoveData->dInspectStage1X[2];	// T1 Pos
					g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE1_X, dIs1PosX);
					nIs1ScanNo = 0;
					bTaktSave1 = FALSE;
					//m_nInspect1Case++; m_tInspect1Loop.Set_LoopTime(5000);
					m_nInspect1Case = 16; m_tInspect1Loop.Set_LoopTime(10000);

				} 
				else if (m_nInspect3Case > 17 && m_nInspect3Case < 50) 
				{	// Top2 ~ Unload 위치에 있을때
					double dCurPos = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE3_X);
					//double dT2Pos = m_pMoveData->dInspectStage3X[3];	//T2 Pos
					double dT2Pos = m_pMoveData->dInspectStage3X[2] + m_pEquipData->dInspectPitchX * 4 + 1;	// R04A/B T1 Pos + Inspection Stage Pitch * 4 + 1
					double dUnPos = m_pMoveData->dInspectStage3X[4];	//Unload Pos
					if (dCurPos > dT2Pos && dCurPos <= dUnPos) {	//위치 확인
						m_tInspect1Loop.Takt_Start();
						dIs1PosX = m_pMoveData->dInspectStage1X[2];	// T1 Pos
						g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE1_X, dIs1PosX);
						nIs1ScanNo = 0;
						bTaktSave1 = FALSE;
						//m_nInspect1Case++; m_tInspect1Loop.Set_LoopTime(5000);
						m_nInspect1Case = 16; m_tInspect1Loop.Set_LoopTime(10000);
					} else { m_tInspect1Loop.Set_LoopTime(5000); return TRUE; }

				} else { m_tInspect1Loop.Set_LoopTime(5000); return TRUE; }
			}
		}
		break;
	case 8:		// Send Load Complete
		if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE1_X, dIs1PosX) && g_objCommon.Check_Position(AX_VISION_TOP1_Z, 0) && g_objCommon.Get_InfoInspectVacOn(1)) {
			m_tInspect1Loop.Takt_Save(7, 3);
			if (m_pEquipData->bUseInspectTop1)
			{
				if (gData.bTop1MirrorErr[0])
				{ 
					nIs1ScanNo--; gData.bTop1MirrorErr[0] = FALSE; 
					g_objCommon.Show_Error(gData.nTop1MirrorErrNo);
					return FALSE;
				}
				Set_InspectJigNo(1, nIs1ScanNo, nI1JNo1, nI1JNo2, nI1JNo3, nI1JNo4);
				g_objInspector.Set_LoadComplete(INSPECTOR_PC2, "T1", gLot.sLotID[gData.nPNoInspect[0]-1], gData.nPNoInspect[0],
						gData.nTNoInspect[0][nI1JNo1-1], gData.nTNoInspect[0][nI1JNo2-1], 0, 0,
						gData.nCNoInspect[0][nI1JNo1-1], gData.nCNoInspect[0][nI1JNo2-1], 0, 0);
				
				m_nInspect1Case = 10; m_tInspect1Loop.Set_LoopTime(90000);
			} 
			else
			{
				m_nInspect1Case = 15; m_tInspect1Loop.Set_LoopTime(5000);
			}
			nIs1ScanNo++;
		}
		break;

	case 10:	// Wait for Top1 inspection(30sec)
		if (!m_pEquipData->bUseInspectTop1) {
			m_nInspect1Case = 15; m_tInspect1Loop.Set_LoopTime(5000);
		}
		break;

	case 11:	// Top1 Z Focus Move
		if (g_objAJinAXL.Is_Done(AX_VISION_TOP1_Z)) {
			m_tInspect1Loop.Takt_Start();
			g_objAJinAXL.Move_Absolute(AX_VISION_TOP1_Z, m_dTop1Z);
			m_nInspect1Case++; m_tInspect1Loop.Set_LoopTime(5000);
		}
		break;
	case 12:	// Send Move Complete
		if (g_objAJinAXL.Is_Done(AX_VISION_TOP1_Z)) {
			m_tInspect1Loop.Takt_Save(7, 4);
			CString strLog;
			double dEnc = g_objAJinAXL.Get_Position(AX_VISION_TOP1_Z);
			strLog.Format("%0.3lf,%0.3lf", m_dTop1Z, dEnc);
			g_objLogFile.Save_FocusAxisLog(AX_VISION_TOP1_Z, strLog);

			g_objInspector.Set_MoveComplete(INSPECTOR_PC2, "T1");
			m_nInspect1Case = 10; m_tInspect1Loop.Set_LoopTime(90000);	// 90초
		}
		break;

	case 13:	// Top1 Mirror Z Move
		if (g_objAJinAXL.Is_Done(AX_TOP1_MIRROR_Z) && g_objAJinAXL.Is_Done(AX_VISION_TOP1_Z)) {
			m_tInspect1Loop.Takt_Start();
			g_objAJinAXL.Move_Absolute(AX_VISION_TOP1_Z, m_dTop1Z);
			g_objCommon.Move_Position(AX_TOP1_MIRROR_Z, 1);
			m_nInspect1Case++; m_tInspect1Loop.Set_LoopTime(5000);
		}
		break;
	case 14:	// Send Move Complete
		if (g_objCommon.Check_Position(AX_TOP1_MIRROR_Z, 1) && g_objAJinAXL.Is_Done(AX_VISION_TOP1_Z)) {
			m_tInspect1Loop.Takt_Save(7, 5);
			g_objInspector.Set_SMoveComplete(INSPECTOR_PC2, "T1");
			m_nInspect1Case = 10; m_tInspect1Loop.Set_LoopTime(90000);	// 90초
		}
		break;

	case 15:	// Mirror Z축 안전 위치로 회피
		if (!m_pEquipData->bUseInspectTop1) { if(!m_tInspect1Loop.Waiting_Time(700)) break; }	// Delay

		g_objCommon.Move_Position(AX_VISION_TOP1_Z, 0);	// Ready Up
		g_objCommon.Move_Position(AX_TOP1_MIRROR_Z, 0);	// Ready Up
		m_nInspect1Case++; m_tInspect1Loop.Set_LoopTime(20000);		
		break;
	case 16:	// 다음 검사 위치 피치 이동
		if (g_objAJinAXL.Is_Done(AX_INSPECT_STAGE1_X) && g_objCommon.Check_Position(AX_VISION_TOP1_Z, 0) && g_objCommon.Check_Position(AX_TOP1_MIRROR_Z, 0)) {
			if (!bTaktSave1) { bTaktSave1 = TRUE; m_tInspect1Loop.Takt_Save(7, 14); }

			if (gData.bReload[2]) {
				nIs1ScanNo--; gData.bReload[2] = FALSE;
			}
			if (nIs1ScanNo > (gData.nBtmPickQt-1)) {	// 모델별로 피치이동 횟수가 다르다.
				nIs1ScanNo = 0;
				bTaktSave1 = FALSE;
				m_nInspect1Case++; m_tInspect1Loop.Set_LoopTime(10000);

			} else {
				double dCurPos = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE3_X);
				double dT2Pos = m_pMoveData->dInspectStage3X[3] + m_pEquipData->dInspectPitchX * gData.nBtmPickQt;	// R04A/B T2 Pos + Inspection Stage Pitch * 4
				if (gData.bR04C) dT2Pos = m_pMoveData->dInspectStage3X[3] + m_pEquipData->dInspectPitchX * gData.nBtmPickQt;
				double dUnPos = m_pMoveData->dInspectStage3X[4];	//Unload Pos

				// 첫번째 스캔이후는 앞의 스테이지가 Unload 위치거나 Down 상태일때 진행가능.
				if (g_objCommon.Check_Position(AX_INSPECT_STAGE3_X, 4) || (dCurPos >= dT2Pos && dCurPos <= dUnPos) ||
					(!m_pDX05->iInspectStage3Up && m_pDX05->iInspectStage3Down && !m_pDX05->iInspectStage3Fwd && m_pDX05->iInspectStage3Bwd)) {

					if (Select_Top1ScanPos(AUTO_INSPECTION1, nIs1ScanNo)) {
						m_tInspect1Loop.Takt_Start();
						dIs1PosX = m_pMoveData->dInspectStage1X[2] + nIs1ScanNo * m_pEquipData->dInspectPitchX;
						g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE1_X, dIs1PosX);
						m_nInspect1Case = 8; m_tInspect1Loop.Set_LoopTime(10000);
					} else {
						nIs1ScanNo++;
					}
				} else { m_tInspect1Loop.Set_LoopTime(10000); return TRUE; }
			}
		}
		break;
	case 17:	// 안전확인, Move to Top2 Position
		if (g_objAJinAXL.Is_Done(AX_INSPECT_STAGE1_X)) {
			// 다운 상태, Load, Unload 위치일때 진행
			if ((!m_pDX05->iInspectStage3Up && m_pDX05->iInspectStage3Down && !m_pDX05->iInspectStage3Fwd && m_pDX05->iInspectStage3Bwd) || 
				g_objCommon.Check_Position(AX_INSPECT_STAGE3_X, 0) || (!gData.bR04C && g_objCommon.Check_Position(AX_INSPECT_STAGE3_X, 4))) {
				m_tInspect1Loop.Takt_Start();
				dIs1PosX = m_pMoveData->dInspectStage1X[3];
				g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE1_X, dIs1PosX);
// 				g_objCommon.Move_Position(AX_VISION_TOP2_Z, 0);
// 				g_objCommon.Move_Position(AX_TOP2_SHIFT_Y, 0);
				nIs1ScanNo = 0;
				//m_nInspect1Case = 19; m_tInspect1Loop.Set_LoopTime(5000); 
				m_nInspect1Case = 25; m_tInspect1Loop.Set_LoopTime(10000);
			}
		}
		return TRUE;

	case 19:	// Send Load Complete
		if (g_objCommon.Get_InfoInspectVacOn(1) &&
			g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE1_X, dIs1PosX) &&
			g_objCommon.Check_Position(AX_VISION_TOP2_Z, 0) && g_objCommon.Check_Position(AX_TOP2_SHIFT_Y, 0)) {

			m_tInspect1Loop.Takt_Save(7, 6);
			if (m_pEquipData->bUseInspectTop2) {
				Set_InspectJigNo(2, nIs1ScanNo, nI1JNo1, nI1JNo2, nI1JNo3, nI1JNo4);
				g_objInspector.Set_LoadComplete(INSPECTOR_PC3, "T2", gLot.sLotID[gData.nPNoInspect[0]-1], gData.nPNoInspect[0], 
					gData.nTNoInspect[0][nI1JNo1-1], gData.nTNoInspect[0][nI1JNo2-1], 0, 0,
					gData.nCNoInspect[0][nI1JNo1-1], gData.nCNoInspect[0][nI1JNo2-1], 0, 0);
				m_nInspect1Case++; m_tInspect1Loop.Set_LoopTime(90000); 
			} else {
				m_nInspect1Case = 25; m_tInspect1Loop.Set_LoopTime(10000); 
			}
			nIs1ScanNo++;
		}
		break;

	case 20:	// Wait for Top2 inspection(30sec)
		if (!m_pEquipData->bUseInspectTop2) {
			m_nInspect1Case = 25; m_tInspect1Loop.Set_LoopTime(10000);
		}
		break;

	case 21:	// Top2 Z Focus Move
		if (g_objAJinAXL.Is_Done(AX_VISION_TOP2_Z)) {
			m_tInspect1Loop.Takt_Start();
			g_objAJinAXL.Move_Absolute(AX_VISION_TOP2_Z, m_dTop2Z);
			m_nInspect1Case++; m_tInspect1Loop.Set_LoopTime(5000);
		}
		break;
	case 22:	// Send Move Complete
		if (g_objAJinAXL.Is_Done(AX_VISION_TOP2_Z)) {
			m_tInspect1Loop.Takt_Save(7, 7);
			CString strLog;
			double dEnc = g_objAJinAXL.Get_Position(AX_VISION_TOP2_Z);
			strLog.Format("%0.3lf,%0.3lf", m_dTop2Z, dEnc);
			g_objLogFile.Save_FocusAxisLog(AX_VISION_TOP2_Z, strLog);

			g_objInspector.Set_MoveComplete(INSPECTOR_PC3, "T2");
			m_nInspect1Case = 20; m_tInspect1Loop.Set_LoopTime(90000);	// 90초
		}
		break;

	case 23:	// Top2 Shift X, Y Move
		if (g_objAJinAXL.Is_Done(AX_INSPECT_STAGE1_X) && g_objAJinAXL.Is_Done(AX_TOP2_SHIFT_Y)) {
			m_tInspect1Loop.Takt_Start();

			dIs1PosX = dIs1PosX + m_pEquipData->dInspectTop2ShiftX;

			g_objAJinAXL.Move_Absolute(AX_VISION_TOP2_Z, m_dTop2Z);
			g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE1_X, dIs1PosX);
			g_objCommon.Move_Position(AX_TOP2_SHIFT_Y, 1);
			m_nInspect1Case++; m_tInspect1Loop.Set_LoopTime(5000);
		}
		break;
	case 24:	// Send Move Complete
		if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE1_X, dIs1PosX) && g_objCommon.Check_Position(AX_TOP2_SHIFT_Y, 1) && g_objAJinAXL.Is_Done(AX_VISION_TOP2_Z)) {
			m_tInspect1Loop.Takt_Save(7, 8);
			g_objInspector.Set_SMoveComplete(INSPECTOR_PC3, "T2");
			m_nInspect1Case = 20; m_tInspect1Loop.Set_LoopTime(90000);	// 90초
		}
		break;

	case 25:	// 다음 검사 위치 피치 이동
		if (!m_pEquipData->bUseInspectTop2) { if(!m_tInspect1Loop.Waiting_Time(500)) break; }	// Delay

		if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE1_X, dIs1PosX) &&
			g_objAJinAXL.Is_Done(AX_VISION_TOP2_Z) && g_objAJinAXL.Is_Done(AX_TOP2_SHIFT_Y)) {

			if (!bTaktSave1) { bTaktSave1 = TRUE; m_tInspect1Loop.Takt_Save(7, 13); }

// 			if (gData.bInspetBWD) {
// 				if(!m_tInspect1Loop.Waiting_Time(2000)) break;
// 				gData.bInspetBWD = FALSE;
// 			}

			if (gData.bReload[3]) {
				nIs1ScanNo--; gData.bReload[3] = FALSE;
			}

			if (nIs1ScanNo > gData.nBtmPickQt-1) {	// 모델별로 피치이동 횟수가 다르다.
				m_tInspect1Loop.Takt_Start();
				nIs1ScanNo = 0;
				bTaktSave1 = FALSE;
				g_objCommon.Move_Position(AX_VISION_TOP2_Z, 0);	// Ready Up
				g_objCommon.Move_Position(AX_TOP2_SHIFT_Y, 0);	// Ready

				g_objCommon.Move_Position(AX_INSPECT_STAGE1_X, 4);	// Unload			
				m_nInspect1Case++; m_tInspect1Loop.Set_LoopTime(10000);
			} else {
				// 첫번째 스캔이후 앞의 스테이지는 다운상태 이거나 로드 위치에 있어야한다.
				if (nIs1ScanNo == 0 || (!m_pDX05->iInspectStage3Up && m_pDX05->iInspectStage3Down && !m_pDX05->iInspectStage3Fwd && m_pDX05->iInspectStage3Bwd) ||
					(g_objCommon.Check_Position(AX_INSPECT_STAGE3_X, 0) || g_objCommon.Check_Position(AX_INSPECT_STAGE3_X, 1, 5.0) || g_objCommon.Check_Position(AX_INSPECT_STAGE3_X, 5))) {
				
					m_tInspect1Loop.Takt_Start();
					if (Select_Top2ScanPos(AUTO_INSPECTION1, nIs1ScanNo)) {
						dIs1PosX = m_pMoveData->dInspectStage1X[3] + nIs1ScanNo * m_pEquipData->dInspectPitchX;

						g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE1_X, dIs1PosX);			
						g_objCommon.Move_Position(AX_VISION_TOP2_Z, 0);	// Ready Up
						g_objCommon.Move_Position(AX_TOP2_SHIFT_Y, 0);	// Ready
						m_nInspect1Case = 19; m_tInspect1Loop.Set_LoopTime(10000);
					} else {
						nIs1ScanNo++;
					}
				} else { m_tInspect1Loop.Set_LoopTime(10000); return TRUE; }
			}
		}
		break;
	case 26:	// Unload Position Check
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE1_X, 4)) {
			m_tInspect1Loop.Takt_Save(7, 9);
			m_nInspect1Case = 30; m_tInspect1Loop.Set_LoopTime(10000);
		}
		break;

	case 30:	// Wait for Unloading
		return TRUE;

	case 31:	// 안전확인, Stage Down
		//if (!g_objCommon.Check_Position(AX_INSPECT_STAGE2_X, 4) && g_objCommon.Check_Position(AX_INSPECT_STAGE3_X, 0)) {
		if (!g_objCommon.Check_Position(AX_INSPECT_STAGE3_X, 4)) {	// Unload 위치에 없으면 다운
			double dCurPos = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE3_X);
			double dT2Pos = m_pMoveData->dInspectStage3X[3];	//T2 Pos
			if (dCurPos <= dT2Pos) {	//위치 확인
				m_tInspect1Loop.Takt_Start();
				m_pDY05->oInspectStage1Up = FALSE;	m_pDY05->oInspectStage1Down = TRUE;
				g_objAJinAXL.Write_Output(5);
				m_nInspect1Case++; m_tInspect1Loop.Set_LoopTime(10000);
			}
		}
		return TRUE;
	case 32:	// Stage Down Check
		if (!m_pDX05->iInspectStage1Up && m_pDX05->iInspectStage1Down) {
			m_tInspect1Loop.Takt_Save(7, 10);
			m_nInspect1Case = 50; m_tInspect1Loop.Set_LoopTime(10000);
		}
		break;

	case 50:	// Wait 회피 위치, Stage가 모두 Up 상태면 진행.
		if ((m_nInspect2Case >= 0 && m_nInspect2Case <= 31 && m_pDX05->iInspectStage2Up && !m_pDX05->iInspectStage2Down) &&
			(m_nInspect3Case >= 0 && m_nInspect3Case <= 31 && m_pDX05->iInspectStage3Up && !m_pDX05->iInspectStage3Down)) {

			m_nInspect1Case++; m_tInspect1Loop.Set_LoopTime(10000);
		}
		return TRUE;

	case 51:	// Move to Load Position
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE1_X, 4)) {
			m_tInspect1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_INSPECT_STAGE1_X, 0);
			m_nInspect1Case++; m_tInspect1Loop.Set_LoopTime(10000);
		}
		break;
	case 52:	// Position Check
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE1_X, 0)) {
			m_tInspect1Loop.Takt_Save(7, 11);
			m_nInspect1Case = 70; m_tInspect1Loop.Set_LoopTime(10000);
		}
		break;

	case 70:	// Wait 회피 위치, Top1 Position 부터 진행한다.
		if (m_nInspect3Case < 80 && m_nInspect3Case > 7 && !g_objCommon.Check_Position(AX_INSPECT_STAGE3_X, 0)) {
			double dCurPos = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE3_X);
			double dT1Pos = m_pMoveData->dInspectStage3X[2];	//T1 Pos
			if (dCurPos >= dT1Pos) {	//위치 확인
				m_nInspect1Case++; m_tInspect1Loop.Set_LoopTime(10000);
			}
		}
		return TRUE;
	case 71:	// Stage Up
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE1_X, 0)) {
			m_tInspect1Loop.Takt_Start();
			m_pDY05->oInspectStage1Up = TRUE;	m_pDY05->oInspectStage1Down = FALSE;
			if (!Check_AngleTrayEmpty() || !Check_Btm1PickerEmpty()) {
				m_pDY05->oInspectStage1Vac01 = TRUE;
				m_pDY05->oInspectStage1Vac02 = TRUE;
				m_pDY05->oInspectStage1Vac03 = TRUE;
				m_pDY05->oInspectStage1Vac04 = TRUE;
				m_pDY05->oInspectStage1Vac05 = TRUE;
				m_pDY05->oInspectStage1Vac06 = TRUE;
				m_pDY05->oInspectStage1Vac07 = TRUE;
				m_pDY05->oInspectStage1Vac08 = TRUE;
				m_pDY05->oInspectStage1Vac09 = TRUE;
				m_pDY05->oInspectStage1Vac10 = TRUE;
			}
			g_objAJinAXL.Write_Output(5);
			m_nInspect1Case++; m_tInspect1Loop.Set_LoopTime(10000);
		}
		break;
	case 72:	// Stage Up Check
		if (m_pDX05->iInspectStage1Up && !m_pDX05->iInspectStage1Down) {
			if(m_nBtm1PickCase == 21 && !m_tInspect1Loop.Waiting_Time(200)) break;
			m_tInspect1Loop.Takt_Save(7, 12);
			m_strLog.Format("Inspection Stage1, %d", GetTickCount() - m_dwInspect1);
			g_objLogFile.Save_TestLog(m_strLog);
			m_nInspect1Case = 0; m_tInspect1Loop.Set_LoopTime(10000);
		}
		break;

	case 80:	// Inspect X Axis Move to Offset
		if (TRUE/*g_objCommon.Get_InspectStage1VacOff(0)*/) {
			dIs1PosX = m_pMoveData->dInspectStage1X[1] + m_pEquipData->dInspectALOffset;
			g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE1_X, dIs1PosX, 0.25);	// X방향 얼라인. 속도는 1/4
			m_nInspect1Case++; m_tInspect1Loop.Set_LoopTime(5000);
		}
		break;
	case 81:	// Move to Align Backward Offset
		if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE1_X, dIs1PosX)) {
			g_objCommon.Set_InfoInspectVacOn(1, 1);
			dIs1PosX = m_pMoveData->dInspectStage1X[1] + m_pEquipData->dInspectALOffset - m_pEquipData->dInspectALBackOffset;
			g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE1_X, dIs1PosX, 0.25);	// X방향 얼라인. 속도는 1/4
			m_nInspect1Case = 4; m_tInspect1Loop.Set_LoopTime(5000);
		}
		break;

	case 85:	// Move to Module Align
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE1_X, 5)) {
			double dCurPos = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE3_X);
			double dT2Pos = m_pMoveData->dInspectStage3X[3];	//T2 Pos
			if (!gData.bR04C) dT2Pos = m_pMoveData->dInspectStage3X[2] + m_pEquipData->dInspectPitchX * 4;	// R04A/B T1 Pos + Inspection Stage Pitch * 4
			double dUnPos = m_pMoveData->dInspectStage3X[4];	//Unload Pos
			if (dCurPos >= dT2Pos && dCurPos <= dUnPos) {	//위치 확인
				g_objCommon.Move_Position(AX_INSPECT_STAGE1_X, 1);
				m_nInspect1Case = 2; m_tInspect1Loop.Set_LoopTime(5000);
			} else {
				m_tInspect1Loop.Set_LoopTime(5000);
				return TRUE;
			}
		}
		break;
	}

	// 7. (Error : 3700)
	if (m_tInspect1Loop.Over_LoopTime()) {
		if (m_nInspect1Case == 10) {
			if (!gData.bR04C && nIs1ScanNo > 2) {
				Set_ScanError(3710, gData.nTNoInspect[0][nI1JNo1-1], gData.nTNoInspect[0][nI1JNo2-1], 0, 0, 
									gData.nCNoInspect[0][nI1JNo1-1], gData.nCNoInspect[0][nI1JNo2-1]);
			} else {
				Set_ScanError(3710, gData.nTNoInspect[0][nI1JNo1-1], gData.nTNoInspect[0][nI1JNo2-1], gData.nTNoInspect[0][nI1JNo3-1], gData.nTNoInspect[0][nI1JNo4-1],
					                gData.nCNoInspect[0][nI1JNo1-1], gData.nCNoInspect[0][nI1JNo2-1], gData.nCNoInspect[0][nI1JNo3-1], gData.nCNoInspect[0][nI1JNo4-1]);
			}

		} else if (m_nInspect1Case == 20) {
			Set_ScanError(3720, gData.nTNoInspect[0][nI1JNo1-1], gData.nTNoInspect[0][nI1JNo2-1], 0, 0,
								gData.nCNoInspect[0][nI1JNo1-1], gData.nCNoInspect[0][nI1JNo2-1]);
		}
		g_objCommon.Show_Error(3700 + m_nInspect1Case);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 8. (Error : 3800)
BOOL CSequenceMain::Inspection2_Run()
{
	static int		nIs2ScanNo = 0;
	static double	dIs2PosX = 0.0;
	static int		nI2JNo1 = 0, nI2JNo2 = 0, nI2JNo3 = 0, nI2JNo4 = 0;	// 검사할때 Jig 번호
	static int		nAlignCnt = 0;
	static BOOL		bTaktSave2 = FALSE;
	static BOOL		bStopOne2 = FALSE;

	switch (m_nInspect2Case) {
	case 0:		// Wait for Module Loading
		return TRUE;

	case 1:		// 안전확인 & Move to Align Position
		{
			BOOL bInspStart = FALSE;
			BOOL bInspWaitStart = FALSE;
			// 모두 다운
			if (!m_pDX05->iInspectStage1Up && m_pDX05->iInspectStage1Down &&
				!m_pDX05->iInspectStage3Up && m_pDX05->iInspectStage3Down && !m_pDX05->iInspectStage3Fwd && m_pDX05->iInspectStage3Bwd) { bInspStart = TRUE; }

			// R04A/B T1 마지막 검사 ~ Unloading, Inspect2번이 언로드 위치면 Inspect3번이 Top2 첫번째 검사 시작위치 진입. R04C T2 ~ Unloading
// 			if (((!m_pDX05->iInspectStage3Up && m_pDX05->iInspectStage3Down && !m_pDX05->iInspectStage3Fwd && m_pDX05->iInspectStage3Bwd)||
// 				(!gData.bR04C && g_objCommon.Check_Position(AX_INSPECT_STAGE3_X, 4))) && m_nInspect1Case > 7 && m_nInspect1Case <= 31) {
			if (m_nInspect1Case > 7 && m_nInspect1Case <= 31) {

				double dCurPos = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE1_X);
				double dT2Pos = m_pMoveData->dInspectStage1X[3];	//T2 Pos
				if (!gData.bR04C) dT2Pos = m_pMoveData->dInspectStage1X[2] + m_pEquipData->dInspectPitchX * 4;	// R04A/B T1 Pos + Inspection Stage Pitch * 4 
				double dUnPos = m_pMoveData->dInspectStage1X[4];	//Unload Pos
				if (dCurPos >= dT2Pos && dCurPos <= dUnPos) {	//위치 확인
					bInspStart = TRUE;
				} else {
					if (!gData.bR04C) dT2Pos = m_pMoveData->dInspectStage1X[2] + m_pEquipData->dInspectPitchX * 2 - 1;	// R04A/B T1 Pos + Inspection Stage Pitch * 2 
					if (dCurPos >= dT2Pos && dCurPos <= dUnPos) {	//위치 확인
						bInspWaitStart = TRUE;
					}
				}
			}

			if (bInspStart) {
				// Interlock Alarm
				if (g_objCommon.Check_Position(AX_INSPECT_STAGE2_X, 0) && g_objCommon.Get_InfoInspectVacOn(2) &&
					m_pDX05->iInspectStage2Up && !m_pDX05->iInspectStage2Down && m_pDX05->iInspectStage2Fwd && !m_pDX05->iInspectStage2Bwd) {

// 					if (!bStopOne2) { bStopOne2 = TRUE; return FALSE; }
// 					else bStopOne2 = FALSE;

					m_dwInspect2 = GetTickCount();
					m_tInspect2Loop.Takt_Start();
					g_objCommon.Move_Position(AX_INSPECT_STAGE2_X, 1);
					m_nInspect2Case++; m_tInspect2Loop.Set_LoopTime(5000);
				} else {
					g_objCommon.Show_Error(3800 + m_nInspect2Case);
					return FALSE;
				}
			}
			if (bInspWaitStart) {
				// Interlock Alarm
				if (g_objCommon.Check_Position(AX_INSPECT_STAGE2_X, 0) && g_objCommon.Get_InfoInspectVacOn(2) &&
					m_pDX05->iInspectStage2Up && !m_pDX05->iInspectStage2Down && m_pDX05->iInspectStage2Fwd && !m_pDX05->iInspectStage2Bwd) {

// 					if (!bStopOne2) { bStopOne2 = TRUE; return FALSE; }
// 					else bStopOne2 = FALSE;

					m_dwInspect2 = GetTickCount();
					m_tInspect2Loop.Takt_Start();
					g_objCommon.Move_Position(AX_INSPECT_STAGE2_X, 5);
					m_nInspect2Case = 85; m_tInspect2Loop.Set_LoopTime(5000);
				} else {
					g_objCommon.Show_Error(3800 + m_nInspect2Case);
					return FALSE;
				}
			}

		}
		return TRUE;

	case 2:		// Align Z Axis Down
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE2_X, 1) && 
			g_objCommon.Check_Position(AX_MODULE_ALIGN_Y, 0) && g_objCommon.Check_Position(AX_MODULE_ALIGN_Z, 0)) {

			m_tInspect2Loop.Takt_Save(8, 1);
			m_tInspect2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_MODULE_ALIGN_Z, 1);
			m_nInspect2Case++; m_tInspect2Loop.Set_LoopTime(5000); 
		}
		break;
	case 3:		// Inspect Stage Vacuum Off
		if (g_objCommon.Check_Position(AX_MODULE_ALIGN_Z, 1)) {
			//g_objCommon.Set_InspectStage2VacOff(0);
			//dIs2PosX = m_pMoveData->dInspectStage2X[1] + m_pEquipData->dInspectALOffset;
			//g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE2_X, dIs2PosX, 0.25);	// X방향 얼라인. 속도는 1/4
			//m_nInspect2Case++; m_tInspect2Loop.Set_LoopTime(5000); 
			m_nInspect2Case = 80; m_tInspect2Loop.Set_LoopTime(5000); 
		}
		break;
	case 4:		// Align Y Axis Move to Side1/2 Position
		if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE2_X, dIs2PosX) && g_objCommon.Get_InfoInspectVacOn(2)) {
			if (nAlignCnt == 0) g_objCommon.Move_Position(AX_MODULE_ALIGN_Y, 1);	// Side 1
			if (nAlignCnt == 1) g_objCommon.Move_Position(AX_MODULE_ALIGN_Y, 2);	// Side 2
			m_nInspect2Case++; m_tInspect2Loop.Set_LoopTime(5000); 
		}
		break;
	case 5:		// Align Y Axis Move to Center Position
		if ((nAlignCnt == 0 && g_objCommon.Check_Position(AX_MODULE_ALIGN_Y, 1)) ||
			(nAlignCnt == 1 && g_objCommon.Check_Position(AX_MODULE_ALIGN_Y, 2))) {
			if (nAlignCnt == 0) {
				nAlignCnt++;
				m_nInspect2Case = 4; m_tInspect2Loop.Set_LoopTime(5000); 

			} else { 
				nAlignCnt = 0; 
				g_objCommon.Move_Position(AX_MODULE_ALIGN_Y, 0);
				m_nInspect2Case++; m_tInspect2Loop.Set_LoopTime(5000); 
			}
		}
		break;
	case 6:		// Align Z Axis Move to Ready Up Position
		if (g_objCommon.Check_Position(AX_MODULE_ALIGN_Y, 0)) {
			g_objCommon.Move_Position(AX_MODULE_ALIGN_Z, 0);
			m_nInspect2Case++; m_tInspect2Loop.Set_LoopTime(10000); 
		}
		break;
	case 7:		// 안전확인 & Move to Top1 Position	// Mirror 실린더 확인
		if (g_objCommon.Check_Position(AX_MODULE_ALIGN_Z, 0) && g_objCommon.Check_Position(AX_TOP1_MIRROR_Z, 0) && g_objCommon.Get_InfoInspectVacOn(2)) {
			if (!bTaktSave2) { bTaktSave2 = TRUE; m_tInspect2Loop.Takt_Save(8, 2); }
			
#ifdef TYPE_R13
			if (1)
#else

			if(gData.bR04A && !m_pDX06->iTop1Mirror1Up && m_pDX06->iTop1Mirror1Down && !m_pDX06->iTop1Mirror2Up && m_pDX06->iTop1Mirror2Down )
			{
				m_pDY06->oTop1Mirror12Up = TRUE;
				m_pDY06->oTop1Mirror12Down = FALSE;
				g_objAJinAXL.Write_Output(6);
			}
			else if(!gData.bR04A && m_pDX06->iTop1Mirror1Up && !m_pDX06->iTop1Mirror1Down && m_pDX06->iTop1Mirror2Up && !m_pDX06->iTop1Mirror2Down)
			{
				m_pDY06->oTop1Mirror12Up = FALSE;
				m_pDY06->oTop1Mirror12Down = TRUE;
				g_objAJinAXL.Write_Output(6);
			}


			if ((gData.bR04A && m_pDX06->iTop1Mirror1Up && !m_pDX06->iTop1Mirror1Down && m_pDX06->iTop1Mirror2Up && !m_pDX06->iTop1Mirror2Down) ||
				(!gData.bR04A && !m_pDX06->iTop1Mirror1Up && m_pDX06->iTop1Mirror1Down && !m_pDX06->iTop1Mirror2Up && m_pDX06->iTop1Mirror2Down))
#endif
			{
				// 다운 상태이거나 Top2, Unload 위치에 있으면 이동
				if (!m_pDX05->iInspectStage1Up && m_pDX05->iInspectStage1Down) {
					m_tInspect2Loop.Takt_Start();
					dIs2PosX = m_pMoveData->dInspectStage2X[2];
					g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE2_X, dIs2PosX);
					nIs2ScanNo = 0;
					bTaktSave2 = FALSE;
					//m_nInspect2Case++; m_tInspect2Loop.Set_LoopTime(5000); 
					m_nInspect2Case = 16; m_tInspect2Loop.Set_LoopTime(10000);

				} else if (m_nInspect1Case > 17 && m_nInspect1Case < 50) {

					double dCurPos = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE1_X);
					//double dT2Pos = m_pMoveData->dInspectStage1X[3];	//T2 Pos
					double dT2Pos = m_pMoveData->dInspectStage1X[2] + m_pEquipData->dInspectPitchX * 4 + 1;	// R04A/B T1 Pos + Inspection Stage Pitch * 4 + 1
					double dUnPos = m_pMoveData->dInspectStage1X[4];	//Unload Pos
					if (dCurPos > dT2Pos && dCurPos <= dUnPos) {	//위치 확인
						m_tInspect2Loop.Takt_Start();
						dIs2PosX = m_pMoveData->dInspectStage2X[2];
						g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE2_X, dIs2PosX);
						nIs2ScanNo = 0;
						bTaktSave2 = FALSE;
						//m_nInspect2Case++; m_tInspect2Loop.Set_LoopTime(5000); 
						m_nInspect2Case = 16; m_tInspect2Loop.Set_LoopTime(10000);
					} else { m_tInspect2Loop.Set_LoopTime(10000); return TRUE; }

				} else { m_tInspect2Loop.Set_LoopTime(10000); return TRUE; }
			}
		}
		break;
	case 8:		// Send Load Complete
		if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE2_X, dIs2PosX) && g_objCommon.Check_Position(AX_VISION_TOP1_Z, 0) && g_objCommon.Get_InfoInspectVacOn(2)) {
			m_tInspect2Loop.Takt_Save(8, 3);
			if (m_pEquipData->bUseInspectTop1) {
				if (gData.bTop1MirrorErr[1]) { 
					nIs2ScanNo--; gData.bTop1MirrorErr[1] = FALSE; 
					g_objCommon.Show_Error(gData.nTop1MirrorErrNo);
					return FALSE;
				}
				Set_InspectJigNo(1, nIs2ScanNo, nI2JNo1, nI2JNo2, nI2JNo3, nI2JNo4);
				g_objInspector.Set_LoadComplete(INSPECTOR_PC2, "T1", gLot.sLotID[gData.nPNoInspect[1]-1], gData.nPNoInspect[1],
						gData.nTNoInspect[1][nI2JNo1-1], gData.nTNoInspect[1][nI2JNo2-1], 0, 0,
						gData.nCNoInspect[1][nI2JNo1-1], gData.nCNoInspect[1][nI2JNo2-1], 0, 0);
				
				m_nInspect2Case = 10; m_tInspect2Loop.Set_LoopTime(90000);
			} else {
				m_nInspect2Case = 15; m_tInspect2Loop.Set_LoopTime(5000);
			}
			nIs2ScanNo++;
		}
		break;

	case 10:	// Wait for Top1 inspection(30sec)
		if (!m_pEquipData->bUseInspectTop1) {
			m_nInspect2Case = 15; m_tInspect2Loop.Set_LoopTime(5000);
		}
		break;

	case 11:	// Top1 Z Focus Move
		if (g_objAJinAXL.Is_Done(AX_VISION_TOP1_Z)) {
			m_tInspect2Loop.Takt_Start();
			g_objAJinAXL.Move_Absolute(AX_VISION_TOP1_Z, m_dTop1Z);
			m_nInspect2Case++; m_tInspect2Loop.Set_LoopTime(5000);
		}
		break;
	case 12:	// Send Move Complete
		if (g_objAJinAXL.Is_Done(AX_VISION_TOP1_Z)) {
			m_tInspect2Loop.Takt_Save(8, 4);
			CString strLog;
			double dEnc = g_objAJinAXL.Get_Position(AX_VISION_TOP1_Z);
			strLog.Format("%0.3lf,%0.3lf", m_dTop1Z, dEnc);
			g_objLogFile.Save_FocusAxisLog(AX_VISION_TOP1_Z, strLog);

			g_objInspector.Set_MoveComplete(INSPECTOR_PC2, "T1");
			m_nInspect2Case = 10; m_tInspect2Loop.Set_LoopTime(90000);	// 90초
		}
		break;

	case 13:	// Top1 Mirror Z Move
		if (g_objAJinAXL.Is_Done(AX_TOP1_MIRROR_Z) && g_objAJinAXL.Is_Done(AX_VISION_TOP1_Z)) {
			m_tInspect2Loop.Takt_Start();
			g_objAJinAXL.Move_Absolute(AX_VISION_TOP1_Z, m_dTop1Z);
			g_objCommon.Move_Position(AX_TOP1_MIRROR_Z, 1);
			m_nInspect2Case++; m_tInspect2Loop.Set_LoopTime(5000);
		}
		break;
	case 14:	// Send Move Complete
		if (g_objCommon.Check_Position(AX_TOP1_MIRROR_Z, 1) && g_objAJinAXL.Is_Done(AX_VISION_TOP1_Z)) {
			m_tInspect2Loop.Takt_Save(8, 5);
			g_objInspector.Set_SMoveComplete(INSPECTOR_PC2, "T1");
			m_nInspect2Case = 10; m_tInspect2Loop.Set_LoopTime(90000);	// 90초
		}
		break;

	case 15:	// 다음 검사 위치 피치 이동
		if (!m_pEquipData->bUseInspectTop1) { if(!m_tInspect2Loop.Waiting_Time(700)) break; }	// Delay

		g_objCommon.Move_Position(AX_VISION_TOP1_Z, 0);	// Ready Up
		g_objCommon.Move_Position(AX_TOP1_MIRROR_Z, 0);	// Ready Up
		m_nInspect2Case++; m_tInspect2Loop.Set_LoopTime(20000);
		break;
	case 16:
		if (g_objAJinAXL.Is_Done(AX_INSPECT_STAGE2_X) && g_objCommon.Check_Position(AX_VISION_TOP1_Z, 0) && g_objCommon.Check_Position(AX_TOP1_MIRROR_Z, 0)) {
			if (!bTaktSave2) { bTaktSave2 = TRUE; m_tInspect2Loop.Takt_Save(8, 14); }

			if (gData.bReload[2]) {
				nIs2ScanNo--; gData.bReload[2] = FALSE;
			}
			if (nIs2ScanNo > (gData.nBtmPickQt-1)) {	// 모델별로 피치이동 횟수가 다르다.
				nIs2ScanNo = 0;
				bTaktSave2 = FALSE;
				m_nInspect2Case++; m_tInspect2Loop.Set_LoopTime(10000);

			} else {
				double dCurPos = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE1_X);
				double dT2Pos = m_pMoveData->dInspectStage1X[3] + m_pEquipData->dInspectPitchX * gData.nBtmPickQt;	// R04A/B T2 Pos + Inspection Stage Pitch * 4
				if (gData.bR04C) dT2Pos = m_pMoveData->dInspectStage1X[3] + m_pEquipData->dInspectPitchX * gData.nBtmPickQt;	// R04A/B T2 Pos + Inspection Stage Pitch * 4
				double dUnPos = m_pMoveData->dInspectStage1X[4];	//Unload Pos

				// 첫번째 스캔이후는 앞의 스테이지가 Unload 위치거나 Down 상태일때 진행가능.
				if (nIs2ScanNo < gData.nScanCnt || g_objCommon.Check_Position(AX_INSPECT_STAGE1_X, 4) || (dCurPos >= dT2Pos && dCurPos <= dUnPos) ||
					(!m_pDX05->iInspectStage1Up && m_pDX05->iInspectStage1Down)) {

					if (Select_Top1ScanPos(AUTO_INSPECTION2, nIs2ScanNo)) {
						m_tInspect2Loop.Takt_Start();
						dIs2PosX = m_pMoveData->dInspectStage2X[2] + nIs2ScanNo * m_pEquipData->dInspectPitchX;
						g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE2_X, dIs2PosX);
						m_nInspect2Case = 8; m_tInspect2Loop.Set_LoopTime(10000);
					} else {
						nIs2ScanNo++;
					}
				} else { m_tInspect2Loop.Set_LoopTime(10000); return TRUE;}
			}
		}
		break;
	case 17:	// 안전확인, Move to Top2 Position
		if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE2_X, dIs2PosX)) {
			// 다운 상태, Load, Unload 위치일때 진행
			if ((!m_pDX05->iInspectStage1Up && m_pDX05->iInspectStage1Down) ||
				g_objCommon.Check_Position(AX_INSPECT_STAGE1_X, 0) || (!gData.bR04C && g_objCommon.Check_Position(AX_INSPECT_STAGE1_X, 4))) {

				m_tInspect2Loop.Takt_Start();
				dIs2PosX = m_pMoveData->dInspectStage2X[3];
				g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE2_X, dIs2PosX);
// 				g_objCommon.Move_Position(AX_VISION_TOP2_Z, 0);
// 				g_objCommon.Move_Position(AX_TOP2_SHIFT_Y, 0);
				nIs2ScanNo = 0;
				//m_nInspect2Case = 19; m_tInspect2Loop.Set_LoopTime(5000); 
				m_nInspect2Case = 25; m_tInspect2Loop.Set_LoopTime(10000); 
			}
		}
		return TRUE;


	case 19:	// Send Load Complete
		if (g_objCommon.Get_InfoInspectVacOn(2) &&
			g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE2_X, dIs2PosX) &&
			g_objCommon.Check_Position(AX_VISION_TOP2_Z, 0) && g_objCommon.Check_Position(AX_TOP2_SHIFT_Y, 0)) {

			m_tInspect2Loop.Takt_Save(8, 6);
			if (m_pEquipData->bUseInspectTop2) {
				Set_InspectJigNo(2, nIs2ScanNo, nI2JNo1, nI2JNo2, nI2JNo3, nI2JNo4);
				g_objInspector.Set_LoadComplete(INSPECTOR_PC3, "T2", gLot.sLotID[gData.nPNoInspect[1]-1], gData.nPNoInspect[1], 
					gData.nTNoInspect[1][nI2JNo1-1], gData.nTNoInspect[1][nI2JNo2-1], 0, 0,
					gData.nCNoInspect[1][nI2JNo1-1], gData.nCNoInspect[1][nI2JNo2-1], 0, 0);
				m_nInspect2Case++; m_tInspect2Loop.Set_LoopTime(90000); 
			} else {
				m_nInspect2Case = 25; m_tInspect2Loop.Set_LoopTime(10000); 
			}
			nIs2ScanNo++;
		}
		break;

	case 20:	// Wait for Top2 inspection(30sec)
		if (!m_pEquipData->bUseInspectTop2) {
			m_nInspect2Case = 25; m_tInspect2Loop.Set_LoopTime(10000);
		}
		break;

	case 21:	// Top2 Z Focus Move
		if (g_objAJinAXL.Is_Done(AX_VISION_TOP2_Z)) {
			m_tInspect2Loop.Takt_Start();
			g_objAJinAXL.Move_Absolute(AX_VISION_TOP2_Z, m_dTop2Z);
			m_nInspect2Case++; m_tInspect2Loop.Set_LoopTime(5000);
		}
		break;
	case 22:	// Send Move Complete
		if (g_objAJinAXL.Is_Done(AX_VISION_TOP2_Z)) {
			m_tInspect2Loop.Takt_Save(8, 7);
			CString strLog;
			double dEnc = g_objAJinAXL.Get_Position(AX_VISION_TOP2_Z);
			strLog.Format("%0.3lf,%0.3lf", m_dTop2Z, dEnc);
			g_objLogFile.Save_FocusAxisLog(AX_VISION_TOP2_Z, strLog);
			g_objInspector.Set_MoveComplete(INSPECTOR_PC3, "T2");
			m_nInspect2Case = 20; m_tInspect2Loop.Set_LoopTime(90000);	// 90초
		}
		break;

	case 23:	// Top2 Shift Y Move
		if (g_objAJinAXL.Is_Done(AX_INSPECT_STAGE2_X) && g_objAJinAXL.Is_Done(AX_TOP2_SHIFT_Y) && g_objAJinAXL.Is_Done(AX_VISION_TOP2_Z)) {
			m_tInspect2Loop.Takt_Start();

			dIs2PosX = dIs2PosX + m_pEquipData->dInspectTop2ShiftX;

			g_objAJinAXL.Move_Absolute(AX_VISION_TOP2_Z, m_dTop2Z);
			g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE2_X, dIs2PosX);
			g_objCommon.Move_Position(AX_TOP2_SHIFT_Y, 1);
			m_nInspect2Case++; m_tInspect2Loop.Set_LoopTime(5000);
		}
		break;
	case 24:	// Send Move Complete
		if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE2_X, dIs2PosX) && g_objCommon.Check_Position(AX_TOP2_SHIFT_Y, 1) && g_objAJinAXL.Is_Done(AX_VISION_TOP2_Z)) {
			m_tInspect2Loop.Takt_Save(8, 8);
			g_objInspector.Set_SMoveComplete(INSPECTOR_PC3, "T2");
			m_nInspect2Case = 20; m_tInspect2Loop.Set_LoopTime(90000);	// 90초
		}
		break;

	case 25:	// 다음 검사 위치 피치 이동
		if (!m_pEquipData->bUseInspectTop2) { if(!m_tInspect2Loop.Waiting_Time(500)) break; }	// Delay

		if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE2_X, dIs2PosX) &&
			g_objAJinAXL.Is_Done(AX_VISION_TOP2_Z) && g_objAJinAXL.Is_Done(AX_TOP2_SHIFT_Y)) {

			if (!bTaktSave2) { bTaktSave2 = TRUE; m_tInspect2Loop.Takt_Save(8, 13); }

// 			if (gData.bInspetBWD) {
// 				if(!m_tInspect2Loop.Waiting_Time(2000)) break;
// 				gData.bInspetBWD = FALSE;
// 			}

			if (gData.bReload[3]) {
				nIs2ScanNo--; gData.bReload[3] = FALSE;
			}

			if (nIs2ScanNo > gData.nBtmPickQt-1) {	// 모델별로 피치이동 횟수가 다르다.
				m_tInspect2Loop.Takt_Start();
				nIs2ScanNo = 0;
				bTaktSave2 = FALSE;
				g_objCommon.Move_Position(AX_VISION_TOP2_Z, 0);	// Ready Up
				g_objCommon.Move_Position(AX_TOP2_SHIFT_Y, 0);	// Ready

				g_objCommon.Move_Position(AX_INSPECT_STAGE2_X, 4);	// Unload			
				m_nInspect2Case++; m_tInspect2Loop.Set_LoopTime(10000);
			} else {
				// 첫번째 스캔이후 앞의 스테이지는 다운상태 이거나 로드 위치에 있어야한다.
				if (nIs2ScanNo == 0 || (!m_pDX05->iInspectStage1Up && m_pDX05->iInspectStage1Down) ||
					(g_objCommon.Check_Position(AX_INSPECT_STAGE1_X, 0) || g_objCommon.Check_Position(AX_INSPECT_STAGE1_X, 1, 5.0) || g_objCommon.Check_Position(AX_INSPECT_STAGE1_X, 5))) {

					m_tInspect2Loop.Takt_Start();
					if (Select_Top2ScanPos(AUTO_INSPECTION2, nIs2ScanNo)) {
						dIs2PosX = m_pMoveData->dInspectStage2X[3] + nIs2ScanNo * m_pEquipData->dInspectPitchX;

						g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE2_X, dIs2PosX);			
						g_objCommon.Move_Position(AX_VISION_TOP2_Z, 0);	// Ready Up
						g_objCommon.Move_Position(AX_TOP2_SHIFT_Y, 0);	// Ready
						m_nInspect2Case = 19; m_tInspect2Loop.Set_LoopTime(10000);
					} else {
						nIs2ScanNo++;
					}
				} else { m_tInspect2Loop.Set_LoopTime(10000); return TRUE; }
			}
		}
		break;
	case 26:	// Unload Position Check
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE2_X, 4)) {
			m_tInspect2Loop.Takt_Save(8, 9);
			m_nInspect2Case = 30; m_tInspect2Loop.Set_LoopTime(10000);
		}
		break;

	case 30:	// Wait for Unloading
		return TRUE;

	case 31:	// 안전확인, Stage Down & Backward
		//if (g_objCommon.Check_Position(AX_INSPECT_STAGE1_X, 0) && !g_objCommon.Check_Position(AX_INSPECT_STAGE3_X, 4)) {
		if (!g_objCommon.Check_Position(AX_INSPECT_STAGE1_X, 4)) {	// Unload 위치에 없으면 다운
			double dCurPos = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE1_X);
			double dT2Pos = m_pMoveData->dInspectStage1X[3];	//T2 Pos
			if (dCurPos <= dT2Pos) {	//위치 확인
				m_tInspect2Loop.Takt_Start();
				m_pDY05->oInspectStage2Up = FALSE;	m_pDY05->oInspectStage2Down = TRUE;
				m_pDY05->oInspectStage2Fwd = FALSE;	m_pDY05->oInspectStage2Bwd = TRUE;
				g_objAJinAXL.Write_Output(5);
				gData.bInspetBWD = TRUE;
				m_nInspect2Case++; m_tInspect2Loop.Set_LoopTime(10000);
			}
		}
		return TRUE;
	case 32:	// Stage Down, Backward Check
		if (!m_pDX05->iInspectStage2Up && m_pDX05->iInspectStage2Down &&
			!m_pDX05->iInspectStage2Fwd && m_pDX05->iInspectStage2Bwd) {
			m_tInspect2Loop.Takt_Save(8, 10);
			m_nInspect2Case = 50; m_tInspect2Loop.Set_LoopTime(10000);
		}
		break;

	case 50:	// Wait 회피 위치, Stage가 모두 Up 상태면 진행.
		if ((m_nInspect1Case >= 0 && m_nInspect1Case <= 31 && m_pDX05->iInspectStage1Up && !m_pDX05->iInspectStage1Down) &&
			(m_nInspect3Case >= 0 && m_nInspect3Case <= 31 && m_pDX05->iInspectStage3Up && !m_pDX05->iInspectStage3Down)) {

			m_nInspect2Case++; m_tInspect2Loop.Set_LoopTime(10000);
		}
		return TRUE;

	case 51:	// Move to Load Position
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE2_X, 4)) {
			m_tInspect2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_INSPECT_STAGE2_X, 0);
			m_nInspect2Case++; m_tInspect2Loop.Set_LoopTime(10000);
		}
		break;
	case 52:	// Position Check
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE2_X, 0)) {
			m_tInspect2Loop.Takt_Save(8, 11);
			gData.bInspetBWD = FALSE;
			m_nInspect2Case = 70; m_tInspect2Loop.Set_LoopTime(10000);
		}
		break;

	case 70:	// Wait 회피 위치, Top1 Position 부터 진행한다.
		if (m_nInspect1Case < 80 && m_nInspect1Case > 7 && !g_objCommon.Check_Position(AX_INSPECT_STAGE1_X, 0)) {
			double dCurPos = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE1_X);
			double dT1Pos = m_pMoveData->dInspectStage1X[2];	//T1 Pos
			if (dCurPos >= dT1Pos) {	//위치 확인
				m_nInspect2Case++; m_tInspect2Loop.Set_LoopTime(10000);
			}
		}
		return TRUE;
	case 71:	// Stage Up
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE2_X, 0)) {
			m_tInspect2Loop.Takt_Start();
			m_pDY05->oInspectStage2Up = TRUE;	m_pDY05->oInspectStage2Down = FALSE;
			m_pDY05->oInspectStage2Fwd = TRUE;	m_pDY05->oInspectStage2Bwd = FALSE;
			if (!Check_AngleTrayEmpty() || !Check_Btm1PickerEmpty()) {
				m_pDY06->oInspectStage2Vac01 = TRUE;
				m_pDY06->oInspectStage2Vac02 = TRUE;
				m_pDY06->oInspectStage2Vac03 = TRUE;
				m_pDY06->oInspectStage2Vac04 = TRUE;
				m_pDY06->oInspectStage2Vac05 = TRUE;
				m_pDY06->oInspectStage2Vac06 = TRUE;
				m_pDY06->oInspectStage2Vac07 = TRUE;
				m_pDY06->oInspectStage2Vac08 = TRUE;
				m_pDY06->oInspectStage2Vac09 = TRUE;
				m_pDY06->oInspectStage2Vac10 = TRUE;
			}
			g_objAJinAXL.Write_Output(5);
			g_objAJinAXL.Write_Output(6);
			m_nInspect2Case++; m_tInspect2Loop.Set_LoopTime(10000);
		}
		break;
	case 72:	// Stage Up Check
		if (m_pDX05->iInspectStage2Up && !m_pDX05->iInspectStage2Down &&
			m_pDX05->iInspectStage2Fwd && !m_pDX05->iInspectStage2Bwd) {
				
			if(m_nBtm1PickCase == 21 && !m_tInspect2Loop.Waiting_Time(300)) break;
			m_tInspect2Loop.Takt_Save(8, 12);
			m_strLog.Format("Inspection Stage2, %d", GetTickCount() - m_dwInspect2);
			g_objLogFile.Save_TestLog(m_strLog);
			m_nInspect2Case = 0; m_tInspect2Loop.Set_LoopTime(10000);
		}
		break;

	case 80:	// Inspect X Axis Move to Align Offset
		if (TRUE/*g_objCommon.Get_InspectStage2VacOff(0)*/) {
			dIs2PosX = m_pMoveData->dInspectStage2X[1] + m_pEquipData->dInspectALOffset;
			g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE2_X, dIs2PosX, 0.25);	// X방향 얼라인. 속도는 1/4
			m_nInspect2Case++; m_tInspect2Loop.Set_LoopTime(5000); 
		}
		break;
	case 81:	// Move to Backward Offset
		if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE2_X, dIs2PosX)) {
			g_objCommon.Set_InfoInspectVacOn(2, 1);
			dIs2PosX = m_pMoveData->dInspectStage2X[1] + m_pEquipData->dInspectALOffset - m_pEquipData->dInspectALBackOffset;
			g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE2_X, dIs2PosX, 0.25);	// X방향 얼라인. 속도는 1/4
			m_nInspect2Case = 4; m_tInspect2Loop.Set_LoopTime(5000); 
		}
		break;

	case 85:	// Move to Module Align
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE2_X, 5)) {
			double dCurPos = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE1_X);
			double dT2Pos = m_pMoveData->dInspectStage1X[3];	//T2 Pos
			if (!gData.bR04C) dT2Pos = m_pMoveData->dInspectStage1X[2] + m_pEquipData->dInspectPitchX * 4;	// R04A/B T1 Pos + Inspection Stage Pitch * 4 
			double dUnPos = m_pMoveData->dInspectStage1X[4];	//Unload Pos
			if (dCurPos >= dT2Pos && dCurPos <= dUnPos) {	//위치 확인
				g_objCommon.Move_Position(AX_INSPECT_STAGE2_X, 1);
				m_nInspect2Case = 2; m_tInspect2Loop.Set_LoopTime(5000); 
			} else {
				m_tInspect2Loop.Set_LoopTime(5000);
				return TRUE;
			}
		}
		break;
	}

	// 8. (Error : 3800)
	if (m_tInspect2Loop.Over_LoopTime()) {
		if (m_nInspect2Case == 10) {
			if (!gData.bR04C && nIs2ScanNo > 2) {
				Set_ScanError(3810, gData.nTNoInspect[1][nI2JNo1-1], gData.nTNoInspect[1][nI2JNo2-1], 0, 0, 
									gData.nCNoInspect[1][nI2JNo1-1], gData.nCNoInspect[1][nI2JNo2-1]);
			} else {
				Set_ScanError(3810, gData.nTNoInspect[1][nI2JNo1-1], gData.nTNoInspect[1][nI2JNo2-1], gData.nTNoInspect[1][nI2JNo3-1], gData.nTNoInspect[1][nI2JNo4-1],
									gData.nCNoInspect[1][nI2JNo1-1], gData.nCNoInspect[1][nI2JNo2-1], gData.nCNoInspect[1][nI2JNo3-1], gData.nCNoInspect[1][nI2JNo4-1]);
			}

		} else if (m_nInspect2Case == 20) {
			Set_ScanError(3820, gData.nTNoInspect[1][nI2JNo1-1], gData.nTNoInspect[1][nI2JNo2-1], 0, 0,
								gData.nCNoInspect[1][nI2JNo1-1], gData.nCNoInspect[1][nI2JNo2-1]);
		}
		g_objCommon.Show_Error(3800 + m_nInspect2Case);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 9. (Error : 3900)
BOOL CSequenceMain::Inspection3_Run()
{
	static int		nIs3ScanNo = 0;
	static double	dIs3PosX = 0.0;
	static int		nI3JNo1 = 0, nI3JNo2 = 0, nI3JNo3 = 0, nI3JNo4 = 0;	// 검사할때 Jig 번호
	static int		nAlignCnt = 0;
	static BOOL		bTaktSave3 = FALSE;
	static BOOL		bStopOne3 = FALSE;

	switch (m_nInspect3Case) {
	case 0:		// Wait for Module Loading
		return TRUE;

	case 1:		// 안전확인 & Move to Align Position
		{
			BOOL bInspStart = FALSE;
			BOOL bInspWaitStart = FALSE;
			// 모두 다운
			if (!m_pDX05->iInspectStage1Up && m_pDX05->iInspectStage1Down &&
				!m_pDX05->iInspectStage2Up && m_pDX05->iInspectStage2Down && !m_pDX05->iInspectStage2Fwd && m_pDX05->iInspectStage2Bwd) { bInspStart = TRUE; }

			// R04A/B T1 마지막 검사 ~ Unloading, Inspect2번이 언로드 위치면 Inspect3번이 Top2 첫번째 검사 시작위치 진입. R04C T2 ~ Unloading
// 			if (((!m_pDX05->iInspectStage1Up && m_pDX05->iInspectStage1Down) || (!gData.bR04C && g_objCommon.Check_Position(AX_INSPECT_STAGE1_X, 4))) && 
// 				m_nInspect2Case > 7 && m_nInspect2Case <= 31) {
			if (m_nInspect2Case > 7 && m_nInspect2Case <= 31) {

				double dCurPos = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE2_X);
				double dT2Pos = m_pMoveData->dInspectStage2X[3];	//T2 Pos
				if (!gData.bR04C) dT2Pos = m_pMoveData->dInspectStage2X[2] + m_pEquipData->dInspectPitchX * 4;	// R04A/B T1 Pos + Inspection Stage Pitch * 4
				double dUnPos = m_pMoveData->dInspectStage2X[4];	//Unload Pos
				if (dCurPos >= dT2Pos && dCurPos <= dUnPos) {	//위치 확인
					bInspStart = TRUE;
				} else {
					if (!gData.bR04C) dT2Pos = m_pMoveData->dInspectStage2X[2] + m_pEquipData->dInspectPitchX * 2 - 1;	// R04A/B T1 Pos + Inspection Stage Pitch * 2
					if (dCurPos >= dT2Pos && dCurPos <= dUnPos) {	//위치 확인
						bInspWaitStart = TRUE;
					}
				}
			}

			if (bInspStart) {
				// Interlock Alarm
				if (g_objCommon.Check_Position(AX_INSPECT_STAGE3_X, 0) && g_objCommon.Get_InfoInspectVacOn(3) &&
					m_pDX05->iInspectStage3Up && !m_pDX05->iInspectStage3Down && m_pDX05->iInspectStage3Fwd && !m_pDX05->iInspectStage3Bwd) {

// 					if (!bStopOne3) { bStopOne3 = TRUE; return FALSE; }
// 					else bStopOne3 = FALSE;

					m_dwInspect3 = GetTickCount();
					m_tInspect3Loop.Takt_Start();
					g_objCommon.Move_Position(AX_INSPECT_STAGE3_X, 1);
					m_nInspect3Case++; m_tInspect3Loop.Set_LoopTime(5000);
				} else {
					g_objCommon.Show_Error(3900 + m_nInspect3Case);
					return FALSE;
				}
			}
			if (bInspWaitStart) {
				// Interlock Alarm
				if (g_objCommon.Check_Position(AX_INSPECT_STAGE3_X, 0) && g_objCommon.Get_InfoInspectVacOn(3) &&
					m_pDX05->iInspectStage3Up && !m_pDX05->iInspectStage3Down && m_pDX05->iInspectStage3Fwd && !m_pDX05->iInspectStage3Bwd) {

// 					if (!bStopOne3) { bStopOne3 = TRUE; return FALSE; }
// 					else bStopOne3 = FALSE;

					m_dwInspect3 = GetTickCount();
					m_tInspect3Loop.Takt_Start();
					g_objCommon.Move_Position(AX_INSPECT_STAGE3_X, 5);
					m_nInspect3Case = 85; m_tInspect3Loop.Set_LoopTime(5000);
				} else {
					g_objCommon.Show_Error(3900 + m_nInspect3Case);
					return FALSE;
				}
			}
		}
		return TRUE;

	case 2:		// Align Z Axis Down
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE3_X, 1) && 
			g_objCommon.Check_Position(AX_MODULE_ALIGN_Y, 0) && g_objCommon.Check_Position(AX_MODULE_ALIGN_Z, 0)) {

			m_tInspect3Loop.Takt_Save(9, 1);
			m_tInspect3Loop.Takt_Start();
			g_objCommon.Move_Position(AX_MODULE_ALIGN_Z, 1);
			m_nInspect3Case++; m_tInspect3Loop.Set_LoopTime(5000); 
		}
		break;
	case 3:		// Inspect X Axis Move to Align Offset
		if (g_objCommon.Check_Position(AX_MODULE_ALIGN_Z, 1)) {
			//g_objCommon.Set_InspectStage3VacOff(0);
			//dIs3PosX = m_pMoveData->dInspectStage3X[1] + m_pEquipData->dInspectALOffset;
			//g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE3_X, dIs3PosX, 0.25);	// X방향 얼라인. 속도는 1/4
			//m_nInspect3Case++; m_tInspect3Loop.Set_LoopTime(5000); 
			m_nInspect3Case = 80; m_tInspect3Loop.Set_LoopTime(5000);
		}
		break;
	case 4:		// Align Y Axis Move to Side1/2 Position
		if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE3_X, dIs3PosX) && g_objCommon.Get_InfoInspectVacOn(3)) {
			if (nAlignCnt == 0) g_objCommon.Move_Position(AX_MODULE_ALIGN_Y, 1);	// Side 1
			if (nAlignCnt == 1) g_objCommon.Move_Position(AX_MODULE_ALIGN_Y, 2);	// Side 2
			m_nInspect3Case++; m_tInspect3Loop.Set_LoopTime(5000); 
		}
		break;
	case 5:		// Align Y Axis Move to Center Position
		if ((nAlignCnt == 0 && g_objCommon.Check_Position(AX_MODULE_ALIGN_Y, 1)) ||
			(nAlignCnt == 1 && g_objCommon.Check_Position(AX_MODULE_ALIGN_Y, 2))) {
			if (nAlignCnt == 0) {
				nAlignCnt++;
				m_nInspect3Case = 4; m_tInspect3Loop.Set_LoopTime(5000); 

			} else { 
				nAlignCnt = 0; 
				g_objCommon.Move_Position(AX_MODULE_ALIGN_Y, 0);
				m_nInspect3Case++; m_tInspect3Loop.Set_LoopTime(5000); 
			}
		}
		break;
	case 6:		// Align Z Axis Move to Ready Up Position
		if (g_objCommon.Check_Position(AX_MODULE_ALIGN_Y, 0)) {
			g_objCommon.Move_Position(AX_MODULE_ALIGN_Z, 0);
			m_nInspect3Case++; m_tInspect3Loop.Set_LoopTime(10000); 
		}
		break;
	case 7:		// 안전확인 & Move to Top1 Position	// Mirror 실린더 확인
		if (g_objCommon.Check_Position(AX_MODULE_ALIGN_Z, 0) && g_objCommon.Check_Position(AX_TOP1_MIRROR_Z, 0) && g_objCommon.Get_InfoInspectVacOn(3)) {
			if (!bTaktSave3) { bTaktSave3 = TRUE; m_tInspect3Loop.Takt_Save(9, 2); }
			
#ifdef TYPE_R13
			if (1)
#else
			if(gData.bR04A && !m_pDX06->iTop1Mirror1Up && m_pDX06->iTop1Mirror1Down && !m_pDX06->iTop1Mirror2Up && m_pDX06->iTop1Mirror2Down )
			{
				m_pDY06->oTop1Mirror12Up = TRUE;
				m_pDY06->oTop1Mirror12Down = FALSE;
				g_objAJinAXL.Write_Output(6);
			}
			else if(!gData.bR04A && m_pDX06->iTop1Mirror1Up && !m_pDX06->iTop1Mirror1Down && m_pDX06->iTop1Mirror2Up && !m_pDX06->iTop1Mirror2Down)
			{
				m_pDY06->oTop1Mirror12Up = FALSE;
				m_pDY06->oTop1Mirror12Down = TRUE;
				g_objAJinAXL.Write_Output(6);
			}

			if ((gData.bR04A && m_pDX06->iTop1Mirror1Up && !m_pDX06->iTop1Mirror1Down && m_pDX06->iTop1Mirror2Up && !m_pDX06->iTop1Mirror2Down) ||
				(!gData.bR04A && !m_pDX06->iTop1Mirror1Up && m_pDX06->iTop1Mirror1Down && !m_pDX06->iTop1Mirror2Up && m_pDX06->iTop1Mirror2Down))
#endif
			{
				// 다운 상태이거나 Unload 위치에 있으면 이동
// 				if ((!m_pDX05->iInspectStage2Up && m_pDX05->iInspectStage2Down && !m_pDX05->iInspectStage2Fwd && m_pDX05->iInspectStage2Bwd) ||
// 					(m_nInspect2Case >= 30 && m_nInspect2Case < 50 && g_objCommon.Check_Position(AX_INSPECT_STAGE2_X, 4))) {

				if (!m_pDX05->iInspectStage2Up && m_pDX05->iInspectStage2Down && !m_pDX05->iInspectStage2Fwd && m_pDX05->iInspectStage2Bwd) {	// 다운 상태
					m_tInspect3Loop.Takt_Start();
					dIs3PosX = m_pMoveData->dInspectStage3X[2];
					g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE3_X, dIs3PosX);
					nIs3ScanNo = 0;
					bTaktSave3 = FALSE;
					//m_nInspect3Case++; m_tInspect3Loop.Set_LoopTime(5000); 
					m_nInspect3Case = 16; m_tInspect3Loop.Set_LoopTime(10000);

				} else if (m_nInspect2Case > 17 && m_nInspect2Case < 50) {	// Top2 ~ Unload 위치에 있을때
					double dCurPos = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE2_X);
					//double dT2Pos = m_pMoveData->dInspectStage2X[3];	//T2 Pos
					double dT2Pos = m_pMoveData->dInspectStage2X[2] + m_pEquipData->dInspectPitchX * 4 + 1;	// R04A/B T1 Pos + Inspection Stage Pitch * 4 + 1
					double dUnPos = m_pMoveData->dInspectStage2X[4];	//Unload Pos
					if (dCurPos > dT2Pos && dCurPos <= dUnPos) {	//위치 확인
						m_tInspect3Loop.Takt_Start();
						dIs3PosX = m_pMoveData->dInspectStage3X[2];
						g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE3_X, dIs3PosX);
						nIs3ScanNo = 0;
						bTaktSave3 = FALSE;
						//m_nInspect3Case++; m_tInspect3Loop.Set_LoopTime(5000); 
						m_nInspect3Case = 16; m_tInspect3Loop.Set_LoopTime(10000);
					} else { m_tInspect3Loop.Set_LoopTime(10000); return TRUE; }

				} else { m_tInspect3Loop.Set_LoopTime(10000); return TRUE; }
			}
		}
		break;
	case 8:		// Send Load Complete
		if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE3_X, dIs3PosX) && g_objCommon.Check_Position(AX_VISION_TOP1_Z, 0) && g_objCommon.Get_InfoInspectVacOn(3)) {
			m_tInspect3Loop.Takt_Save(9, 3);
			if (m_pEquipData->bUseInspectTop1) {
				if (gData.bTop1MirrorErr[2]) {
					nIs3ScanNo--; gData.bTop1MirrorErr[2] = FALSE; 
					g_objCommon.Show_Error(gData.nTop1MirrorErrNo);
					return FALSE;
				}
				Set_InspectJigNo(1, nIs3ScanNo, nI3JNo1, nI3JNo2, nI3JNo3, nI3JNo4);
				g_objInspector.Set_LoadComplete(INSPECTOR_PC2, "T1", gLot.sLotID[gData.nPNoInspect[2]-1], gData.nPNoInspect[2],
						gData.nTNoInspect[2][nI3JNo1-1], gData.nTNoInspect[2][nI3JNo2-1], 0, 0,
						gData.nCNoInspect[2][nI3JNo1-1], gData.nCNoInspect[2][nI3JNo2-1], 0, 0);
				
				m_nInspect3Case = 10; m_tInspect3Loop.Set_LoopTime(90000);
			} else {
				m_nInspect3Case = 15; m_tInspect3Loop.Set_LoopTime(5000);
			}
			nIs3ScanNo++;
		}
		break;

	case 10:	// Wait for Top1 inspection(30sec)
		if (!m_pEquipData->bUseInspectTop1) {
			m_nInspect3Case = 15; m_tInspect3Loop.Set_LoopTime(5000);
		}
		break;

	case 11:	// Top1 Z Focus Move
		if (g_objAJinAXL.Is_Done(AX_VISION_TOP1_Z)) {
			m_tInspect3Loop.Takt_Start();
			g_objAJinAXL.Move_Absolute(AX_VISION_TOP1_Z, m_dTop1Z);
			m_nInspect3Case++; m_tInspect3Loop.Set_LoopTime(5000);
		}
		break;
	case 12:	// Send Move Complete
		if (g_objAJinAXL.Is_Done(AX_VISION_TOP1_Z)) {
			m_tInspect3Loop.Takt_Save(9, 4);
			CString strLog;
			double dEnc = g_objAJinAXL.Get_Position(AX_VISION_TOP1_Z);
			strLog.Format("%0.3lf,%0.3lf", m_dTop1Z, dEnc);
			g_objLogFile.Save_FocusAxisLog(AX_VISION_TOP1_Z, strLog);

			g_objInspector.Set_MoveComplete(INSPECTOR_PC2, "T1");
			m_nInspect3Case = 10; m_tInspect3Loop.Set_LoopTime(90000);	// 90초
		}
		break;

	case 13:	// Top1 Mirror Z Move
		if (g_objAJinAXL.Is_Done(AX_TOP1_MIRROR_Z) && g_objAJinAXL.Is_Done(AX_VISION_TOP1_Z)) {
			m_tInspect3Loop.Takt_Start();
			g_objAJinAXL.Move_Absolute(AX_VISION_TOP1_Z, m_dTop1Z);
			g_objCommon.Move_Position(AX_TOP1_MIRROR_Z, 1);
			m_nInspect3Case++; m_tInspect3Loop.Set_LoopTime(5000);
		}
		break;
	case 14:	// Send Move Complete
		if (g_objCommon.Check_Position(AX_TOP1_MIRROR_Z, 1) && g_objAJinAXL.Is_Done(AX_VISION_TOP1_Z)) {
			m_tInspect3Loop.Takt_Save(9, 5);
			g_objInspector.Set_SMoveComplete(INSPECTOR_PC2, "T1");
			m_nInspect3Case = 10; m_tInspect3Loop.Set_LoopTime(90000);	// 90초
		}
		break;

	case 15:	// Mirror Z축 회피위치 이동.
		if (!m_pEquipData->bUseInspectTop1) { if(!m_tInspect3Loop.Waiting_Time(700)) break; }	// Delay

		g_objCommon.Move_Position(AX_VISION_TOP1_Z, 0);	// Ready Up
		g_objCommon.Move_Position(AX_TOP1_MIRROR_Z, 0);	// Ready Up
		m_nInspect3Case++; m_tInspect3Loop.Set_LoopTime(20000);
		break;
	case 16:	// 다음 검사 위치 피치 이동
		if (g_objAJinAXL.Is_Done(AX_INSPECT_STAGE3_X) && g_objCommon.Check_Position(AX_VISION_TOP1_Z, 0) && g_objCommon.Check_Position(AX_TOP1_MIRROR_Z, 0)) {
			if (!bTaktSave3) { bTaktSave3 = TRUE; m_tInspect3Loop.Takt_Save(9, 14); }

			if (gData.bReload[2]) {
				nIs3ScanNo--; gData.bReload[2] = FALSE;
			}
			if (nIs3ScanNo > (gData.nBtmPickQt-1)) {	// 모델별로 피치이동 횟수가 다르다.
				nIs3ScanNo = 0;
				bTaktSave3 = FALSE;
				m_nInspect3Case++; m_tInspect3Loop.Set_LoopTime(10000);

			} else {
				double dCurPos = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE2_X);
				double dT2Pos = m_pMoveData->dInspectStage2X[3] + m_pEquipData->dInspectPitchX * gData.nBtmPickQt;	// R04A/B T2 Pos + Inspection Stage Pitch * 4
				if (gData.bR04C) dT2Pos = m_pMoveData->dInspectStage2X[3] + m_pEquipData->dInspectPitchX * gData.nBtmPickQt;
				double dUnPos = m_pMoveData->dInspectStage2X[4];	//Unload Pos

				// 첫번째 스캔이후는 앞의 스테이지가 Unload 위치거나 Down 상태일때 진행가능.
				if (nIs3ScanNo < gData.nScanCnt || g_objCommon.Check_Position(AX_INSPECT_STAGE2_X, 4) || (dCurPos >= dT2Pos && dCurPos <= dUnPos) ||
					(!m_pDX05->iInspectStage2Up && m_pDX05->iInspectStage2Down && !m_pDX05->iInspectStage2Fwd && m_pDX05->iInspectStage2Bwd)) {

					if (Select_Top1ScanPos(AUTO_INSPECTION3, nIs3ScanNo)) {
						m_tInspect3Loop.Takt_Start();
						dIs3PosX = m_pMoveData->dInspectStage3X[2] + nIs3ScanNo *  m_pEquipData->dInspectPitchX;
						g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE3_X, dIs3PosX);			
						m_nInspect3Case = 8; m_tInspect3Loop.Set_LoopTime(10000);
					} else {
						nIs3ScanNo++;
					}
				} else { m_tInspect3Loop.Set_LoopTime(10000); return TRUE;}
			}
		}
		break;
	case 17:	// 안전확인, Move to Top2 Position
		if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE3_X, dIs3PosX)) {
			// 다운 상태, Load, Unload 위치일때 진행, R04C는 검사 시작위치가 4번부터 하기 때문에 앞 Stage가 Up된 상태로 Unload에 있으면 충돌 발생.
			if ((!m_pDX05->iInspectStage2Up && m_pDX05->iInspectStage2Down && !m_pDX05->iInspectStage2Fwd && m_pDX05->iInspectStage2Bwd) ||
				g_objCommon.Check_Position(AX_INSPECT_STAGE2_X, 0) || (!gData.bR04C && g_objCommon.Check_Position(AX_INSPECT_STAGE2_X, 4))) {
				m_tInspect3Loop.Takt_Start();
				dIs3PosX = m_pMoveData->dInspectStage3X[3];
				g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE3_X, dIs3PosX);
// 				g_objCommon.Move_Position(AX_VISION_TOP2_Z, 0);
// 				g_objCommon.Move_Position(AX_TOP2_SHIFT_Y, 0);
				nIs3ScanNo = 0;
				//m_nInspect3Case = 19; m_tInspect3Loop.Set_LoopTime(5000); 
				m_nInspect3Case = 25; m_tInspect3Loop.Set_LoopTime(10000); 
			}
		}
		return TRUE;


	case 19:	// Send Load Complete
		if (g_objCommon.Get_InfoInspectVacOn(3) &&
			g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE3_X, dIs3PosX) &&
			g_objCommon.Check_Position(AX_VISION_TOP2_Z, 0) && g_objCommon.Check_Position(AX_TOP2_SHIFT_Y, 0)) {

			m_tInspect3Loop.Takt_Save(9, 6);
			if (m_pEquipData->bUseInspectTop2) {
				Set_InspectJigNo(2, nIs3ScanNo, nI3JNo1, nI3JNo2, nI3JNo3, nI3JNo4);
				g_objInspector.Set_LoadComplete(INSPECTOR_PC3, "T2", gLot.sLotID[gData.nPNoInspect[2]-1], gData.nPNoInspect[2], 
					gData.nTNoInspect[2][nI3JNo1-1], gData.nTNoInspect[2][nI3JNo2-1], 0, 0,
					gData.nCNoInspect[2][nI3JNo1-1], gData.nCNoInspect[2][nI3JNo2-1], 0, 0);
				m_nInspect3Case++; m_tInspect3Loop.Set_LoopTime(90000); 
			} else {
				m_nInspect3Case = 25; m_tInspect3Loop.Set_LoopTime(10000); 
			}
			nIs3ScanNo++;
		}
		break;

	case 20:	// Wait for Top2 inspection(30sec)
		if (!m_pEquipData->bUseInspectTop2) {
			m_nInspect3Case = 25; m_tInspect3Loop.Set_LoopTime(10000);
		}
		break;

	case 21:	// Top2 Z Focus Move
		if (g_objAJinAXL.Is_Done(AX_VISION_TOP2_Z)) {
			m_tInspect3Loop.Takt_Start();
			g_objAJinAXL.Move_Absolute(AX_VISION_TOP2_Z, m_dTop2Z);
			m_nInspect3Case++; m_tInspect3Loop.Set_LoopTime(5000);
		}
		break;
	case 22:	// Send Move Complete
		if (g_objAJinAXL.Is_Done(AX_VISION_TOP2_Z)) {
			m_tInspect3Loop.Takt_Save(9, 7);
			CString strLog;
			double dEnc = g_objAJinAXL.Get_Position(AX_VISION_TOP2_Z);
			strLog.Format("%0.3lf,%0.3lf", m_dTop2Z, dEnc);
			g_objLogFile.Save_FocusAxisLog(AX_VISION_TOP2_Z, strLog);

			g_objInspector.Set_MoveComplete(INSPECTOR_PC3, "T2");
			m_nInspect3Case = 20; m_tInspect3Loop.Set_LoopTime(90000);	// 90초
		}
		break;

	case 23:	// Top2 Shift Y Move
		if (g_objAJinAXL.Is_Done(AX_INSPECT_STAGE3_X) && g_objAJinAXL.Is_Done(AX_TOP2_SHIFT_Y) && g_objAJinAXL.Is_Done(AX_VISION_TOP2_Z)) {
			m_tInspect3Loop.Takt_Start();

			dIs3PosX = dIs3PosX + m_pEquipData->dInspectTop2ShiftX;

			g_objAJinAXL.Move_Absolute(AX_VISION_TOP2_Z, m_dTop2Z);
			g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE3_X, dIs3PosX);
			g_objCommon.Move_Position(AX_TOP2_SHIFT_Y, 1);
			m_nInspect3Case++; m_tInspect3Loop.Set_LoopTime(5000);
		}
		break;
	case 24:	// Send Move Complete
		if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE3_X, dIs3PosX) && g_objCommon.Check_Position(AX_TOP2_SHIFT_Y, 1) && g_objAJinAXL.Is_Done(AX_VISION_TOP2_Z)) {
			m_tInspect3Loop.Takt_Save(9, 8);
			g_objInspector.Set_SMoveComplete(INSPECTOR_PC3, "T2");
			m_nInspect3Case = 20; m_tInspect3Loop.Set_LoopTime(90000);	// 90초
		}
		break;

	case 25:	// 다음 검사 위치 피치 이동
		if (!m_pEquipData->bUseInspectTop2) { if(!m_tInspect3Loop.Waiting_Time(500)) break; }	// Delay

		if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE3_X, dIs3PosX) &&
			g_objAJinAXL.Is_Done(AX_VISION_TOP2_Z) && g_objAJinAXL.Is_Done(AX_TOP2_SHIFT_Y)) {

			if (!bTaktSave3) { bTaktSave3 = TRUE; m_tInspect3Loop.Takt_Save(9, 13); }

// 			if (gData.bInspetBWD) {
// 				if(!m_tInspect3Loop.Waiting_Time(2000)) break;
// 				gData.bInspetBWD = FALSE;
// 			}

			if (gData.bReload[3]) {
				nIs3ScanNo--; gData.bReload[3] = FALSE;
			}

			if (nIs3ScanNo > gData.nBtmPickQt-1) {	// 모델별로 피치이동 횟수가 다르다.
				m_tInspect3Loop.Takt_Start();
				nIs3ScanNo = 0;
				bTaktSave3 = FALSE;
				g_objCommon.Move_Position(AX_VISION_TOP2_Z, 0);	// Ready Up
				g_objCommon.Move_Position(AX_TOP2_SHIFT_Y, 0);	// Ready

				g_objCommon.Move_Position(AX_INSPECT_STAGE3_X, 4);	// Unload			
				m_nInspect3Case++; m_tInspect3Loop.Set_LoopTime(10000);
			} else {
				// 첫번째 스캔이후 앞의 스테이지는 다운상태 이거나 로드 위치에 있어야한다.
				if (nIs3ScanNo == 0 || (!m_pDX05->iInspectStage2Up && m_pDX05->iInspectStage2Down && !m_pDX05->iInspectStage2Fwd && m_pDX05->iInspectStage2Bwd) ||
					(g_objCommon.Check_Position(AX_INSPECT_STAGE2_X, 0) || g_objCommon.Check_Position(AX_INSPECT_STAGE2_X, 1, 5.0) || g_objCommon.Check_Position(AX_INSPECT_STAGE2_X, 5))) {

					m_tInspect3Loop.Takt_Start();
					if (Select_Top2ScanPos(AUTO_INSPECTION3, nIs3ScanNo)) {
						dIs3PosX = m_pMoveData->dInspectStage3X[3] + nIs3ScanNo * m_pEquipData->dInspectPitchX;

						g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE3_X, dIs3PosX);			
						g_objCommon.Move_Position(AX_VISION_TOP2_Z, 0);	// Ready Up
						g_objCommon.Move_Position(AX_TOP2_SHIFT_Y, 0);	// Ready
						m_nInspect3Case = 19; m_tInspect3Loop.Set_LoopTime(10000);
					} else {
						nIs3ScanNo++;
					}
				} else { m_tInspect3Loop.Set_LoopTime(10000); return TRUE; }
			}
		}
		break;
	case 26:	// Unload Position Check
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE3_X, 4)) {
			m_tInspect3Loop.Takt_Save(9, 9);
			m_nInspect3Case = 30; m_tInspect3Loop.Set_LoopTime(10000);
		}
		break;

	case 30:	// Wait for Unloading
		return TRUE;

	case 31:	// 안전확인, Stage Down & Backward
		//if (!g_objCommon.Check_Position(AX_INSPECT_STAGE1_X, 4) && g_objCommon.Check_Position(AX_INSPECT_STAGE2_X, 0)) {
		if (!g_objCommon.Check_Position(AX_INSPECT_STAGE2_X, 4)) {	// Unload 위치에 없으면 다운
			double dCurPos = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE2_X);
			double dT2Pos = m_pMoveData->dInspectStage2X[3];	//T2 Pos
			if (dCurPos <= dT2Pos) {	//위치 확인
				m_tInspect3Loop.Takt_Start();
				m_pDY05->oInspectStage3Up = FALSE;	m_pDY05->oInspectStage3Down = TRUE;
				m_pDY05->oInspectStage3Fwd = FALSE;	m_pDY05->oInspectStage3Bwd = TRUE;
				g_objAJinAXL.Write_Output(5);
				gData.bInspetBWD = TRUE;
				m_nInspect3Case++; m_tInspect3Loop.Set_LoopTime(10000);
			}
		}
		return TRUE;
	case 32:	// Stage Down, Backward Check
		if (!m_pDX05->iInspectStage3Up  && m_pDX05->iInspectStage3Down &&
			!m_pDX05->iInspectStage3Fwd && m_pDX05->iInspectStage3Bwd) {
				m_tInspect3Loop.Takt_Save(9, 10);
				m_nInspect3Case = 50; m_tInspect3Loop.Set_LoopTime(10000);
		}
		break;

	case 50:	// Wait 회피 위치, Stage가 모두 Up 상태면 진행.
		if ((m_nInspect1Case >= 0 && m_nInspect1Case <= 31 && m_pDX05->iInspectStage1Up && !m_pDX05->iInspectStage1Down) &&
			(m_nInspect2Case >= 0 && m_nInspect2Case <= 31 && m_pDX05->iInspectStage2Up && !m_pDX05->iInspectStage2Down)) {

			m_nInspect3Case++; m_tInspect3Loop.Set_LoopTime(10000);
		}
		return TRUE;

	case 51:	// Move to Load Position
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE3_X, 4)) {
			m_tInspect3Loop.Takt_Start();
			g_objCommon.Move_Position(AX_INSPECT_STAGE3_X, 0);
			m_nInspect3Case++; m_tInspect3Loop.Set_LoopTime(10000);
		}
		break;
	case 52:	// Position Check
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE3_X, 0)) {
			m_tInspect3Loop.Takt_Save(9, 11);
			gData.bInspetBWD = FALSE;
			m_nInspect3Case = 70; m_tInspect3Loop.Set_LoopTime(10000);
		}
		break;

	case 70:	// Wait 회피 위치, Top1 Position 부터 진행한다.
		if (m_nInspect2Case < 80 && m_nInspect2Case > 7 && !g_objCommon.Check_Position(AX_INSPECT_STAGE2_X, 0)) {
			double dCurPos = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE2_X);
			double dT1Pos = m_pMoveData->dInspectStage2X[2];	//T1 Pos
			if (dCurPos >= dT1Pos) {	//위치 확인
				m_nInspect3Case++; m_tInspect3Loop.Set_LoopTime(10000);
			}
		}
		return TRUE;
	case 71:	// Stage Up
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE3_X, 0)) {
			m_tInspect3Loop.Takt_Start();
			m_pDY05->oInspectStage3Up = TRUE;	m_pDY05->oInspectStage3Down = FALSE;
			m_pDY05->oInspectStage3Fwd = TRUE;	m_pDY05->oInspectStage3Bwd = FALSE;
			if (!Check_AngleTrayEmpty() || !Check_Btm1PickerEmpty()) {
				m_pDY06->oInspectStage3Vac01 = TRUE;
				m_pDY06->oInspectStage3Vac02 = TRUE;
				m_pDY06->oInspectStage3Vac03 = TRUE;
				m_pDY06->oInspectStage3Vac04 = TRUE;
				m_pDY06->oInspectStage3Vac05 = TRUE;
				m_pDY06->oInspectStage3Vac06 = TRUE;
				m_pDY06->oInspectStage3Vac07 = TRUE;
				m_pDY06->oInspectStage3Vac08 = TRUE;
				m_pDY06->oInspectStage3Vac09 = TRUE;
				m_pDY06->oInspectStage3Vac10 = TRUE;
			}
			g_objAJinAXL.Write_Output(5);
			g_objAJinAXL.Write_Output(6);
			m_nInspect3Case++; m_tInspect3Loop.Set_LoopTime(10000);
		}
		break;
	case 72:	// Stage Up Check
		if (m_pDX05->iInspectStage3Up  && !m_pDX05->iInspectStage3Down &&
			m_pDX05->iInspectStage3Fwd && !m_pDX05->iInspectStage3Bwd) {

			if(m_nBtm1PickCase == 21 && !m_tInspect3Loop.Waiting_Time(300)) break;
			m_tInspect3Loop.Takt_Save(9, 12);
			m_strLog.Format("Inspection Stage3, %d", GetTickCount() - m_dwInspect3);
			g_objLogFile.Save_TestLog(m_strLog);
			m_nInspect3Case = 0; m_tInspect3Loop.Set_LoopTime(10000);
		}
		break;

	case 80:	//Move to Backward Offset
		if (TRUE/*g_objCommon.Get_InspectStage3VacOff(0)*/) {
			dIs3PosX = m_pMoveData->dInspectStage3X[1] + m_pEquipData->dInspectALOffset;
			g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE3_X, dIs3PosX, 0.25);	// X방향 얼라인. 속도는 1/4
			m_nInspect3Case++; m_tInspect3Loop.Set_LoopTime(5000);
		}
		break;
	case 81:
		if (g_objAJinAXL.Is_MoveDone(AX_INSPECT_STAGE3_X, dIs3PosX)) {
			g_objCommon.Set_InfoInspectVacOn(3, 1);
			dIs3PosX = m_pMoveData->dInspectStage3X[1] + m_pEquipData->dInspectALOffset - m_pEquipData->dInspectALBackOffset;
			g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE3_X, dIs3PosX, 0.25);	// X방향 얼라인. 속도는 1/4
			m_nInspect3Case = 4; m_tInspect3Loop.Set_LoopTime(5000);
		}
		break;

	case 85:	// Move to Module Align
		if (g_objCommon.Check_Position(AX_INSPECT_STAGE3_X, 5)) {
			double dCurPos = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE2_X);
			double dT2Pos = m_pMoveData->dInspectStage2X[3];	//T2 Pos
			if (!gData.bR04C) dT2Pos = m_pMoveData->dInspectStage2X[2] + m_pEquipData->dInspectPitchX * 4;	// R04A/B T1 Pos + Inspection Stage Pitch * 4
			double dUnPos = m_pMoveData->dInspectStage2X[4];	//Unload Pos
			if (dCurPos >= dT2Pos && dCurPos <= dUnPos) {	//위치 확인
				g_objCommon.Move_Position(AX_INSPECT_STAGE3_X, 1);
				m_nInspect3Case = 2; m_tInspect3Loop.Set_LoopTime(5000);
			} else {
				m_tInspect3Loop.Set_LoopTime(5000);
				return TRUE;
			}
		}
		break;
	}
 
	// 9. (Error : 3900)
	if (m_tInspect3Loop.Over_LoopTime()) {
		if (m_nInspect3Case == 10) {
			if (!gData.bR04C && nIs3ScanNo > 2) {
				Set_ScanError(3910, gData.nTNoInspect[2][nI3JNo1-1], gData.nTNoInspect[2][nI3JNo2-1], 0, 0, 
									gData.nCNoInspect[2][nI3JNo1-1], gData.nCNoInspect[2][nI3JNo2-1]);
			} else {
				Set_ScanError(3910, gData.nTNoInspect[2][nI3JNo1-1], gData.nTNoInspect[1][nI3JNo2-1], gData.nTNoInspect[2][nI3JNo3-1], gData.nTNoInspect[2][nI3JNo4-1],
									gData.nCNoInspect[2][nI3JNo1-1], gData.nCNoInspect[1][nI3JNo2-1], gData.nCNoInspect[2][nI3JNo3-1], gData.nCNoInspect[2][nI3JNo4-1]);
			}

		} else if (m_nInspect3Case == 20) {
			Set_ScanError(3920, gData.nTNoInspect[2][nI3JNo1-1], gData.nTNoInspect[2][nI3JNo2-1], 0, 0,
								gData.nCNoInspect[2][nI3JNo1-1], gData.nCNoInspect[2][nI3JNo2-1]);
		}
		g_objCommon.Show_Error(3900 + m_nInspect3Case);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 10. (Error : 4000)
BOOL CSequenceMain::Btm2Picker_Run()
{
	static int		nB2pWorkTray = 0;								// 작업중인 Buffer Tray (1 ~ 2)
	static int		nB2pStartNo = 0, nB2PickNo = 0;						// Btm2 Picker 시작 번호(0~9), 열별 순번(0~4)
	static int		nB2pTrayPosY = 0;								// 작업중인 Buffer Tray 위치 Y (R04A:0 ~ 4, R04B:0 ~ 2, R04C:0 ~ 3), X (R04A:0 ~ 3, R04B:0 ~ 4, R04C:0 ~ 7)
	static double	dB2pTrayY = 0.0, dB2pX = 0.0, dB2pZ = 0.0;					// Y, X, Z 이동위치
	static int		nB2pRow = 0, nB2pScanNo = 0, nB2pInspStageNo = 0;
	static int		nB2JNo1 = 0, nB2JNo2 = 0, nB2JNo3 = 0, nB2JNo4 = 0;
	static BOOL		bSave = FALSE;

	switch (m_nBtm2PickCase) {
	case 0:		// Wait for Angle Tray L1 Position
		if (m_nInspect1Case == 30 || m_nInspect2Case == 30 || m_nInspect3Case == 30) m_nBtm2PickCase++;
		return TRUE;

	case 1:
		if (g_objCommon.Check_Position(AX_BTM2_PICKER_X, 0) &&
			g_objCommon.Check_Position(AX_BTM2_PICKER_P1, 0) && g_objCommon.Check_Position(AX_BTM2_PICKER_P2, 0)) {
			if (m_nInspect1Case != 30 && m_nInspect2Case != 30 && m_nInspect3Case != 30) break;	// 인터락

			m_dwBtm2Pick = GetTickCount();
			if (m_nInspect1Case == 30) nB2pInspStageNo = 1;
			if (m_nInspect2Case == 30) nB2pInspStageNo = 2;
			if (m_nInspect3Case == 30) nB2pInspStageNo = 3;
			m_nBtm2PickCase++; m_tBtm2PickLoop.Set_LoopTime(5000);
		}
		break;

	case 2:		// Picker Z Inspection Stage Down, Btm2 Picker Vac On
		if (g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 0) &&
			g_objCommon.Check_Position(AX_BTM2_PICKER_P1, 0) && g_objCommon.Check_Position(AX_BTM2_PICKER_P2, 0))
		{
			g_objCommon.Move_Position(AX_BTM2_PICKER_Z, 1);
			
			//g_objCommon.Set_Btm2PickerDown(0);	// Picker 고정으로 사용.
			g_objCommon.Set_Btm2PickerDown();

			g_objCommon.Set_InfoBtm2VacOn(nB2pInspStageNo);
			m_nBtm2PickCase++; m_tBtm2PickLoop.Set_LoopTime(10000);
		}
		break;
	case 3:		// 정보전달(Stage->Picker), Inspection Stage Vac Off
		if (g_objCommon.Get_Btm2PickerDown(0) && g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 1)) {

			if (!m_tBtm2PickLoop.Waiting_Time(m_pEquipData->nDelayAdd[1])) break;
			for (int i = 0; i < 10; i++) {
				gData.InfoBtm2Pick[i] = gData.InfoInspect[nB2pInspStageNo-1][i]; gData.InfoInspect[nB2pInspStageNo-1][i] = 0;
									   
				gData.nTNoBtm2Pick[i] = gData.nTNoInspect[nB2pInspStageNo-1][i];
				gData.nCNoBtm2Pick[i] = gData.nCNoInspect[nB2pInspStageNo-1][i];
				gData.nTNoInspect[nB2pInspStageNo-1][i] = gData.nCNoInspect[nB2pInspStageNo-1][i] = 0;
			}
			gData.nPNoBtm2Pick = gData.nPNoInspect[nB2pInspStageNo-1]; gData.nPNoInspect[nB2pInspStageNo-1] = 0;
			g_objCommon.Set_InspectVacOff(nB2pInspStageNo, 0);
			m_nBtm2PickCase++; m_tBtm2PickLoop.Set_LoopTime(10000);
		}
		break;
	case 4:		// Picker Up
		if (/*g_objCommon.Get_InfoBtm2VacOn(nB2pInspStageNo) &&*/ g_objCommon.Get_InspectVacOff(nB2pInspStageNo, 0)) {	// Btm2 Picker Delay Time만 사용한다.
			if (!m_tBtm2PickLoop.Waiting_Time(m_pEquipData->nDelayAdd[1])) break;	// Btm2 Delay

			//g_objCommon.Set_Btm2PickerUp(0);	// Picker 고정으로 사용.
			g_objCommon.Move_Position(AX_BTM2_PICKER_Z, 0);
			// LJH A모델에서만 Pitch1/2 원점 잡아주면 될꺼 같다.
			if (gData.bR04A) {
				g_objAJinAXL.Move_Absolute(AX_BTM2_PICKER_P1, -2.5);
				g_objAJinAXL.Move_Absolute(AX_BTM2_PICKER_P2, -2.5);
				g_objCommon.Set_Btm2PickerUp24();
				g_objCommon.Set_Btm2PickerUp79();
			}
			m_nBtm2PickCase++; m_tBtm2PickLoop.Set_LoopTime(5000);
		}
		break;
	case 5:		// Position Check 
		if (/*g_objCommon.Get_Btm2PickerUp(0) && */g_objCommon.Get_InfoBtm2VacOn(nB2pInspStageNo) && g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 0) &&
			((gData.bR04A && g_objAJinAXL.Is_MoveDone(AX_BTM2_PICKER_P1, -2.5) && g_objAJinAXL.Is_MoveDone(AX_BTM2_PICKER_P2, -2.5) && g_objCommon.Get_Btm2PickerUp24() && g_objCommon.Get_Btm2PickerUp79()) ||
			 (!gData.bR04A && g_objCommon.Check_Position(AX_BTM2_PICKER_P1, 0) && g_objCommon.Check_Position(AX_BTM2_PICKER_P2, 0)))) {

			m_tBtm2PickLoop.Takt_Save(10, 1);
			if (nB2pInspStageNo == 1 && m_nInspect1Case == 30) m_nInspect1Case = 31;
			if (nB2pInspStageNo == 2 && m_nInspect2Case == 30) m_nInspect2Case = 31;
			if (nB2pInspStageNo == 3 && m_nInspect3Case == 30) m_nInspect3Case = 31;

			if (gData.bR04A) {
#ifdef EDITION_2ND
				m_pDY07->oBtm2PickerReadyL1 = FALSE;	m_pDY07->oBtm2PickerTurnL1 = TRUE;
				m_pDY07->oBtm2PickerReadyL2 = FALSE;	m_pDY07->oBtm2PickerTurnL2 = TRUE;
				m_pDY07->oBtm2PickerReadyL3 = FALSE;	m_pDY07->oBtm2PickerTurnL3 = TRUE;
				m_pDY07->oBtm2PickerReadyL4 = FALSE;	m_pDY07->oBtm2PickerTurnL4 = TRUE;
				m_pDY07->oBtm2PickerReadyL5 = FALSE;	m_pDY07->oBtm2PickerTurnL5 = TRUE;
				m_pDY07->oBtm2PickerReadyL6 = FALSE;	m_pDY07->oBtm2PickerTurnL6 = TRUE;
				m_pDY07->oBtm2PickerReadyL7 = FALSE;	m_pDY07->oBtm2PickerTurnL7 = TRUE;
				m_pDY07->oBtm2PickerReadyL8 = FALSE;	m_pDY07->oBtm2PickerTurnL8 = TRUE;
				m_pDY07->oBtm2PickerReadyL9 = FALSE;	m_pDY07->oBtm2PickerTurnL9 = TRUE;
				m_pDY07->oBtm2PickerReadyL10 = FALSE;	m_pDY07->oBtm2PickerTurnL10 = TRUE;
#else
				m_pDY07->oBtm2PickerTurn01 = TRUE; m_pDY07->oBtm2PickerTurn67 = TRUE;
				m_pDY07->oBtm2PickerTurn23 = TRUE; m_pDY07->oBtm2PickerTurn89 = TRUE;
				m_pDY07->oBtm2PickerTurn45 = TRUE; m_pDY07->oBtm2PickerTurn10 = TRUE;
#endif
				g_objAJinAXL.Write_Output(7);
			}
			m_nBtm2PickCase++; m_tBtm2PickLoop.Set_LoopTime(5000);
		}
		break;
	case 6:		// Wait. 안전확인 
		// TransY 0, 20~: Good Stage Position, 3~15: Empty Port Position 
		// TransX 10~20: Empty Port Position에 있으면 TransY는 대기중.
		if (((m_nEmptyTrayYCase ==  0 || m_nEmptyTrayYCase ==  2 || m_nEmptyTrayYCase >= 20) && g_objCommon.Check_Position(AX_EMPTY_TRANS2_Y, 0)) ||
			((m_nEmptyTrayYCase >=  3 && m_nEmptyTrayYCase <  15) && g_objCommon.Check_Position(AX_EMPTY_TRANS2_Y, 1)) ||
			((m_nEmptyTrayXCase >  10 && m_nEmptyTrayXCase <  20) && g_objCommon.Check_Position(AX_EMPTY_TRANS1_X, 1))) {

			m_nBtm2PickCase++; m_tBtm2PickLoop.Set_LoopTime(5000);
		} 
		return TRUE;
	case 7:		// Btm2 Inspection 위치로 이동.
		if (g_objCommon.Check_Position(AX_BTM2_PICKER_X, 0) && g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 0) 
		&& g_objCommon.Get_InfoBtm2VacOn(nB2pInspStageNo) && g_objCommon.Get_Btm2PickerRotate()/* && g_objCommon.Get_Btm2PickerUp(0)*/) {
			m_tBtm2PickLoop.Takt_Start();

			dB2pX = m_pMoveData->dBtm2PickerX[1];	// Btm2 Inspect
			g_objCommon.Move_Position(AX_BTM2_PICKER_Z, 2);
			g_objAJinAXL.Move_Absolute(AX_BTM2_PICKER_X, dB2pX);
			
			
			nB2pScanNo = 0;

			//m_nBtm2PickCase++; m_tBtm2PickLoop.Set_LoopTime(5000);
			m_nBtm2PickCase = 14; m_tBtm2PickLoop.Set_LoopTime(10000);
		}
		break;
	case 8:		// Z Axis Move To Btm2 Inspection Down
		if (g_objAJinAXL.Is_MoveDone(AX_BTM2_PICKER_X, dB2pX) && g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 2) &&
			g_objCommon.Check_Position(AX_BTM2_PICKER_P1, 0) && g_objCommon.Check_Position(AX_BTM2_PICKER_P2, 0) &&
			g_objCommon.Get_InfoBtm2VacOn(nB2pInspStageNo)) {

			m_tBtm2PickLoop.Takt_Save(10, 2);
			m_tBtm2PickLoop.Takt_Start();
			g_objCommon.Move_Position(AX_BTM2_PICKER_Z, 2);

			m_nBtm2PickCase++; m_tBtm2PickLoop.Set_LoopTime(10000);
		}
		break;

	case 9:		// Set Load Complete
		if (g_objAJinAXL.Is_MoveDone(AX_BTM2_PICKER_X, dB2pX) && g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 2) && g_objCommon.Get_InfoBtm2VacOn(nB2pInspStageNo)) {
			if (m_pEquipData->bUseInspectBtm2)
			{
				m_tBtm2PickLoop.Takt_Save(10, 3);
				Set_InspectJigNo(3, nB2pScanNo, nB2JNo1, nB2JNo2, nB2JNo3, nB2JNo4);

				g_objInspector.Set_LoadComplete(INSPECTOR_PC4, "B2", gLot.sLotID[gData.nPNoBtm2Pick-1], gData.nPNoBtm2Pick, 
					gData.nTNoBtm2Pick[nB2JNo1-1], gData.nTNoBtm2Pick[nB2JNo2-1], 0, 0,
					gData.nCNoBtm2Pick[nB2JNo1-1], gData.nCNoBtm2Pick[nB2JNo2-1], 0, 0);

				m_nBtm2PickCase = 10; m_tBtm2PickLoop.Set_LoopTime(90000);
			}
			else
			{
				if (!m_pEquipData->bUseInspectBtm2) { if(!m_tBtm2PickLoop.Waiting_Time(250)) break; }
				m_tBtm2PickLoop.Takt_Save(10, 3);
				m_nBtm2PickCase = 15; m_tBtm2PickLoop.Set_LoopTime(5000);
			}
			nB2pScanNo++;
		}
		break;

	case 10:	// Wait
		if (!m_pEquipData->bUseInspectBtm2) {
			m_nBtm2PickCase = 15; m_tBtm2PickLoop.Set_LoopTime(5000);
		}
		break;

	case 11:	// Btm2 Z Position Move
		if (g_objAJinAXL.Is_Done(AX_BTM2_PICKER_Z)) {
			m_tBtm2PickLoop.Takt_Start();
			g_objAJinAXL.Move_Absolute(AX_BTM2_PICKER_Z, m_dBtm2Z);
			m_nBtm2PickCase++; m_tBtm2PickLoop.Set_LoopTime(5000);
		}
		break;
	case 12:	// Send Move Complete
		if (g_objAJinAXL.Is_Done(AX_BTM2_PICKER_Z)) {
			m_tBtm2PickLoop.Takt_Save(10, 4);
			CString strLog;
			double dEnc = g_objAJinAXL.Get_Position(AX_BTM2_PICKER_Z);
			strLog.Format("%0.3lf,%0.3lf", m_dBtm2Z, dEnc);
			g_objLogFile.Save_FocusAxisLog(AX_BTM2_PICKER_Z, strLog);

			g_objInspector.Set_MoveComplete(INSPECTOR_PC4, "B2");
			m_nBtm2PickCase = 10; m_tBtm2PickLoop.Set_LoopTime(90000);	// 90초
		}
		break;

	case 14:	// Trun Wait
		if (g_objAJinAXL.Is_MoveDone(AX_BTM2_PICKER_X, dB2pX)) {
			if (!bSave) { bSave = TRUE; m_tBtm2PickLoop.Takt_Save(10, 9); }
			if (g_objCommon.Get_Btm2PickerRotate()) {
				if (!g_objCommon.Get_InfoBtm2VacOn(nB2pInspStageNo)) { m_tBtm2PickLoop.Set_LoopTime(0); break;}	// 바로 알람 처리 해준다.

				bSave = FALSE;
				m_tBtm2PickLoop.Takt_Save(10, 10);
				if(gData.bR04A || gData.bR04C || gData.bR04B)
				{
					g_objCommon.Move_Position(AX_BTM2_PICKER_P1, 0);	// Inspection Pitch
					g_objCommon.Move_Position(AX_BTM2_PICKER_P2, 0);	// Inspection Pitch
				}				
				
				g_objCommon.Move_Position(AX_BTM2_PICKER_Z, 2);
				g_objCommon.Set_Btm2PickerDown();

				m_nBtm2PickCase++; m_tBtm2PickLoop.Set_LoopTime(10000);
			}
		}
		break;
// Turn Sensor 확인 후 적용 생각해보자.
// 		if (g_objCommon.Get_Btm2PickerRotate()) {
// 			if (!bSave) {
// 				g_objCommon.Move_Position(AX_BTM2_PICKER_P1, 1);	// Tray Pitch
// 				g_objCommon.Move_Position(AX_BTM2_PICKER_P2, 1);	// Tray Pitch
// 				g_objCommon.Move_Position(AX_BTM2_PICKER_Z, 2);
// 				bSave = TRUE; m_tBtm2PickLoop.Takt_Save(10, 10);
// 			}
// 			if (g_objAJinAXL.Is_MoveDone(AX_BTM2_PICKER_X, dB2pX)) {
// 				//if (bR04A) { if (!m_tBtm2PickLoop.Waiting_Time(m_pEquipData->nDelayAdd[3])) break;	}	// LJH 필요없으면 삭제 가능
// 				bSave = FALSE; m_tBtm2PickLoop.Takt_Save(10, 9);
// 				m_nBtm2PickCase++; m_tBtm2PickLoop.Set_LoopTime(10000);
// 			}
//		}

	case 15:	// X Move to Pitch & Inspection End Check
		if (g_objAJinAXL.Is_MoveDone(AX_BTM2_PICKER_X, dB2pX)  && g_objCommon.Get_Btm2PickerDown(0) 
			&& g_objCommon.Check_Position(AX_BTM2_PICKER_P1, 0) && g_objCommon.Check_Position(AX_BTM2_PICKER_P2, 0)) 
		{
			//nB2pScanNo++;
			if (gData.bReload[4]) {
				nB2pScanNo--; gData.bReload[4] = FALSE;
			}
			if (nB2pScanNo > (gData.nBtmPickQt - 1)) { 
				m_tBtm2PickLoop.Takt_Start();
				nB2pScanNo = 0;
				//g_objCommon.Move_Position(AX_BTM2_PICKER_Z, 0);
				if(gData.bR04A || gData.bR04C || gData.bR04B)
				{
					g_objCommon.Move_Position(AX_BTM2_PICKER_P1, 1);	// Tray Pitch
					g_objCommon.Move_Position(AX_BTM2_PICKER_P2, 1);	// Tray Pitch
				}
				m_nBtm2PickCase++; m_tBtm2PickLoop.Set_LoopTime(10000);
			} else {
				if (Select_BtmScanPos(AUTO_VISION_BTM2, nB2pScanNo)) {
					m_tBtm2PickLoop.Takt_Start();
					if (gData.bR04A || gData.bR13A)	dB2pX = m_pMoveData->dBtm2PickerX[1] + nB2pScanNo * m_pEquipData->dInspectPitchX;	// Btm2 Inspect 
					else		dB2pX = m_pMoveData->dBtm2PickerX[1] + nB2pScanNo * m_pEquipData->dInspectPitchX;	// Btm2 Inspect

					g_objAJinAXL.Move_Absolute(AX_BTM2_PICKER_X, dB2pX);
					g_objCommon.Move_Position(AX_BTM2_PICKER_Z, 2);	// Btm2 Down
					m_nBtm2PickCase = 9; m_tBtm2PickLoop.Set_LoopTime(10000);
				} else {
					nB2pScanNo++;
				}
			}
		}
		break;
	case 16:	// Buffer Tray Position X, Y Move
		if (g_objAJinAXL.Is_MoveDone(AX_BTM2_PICKER_X, dB2pX) 
			&& g_objCommon.Check_Position(AX_BTM2_PICKER_P1, 1)
			&& g_objCommon.Check_Position(AX_BTM2_PICKER_P2, 1)/* && g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 0)*/) {

			m_tBtm2PickLoop.Takt_Save(10, 5);
			m_tBtm2PickLoop.Takt_Start();

			if (m_nBuffTray1Case == 10 || m_nBuffTray2Case == 10) {
				if (m_nBuffTray1Case == 10) nB2pWorkTray = 1;
				if (m_nBuffTray2Case == 10) nB2pWorkTray = 2;
				// Buffer Tray Position
				if (Select_Btm2PickPos(nB2pStartNo)) {
					Select_BufferTrayDownPos(nB2pWorkTray, nB2pTrayPosY, nB2pRow);

					if(nB2pRow == 0) g_objCommon.Set_Btm2PickerUp(0);
					else if(nB2pRow == 1) g_objCommon.Set_Btm2PickerUp(1);
					// 2:Buffer1 L1, 3:Buffer1 L2, 4:Buffer2 L1, 5:Buffer L2
					// R04C는 Tray 8x4이다
					if (gData.bR04C) {
						if (nB2pWorkTray == 1) {
							if (nB2pRow == 0)	dB2pX = m_pMoveData->dBtm2PickerX[2];
							else				dB2pX = m_pMoveData->dBtm2PickerX[3] + m_pEquipData->dTrayPitchX * 4;
						}
						if (nB2pWorkTray == 2) {
							if (nB2pRow == 0)	dB2pX = m_pMoveData->dBtm2PickerX[4];
							else				dB2pX = m_pMoveData->dBtm2PickerX[5] + m_pEquipData->dTrayPitchX * 4;
						}

					} else {
						if (gData.bR04B || gData.bR13B_5 || gData.bR13A) {
							if (nB2pStartNo > 4) nB2pRow = 1;
							else				 nB2pRow = 0;
						}
						if (nB2pWorkTray == 1) {
							if (nB2pRow == 0) dB2pX = m_pMoveData->dBtm2PickerX[2];
							if (nB2pRow == 1) dB2pX = m_pMoveData->dBtm2PickerX[3];
						}
						if (nB2pWorkTray == 2) {
							if (nB2pRow == 0) dB2pX = m_pMoveData->dBtm2PickerX[4];
							if (nB2pRow == 1) dB2pX = m_pMoveData->dBtm2PickerX[5];
						}
						
					}
					g_objAJinAXL.Move_Absolute(AX_BTM2_PICKER_X, dB2pX);
					g_objCommon.Move_Position(AX_BTM2_PICKER_Z, 0);

					if (nB2pWorkTray == 1 && m_nBuffTray1Case == 10) {
									
						if (gData.bR04A || gData.bR13A)	dB2pTrayY = m_pMoveData->dBufferStage1Y[nB2pRow] + nB2pTrayPosY * m_pEquipData->dTrayPitchX;
						else				dB2pTrayY = m_pMoveData->dBufferStage1Y[nB2pRow] + nB2pTrayPosY * m_pEquipData->dTrayPitchY;
						g_objAJinAXL.Move_Absolute(AX_BUFFER_STAGE1_Y, dB2pTrayY);
					}
					if (nB2pWorkTray == 2 && m_nBuffTray2Case == 10) {
						if (gData.bR04A || gData.bR13A)	dB2pTrayY = m_pMoveData->dBufferStage2Y[nB2pRow] + nB2pTrayPosY * m_pEquipData->dTrayPitchX;
						else				dB2pTrayY = m_pMoveData->dBufferStage2Y[nB2pRow] + nB2pTrayPosY * m_pEquipData->dTrayPitchY;
						g_objAJinAXL.Move_Absolute(AX_BUFFER_STAGE2_Y, dB2pTrayY);
					}
					m_nBtm2PickCase = 21; m_tBtm2PickLoop.Set_LoopTime(10000);
				}
			} else {
				// Tray 교체 이유로 Buffer가 대기중이 아니면 기본 위치로 간다.
				dB2pX = m_pMoveData->dBtm2PickerX[2];
				g_objAJinAXL.Move_Absolute(AX_BTM2_PICKER_X, dB2pX);
				g_objCommon.Move_Position(AX_BTM2_PICKER_Z, 0);
				//g_objCommon.Move_Position(AX_BTM2_PICKER_X, 2);
				m_nBtm2PickCase = 20; m_tBtm2PickLoop.Set_LoopTime(10000);
			}
			
		}
		break;

	case 20:	// Move to Buffer Position
		if (g_objAJinAXL.Is_MoveDone(AX_BTM2_PICKER_X, dB2pX) && g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 0)) {
			if (m_nBuffTray1Case == 10 || m_nBuffTray2Case == 10) {
				if (m_nBuffTray1Case == 10) nB2pWorkTray = 1;
				if (m_nBuffTray2Case == 10) nB2pWorkTray = 2;
				// Buffer Tray Position
				if (Select_Btm2PickPos(nB2pStartNo)) {
					Select_BufferTrayDownPos(nB2pWorkTray, nB2pTrayPosY, nB2pRow);

					if(nB2pRow == 0) g_objCommon.Set_Btm2PickerUp(0);
					else if(nB2pRow == 1) g_objCommon.Set_Btm2PickerUp(1);

					// 2:Buffer1 L1, 3:Buffer1 L2, 4:Buffer2 L1, 5:Buffer L2
					// R04C는 Tray 8x4이다
					if (gData.bR04C)
					{
						if (nB2pWorkTray == 1) {
							if (nB2pRow == 0)	dB2pX = m_pMoveData->dBtm2PickerX[2];
							else				dB2pX = m_pMoveData->dBtm2PickerX[3] + m_pEquipData->dTrayPitchX * 4;
						}
						if (nB2pWorkTray == 2) {
							if (nB2pRow == 0)	dB2pX = m_pMoveData->dBtm2PickerX[4];
							else				dB2pX = m_pMoveData->dBtm2PickerX[5] + m_pEquipData->dTrayPitchX * 4;
						}

					} 
					else
					{
						if (gData.bR04B || gData.bR13B_5 || gData.bR13A) {
							if (nB2pStartNo > 4) nB2pRow = 1;
							else				 nB2pRow = 0;
						}
						if (nB2pWorkTray == 1) {
							if (nB2pRow == 0) dB2pX = m_pMoveData->dBtm2PickerX[2];	// Buffer1 L1 Position
							if (nB2pRow == 1) dB2pX = m_pMoveData->dBtm2PickerX[3];	// Buffer1 L2 Position
						}
						if (nB2pWorkTray == 2) {
							if (nB2pRow == 0) dB2pX = m_pMoveData->dBtm2PickerX[4];	// Buffer2 L1 Position
							if (nB2pRow == 1) dB2pX = m_pMoveData->dBtm2PickerX[5];	// Buffer2 L2 Position
						}

					}
					g_objAJinAXL.Move_Absolute(AX_BTM2_PICKER_X, dB2pX);

					if (nB2pWorkTray == 1 && m_nBuffTray1Case == 10) {
						if (gData.bR04A || gData.bR13A)	dB2pTrayY = m_pMoveData->dBufferStage1Y[nB2pRow] + nB2pTrayPosY * m_pEquipData->dTrayPitchX;
						else				dB2pTrayY = m_pMoveData->dBufferStage1Y[nB2pRow] + nB2pTrayPosY * m_pEquipData->dTrayPitchY;
						g_objAJinAXL.Move_Absolute(AX_BUFFER_STAGE1_Y, dB2pTrayY);
					}
					if (nB2pWorkTray == 2 && m_nBuffTray2Case == 10) {
						if (gData.bR04A || gData.bR13A)	dB2pTrayY = m_pMoveData->dBufferStage2Y[nB2pRow] + nB2pTrayPosY * m_pEquipData->dTrayPitchX;
						else				dB2pTrayY = m_pMoveData->dBufferStage2Y[nB2pRow] + nB2pTrayPosY * m_pEquipData->dTrayPitchY;
						g_objAJinAXL.Move_Absolute(AX_BUFFER_STAGE2_Y, dB2pTrayY);
					}
					m_nBtm2PickCase++; m_tBtm2PickLoop.Set_LoopTime(5000);
				}
			}
		}
		return TRUE;
	case 21:	// Picker Down	동시동작
		if ( ((nB2pRow == 0 && g_objCommon.Get_Btm2PickerUp(0)) || (nB2pRow == 1 && g_objCommon.Get_Btm2PickerUp(1)) )
			 && g_objAJinAXL.Is_MoveDone(AX_BTM2_PICKER_X, dB2pX) && g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 0) &&
			((nB2pWorkTray == 1 && g_objAJinAXL.Is_MoveDone(AX_BUFFER_STAGE1_Y, dB2pTrayY)) ||
			(nB2pWorkTray == 2 && g_objAJinAXL.Is_MoveDone(AX_BUFFER_STAGE2_Y, dB2pTrayY)))) {

			m_tBtm2PickLoop.Takt_Save(10, 6);
			m_tBtm2PickLoop.Takt_Start();
			g_objCommon.Move_Position(AX_BTM2_PICKER_Z, 3);	// Buffer Down
			//g_objCommon.Set_InfoBtm2PickerDown(nB2pRow);	// Picker 고정으로 사용.
			m_nBtm2PickCase++; m_tBtm2PickLoop.Set_LoopTime(5000);
		}
		break;
	case 22:	// 정보전달, Btm2 Picker Vac Off
		if (g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 3)/* && g_objCommon.Get_InfoBtm2PickerDown(nB2pRow)*/) {

			for (int i = 0; i < gData.nBtmPickQt; i++) {
				int nBtmX = 0, nBuffX = 0;
				nBtmX = 5 * nB2pRow;

				if (gData.bR04A || gData.bR13A) {	// Buffer Stage 회전 되어 있다.
					gData.InfoBuffTray[nB2pWorkTray-1][4-i][nB2pTrayPosY] = gData.InfoBtm2Pick[nBtmX+i]; 
					gData.InfoBtm2Pick[nBtmX+i] = 0;

					gData.nTNoBuffTray[nB2pWorkTray-1][4-i][nB2pTrayPosY] = gData.nTNoBtm2Pick[nBtmX+i];
					gData.nCNoBuffTray[nB2pWorkTray-1][4-i][nB2pTrayPosY] = gData.nCNoBtm2Pick[nBtmX+i];
					gData.nTNoBtm2Pick[nBtmX+i] = gData.nCNoBtm2Pick[nBtmX+i] = 0;

				} else {
					if (gData.bR04C)	nBuffX = gData.nBtmPickQt * nB2pRow;
					else		nBuffX = 0;

					gData.InfoBuffTray[nB2pWorkTray-1][nB2pTrayPosY][nBuffX+i] = gData.InfoBtm2Pick[nBtmX+i]; 
					gData.InfoBtm2Pick[nBtmX+i] = 0;

					gData.nTNoBuffTray[nB2pWorkTray-1][nB2pTrayPosY][nBuffX+i] = gData.nTNoBtm2Pick[nBtmX+i];
					gData.nCNoBuffTray[nB2pWorkTray-1][nB2pTrayPosY][nBuffX+i] = gData.nCNoBtm2Pick[nBtmX+i];
					gData.nTNoBtm2Pick[nBtmX+i] = gData.nCNoBtm2Pick[nBtmX+i] = 0;
				}				
			}
			gData.nPNoBuffTray[nB2pWorkTray-1] = gData.nPNoBtm2Pick; 
			if (Check_Btm2PickerEmpty()) gData.nPNoBtm2Pick = 0;	// 모듈이 없을때 초기화
			g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 3, nB2pWorkTray-1);

// 			//R04C는 한라인에서 4개씩 2번 내려 놓는다.
// 			if (bR04C)	{ if (nB2pRow == 1) nB2pTrayPosY++; }
// 			else		{ nB2pTrayPosY++; }			
			
			m_nBtm2PickMultiCnt = gData.nBtmPickQt;	m_nBtm2PickRow = nB2pRow;
			m_pThreadVacuumB2pMulti = AfxBeginThread(Thread_Vacuum_B2p_Multi, (LPVOID)(1));
			m_nBtm2PickCase++; m_tBtm2PickLoop.Set_LoopTime(20000);
		}
		break;
	case 23:	// Picker Up
		if (!m_pThreadVacuumB2pMulti && g_objCommon.Get_Btm2PickerVacOffMulti(1, gData.nBtmPickQt, nB2pRow)) {
		
			g_objCommon.Move_Position(AX_BTM2_PICKER_Z, 0);	// Ready Up
			if(!m_tBtm2PickLoop.Waiting_Time(300)) break;
			g_objCommon.Set_Btm2PickerDown();
			m_nBtm2PickCase++; m_tBtm2PickLoop.Set_LoopTime(5000);
		}
		break;
	case 24:	// Tray Check
		if (/*g_objCommon.Get_Btm2PickerUp(0) && */g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 0) && g_objCommon.Get_Btm2PickerDown(0)) {
			m_tBtm2PickLoop.Takt_Save(10, 7);
			// 더이상 로드할께 없다 || 라인확인 || 현재 LOT 마지막 모듈이다 || 트레이가 꽉찼다
			if (m_bUnloadLotEnd || ((gData.bR04A || gData.bR13A)  && nB2pTrayPosY >= gData.nTrayX) || ( (!gData.bR04A && !gData.bR13A) && nB2pTrayPosY >= gData.nTrayY) ||
				Check_Btm2LoadEnd(gData.nPNoBuffTray[nB2pWorkTray-1])|| Check_BufferFull(nB2pWorkTray-1)) {

				nB2pTrayPosY = 0;	// 초기화
				if (nB2pWorkTray == 1) { m_nBuffTray1Case = 11; m_tBuffTray1Loop.Set_LoopTime(20000); }
				if (nB2pWorkTray == 2) { m_nBuffTray2Case = 11; m_tBuffTray2Loop.Set_LoopTime(20000); }
			}			
			m_nBtm2PickCase++; m_tBtm2PickLoop.Set_LoopTime(5000);
		}
		break;
	case 25:	// Picker Check, Unload or X, P1, P2 Move to Load Position
		if (g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 0) && Check_Btm2PickerEmpty()) {
			m_tBtm2PickLoop.Takt_Start();
			g_objCommon.Move_Position(AX_BTM2_PICKER_X, 0);
			g_objCommon.Move_Position(AX_BTM2_PICKER_P1, 0);
			g_objCommon.Move_Position(AX_BTM2_PICKER_P2, 0);
			m_nBtm2PickCase++; m_tBtm2PickLoop.Set_LoopTime(5000);

		} else {
			m_tBtm2PickLoop.Takt_Start();
			m_nBtm2PickCase = 20; m_tBtm2PickLoop.Set_LoopTime(5000);
		}
		break;
	case 26:	// Picker Turn
		if (g_objCommon.Check_Position(AX_BTM2_PICKER_P1, 0) && g_objCommon.Check_Position(AX_BTM2_PICKER_P2, 0)) {

			if (gData.bR04A) {
#ifdef EDITION_2ND
				m_pDY07->oBtm2PickerReadyL1 = TRUE;	m_pDY07->oBtm2PickerTurnL1 = FALSE;
				m_pDY07->oBtm2PickerReadyL2 = TRUE;	m_pDY07->oBtm2PickerTurnL2 = FALSE;
				m_pDY07->oBtm2PickerReadyL3 = TRUE;	m_pDY07->oBtm2PickerTurnL3 = FALSE;
				m_pDY07->oBtm2PickerReadyL4 = TRUE;	m_pDY07->oBtm2PickerTurnL4 = FALSE;
				m_pDY07->oBtm2PickerReadyL5 = TRUE;	m_pDY07->oBtm2PickerTurnL5 = FALSE;
				m_pDY07->oBtm2PickerReadyL6 = TRUE;	m_pDY07->oBtm2PickerTurnL6 = FALSE;
				m_pDY07->oBtm2PickerReadyL7 = TRUE;	m_pDY07->oBtm2PickerTurnL7 = FALSE;
				m_pDY07->oBtm2PickerReadyL8 = TRUE;	m_pDY07->oBtm2PickerTurnL8 = FALSE;
				m_pDY07->oBtm2PickerReadyL9 = TRUE;	m_pDY07->oBtm2PickerTurnL9 = FALSE;
				m_pDY07->oBtm2PickerReadyL10 = TRUE; m_pDY07->oBtm2PickerTurnL10 = FALSE;
#else
				m_pDY07->oBtm2PickerTurn01 = FALSE; m_pDY07->oBtm2PickerTurn67 = FALSE;
				m_pDY07->oBtm2PickerTurn23 = FALSE; m_pDY07->oBtm2PickerTurn89 = FALSE;
				m_pDY07->oBtm2PickerTurn45 = FALSE; m_pDY07->oBtm2PickerTurn10 = FALSE;
#endif
				g_objAJinAXL.Write_Output(7);
			}
			m_nBtm2PickCase++; m_tBtm2PickLoop.Set_LoopTime(5000);
		}
		break;
	case 27: // Position Check
		if (g_objCommon.Check_Position(AX_BTM2_PICKER_X, 0) && g_objCommon.Check_Position(AX_BTM2_PICKER_P1, 0) &&
			g_objCommon.Check_Position(AX_BTM2_PICKER_P2, 0) && g_objCommon.Get_Btm2PickerNormal()) {

				m_tBtm2PickLoop.Waiting_Time(2000);

				m_tBtm2PickLoop.Takt_Save(10, 8);
				m_strLog.Format("Btm2 Picker, %d", GetTickCount() - m_dwBtm1Pick);
				g_objLogFile.Save_TestLog(m_strLog);
				m_nBtm2PickCase = 0; m_tBtm2PickLoop.Set_LoopTime(5000);
		}
		break;

	}

	// 10. (Error : 4000)
	if (m_tBtm2PickLoop.Over_LoopTime()) {
		if (m_nBtm2PickCase == 10) {
			if (!gData.bR04C && nB2pScanNo > 2) {
				Set_ScanError(4010, gData.nTNoBtm2Pick[nB2JNo1-1], gData.nTNoBtm2Pick[nB2JNo3-1], 0, 0, gData.nCNoBtm2Pick[nB2JNo1-1], gData.nCNoBtm2Pick[nB2JNo3-1]);
			}
			else {
				Set_ScanError(4010, gData.nTNoBtm2Pick[nB2JNo1-1], gData.nTNoBtm2Pick[nB2JNo2-1], gData.nTNoBtm2Pick[nB2JNo3-1], gData.nTNoBtm2Pick[nB2JNo4-1],
									gData.nCNoBtm2Pick[nB2JNo1-1], gData.nCNoBtm2Pick[nB2JNo2-1], gData.nCNoBtm2Pick[nB2JNo3-1], gData.nCNoBtm2Pick[nB2JNo4-1]);
			}
		}
		g_objCommon.Show_Error(4000 + m_nBtm2PickCase);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 11. (Error : 4100)
BOOL CSequenceMain::BufferTray1_Run()
{
	switch (m_nBuffTray1Case) {
	case 0:		// Start시 1로 바뀜
		return TRUE;

	case 1:		// 안전확인.	//R04A는 회전 되어있어야 한다.
		if (m_bUnloadLotEnd) {
			m_nBuffTray1Case = 0;

		} else if ((((gData.bR04A || gData.bR13A) && m_pDX09->iBufferStage1Rotate && !m_pDX09->iBufferStage1Normal) ||
			((!gData.bR04A && !gData.bR13A) && !m_pDX09->iBufferStage1Rotate && m_pDX09->iBufferStage1Normal)) && 
			m_pDX09->iBufferStage1Up && g_objCommon.Check_Position(AX_BUFFER_STAGE1_Y, 0)) {

			m_nBuffTray1Case++; m_tBuffTray1Loop.Set_LoopTime(5000);
		}
		break;
	case 2:		// Tray Check
		if (m_pDX09->iBufferStage1Vacuum) {
			m_dwBuffTray1 = GetTickCount();
			m_nBuffTray1Case = 10; m_tBuffTray1Loop.Set_LoopTime(5000);
		}
		break;

	case 10:	// Wait for Module Loading
		//if (Check_UnloadLotEnd(1) && Check_BufferEmpty(1)) { m_nBuffTray1Case = 0; m_tBuffTray1Loop.Set_LoopTime(5000); }
		if (m_bUnloadLotEnd && Check_BufferEmpty(1)) { m_nBuffTray1Case = 0; m_tBuffTray1Loop.Set_LoopTime(5000); }
		return TRUE;

	case 11:	// Y Axis Move to Sort Picker Position
		if (!m_pDX09->iBufferStage2Up && m_pDX09->iBufferStage2Down) {
			if (m_nBuffTray2Case > 21) {	// Down 이후
				m_tBuffTray1Loop.Takt_Start();
				if (m_nSortPick1Case != 0 && m_nSortPick2Case == 0) g_objCommon.Move_Position(AX_BUFFER_STAGE1_Y, 3);
				else												g_objCommon.Move_Position(AX_BUFFER_STAGE1_Y, 2);

				if (gData.bR04A || gData.bR13A) {
					m_pDY09->oBufferStage1Rotate = FALSE;
					g_objAJinAXL.Write_Output(9);
				}
				m_nBuffTray1Case = 15; m_tBuffTray1Loop.Set_LoopTime(5000);
			} 

		} else {
			if (m_nBuffTray2Case == 20) { 
				m_nBuffTray1Case++; m_tBuffTray1Loop.Set_LoopTime(5000);
			}
		}
		break;
	case 12:	// Move to Sort Unload Wait Position
		if (g_objAJinAXL.Is_Done(AX_BUFFER_STAGE1_Y)) {
			g_objCommon.Move_Position(AX_BUFFER_STAGE1_Y, 5);
			if (gData.bR04A || gData.bR13A) {
				m_pDY09->oBufferStage1Rotate = FALSE;
				g_objAJinAXL.Write_Output(9);
			}
			m_nBuffTray1Case++; m_tBuffTray1Loop.Set_LoopTime(5000);
		}
		break;
	case 13:	// Position Check & Move to Sort Picker Position
		if (!m_pDX09->iBufferStage1Rotate && m_pDX09->iBufferStage1Normal &&
			g_objCommon.Check_Position(AX_BUFFER_STAGE1_Y, 5)) {

			if (!m_pDX09->iBufferStage2Up && m_pDX09->iBufferStage2Down) {
				if (m_nBuffTray2Case > 21) {	// Down 이후
					m_tBuffTray1Loop.Takt_Start();
					
					if (m_nSortPick1Case != 0 && m_nSortPick2Case == 0) g_objCommon.Move_Position(AX_BUFFER_STAGE1_Y, 3);
					else												g_objCommon.Move_Position(AX_BUFFER_STAGE1_Y, 2);

					if (gData.bR04A || gData.bR13A) {
						m_pDY09->oBufferStage1Rotate = FALSE;
						g_objAJinAXL.Write_Output(9);
					}
					m_nBuffTray1Case = 15; m_tBuffTray1Loop.Set_LoopTime(5000);
				} 

			} else {
				if (m_nBuffTray2Case == 20) { 
					m_tBuffTray1Loop.Set_LoopTime(5000); return TRUE;
				}
			}

		}
		break;

	case 15:	// Position Check
		if (!m_pDX09->iBufferStage1Rotate && m_pDX09->iBufferStage1Normal &&
			(g_objCommon.Check_Position(AX_BUFFER_STAGE1_Y, 2) || g_objCommon.Check_Position(AX_BUFFER_STAGE1_Y, 3))) {

			m_tBuffTray1Loop.Takt_Save(11, 1);
			m_nBuffTray1Case = 20; m_tBuffTray1Loop.Set_LoopTime(30000);	// 30초
		}
		break;

	case 20:	// Wait for Module Unloading
		return TRUE;

	case 21:	// Z Cylinder Down
		if (m_pDX09->iBufferStage1Up) {
			double dB1Y = g_objAJinAXL.Get_Position(AX_BUFFER_STAGE1_Y);
			double dB2Y = g_objAJinAXL.Get_Position(AX_BUFFER_STAGE2_Y);
			double dDiff = fabs(dB1Y-dB2Y);
			if (dDiff > TRAY_WIDTH) {	// Buffer Stage2 Y Position Check
				m_tBuffTray1Loop.Takt_Start();
				m_pDY09->oBufferStage1Up = FALSE; m_pDY09->oBufferStage1Down = TRUE;
				if (gData.bR04A || gData.bR13A) { m_pDY09->oBufferStage1Rotate = TRUE; }
				g_objAJinAXL.Write_Output(9);
				
				m_nBuffTray1Case++; m_tBuffTray1Loop.Set_LoopTime(5000);
			}
		}
		break;
	case 22:	// Down Check
		if (!m_pDX09->iBufferStage1Up && m_pDX09->iBufferStage1Down &&
			(((gData.bR04A || gData.bR13A) && m_pDX09->iBufferStage1Rotate && !m_pDX09->iBufferStage1Normal) ||
			((!gData.bR04A && !gData.bR13A) && !m_pDX09->iBufferStage1Rotate && m_pDX09->iBufferStage1Normal))) {

			m_tBuffTray1Loop.Takt_Save(11, 2);
			m_nBuffTray1Case = 50; m_tBuffTray1Loop.Set_LoopTime(5000);
		}
		break;

	case 50:	// Wait
		if (m_nBuffTray2Case > 2) { m_nBuffTray1Case++; m_tBuffTray1Loop.Set_LoopTime(5000); }
		return TRUE;

	case 51:	// Y Axis Move to Module Loading Position
		if (m_pDX09->iBufferStage1Vacuum && m_pDX09->iBufferStage2Up && !m_pDX09->iBufferStage2Down  &&
			(((gData.bR04A || gData.bR13A) && m_pDX09->iBufferStage1Rotate && !m_pDX09->iBufferStage1Normal) ||
			((!gData.bR04A && !gData.bR13A) && !m_pDX09->iBufferStage1Rotate && m_pDX09->iBufferStage1Normal))) {	// Inter lock

			m_tBuffTray1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_BUFFER_STAGE1_Y, 0);
			m_nBuffTray1Case++; m_tBuffTray1Loop.Set_LoopTime(5000);
		}
		break;
	case 52:	// Z Cylinder Up
		if (g_objCommon.Check_Position(AX_BUFFER_STAGE1_Y, 0)) {

			if (m_nBuffTray2Case >= 20 && m_nBuffTray2Case < 50) {
				double dB1Y = g_objAJinAXL.Get_Position(AX_BUFFER_STAGE1_Y);
				double dB2Y = g_objAJinAXL.Get_Position(AX_BUFFER_STAGE2_Y);
				double dDiff = fabs(dB1Y-dB2Y);
				if (dDiff > TRAY_WIDTH) {	// Buffer Stage2 Y Position Check
					m_tBuffTray1Loop.Takt_Save(11, 3);
					m_tBuffTray1Loop.Takt_Start();
					m_pDY09->oBufferStage1Up = TRUE; m_pDY09->oBufferStage1Down = FALSE;
					g_objAJinAXL.Write_Output(9);
					m_nBuffTray1Case = 60; m_tBuffTray1Loop.Set_LoopTime(5000);
				}
			} else if (m_nBuffTray2Case == 0 && m_bUnloadLotEnd) {
				m_nBuffTray1Case = 80; m_tBuffTray1Loop.Set_LoopTime(5000);	// 후처리
			}
		}
		return TRUE;
	case 60:	// Lot End Check
		if (m_pDX09->iBufferStage1Up && !m_pDX09->iBufferStage1Down) {
			m_tBuffTray1Loop.Takt_Save(11, 4);
			m_strLog.Format("Buffer Stage1, %d", GetTickCount() - m_dwBuffTray1);
			g_objLogFile.Save_TestLog(m_strLog);
			if (m_bUnloadLotEnd) { m_nBuffTray1Case = 0; m_tBuffTray1Loop.Set_LoopTime(5000); }
			else				 { m_nBuffTray1Case = 1; m_tBuffTray1Loop.Set_LoopTime(5000); }
		}
		break;

	case 80:	// 후처리
		if (g_objCommon.Check_Position(AX_BUFFER_STAGE1_Y, 0)) {
			if (!m_pDX09->iBufferStage1Up && m_pDX09->iBufferStage1Down &&
				 m_pDX09->iBufferStage2Up && !m_pDX09->iBufferStage2Down) {

				g_objCommon.Move_Position(AX_BUFFER_STAGE1_Y, 2);
			}
			m_nBuffTray1Case++; m_tBuffTray1Loop.Set_LoopTime(5000);
		}
		break;
	case 81:
		if (g_objCommon.Check_Position(AX_BUFFER_STAGE1_Y, 2)) {
			if (gData.bR04A || gData.bR13A) { m_pDY09->oBufferStage1Rotate = TRUE; }
			g_objAJinAXL.Write_Output(9);
			m_nBuffTray1Case = 50; m_tBuffTray1Loop.Set_LoopTime(5000);
		}
		break;

	}

	// 11. (Error : 4100)
	if (m_tBuffTray1Loop.Over_LoopTime()) {
		g_objCommon.Show_Error(4100 + m_nBuffTray1Case);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 12. (Error : 4200)
BOOL CSequenceMain::BufferTray2_Run()
{
	switch (m_nBuffTray2Case) {
	case 0:		// Start시 1로 바뀜
		return TRUE;

	case 1:		// 안전확인.
		if (m_bUnloadLotEnd) {
			m_nBuffTray2Case = 0;

		} else if ((((gData.bR04A || gData.bR13A) &&  m_pDX09->iBufferStage2Rotate && !m_pDX09->iBufferStage2Normal) ||
			((!gData.bR04A && !gData.bR13A) && !m_pDX09->iBufferStage2Rotate &&  m_pDX09->iBufferStage2Normal)) &&
			g_objCommon.Check_Position(AX_BUFFER_STAGE2_Y, 0)) {
			m_nBuffTray2Case++; m_tBuffTray2Loop.Set_LoopTime(5000);
		}
		break;
	case 2:		// Tray Check
		if (m_pDX09->iBufferStage2Vacuum) {
			m_dwBuffTray2 = GetTickCount();
			m_nBuffTray2Case = 10; m_tBuffTray2Loop.Set_LoopTime(5000);
		}
		break;

	case 10:	// Wait for Module Loading
		//if (Check_UnloadLotEnd(1) && Check_BufferEmpty(2)) { m_nBuffTray2Case = 0; m_tBuffTray2Loop.Set_LoopTime(5000); }
		if (m_bUnloadLotEnd && Check_BufferEmpty(2)) { m_nBuffTray2Case = 0; m_tBuffTray2Loop.Set_LoopTime(5000); }
		return TRUE;

	case 11:	// Y Axis Move to Sort Picker Position
		if (!m_pDX09->iBufferStage1Up && m_pDX09->iBufferStage1Down) {
			if (m_nBuffTray1Case > 21) {	// Down 이후
				m_tBuffTray2Loop.Takt_Start();
				if (m_nSortPick1Case != 0 && m_nSortPick2Case == 0) g_objCommon.Move_Position(AX_BUFFER_STAGE2_Y, 3);
				else												g_objCommon.Move_Position(AX_BUFFER_STAGE2_Y, 2);
				if (gData.bR04A || gData.bR13A) {
					m_pDY09->oBufferStage2Rotate = FALSE;
					g_objAJinAXL.Write_Output(9);
				}
				m_nBuffTray2Case = 15; m_tBuffTray2Loop.Set_LoopTime(5000);
			}

		} else {
			if (m_nBuffTray1Case == 20) {
				m_nBuffTray2Case++; m_tBuffTray2Loop.Set_LoopTime(5000);
			}
		}
		break;

	case 12:	// Move to Sort Unload Wait Position
		if (g_objAJinAXL.Is_Done(AX_BUFFER_STAGE2_Y)) {
			g_objCommon.Move_Position(AX_BUFFER_STAGE2_Y, 5);
			if (gData.bR04A || gData.bR13A) {
				m_pDY09->oBufferStage2Rotate = FALSE;
				g_objAJinAXL.Write_Output(9);
			}
			m_nBuffTray2Case++; m_tBuffTray2Loop.Set_LoopTime(5000);
		}
		break;
	case 13:	// Position Check & Move to Sort Picker Position
		if (!m_pDX09->iBufferStage2Rotate && m_pDX09->iBufferStage2Normal &&
			g_objCommon.Check_Position(AX_BUFFER_STAGE2_Y, 5)) {

			if (!m_pDX09->iBufferStage1Up && m_pDX09->iBufferStage1Down) {
				if (m_nBuffTray1Case > 21) {	// Down 이후
					m_tBuffTray2Loop.Takt_Start();
					if (m_nSortPick1Case != 0 && m_nSortPick2Case == 0) g_objCommon.Move_Position(AX_BUFFER_STAGE2_Y, 3);
					else												g_objCommon.Move_Position(AX_BUFFER_STAGE2_Y, 2);

					if (gData.bR04A || gData.bR13A) {
						m_pDY09->oBufferStage2Rotate = FALSE;
						g_objAJinAXL.Write_Output(9);
					}
					m_nBuffTray2Case = 15; m_tBuffTray2Loop.Set_LoopTime(5000);
				} 

			} else {
				if (m_nBuffTray1Case == 20) { 
					m_tBuffTray2Loop.Set_LoopTime(5000); return TRUE;
				}
			}

		}
		break;

	case 15:	// Position Check
		if (!m_pDX09->iBufferStage2Rotate && m_pDX09->iBufferStage2Normal &&
			(g_objCommon.Check_Position(AX_BUFFER_STAGE2_Y, 2) || g_objCommon.Check_Position(AX_BUFFER_STAGE2_Y, 3))) {
			m_tBuffTray2Loop.Takt_Save(12, 1);
			m_nBuffTray2Case = 20; m_tBuffTray2Loop.Set_LoopTime(30000);	// 30초
		}
		break;

	case 20:	// Wait for Module Unloading
		return TRUE;

	case 21:	// Z Cylinder Down
		if (m_pDX09->iBufferStage2Up) {
			double dB1Y = g_objAJinAXL.Get_Position(AX_BUFFER_STAGE1_Y);
			double dB2Y = g_objAJinAXL.Get_Position(AX_BUFFER_STAGE2_Y);
			double dDiff = fabs(dB1Y-dB2Y);
			if (dDiff > TRAY_WIDTH) {	// Buffer Stage Y Position Check
				m_tBuffTray2Loop.Takt_Start();
				m_pDY09->oBufferStage2Up = FALSE; m_pDY09->oBufferStage2Down = TRUE;
				if (gData.bR04A || gData.bR13A) { m_pDY09->oBufferStage2Rotate = TRUE; }
				g_objAJinAXL.Write_Output(9);
				m_nBuffTray2Case++; m_tBuffTray2Loop.Set_LoopTime(5000);
			}
		}
		break;
	case 22:	// Down Check
		if (!m_pDX09->iBufferStage2Up && m_pDX09->iBufferStage2Down &&
			(((gData.bR04A || gData.bR13A) &&  m_pDX09->iBufferStage2Rotate && !m_pDX09->iBufferStage2Normal) ||
			((!gData.bR04A && !gData.bR13A) && !m_pDX09->iBufferStage2Rotate &&  m_pDX09->iBufferStage2Normal))) {

			m_tBuffTray2Loop.Takt_Save(12, 2);
			m_nBuffTray2Case = 50; m_tBuffTray2Loop.Set_LoopTime(5000);
		}
		break;

	case 50:	// Wait
		if (m_nBuffTray1Case > 2) { m_nBuffTray2Case++; m_tBuffTray2Loop.Set_LoopTime(5000); }
		return TRUE;

	case 51:	// Y Axis Move to Module Loading Position
		if (m_pDX09->iBufferStage2Vacuum && m_pDX09->iBufferStage1Up && !m_pDX09->iBufferStage1Down  &&
			(((gData.bR04A || gData.bR13A) && m_pDX09->iBufferStage2Rotate && !m_pDX09->iBufferStage2Normal) ||
			((!gData.bR04A && !gData.bR13A)&& !m_pDX09->iBufferStage2Rotate && m_pDX09->iBufferStage2Normal))) {	// Inter lock

			m_tBuffTray2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_BUFFER_STAGE2_Y, 0);
			m_nBuffTray2Case++; m_tBuffTray2Loop.Set_LoopTime(5000);
		}
		break;
	case 52:	// Z Cylinder Up
		if (g_objCommon.Check_Position(AX_BUFFER_STAGE2_Y, 0)) {

			if (m_nBuffTray1Case >= 20 && m_nBuffTray1Case < 50) {
				double dB1Y = g_objAJinAXL.Get_Position(AX_BUFFER_STAGE1_Y);
				double dB2Y = g_objAJinAXL.Get_Position(AX_BUFFER_STAGE2_Y);
				double dDiff = fabs(dB1Y-dB2Y);
				if (dDiff > TRAY_WIDTH) {	// Buffer Stage Y Position Check
					m_tBuffTray2Loop.Takt_Save(12, 3);
					m_tBuffTray2Loop.Takt_Start();
					m_pDY09->oBufferStage2Up = TRUE; m_pDY09->oBufferStage2Down = FALSE;
					g_objAJinAXL.Write_Output(9);
					m_nBuffTray2Case = 60; m_tBuffTray2Loop.Set_LoopTime(5000);
				}
			} else if (m_nBuffTray1Case == 0 && m_bUnloadLotEnd) {
				m_nBuffTray2Case = 80; m_tBuffTray2Loop.Set_LoopTime(5000);	// 후처리
			}
		}
		return TRUE;
	case 60:	// Lot End Check
		if (m_pDX09->iBufferStage2Up && !m_pDX09->iBufferStage2Down) {
			m_tBuffTray2Loop.Takt_Save(12, 4);
			m_strLog.Format("Buffer Stage2, %d", GetTickCount() - m_dwBuffTray2);
			g_objLogFile.Save_TestLog(m_strLog);
			if (m_bUnloadLotEnd) { m_nBuffTray2Case = 0; m_tBuffTray2Loop.Set_LoopTime(5000); }
			else				 { m_nBuffTray2Case = 1; m_tBuffTray2Loop.Set_LoopTime(5000); }
		}
		break;

	case 80:	// 후처리
		if (g_objCommon.Check_Position(AX_BUFFER_STAGE2_Y, 0)) {
			if (m_pDX09->iBufferStage1Up && !m_pDX09->iBufferStage1Down &&
				!m_pDX09->iBufferStage2Up && m_pDX09->iBufferStage2Down) {

				g_objCommon.Move_Position(AX_BUFFER_STAGE2_Y, 2);
			}
			m_nBuffTray2Case++; m_tBuffTray2Loop.Set_LoopTime(5000);
		}
		break;
	case 81:
		if (g_objCommon.Check_Position(AX_BUFFER_STAGE2_Y, 2)) {
			if (gData.bR04A || gData.bR13A) { m_pDY09->oBufferStage2Rotate = TRUE; }
			g_objAJinAXL.Write_Output(9);
			m_nBuffTray2Case = 50; m_tBuffTray2Loop.Set_LoopTime(5000);
		}
		break;
	}

	// 12. (Error : 4200)
	if (m_tBuffTray2Loop.Over_LoopTime()) {
		g_objCommon.Show_Error(4200 + m_nBuffTray2Case);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 13. (Error : 4300)
BOOL CSequenceMain::SortPicker1_Run()
{
	static int		nSp1WorkBuff = 0;
	static int		nSp1WorkGood = 0;
	static int		nSp1WorkNg = 0;
	static int		nSp1TrayPosX = 0, nSp1TrayPosY = 0;
	static double	dSp1X = 0.0, dSp1Y = 0.0;
	static int		nSp1TrayCnt = 0, nSp1PickCnt = 0, nSp1StartNo = 0, nSp1DownSu = 0, nSp1Base = 0, nPNo = 0;
	static int		nSp1VacErr = 0;
	static BOOL		bLastNgBuffPick1 = FALSE;
	BOOL bS1Checking = FALSE;							// Inspect Done Check
	

	switch (m_nSortPick1Case) {
	case 0:		// Wait
		if (m_nBuffTray1Case == 20 || m_nBuffTray2Case == 20) {	// Unload 위치
			if (!Check_NgBufferEmpty(1) && Check_SortPickerEmpty(1)) {
				if (m_nBuffTray1Case == 20) nSp1WorkBuff = 1;
				if (m_nBuffTray2Case == 20) nSp1WorkBuff = 2;
				if (gData.nPNoNgTray != 0 && (gData.nPNoNgTray != gData.nPNoBuffTray[nSp1WorkBuff-1])) {
					bLastNgBuffPick1 = TRUE;
					m_nSortPick1Case = 50; m_tSortPick1Loop.Set_LoopTime(10000);
					break;
				}
			}
			//if (m_nSortPick2Case == 0 || (m_nSortPick2Case >= 10 && m_nSortPick2Case < 30) ||
			if (m_nSortPick2Case == 0 || (m_nSortPick2Case >= 6 && m_nSortPick2Case < 30) ||
				(m_nSortPick2Case >= 40 && m_nSortPick2Case < 60)) {	// Sort2가 Buffer 대기중 이거나 NG or Good Unload 작업중.

				m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(5000);
			}
		} else {
			if (!gData.bNGTrayWait && !Check_NgBufferEmpty(1) && Check_SortPickerEmpty(1)) {
				m_nSortPick1Case = 50; m_tSortPick1Loop.Set_LoopTime(10000);
			}
		}
		return TRUE;

	case 1:		// Select Pick Position
		if ((g_objCommon.Check_Position(AX_SORT_PICKER1_X, 0) || g_objCommon.Check_Position(AX_SORT_PICKER1_X, 1)) &&
			g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 0) && g_objCommon.Check_Position(AX_SORT_PICKER1_P, 0)){
			if (m_nBuffTray1Case == 20 || m_nBuffTray2Case == 20) {
				if (m_nBuffTray1Case == 20) nSp1WorkBuff = 1;
				if (m_nBuffTray2Case == 20) nSp1WorkBuff = 2;
				if (nSp1WorkBuff != 1 && nSp1WorkBuff != 2) break;	// Test Interlock
				if ((nSp1WorkBuff == 1 && !g_objAJinAXL.Is_Done(AX_BUFFER_STAGE1_Y)) ||
					(nSp1WorkBuff == 2 && !g_objAJinAXL.Is_Done(AX_BUFFER_STAGE2_Y))) break;
			
				gData.bBuffStageMove = TRUE;
				if (Check_SortPickerEmpty(1)) {
					m_dwSortPick1 = GetTickCount();
					m_tSortPick1Loop.Takt_Start();

					if (!Select_BufferTrayUpPos(nSp1WorkBuff, nSp1TrayPosX, nSp1TrayPosY)) nSp1TrayPosX = 0;	// Good Tray 교체중...

					// R04C는 Tray 8x4이다
					if (gData.bR04C) {
						if (nSp1TrayPosX < 4)	{ nSp1Base = 0; dSp1X = m_pMoveData->dSortPicker1X[nSp1WorkBuff-1]; }
						else					{ nSp1Base = 4; dSp1X = m_pMoveData->dSortPicker1X[nSp1WorkBuff-1] + m_pEquipData->dTrayPitchX * 4; }

					} else { nSp1Base = 0; dSp1X = m_pMoveData->dSortPicker1X[nSp1WorkBuff-1]; }
					g_objAJinAXL.Move_Absolute(AX_SORT_PICKER1_X, dSp1X);

					if (nSp1WorkBuff == 1 && m_nBuffTray1Case == 20) { 
						dSp1Y = m_pMoveData->dBufferStage1Y[2] + nSp1TrayPosY * m_pEquipData->dTrayPitchY;
						g_objAJinAXL.Move_Absolute(AX_BUFFER_STAGE1_Y, dSp1Y);
					}
					if (nSp1WorkBuff == 2 && m_nBuffTray2Case == 20) {
						dSp1Y = m_pMoveData->dBufferStage2Y[2] + nSp1TrayPosY * m_pEquipData->dTrayPitchY;
						g_objAJinAXL.Move_Absolute(AX_BUFFER_STAGE2_Y, dSp1Y);
					}
					nSp1VacErr = 0;
					m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000);
				}

// 					if (!Select_BufferTrayUpPos(nSp1WorkNo, nSp1TrayPosX, nSp1TrayPosY)) nSp1TrayPosX = 0;	// Good Tray 교체중...
// 					
// 					dSp1X = m_pMoveData->dSortPicker1X[0] + nSp1TrayPosX * m_pEquipData->dTrayPitchX;
// 					g_objAJinAXL.Move_Absolute(AX_SORT_PICKER1_X, dSp1Y);
// 
// 					if (nSp1WorkNo == 1 && m_nBuffTray1Case == 20) {
// 						dSp1Y = m_pMoveData->dBufferStage1Y[2] + nSp1TrayPosY * m_pEquipData->dTrayPitchY;
// 						g_objAJinAXL.Move_Absolute(AX_BUFFER_STAGE1_Y, dSp1Y);
// 					}
// 					if (nSp1WorkNo == 2 && m_nBuffTray2Case == 20) {
// 						dSp1Y = m_pMoveData->dBufferStage2Y[2] + nSp1TrayPosY * m_pEquipData->dTrayPitchY;
// 						g_objAJinAXL.Move_Absolute(AX_BUFFER_STAGE2_Y, dSp1Y);
// 					}
// 					m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000);
// 				}
			}
		}
		break;
	case 2:		// Picker Down
		if (g_objAJinAXL.Is_MoveDone(AX_SORT_PICKER1_X, dSp1X) &&
			((nSp1WorkBuff == 1 && g_objAJinAXL.Is_MoveDone(AX_BUFFER_STAGE1_Y, dSp1Y)) ||
			 (nSp1WorkBuff == 2 && g_objAJinAXL.Is_MoveDone(AX_BUFFER_STAGE2_Y, dSp1Y))) ) {

			if (nSp1VacErr == 0) m_tSortPick1Loop.Takt_Save(13, 1);
			m_tSortPick1Loop.Takt_Start();

			if (nSp1WorkBuff == 1) g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 1);	//Buffer 1 Down
			if (nSp1WorkBuff == 2) g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 5);	//Buffer 2 Down

			g_objCommon.Set_SortPicker1Down(0);	// All Down
			m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000);
		} 
		break;
	case 3:		// 정보전달, Picker Vac On
		if (((nSp1WorkBuff == 1 && g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 1)) ||
			 (nSp1WorkBuff == 2 && g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 5))) &&
			g_objCommon.Get_SortPicker1Down(0)) {

			if (nSp1VacErr == 1) { m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000); break; }

			for (int i = 0; i < gData.nSortPickQt; i++) {
				gData.InfoSortPick[0][i] = gData.InfoBuffTray[nSp1WorkBuff-1][nSp1TrayPosY][nSp1Base+i]; 
				gData.InfoBuffTray[nSp1WorkBuff-1][nSp1TrayPosY][nSp1Base+i] = 0;

				gData.nTNoSortPick[0][i] = gData.nTNoBuffTray[nSp1WorkBuff-1][nSp1TrayPosY][nSp1Base+i];
				gData.nTNoBuffTray[nSp1WorkBuff-1][nSp1TrayPosY][nSp1Base+i] = 0;
				gData.nCNoSortPick[0][i] = gData.nCNoBuffTray[nSp1WorkBuff-1][nSp1TrayPosY][nSp1Base+i];
				gData.nCNoBuffTray[nSp1WorkBuff-1][nSp1TrayPosY][nSp1Base+i] = 0;
			}
			gData.nPNoSortPick[0] = gData.nPNoBuffTray[nSp1WorkBuff-1];

			if ((gData.nCNoSortPick[0][0] == 1 || gData.nCNoSortPick[0][0] == 6 || gData.nCNoSortPick[0][0] == 11) && nSp1TrayPosY == 0) {
				if (gData.nTNoSortPick[0][0] == 1 && gData.nCNoSortPick[0][0] == 1) {
					m_dwULCycleTime = 0;
					m_dwULPickUpTime = GetTickCount();

				} else {
					m_dwULCycleTime = GetTickCount() - m_dwULPickUpTime;
					m_dwULPickUpTime = GetTickCount();
					g_dlgWork.PostMessage(UM_UPDATE_UPH, 1, NULL);
				}
			}

			if (Check_BufferEmpty(nSp1WorkBuff)) {
				gData.nPNoBuffTray[nSp1WorkBuff-1] = 0;
			}
			g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 3, nSp1WorkBuff-1);

			g_objCommon.Set_InfoSortPicker1VacOn();
			m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000);
		} 
		break;
	case 4:		// Picker Up
		//if (g_objCommon.Get_InfoSortPicker1VacOn()) {	// Delay Time만 사용한다.
		if (nSp1VacErr == 0) { if (!m_tSortPick1Loop.Waiting_Time(m_pEquipData->nDelayAdd[2])) break; }	// SortPicker1 Delay
		else				 { if (!m_tSortPick1Loop.Waiting_Time(m_pEquipData->nDelayAdd[2]*2)) break; }	// Retry할때는 대기시간을 2배로 늘려준다.

			g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 0);	//Ready Up
			g_objCommon.Set_SortPicker1Up(0);	// All Up
			m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(6000);
		//}
		break;
	case 5:		
		if (g_objCommon.Get_InfoSortPicker1VacOn() && 
			g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 0) && g_objCommon.Get_SortPicker1Up(0)) {

			m_tSortPick1Loop.Takt_Save(13, 2);
			if (Check_BufferEmpty(nSp1WorkBuff)) {	// Tray Change
				nSp1TrayPosY = 0;
				if (nSp1WorkBuff == 1 && m_nBuffTray1Case == 20) m_nBuffTray1Case = 21;
				if (nSp1WorkBuff == 2 && m_nBuffTray2Case == 20) m_nBuffTray2Case = 21;
			}
			gData.bBuffStageMove = FALSE;
			gData.nSp1Timer = GetTickCount();
			SYSTEMTIME time;
			GetLocalTime(&time);
			gData.sSortWaitStartTime[0].Format("%02d:%02d:%02d %03d", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

			m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(30000);	// 검사 완료
		} 
		break;
	case 6:		// NG or Good
		for (int i = 4; i >= 0; i--) { if (!Check_InspectDone(gData.nPNoSortPick[0], gData.nTNoSortPick[0][i], gData.nCNoSortPick[0][i], gData.InfoSortPick[0][i], 1)) { bS1Checking = TRUE; break; } }
		if (bS1Checking) break;	// Not Complete

		nPNo = gData.nPNoSortPick[0];
		for (int i = 0; i < 5; i++) {
			if (gData.InfoSortPick[0][i] == 1) gLot.nGoodCount[nPNo-1]++;
			else if (gData.InfoSortPick[0][i] != 0) gLot.nNgCount[nPNo-1]++;
			for (int j = 0; j < 5; j++) { if (gData.InfoSortPick[0][i] == j + 3) gLot.nSNgCount[nPNo-1][j]++; }	// S-NG (3,4,5,6,7,8)
			Write_LotJudge(nPNo, gData.nTNoSortPick[0][i], gData.nCNoSortPick[0][i], gData.InfoSortPick[0][i]);
		}
		g_dlgWork.PostMessage(UM_VISION_RESULT, nPNo, NULL);

		//	NG가 있으면 무조건 NG 부터 간다.
		if (Select_SortPickNgPos(1, nSp1StartNo, nSp1PickCnt)) {	// NG
			m_nSortPick1Case = 7; m_tSortPick1Loop.Set_LoopTime(10000);
		} else {
			if (Select_SortPickGoodPos(1, nSp1StartNo, nSp1PickCnt)) {	// NG 없고 Good만 있을때
				m_nSortPick1Case = 17; m_tSortPick1Loop.Set_LoopTime(10000);
			}
		}
		break;
	case 7:	// Wait Move to Ng Position
		// SortPicker2번이 Pickup 작업중, Good Unload 작업중,  NG Buffer 작업중이면 NG로 갈수있다.
		if ((m_nSortPick2Case >=  0 && m_nSortPick2Case < 10) ||
			(m_nSortPick2Case >= 19 && m_nSortPick2Case < 30) ||
			(m_nSortPick2Case >= 40 && m_nSortPick2Case < 60) ||
			(m_nSortPick2Case == 12) && (gData.nPNoSortPick[0] != gData.nPNoSortPick[1])||
			(m_nSortPick2Case == 17)) {

			if ((gData.nPNoSortPick[0] != gData.nPNoNgTray) && (gData.nPNoSortPick[0] != 0)) {
				if (gData.nPNoNgTray != 0) return TRUE; 
			}
			m_nSortPick1Case = 10; m_tSortPick1Loop.Set_LoopTime(10000);

		} else {
			if (Select_SortPickGoodPos(1, nSp1StartNo, nSp1PickCnt)) {		// Sort2번이 NG 작업 중이고 Good이 있다면 Good Unload 먼저 해준다.
				m_nSortPick1Case = 17; m_tSortPick1Loop.Set_LoopTime(10000);

			} else if (m_nSortPick2Case >= 11 && m_nSortPick2Case < 20) {	// Sort2번이 NG 작업 중이고 NG만 있다면 NG Tray 1번 위치로 가서 대기한다.
				m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000);
			}
		}
		return TRUE;

	case 8:	// Move to NG Tray1 Position
		if (g_objAJinAXL.Is_Done(AX_SORT_PICKER1_X)) {
			g_objCommon.Move_Position(AX_SORT_PICKER1_X, 4);
			m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000);
		}
		break;
	case 9:	// Check Position & Sort2
		if (g_objCommon.Check_Position(AX_SORT_PICKER1_X, 4)) {
			// SortPicker2번이 Pickup 작업중, Good Unload 작업중,  NG Buffer 작업중이면 NG Unloading 할수있다.
			if ((m_nSortPick2Case >=  0 && m_nSortPick2Case < 10) ||
				(m_nSortPick2Case >= 19 && m_nSortPick2Case < 30) ||
				(m_nSortPick2Case >= 40 && m_nSortPick2Case < 60) ||
				(m_nSortPick2Case == 17)) {

				if ((gData.nPNoSortPick[0] != gData.nPNoNgTray) && (gData.nPNoSortPick[0] != 0)) {
					if (gData.nPNoNgTray != 0) return TRUE; 
				}
				m_nSortPick1Case = 10; m_tSortPick1Loop.Set_LoopTime(10000);
			}
		}
		return TRUE;

	// NG Unload
	case 10:	// Check NG Tray
		// NG Tray가 받을 준비 됐는지 확인한다.
		if (m_nNgTrayCase == 10) { 
			m_nSortPick1Case = 12; m_tSortPick1Loop.Set_LoopTime(10000); 

		} else {
			if (g_objAJinAXL.Is_Done(AX_SORT_PICKER1_Z) && g_objAJinAXL.Is_Done(AX_SORT_PICKER1_X)) {
				g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 0);	// Ready Up

				if (Select_SortPickNgPos(1, nSp1StartNo, nSp1PickCnt)) {	// startNo 0base pickCnt 1base
					if (bLastNgBuffPick1 == FALSE && !Check_UnloadLotEnd(1) && !Check_NgBufferFull(1)) { // Ng Tray 교체 작업 중에 Ng Buffer에 내려놓는다.
						m_nSortPick1Case = 40; m_tSortPick1Loop.Set_LoopTime(10000);	

					} else {
						g_objCommon.Move_Position(AX_SORT_PICKER1_X, 5);
						m_nSortPick1Case = 11; m_tSortPick1Loop.Set_LoopTime(10000);
					}
				} else {
					m_nSortPick1Case = 17; m_tSortPick1Loop.Set_LoopTime(10000);
				}
			}
		}
		break;
	case 11:	// Check Position
		if (g_objCommon.Check_Position(AX_SORT_PICKER1_X, 5)) {
			if (m_nNgTrayCase == 10) { m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000); }
		}
		return TRUE;

	case 12:	// Move to NG Unload Position
		if ((gData.nPNoSortPick[0] != gData.nPNoNgTray) && (gData.nPNoSortPick[0] != 0)) {
			if (gData.nPNoNgTray != 0) return TRUE; 
		}
		if (m_nNgTrayCase != 10) { m_nSortPick1Case = 10; m_tSortPick1Loop.Set_LoopTime(10000); return TRUE; }
		if (bLastNgBuffPick1 == TRUE) bLastNgBuffPick1 = FALSE;

		if (Select_SortPickNgPos(1, nSp1StartNo, nSp1PickCnt)) {	// startNo 0base pickCnt 1base
			if (Select_NgTrayPos(1, nSp1WorkNg, nSp1TrayPosX, nSp1TrayPosY)) {
				m_tSortPick1Loop.Takt_Start();			
				if (!Select_NgTrayPos(1, nSp1WorkNg, nSp1TrayPosX, nSp1TrayPosY)) nSp1TrayPosX = 0;	// Tray 교체중...	// ng Position 0base
				nSp1TrayCnt = gData.nTrayX - nSp1TrayPosX;
				nSp1DownSu = ((nSp1PickCnt < nSp1TrayCnt) ? nSp1PickCnt : nSp1TrayCnt);
				m_nSortPick1MultiCnt = nSp1DownSu;

				int nNgTrayY = nSp1WorkNg / 2;
				int nNgTrayX = nSp1WorkNg % 2;
				dSp1X = m_pMoveData->dSortPicker1X[4 + nNgTrayX] + (nSp1TrayPosX - nSp1StartNo) * m_pEquipData->dTrayPitchX;
				dSp1Y = m_pMoveData->dNgStageY[1 + nNgTrayY] + nSp1TrayPosY * m_pEquipData->dTrayPitchY;

				g_objAJinAXL.Move_Absolute(AX_SORT_PICKER1_X, dSp1X);
				g_objAJinAXL.Move_Absolute(AX_NG_STAGE_Y, dSp1Y);
				m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000);

			}
		} else {
			g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 0);	// Ready Up
			m_nSortPick1Case = 17; m_tSortPick1Loop.Set_LoopTime(10000);
		}
		break;
	case 13:	// Picker Down
		if (g_objAJinAXL.Is_MoveDone(AX_SORT_PICKER1_X, dSp1X) && g_objAJinAXL.Is_MoveDone(AX_NG_STAGE_Y, dSp1Y) && g_objCommon.Check_Position(AX_SORT_PICKER1_P, 0)) {
			m_tSortPick1Loop.Takt_Save(13, 3);
			m_tSortPick1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 3);	//NG Down
			g_objCommon.Set_SortPicker1DownMulti(nSp1StartNo+1, nSp1DownSu);
			m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000);
		}
		break;
	case 14:	// 정보전달, Vac Off
		if (g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 3) && g_objCommon.Get_SortPicker1DownMulti(nSp1StartNo+1, nSp1DownSu)) {
			for (int i = 0; i < nSp1DownSu; i++) {
				gData.InfoNgTray[nSp1WorkNg][nSp1TrayPosY][nSp1TrayPosX+i] = gData.InfoSortPick[0][nSp1StartNo+i]; 
				gData.InfoSortPick[0][nSp1StartNo+i] = 0;

// 				// 연속랏을 위해 모듈 언로딩 했으면 판정 완료 초기화
// 				gData.byInspectDone[gData.nPNoSortPick[0]-1][gData.nTNoSortPick[0][nSp1StartNo+i]-1][gData.nCNoSortPick[0][nSp1StartNo+i]-1] |= (0 << 7);
				CString sInfo; 
				sInfo.Format("%d", gData.InfoNgTray[nSp1WorkNg][nSp1TrayPosY][nSp1TrayPosX+i]);
				int nTNo = gData.nTNoSortPick[0][nSp1StartNo+i];
				int nCNo = gData.nCNoSortPick[0][nSp1StartNo+i];
				if(gMes.sJudge[nPNo-1][nTNo-1][nCNo-1] == "") {
					if (gData.InfoNgTray[nSp1WorkNg][nSp1TrayPosY][nSp1TrayPosX+i] == 5) gMes.sJudge[nPNo-1][nTNo-1][nCNo-1] = "N2";
					else																 gMes.sJudge[nPNo-1][nTNo-1][nCNo-1] = "N1";
				}
				g_objMES.Set_Result(gLot.sLotID[nPNo-1], gMes.sBarID[nPNo-1][nTNo-1][nCNo-1], gMes.sJudge[nPNo-1][nTNo-1][nCNo-1], sInfo, gMes.sNGCode[nPNo-1][nTNo-1][nCNo-1], nTNo, nCNo, 0,0,0,0);
				g_objLogFile.Save_OutTray("NG", nSp1WorkNg, nSp1TrayPosX+i, nSp1TrayPosY, gData.nPNoSortPick[0], gData.nTNoSortPick[0][nSp1StartNo+i], gData.nCNoSortPick[0][nSp1StartNo+i]);

				gData.nTNoSortPick[0][nSp1StartNo+i] = gData.nCNoSortPick[0][nSp1StartNo+i] = 0;
				
			}
			gData.nULPNo = gData.nPNoNgTray = gData.nPNoSortPick[0];
			if (Check_SortPickerEmpty(1)) gData.nPNoSortPick[0] = 0;
			g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 5, nSp1WorkNg);

			m_pThreadVacuumSp1Multi = AfxBeginThread(Thread_Vacuum_Sp1_Multi, (LPVOID)(nSp1StartNo+1));
			m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000);
		}
		break;
	case 15 :	// Picker Up
		if (!m_pThreadVacuumSp1Multi && g_objCommon.Get_SortPicker1VacOffMulti(nSp1StartNo+1, nSp1DownSu)) {
			g_objCommon.Set_SortPicker1Up(0);
			g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 0);	// Ready Up
			
			m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(5000);
		}
		break;
	case 16:	// Positon Check
		if (g_objCommon.Get_SortPicker1Up(0) && g_objCommon.Get_InfoSortPicker1VacOn() && g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 0)) {
			if(!m_tSortPick1Loop.Waiting_Time(300)) break;
			m_tSortPick1Loop.Takt_Save(13, 4);

			if (Check_NgTrayFull()) {
				if (m_nNgTrayCase == 10) m_nNgTrayCase = 11;
			}
			m_nSortPick1Case = 10; m_tSortPick1Loop.Set_LoopTime(5000);
		}
		break;
	case 17:	// Wait
		if (g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 0) && g_objCommon.Check_Position(AX_SORT_PICKER1_P, 0)) {
			
			// Good Module이 있는지 확인 후 어디로 갈지 정한다.
			if (Select_SortPickGoodPos(1, nSp1StartNo, nSp1PickCnt)) {
				if ((m_nSortPick2Case >=  0 && m_nSortPick2Case < 10) ||
					(m_nSortPick2Case >=  9 && m_nSortPick2Case < 20) ||
					(m_nSortPick2Case >= 40 && m_nSortPick2Case < 60) ||
					(m_nSortPick2Case == 22) && (gData.nPNoSortPick[0] != gData.nPNoSortPick[1])||
					(m_nSortPick2Case == 27)) {

					if ((gData.nPNoSortPick[0] != gData.nPNoGoodTray) && (gData.nPNoSortPick[0] != 0)) {
						if (gData.nPNoGoodTray != 0) return TRUE;
					}
					m_tSortPick1Loop.Takt_Save(13, 5);
					m_nSortPick1Case = 20; m_tSortPick1Loop.Set_LoopTime(10000);

				} else if (m_nSortPick2Case >= 21 && m_nSortPick2Case < 30) {
					m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000);
				}
			} else {
				// NG Buffer에 Module이 있을 경우.
// 				if (!gData.bNGTrayWait && Check_NgBufferFull(1) && Check_SortPickerEmpty(1)) {
// 					m_nSortPick1Case = 50; m_tSortPick1Loop.Set_LoopTime(10000);
// 
// 				} else {
					if (Check_InspectLotEnd(gData.nPNoNgTray, 1)) {
						if (Check_NgBufferLotEnd(gData.nPNoNgTray)) {	// NG Buffer에 모듈이 없으면 LotEnd 모듈이 있으면 포트 넘버 확인 후 언로딩 작업.
							if (gData.bCycleStop == FALSE) g_objMES.LotEnd_WriteAPD();
							if(!m_tSortPick1Loop.Waiting_Time(700)) break;
// 							m_strLog.Format("NG Port No: %d, Sort1 Port No: %d, Sort2 Port No: %d",gData.nPNoNgTray, gData.nPNoSortPick[0], gData.nPNoSortPick[1]);
// 							g_objLogFile.Save_HandlerLog(m_strLog);
							Job_LotEnd(gData.nPNoNgTray);
							if (m_pThreadBeep == NULL && m_pThreadNgFullBeep == NULL) {
								m_pThreadBeep = AfxBeginThread(Thread_Beep, (LPVOID)(2000));

							} else {
								gData.bLotEndBeep = TRUE;
							}

							gData.bContinueLotEnd = TRUE;
							if (m_nNgTrayCase == 10) m_nNgTrayCase = 11; 
							if (!Check_GoodTrayEmpty()) {
								if (m_nGoodTray1Case == 20) { gData.bGoodTrayLotEnd[0] = TRUE; m_nGoodTray1Case = 21; }
								if (m_nGoodTray2Case == 20) { gData.bGoodTrayLotEnd[1] = TRUE; m_nGoodTray2Case = 21; }
							} else {
								if (m_nGoodTray1Case <= 20 && m_nGoodTray2Case <= 20) { gData.bGoodTrayWait = TRUE; }
								if (m_nGoodTray1Case <= 20 && m_nGoodTray2Case >  30) { gData.bGoodTrayWait = TRUE; }
								if (m_nGoodTray1Case >  30 && m_nGoodTray2Case <= 20) { gData.bGoodTrayWait = TRUE; }
							}
							if (!gData.bGoodTrayLotEnd[0] && m_nGoodTray1Case > 20 && m_nGoodTray1Case < 30) { gData.bGoodTrayLotEnd[0] = TRUE; }
							if (!gData.bGoodTrayLotEnd[1] && m_nGoodTray2Case > 20 && m_nGoodTray2Case < 30) { gData.bGoodTrayLotEnd[1] = TRUE; }

						} else {
							if (!Check_NgBufferEmpty(1) && Check_SortPickerEmpty(1)) {
								bLastNgBuffPick1 = TRUE;
								m_nSortPick1Case = 50; m_tSortPick1Loop.Set_LoopTime(10000);
								break;
							}
						}
					}
					if (Check_UnloadLotEnd() && !m_bUnloadLotEnd) { m_bUnloadLotEnd = TRUE; }

					m_tSortPick1Loop.Takt_Save(13, 5);
					m_strLog.Format("Sort Picker1, %d", GetTickCount() - m_dwSortPick1);
					g_objLogFile.Save_TestLog(m_strLog);

					if (nSp1WorkBuff != 1 && nSp1WorkBuff != 2) nSp1WorkBuff = 1;	// Test
					if (nSp1WorkBuff == 1) g_objCommon.Move_Position(AX_SORT_PICKER1_X, 0);	// Buffer1 Position
					if (nSp1WorkBuff == 2) g_objCommon.Move_Position(AX_SORT_PICKER1_X, 1);	// Buffer2 Position
					//g_objCommon.Set_SortPicker1Down(0);	// 복귀할때 실린더 다운해준다.

					if (m_nBuffTray1Case == 20)		 nSp1WorkBuff = 1;
					else if (m_nBuffTray2Case == 20) nSp1WorkBuff = 2;
					else							 nSp1WorkBuff = 0;

					if (!gData.bBuffStageMove && nSp1WorkBuff != 0 && Select_BufferTrayUpPos(nSp1WorkBuff, nSp1TrayPosX, nSp1TrayPosY)){	// 복귀할때 Y축 움직일수 있으면 움직여 주자.

						if (nSp1WorkBuff == 1 && m_nBuffTray1Case == 20) { 
							dSp1Y = m_pMoveData->dBufferStage1Y[2] + nSp1TrayPosY * m_pEquipData->dTrayPitchY;
							g_objAJinAXL.Move_Absolute(AX_BUFFER_STAGE1_Y, dSp1Y);
						}
						if (nSp1WorkBuff == 2 && m_nBuffTray2Case == 20) {
							dSp1Y = m_pMoveData->dBufferStage2Y[2] + nSp1TrayPosY * m_pEquipData->dTrayPitchY;
							g_objAJinAXL.Move_Absolute(AX_BUFFER_STAGE2_Y, dSp1Y);
						}
						gData.bBuffStageMove = TRUE;
					} else {
						if (gData.bBuffStageMove) gData.bBuffStageMove = FALSE;
					}

					m_nSortPick1Case = 0; m_tSortPick1Loop.Set_LoopTime(10000);
//				}
			}
		}		
		return TRUE;

	case 18:	// Move to NG Tray1 Position
		if (g_objAJinAXL.Is_Done(AX_SORT_PICKER1_X)) {
			g_objCommon.Move_Position(AX_SORT_PICKER1_X, 2);	// Good1
			m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000);
		}
		break;
	case 19:	// Check Position & Sort2
		if (g_objCommon.Check_Position(AX_SORT_PICKER1_X, 2)) {	// Good1
			if ((m_nSortPick2Case >=  0 && m_nSortPick2Case < 10) ||
				(m_nSortPick2Case >=  9 && m_nSortPick2Case < 20) ||
				(m_nSortPick2Case >= 40 && m_nSortPick2Case < 60) ||
				(m_nSortPick2Case == 27)) {

				if ((gData.nPNoSortPick[0] != gData.nPNoGoodTray) && (gData.nPNoSortPick[0] != 0)) {
					if (gData.nPNoGoodTray != 0) return TRUE;
				}
				m_nSortPick1Case = 20; m_tSortPick1Loop.Set_LoopTime(10000);
			}
		}
		return TRUE;

	// Good Unload
	case 20:	// Check Good Tray
		if (m_nGoodTray1Case == 20 || m_nGoodTray2Case == 20) {
			m_nSortPick1Case = 22; m_tSortPick1Loop.Set_LoopTime(10000); 
		} else {
			g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 0);	// Ready Up
			if (Check_SortPickerEmpty(1)) {				
				m_nSortPick1Case = 27; m_tSortPick1Loop.Set_LoopTime(10000);

			} else {
				if (Select_SortPickGoodPos(1, nSp1StartNo, nSp1PickCnt)) {
					g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 0);	// Ready Up
					g_objCommon.Move_Position(AX_SORT_PICKER1_X, 2);	// Good1
					m_nSortPick1Case = 21; m_tSortPick1Loop.Set_LoopTime(10000);
				} else {
					m_nSortPick1Case = 27; m_tSortPick1Loop.Set_LoopTime(10000);
				}
			}
		}
		break;
	case 21:	// Check Position
		if (g_objCommon.Check_Position(AX_SORT_PICKER1_X, 2)) {	// Good1
			if (m_nGoodTray1Case == 20 || m_nGoodTray2Case == 20) { m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000); }
			else if (Check_UnloadLotEnd()) { 
				g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 0);	// Ready Up
				m_nSortPick1Case = 27; m_tSortPick1Loop.Set_LoopTime(10000);
			}
		}
		return TRUE;

	case 22:	// Move to NG Unload Position
		if ((gData.nPNoSortPick[0] != gData.nPNoGoodTray) && (gData.nPNoSortPick[0] != 0)) {
			if (gData.nPNoGoodTray != 0) return TRUE;
		}
		if (m_nGoodTray1Case == 20 || m_nGoodTray2Case == 20) {
			if (m_nGoodTray1Case == 20) nSp1WorkGood = 1;
			if (m_nGoodTray2Case == 20) nSp1WorkGood = 2;
		
			if ((nSp1WorkGood == 1 && g_objAJinAXL.Is_Done(AX_GOOD_STAGE1_Y)) || 
				(nSp1WorkGood == 2 && g_objAJinAXL.Is_Done(AX_GOOD_STAGE2_Y))) {
				if (Select_GoodTrayPos(nSp1TrayPosX, nSp1TrayPosY)) {
					m_tSortPick1Loop.Takt_Start();
					if (Select_SortPickGoodPos(1, nSp1StartNo, nSp1PickCnt)) {	//startNo:0base, PickCnt:1base
						if (!Select_GoodTrayPos(nSp1TrayPosX, nSp1TrayPosY)) nSp1TrayPosX = 0;	// Tray 교체중...
						nSp1TrayCnt = gData.nTrayX - nSp1TrayPosX;
						nSp1DownSu = ((nSp1PickCnt < nSp1TrayCnt) ? nSp1PickCnt : nSp1TrayCnt);
						m_nSortPick1MultiCnt = nSp1DownSu;

						if (nSp1WorkGood == 1) dSp1X = m_pMoveData->dSortPicker1X[2] + (nSp1TrayPosX - nSp1StartNo) * m_pEquipData->dTrayPitchX;	// Good1
						if (nSp1WorkGood == 2) dSp1X = m_pMoveData->dSortPicker1X[3] + (nSp1TrayPosX - nSp1StartNo) * m_pEquipData->dTrayPitchX;	// Good2
						g_objAJinAXL.Move_Absolute(AX_SORT_PICKER1_X, dSp1X);

						if (nSp1WorkGood == 1 && m_nGoodTray1Case == 20) {
							dSp1Y = m_pMoveData->dGoodStage1Y[1] + nSp1TrayPosY * m_pEquipData->dTrayPitchY;
							g_objAJinAXL.Move_Absolute(AX_GOOD_STAGE1_Y, dSp1Y);
						}
						if (nSp1WorkGood == 2 && m_nGoodTray2Case == 20) {
							dSp1Y = m_pMoveData->dGoodStage2Y[1] + nSp1TrayPosY * m_pEquipData->dTrayPitchY;
							g_objAJinAXL.Move_Absolute(AX_GOOD_STAGE2_Y, dSp1Y);
						}
						m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000);

					} else {
						g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 0);	// Ready Up
						m_nSortPick1Case = 27; m_tSortPick1Loop.Set_LoopTime(10000);
					}
				}
			} else {
				break;
			}
		}
		return TRUE;
	case 23:	// Picker Down
		if (g_objAJinAXL.Is_MoveDone(AX_SORT_PICKER1_X, dSp1X) &&
			((nSp1WorkGood == 1 && g_objAJinAXL.Is_MoveDone(AX_GOOD_STAGE1_Y, dSp1Y)) ||
			 (nSp1WorkGood == 2 && g_objAJinAXL.Is_MoveDone(AX_GOOD_STAGE2_Y, dSp1Y))) ) {

			m_tSortPick1Loop.Takt_Save(13, 6);
			m_tSortPick1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 2);	//Good Down
			g_objCommon.Set_SortPicker1DownMulti(nSp1StartNo+1, nSp1DownSu);
			m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000);
		}
		break;
	case 24:	// 정보전달, Vac Off
		if (g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 2) && g_objCommon.Get_SortPicker1DownMulti(nSp1StartNo+1, nSp1DownSu)) {
			for (int i = 0; i < nSp1DownSu; i++) {
				gData.InfoGoodTray[nSp1TrayPosY][nSp1TrayPosX+i] = gData.InfoSortPick[0][nSp1StartNo+i]; 
				gData.InfoSortPick[0][nSp1StartNo+i] = 0;

// 				// 연속랏을 위해 모듈 언로딩 했으면 판정 완료 초기화
// 				gData.byInspectDone[gData.nPNoSortPick[0]-1][gData.nTNoSortPick[0][nSp1StartNo+i]-1][gData.nCNoSortPick[0][nSp1StartNo+i]-1] |= (0 << 7);
				CString sInfo; 
				sInfo.Format("%d", gData.InfoGoodTray[nSp1TrayPosY][nSp1TrayPosX+i]);
				int nTNo = gData.nTNoSortPick[0][nSp1StartNo+i];
				int nCNo = gData.nCNoSortPick[0][nSp1StartNo+i];
				g_objMES.Set_Result(gLot.sLotID[nPNo-1], gMes.sBarID[nPNo-1][nTNo-1][nCNo-1], "OK", sInfo, gMes.sNGCode[nPNo-1][nTNo-1][nCNo-1], nTNo, nCNo, 0,0,0,0);
				g_objLogFile.Save_OutTray("GOOD", gData.nGoodTrayCount, nSp1TrayPosX+i, nSp1TrayPosY, gData.nPNoSortPick[0], gData.nTNoSortPick[0][nSp1StartNo+i], gData.nCNoSortPick[0][nSp1StartNo+i]);

				gData.nTNoSortPick[0][nSp1StartNo+i] = gData.nCNoSortPick[0][nSp1StartNo+i] = 0;
			}
			gData.nULPNo = gData.nPNoGoodTray = gData.nPNoSortPick[0];
			if (Check_SortPickerEmpty(1)) gData.nPNoSortPick[0] = 0;
			g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 4, nSp1WorkGood-1);

			m_pThreadVacuumSp1Multi = AfxBeginThread(Thread_Vacuum_Sp1_Multi, (LPVOID)(nSp1StartNo+1));
			m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000);
		}
		break;
	case 25 :	// Picker Up
		if (!m_pThreadVacuumSp1Multi && g_objCommon.Get_SortPicker1VacOffMulti(nSp1StartNo+1, nSp1DownSu)) {
			g_objCommon.Set_SortPicker1Up(0);
			g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 0);	// Ready Up
			m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(5000);
		}
		break;
	case 26:	// Picker Check
		if (g_objCommon.Get_SortPicker1Up(0) && g_objCommon.Get_InfoSortPicker1VacOn() && g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 0)) {
			m_tSortPick1Loop.Takt_Save(13, 7);
			if (Check_GoodTrayFull()) {
				if (m_nGoodTray1Case == 20) m_nGoodTray1Case = 21;
				if (m_nGoodTray2Case == 20) m_nGoodTray2Case = 21;
			}
			if (Check_SortPickerEmpty(1)){
				g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 0);	// Ready Up
				m_nSortPick1Case = 27; m_tSortPick1Loop.Set_LoopTime(5000);
			} else {
				m_nSortPick1Case = 20; m_tSortPick1Loop.Set_LoopTime(5000);
			}
		}
		break;
	case 27:	// Positon Check
		if (g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 0)) {
			m_tSortPick1Loop.Takt_Save(13, 8);
			if (Check_SortPickerEmpty(1)){
				// NG Buffer에 Module이 있을 경우.
				if (!gData.bNGTrayWait && Check_NgBufferFull(1)) {
					m_nSortPick1Case = 50; m_tSortPick1Loop.Set_LoopTime(10000);

				} else {
					m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000);
				}
			} else {
				if (Select_SortPickNgPos(1, nSp1StartNo, nSp1PickCnt)) {
					m_nSortPick1Case = 7; m_tSortPick1Loop.Set_LoopTime(10000);
				}
			}
		}
		break;
	case 28:	// Buffer 복귀
		if (g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 0)) {
			if (Check_InspectLotEnd(gData.nULPNo, 1)) {
				if (Check_NgBufferLotEnd(gData.nULPNo)) {	// NG Buffer에 모듈이 없으면 LotEnd 모듈이 있으면 포트 넘버 확인 후 언로딩 작업.
					if (gData.bCycleStop == FALSE) g_objMES.LotEnd_WriteAPD();
					if(!m_tSortPick1Loop.Waiting_Time(700)) break;
					Job_LotEnd(gData.nULPNo);
					if (m_pThreadBeep == NULL && m_pThreadNgFullBeep == NULL) {						
						m_pThreadBeep = AfxBeginThread(Thread_Beep, (LPVOID)(2000));

					} else {
						gData.bLotEndBeep = TRUE;
					}

					gData.bContinueLotEnd = TRUE;
					if (m_nNgTrayCase == 10) m_nNgTrayCase = 11; 
					if (!Check_GoodTrayEmpty()) {
						if (m_nGoodTray1Case == 20) { gData.bGoodTrayLotEnd[0]= TRUE; m_nGoodTray1Case = 21; }
						if (m_nGoodTray2Case == 20) { gData.bGoodTrayLotEnd[1]= TRUE; m_nGoodTray2Case = 21; }
					}  else {
						if (m_nGoodTray1Case <= 20 && m_nGoodTray2Case <= 20) { gData.bGoodTrayWait = TRUE; }
						if (m_nGoodTray1Case <= 20 && m_nGoodTray2Case >  30) { gData.bGoodTrayWait = TRUE; }
						if (m_nGoodTray1Case >  30 && m_nGoodTray2Case <= 20) { gData.bGoodTrayWait = TRUE; }
					}
					if (!gData.bGoodTrayLotEnd[0] && m_nGoodTray1Case > 20 && m_nGoodTray1Case < 30) { gData.bGoodTrayLotEnd[0] = TRUE; }
					if (!gData.bGoodTrayLotEnd[1] && m_nGoodTray2Case > 20 && m_nGoodTray2Case < 30) { gData.bGoodTrayLotEnd[1] = TRUE; }

				} else {
					if (!Check_NgBufferEmpty(1) && Check_SortPickerEmpty(1)) {
						bLastNgBuffPick1 = TRUE;
						m_nSortPick1Case = 50; m_tSortPick1Loop.Set_LoopTime(10000);
						break;
					}
				}
			}
			if (Check_UnloadLotEnd() && !m_bUnloadLotEnd) { m_bUnloadLotEnd = TRUE; }

			//m_tSortPick1Loop.Takt_Start();
			m_strLog.Format("Sort Picker1, %d", GetTickCount() - m_dwSortPick1);
			g_objLogFile.Save_TestLog(m_strLog);
			if (nSp1WorkBuff != 1 && nSp1WorkBuff != 2) nSp1WorkBuff = 1;	// Test
			if (nSp1WorkBuff == 1) g_objCommon.Move_Position(AX_SORT_PICKER1_X, 0);	// Buffer1 Position
			if (nSp1WorkBuff == 2) g_objCommon.Move_Position(AX_SORT_PICKER1_X, 1);	// Buffer2 Position
			//g_objCommon.Set_SortPicker1Down(0);	// 복귀할때 실린더 다운해준다.

			if (m_nBuffTray1Case == 20)		 nSp1WorkBuff = 1;
			else if (m_nBuffTray2Case == 20) nSp1WorkBuff = 2;
			else							 nSp1WorkBuff = 0;

			if (!gData.bBuffStageMove && nSp1WorkBuff != 0 && Select_BufferTrayUpPos(nSp1WorkBuff, nSp1TrayPosX, nSp1TrayPosY)){	// 복귀할때 Y축 움직일수 있으면 움직여 주자.

				if (nSp1WorkBuff == 1 && m_nBuffTray1Case == 20) { 
					dSp1Y = m_pMoveData->dBufferStage1Y[2] + nSp1TrayPosY * m_pEquipData->dTrayPitchY;
					g_objAJinAXL.Move_Absolute(AX_BUFFER_STAGE1_Y, dSp1Y);
				}
				if (nSp1WorkBuff == 2 && m_nBuffTray2Case == 20) {
					dSp1Y = m_pMoveData->dBufferStage2Y[2] + nSp1TrayPosY * m_pEquipData->dTrayPitchY;
					g_objAJinAXL.Move_Absolute(AX_BUFFER_STAGE2_Y, dSp1Y);
				}
				gData.bBuffStageMove = TRUE;
			} else {
				if (gData.bBuffStageMove) gData.bBuffStageMove = FALSE;
			}

			m_nSortPick1Case = 0; m_tSortPick1Loop.Set_LoopTime(10000);
		}		
		break;
// 	case 29:	// Position Check
// 		if ((nSp1WorkBuff == 1 && g_objCommon.Check_Position(AX_SORT_PICKER1_X, 0)) || 
// 			(nSp1WorkBuff == 2 && g_objCommon.Check_Position(AX_SORT_PICKER1_X, 1))) {
// 
// 			m_tSortPick1Loop.Takt_Save(13, 9);
// 			m_strLog.Format("Sort Picker1, %d", GetTickCount() - m_dwSortPick1);
// 			g_objLogFile.Save_TestLog(m_strLog);
// 			m_nSortPick1Case = 0; m_tSortPick1Loop.Set_LoopTime(10000);
// 		}
// 		break;

	// NG Buffer place
	case 40:	// Move to NG Buffer Position
		if (g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 0) && g_objAJinAXL.Is_Done(AX_SORT_PICKER1_X) && g_objAJinAXL.Is_Done(AX_SORT_PICKER1_P)) {
			if (Select_SortPickNgPos(1, nSp1StartNo, nSp1PickCnt, TRUE)) {	// startNo 0base pickCnt 1base
				if (Select_NgBufferPos(1, nSp1TrayPosX)) {	// Ng Buffer에 자리가 있을때
					m_tSortPick1Loop.Takt_Start();			
					nSp1TrayCnt = gData.nSortPickQt - nSp1TrayPosX;
					nSp1DownSu = ((nSp1PickCnt < nSp1TrayCnt) ? nSp1PickCnt : nSp1TrayCnt);
					m_nSortPick1MultiCnt = nSp1DownSu;

					dSp1X = m_pMoveData->dSortPicker1X[6] + (nSp1TrayPosX - nSp1StartNo) * m_pEquipData->dNgBufferPitchX;

					g_objAJinAXL.Move_Absolute(AX_SORT_PICKER1_X, dSp1X);
					g_objCommon.Move_Position(AX_SORT_PICKER1_P, 1);	// NG Buffer Pitch
					m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000);

				} else {	// Ng Buffer에 자리가 없고 Picker에 Ng가 있을때
					g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 0);	// Ready Up
					g_objCommon.Move_Position(AX_SORT_PICKER1_P, 0);	// Tray Pitch
					m_nSortPick1Case = 10; m_tSortPick1Loop.Set_LoopTime(10000);
				}

			} else {	// Picker에 Ng가 없을때
				g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 0);	// Ready Up
				g_objCommon.Move_Position(AX_SORT_PICKER1_P, 0);	// Tray Pitch
				m_nSortPick1Case = 17; m_tSortPick1Loop.Set_LoopTime(10000);
			}
		}
		break;
	case 41:	// Check Position
		if (g_objAJinAXL.Is_MoveDone(AX_SORT_PICKER1_X, dSp1X) && g_objCommon.Check_Position(AX_SORT_PICKER1_P, 1)) {
			m_tSortPick1Loop.Takt_Save(13, 10);
			m_tSortPick1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 4);	//NG Buffer Down
			g_objCommon.Set_SortPicker1DownMulti(nSp1StartNo+1, nSp1DownSu);
			m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000);
		}
		break;
	case 42:	// 정보전달, Vac Off	// Sort Picker = 0, NG Buffer = 1
		if (g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 4) && g_objCommon.Get_SortPicker1DownMulti(nSp1StartNo+1, nSp1DownSu)) {
			for (int i = 0; i < nSp1DownSu; i++) {
				gData.InfoNgBuffer[1][nSp1TrayPosX+i] = gData.InfoSortPick[0][nSp1StartNo+i]; 
				gData.nTNoNgBuffer[1][nSp1TrayPosX+i] = gData.nTNoSortPick[0][nSp1StartNo+i];
				gData.nCNoNgBuffer[1][nSp1TrayPosX+i] = gData.nCNoSortPick[0][nSp1StartNo+i];

				gData.InfoSortPick[0][nSp1StartNo+i] = gData.nTNoSortPick[0][nSp1StartNo+i] = gData.nCNoSortPick[0][nSp1StartNo+i] = 0;

			}
			gData.nPNoNgBuffer[1] = gData.nPNoSortPick[0];
			if (Check_SortPickerEmpty(1)) gData.nPNoSortPick[0] = 0;

			g_objCommon.Set_InfoNgBufferVacOn(1);
			m_pThreadVacuumSp1Multi = AfxBeginThread(Thread_Vacuum_Sp1_Multi, (LPVOID)(nSp1StartNo+1));
			m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000);
		}
		break;
	case 43 :	// Picker Up
		if (!m_pThreadVacuumSp1Multi && g_objCommon.Get_InfoNgBufferVacOn(1) && g_objCommon.Get_SortPicker1VacOffMulti(nSp1StartNo+1, nSp1DownSu)) {
			g_objCommon.Set_SortPicker1Up(0);
			g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 0);	// Ready Up
			m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(5000);
		}
		break;
	case 44:	// Positon Check
		if (g_objCommon.Get_SortPicker1Up(0) && g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 0)) {
			m_tSortPick1Loop.Takt_Save(13, 11);

			if (Select_SortPickNgPos(1, nSp1StartNo, nSp1PickCnt, TRUE)) {
				if (Check_NgBufferFull(1)) {
					g_objCommon.Move_Position(AX_SORT_PICKER1_P, 0);	// Tray Pitch
					m_nSortPick1Case = 7; m_tSortPick1Loop.Set_LoopTime(5000);

				} else {
					m_nSortPick1Case = 40; m_tSortPick1Loop.Set_LoopTime(5000);
				}
			} else {
				g_objCommon.Move_Position(AX_SORT_PICKER1_P, 0);	// Tray Pitch
				m_nSortPick1Case = 17; m_tSortPick1Loop.Set_LoopTime(10000);
			}			
		}
		break;

	// NG Buffer Pickup
	case 50:
		if (g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 0) && g_objAJinAXL.Is_Done(AX_SORT_PICKER1_X) && g_objAJinAXL.Is_Done(AX_SORT_PICKER1_P)) {
			if (Check_SortPickerEmpty(1)) {
				g_objCommon.Move_Position(AX_SORT_PICKER1_X, 6);	// NG Buffer Position
				g_objCommon.Move_Position(AX_SORT_PICKER1_P, 1);	// NG Buffer Pitch
				m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000);
			}
		}
		break;
	case 51:	// Check Position
		if (g_objCommon.Check_Position(AX_SORT_PICKER1_X, 6) && g_objCommon.Check_Position(AX_SORT_PICKER1_P, 1)) {
			m_tSortPick1Loop.Takt_Save(13, 10);
			m_tSortPick1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 4);	//NG Buffer Down
			g_objCommon.Set_InfoSortPicker1Down(1);
			g_objCommon.Set_InfoSortPicker1VacOn(1);
			m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000);
		}
		break;
	case 52:	// 정보전달, Vac Off
		if (g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 4) && g_objCommon.Get_InfoSortPicker1Down(1)) {
			for (int i = 0; i < gData.nSortPickQt; i++) {
				gData.InfoSortPick[0][i] = gData.InfoNgBuffer[1][i]; 
				gData.nTNoSortPick[0][i] = gData.nTNoNgBuffer[1][i];
				gData.nCNoSortPick[0][i] = gData.nCNoNgBuffer[1][i];

				gData.InfoNgBuffer[1][i] = gData.nTNoNgBuffer[1][i] = gData.nCNoNgBuffer[1][i] = 0;

			}
			gData.nPNoSortPick[0] = gData.nPNoNgBuffer[1]; gData.nPNoNgBuffer[1] = 0;

			g_objCommon.Set_NgBufferVacOff(1);
			m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(10000);
		}
		break;
	case 53 :	// Picker Up
		if (!m_tSortPick1Loop.Waiting_Time(m_pEquipData->nDelayAdd[2])) break;	// SortPicker Delay
		
		g_objCommon.Set_SortPicker1Up(0);
		g_objCommon.Move_Position(AX_SORT_PICKER1_Z, 0);	// Ready Up
		m_nSortPick1Case++; m_tSortPick1Loop.Set_LoopTime(5000);
		break;
	case 54:
		if (g_objCommon.Get_InfoSortPicker1VacOn() && 
			g_objCommon.Check_Position(AX_SORT_PICKER1_Z, 0) && g_objCommon.Get_SortPicker1Up(0)) {

			m_tSortPick1Loop.Takt_Save(13, 12);
			g_objCommon.Move_Position(AX_SORT_PICKER1_P, 0);	// Tray Pitch
			m_nSortPick1Case = 7; m_tSortPick1Loop.Set_LoopTime(5000);
		}
		break;
	}

	// 13. (Error : 4300)
	if (m_tSortPick1Loop.Over_LoopTime()) {
		if (m_nSortPick1Case == 5) {
			if (nSp1VacErr == 1) { gData.nErrBufferStageNo = nSp1WorkBuff; }
			if (!g_objCommon.Get_InfoSortPicker1VacOn() && nSp1VacErr == 0) {
				CCMI8000Dlg *pMainDlg = (CCMI8000Dlg*)AfxGetApp()->GetMainWnd();
				pMainDlg->Set_LotErrorLog("RETRY", 4305, "Sort1 Retry", gData.nPNoSortPick[0]);

				nSp1VacErr = 1; 
				m_nSortPick1Case = 2; m_tSortPick1Loop.Set_LoopTime(5000); return TRUE;
			}
		} else { gData.nErrBufferStageNo = 0; }

		if (m_nSortPick1Case == 6) {
			for (int i = 4; i >= 0; i--) { Set_InspectError(gData.nPNoSortPick[0], gData.nTNoSortPick[0][i], gData.nCNoSortPick[0][i]); }
		}
		g_objCommon.Show_Error(4300 + m_nSortPick1Case);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 14. (Error : 4400)
BOOL CSequenceMain::SortPicker2_Run()
{
	static int		nSp2WorkBuff = 0;
	static int		nSp2WorkGood = 0;
	static int		nSp2WorkNg = 0;
	static int		nSp2TrayPosX, nSp2TrayPosY = 0;
	static double	dSp2X, dSp2Y = 0.0;
	static int		nSp2TrayCnt = 0, nSp2PickCnt = 0, nSp2StartNo = 0, nSp2DownSu = 0, nSp2Base = 0, nPNo = 0;
	static int		nSp2VacErr = 0;
	static BOOL		bLastNgBuffPick2 = FALSE;
	BOOL bS2Checking = FALSE;							// Inspect Done Check

	switch (m_nSortPick2Case) {
	case 0:		// Wait
		if (m_nBuffTray1Case == 20 || m_nBuffTray2Case == 20) {	// Unload 위치
			if (!Check_NgBufferEmpty(2) && Check_SortPickerEmpty(2)) {
				if (m_nBuffTray1Case == 20) nSp2WorkBuff = 1;
				if (m_nBuffTray2Case == 20) nSp2WorkBuff = 2;
				if (gData.nPNoNgTray != 0 && (gData.nPNoNgTray != gData.nPNoBuffTray[nSp2WorkBuff-1])) {
					bLastNgBuffPick2 = TRUE;
					m_nSortPick2Case = 50; m_tSortPick2Loop.Set_LoopTime(10000);
					break;
				}
			}
			//if ((m_nSortPick1Case >= 10 && m_nSortPick1Case < 30) || (m_nSortPick1Case >= 40 && m_nSortPick1Case < 60)) {	// Sort1이 NG or Good Unload 작업중.
			if ((m_nSortPick1Case >= 6 && m_nSortPick1Case < 30) || (m_nSortPick1Case >= 40 && m_nSortPick1Case < 60)) {	// Sort1이 NG or Good Unload 작업중.
				m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(5000);
			}
		} else {
			if (!gData.bNGTrayWait && !Check_NgBufferEmpty(2) && Check_SortPickerEmpty(2)) {
				m_nSortPick2Case = 50; m_tSortPick2Loop.Set_LoopTime(10000);
			}
		}
		return TRUE;

	case 1:		// Select Pick Position
		if ((g_objCommon.Check_Position(AX_SORT_PICKER2_X, 0) || g_objCommon.Check_Position(AX_SORT_PICKER2_X, 1)) &&
			g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 0) && g_objCommon.Check_Position(AX_SORT_PICKER2_P, 0)){
			if (m_nBuffTray1Case == 20 || m_nBuffTray2Case == 20) {
				if (m_nBuffTray1Case == 20) nSp2WorkBuff = 1;
				if (m_nBuffTray2Case == 20) nSp2WorkBuff = 2;
				if (nSp2WorkBuff != 1 && nSp2WorkBuff != 2) break;	// Test Interlock
				if ((nSp2WorkBuff == 1 && !g_objAJinAXL.Is_Done(AX_BUFFER_STAGE1_Y)) ||
					(nSp2WorkBuff == 2 && !g_objAJinAXL.Is_Done(AX_BUFFER_STAGE2_Y))) break;

				gData.bBuffStageMove = TRUE;
				if (Check_SortPickerEmpty(2)) {
					m_dwSortPick2 = GetTickCount();
					m_tSortPick2Loop.Takt_Start();

					if (!Select_BufferTrayUpPos(nSp2WorkBuff, nSp2TrayPosX, nSp2TrayPosY)) nSp2TrayPosX = 0;	// Good Tray 교체중...

					// R04C는 Tray 8x4이다
					if (gData.bR04C) {
						if (nSp2TrayPosX < 4)	{ nSp2Base = 0; dSp2X = m_pMoveData->dSortPicker2X[nSp2WorkBuff-1]; }
						else					{ nSp2Base = 4; dSp2X = m_pMoveData->dSortPicker2X[nSp2WorkBuff-1] + m_pEquipData->dTrayPitchX * 4; }

					} else { nSp2Base = 0; dSp2X = m_pMoveData->dSortPicker2X[nSp2WorkBuff-1]; }
					g_objAJinAXL.Move_Absolute(AX_SORT_PICKER2_X, dSp2X);

					if (nSp2WorkBuff == 1 && m_nBuffTray1Case == 20) { 
						dSp2Y = m_pMoveData->dBufferStage1Y[3] + nSp2TrayPosY * m_pEquipData->dTrayPitchY;
						g_objAJinAXL.Move_Absolute(AX_BUFFER_STAGE1_Y, dSp2Y);
					}
					if (nSp2WorkBuff == 2 && m_nBuffTray2Case == 20) { 
						dSp2Y = m_pMoveData->dBufferStage2Y[3] + nSp2TrayPosY * m_pEquipData->dTrayPitchY;
						g_objAJinAXL.Move_Absolute(AX_BUFFER_STAGE2_Y, dSp2Y);
					}
					nSp2VacErr = 0;
					m_nSortPick2Case++; m_tSortPick1Loop.Set_LoopTime(10000);
				}
// 					if (!Select_BufferTrayUpPos(nSp2WorkNo, nSp2TrayPosX, nSp2TrayPosY)) nSp2TrayPosX = 0;	// Good Tray 교체중...
// 
// 					dSp2X = m_pMoveData->dSortPicker2X[0] + nSp2TrayPosX * m_pEquipData->dTrayPitchX;
// 					g_objAJinAXL.Move_Absolute(AX_SORT_PICKER2_X, dSp2Y);
// 
// 					if (nSp2WorkNo == 1 && m_nBuffTray1Case == 20) {
// 						dSp2Y = m_pMoveData->dBufferStage1Y[2] + nSp2TrayPosY * m_pEquipData->dTrayPitchY;
// 						g_objAJinAXL.Move_Absolute(AX_BUFFER_STAGE1_Y, dSp2Y);
// 					}
// 					if (nSp2WorkNo == 2 && m_nBuffTray2Case == 20) {
// 						dSp2Y = m_pMoveData->dBufferStage2Y[2] + nSp2TrayPosY * m_pEquipData->dTrayPitchY;
// 						g_objAJinAXL.Move_Absolute(AX_BUFFER_STAGE2_Y, dSp2Y);
// 					}
// 					m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000);
// 				}
			}
		}
		break;
	case 2:		// Picker Down
		if (g_objAJinAXL.Is_MoveDone(AX_SORT_PICKER2_X, dSp2X) &&
			((nSp2WorkBuff == 1 && g_objAJinAXL.Is_MoveDone(AX_BUFFER_STAGE1_Y, dSp2Y)) ||
			 (nSp2WorkBuff == 2 && g_objAJinAXL.Is_MoveDone(AX_BUFFER_STAGE2_Y, dSp2Y))) ) {

				if (nSp2VacErr == 0) m_tSortPick2Loop.Takt_Save(14, 1);
				m_tSortPick2Loop.Takt_Start();

				if(nSp2WorkBuff == 1) g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 1);	//Buffer 1 Down
				if(nSp2WorkBuff == 2) g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 5);	//Buffer 2 Down

				g_objCommon.Set_SortPicker2Down(0);	// All Down
				m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000);
		} 
		break;
	case 3:		// 정보전달, Picker Vac On
		if (((nSp2WorkBuff == 1 && g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 1)) ||
			 (nSp2WorkBuff == 2 && g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 5))) &&
			g_objCommon.Get_SortPicker2Down(0)) {

			if (nSp2VacErr == 1) { m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000); break;}

			for (int i = 0; i < gData.nSortPickQt; i++) {
				gData.InfoSortPick[1][i] = gData.InfoBuffTray[nSp2WorkBuff-1][nSp2TrayPosY][nSp2Base+i]; 
				gData.InfoBuffTray[nSp2WorkBuff-1][nSp2TrayPosY][nSp2Base+i] = 0;

				gData.nTNoSortPick[1][i] = gData.nTNoBuffTray[nSp2WorkBuff-1][nSp2TrayPosY][nSp2Base+i];
				gData.nTNoBuffTray[nSp2WorkBuff-1][nSp2TrayPosY][nSp2Base+i] = 0;
				gData.nCNoSortPick[1][i] = gData.nCNoBuffTray[nSp2WorkBuff-1][nSp2TrayPosY][nSp2Base+i];
				gData.nCNoBuffTray[nSp2WorkBuff-1][nSp2TrayPosY][nSp2Base+i] = 0;
			}
			gData.nPNoSortPick[1] = gData.nPNoBuffTray[nSp2WorkBuff-1];

			if ((gData.nCNoSortPick[1][0] == 1 || gData.nCNoSortPick[1][0] == 6 || gData.nCNoSortPick[1][0] == 11) && nSp2TrayPosY == 0) {
				if (gData.nTNoSortPick[1][0] == 1 && gData.nCNoSortPick[1][0] == 1) {
					m_dwULCycleTime = 0;
					m_dwULPickUpTime = GetTickCount();

				} else {
					m_dwULCycleTime = GetTickCount() - m_dwULPickUpTime;
					m_dwULPickUpTime = GetTickCount();
					g_dlgWork.PostMessage(UM_UPDATE_UPH, 1, NULL);
				}
			}

			if (Check_BufferEmpty(nSp2WorkBuff)) {
				gData.nPNoBuffTray[nSp2WorkBuff-1] = 0;
			}
			g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 3, nSp2WorkBuff-1);

			g_objCommon.Set_InfoSortPicker2VacOn();
			m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000);
		} 
		break;
	case 4:		// Picker Up
		//if (g_objCommon.Get_InfoSortPicker2VacOn()) {	// Delay Time만 사용한다.
		if (nSp2VacErr == 0) { if (!m_tSortPick2Loop.Waiting_Time(m_pEquipData->nDelayAdd[2])) break; }	// SortPicker Delay
		else				 { if (!m_tSortPick2Loop.Waiting_Time(m_pEquipData->nDelayAdd[2]*2)) break; }

			g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 0);	//Ready Up
			g_objCommon.Set_SortPicker2Up(0);	// All Up
			m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(6000);
		//}
		break;
	case 5:
		if (g_objCommon.Get_InfoSortPicker2VacOn() && 
			g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 0) && g_objCommon.Get_SortPicker2Up(0)) {

			m_tSortPick2Loop.Takt_Save(14, 2);
			if (Check_BufferEmpty(nSp2WorkBuff)) {	// Tray Change
				nSp2TrayPosY = 0;
				if (nSp2WorkBuff == 1 && m_nBuffTray1Case == 20) m_nBuffTray1Case = 21;
				if (nSp2WorkBuff == 2 && m_nBuffTray2Case == 20) m_nBuffTray2Case = 21;
			}
			gData.bBuffStageMove = FALSE;
			gData.nSp2Timer = GetTickCount();
			SYSTEMTIME time;
			GetLocalTime(&time);
			gData.sSortWaitStartTime[1].Format("%02d:%02d:%02d %03d", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

			m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(30000);	// 검사 완료
		} 
		break;
	case 6:		// NG or Good
		for (int i = 4; i >= 0; i--) { if (!Check_InspectDone(gData.nPNoSortPick[1], gData.nTNoSortPick[1][i], gData.nCNoSortPick[1][i], gData.InfoSortPick[1][i], 2)) { bS2Checking = TRUE; break; } }
		if (bS2Checking) break;	// Not Complete

		nPNo = gData.nPNoSortPick[1];
		for (int i = 0; i < 5; i++) {
			if (gData.InfoSortPick[1][i] == 1) gLot.nGoodCount[nPNo-1]++;
			else if (gData.InfoSortPick[1][i] != 0) gLot.nNgCount[nPNo-1]++;
			for (int j = 0; j < 5; j++) { if (gData.InfoSortPick[1][i] == j + 3) gLot.nSNgCount[nPNo-1][j]++; }	// S-NG (3,4,5,6,7,8)
			Write_LotJudge(nPNo, gData.nTNoSortPick[1][i], gData.nCNoSortPick[1][i], gData.InfoSortPick[1][i]);
		}
		g_dlgWork.PostMessage(UM_VISION_RESULT, nPNo, NULL);

		//	NG가 있으면 무조건 NG 부터 간다.
		if (Select_SortPickNgPos(2, nSp2StartNo, nSp2PickCnt)) {	// NG
			m_nSortPick2Case = 7; m_tSortPick2Loop.Set_LoopTime(10000);
		} else {
			if (Select_SortPickGoodPos(2, nSp2StartNo, nSp2PickCnt)) {	// NG 없고 Good만 있을때
				m_nSortPick2Case = 17; m_tSortPick2Loop.Set_LoopTime(10000);
			}
		}
		break;
	case 7:	// Wait Move to Ng Position
		// SortPicker1번이 Pickup 작업중, Good Unload 작업중,  NG Buffer 작업중이면 NG로 갈수있다.이면 NG로 갈수있다.
		if ((m_nSortPick1Case >=  0 && m_nSortPick1Case < 10) ||
			(m_nSortPick1Case >= 19 && m_nSortPick1Case < 30) ||
			(m_nSortPick1Case >= 40 && m_nSortPick1Case < 60) ||
			(m_nSortPick1Case == 12) && (gData.nPNoSortPick[0] != gData.nPNoSortPick[1])||
			(m_nSortPick1Case == 17)) {

			if ((gData.nPNoSortPick[1] != gData.nPNoNgTray) && (gData.nPNoSortPick[1] != 0)) {
				if (gData.nPNoNgTray != 0) return TRUE;
			}
			m_nSortPick2Case = 10; m_tSortPick2Loop.Set_LoopTime(10000);

		} else {
			if (Select_SortPickGoodPos(2, nSp2StartNo, nSp2PickCnt)) {		// Sort1번이 NG 작업 중이고 Good이 있다면 Good Unload 먼저 해준다.
				m_nSortPick2Case = 17; m_tSortPick2Loop.Set_LoopTime(10000);

			} else if (m_nSortPick1Case >= 11 && m_nSortPick1Case < 20) {	// Sort1번이 NG 작업 중이고 NG만 있다면 NG Tray 1번 위치로 가서 대기한다.
				m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000);
			}
		}
		return TRUE;

	case 8:	// Move to NG Tray1 Position
		if (g_objAJinAXL.Is_Done(AX_SORT_PICKER2_X)) {
			g_objCommon.Move_Position(AX_SORT_PICKER2_X, 4);
			m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000);
		}
		break;
	case 9:	// Check Position & Sort1
		if (g_objCommon.Check_Position(AX_SORT_PICKER2_X, 4)) {
			// SortPicker2번이 Pickup 작업중, Good Unload 작업중,  NG Buffer 작업중이면 NG Unloading 할수있다.
			if ((m_nSortPick1Case >=  0 && m_nSortPick1Case < 10) ||
				(m_nSortPick1Case >= 19 && m_nSortPick1Case < 30) ||
				(m_nSortPick1Case >= 40 && m_nSortPick1Case < 60) ||
				(m_nSortPick1Case == 17)) {

				if ((gData.nPNoSortPick[1] != gData.nPNoNgTray) && (gData.nPNoSortPick[1] != 0)) {
					if (gData.nPNoNgTray != 0) return TRUE;
				}
				m_nSortPick2Case = 10; m_tSortPick2Loop.Set_LoopTime(10000);
			}
		}
		return TRUE;

	// NG Unload
	case 10:	// Check NG Tray
		// NG Tray가 받을 준비 됐는지 확인한다.
		if (m_nNgTrayCase == 10) {
			m_nSortPick2Case = 12; m_tSortPick2Loop.Set_LoopTime(10000); 

		} else {
			if (g_objAJinAXL.Is_Done(AX_SORT_PICKER2_Z) && g_objAJinAXL.Is_Done(AX_SORT_PICKER2_X)) {
				g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 0);	// Ready Up

				if (Select_SortPickNgPos(2, nSp2StartNo, nSp2PickCnt)) {
					if (bLastNgBuffPick2 == FALSE && !Check_UnloadLotEnd(1) && !Check_NgBufferFull(2)) { // Ng Tray 교체 작업 중에 Ng Buffer에 내려놓는다.
						m_nSortPick2Case = 40; m_tSortPick2Loop.Set_LoopTime(10000);	

					} else {
						g_objCommon.Move_Position(AX_SORT_PICKER2_X, 5);
						m_nSortPick2Case = 11; m_tSortPick2Loop.Set_LoopTime(10000);
					}
				} else {
					m_nSortPick2Case = 17; m_tSortPick2Loop.Set_LoopTime(10000);
				}
			}
		}
		break;
	case 11:	// Check Position
		if (g_objCommon.Check_Position(AX_SORT_PICKER2_X, 5)) {
			if (m_nNgTrayCase == 10) { m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000); }
		}
		return TRUE;

	case 12:	// Move to NG Unload Position
		if ((gData.nPNoSortPick[1] != gData.nPNoNgTray) && (gData.nPNoSortPick[1] != 0)) {
			if (gData.nPNoNgTray != 0) return TRUE;
		}
		if (m_nNgTrayCase != 10) { m_nSortPick2Case = 10; m_tSortPick2Loop.Set_LoopTime(10000); return TRUE; }
		if (bLastNgBuffPick2 == TRUE) bLastNgBuffPick2 = FALSE;

		if (Select_SortPickNgPos(2, nSp2StartNo, nSp2PickCnt)) {
			if (Select_NgTrayPos(2, nSp2WorkNg, nSp2TrayPosX, nSp2TrayPosY)) {
				m_tSortPick2Loop.Takt_Start();
				if (!Select_NgTrayPos(2, nSp2WorkNg, nSp2TrayPosX, nSp2TrayPosY)) nSp2TrayPosX = 0;	// Tray 교체중...
				nSp2TrayCnt = gData.nTrayX - nSp2TrayPosX;
				nSp2DownSu = ((nSp2PickCnt < nSp2TrayCnt) ? nSp2PickCnt : nSp2TrayCnt);
				m_nSortPick2MultiCnt = nSp2DownSu;

				int nNgTrayY = nSp2WorkNg / 2;
				int nNgTrayX = nSp2WorkNg % 2;
				dSp2X = m_pMoveData->dSortPicker2X[4 + nNgTrayX] + (nSp2TrayPosX - nSp2StartNo) * m_pEquipData->dTrayPitchX;
				dSp2Y = m_pMoveData->dNgStageY[3 + nNgTrayY] + nSp2TrayPosY * m_pEquipData->dTrayPitchY;

				g_objAJinAXL.Move_Absolute(AX_SORT_PICKER2_X, dSp2X);
				g_objAJinAXL.Move_Absolute(AX_NG_STAGE_Y, dSp2Y);
				m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000);

			}
		} else {
			g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 0);	// Ready Up
			m_nSortPick2Case = 17; m_tSortPick2Loop.Set_LoopTime(10000);
		}
		break;
	case 13:	// Picker Down
		if (g_objAJinAXL.Is_MoveDone(AX_SORT_PICKER2_X, dSp2X) && g_objAJinAXL.Is_MoveDone(AX_NG_STAGE_Y, dSp2Y) && g_objCommon.Check_Position(AX_SORT_PICKER2_P, 0)) {
			m_tSortPick2Loop.Takt_Save(14, 3);
			m_tSortPick2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 3);	//NG Down
			g_objCommon.Set_SortPicker2DownMulti(nSp2StartNo+1, nSp2DownSu);
			m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000);
		}
		break;
	case 14:	// 정보전달, Vac Off
		if (g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 3) && g_objCommon.Get_SortPicker2DownMulti(nSp2StartNo+1, nSp2DownSu)) {
			for (int i = 0; i < nSp2DownSu; i++) {
				gData.InfoNgTray[nSp2WorkNg][nSp2TrayPosY][nSp2TrayPosX+i] = gData.InfoSortPick[1][nSp2StartNo+i]; 
				gData.InfoSortPick[1][nSp2StartNo+i] = 0;

// 				// 연속랏을 위해 모듈 언로딩 했으면 판정 완료 초기화
// 				gData.byInspectDone[gData.nPNoSortPick[1]-1][gData.nTNoSortPick[1][nSp2StartNo+i]-1][gData.nCNoSortPick[1][nSp2StartNo+i]-1] |= (0 << 7);
				CString sInfo; 
				sInfo.Format("%d", gData.InfoNgTray[nSp2WorkNg][nSp2TrayPosY][nSp2TrayPosX+i]);
				int nTNo = gData.nTNoSortPick[1][nSp2StartNo+i];
				int nCNo = gData.nCNoSortPick[1][nSp2StartNo+i];
				if(gMes.sJudge[nPNo-1][nTNo-1][nCNo-1] == "") {
					if (gData.InfoNgTray[nSp2WorkNg][nSp2TrayPosY][nSp2TrayPosX+i] == 5) gMes.sJudge[nPNo-1][nTNo-1][nCNo-1] = "N2";
					else																 gMes.sJudge[nPNo-1][nTNo-1][nCNo-1] = "N1";
				}
				g_objMES.Set_Result(gLot.sLotID[nPNo-1], gMes.sBarID[nPNo-1][nTNo-1][nCNo-1], gMes.sJudge[nPNo-1][nTNo-1][nCNo-1], sInfo, gMes.sNGCode[nPNo-1][nTNo-1][nCNo-1], nTNo, nCNo, 0,0,0,0);
				g_objLogFile.Save_OutTray("NG", nSp2WorkNg, nSp2TrayPosX+i, nSp2TrayPosY, gData.nPNoSortPick[1], gData.nTNoSortPick[1][nSp2StartNo+i], gData.nCNoSortPick[1][nSp2StartNo+i]);

				gData.nTNoSortPick[1][nSp2StartNo+i] = gData.nCNoSortPick[1][nSp2StartNo+i] = 0;
			}
			gData.nULPNo = gData.nPNoNgTray = gData.nPNoSortPick[1];
			if (Check_SortPickerEmpty(2)) gData.nPNoSortPick[1] = 0;
			g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 5, nSp2WorkNg);

			m_pThreadVacuumSp2Multi = AfxBeginThread(Thread_Vacuum_Sp2_Multi, (LPVOID)(nSp2StartNo+1));
			m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000);
		}
		break;
	case 15 :	// Picker Up
		if (!m_pThreadVacuumSp2Multi && g_objCommon.Get_SortPicker2VacOffMulti(nSp2StartNo+1, nSp2DownSu)) {
			g_objCommon.Set_SortPicker2Up(0);	
			g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 0);	//NG Down
			m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(5000);
		}
		break;
	case 16:	// Positon Check
		if (g_objCommon.Get_SortPicker2Up(0) && g_objCommon.Get_InfoSortPicker2VacOn() && g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 0)) {
			if(!m_tSortPick2Loop.Waiting_Time(300)) break;
			m_tSortPick2Loop.Takt_Save(14, 4);

			if (Check_NgTrayFull()) {
				if (m_nNgTrayCase == 10) m_nNgTrayCase = 11;
			}
			m_nSortPick2Case = 10; m_tSortPick2Loop.Set_LoopTime(5000);
		}
		break;
	case 17:	// Wait
		if (g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 0) && g_objCommon.Check_Position(AX_SORT_PICKER2_P, 0)) {

			// Good Module이 있는지 확인 후 어디로 갈지 정한다.
			if (Select_SortPickGoodPos(2, nSp2StartNo, nSp2PickCnt)) {
				if ((m_nSortPick1Case >=  0 && m_nSortPick1Case < 10) ||
					(m_nSortPick1Case >=  9 && m_nSortPick1Case < 20) ||
					(m_nSortPick1Case >= 40 && m_nSortPick1Case < 60) ||
					(m_nSortPick1Case == 22) && (gData.nPNoSortPick[0] != gData.nPNoSortPick[1])||
					(m_nSortPick1Case == 27)) {

					if ((gData.nPNoSortPick[1] != gData.nPNoGoodTray) && (gData.nPNoSortPick[1] != 0)) {
						if (gData.nPNoGoodTray != 0) return TRUE;
					}
					m_tSortPick2Loop.Takt_Save(14, 5);
					m_nSortPick2Case = 20; m_tSortPick2Loop.Set_LoopTime(10000);

				} else if (m_nSortPick1Case >= 21 && m_nSortPick1Case < 30) {
					m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000);
				}
			} else {
				// NG Buffer에 Module이 있을 경우.
// 				if (!gData.bNGTrayWait && Check_NgBufferFull(2) && Check_SortPickerEmpty(2)) {
// 					m_nSortPick2Case = 50; m_tSortPick2Loop.Set_LoopTime(10000);
// 
// 				} else {
					if (Check_InspectLotEnd(gData.nPNoNgTray, 2)) {
						if (Check_NgBufferLotEnd(gData.nPNoNgTray)) {	// NG Buffer에 모듈이 없으면 LotEnd 모듈이 있으면 포트 넘버 확인 후 언로딩 작업.
							if (gData.bCycleStop == FALSE) g_objMES.LotEnd_WriteAPD();
							if(!m_tSortPick2Loop.Waiting_Time(700)) break;
// 							m_strLog.Format("NG Port No: %d, Sort1 Port No: %d, Sort2 Port No: %d",gData.nPNoNgTray, gData.nPNoSortPick[0], gData.nPNoSortPick[1]);
// 							g_objLogFile.Save_HandlerLog(m_strLog);
							Job_LotEnd(gData.nPNoNgTray);
							if (m_pThreadBeep == NULL && m_pThreadNgFullBeep == NULL) {						
								m_pThreadBeep = AfxBeginThread(Thread_Beep, (LPVOID)(2000));

							} else {
								gData.bLotEndBeep = TRUE;
							}

							gData.bContinueLotEnd = TRUE;
							if (m_nNgTrayCase == 10) m_nNgTrayCase = 11; 
							if (!Check_GoodTrayEmpty()) {
								if (m_nGoodTray1Case == 20) { gData.bGoodTrayLotEnd[0]= TRUE; m_nGoodTray1Case = 21; }
								if (m_nGoodTray2Case == 20) { gData.bGoodTrayLotEnd[1]= TRUE; m_nGoodTray2Case = 21; }
							}  else {
								if (m_nGoodTray1Case <= 20 && m_nGoodTray2Case <= 20) { gData.bGoodTrayWait = TRUE; }
								if (m_nGoodTray1Case <= 20 && m_nGoodTray2Case >  30) { gData.bGoodTrayWait = TRUE; }
								if (m_nGoodTray1Case >  30 && m_nGoodTray2Case <= 20) { gData.bGoodTrayWait = TRUE; }
							}
							if (!gData.bGoodTrayLotEnd[0] && m_nGoodTray1Case > 20 && m_nGoodTray1Case < 30) { gData.bGoodTrayLotEnd[0] = TRUE; }
							if (!gData.bGoodTrayLotEnd[1] && m_nGoodTray2Case > 20 && m_nGoodTray2Case < 30) { gData.bGoodTrayLotEnd[1] = TRUE; }

						} else {
							if (!Check_NgBufferEmpty(2) && Check_SortPickerEmpty(2)) {
								bLastNgBuffPick2 = TRUE;
								m_nSortPick2Case = 50; m_tSortPick2Loop.Set_LoopTime(10000);
								break;
							}
						}
 					}
					if (Check_UnloadLotEnd() && !m_bUnloadLotEnd) { m_bUnloadLotEnd = TRUE; }

					m_tSortPick2Loop.Takt_Save(14, 5);
					m_strLog.Format("Sort Picker2, %d", GetTickCount() - m_dwSortPick2);
					g_objLogFile.Save_TestLog(m_strLog);

					if (nSp2WorkBuff != 1 && nSp2WorkBuff != 2) nSp2WorkBuff = 1;	// Test
					if (nSp2WorkBuff == 1) g_objCommon.Move_Position(AX_SORT_PICKER2_X, 0);	// Buffer1 Position
					if (nSp2WorkBuff == 2) g_objCommon.Move_Position(AX_SORT_PICKER2_X, 1);	// Buffer2 Position
					//g_objCommon.Set_SortPicker2Down(0);	// 복귀할때 실린더 다운해준다.

					if (m_nBuffTray1Case == 20)		 nSp2WorkBuff = 1;
					else if (m_nBuffTray2Case == 20) nSp2WorkBuff = 2;
					else							 nSp2WorkBuff = 0;

					if (!gData.bBuffStageMove && nSp2WorkBuff != 0 && Select_BufferTrayUpPos(nSp2WorkBuff, nSp2TrayPosX, nSp2TrayPosY)){	// 복귀할때 Y축 움직일수 있으면 움직여 주자.

						if (nSp2WorkBuff == 1 && m_nBuffTray1Case == 20) { 
							dSp2Y = m_pMoveData->dBufferStage1Y[3] + nSp2TrayPosY * m_pEquipData->dTrayPitchY;
							g_objAJinAXL.Move_Absolute(AX_BUFFER_STAGE1_Y, dSp2Y);
						}
						if (nSp2WorkBuff == 2 && m_nBuffTray2Case == 20) {
							dSp2Y = m_pMoveData->dBufferStage2Y[3] + nSp2TrayPosY * m_pEquipData->dTrayPitchY;
							g_objAJinAXL.Move_Absolute(AX_BUFFER_STAGE2_Y, dSp2Y);
						}
						gData.bBuffStageMove = TRUE;
					}

					m_nSortPick2Case = 0; m_tSortPick2Loop.Set_LoopTime(10000);
// 				}
			}
		}		
		return TRUE;

	case 18:	// Move to Good Tray1 Position
		if (g_objAJinAXL.Is_Done(AX_SORT_PICKER2_X)) {
			g_objCommon.Move_Position(AX_SORT_PICKER2_X, 2);	// Good1
			m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000);
		}
		break;
	case 19:	// Check Position & Sort2
		if (g_objCommon.Check_Position(AX_SORT_PICKER2_X, 2)) {	// Good1
			if ((m_nSortPick1Case >=  0 && m_nSortPick1Case < 10) ||
				(m_nSortPick1Case >=  9 && m_nSortPick1Case < 20) ||
				(m_nSortPick1Case >= 40 && m_nSortPick1Case < 60) ||
				(m_nSortPick1Case == 27)) {

				if ((gData.nPNoSortPick[1] != gData.nPNoGoodTray) && (gData.nPNoSortPick[1] != 0)) {
					if (gData.nPNoGoodTray != 0) return TRUE;
				}
				m_nSortPick2Case = 20; m_tSortPick2Loop.Set_LoopTime(10000);
			}
		}
		return TRUE;

	// Good Unload
	case 20:	// Check Good Tray
		if (m_nGoodTray1Case == 20 || m_nGoodTray2Case == 20) {
			m_nSortPick2Case = 22; m_tSortPick2Loop.Set_LoopTime(10000); 
		} else {
			g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 0);	// Ready Up
			if (Check_SortPickerEmpty(2)){				
				m_nSortPick2Case = 27; m_tSortPick2Loop.Set_LoopTime(10000);

			} else {
				if (Select_SortPickGoodPos(2, nSp2StartNo, nSp2PickCnt)) {
					g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 0);	// Ready Up
					g_objCommon.Move_Position(AX_SORT_PICKER2_X, 2);	// Good1
					m_nSortPick2Case = 21; m_tSortPick2Loop.Set_LoopTime(10000);
				} else {
					m_nSortPick2Case = 27; m_tSortPick2Loop.Set_LoopTime(10000);
				}
			}
		}
		break;
	case 21:	// Check Position
		if (g_objCommon.Check_Position(AX_SORT_PICKER2_X, 2)) {	// Good1
			if (m_nGoodTray1Case == 20 || m_nGoodTray2Case == 20) { m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000); }
			else if (Check_UnloadLotEnd()) { 
				g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 0);	// Ready Up
				m_nSortPick2Case = 27; m_tSortPick2Loop.Set_LoopTime(10000);
			}
		}
		return TRUE;

	case 22:	// Move to Good Unload Position
		if ((gData.nPNoSortPick[1] != gData.nPNoGoodTray) && (gData.nPNoSortPick[1] != 0)) {
			if (gData.nPNoGoodTray != 0) return TRUE;
		}
		if (m_nGoodTray1Case == 20 || m_nGoodTray2Case == 20) {
			if (m_nGoodTray1Case == 20) nSp2WorkGood = 1;
			if (m_nGoodTray2Case == 20) nSp2WorkGood = 2;
		
			if ((nSp2WorkGood == 1 && g_objAJinAXL.Is_Done(AX_GOOD_STAGE1_Y)) || 
				(nSp2WorkGood == 2 && g_objAJinAXL.Is_Done(AX_GOOD_STAGE2_Y))) {
				if (Select_GoodTrayPos(nSp2TrayPosX, nSp2TrayPosY)) {
					m_tSortPick2Loop.Takt_Start();
					if (Select_SortPickGoodPos(2, nSp2StartNo, nSp2PickCnt)) {
						if (!Select_GoodTrayPos(nSp2TrayPosX, nSp2TrayPosY)) nSp2TrayPosX = 0;	// Tray 교체중...
						nSp2TrayCnt = gData.nTrayX - nSp2TrayPosX;
						nSp2DownSu = ((nSp2PickCnt < nSp2TrayCnt) ? nSp2PickCnt : nSp2TrayCnt);
						m_nSortPick2MultiCnt = nSp2DownSu;

						if (nSp2WorkGood == 1) dSp2X = m_pMoveData->dSortPicker2X[2] + (nSp2TrayPosX - nSp2StartNo) * m_pEquipData->dTrayPitchX;
						if (nSp2WorkGood == 2) dSp2X = m_pMoveData->dSortPicker2X[3] + (nSp2TrayPosX - nSp2StartNo) * m_pEquipData->dTrayPitchX;
						g_objAJinAXL.Move_Absolute(AX_SORT_PICKER2_X, dSp2X);

						if (nSp2WorkGood == 1 && m_nGoodTray1Case == 20) {
							dSp2Y = m_pMoveData->dGoodStage1Y[2] + nSp2TrayPosY * m_pEquipData->dTrayPitchY;
							g_objAJinAXL.Move_Absolute(AX_GOOD_STAGE1_Y, dSp2Y);
						}
						if (nSp2WorkGood == 2 && m_nGoodTray2Case == 20) {
							dSp2Y = m_pMoveData->dGoodStage2Y[2] + nSp2TrayPosY * m_pEquipData->dTrayPitchY;
							g_objAJinAXL.Move_Absolute(AX_GOOD_STAGE2_Y, dSp2Y);
						}
						m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000);

					} else {
						g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 0);	// Ready Up
						m_nSortPick2Case = 27; m_tSortPick2Loop.Set_LoopTime(10000);
					}
				}
			} else {
				break;
			}
		}
		return TRUE;
	case 23:	// Picker Down
		if (g_objAJinAXL.Is_MoveDone(AX_SORT_PICKER2_X, dSp2X) &&
			((nSp2WorkGood == 1 && g_objAJinAXL.Is_MoveDone(AX_GOOD_STAGE1_Y, dSp2Y)) ||
			 (nSp2WorkGood == 2 && g_objAJinAXL.Is_MoveDone(AX_GOOD_STAGE2_Y, dSp2Y))) ) {

			m_tSortPick2Loop.Takt_Save(14, 6);
			m_tSortPick2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 2);	//Good Down
			g_objCommon.Set_SortPicker2DownMulti(nSp2StartNo+1, nSp2DownSu);
			m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000);
		}
		break;
	case 24:	// 정보전달, Vac Off
		if (g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 2) && g_objCommon.Get_SortPicker2DownMulti(nSp2StartNo+1, nSp2DownSu)) {
			for (int i = 0; i < nSp2DownSu; i++) {
				gData.InfoGoodTray[nSp2TrayPosY][nSp2TrayPosX+i] = gData.InfoSortPick[1][nSp2StartNo+i]; 
				gData.InfoSortPick[1][nSp2StartNo+i] = 0;

// 				// 연속랏을 위해 모듈 언로딩 했으면 판정 완료 초기화
// 				gData.byInspectDone[gData.nPNoSortPick[1]-1][gData.nTNoSortPick[1][nSp2StartNo+i]-1][gData.nCNoSortPick[1][nSp2StartNo+i]-1] |= (0 << 7);
				CString sInfo; 
				sInfo.Format("%d", gData.InfoGoodTray[nSp2TrayPosY][nSp2TrayPosX+i]);
				int nTNo = gData.nTNoSortPick[1][nSp2StartNo+i];
				int nCNo = gData.nCNoSortPick[1][nSp2StartNo+i];
				g_objMES.Set_Result(gLot.sLotID[nPNo-1], gMes.sBarID[nPNo-1][nTNo-1][nCNo-1], "OK", sInfo, gMes.sNGCode[nPNo-1][nTNo-1][nCNo-1], nTNo, nCNo, 0,0,0,0);
				g_objLogFile.Save_OutTray("GOOD", gData.nGoodTrayCount, nSp2TrayPosX+i, nSp2TrayPosY, gData.nPNoSortPick[1], gData.nTNoSortPick[1][nSp2StartNo+i], gData.nCNoSortPick[1][nSp2StartNo+i]);

				gData.nTNoSortPick[1][nSp2StartNo+i] = gData.nCNoSortPick[1][nSp2StartNo+i] = 0;
			}
			gData.nULPNo = gData.nPNoGoodTray = gData.nPNoSortPick[1]; 
			if (Check_SortPickerEmpty(2)) gData.nPNoSortPick[1] = 0;
			g_dlgWork.PostMessage(UM_UPDATE_TRAY_INFO, 4, nSp2WorkGood-1);

			m_pThreadVacuumSp2Multi = AfxBeginThread(Thread_Vacuum_Sp2_Multi, (LPVOID)(nSp2StartNo+1));
			m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000);
		}
		break;
	case 25 :	// Picker Up
		if (!m_pThreadVacuumSp2Multi && g_objCommon.Get_SortPicker2VacOffMulti(nSp2StartNo+1, nSp2DownSu)) {
			g_objCommon.Set_SortPicker2Up(0);	
			g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 0);	
			m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(5000);
		}
		break;
	case 26:	// Picker Check
		if (g_objCommon.Get_SortPicker2Up(0) && g_objCommon.Get_InfoSortPicker2VacOn() && g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 0)) {
			m_tSortPick2Loop.Takt_Save(14, 7);
			if (Check_GoodTrayFull()) {
				if (m_nGoodTray1Case == 20) m_nGoodTray1Case = 21;
				if (m_nGoodTray2Case == 20) m_nGoodTray2Case = 21;
			}
			m_nSortPick2Case = 20; m_tSortPick2Loop.Set_LoopTime(5000);
		}
		break;
	case 27:	// Position Check
		if (g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 0)) {
			m_tSortPick2Loop.Takt_Save(14, 8);
			if (Check_SortPickerEmpty(2)){
				if (!gData.bNGTrayWait && Check_NgBufferFull(2)) {
					m_nSortPick2Case = 50; m_tSortPick2Loop.Set_LoopTime(10000);

				} else {
					m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000);
				}
			} else {
				if (Select_SortPickNgPos(2, nSp2StartNo, nSp2PickCnt)) {	// startNo 0base pickCnt 1base
 					m_nSortPick2Case = 7; m_tSortPick2Loop.Set_LoopTime(10000);
				}
			}
		}
		break;
	case 28:	// Buffer 복귀
		if (g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 0)) {
			if (Check_InspectLotEnd(gData.nULPNo, 2)) {
				if (Check_NgBufferLotEnd(gData.nULPNo)) {	// NG Buffer에 모듈이 없으면 LotEnd 모듈이 있으면 포트 넘버 확인 후 언로딩 작업.
					if (gData.bCycleStop == FALSE) g_objMES.LotEnd_WriteAPD();
					if(!m_tSortPick2Loop.Waiting_Time(700)) break;
					Job_LotEnd(gData.nULPNo);
					if (m_pThreadBeep == NULL && m_pThreadNgFullBeep == NULL) {						
						m_pThreadBeep = AfxBeginThread(Thread_Beep, (LPVOID)(2000));

					} else {
						gData.bLotEndBeep = TRUE;
					}

					gData.bContinueLotEnd = TRUE;
					if (m_nNgTrayCase == 10) m_nNgTrayCase = 11; 
					if (!Check_GoodTrayEmpty()) {
						if (m_nGoodTray1Case == 20) { gData.bGoodTrayLotEnd[0]= TRUE; m_nGoodTray1Case = 21; } 
						if (m_nGoodTray2Case == 20) { gData.bGoodTrayLotEnd[1]= TRUE; m_nGoodTray2Case = 21; } 
					}  else {
						if (m_nGoodTray1Case <= 20 && m_nGoodTray2Case <= 20) { gData.bGoodTrayWait = TRUE; }
						if (m_nGoodTray1Case <= 20 && m_nGoodTray2Case >  30) { gData.bGoodTrayWait = TRUE; }
						if (m_nGoodTray1Case >  30 && m_nGoodTray2Case <= 20) { gData.bGoodTrayWait = TRUE; }
					}
					if (!gData.bGoodTrayLotEnd[0] && m_nGoodTray1Case > 20 && m_nGoodTray1Case < 30) { gData.bGoodTrayLotEnd[0] = TRUE; }
					if (!gData.bGoodTrayLotEnd[1] && m_nGoodTray2Case > 20 && m_nGoodTray2Case < 30) { gData.bGoodTrayLotEnd[1] = TRUE; }

				} else {
					if (!Check_NgBufferEmpty(2) && Check_SortPickerEmpty(2)) {
						bLastNgBuffPick2 = TRUE;
						m_nSortPick2Case = 50; m_tSortPick2Loop.Set_LoopTime(10000);
						break;
					}
				}
			}
			if (Check_UnloadLotEnd() && !m_bUnloadLotEnd) { m_bUnloadLotEnd = TRUE; }

			//m_tSortPick2Loop.Takt_Start();
			m_strLog.Format("Sort Picker2, %d", GetTickCount() - m_dwSortPick2);
			g_objLogFile.Save_TestLog(m_strLog);
			if (nSp2WorkBuff != 1 && nSp2WorkBuff != 2) nSp2WorkBuff = 1;	// Test
			if (nSp2WorkBuff == 1) g_objCommon.Move_Position(AX_SORT_PICKER2_X, 0);	// Buffer1 Position
			if (nSp2WorkBuff == 2) g_objCommon.Move_Position(AX_SORT_PICKER2_X, 1);	// Buffer2 Position
			//g_objCommon.Set_SortPicker2Down(0);	// 복귀할때 실린더 다운해준다.

			if (m_nBuffTray1Case == 20)		 nSp2WorkBuff = 1;
			else if (m_nBuffTray2Case == 20) nSp2WorkBuff = 2;
			else							 nSp2WorkBuff = 0;

			if (!gData.bBuffStageMove && nSp2WorkBuff != 0 && Select_BufferTrayUpPos(nSp2WorkBuff, nSp2TrayPosX, nSp2TrayPosY)){	// 복귀할때 Y축 움직일수 있으면 움직여 주자.

				if (nSp2WorkBuff == 1 && m_nBuffTray1Case == 20) { 
					dSp2Y = m_pMoveData->dBufferStage1Y[3] + nSp2TrayPosY * m_pEquipData->dTrayPitchY;
					g_objAJinAXL.Move_Absolute(AX_BUFFER_STAGE1_Y, dSp2Y);
				}
				if (nSp2WorkBuff == 2 && m_nBuffTray2Case == 20) {
					dSp2Y = m_pMoveData->dBufferStage2Y[3] + nSp2TrayPosY * m_pEquipData->dTrayPitchY;
					g_objAJinAXL.Move_Absolute(AX_BUFFER_STAGE2_Y, dSp2Y);
				}
				gData.bBuffStageMove = TRUE;
			} else {
				if (gData.bBuffStageMove) gData.bBuffStageMove = FALSE;
			}

			m_nSortPick2Case = 0; m_tSortPick2Loop.Set_LoopTime(10000);
		}		
		break;
// 	case 29:	// Position Check
// 		if ((nSp2WorkBuff == 1 && g_objCommon.Check_Position(AX_SORT_PICKER2_X, 0)) ||
// 			(nSp2WorkBuff == 2 && g_objCommon.Check_Position(AX_SORT_PICKER2_X, 1))) {
// 
// 			m_tSortPick2Loop.Takt_Save(14, 9);
// 			m_strLog.Format("Sort Picker2, %d", GetTickCount() - m_dwSortPick2);
// 			g_objLogFile.Save_TestLog(m_strLog);
// 			m_nSortPick2Case = 0; m_tSortPick2Loop.Set_LoopTime(10000);
// 		}
// 		break;

	// NG Buffer place
	case 40:	// Move to NG Buffer Position
		if (g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 0) && g_objAJinAXL.Is_Done(AX_SORT_PICKER2_X) && g_objAJinAXL.Is_Done(AX_SORT_PICKER2_P)) {
			if (Select_SortPickNgPos(2, nSp2StartNo, nSp2PickCnt, TRUE)) {	// startNo 0base pickCnt 1base
				if (Select_NgBufferPos(2, nSp2TrayPosX)) {	// Ng Buffer에 자리가 있을때
					m_tSortPick2Loop.Takt_Start();			
					nSp2TrayCnt = gData.nSortPickQt - nSp2TrayPosX;
					nSp2DownSu = ((nSp2PickCnt < nSp2TrayCnt) ? nSp2PickCnt : nSp2TrayCnt);
					m_nSortPick2MultiCnt = nSp2DownSu;

					dSp2X = m_pMoveData->dSortPicker2X[6] + (nSp2TrayPosX - nSp2StartNo) * m_pEquipData->dNgBufferPitchX;

					g_objAJinAXL.Move_Absolute(AX_SORT_PICKER2_X, dSp2X);
					g_objCommon.Move_Position(AX_SORT_PICKER2_P, 1);	// NG Buffer Pitch
					m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000);

				} else {	// Ng Buffer에 자리가 없고 Picker에 Ng가 있을때
					g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 0);	// Ready Up
					g_objCommon.Move_Position(AX_SORT_PICKER2_P, 0);	// Tray Pitch
					m_nSortPick2Case = 10; m_tSortPick2Loop.Set_LoopTime(10000);
				}

			} else {	// Picker에 Ng가 없을때
				g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 0);	// Ready Up
				g_objCommon.Move_Position(AX_SORT_PICKER2_P, 0);	// Tray Pitch
				m_nSortPick2Case = 17; m_tSortPick2Loop.Set_LoopTime(10000);
			}
		}
		break;
	case 41:	// Check Position
		if (g_objAJinAXL.Is_MoveDone(AX_SORT_PICKER2_X, dSp2X) && g_objCommon.Check_Position(AX_SORT_PICKER2_P, 1)) {
			m_tSortPick2Loop.Takt_Save(14, 10);
			m_tSortPick2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 4);	//NG Buffer Down
			g_objCommon.Set_SortPicker2DownMulti(nSp2StartNo+1, nSp2DownSu);
			m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000);
		}
		break;
	case 42:	// 정보전달, Vac Off
		if (g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 4) && g_objCommon.Get_SortPicker2DownMulti(nSp2StartNo+1, nSp2DownSu)) {
			for (int i = 0; i < nSp2DownSu; i++) {
				gData.InfoNgBuffer[0][nSp2TrayPosX+i] = gData.InfoSortPick[1][nSp2StartNo+i]; 
				gData.nTNoNgBuffer[0][nSp2TrayPosX+i] = gData.nTNoSortPick[1][nSp2StartNo+i];
				gData.nCNoNgBuffer[0][nSp2TrayPosX+i] = gData.nCNoSortPick[1][nSp2StartNo+i];

				gData.InfoSortPick[1][nSp2StartNo+i] = gData.nTNoSortPick[1][nSp2StartNo+i] = gData.nCNoSortPick[1][nSp2StartNo+i] = 0;

			}
			gData.nPNoNgBuffer[0] = gData.nPNoSortPick[1];
			if (Check_SortPickerEmpty(2)) gData.nPNoSortPick[1] = 0;

			g_objCommon.Set_InfoNgBufferVacOn(2);
			m_pThreadVacuumSp2Multi = AfxBeginThread(Thread_Vacuum_Sp2_Multi, (LPVOID)(nSp2StartNo+1));
			m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000);
		}
		break;
	case 43 :	// Picker Up
		if (!m_pThreadVacuumSp2Multi && g_objCommon.Get_InfoNgBufferVacOn(2) && g_objCommon.Get_SortPicker2VacOffMulti(nSp2StartNo+1, nSp2DownSu)) {
			g_objCommon.Set_SortPicker2Up(0);
			g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 0);	// Ready Up
			m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(5000);
		}
		break;
	case 44:	// Positon Check
		if (g_objCommon.Get_SortPicker2Up(0) && g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 0)) {
			m_tSortPick2Loop.Takt_Save(14, 11);

			if (Select_SortPickNgPos(2, nSp2StartNo, nSp2PickCnt, TRUE)) {
				if (Check_NgBufferFull(2)) {
					g_objCommon.Move_Position(AX_SORT_PICKER2_P, 0);	// Tray Pitch
					m_nSortPick2Case = 7; m_tSortPick2Loop.Set_LoopTime(5000);

				} else {
					m_nSortPick2Case = 40; m_tSortPick2Loop.Set_LoopTime(5000);
				}
			} else {
				g_objCommon.Move_Position(AX_SORT_PICKER2_P, 0);	// Tray Pitch
				m_nSortPick2Case = 17; m_tSortPick2Loop.Set_LoopTime(10000);
			}			
		}
		break;

	// NG Buffer Pickup
	case 50:
		if (g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 0) && g_objAJinAXL.Is_Done(AX_SORT_PICKER2_X) && g_objAJinAXL.Is_Done(AX_SORT_PICKER2_P)) {
			if (Check_SortPickerEmpty(2)) {
				g_objCommon.Move_Position(AX_SORT_PICKER2_X, 6);	// NG Buffer Position
				g_objCommon.Move_Position(AX_SORT_PICKER2_P, 1);	// NG Buffer Pitch
				m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000);
			}
		}
		break;
	case 51:	// Check Position
		if (g_objCommon.Check_Position(AX_SORT_PICKER2_X, 6) && g_objCommon.Check_Position(AX_SORT_PICKER2_P, 1)) {
			m_tSortPick2Loop.Takt_Save(14, 10);
			m_tSortPick2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 4);	//NG Buffer Down
			g_objCommon.Set_InfoSortPicker2Down(1);
			g_objCommon.Set_InfoSortPicker2VacOn(1);
			m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000);
		}
		break;
	case 52:	// 정보전달, Vac Off
		if (g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 4) && g_objCommon.Get_InfoSortPicker2Down(1)) {
			for (int i = 0; i < gData.nSortPickQt; i++) {
				gData.InfoSortPick[1][i] = gData.InfoNgBuffer[0][i]; 
				gData.nTNoSortPick[1][i] = gData.nTNoNgBuffer[0][i];
				gData.nCNoSortPick[1][i] = gData.nCNoNgBuffer[0][i];

				gData.InfoNgBuffer[0][i] = gData.nTNoNgBuffer[0][i] = gData.nCNoNgBuffer[0][i] = 0;

			}
			gData.nPNoSortPick[1] = gData.nPNoNgBuffer[0]; gData.nPNoNgBuffer[0] = 0;

			g_objCommon.Set_NgBufferVacOff(2);
			m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(10000);
		}
		break;
	case 53 :	// Picker Up
		if (!m_tSortPick2Loop.Waiting_Time(m_pEquipData->nDelayAdd[2])) break;	// SortPicker Delay

		g_objCommon.Set_SortPicker2Up(0);
		g_objCommon.Move_Position(AX_SORT_PICKER2_Z, 0);	// Ready Up
		m_nSortPick2Case++; m_tSortPick2Loop.Set_LoopTime(5000);
		break;
	case 54:
		if (g_objCommon.Get_InfoSortPicker2VacOn() && 
			g_objCommon.Check_Position(AX_SORT_PICKER2_Z, 0) && g_objCommon.Get_SortPicker2Up(0)) {

			m_tSortPick2Loop.Takt_Save(14, 12);
			g_objCommon.Move_Position(AX_SORT_PICKER2_P, 0);	// Tray Pitch
			m_nSortPick2Case = 7; m_tSortPick2Loop.Set_LoopTime(5000);
		}
		break;
	}

	// 14. (Error : 4400)
	if (m_tSortPick2Loop.Over_LoopTime()) {
		if (m_nSortPick2Case == 5) {
			if (nSp2VacErr == 1) { gData.nErrBufferStageNo = nSp2WorkBuff; }
			if (!g_objCommon.Get_InfoSortPicker2VacOn() && nSp2VacErr == 0) {
				CCMI8000Dlg *pMainDlg = (CCMI8000Dlg*)AfxGetApp()->GetMainWnd();
				pMainDlg->Set_LotErrorLog("RETRY", 4405, "Sort2 Retry", gData.nPNoSortPick[1]);

				nSp2VacErr = 1; 
				m_nSortPick2Case = 2; m_tSortPick2Loop.Set_LoopTime(5000); return TRUE;
			}
		} else { gData.nErrBufferStageNo = 0; }

		if (m_nSortPick2Case == 6) {
			for (int i = 4; i >= 0; i--) { Set_InspectError(gData.nPNoSortPick[1], gData.nTNoSortPick[1][i], gData.nCNoSortPick[1][i]); }
		}
		g_objCommon.Show_Error(4400 + m_nSortPick2Case);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 15. (Error : 4500)
BOOL CSequenceMain::GoodTray1_Run()
{
	// 양품 Tray 배출 시 도어락 및 버튼 확인 필요
	if (gData.bGoodTrayWait && m_nGoodTray1Case > 24 && m_nGoodTray1Case < 50) return TRUE;
//	if (m_pEquipData->bUseDoorLock && m_pDX15->iDoor05Unlock && m_nGoodTray1Case > 24 && m_nGoodTray1Case < 50) return TRUE;

	switch (m_nGoodTray1Case) {
	case 0:		// Start 하면 1로 바꿔준다.
		return TRUE;

	case 1:		// Tray Check
		if (m_bUnloadLotEnd) {
			m_nGoodTray1Case = 0;

		} else if (!m_pDX12->iGoodStage1Exist && g_objCommon.Get_GoodTray1MasterSlaveOut()) {
			if (m_pDX12->iGoodTrayBufferBottom && m_nEmptyTrayYCase < 21) {
				m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);

			} else {
				m_tGoodTray1Loop.Set_LoopTime(5000); return TRUE;
			}
		} 
		break;
	case 2:		// Position Check & Z Axis Move to Load Support Up Position
		if (g_objCommon.Check_Position(AX_GOOD_STAGE1_Y, 0) && g_objCommon.Check_Position(AX_GOOD_STAGE1_Z, 1)) {	// Good Tray Z축 기본위치 확인 필요.
			g_objCommon.Move_Position(AX_GOOD_STAGE1_Z, 2);	// Support Up
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		} 
		break;
	case 3:	// Good Tray Buffer Support Out
		if (g_objCommon.Check_Position(AX_GOOD_STAGE1_Z, 2)) {
			//m_nGoodTray1Case = 10; m_tGoodTray1Loop.Set_LoopTime(5000);
			m_pDY12->oGoodTrayBuffSupport1In = FALSE; m_pDY12->oGoodTrayBuffSupport2In = FALSE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		} 
		break;
	case 4:	// Support Down
		if (m_pDX12->iGoodTrayBuffSupport1Out && m_pDX12->iGoodTrayBuffSupport2Out) {
			if (m_tGoodTray1Loop.Waiting_Time(100)) break;
			g_objCommon.Move_Position(AX_GOOD_STAGE1_Z, 4);	// Support Down
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		}
		break;
	case 5:	// Support In
		if (g_objCommon.Check_Position(AX_GOOD_STAGE1_Z, 4)) {
			m_pDY12->oGoodTrayBuffSupport1In = TRUE; m_pDY12->oGoodTrayBuffSupport2In = TRUE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		}
		break;
// 	case 6:	// Moving Down
// 		if (m_pDX12->iGoodTrayBuffSupport1In && m_pDX12->iGoodTrayBuffSupport2In) {
// 			g_objCommon.Move_Position(AX_GOOD_STAGE1_Z, 1);
// 			m_nGoodTray1Case = 11; m_tGoodTray1Loop.Set_LoopTime(5000);
// 		} 
// 		break;
	case 6:	// Master In
		if (m_pDX12->iGoodTrayBuffSupport1In && m_pDX12->iGoodTrayBuffSupport2In) {
			m_pDY12->oGoodStage1MasterIn = TRUE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		} 
		break;
	case 7:	// Slave In
		if (m_pDX12->iGoodStage1MasterIn && !m_pDX12->iGoodStage1MasterOut) {
			m_pDY12->oGoodStage1SlaveIn = TRUE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		} 
		break;
	case 8:	// Moving Down
		if (g_objCommon.Get_GoodTray1MasterSlaveIn()) {
			g_objCommon.Move_Position(AX_GOOD_STAGE1_Z, 1);
			m_nGoodTray1Case = 11; m_tGoodTray1Loop.Set_LoopTime(5000);
		}
		break;

// 	case 10:	// Wait for Empty Tray Loading
// 		if (m_bUnloadLotEnd && !m_pDX12->iGoodStage1Exist) { m_nGoodTray1Case = 80; }
// 		return TRUE;
	case 11:	// Master In
		if (g_objCommon.Check_Position(AX_GOOD_STAGE1_Z, 1) && m_pDX12->iGoodStage1Exist) {
			m_dwGoodTray1 = GetTickCount();
			m_tGoodTray1Loop.Takt_Start();
			m_pDY12->oGoodStage1MasterIn = TRUE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		} 
		break;
	case 12:	// Slave In
		if (m_pDX12->iGoodStage1MasterIn && !m_pDX12->iGoodStage1MasterOut) {
			m_pDY12->oGoodStage1SlaveIn = TRUE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		} 
		break;
	case 13:	// Z Axis Move to Moving Up Position
		if (g_objCommon.Get_GoodTray1MasterSlaveIn()) {
			m_tGoodTray1Loop.Takt_Save(15, 1);
			m_tGoodTray1Loop.Takt_Start();
//			g_objCommon.Move_Position(AX_GOOD_STAGE1_Z, 1);
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		} 
		break;
	case 14:	// Position Check
		if (g_objCommon.Check_Position(AX_GOOD_STAGE1_Z, 1)) {
			m_tGoodTray1Loop.Takt_Save(15, 2);
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		}
		break;
	case 15:	// 안전 확인.
		if (m_nGoodTray2Case > 24) { m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000); }
		return TRUE;

	case 16:	// Y Axis Move to Sort Picker1 Position 
		if (g_objCommon.Check_Position(AX_GOOD_STAGE1_Y, 0) && g_objCommon.Check_Position(AX_GOOD_STAGE1_Z, 1) && m_pDX12->iGoodStage1Exist) {
			m_tGoodTray1Loop.Takt_Start();
			if (m_nSortPick2Case == 21 || m_nSortPick2Case == 22) g_objCommon.Move_Position(AX_GOOD_STAGE1_Y, 2);
			else												  g_objCommon.Move_Position(AX_GOOD_STAGE1_Y, 1);
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		}
		break;
	case 17:	// Init Tray
		if ((g_objCommon.Check_Position(AX_GOOD_STAGE1_Y, 1) || g_objCommon.Check_Position(AX_GOOD_STAGE1_Y, 2)) && m_pDX12->iGoodStage1Exist) {
			m_tGoodTray1Loop.Takt_Save(15, 3);
//			m_tGoodTray1Loop.Takt_Start();
//			g_objCommon.Move_Position(AX_GOOD_STAGE1_Z, 2);
			Init_GoodTray();
//			gData.nPNoGoodTray = 0;
			gData.nGoodTrayCount++;
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		}
		break;
	case 18:	// Position Check
		if ((g_objCommon.Check_Position(AX_GOOD_STAGE1_Y, 1) || g_objCommon.Check_Position(AX_GOOD_STAGE1_Y, 2)) && g_objCommon.Check_Position(AX_GOOD_STAGE1_Z, 1)) {
//			m_tGoodTray1Loop.Takt_Save(15, 4);
			m_nGoodTray1Case = 20; m_tGoodTray1Loop.Set_LoopTime(5000);
		}
		break;

	case 20:	// Working
		if (m_bUnloadLotEnd && Check_GoodTrayEmpty()) { m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000); }
		return TRUE;

	case 21:	// 안전 확인.
		if (m_nGoodTray2Case > 53 || (m_nGoodTray2Case >= 0 && m_nGoodTray2Case <= 15)) {	//로드 위치로 갈때까지 대기
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		}
		return TRUE;
		
	case 22:	// Z Axis Move to Moving Down Position
		if (g_objAJinAXL.Is_Done(AX_GOOD_STAGE1_Y) && g_objCommon.Check_Position(AX_GOOD_STAGE1_Z, 1)) {
			m_tGoodTray1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_GOOD_STAGE1_Z, 0);
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		}
		break;
	case 23:	// Position Check
		if (g_objCommon.Check_Position(AX_GOOD_STAGE1_Z, 0)) {
			m_tGoodTray1Loop.Takt_Save(15, 5);
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(30000);
		}
		break;	
	case 24:	// Y Axis Move to Unload Position
		if (g_objCommon.Check_Position(AX_GOOD_STAGE1_Z, 0)) {
			m_tGoodTray1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_GOOD_STAGE1_Y, 3);
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		}
		break;
	case 25:	// Z Axis Move to Unload Up Position
		if (g_objCommon.Check_Position(AX_GOOD_STAGE1_Y, 3) && m_pDX12->iGoodPortSlideClose) {
			m_tGoodTray1Loop.Takt_Save(15, 6);
			m_tGoodTray1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_GOOD_STAGE1_Z, 3);
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		}
		break;
	case 26:	// Slave Out
		if (g_objCommon.Check_Position(AX_GOOD_STAGE1_Z, 3)) {
			m_tGoodTray1Loop.Takt_Save(15, 7);
			m_tGoodTray1Loop.Takt_Start();
			m_pDY12->oGoodStage1SlaveIn = FALSE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		}
		break;
	case 27:	// Master Out
		if (!m_pDX12->iGoodStage1SlaveIn && m_pDX12->iGoodStage1SlaveOut) {
			m_pDY12->oGoodStage1MasterIn = FALSE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		}
		break;
	case 28:	// Master/Slave Out Check
		if (g_objCommon.Get_GoodTray1MasterSlaveOut()) {
			m_tGoodTray1Loop.Takt_Save(15, 8);
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		}
		break;
	case 29:	// Z Axis Move to Moving Down Position
		if (g_objCommon.Check_Position(AX_GOOD_STAGE1_Y, 3) && !g_objCommon.Check_Position(AX_GOOD_STAGE2_Y, 3)) {	// Inter lock
			m_tGoodTray1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_GOOD_STAGE1_Z, 0);
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		}
		break;
	case 30:	// Lot End Check
		if (g_objCommon.Check_Position(AX_GOOD_STAGE1_Z, 0)) {
			// 해당 Lot 종료이면 문을 열어 Tray 제거 할수 있게 해준다.
			if (gData.bGoodTrayLotEnd[0] && m_bUnloadLotEnd && m_nGoodTray2Case < 11) {	// 도어락 오픈 후처리 확인.
				m_pDY12->oGoodPortSlideLock = FALSE; m_pDY12->oGoodPortSlideUnlock = TRUE;
				//m_pDY15->oDoor05Unlock = TRUE;
				g_objAJinAXL.Write_Output(12);
				g_objAJinAXL.Write_Output(15);
				gData.bGoodTrayWait = TRUE;
			} else if (gData.bGoodTrayLotEnd[0] && !m_bUnloadLotEnd) {	// 연속랏
				m_pDY12->oGoodPortSlideLock = FALSE; m_pDY12->oGoodPortSlideUnlock = TRUE;
				//m_pDY15->oDoor05Unlock = TRUE;
				g_objAJinAXL.Write_Output(12);
				g_objAJinAXL.Write_Output(15);
				gData.bGoodTrayWait = TRUE;
			}
			m_nGoodTray1Case = 50; m_tGoodTray1Loop.Set_LoopTime(5000);

			m_tGoodTray1Loop.Takt_Save(15, 9);
			m_strLog.Format("Good Stage1, %d", GetTickCount() - m_dwGoodTray1);
			g_objLogFile.Save_TestLog(m_strLog);
		}
		break;

	case 50:	// Wait
		if (m_nGoodTray2Case > 10 && m_nGoodTray2Case < 50) m_nGoodTray1Case++;
		return TRUE;
	case 51:	// Tray Check
		if (!m_pDX12->iGoodStage1Exist && g_objCommon.Get_GoodTray1MasterSlaveOut()) {
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		} 
		break;
	case 52:	// 안전 확인 및 Y Axis Move to Tray Load Position
		if (g_objCommon.Check_Position(AX_GOOD_STAGE2_Z, 1) ||
			g_objCommon.Check_Position(AX_GOOD_STAGE2_Z, 2) ||
			g_objCommon.Check_Position(AX_GOOD_STAGE2_Z, 3)) {	// Stage2 Up Check

			m_tGoodTray1Loop.Takt_Start();
			g_objCommon.Move_Position(AX_GOOD_STAGE1_Y, 0);
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		}
		break;
	case 53:	// Position Check
		if (g_objCommon.Check_Position(AX_GOOD_STAGE1_Y, 0)) {
			m_tGoodTray1Loop.Takt_Save(15, 10);
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(30000);
		}
		break;
	case 54:	// 안전 확인
		if (m_nGoodTray2Case > 15 && m_nGoodTray2Case < 50) {
			m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
		}
		return TRUE;
	case 55:	// Z Axis Move to Moving Up Position
		if (g_objCommon.Check_Position(AX_GOOD_STAGE1_Y, 0)) {
			double dS1Y = g_objAJinAXL.Get_Position(AX_GOOD_STAGE1_Y);
			double dS2Y = g_objAJinAXL.Get_Position(AX_GOOD_STAGE2_Y);
			double dDiff = fabs(dS1Y-dS2Y);
			if (dDiff > TRAY_WIDTH) {	// Good Stage Y Position Check
				m_tGoodTray1Loop.Takt_Start();
				g_objCommon.Move_Position(AX_GOOD_STAGE1_Z, 1);
				m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
			}
		}
		break;
	case 56:	// Lot End면 case 0, 계속 진행하면 case 10
		if (g_objCommon.Check_Position(AX_GOOD_STAGE1_Z, 1)) {
			m_tGoodTray1Loop.Takt_Save(15, 11);

			if (m_bUnloadLotEnd) m_nGoodTray1Case = 0;
			else				 m_nGoodTray1Case = 1;
			m_tGoodTray1Loop.Set_LoopTime(5000);
		}
		break;

	case 80:	// 후처리
		if (g_objCommon.Check_Position(AX_GOOD_STAGE1_Z, 2)) {
			if (m_nGoodTray2Case > 20) {	// 안전 확인.
				g_objCommon.Move_Position(AX_GOOD_STAGE1_Z, 1);
				m_nGoodTray1Case++; m_tGoodTray1Loop.Set_LoopTime(5000);
			}
		}
		break;
	case 81:
		if (g_objCommon.Check_Position(AX_GOOD_STAGE1_Z, 1)) {
			m_nGoodTray1Case = 0; m_tGoodTray1Loop.Set_LoopTime(5000);
		}
		break;
	}

	// 15. (Error : 4500)
	if (m_tGoodTray1Loop.Over_LoopTime()) {
		g_objCommon.Show_Error(4500 + m_nGoodTray1Case);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 16. (Error : 4600)
BOOL CSequenceMain::GoodTray2_Run()
{
	// 양품 Tray 배출 시 도어락 및 버튼 확인 필요
	if (gData.bGoodTrayWait && m_nGoodTray2Case > 24 && m_nGoodTray2Case < 50) return TRUE;

	switch (m_nGoodTray2Case) {
	case 0:		// Start 하면 1로 바꿔준다.
		return TRUE;

	case 1:		// Tray Check
		if (m_bUnloadLotEnd) {
			m_nGoodTray2Case = 0;

		} else if (!m_pDX12->iGoodStage2Exist && g_objCommon.Get_GoodTray2MasterSlaveOut()) {
			if (m_pDX12->iGoodTrayBufferBottom && m_nEmptyTrayYCase < 21) {
				m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);

			} else {
				m_tGoodTray2Loop.Set_LoopTime(5000); return TRUE;
			}
		} 
		break;
	case 2:		// Position Check & Z Axis Move to Load Support Up Position
		if (g_objCommon.Check_Position(AX_GOOD_STAGE2_Y, 0) && g_objCommon.Check_Position(AX_GOOD_STAGE2_Z, 1)) {	// Good Tray Z축 기본위치 확인 필요.
			g_objCommon.Move_Position(AX_GOOD_STAGE2_Z, 2);	// Support Up
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		} 
		break;
	case 3:	// Good Tray Buffer Support Out
		if (g_objCommon.Check_Position(AX_GOOD_STAGE2_Z, 2)) {
			m_pDY12->oGoodTrayBuffSupport1In = FALSE; m_pDY12->oGoodTrayBuffSupport2In = FALSE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		} 
		break;
	case 4:	// Stage Support Down
		if (m_pDX12->iGoodTrayBuffSupport1Out && m_pDX12->iGoodTrayBuffSupport2Out) {
			if (m_tGoodTray2Loop.Waiting_Time(100)) break;
			g_objCommon.Move_Position(AX_GOOD_STAGE2_Z, 4);	// Support Down
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		}
		break;
	case 5:	// Good Tray Buffer Support In
		if (g_objCommon.Check_Position(AX_GOOD_STAGE2_Z, 4)) {
			m_pDY12->oGoodTrayBuffSupport1In = TRUE; m_pDY12->oGoodTrayBuffSupport2In = TRUE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		}
		break;
// 	case 6:	// Good Stage Moving Down
// 		if (m_pDX12->iGoodTrayBuffSupport1In && m_pDX12->iGoodTrayBuffSupport2In) {
// 			g_objCommon.Move_Position(AX_GOOD_STAGE2_Z, 1);	// Moving Down
// 			m_nGoodTray2Case = 11; m_tGoodTray2Loop.Set_LoopTime(5000);
// 		} 
// 		break;
	case 6:	// Master In
		if (m_pDX12->iGoodTrayBuffSupport1In && m_pDX12->iGoodTrayBuffSupport2In) {
			m_pDY12->oGoodStage2MasterIn = TRUE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		} 
		break;
	case 7:	// Slave In
		if (m_pDX12->iGoodStage2MasterIn && !m_pDX12->iGoodStage2MasterOut) {
			m_pDY12->oGoodStage2SlaveIn = TRUE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		} 
		break;
	case 8:	// Good Stage Moving Down
		if (g_objCommon.Get_GoodTray2MasterSlaveIn()) {
			g_objCommon.Move_Position(AX_GOOD_STAGE2_Z, 1);	// Moving Down
			m_nGoodTray2Case = 11; m_tGoodTray2Loop.Set_LoopTime(5000);
		}
		break;

// 	case 10:	// Wait for Empty Tray Loading
// 		if (m_bUnloadLotEnd && !m_pDX12->iGoodStage2Exist) m_nGoodTray2Case = 80;
// 		return TRUE;
	case 11:	// Master In
		if (g_objCommon.Check_Position(AX_GOOD_STAGE2_Z, 1) && m_pDX12->iGoodStage2Exist) {
			m_dwGoodTray2 = GetTickCount();
			m_tGoodTray2Loop.Takt_Start();
			m_pDY12->oGoodStage2MasterIn = TRUE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		} 
		break;
	case 12:	// Slave In
		if (m_pDX12->iGoodStage2MasterIn && !m_pDX12->iGoodStage2MasterOut) {
			m_pDY12->oGoodStage2SlaveIn = TRUE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		} 
		break;
	case 13:	// Z Axis Move to Moving Up Position
		if (g_objCommon.Get_GoodTray2MasterSlaveIn()) {
			m_tGoodTray2Loop.Takt_Save(16, 1);
			m_tGoodTray2Loop.Takt_Start();
//			g_objCommon.Move_Position(AX_GOOD_STAGE2_Z, 1);
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		} 
		break;
	case 14:	// Position Check
		if (g_objCommon.Check_Position(AX_GOOD_STAGE2_Z, 1)) {
			m_tGoodTray2Loop.Takt_Save(16, 2);
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		}
		break;
	case 15:	// 안전 확인.
		if (m_nGoodTray1Case > 24) { m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000); }
		return TRUE;

	case 16:	// Y Axis Move to Sort Picker1 Position 
		if (g_objCommon.Check_Position(AX_GOOD_STAGE2_Y, 0) && g_objCommon.Check_Position(AX_GOOD_STAGE2_Z, 1) && m_pDX12->iGoodStage2Exist) {
			m_tGoodTray2Loop.Takt_Start();
			if (m_nSortPick2Case == 21 || m_nSortPick2Case == 22) g_objCommon.Move_Position(AX_GOOD_STAGE2_Y, 2);
			else												  g_objCommon.Move_Position(AX_GOOD_STAGE2_Y, 1);
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		}
		break;
	case 17:	// Z Axis Move to Load Up Position
		if ((g_objCommon.Check_Position(AX_GOOD_STAGE2_Y, 1) || g_objCommon.Check_Position(AX_GOOD_STAGE2_Y, 2)) && m_pDX12->iGoodStage2Exist) {
			m_tGoodTray2Loop.Takt_Save(16, 3);
//			m_tGoodTray2Loop.Takt_Start();
//			g_objCommon.Move_Position(AX_GOOD_STAGE2_Z, 2);
			Init_GoodTray();
//			gData.nPNoGoodTray = 0;
			gData.nGoodTrayCount++;
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		}
		break;
	case 18:	// Position Check
		if ((g_objCommon.Check_Position(AX_GOOD_STAGE2_Y, 1) || g_objCommon.Check_Position(AX_GOOD_STAGE2_Y, 2)) && g_objCommon.Check_Position(AX_GOOD_STAGE2_Z, 1)) {
//			m_tGoodTray2Loop.Takt_Save(16, 4);
			m_nGoodTray2Case = 20; m_tGoodTray2Loop.Set_LoopTime(5000);
		}
		break;

	case 20:	// Working
		if (m_bUnloadLotEnd && Check_GoodTrayEmpty()) { m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000); }	// 후처리
		return TRUE;

	case 21:	// 안전 확인.
		if (m_nGoodTray1Case > 53 || (m_nGoodTray1Case >= 0 && m_nGoodTray1Case <= 15) ) {
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		}
		return TRUE;

	case 22:	// Z Axis Move to Moving Up Position
		if (g_objAJinAXL.Is_Done(AX_GOOD_STAGE2_Y) && g_objCommon.Check_Position(AX_GOOD_STAGE2_Z, 1)) {
			m_tGoodTray2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_GOOD_STAGE2_Z, 0);
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		}
		break;
	case 23:	// Position Check
		if (g_objCommon.Check_Position(AX_GOOD_STAGE2_Z, 0)) {
			m_tGoodTray2Loop.Takt_Save(16, 5);
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(30000);
		}
		break;
	case 24:	// Y Axis Move to Unload Position
		if (g_objCommon.Check_Position(AX_GOOD_STAGE2_Z, 0)) {
			m_tGoodTray2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_GOOD_STAGE2_Y, 3);
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		}
		break;
	case 25:	// Z Axis Move to Unload Up Position
		if (g_objCommon.Check_Position(AX_GOOD_STAGE2_Y, 3) && m_pDX12->iGoodPortSlideClose) {
			m_tGoodTray2Loop.Takt_Save(16, 6);
			m_tGoodTray2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_GOOD_STAGE2_Z, 3);
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		}
		break;
	case 26:	// Slave Out
		if (g_objCommon.Check_Position(AX_GOOD_STAGE2_Z, 3)) {
			m_tGoodTray2Loop.Takt_Save(16, 7);
			m_tGoodTray2Loop.Takt_Start();
			m_pDY12->oGoodStage2SlaveIn = FALSE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		}
		break;
	case 27:	// Master Out
		if (!m_pDX12->iGoodStage2SlaveIn && m_pDX12->iGoodStage2SlaveOut) {
			m_pDY12->oGoodStage2MasterIn = FALSE;
			g_objAJinAXL.Write_Output(12);
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		}
		break;
	case 28:	// Master/Slave Out Check
		if (g_objCommon.Get_GoodTray2MasterSlaveOut()) {
			m_tGoodTray2Loop.Takt_Save(16, 8);
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		}
		break;
	case 29:	// Z Axis Move to Moving Down Position
		if (g_objCommon.Check_Position(AX_GOOD_STAGE2_Y, 3) && !g_objCommon.Check_Position(AX_GOOD_STAGE1_Y, 3)) {	// Inter lock
			m_tGoodTray2Loop.Takt_Start();
			g_objCommon.Move_Position(AX_GOOD_STAGE2_Z, 0);
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		}
		break;
	case 30:	// Lot End Check
		if (g_objCommon.Check_Position(AX_GOOD_STAGE2_Z, 0)) {
			// 해당 Lot 종료이면 문을 열어 Tray 제거 할수 있게 해준다.
			if (gData.bGoodTrayLotEnd[1] && m_bUnloadLotEnd && m_nGoodTray1Case < 11) {	// 도어락 오픈 후처리 해야할 상황인지 확인.
				m_pDY12->oGoodPortSlideLock = FALSE; m_pDY12->oGoodPortSlideUnlock = TRUE;
				//m_pDY15->oDoor05Unlock = TRUE;
				g_objAJinAXL.Write_Output(12);
				g_objAJinAXL.Write_Output(15);
				gData.bGoodTrayWait = TRUE;
			} else if (gData.bGoodTrayLotEnd[1] && !m_bUnloadLotEnd) {	// 연속 랏 상황
				m_pDY12->oGoodPortSlideLock = FALSE; m_pDY12->oGoodPortSlideUnlock = TRUE;
				//m_pDY15->oDoor05Unlock = TRUE;
				g_objAJinAXL.Write_Output(12);
				g_objAJinAXL.Write_Output(15);
				gData.bGoodTrayWait = TRUE;
			}
			m_nGoodTray2Case = 50; m_tGoodTray2Loop.Set_LoopTime(5000);

			m_tGoodTray2Loop.Takt_Save(16, 9);
			m_strLog.Format("Good Stage2, %d", GetTickCount() - m_dwGoodTray2);
			g_objLogFile.Save_TestLog(m_strLog);
		}
		break;

	case 50:	// Wait
		if (m_nGoodTray1Case >= 10 && m_nGoodTray1Case < 50) m_nGoodTray2Case++;
		return TRUE;
	case 51:	// Tray Check
		if (!m_pDX12->iGoodStage2Exist && g_objCommon.Get_GoodTray2MasterSlaveOut()) {
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		} 
		break;
	case 52:	// 안전 확인 및 Y Axis Move to Tray Load Position
		if (g_objCommon.Check_Position(AX_GOOD_STAGE1_Z, 1) ||
			g_objCommon.Check_Position(AX_GOOD_STAGE1_Z, 2) ||
			g_objCommon.Check_Position(AX_GOOD_STAGE1_Z, 3)) {	// Stage1 Up Check

				m_tGoodTray2Loop.Takt_Start();
				g_objCommon.Move_Position(AX_GOOD_STAGE2_Y, 0);
				m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		}
		break;
	case 53:	// Position Check
		if (g_objCommon.Check_Position(AX_GOOD_STAGE2_Y, 0)) {
			m_tGoodTray2Loop.Takt_Save(16, 10);
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(30000);
		}
		break;
	case 54:	// 안전 확인
		if (m_nGoodTray1Case > 15 && m_nGoodTray1Case < 50) {
			m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
		}
		return TRUE;
	case 55:	// Z Axis Move to Moving Up Position
		if (g_objCommon.Check_Position(AX_GOOD_STAGE2_Y, 0)) {
			double dS1Y = g_objAJinAXL.Get_Position(AX_GOOD_STAGE1_Y);
			double dS2Y = g_objAJinAXL.Get_Position(AX_GOOD_STAGE2_Y);
			double dDiff = fabs(dS1Y-dS2Y);
			if (dDiff > TRAY_WIDTH) {	// Good Stage Y Position Check
				m_tGoodTray2Loop.Takt_Start();
				g_objCommon.Move_Position(AX_GOOD_STAGE2_Z, 1);
				m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
			}
		}
		break;
	case 56:	// Lot End면 case 0, 계속 진행하면 case 10
		if (g_objCommon.Check_Position(AX_GOOD_STAGE2_Z, 1)) {
			m_tGoodTray2Loop.Takt_Save(16, 11);

			if (m_bUnloadLotEnd) m_nGoodTray2Case = 0; 
			else				 m_nGoodTray2Case = 1;
			m_tGoodTray2Loop.Set_LoopTime(5000);
		}
		break;

	case 80:	// 후처리
		if (g_objCommon.Check_Position(AX_GOOD_STAGE2_Z, 2)) {
			if (m_nGoodTray1Case > 20) {	// 안전 확인.
				g_objCommon.Move_Position(AX_GOOD_STAGE2_Z, 1);
				m_nGoodTray2Case++; m_tGoodTray2Loop.Set_LoopTime(5000);
			}
		}
		break;
	case 81:
		if (g_objCommon.Check_Position(AX_GOOD_STAGE2_Z, 1)) {
			m_nGoodTray2Case = 0; m_tGoodTray2Loop.Set_LoopTime(5000);
		}
		break;

	}

	// 16. (Error : 4600)
	if (m_tGoodTray2Loop.Over_LoopTime()) {
		g_objCommon.Show_Error(4600 + m_nGoodTray2Case);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 17. (Error : 4700)
BOOL CSequenceMain::NgTray_Run()
{
	// LotEnd되어 NG Tray 배출과정에서도 장비가 멈추면 안되는 예외 처리
	if (gData.bNGTrayWait) return TRUE;

	switch (m_nNgTrayCase) {
	case 0:		// Start 또는 버튼을 눌렀을때 1로 바뀐다.
		return TRUE;

	case 1:		// Slide Close Check
		if (m_bUnloadLotEnd) {
			m_nNgTrayCase = 0;

		} else if (m_pDX09->iNgPortSlideClose) {
			m_dwNgTray = GetTickCount();
//			gData.nPNoNgTray = 0;
			if (Check_NgTrayFull()) { m_nNgTrayCase = 0; g_objCommon.Show_Error(4720); return FALSE;}	// NG Full Alarm
			m_nNgTrayCase++; m_tNgTrayLoop.Set_LoopTime(10000);
		}
		break;
	case 2:		// Ready Position Check	// B접
		if (!m_pDX09->iNgStageExist1 && !m_pDX09->iNgStageExist2 &&
			!m_pDX09->iNgStageExist3 && !m_pDX09->iNgStageExist4) {
//		if (TRUE) {	// TEST

			m_pDY09->oNgPortSlideLock = TRUE;	m_pDY09->oNgPortSlideUnlock = FALSE;
			g_objAJinAXL.Write_Output(9);
			m_nNgTrayCase++; m_tNgTrayLoop.Set_LoopTime(5000);
		}
		break;
	case 3:		// Tray Exist Check & Move to Sort Picker2 Tray1 Position
		if (m_pDX09->iNgPortSlideClose && m_pDX09->iNgPortSlideLock && g_objCommon.Check_Position(AX_NG_STAGE_Y, 0)) {
			m_tNgTrayLoop.Takt_Start();
			g_objCommon.Move_Position(AX_NG_STAGE_Y, 3);
			m_nNgTrayCase++; m_tNgTrayLoop.Set_LoopTime(10000);
		}
		break;
	case 4:		// Position Check 
		if (g_objCommon.Check_Position(AX_NG_STAGE_Y, 3)) {
			m_tNgTrayLoop.Takt_Save(17, 1);
			m_nNgTrayCase = 10; m_tNgTrayLoop.Set_LoopTime(5000);
		}

	case 10:	// Working
		return TRUE;

	case 11:	// Move to Ready Position
		if (g_objAJinAXL.Is_Done(AX_NG_STAGE_Y)) {
			m_tNgTrayLoop.Takt_Start();
			g_objCommon.Move_Position(AX_NG_STAGE_Y, 0);
			m_nNgTrayCase++; m_tNgTrayLoop.Set_LoopTime(10000);
		}
		break;
	case 12:	// Position Check
		if (g_objCommon.Check_Position(AX_NG_STAGE_Y, 0)) {
			m_tNgTrayLoop.Takt_Save(17, 2);
			m_pDY09->oNgPortSlideLock = FALSE;	m_pDY09->oNgPortSlideUnlock = TRUE;
			//m_pDY15->oDoor04Unlock = TRUE;
			g_objAJinAXL.Write_Output(9);
			m_nNgTrayCase++; m_tNgTrayLoop.Set_LoopTime(10000);
		}
		break;
	case 13:	// Unlock Check
		if (m_pDX09->iNgPortSlideUnlock) {
			m_strLog.Format("NG Stage, %d", GetTickCount() - m_dwNgTray);
			g_objLogFile.Save_TestLog(m_strLog);

			if (!m_bUnloadLotEnd && Check_NgTrayFull()) {
				gData.bNGTrayWait = TRUE; 
				g_dlgWork.PostMessage(UM_SHOW_MSG, 1, NULL);
				if (m_pThreadNgFullBeep == NULL) m_pThreadNgFullBeep = AfxBeginThread(Thread_NgFullBeep, (LPVOID)(2000));
				//g_objCommon.Show_Error(4720);	// NG Full Alarm

			} else if (!m_bUnloadLotEnd) {
				// 장비 종료만 아니면 대기상태로 만들어 준다.
				gData.bNGTrayWait = TRUE;
				gData.bContinueLotEnd = TRUE;
				g_dlgWork.PostMessage(UM_SHOW_MSG, 2, NULL);		// 2020.09.14 khs
			} else if (m_bUnloadLotEnd) {
				gData.bNgTrayEnd = TRUE;
			}
			m_nNgTrayCase = 0; m_tNgTrayLoop.Set_LoopTime(5000);
		}
		break;
	}

	// 17. (Error : 4700)
	if (m_tNgTrayLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(4700 + m_nNgTrayCase);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 18. (Error : 4800)
BOOL CSequenceMain::EmptyTrayX_Run()
{
	static int nEtWorkNo = 0;

	switch (m_nEmptyTrayXCase) {
	case 0:		// Start시 1로 바꿔준다.
		return TRUE;

	case 1:		// 안전 확인. 및 Down 상태에서 대기한다.
		if (m_bUnloadLotEnd) {
			g_objCommon.Set_EmptyTrayXUp();
			m_nEmptyTrayXCase = 0;

		} else if (!m_pDX13->iEmptyTrans1Exist && g_objCommon.Check_Position(AX_EMPTY_TRANS1_X, 0) &&
			g_objCommon.Get_EmptyTrayXUp() && g_objCommon.Get_EmptyTrayXMasterSlaveOut()) {

			g_objCommon.Set_EmptyTrayXDown();
			m_nEmptyTrayXCase++; m_tEmptyTrayXLoop.Set_LoopTime(5000);
		}
		break;
	case 2:		// Angle Tray Wait
		if (m_nAngleTray1Case == 30 || m_nAngleTray2Case == 30) {
			m_dwEmptyTrayX = GetTickCount();
			if (m_nAngleTray1Case == 30) nEtWorkNo = 1;
			if (m_nAngleTray2Case == 30) nEtWorkNo = 2;
			m_nEmptyTrayXCase++; m_tEmptyTrayXLoop.Set_LoopTime(5000);
		} else if (m_bUnloadLotEnd && Check_UnloadLotEnd()) {	// Load 할께 없으면 종료.
			g_objCommon.Set_EmptyTrayXUp();
			m_nEmptyTrayXCase = 0;
		}
		return TRUE;
	case 3:		// Empty Trans Down
		if ((nEtWorkNo == 1 && g_objCommon.Check_Position(AX_ANGLE_STAGE1_Y, 4)) ||
			(nEtWorkNo == 2 && g_objCommon.Check_Position(AX_ANGLE_STAGE2_Y, 4))) {

			m_tEmptyTrayXLoop.Takt_Start();
			g_objCommon.Set_EmptyTrayXDown();
			m_nEmptyTrayXCase++; m_tEmptyTrayXLoop.Set_LoopTime(5000);
		}
		break;
	case 4:		// Angle Tray Vacuum Off
		if (g_objCommon.Get_EmptyTrayXDown()) {
			m_tEmptyTrayXLoop.Takt_Save(18,1);
			m_tEmptyTrayXLoop.Takt_Start();

			if (nEtWorkNo == 1) m_pDY02->oAngleStage1AlignIn = FALSE;
			if (nEtWorkNo == 2) m_pDY02->oAngleStage2AlignIn = FALSE;
			g_objAJinAXL.Write_Output(2);
			m_nEmptyTrayXCase++; m_tEmptyTrayXLoop.Set_LoopTime(5000);
		}
		break;
	case 5:		// Empty Tray Master In
		if ((nEtWorkNo == 1 && !m_pDX02->iAngleStage1AlignIn && m_pDX02->iAngleStage1AlignOut) ||
			(nEtWorkNo == 2 && !m_pDX02->iAngleStage2AlignIn && m_pDX02->iAngleStage2AlignOut))
		{

			m_tEmptyTrayXLoop.Takt_Save(18,2);
			m_tEmptyTrayXLoop.Takt_Start();

			m_pDY13->oEmptyTrans1MasterIn = TRUE; m_pDY13->oEmptyTrans1MasterOut = FALSE;
			g_objAJinAXL.Write_Output(13);
			m_nEmptyTrayXCase++; m_tEmptyTrayXLoop.Set_LoopTime(5000);
		}
		break;
	case 6:		// Empty Tray Slave In
		if (m_pDX13->iEmptyTrans1MasterIn && !m_pDX13->iEmptyTrans1MasterOut) {

			m_pDY13->oEmptyTrans1SlaveIn = TRUE; m_pDY13->oEmptyTrans1SlaveOut = FALSE;
			g_objAJinAXL.Write_Output(13);
			m_nEmptyTrayXCase++; m_tEmptyTrayXLoop.Set_LoopTime(5000);
		}
		break;
	case 7:		// Empty Tray Up & Angle Tray 진행.
		if (g_objCommon.Get_EmptyTrayXMasterSlaveIn()){
			m_tEmptyTrayXLoop.Takt_Save(18,3);
			m_tEmptyTrayXLoop.Takt_Start();

			g_objCommon.Set_EmptyTrayXUp();
			if (nEtWorkNo == 1 || m_nAngleTray1Case == 30) { m_nAngleTray1Case = 31; m_tAngleTray1Loop.Set_LoopTime(10000); }
			if (nEtWorkNo == 2 || m_nAngleTray2Case == 30) { m_nAngleTray2Case = 31; m_tAngleTray2Loop.Set_LoopTime(10000); }
			m_nEmptyTrayXCase++; m_tEmptyTrayXLoop.Set_LoopTime(5000);
		}
		break;
	case 8:		// Move to Wait Position
		if (g_objCommon.Get_EmptyTrayXUp() && m_pDX13->iEmptyTrans1Exist) {
			m_tEmptyTrayXLoop.Takt_Save(18,4);

			g_objCommon.Move_Position(AX_EMPTY_TRANS1_X, 3);	// Wait Position
			m_nEmptyTrayXCase++; m_tEmptyTrayXLoop.Set_LoopTime(5000);
		}
		break;
	case 9:	// Wait Position Check
		if (g_objCommon.Check_Position(AX_EMPTY_TRANS1_X, 3)) {

			m_nEmptyTrayXCase = 10; m_tEmptyTrayXLoop.Set_LoopTime(5000);
		}
		break;

	case 10:	// Wait
		if (m_nEmptyTrayYCase == 0 || m_nEmptyTrayYCase == 2 || m_nEmptyTrayYCase >= 20) {
			// EmptyPort에 Tray가 있고 Good Stage에 Tray가 없으면 먼저 작업할수있게 기다려 준다.
#ifdef EDITION_2ND
			if (m_pDX01->iEmptyPortExist && m_pDX01->iEmptyPortTopCheck && (!m_pDX12->iGoodTrayBufferBottom || !m_pDX12->iGoodStage1Exist || !m_pDX12->iGoodStage2Exist))
#else
			if (m_pDX07->iEmptyPortExist && m_pDX07->iEmptyPortTopCheck && (!m_pDX12->iGoodTrayBufferBottom || !m_pDX12->iGoodStage1Exist || !m_pDX12->iGoodStage2Exist))
#endif
			{
				return TRUE;
			} else {
				m_nEmptyTrayXCase++; m_tEmptyTrayXLoop.Set_LoopTime(5000);
			}
		}
		return TRUE;

	case 11:	// X Move to Empty Tray Elevator Position
		if (g_objCommon.Check_Position(AX_EMPTY_TRANS1_X, 3) && g_objCommon.Check_Position(AX_EMPTY_TRANS2_Y, 0)) {
			m_tEmptyTrayXLoop.Takt_Start();
			g_objCommon.Move_Position(AX_EMPTY_TRANS1_X, 1);
			m_nEmptyTrayXCase++; m_tEmptyTrayXLoop.Set_LoopTime(300000);	// 속도 100일때 50초
		}
		break;
	case 12:	// Position Check
		if (g_objCommon.Check_Position(AX_EMPTY_TRANS1_X, 1)) {
			m_tEmptyTrayXLoop.Takt_Save(18,5);
			m_nEmptyTrayXCase++; m_tEmptyTrayXLoop.Set_LoopTime(60000);
		}
		break;
	case 13:	// Elevator 작업 완료 확인. (30sec)
		if (m_nEmptyTrayElCase == 10) {
			//g_objCommon.Check_Position(AX_EMPTY_PORT_Z, 1)	//ready Position
			double dCurPos = g_objAJinAXL.Get_Position(AX_EMPTY_PORT_Z) - m_pMoveData->dEmptyTrans1X[2];	// 현재위치 - Down Offset 하한 리미트 알람 뜨기전에 Full Alarm
			gData.dEmptyPortZDownOffset = dCurPos;
			double dReadyPos = m_pMoveData->dEmptyPortZ[1];
			if (dReadyPos < dCurPos) {
				//g_objAJinAXL.Move_Relative(AX_EMPTY_PORT_Z, -m_pMoveData->dEmptyTrans1X[2], 0.5);	// Down Offset Move
				g_objAJinAXL.Move_Absolute(AX_EMPTY_PORT_Z, gData.dEmptyPortZDownOffset, 0.5);	// Down Offset Move
				m_nEmptyTrayXCase++; m_tEmptyTrayXLoop.Set_LoopTime(10000);
			} else {
				m_nEmptyTrayElCase = 30;	// Full Alarm 처리
				m_tEmptyTrayXLoop.Set_LoopTime(10000);
			}
		}
		break;
	case 14:	// Empty Tray Down
		if (g_objAJinAXL.Is_MoveDone(AX_EMPTY_PORT_Z, gData.dEmptyPortZDownOffset) && g_objCommon.Check_Position(AX_EMPTY_TRANS1_X, 1)) {
			m_tEmptyTrayXLoop.Takt_Start();

			gData.dEmptyPortZDownOffset = 0.0;
			g_objCommon.Set_EmptyTrayXDown();
			m_nEmptyTrayXCase++; m_tEmptyTrayXLoop.Set_LoopTime(5000);
		} else if (g_objAJinAXL.Is_Done(AX_EMPTY_PORT_Z) && !g_objAJinAXL.Is_MoveDone(AX_EMPTY_PORT_Z, gData.dEmptyPortZDownOffset)) {
			if (!m_tEmptyTrayXLoop.Waiting_Time(100)) break;
			g_objAJinAXL.Move_Absolute(AX_EMPTY_PORT_Z, gData.dEmptyPortZDownOffset, 0.5);	// Down Offset Move
		}
		break;
	case 15:	// Empty Tray Master Out
		if (g_objCommon.Get_EmptyTrayXDown()) {
			m_tEmptyTrayXLoop.Takt_Save(18,6);
			m_tEmptyTrayXLoop.Takt_Start();

			m_pDY13->oEmptyTrans1MasterIn = FALSE; m_pDY13->oEmptyTrans1MasterOut = TRUE;
			g_objAJinAXL.Write_Output(13);
			m_nEmptyTrayXCase++; m_tEmptyTrayXLoop.Set_LoopTime(5000);
		}
		break;
	case 16:	// Empty Tray Slave Out
		if (!m_pDX13->iEmptyTrans1MasterIn && m_pDX13->iEmptyTrans1MasterOut) {

			m_pDY13->oEmptyTrans1SlaveIn = FALSE; m_pDY13->oEmptyTrans1SlaveOut = TRUE;
			g_objAJinAXL.Write_Output(13);
			m_nEmptyTrayXCase++; m_tEmptyTrayXLoop.Set_LoopTime(5000);
		}
		break;
	case 17:	// Empty Tray Up
		if (g_objCommon.Get_EmptyTrayXMasterSlaveOut()){
			m_tEmptyTrayXLoop.Takt_Save(18,7);
			m_tEmptyTrayXLoop.Takt_Start();

			g_objCommon.Set_EmptyTrayXUp();
			//g_objAJinAXL.Move_Relative(AX_EMPTY_PORT_Z, m_pMoveData->dEmptyTrans1X[2], 0.5);	// return
			m_nEmptyTrayXCase++; m_tEmptyTrayXLoop.Set_LoopTime(5000);
		}
		break;
	case 18:	// Elevator 진행.
		if (g_objAJinAXL.Is_Done(AX_EMPTY_PORT_Z) && g_objCommon.Get_EmptyTrayXUp() && !m_pDX13->iEmptyTrans1Exist) {
			if (m_nEmptyTrayElCase == 10) {
				m_tEmptyTrayXLoop.Takt_Save(18,8);
#ifdef EDITION_2ND
				if (m_pDX01->iEmptyPortTopCheck)	m_nEmptyTrayElCase = 5;	// Slow Down
				else								m_nEmptyTrayElCase = 2;	// Slow Up
#else
				if (m_pDX07->iEmptyPortTopCheck)	m_nEmptyTrayElCase = 5;	// Slow Down
				else								m_nEmptyTrayElCase = 2;	// Slow Up
#endif
				m_nEmptyTrayXCase = 20; m_tEmptyTrayXLoop.Set_LoopTime(5000);
			}			
		}
		break;
		
	case 20:	// X Move to Angle Tray Position
		if (g_objCommon.Check_Position(AX_EMPTY_TRANS1_X, 1)) {
			m_tEmptyTrayXLoop.Takt_Start();
			g_objCommon.Move_Position(AX_EMPTY_TRANS1_X, 0);
			m_nEmptyTrayXCase++; m_tEmptyTrayXLoop.Set_LoopTime(90000);	// 속도 100일때 50초
		}
		break;
	case 21:	//Done
		if (g_objCommon.Check_Position(AX_EMPTY_TRANS1_X, 0)) {
			m_tEmptyTrayXLoop.Takt_Save(18,9);
			m_strLog.Format("Empty Trans X, %d", GetTickCount() - m_dwEmptyTrayX);
			g_objLogFile.Save_TestLog(m_strLog);

			m_nEmptyTrayXCase = 1; m_tEmptyTrayXLoop.Set_LoopTime(5000);
		}
		break;
	}

	// 18. (Error : 4800)
	if (m_tEmptyTrayXLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(4800 + m_nEmptyTrayXCase);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 19. (Error : 4900)
BOOL CSequenceMain::EmptyTrayElevator_Run()
{
	// Full Tray 상태이면 대기한다.
	//if (gData.bEmptyZWait) return TRUE;

	// Top check Sensor 켜져있으면 준비 완료.
	switch (m_nEmptyTrayElCase) {
	case 0:		// Start시 1로 바뀜.
		return TRUE;

	case 1:		// Tray 확인.
		if (m_bUnloadLotEnd) {
			 m_nEmptyTrayElCase = 0;

		}
#ifdef EDITION_2ND
		else if (m_pDX01->iEmptyPortSlideClose)
#else
		else if (m_pDX07->iEmptyPortSlideClose)
#endif
		{
			gData.bEmptyZTopCheckRepeat = FALSE;
			gData.bEmptyFull = FALSE;
			if (g_objCommon.Check_Position(AX_EMPTY_PORT_Z, 0)) m_nEmptyTrayElCase = 20;
			else if (g_objCommon.Check_Position(AX_EMPTY_PORT_Z, 1)) m_nEmptyTrayElCase = 21;
#ifdef EDITION_2ND
			else if (g_objCommon.Check_Position(AX_EMPTY_PORT_Z, 2, 1.0) && !m_pDX01->iEmptyPortTopCheck && !m_pDX01->iEmptyPortExist) m_nEmptyTrayElCase++;
#else
			else if (g_objCommon.Check_Position(AX_EMPTY_PORT_Z, 2, 1.0) && !m_pDX07->iEmptyPortTopCheck && !m_pDX07->iEmptyPortExist) m_nEmptyTrayElCase++;
#endif
			m_tEmptyTrayElLoop.Set_LoopTime(5000);
		}
		break;

		// 1. Slow Up
	case 2:		// Elevator Z Slow Up
#ifdef EDITION_2ND
		if (!m_pDX01->iEmptyPortTopCheck)
#else
		if (!m_pDX07->iEmptyPortTopCheck)
#endif
		{
			if (m_dwEmptyTrayEl == 0) m_dwEmptyTrayEl = GetTickCount();
			m_tEmptyTrayElLoop.Takt_Start();
// 			if (gData.bEmptyZTopCheckRepeat) {
				g_objAJinAXL.Move_Relative(AX_EMPTY_PORT_Z, 2.0, 0.25);	// 2mm씩 올려준다.
// 			} else {
// 				g_objAJinAXL.Move_Absolute(AX_EMPTY_PORT_Z, m_pMoveData->dEmptyPortZ[2], 0.25);
// 			}
			m_nEmptyTrayElCase++; m_tEmptyTrayElLoop.Set_LoopTime(20000);	// 20초
		} 
		break;
	case 3:		// Buffer Z Up Stop
#ifdef EDITION_2ND
		if (m_pDX01->iEmptyPortTopCheck || g_objCommon.Check_Position(AX_EMPTY_PORT_Z, 2, 1.0)) {
			g_objAJinAXL.Stop_Motion(AX_EMPTY_PORT_Z);
			m_nEmptyTrayElCase++; m_tEmptyTrayElLoop.Set_LoopTime(5000);

		} else if (!m_pDX01->iEmptyPortTopCheck && g_objAJinAXL.Is_Done(AX_EMPTY_PORT_Z) && !g_objCommon.Check_Position(AX_EMPTY_PORT_Z, 2, 1.0)) {
			m_nEmptyTrayElCase = 2; m_tEmptyTrayElLoop.Set_LoopTime(5000);
		}
#else
		if (m_pDX07->iEmptyPortTopCheck || g_objCommon.Check_Position(AX_EMPTY_PORT_Z, 2, 1.0)) {
			g_objAJinAXL.Stop_Motion(AX_EMPTY_PORT_Z);
			m_nEmptyTrayElCase++; m_tEmptyTrayElLoop.Set_LoopTime(5000);

		} else if (!m_pDX07->iEmptyPortTopCheck && g_objAJinAXL.Is_Done(AX_EMPTY_PORT_Z) && !g_objCommon.Check_Position(AX_EMPTY_PORT_Z, 2, 1.0)) {
			m_nEmptyTrayElCase = 2; m_tEmptyTrayElLoop.Set_LoopTime(5000);
		}
#endif
		break;
	case 4:		// Check Buffer Z Done
#ifdef EDITION_2ND
		if ((g_objAJinAXL.Is_Done(AX_EMPTY_PORT_Z) && m_pDX01->iEmptyPortTopCheck) ||
			(!m_pDX01->iEmptyPortTopCheck /*&& !m_pDX07->iEmptyPortExist*/ && g_objCommon.Check_Position(AX_EMPTY_PORT_Z, 2, 1.0)))
#else
		if ((g_objAJinAXL.Is_Done(AX_EMPTY_PORT_Z) && m_pDX07->iEmptyPortTopCheck) ||
			(!m_pDX07->iEmptyPortTopCheck /*&& !m_pDX07->iEmptyPortExist*/ && g_objCommon.Check_Position(AX_EMPTY_PORT_Z, 2, 1.0)))
#endif
		{
			m_tEmptyTrayElLoop.Takt_Save(19, 1);

// 			if (gData.bEmptyZTopCheckRepeat == FALSE) {	// TopCheck 후 한번더 확인 해준다.
// 				gData.bEmptyZTopCheckRepeat = TRUE;
// 				m_nEmptyTrayElCase = 5; m_tEmptyTrayElLoop.Set_LoopTime(5000);
// 
// 			} else {
				gData.bEmptyZTopCheckRepeat = FALSE;
				m_strLog.Format("Empty Elevator, %d", GetTickCount() - m_dwEmptyTrayEl);
				g_objLogFile.Save_TestLog(m_strLog);
				m_dwEmptyTrayEl = 0;
				m_nEmptyTrayElCase = 10; m_tEmptyTrayElLoop.Set_LoopTime(5000);
// 			}
		}
		break;

		// 2. Slow Down
	case 5:		// Buffer Z Slow Down
#ifdef EDITION_2ND
		if (m_pDX01->iEmptyPortTopCheck)
#else
		if (m_pDX07->iEmptyPortTopCheck)
#endif
		{
			if (m_dwEmptyTrayEl == 0) m_dwEmptyTrayEl = GetTickCount();
			m_tEmptyTrayElLoop.Takt_Start();
			g_objAJinAXL.Move_Absolute(AX_EMPTY_PORT_Z, m_pMoveData->dEmptyPortZ[1], 0.25);	//Ready Position
			m_nEmptyTrayElCase++; m_tEmptyTrayElLoop.Set_LoopTime(10000);	// 10초
		} else {
			gData.bEmptyZTopCheckRepeat = FALSE;
			m_nEmptyTrayElCase = 10; m_tEmptyTrayElLoop.Set_LoopTime(5000);
		}
		break;
	case 6:		// Buffer Z Down Stop
#ifdef EDITION_2ND
		if (!m_pDX01->iEmptyPortTopCheck)
#else
		if (!m_pDX07->iEmptyPortTopCheck)
#endif
		{
			g_objAJinAXL.Stop_Motion(AX_EMPTY_PORT_Z);
			m_nEmptyTrayElCase++; m_tEmptyTrayElLoop.Set_LoopTime(5000);
		}
		break;
	case 7:		// Check Buffer Z Done
#ifdef EDITION_2ND
		if (g_objAJinAXL.Is_Done(AX_EMPTY_PORT_Z) && !m_pDX01->iEmptyPortTopCheck)
#else
		if (g_objAJinAXL.Is_Done(AX_EMPTY_PORT_Z) && !m_pDX07->iEmptyPortTopCheck)
#endif
		{
			if (!m_tEmptyTrayElLoop.Waiting_Time(500)) break;	// 센서가 정말로 꺼졌나 확인하는 시간.
			m_tEmptyTrayElLoop.Takt_Save(19, 2);

			m_nEmptyTrayElCase = 2;
			m_tEmptyTrayElLoop.Set_LoopTime(5000);

		} 
#ifdef EDITION_2ND
		else if (g_objAJinAXL.Is_Done(AX_EMPTY_PORT_Z) && m_pDX01->iEmptyPortTopCheck)
#else
		else if (g_objAJinAXL.Is_Done(AX_EMPTY_PORT_Z) && m_pDX07->iEmptyPortTopCheck)
#endif
		{
			m_nEmptyTrayElCase = 5;
			m_tEmptyTrayElLoop.Set_LoopTime(5000);
		}
		break;

	case 10:	// Wait (EmptyTray X 또는 Y가 Start시킴)
		return TRUE;


	case 20:	// Move to Ready Position
		if (g_objCommon.Check_Position(AX_EMPTY_PORT_Z, 0)) {
			g_objCommon.Move_Position(AX_EMPTY_PORT_Z, 1);
			m_nEmptyTrayElCase++; m_tEmptyTrayElLoop.Set_LoopTime(10000);
		}
		break;
	case 21:	// Position Check
		if (g_objCommon.Check_Position(AX_EMPTY_PORT_Z, 1)) {
			m_dwEmptyTrayEl = GetTickCount();
			m_nEmptyTrayElCase++; m_tEmptyTrayElLoop.Set_LoopTime(10000);
		}
		break;
	case 22:	// 트레이가 있어야하고 탑체크 감지 안되어 있어야한다.
#ifdef EDITION_2ND
		if (/*m_pDX07->iEmptyPortExist && */!m_pDX01->iEmptyPortTopCheck && m_pDX01->iEmptyPortSlideClose) {
			m_nEmptyTrayElCase = 2; m_tEmptyTrayElLoop.Set_LoopTime(20000);	// 20초

		} else if (m_pDX01->iEmptyPortTopCheck) {
			m_nEmptyTrayElCase = 25; m_tEmptyTrayElLoop.Set_LoopTime(10000);
		}
#else
		if (/*m_pDX07->iEmptyPortExist && */!m_pDX07->iEmptyPortTopCheck && m_pDX07->iEmptyPortSlideClose) {
			m_nEmptyTrayElCase = 2; m_tEmptyTrayElLoop.Set_LoopTime(20000);	// 20초

		} else if (m_pDX07->iEmptyPortTopCheck) {
			m_nEmptyTrayElCase = 25; m_tEmptyTrayElLoop.Set_LoopTime(10000);
		}
#endif
		break;

	case 25:	// Move to Base Position
		if (g_objCommon.Check_Position(AX_EMPTY_PORT_Z, 1)) {
			m_tEmptyTrayElLoop.Takt_Start();
			g_objCommon.Move_Position(AX_EMPTY_PORT_Z, 0);
			m_nEmptyTrayElCase++; m_tEmptyTrayElLoop.Set_LoopTime(10000);
		}
		break;
	case 26:	// 알람 처리
		if (g_objCommon.Check_Position(AX_EMPTY_PORT_Z, 0)) {
			m_tEmptyTrayElLoop.Takt_Save(19, 3);
			//m_pDY15->oDoor10Unlock = TRUE;
			//g_objAJinAXL.Write_Output(15);
			//gData.bEmptyZWait = TRUE;	// 현재는 알람 처리하도록 해준다.
			//g_objCommon.Show_Error(4926);	// Empty Tray Full Alarm	error로 잡으면 안된다.
			m_nEmptyTrayElCase = 0;
			gData.bEmptyFull = TRUE;
			g_objCommon.Show_Alarm("Tray가 가득 찼습니다. Tray를 제거해 주십시오."); return FALSE;

		}
		break;

	case 30:	//
		if (g_objAJinAXL.Is_Done(AX_EMPTY_PORT_Z)) {
			m_tEmptyTrayElLoop.Takt_Start();
			g_objCommon.Move_Position(AX_EMPTY_PORT_Z, 0);
			m_nEmptyTrayElCase++; m_tEmptyTrayElLoop.Set_LoopTime(10000);
		}
		break;
	case 31:	// 알람 처리
		if (g_objCommon.Check_Position(AX_EMPTY_PORT_Z, 0)) {
			m_tEmptyTrayElLoop.Takt_Save(19, 3);
			//g_objCommon.Show_Error(4932);	// Empty Tray Full Alarm
			m_nEmptyTrayElCase = 0; 
			gData.bEmptyFull = TRUE;
			g_objCommon.Show_Alarm("Tray가 가득 찼습니다. Tray를 제거해 주십시오."); return FALSE;
		}
		break;


		// 6. Unload (Lot End)	// Empty Tray Y에서 종료 시켜준다.
	case 51:	// Buffer Z Move to Base Down Position
		if (m_bUnloadLotEnd && m_nEmptyTrayXCase == 0) {
			m_tEmptyTrayElLoop.Takt_Start();
			g_objCommon.Move_Position(AX_EMPTY_PORT_Z, 0);
			m_nEmptyTrayElCase++; m_tEmptyTrayElLoop.Set_LoopTime(10000);
		}
		break;
	case 52:	// Check Ready Down Position
		if (g_objCommon.Check_Position(AX_EMPTY_PORT_Z, 0)) {
			m_tEmptyTrayElLoop.Takt_Save(19, 3);
			m_nEmptyTrayElCase = 0; m_tEmptyTrayElLoop.Set_LoopTime(5000);
		}
		break;
	}

	// 19. (Error : 4900)
	if (m_tEmptyTrayElLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(4900 + m_nEmptyTrayElCase);
		return FALSE;
	}
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// 20. (Error : 5000)
BOOL CSequenceMain::EmptyTrayY_Run()
{
	static int nEtWorkNo = 0;
	switch (m_nEmptyTrayYCase) {
	case 0:		// Start 시 1로 바뀐다.
		return TRUE;

	case 1:		// 안전 확인.
		if (m_bUnloadLotEnd) {
			if (m_nEmptyTrayElCase == 10) { m_nEmptyTrayElCase = 51; m_tEmptyTrayElLoop.Set_LoopTime(20000); }
			if (m_nEmptyTrayElCase == 0)  { m_nEmptyTrayYCase = 0; }	//Elevator 종료 확인.

		} else if (!m_pDX13->iEmptyTrans2Exist && g_objCommon.Check_Position(AX_EMPTY_TRANS2_Y, 0) &&
			g_objCommon.Get_EmptyTrayYUp() && g_objCommon.Get_EmptyTrayYMasterSlaveOut()) {
				m_dwEmptyTrayY = GetTickCount();
				m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(95000);	// Tray 2장 가져가는걸로 셋팅 되어있으면 무언정지 일때가 발생하여 1분35초 후 알람 나도록 설정.
		}
		break;
	case 2:		// 대기 (Btm2, Empty Tray X 확인)
		// Btm2 검사 위치 아닐때 && Empty Tray X Port 위치 아닐때 && Port에 Tray가 있을때 && Good Tray Buffer에 트레이가 없을때
		if (m_bUnloadLotEnd) {	// Load 할께 없으면 종료.
			if (m_nEmptyTrayElCase == 10) { m_nEmptyTrayElCase = 51; m_tEmptyTrayElLoop.Set_LoopTime(20000); }
			if (m_nEmptyTrayElCase == 0)  { m_nEmptyTrayYCase = 0; }	//Elevator 종료 확인.

		}
#ifdef EDITION_2ND
		else if (m_pDX01->iEmptyPortExist && m_pDX01->iEmptyPortTopCheck && m_nEmptyTrayXCase < 11 &&
			(m_nBtm2PickCase < 7 || m_nBtm2PickCase > 21 ||
			g_objCommon.Check_Position(AX_BTM2_PICKER_X, 2)|| g_objCommon.Check_Position(AX_BTM2_PICKER_X, 3)||
			g_objCommon.Check_Position(AX_BTM2_PICKER_X, 4)|| g_objCommon.Check_Position(AX_BTM2_PICKER_X, 5)) && 
			!m_pDX13->iEmptyTrans2Exist/*!m_pDX12->iGoodTrayBufferBottom*/)
#else
		else if (m_pDX07->iEmptyPortExist && m_pDX07->iEmptyPortTopCheck && m_nEmptyTrayXCase < 11 &&
			(m_nBtm2PickCase < 7 || m_nBtm2PickCase > 21 ||
			g_objCommon.Check_Position(AX_BTM2_PICKER_X, 2)|| g_objCommon.Check_Position(AX_BTM2_PICKER_X, 3)||
			g_objCommon.Check_Position(AX_BTM2_PICKER_X, 4)|| g_objCommon.Check_Position(AX_BTM2_PICKER_X, 5)) && 
			!m_pDX13->iEmptyTrans2Exist/*!m_pDX12->iGoodTrayBufferBottom*/)
#endif
		{
			//(!m_pDX12->iGoodStage1Exist || !m_pDX12->iGoodStage2Exist)) {

			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);

		}
		break;
		//return TRUE;
	case 3:		// Y Axis Move to Load Position
		if (g_objCommon.Check_Position(AX_EMPTY_TRANS2_Y, 0) && (g_objCommon.Check_Position(AX_EMPTY_TRANS1_X, 0) || g_objCommon.Check_Position(AX_EMPTY_TRANS1_X, 3)) &&
			(m_nBtm2PickCase < 7 || m_nBtm2PickCase > 21 ||
			 g_objCommon.Check_Position(AX_BTM2_PICKER_X, 2)|| g_objCommon.Check_Position(AX_BTM2_PICKER_X, 3)||
			 g_objCommon.Check_Position(AX_BTM2_PICKER_X, 4)|| g_objCommon.Check_Position(AX_BTM2_PICKER_X, 5))) {
			m_tEmptyTrayYLoop.Takt_Start();
			g_objCommon.Move_Position(AX_EMPTY_TRANS2_Y, 1);
			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);
		}
		break;
	case 4:		// Position Check
		if (g_objCommon.Check_Position(AX_EMPTY_TRANS2_Y, 1)) {
			m_tEmptyTrayYLoop.Takt_Save(20, 1);
			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(60000);
		}
		break;
	case 5:		// Angle Port 확인 (30sec)
		if (m_nEmptyTrayElCase == 10) {
			double dLimitPos = m_pMoveData->dEmptyPortZ[2] + m_pMoveData->dEmptyTrans2Y[2];	// LimitUp + Up Offset 보다 위로 올라가지 못하게 해준다.
			double dCurPos = g_objAJinAXL.Get_Position(AX_EMPTY_PORT_Z);

			gData.dEmptyPortZUpOffset = dCurPos + m_pMoveData->dEmptyTrans2Y[2];	// 현재 위치 + Up Offset
			if (gData.dEmptyPortZUpOffset > dLimitPos) gData.dEmptyPortZUpOffset = dLimitPos;

			g_objAJinAXL.Move_Absolute(AX_EMPTY_PORT_Z, gData.dEmptyPortZUpOffset, 0.5);	// Offset Move
			m_nEmptyTrayYCase = 10; m_tEmptyTrayYLoop.Set_LoopTime(10000);
		}
		break;

	case 10:	// Empty Tray Down
		if (g_objAJinAXL.Is_MoveDone(AX_EMPTY_PORT_Z, gData.dEmptyPortZUpOffset) && g_objCommon.Check_Position(AX_EMPTY_TRANS2_Y, 1)) {
			m_tEmptyTrayYLoop.Takt_Start();

			gData.dEmptyPortZUpOffset = 0.0;
			g_objCommon.Set_EmptyTrayYDown();
			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);

		} else if (g_objAJinAXL.Is_Done(AX_EMPTY_PORT_Z) && !g_objAJinAXL.Is_MoveDone(AX_EMPTY_PORT_Z, gData.dEmptyPortZUpOffset)) {
			if (!m_tEmptyTrayYLoop.Waiting_Time(100)) break;
			// 매뉴얼로 조치했을수 있기 때문에 무조건 옵셋으로 보내주지 않고 Z축 Slow Up/Down을 다시 해준다.
			// case는 5번으로 보내줘 옵셋을 다시 설정해준다.
#ifdef EDITION_2ND
			if (m_pDX01->iEmptyPortTopCheck)	m_nEmptyTrayElCase = 5;	// Slow Down
			else								m_nEmptyTrayElCase = 2;	// Slow Up
#else
			if (m_pDX07->iEmptyPortTopCheck)	m_nEmptyTrayElCase = 5;	// Slow Down
			else								m_nEmptyTrayElCase = 2;	// Slow Up
#endif
			m_nEmptyTrayYCase = 5; m_tEmptyTrayYLoop.Set_LoopTime(30000);
			//g_objAJinAXL.Move_Absolute(AX_EMPTY_PORT_Z, gData.dEmptyPortZUpOffset, 0.5);	// Offset Move
		}
		break;
	case 11:	// Empty Tray Master In
		if (g_objCommon.Get_EmptyTrayYDown()) {
			m_tEmptyTrayYLoop.Takt_Save(20, 2);
			m_tEmptyTrayYLoop.Takt_Start();
			m_pDY13->oEmptyTrans2MasterIn = TRUE; m_pDY13->oEmptyTrans2MasterOut = FALSE;
			g_objAJinAXL.Write_Output(13);
			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);
		}
		break;
	case 12:	// Empty Tray Slave In
		if (m_pDX13->iEmptyTrans2MasterIn && !m_pDX13->iEmptyTrans2MasterOut) {

			m_pDY13->oEmptyTrans2SlaveIn = TRUE; m_pDY13->oEmptyTrans2SlaveOut = FALSE;
			g_objAJinAXL.Write_Output(13);
			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);
		}
		break;
	case 13:	// Empty Tray Up
		if (g_objCommon.Get_EmptyTrayYMasterSlaveIn()){
			m_tEmptyTrayYLoop.Takt_Save(20, 3);
			m_tEmptyTrayYLoop.Takt_Start();

			g_objCommon.Set_EmptyTrayYUp();
			//g_objAJinAXL.Move_Relative(AX_EMPTY_PORT_Z, -m_pMoveData->dEmptyTrans2Y[2], 0.5);	// return
			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);
		}
		break;
	case 14:	// Angle Port 진행.
		if (g_objAJinAXL.Is_Done(AX_EMPTY_PORT_Z) && g_objCommon.Get_EmptyTrayYUp() && m_pDX13->iEmptyTrans2Exist) {
			if (m_nEmptyTrayElCase == 10) {
				m_tEmptyTrayYLoop.Takt_Save(20, 4);
#ifdef EDITION_2ND
				if (m_pDX01->iEmptyPortTopCheck)	m_nEmptyTrayElCase = 5;	// Slow Down
				else								m_nEmptyTrayElCase = 2;	// Slow Up
#else
				if (m_pDX07->iEmptyPortTopCheck)	m_nEmptyTrayElCase = 5;	// Slow Down
				else								m_nEmptyTrayElCase = 2;	// Slow Up
#endif
				m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);
			}			
		}
		break;
	case 15:	//  안전 확인
		if (m_nBtm2PickCase < 7 || m_nBtm2PickCase > 21 ||
			g_objCommon.Check_Position(AX_BTM2_PICKER_X, 2)|| g_objCommon.Check_Position(AX_BTM2_PICKER_X, 3)||
			g_objCommon.Check_Position(AX_BTM2_PICKER_X, 4)|| g_objCommon.Check_Position(AX_BTM2_PICKER_X, 5)) {	// Btm2 검사 위치 아닐때

			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);
		}
		return TRUE;
	case 16:	// Move to Good Tray Position
		if (g_objCommon.Check_Position(AX_EMPTY_TRANS2_Y, 1) &&
			(m_nBtm2PickCase < 7 || m_nBtm2PickCase > 21 ||
			 g_objCommon.Check_Position(AX_BTM2_PICKER_X, 2)|| g_objCommon.Check_Position(AX_BTM2_PICKER_X, 3)||
			 g_objCommon.Check_Position(AX_BTM2_PICKER_X, 4)|| g_objCommon.Check_Position(AX_BTM2_PICKER_X, 5))) {
			m_tEmptyTrayYLoop.Takt_Start();
			g_objCommon.Move_Position(AX_EMPTY_TRANS2_Y, 0);
			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);
		}
		break;
	case 17:	// Position Check
		if (g_objCommon.Check_Position(AX_EMPTY_TRANS2_Y, 0)) {
			m_tEmptyTrayYLoop.Takt_Save(20, 5);
			m_nEmptyTrayYCase = 20; m_tEmptyTrayYLoop.Set_LoopTime(5000);
		}
		break;

		// Tray Unloading
	case 20:	// Good Tray 확인
// 		if (m_nGoodTray1Case == 10 || m_nGoodTray2Case == 10) {
// 			if (m_nGoodTray1Case == 10)	nEtWorkNo = 1;
// 			if (m_nGoodTray2Case == 10) nEtWorkNo = 2;
// 			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);
// 		}
		// 트레이를 집고 왔는데 전체 LotEnd되면 정리해야하는지 판단해야 한다.
// 		if (m_bUnloadLotEnd && 
// 			((m_nGoodTray1Case ==  0 && m_nGoodTray2Case == 50) ||
// 			 (m_nGoodTray1Case == 50 && m_nGoodTray2Case ==  0))) {
// 
// 			m_nEmptyTrayYCase = 40; m_tEmptyTrayYLoop.Set_LoopTime(5000);
// 		}
		// Buffer에 Tray가 없고 Good Stage가 Tray를 집는 동작이 아니어야한다. 
		if (!m_pDX12->iGoodTrayBufferBottom &&
			((m_nGoodTray1Case <= 1 || m_nGoodTray1Case > 10) ||
			 (m_nGoodTray2Case <= 1 || m_nGoodTray2Case > 10))) {

			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);

		}
		if (m_bUnloadLotEnd && m_pDX12->iGoodTrayBufferBottom && 
			((m_nGoodTray1Case ==  0 && m_nGoodTray2Case == 50) ||
			 (m_nGoodTray1Case == 50 && m_nGoodTray2Case ==  0))) {

			m_nEmptyTrayYCase = 40; m_tEmptyTrayYLoop.Set_LoopTime(5000);
		}
		return TRUE;
	case 21:	// Transfer Down
// 		if (m_pDX13->iEmptyTrans2Exist && g_objCommon.Check_Position(AX_EMPTY_TRANS2_Y, 0) &&
// 			((nEtWorkNo == 1 && g_objCommon.Check_Position(AX_GOOD_STAGE1_Y, 0)) ||
// 			 (nEtWorkNo == 2 && g_objCommon.Check_Position(AX_GOOD_STAGE2_Y, 0)))) {
		if (m_pDX13->iEmptyTrans2Exist && g_objCommon.Check_Position(AX_EMPTY_TRANS2_Y, 0) &&
			m_pDX12->iGoodTrayBuffSupport1In && m_pDX12->iGoodTrayBuffSupport2In) {

			m_tEmptyTrayYLoop.Takt_Start();
			g_objCommon.Set_EmptyTrayYDown();
			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);
		}
		break;
	case 22:	// Slave Out
		if (g_objCommon.Get_EmptyTrayYDown()) {
			m_tEmptyTrayYLoop.Takt_Save(20, 6);
			m_tEmptyTrayYLoop.Takt_Start();
			m_pDY13->oEmptyTrans2SlaveIn = FALSE; m_pDY13->oEmptyTrans2SlaveOut = TRUE;
			g_objAJinAXL.Write_Output(13);
			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);
		}
		break;
	case 23:	// Empty Tray Master Out
		if (!m_pDX13->iEmptyTrans2SlaveIn && m_pDX13->iEmptyTrans2SlaveOut) {

			m_pDY13->oEmptyTrans2MasterIn = FALSE; m_pDY13->oEmptyTrans2MasterOut = TRUE;
			g_objAJinAXL.Write_Output(13);
			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);
		}
		break;
	case 24:	// Transfer Up 
		if (g_objCommon.Get_EmptyTrayYMasterSlaveOut()){
			m_tEmptyTrayYLoop.Takt_Save(20, 7);
			m_tEmptyTrayYLoop.Takt_Start();

			g_objCommon.Set_EmptyTrayYUp();
			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);
		}
		break;
	case 25:	//
		if (!m_pDX13->iEmptyTrans2Exist && g_objCommon.Get_EmptyTrayYUp()) {
			m_tEmptyTrayYLoop.Takt_Save(20, 8);
			m_strLog.Format("Empty Trans Y, %d", GetTickCount() - m_dwEmptyTrayY);
			g_objLogFile.Save_TestLog(m_strLog);

// 			if (nEtWorkNo = 1 && m_nGoodTray1Case == 10)	m_nGoodTray1Case = 11;
// 			if (nEtWorkNo = 2 && m_nGoodTray2Case == 10)	m_nGoodTray2Case = 11;
			m_nEmptyTrayYCase = 1; m_tEmptyTrayYLoop.Set_LoopTime(5000);
		}
		break;

	// 후처리
	case 40:	// 안전 확인 & Move to Port Position
		if (m_nEmptyTrayXCase < 11 && (m_nBtm2PickCase < 7 || m_nBtm2PickCase > 21)) {
			m_tEmptyTrayYLoop.Takt_Start();
			g_objCommon.Move_Position(AX_EMPTY_TRANS2_Y, 1);
			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(10000);
		}
		break;
	case 41:	// Offset Move
		if (g_objCommon.Check_Position(AX_EMPTY_TRANS2_Y, 1)) {
			if (m_nEmptyTrayElCase == 10) {
				m_tEmptyTrayYLoop.Takt_Save(20, 1);
				g_objAJinAXL.Move_Relative(AX_EMPTY_PORT_Z, -m_pMoveData->dEmptyTrans2Y[2], 0.5);	// Offset Move
				m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(10000);
			}
		}
		break;
	case 42:
		if (g_objAJinAXL.Is_Done(AX_EMPTY_PORT_Z) && g_objCommon.Check_Position(AX_EMPTY_TRANS2_Y, 1)) {
			m_tEmptyTrayYLoop.Takt_Start();
			g_objCommon.Set_EmptyTrayYDown();
			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);
		}
		break;
	case 43:	// Empty Tray Master Out
		if (g_objCommon.Get_EmptyTrayYDown()) {
			m_tEmptyTrayYLoop.Takt_Save(20, 2);
			m_tEmptyTrayYLoop.Takt_Start();
			m_pDY13->oEmptyTrans2MasterIn = FALSE; m_pDY13->oEmptyTrans2MasterOut = TRUE;
			g_objAJinAXL.Write_Output(13);
			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);
		}
		break;
	case 44:	// Empty Tray Slave Out
		if (!m_pDX13->iEmptyTrans2MasterIn && m_pDX13->iEmptyTrans2MasterOut) {

			m_pDY13->oEmptyTrans2SlaveIn = FALSE; m_pDY13->oEmptyTrans2SlaveOut = TRUE;
			g_objAJinAXL.Write_Output(13);
			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);
		}
		break;
	case 45:	// Empty Tray Up
		if (g_objCommon.Get_EmptyTrayYMasterSlaveOut()){
			m_tEmptyTrayYLoop.Takt_Save(20, 7);
			m_tEmptyTrayYLoop.Takt_Start();

			g_objCommon.Set_EmptyTrayYUp();
			//g_objAJinAXL.Move_Relative(AX_EMPTY_PORT_Z, m_pMoveData->dEmptyTrans2Y[2], 0.5);	// return
			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);
		}
		break;
	case 46:	// Angle Port 진행.
		if (g_objAJinAXL.Is_Done(AX_EMPTY_PORT_Z) && g_objCommon.Get_EmptyTrayYUp()) {
// 			if (m_nEmptyTrayElCase == 10) {
// 				m_tEmptyTrayYLoop.Takt_Save(20, 4);
// #ifdef EDITION_2ND
// 				if (m_pDX01->iEmptyPortTopCheck)	m_nEmptyTrayElCase = 5;	// Slow Down
// 				else								m_nEmptyTrayElCase = 2;	// Slow Up
// #else
// 				if (m_pDX07->iEmptyPortTopCheck)	m_nEmptyTrayElCase = 5;	// Slow Down
// 				else								m_nEmptyTrayElCase = 2;	// Slow Up
// #endif
// 				m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);
// 			}
			if (m_nEmptyTrayElCase == 10) { m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000); }
		}
		break;
	case 47:	//  안전 확인
		if (m_nBtm2PickCase < 7 || m_nBtm2PickCase > 21) {	// Btm2 검사 위치 아닐때
			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);
		}
		return TRUE;
	case 48:	// Move to Good Tray Position
		if (g_objCommon.Check_Position(AX_EMPTY_TRANS2_Y, 1)) {
			m_tEmptyTrayYLoop.Takt_Start();
			g_objCommon.Move_Position(AX_EMPTY_TRANS2_Y, 0);
			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);
		}
		break;
	case 49:	// Position Check
		if (g_objCommon.Check_Position(AX_EMPTY_TRANS2_Y, 0)) {
			m_tEmptyTrayYLoop.Takt_Save(20, 5);
			m_nEmptyTrayYCase++; m_tEmptyTrayYLoop.Set_LoopTime(5000);
		}
		break;
	case 50:
		if (m_bUnloadLotEnd) {
			if (m_nEmptyTrayElCase == 10) { m_nEmptyTrayElCase = 51; m_tEmptyTrayElLoop.Set_LoopTime(20000); }
			if (m_nEmptyTrayElCase == 0)  { m_nEmptyTrayYCase = 0; }	//Elevator 종료 확인.
		}
		break;
	}
 
	// 20. (Error : 5000)
	if (m_tEmptyTrayYLoop.Over_LoopTime()) {
		g_objCommon.Show_Error(5000 + m_nEmptyTrayYCase);
		return FALSE;
	}
	return TRUE;
}
///////////////////////////////////////////////////////////////////////////////


void CSequenceMain::Test_CSKIM()
{
	m_pThreadNgFullBeep = AfxBeginThread(Thread_NgFullBeep, (LPVOID)(2000));
}


BOOL CSequenceMain::Run_Simulation_04A()
{
#ifdef AJIN_BOARD_USE
	return TRUE;
#endif

	if(!gData.bR04A || gData.bR04B || gData.bR04C) return TRUE;
	

	m_pDX01->iEmptyPortExist =TRUE;
	m_pDX09->iBufferStage1Vacuum = TRUE;
	m_pDX09->iBufferStage2Vacuum = TRUE;

	//m_pDX01->iEmptyPortTopCheck = TRUE;

	if(m_nEmptyTrayElCase == 2){
		m_pDX01->iEmptyPortTopCheck = FALSE;
	}


	if (m_nLoadTrayCase == 10) { // 로드트레이가 포트에서 get
		Sleep(SIM_WAITTIMES); m_pDX01->iLoadStageExist = TRUE; 
	}
	else if(m_nLoadTrayCase == 21){ 
		
	}
		
	if (m_nLoadPickCase == 7) { //로드 피커가 로드트레이에서 get
		Sleep(SIM_WAITTIMES); m_pDX01->iLoadPickerExist = TRUE; 
		Sleep(SIM_WAITTIMES); m_pDX01->iLoadStageExist = FALSE; 
	}
	else if(m_nLoadPickCase == 15 ){ // 로드 피커가 angle port에 put
		Sleep(SIM_WAITTIMES); m_pDX01->iLoadPickerExist = FALSE;
		Sleep(SIM_WAITTIMES); m_pDX02->iAnglePortExist = TRUE;
	}

	if (m_nAngleTray1Case == 4 ) { // 
		Sleep(SIM_WAITTIMES); m_pDX02->iAnglePortExist = FALSE;
		Sleep(SIM_WAITTIMES); m_pDX02->iAngleStage1Exist = TRUE;
	}

	 // angle tray 1 대기중 btm1 피커가 픽
	if(m_nAngleTray1Case == 20 && m_nBtm1PickCase == 5){
		//Sleep(SIM_WAITTIMES); m_pDX02->iAngleStage1Exist = FALSE;
	}
	// angle tray 2 대기중 btm1 피커가 픽
	if(m_nAngleTray2Case == 20 && m_nBtm1PickCase == 5){
		//Sleep(SIM_WAITTIMES); m_pDX02->iAngleStage2Exist = FALSE;
	}

	if (m_nAngleTray2Case == 4 ) { 
		Sleep(SIM_WAITTIMES); m_pDX02->iAnglePortExist = FALSE;
		Sleep(SIM_WAITTIMES); m_pDX02->iAngleStage2Exist = TRUE;
	}

	if(m_nEmptyTrayXCase == 6 ){
		Sleep(SIM_WAITTIMES); m_pDX13->iEmptyTrans1Exist = TRUE;
		if(m_nAngleTray1Case == 30 ) m_pDX02->iAngleStage1Exist = FALSE;
		if(m_nAngleTray2Case == 30 ) m_pDX02->iAngleStage2Exist = FALSE;

	}
	if(m_nEmptyTrayXCase == 17 ){
		Sleep(SIM_WAITTIMES); m_pDX13->iEmptyTrans1Exist = FALSE;
		m_pDX01->iEmptyPortExist = TRUE;
		m_pDX01->iEmptyPortTopCheck = TRUE;
	}	
	
	
	if(m_nEmptyTrayYCase == 2){//4
		g_objCommon.Move_Position(AX_EMPTY_TRANS2_Y, 1);
		g_objCommon.Set_EmptyTrayYDown();
		m_nEmptyTrayYCase = 11;
	}
	if(m_nEmptyTrayYCase == 14){
		m_nEmptyTrayYCase = 15;
		m_pDX01->iEmptyPortExist = FALSE;
		m_pDX01->iEmptyPortTopCheck = FALSE;
		m_pDX13->iEmptyTrans2Exist = TRUE;
	}
	if(m_nEmptyTrayYCase == 22){
		m_pDX13->iEmptyTrans2Exist = FALSE;
		m_pDX12->iGoodTrayBufferBottom = TRUE;
		iGoodTrayBufferCount++;
		iGoodTrayBufferCount++;
	}

	if(m_nGoodTray1Case == 11){
		m_pDX12->iGoodStage1Exist = TRUE;
		iGoodTrayBufferCount--;
		if(iGoodTrayBufferCount <= 0){
			m_pDX12->iGoodTrayBufferBottom = FALSE;
		}
	}

	if(m_nGoodTray2Case == 11){
		m_pDX12->iGoodStage2Exist = TRUE;
		iGoodTrayBufferCount--;
		if(iGoodTrayBufferCount <= 0){
			m_pDX12->iGoodTrayBufferBottom = FALSE;
		}
	}

	if(m_nGoodTray1Case == 27){
		m_pDX12->iGoodStage1Exist = FALSE;
		m_pDX12->iGoodPortBottom = TRUE;
	}
	if(m_nGoodTray2Case == 27){
		m_pDX12->iGoodStage2Exist = FALSE;
		m_pDX12->iGoodPortBottom = TRUE;
	}



	if(m_nInspect1Case == 0 || m_nInspect1Case == 30){
		m_pDX05->iInspectStage1Up = TRUE; // 맨처음 시작시 Up 상태로 시작 가정 
		m_pDX05->iInspectStage1Down = FALSE;
	}

	if(m_nBuffTray1Case == 2){
		m_pDX09->iBufferStage1Vacuum = TRUE;
	}
	else if(m_nBuffTray1Case == 51){
		m_pDX09->iBufferStage1Rotate = TRUE; m_pDX09->iBufferStage1Normal = FALSE;
		m_pDX09->iBufferStage2Up = TRUE; m_pDX09->iBufferStage2Down = FALSE;
	}
	
	if(m_nBuffTray2Case == 2){
		m_pDX09->iBufferStage2Vacuum = TRUE;
	}
	else if(m_nBuffTray2Case == 51){
		m_pDX09->iBufferStage2Rotate = TRUE; m_pDX09->iBufferStage2Normal = FALSE;
		m_pDX09->iBufferStage1Up = TRUE; m_pDX09->iBufferStage1Down = FALSE;
	}
	
	if(m_nEmptyTrayXCase == 13)
	{
		m_nEmptyTrayXCase++;
	}
	
	if(m_nEmptyTrayXCase == 18){
		m_nEmptyTrayXCase = 20;
	}	
	return TRUE;
}



BOOL CSequenceMain::Run_Simulation_04B()
{
#ifdef AJIN_BOARD_USE
	return TRUE;
#endif

	if(gData.bR04A || !gData.bR04B || gData.bR04C) return TRUE;

	m_pDX01->iEmptyPortExist =TRUE;
	m_pDX09->iBufferStage1Vacuum = TRUE;
	m_pDX09->iBufferStage2Vacuum = TRUE;

	//m_pDX01->iEmptyPortTopCheck = TRUE;

	if(m_nEmptyTrayElCase == 2)
	{
		m_pDX01->iEmptyPortTopCheck = FALSE;
	}


	if (m_nLoadTrayCase == 10) { // 로드트레이가 포트에서 get
		Sleep(SIM_WAITTIMES); m_pDX01->iLoadStageExist = TRUE; 
	}
	else if(m_nLoadTrayCase == 21){ 

	}

	if (m_nLoadPickCase == 7) { //로드 피커가 로드트레이에서 get
		Sleep(SIM_WAITTIMES); m_pDX01->iLoadPickerExist = TRUE; 
		Sleep(SIM_WAITTIMES); m_pDX01->iLoadStageExist = FALSE; 
	}
	else if(m_nLoadPickCase == 15 ){ // 로드 피커가 angle port에 put
		Sleep(SIM_WAITTIMES); m_pDX01->iLoadPickerExist = FALSE;
		Sleep(SIM_WAITTIMES); m_pDX02->iAnglePortExist = TRUE;
	}

	if (m_nAngleTray1Case == 4 ) { // 
		Sleep(SIM_WAITTIMES); m_pDX02->iAnglePortExist = FALSE;
		Sleep(SIM_WAITTIMES); m_pDX02->iAngleStage1Exist = TRUE;
	}

	// angle tray 1 대기중 btm1 피커가 픽
	if(m_nAngleTray1Case == 20 && m_nBtm1PickCase == 5){
		//Sleep(SIM_WAITTIMES); m_pDX02->iAngleStage1Exist = FALSE;
	}
	// angle tray 2 대기중 btm1 피커가 픽
	if(m_nAngleTray2Case == 20 && m_nBtm1PickCase == 5){
		//Sleep(SIM_WAITTIMES); m_pDX02->iAngleStage2Exist = FALSE;
	}

	if (m_nAngleTray2Case == 4 ) { 
		Sleep(SIM_WAITTIMES); m_pDX02->iAnglePortExist = FALSE;
		Sleep(SIM_WAITTIMES); m_pDX02->iAngleStage2Exist = TRUE;
	}

	if(m_nEmptyTrayXCase == 6 ){
		Sleep(SIM_WAITTIMES); m_pDX13->iEmptyTrans1Exist = TRUE;
		if(m_nAngleTray1Case == 30 ) m_pDX02->iAngleStage1Exist = FALSE;
		if(m_nAngleTray2Case == 30 ) m_pDX02->iAngleStage2Exist = FALSE;

	}
	if(m_nEmptyTrayXCase == 17 ){
		Sleep(SIM_WAITTIMES); m_pDX13->iEmptyTrans1Exist = FALSE;
		m_pDX01->iEmptyPortExist = TRUE;
		m_pDX01->iEmptyPortTopCheck = TRUE;
	}	


	if(m_nEmptyTrayYCase == 2){//4
		g_objCommon.Move_Position(AX_EMPTY_TRANS2_Y, 1);
		g_objCommon.Set_EmptyTrayYDown();
		m_nEmptyTrayYCase = 11;
	}
	if(m_nEmptyTrayYCase == 14){
		m_nEmptyTrayYCase = 15;
		m_pDX01->iEmptyPortExist = FALSE;
		m_pDX01->iEmptyPortTopCheck = FALSE;
		m_pDX13->iEmptyTrans2Exist = TRUE;
	}
	if(m_nEmptyTrayYCase == 22){
		m_pDX13->iEmptyTrans2Exist = FALSE;
		m_pDX12->iGoodTrayBufferBottom = TRUE;
		iGoodTrayBufferCount++;
		iGoodTrayBufferCount++;
	}

	if(m_nGoodTray1Case == 11){
		m_pDX12->iGoodStage1Exist = TRUE;
		iGoodTrayBufferCount--;
		if(iGoodTrayBufferCount <= 0){
			m_pDX12->iGoodTrayBufferBottom = FALSE;
		}
	}

	if(m_nGoodTray2Case == 11){
		m_pDX12->iGoodStage2Exist = TRUE;
		iGoodTrayBufferCount--;
		if(iGoodTrayBufferCount <= 0){
			m_pDX12->iGoodTrayBufferBottom = FALSE;
		}
	}

	if(m_nGoodTray1Case == 27){
		m_pDX12->iGoodStage1Exist = FALSE;
		m_pDX12->iGoodPortBottom = TRUE;
	}
	if(m_nGoodTray2Case == 27){
		m_pDX12->iGoodStage2Exist = FALSE;
		m_pDX12->iGoodPortBottom = TRUE;
	}



	if(m_nInspect1Case == 0 || m_nInspect1Case == 30){
		m_pDX05->iInspectStage1Up = TRUE; // 맨처음 시작시 Up 상태로 시작 가정 
		m_pDX05->iInspectStage1Down = FALSE;
	}

	if(m_nBuffTray1Case == 2){
		m_pDX09->iBufferStage1Vacuum = TRUE;
	}
	else if(m_nBuffTray1Case == 51){
		m_pDX09->iBufferStage1Rotate = FALSE; m_pDX09->iBufferStage1Normal = TRUE;
		m_pDX09->iBufferStage2Up = TRUE; m_pDX09->iBufferStage2Down = FALSE;
	}

	if(m_nBuffTray2Case == 2){
		m_pDX09->iBufferStage2Vacuum = TRUE;
	}
	else if(m_nBuffTray2Case == 51){
		m_pDX09->iBufferStage2Rotate = FALSE; m_pDX09->iBufferStage2Normal = TRUE;
		m_pDX09->iBufferStage1Up = TRUE; m_pDX09->iBufferStage1Down = FALSE;
	}

	if(m_nEmptyTrayXCase == 13)
	{
		m_nEmptyTrayXCase++;
	}

	if(m_nEmptyTrayXCase == 18){
		m_nEmptyTrayXCase = 20;
	}
	return TRUE;
}





BOOL CSequenceMain::Run_Simulation_04C()
{
#ifdef AJIN_BOARD_USE
	return TRUE;
#endif
	if(gData.bR04A || gData.bR04B || !gData.bR04C) return TRUE;

	m_pDX01->iEmptyPortExist =TRUE;
	m_pDX09->iBufferStage1Vacuum = TRUE;
	m_pDX09->iBufferStage2Vacuum = TRUE;

	//m_pDX01->iEmptyPortTopCheck = TRUE;

	if(m_nEmptyTrayElCase == 2)
	{
		m_pDX01->iEmptyPortTopCheck = FALSE;
	}


	if (m_nLoadTrayCase == 10) { // 로드트레이가 포트에서 get
		Sleep(SIM_WAITTIMES); m_pDX01->iLoadStageExist = TRUE; 
	}
	else if(m_nLoadTrayCase == 21){ 

	}

	if (m_nLoadPickCase == 7) { //로드 피커가 로드트레이에서 get
		Sleep(SIM_WAITTIMES); m_pDX01->iLoadPickerExist = TRUE; 
		Sleep(SIM_WAITTIMES); m_pDX01->iLoadStageExist = FALSE; 
	}
	else if(m_nLoadPickCase == 15 ){ // 로드 피커가 angle port에 put
		Sleep(SIM_WAITTIMES); m_pDX01->iLoadPickerExist = FALSE;
		Sleep(SIM_WAITTIMES); m_pDX02->iAnglePortExist = TRUE;
	}

	if (m_nAngleTray1Case == 4 ) { // 
		Sleep(SIM_WAITTIMES); m_pDX02->iAnglePortExist = FALSE;
		Sleep(SIM_WAITTIMES); m_pDX02->iAngleStage1Exist = TRUE;
	}

	// angle tray 1 대기중 btm1 피커가 픽
	if(m_nAngleTray1Case == 20 && m_nBtm1PickCase == 5){
		//Sleep(SIM_WAITTIMES); m_pDX02->iAngleStage1Exist = FALSE;
	}
	// angle tray 2 대기중 btm1 피커가 픽
	if(m_nAngleTray2Case == 20 && m_nBtm1PickCase == 5){
		//Sleep(SIM_WAITTIMES); m_pDX02->iAngleStage2Exist = FALSE;
	}

	if (m_nAngleTray2Case == 4 ) { 
		Sleep(SIM_WAITTIMES); m_pDX02->iAnglePortExist = FALSE;
		Sleep(SIM_WAITTIMES); m_pDX02->iAngleStage2Exist = TRUE;
	}

	if(m_nEmptyTrayXCase == 6 ){
		Sleep(SIM_WAITTIMES); m_pDX13->iEmptyTrans1Exist = TRUE;
		if(m_nAngleTray1Case == 30 ) m_pDX02->iAngleStage1Exist = FALSE;
		if(m_nAngleTray2Case == 30 ) m_pDX02->iAngleStage2Exist = FALSE;

	}
	if(m_nEmptyTrayXCase == 17 ){
		Sleep(SIM_WAITTIMES); m_pDX13->iEmptyTrans1Exist = FALSE;
		m_pDX01->iEmptyPortExist = TRUE;
		m_pDX01->iEmptyPortTopCheck = TRUE;
	}	


	if(m_nEmptyTrayYCase == 2){//4
		g_objCommon.Move_Position(AX_EMPTY_TRANS2_Y, 1);
		g_objCommon.Set_EmptyTrayYDown();
		m_nEmptyTrayYCase = 11;
	}
	if(m_nEmptyTrayYCase == 14){
		m_nEmptyTrayYCase = 15;
		m_pDX01->iEmptyPortExist = FALSE;
		m_pDX01->iEmptyPortTopCheck = FALSE;
		m_pDX13->iEmptyTrans2Exist = TRUE;
	}
	if(m_nEmptyTrayYCase == 22){
		m_pDX13->iEmptyTrans2Exist = FALSE;
		m_pDX12->iGoodTrayBufferBottom = TRUE;
		iGoodTrayBufferCount++;
		iGoodTrayBufferCount++;
	}

	if(m_nGoodTray1Case == 11){
		m_pDX12->iGoodStage1Exist = TRUE;
		iGoodTrayBufferCount--;
		if(iGoodTrayBufferCount <= 0){
			m_pDX12->iGoodTrayBufferBottom = FALSE;
		}
	}

	if(m_nGoodTray2Case == 11){
		m_pDX12->iGoodStage2Exist = TRUE;
		iGoodTrayBufferCount--;
		if(iGoodTrayBufferCount <= 0){
			m_pDX12->iGoodTrayBufferBottom = FALSE;
		}
	}

	if(m_nGoodTray1Case == 27){
		m_pDX12->iGoodStage1Exist = FALSE;
		m_pDX12->iGoodPortBottom = TRUE;
	}
	if(m_nGoodTray2Case == 27){
		m_pDX12->iGoodStage2Exist = FALSE;
		m_pDX12->iGoodPortBottom = TRUE;
	}
	
	if(m_nInspect1Case == 0 || m_nInspect1Case == 30)
	{
		m_pDX05->iInspectStage1Up = TRUE; // 맨처음 시작시 Up 상태로 시작 가정 
		m_pDX05->iInspectStage1Down = FALSE;
	}

	if(m_nBuffTray1Case == 2)
	{
		m_pDX09->iBufferStage1Vacuum = TRUE;
	}
	else if(m_nBuffTray1Case == 51)
	{
		m_pDX09->iBufferStage1Rotate = FALSE; m_pDX09->iBufferStage1Normal = TRUE;
		m_pDX09->iBufferStage2Up = TRUE; m_pDX09->iBufferStage2Down = FALSE;
	}

	if(m_nBuffTray2Case == 2)
	{
		m_pDX09->iBufferStage2Vacuum = TRUE;
	}
	else if(m_nBuffTray2Case == 51)
	{
		m_pDX09->iBufferStage2Rotate = FALSE; m_pDX09->iBufferStage2Normal = TRUE;
		m_pDX09->iBufferStage1Up = TRUE; m_pDX09->iBufferStage1Down = FALSE;
	}

	if(m_nEmptyTrayXCase == 13)
	{
		m_nEmptyTrayXCase++;
	}

	if(m_nEmptyTrayXCase == 18){
		m_nEmptyTrayXCase = 20;
	}
	return TRUE;
}