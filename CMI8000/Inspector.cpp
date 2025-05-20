// Inspector.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CMI8000.h"
#include "Inspector.h"

#include "LogFile.h"
#include "Common.h"
#include "DataManager.h"

#include "SequenceMain.h"
#include "MESInterface.h"
#include "Dispatcher.h"

#define UDP_PC1_HOST_IP		"192.168.0.21"
#define UDP_PC2_HOST_IP		"192.168.0.22"
#define UDP_PC3_HOST_IP		"192.168.0.23"
#define UDP_PC4_HOST_IP		"192.168.0.24"

#define UDP_PC1_LPORT		8001
#define UDP_PC1_HPORT		8001
#define UDP_PC2_LPORT		8002
#define UDP_PC2_HPORT		8002
#define UDP_PC3_LPORT		8003
#define UDP_PC3_HPORT		8003
#define UDP_PC4_LPORT		8004
#define UDP_PC4_HPORT		8004

IMPLEMENT_DYNAMIC(CInspector, CWnd)

CInspector g_objInspector;

CCriticalSection g_csInspector;	// Send_Command 문제 해결하기 위함

// CInspector

CInspector::CInspector()
{
	m_strRecvCmd = "";

	m_bConnectPC1 = FALSE;
	m_bConnectPC2 = FALSE;
	m_bConnectPC3 = FALSE;
	m_bConnectPC4 = FALSE;

	m_nStatusPC1 = 0;		// Vision PC1 상태 (0:Not Ready, 1:Ready)
	m_nStatusPC2 = 0;		// Vision PC2 상태 (0:Not Ready, 1:Ready)
	m_nStatusPC3 = 0;		// Vision PC3 상태 (0:Not Ready, 1:Ready)
	m_nStatusPC4 = 0;		// Vision PC4 상태 (0:Not Ready, 1:Ready)

	m_bLotReady1 = FALSE;
	m_bLotReady2 = FALSE;
	m_bLotReady3 = FALSE;
	m_bLotReady4 = FALSE;

	for (int i = 0; i < 4; i++) for (int j = 0; j < 2; j++) m_bHddVision[i][j] = FALSE;
}

CInspector::~CInspector()
{
}

BEGIN_MESSAGE_MAP(CInspector, CWnd)
	ON_WM_TIMER()
	ON_MESSAGE(UM_UDP_RECEIVE, OnUdpReceive)
END_MESSAGE_MAP()

// CInspector 메시지 처리기입니다.

void CInspector::Initialize()
{
	BOOL bOpenedPC1 = m_UdpVisionPC1.Open_Socket(UDP_PC1_LPORT, UDP_PC1_HPORT, UDP_PC1_HOST_IP, this);
	BOOL bOpenedPC2 = m_UdpVisionPC2.Open_Socket(UDP_PC2_LPORT, UDP_PC2_HPORT, UDP_PC2_HOST_IP, this);
	BOOL bOpenedPC3 = m_UdpVisionPC3.Open_Socket(UDP_PC3_LPORT, UDP_PC3_HPORT, UDP_PC3_HOST_IP, this);
	BOOL bOpenedPC4 = m_UdpVisionPC4.Open_Socket(UDP_PC4_LPORT, UDP_PC4_HPORT, UDP_PC4_HOST_IP, this);

	if (bOpenedPC1) Set_ConnectRequest(INSPECTOR_PC1);
	if (bOpenedPC2) Set_ConnectRequest(INSPECTOR_PC2);
	if (bOpenedPC3) Set_ConnectRequest(INSPECTOR_PC3);
	if (bOpenedPC4) Set_ConnectRequest(INSPECTOR_PC4);
}

void CInspector::Terminate()
{
	Set_ConnectEnd(INSPECTOR_PC1);
	Set_ConnectEnd(INSPECTOR_PC2);
	Set_ConnectEnd(INSPECTOR_PC3);
	Set_ConnectEnd(INSPECTOR_PC4);

	m_UdpVisionPC1.Close_Socket();
	m_UdpVisionPC2.Close_Socket();
	m_UdpVisionPC3.Close_Socket();
	m_UdpVisionPC4.Close_Socket();
}

///////////////////////////////////////////////////////////////////////////////
// UDP Socket Message

void CInspector::OnTimer(UINT_PTR nIDEvent)
{
	KillTimer(nIDEvent);
	switch (nIDEvent) {
	case INSPECTOR_PC1:	m_nStatusPC1 = 0; break;
	case INSPECTOR_PC2:	m_nStatusPC2 = 0; break;
	case INSPECTOR_PC3:	m_nStatusPC3 = 0; break;
	case INSPECTOR_PC4:	m_nStatusPC4 = 0; break;
	}
	CWnd::OnTimer(nIDEvent);
}

LRESULT CInspector::OnUdpReceive(WPARAM wLocalPort, LPARAM lParam)
{
	UINT nPort = (UINT)wLocalPort;
	int nInspector = 0, nLen = 0;
	BYTE byRecv[1024] = { 0 };
	CString strLog;

	if (nPort == UDP_PC1_HPORT) { nInspector = INSPECTOR_PC1; nLen = m_UdpVisionPC1.Read_Socket(byRecv); }
	if (nPort == UDP_PC2_HPORT) { nInspector = INSPECTOR_PC2; nLen = m_UdpVisionPC2.Read_Socket(byRecv); }
	if (nPort == UDP_PC3_HPORT) { nInspector = INSPECTOR_PC3; nLen = m_UdpVisionPC3.Read_Socket(byRecv); }
	if (nPort == UDP_PC4_HPORT) { nInspector = INSPECTOR_PC4; nLen = m_UdpVisionPC4.Read_Socket(byRecv); }

	if (nInspector == 0 || nLen < 1) {
		strLog.Format("[H<-V%d] : Local Port (%d) Mismatch or Receive Data Zero (%d)", nInspector, nPort, nLen);
		g_objLogFile.Save_InspectorLog(strLog);
		return 0;
	}

	CString strRecvSocket;
	strRecvSocket.Format("%s", byRecv);
	m_strRecvCmd += strRecvSocket;

	while (!m_strRecvCmd.IsEmpty()) {
		int nStart = m_strRecvCmd.Find("@");
		int nEnd = m_strRecvCmd.Find("\n");

		if (nEnd < 0) break;	// 버퍼에 들어오는 중...

		if (nStart < 0 || nStart > nEnd) {
			strLog.Format("[H<-V%d] : <<Error>> %s : Start(%d), End(%d)", nInspector, m_strRecvCmd, nStart, nEnd);
			g_objLogFile.Save_InspectorLog(strLog);
			m_strRecvCmd.Delete(0, nEnd + 1);	// 쓰레기값이 채워져 있어서...
			continue;
		}

		CString strRecv = m_strRecvCmd.Mid(nStart + 1, nEnd - nStart - 1);
		m_strRecvCmd.Delete(0, nEnd + 1);

		char chSep = ',';
		CString strCmd, strOp;

		AfxExtractSubString(strCmd, strRecv, 0, chSep);
		AfxExtractSubString(strOp, strRecv, 1, chSep);

		// Inspector Log ////////////////////////////////////////
		if (strCmd != "HEART" && strOp != "BEAT") {
			strLog.Format("[H<-V%d] : %s", nInspector, strRecv);
			g_objLogFile.Save_InspectorLog(strLog);
		}
		/////////////////////////////////////////////////////////

		EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
		if (nInspector == INSPECTOR_PC1 && !pEquipData->bUseVisionAlign && !pEquipData->bUseInspectAngle && !pEquipData->bUseInspectBtm1) return 0;
		if (nInspector == INSPECTOR_PC2 && !pEquipData->bUseInspectTop1) return 0;
		if (nInspector == INSPECTOR_PC3 && !pEquipData->bUseInspectTop2) return 0;
		if (nInspector == INSPECTOR_PC4 && !pEquipData->bUseInspectBtm2) return 0;

		CString strArg[7];
		for (int i = 0; i < 7; i++) AfxExtractSubString(strArg[i], strRecv, i + 2, chSep);

		if (strCmd == "CONNECT") {
			if (strOp == "REQUEST")	Get_ConnectRequest(nInspector);
			else if (strOp == "REPLY") Get_ConnectReply(nInspector);
			else if (strOp == "END") Get_ConnectEnd(nInspector);

		} else if (strCmd == "STATUS") {
			if (strOp == "REQUEST")	Get_StatusRequest(nInspector);
			else if (strOp == "REPLY") Get_StatusReply(nInspector, strArg[0]);
			else if (strOp == "UPDATE") Get_StatusUpdate(nInspector, strArg[0]);

		} else if (strCmd == "LOT") {
			if (strOp == "READY") Get_LotReady(nInspector, strArg[0], strArg[1], strArg[2], strArg[3]);

		} else if (strCmd == "RELOAD") {
			if (strOp == "REQUEST") Get_ReloadRequest(nInspector, strArg[0]);

		} else if (strCmd == "SCAN") {
			if (strOp == "COMPLETE") Get_ScanComplete(nInspector, strArg[0], strArg[1], strArg[2], strArg[3], strArg[4]);

		} else if (strCmd == "INSPECT") {
			if (strOp == "COMPLETE") Get_InspectComplete(nInspector, strArg[0], strArg[1], strArg[2], strArg[3], strArg[4], strArg[5], strArg[6]);
			else if (strOp == "ROS") g_objDispatcher.Set_InspectRos(strRecv);

		} else if (strCmd == "AMOVE") {
			if (strOp == "REQUEST") Get_AMoveRequest(nInspector, strArg[0], strArg[1]);

		} else if (strCmd == "POSITION") {
			if (strOp == "REQUEST") Get_PositionRequest(nInspector, strArg[0]);

		} else if (strCmd == "ALIGN") {
			int nStrCnt = 0;
			CString str[7];
			for (int i = 0; i < 7; i++) {
				AfxExtractSubString(str[i], strRecv, i, chSep);
				nStrCnt += (str[i].GetLength() + 1);
			}
			CString strResult = strRecv.Mid(nStrCnt, strRecv.GetLength() - nStrCnt);
			if (strOp == "COMPLETE") Get_AlignComplete(nInspector, strArg[0], strArg[1], strArg[2], strArg[3], strArg[4], strResult);
			else if (strOp == "REPLY") gData.bAlignReply = TRUE;

		} else if (strCmd == "SHIFT") {
			if (strOp == "REQUEST") Get_ShiftRequest(nInspector, strArg[0]);

		} else if (strCmd == "SMOVE") {
			if (strOp == "REQUEST") Get_SMoveRequest(nInspector, strArg[0], strArg[1]);

		} else if (strCmd == "BARCODE") {
			if (strOp == "RESULT") Get_BarcodeResult(nInspector, strArg[0], strArg[1], strArg[2], strArg[3], strArg[4]);

		} else if (strCmd == "TIME") {
			if (strOp == "UPDATE") Get_TimeUpdate(nInspector, strArg[0]);

		} else if (strCmd == "ERROR") {
			if (strOp == "REQUEST") Get_ErrorRequest(nInspector, strArg[0], strArg[1], strArg[2], strArg[3], strArg[4], strArg[5]);
		
		} else if (strCmd == "HDD") {
			if (strOp == "REPLY") Get_HddReply(nInspector, strArg[0]);

		} else if (strCmd == "HEART") {
			if (strOp == "BEAT") Get_HeartBeat(nInspector);
		}
	}

	return 1;
}

///////////////////////////////////////////////////////////////////////////////
// Get Command

void CInspector::Get_ConnectRequest(int nInspector)
{
	if (nInspector == INSPECTOR_PC1) m_bConnectPC1 = TRUE;
	if (nInspector == INSPECTOR_PC2) m_bConnectPC2 = TRUE;
	if (nInspector == INSPECTOR_PC3) m_bConnectPC3 = TRUE;
	if (nInspector == INSPECTOR_PC4) m_bConnectPC4 = TRUE;
	Set_ConnectReply(nInspector);
}

void CInspector::Get_ConnectReply(int nInspector)
{
	if (nInspector == INSPECTOR_PC1) m_bConnectPC1 = TRUE;
	if (nInspector == INSPECTOR_PC2) m_bConnectPC2 = TRUE;
	if (nInspector == INSPECTOR_PC3) m_bConnectPC3 = TRUE;
	if (nInspector == INSPECTOR_PC4) m_bConnectPC4 = TRUE;
}

void CInspector::Get_ConnectEnd(int nInspector)
{
	if (nInspector == INSPECTOR_PC1) m_bConnectPC1 = FALSE;
	if (nInspector == INSPECTOR_PC2) m_bConnectPC2 = FALSE;
	if (nInspector == INSPECTOR_PC3) m_bConnectPC3 = FALSE;
	if (nInspector == INSPECTOR_PC4) m_bConnectPC4 = FALSE;
}

void CInspector::Get_StatusRequest(int nInspector)
{
	BOOL bStatus = g_objSequenceMain.Is_MainThreadRun();
	if (bStatus) Set_StatusReply(nInspector, 1);
	else Set_StatusReply(nInspector, 0);
}

void CInspector::Get_StatusReply(int nInspector, CString sStatus)
{
	if (nInspector == INSPECTOR_PC1) m_nStatusPC1 = atoi(sStatus);
	if (nInspector == INSPECTOR_PC2) m_nStatusPC2 = atoi(sStatus);
	if (nInspector == INSPECTOR_PC3) m_nStatusPC3 = atoi(sStatus);
	if (nInspector == INSPECTOR_PC4) m_nStatusPC4 = atoi(sStatus);
}

void CInspector::Get_StatusUpdate(int nInspector, CString sStatus)
{
	KillTimer(nInspector);
	if (nInspector == INSPECTOR_PC1) m_nStatusPC1 = atoi(sStatus);
	if (nInspector == INSPECTOR_PC2) m_nStatusPC2 = atoi(sStatus);
	if (nInspector == INSPECTOR_PC3) m_nStatusPC3 = atoi(sStatus);
	if (nInspector == INSPECTOR_PC4) m_nStatusPC4 = atoi(sStatus);
	SetTimer(nInspector, 15000, NULL);
}

void CInspector::Get_LotReady(int nInspector, CString sLotId, CString sPortNo, CString sProgVer, CString sParaVer)
{
	int nPortNo = atoi(sPortNo);
	if (sLotId != gData.sLotID[nPortNo - 1]) return;
	if (nInspector == INSPECTOR_PC1) m_bLotReady1 = TRUE;
	if (nInspector == INSPECTOR_PC2) m_bLotReady2 = TRUE;
	if (nInspector == INSPECTOR_PC3) m_bLotReady3 = TRUE;
	if (nInspector == INSPECTOR_PC4) m_bLotReady4 = TRUE;

	CString strLog;
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	if ((pEquipData->sVisionProgVer == "0000") || (pEquipData->sVisionParaVer == "0000")) return;
	if (sProgVer != pEquipData->sVisionProgVer || sParaVer != pEquipData->sVisionParaVer) {
		strLog.Format("Vision PC %d 의 프로그램 버전 또는 파라메터 버전을 확인하여 주십시오.", nInspector);
		g_objLogFile.Save_InspectorLog(strLog);

		gData.bVisionVerErr = TRUE; 
		m_bLotReady1 = FALSE; m_bLotReady2 = FALSE; m_bLotReady3 = FALSE; m_bLotReady4 = FALSE;

		// 비전 프로그램 완료되면 아래 주석 풀것. 2020.08.27
		if (nInspector == INSPECTOR_PC1) g_objCommon.Show_Error(6141);
		if (nInspector == INSPECTOR_PC2) g_objCommon.Show_Error(6142);
		if (nInspector == INSPECTOR_PC3) g_objCommon.Show_Error(6143);
		if (nInspector == INSPECTOR_PC4) g_objCommon.Show_Error(6144);
	}
}

void CInspector::Get_ReloadRequest(int nInspector, CString sPc)
{
	// bScanDone[5] ==> 0:Angle, 1:Btm1, 2:Top1, 3:Top2, 4:Btm2
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	if (nInspector == INSPECTOR_PC1) {			// Align, Angle, Btm1
		if (sPc != "PC1") return;
		Set_ReloadComplete(INSPECTOR_PC1, sPc);

		if (pEquipData->bUseVisionAlign) {	// Align
			int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_VISION_ANGLE);
			if (nCase == 5) g_objSequenceMain.Set_MainRunCase(AUTO_VISION_ANGLE, 4);
		}

		if (pEquipData->bUseInspectAngle && !gData.bScanDone[0]) {	// Angle
			int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_VISION_ANGLE);
			if (nCase == 15 && nCase < 19) g_objSequenceMain.Set_MainRunCase(AUTO_VISION_ANGLE, 13);
		}

		if (pEquipData->bUseInspectBtm1 && !gData.bScanDone[1]) {	// Btm1
			int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_VISION_BTM1);
			if (nCase >= 10 && nCase < 15) { gData.bReload[1] = TRUE; g_objSequenceMain.Set_MainRunCase(AUTO_VISION_BTM1, 15); }
		}

		int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_SORT_PICKER1);
		if (nCase == 6) {
			for (int i = 4; i >= 0; i--) { Set_InspectResult(1, gData.nPNoSortPick[0], gData.nTNoSortPick[0][i], gData.nCNoSortPick[0][i]); }
		}

		nCase = g_objSequenceMain.Get_MainRunCase(AUTO_SORT_PICKER2);
		if (nCase == 6) {
			for (int i = 4; i >= 0; i--) { Set_InspectResult(1, gData.nPNoSortPick[1], gData.nTNoSortPick[1][i], gData.nCNoSortPick[1][i]); }
		}		

	} else if (nInspector == INSPECTOR_PC2) {	// Top1
		if (sPc != "PC2") return;
		Set_ReloadComplete(INSPECTOR_PC2, sPc);
		
		if (pEquipData->bUseInspectTop1 && !gData.bScanDone[2]) {	// Top1
			int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION1);
			if (nCase >= 10 && nCase < 15) { gData.bReload[2] = TRUE; g_objSequenceMain.Set_MainRunCase(AUTO_INSPECTION1, 15); }

			nCase = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION2);
			if (nCase >= 10 && nCase < 15) { gData.bReload[2] = TRUE; g_objSequenceMain.Set_MainRunCase(AUTO_INSPECTION2, 15); }

			nCase = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION3);
			if (nCase >= 10 && nCase < 15) { gData.bReload[2] = TRUE; g_objSequenceMain.Set_MainRunCase(AUTO_INSPECTION3, 15); }
		}

		int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_SORT_PICKER1);
		if (nCase == 6) {
			for (int i = 4; i >= 0; i--) { Set_InspectResult(2, gData.nPNoSortPick[0], gData.nTNoSortPick[0][i], gData.nCNoSortPick[0][i]); }
		}

		nCase = g_objSequenceMain.Get_MainRunCase(AUTO_SORT_PICKER2);
		if (nCase == 6) {
			for (int i = 4; i >= 0; i--) { Set_InspectResult(2, gData.nPNoSortPick[1], gData.nTNoSortPick[1][i], gData.nCNoSortPick[1][i]); }
		}

	} else if (nInspector == INSPECTOR_PC3) {	// Top2
		if (sPc != "PC3") return;
		Set_ReloadComplete(INSPECTOR_PC3, sPc);

		if (pEquipData->bUseInspectTop2 && !gData.bScanDone[3]) {	// Top2
			int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION1);
			if (nCase >= 20 && nCase < 25) { gData.bReload[3] = TRUE; g_objSequenceMain.Set_MainRunCase(AUTO_INSPECTION1, 25); }

			nCase = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION2);
			if (nCase >= 20 && nCase < 25) { gData.bReload[3] = TRUE; g_objSequenceMain.Set_MainRunCase(AUTO_INSPECTION2, 25); }

			nCase = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION3);
			if (nCase >= 20 && nCase < 25) { gData.bReload[3] = TRUE; g_objSequenceMain.Set_MainRunCase(AUTO_INSPECTION3, 25); }
		}

		int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_SORT_PICKER1);
		if (nCase == 6) {
			for (int i = 4; i >= 0; i--) { Set_InspectResult(3, gData.nPNoSortPick[0], gData.nTNoSortPick[0][i], gData.nCNoSortPick[0][i]); }
		}

		nCase = g_objSequenceMain.Get_MainRunCase(AUTO_SORT_PICKER2);
		if (nCase == 6) {
			for (int i = 4; i >= 0; i--) { Set_InspectResult(3, gData.nPNoSortPick[1], gData.nTNoSortPick[1][i], gData.nCNoSortPick[1][i]); }
		}

	} else if (nInspector == INSPECTOR_PC4) {	// BTM2
		if (sPc != "PC4") return;
		Set_ReloadComplete(INSPECTOR_PC4, sPc);

		if (pEquipData->bUseInspectAngle && !gData.bScanDone[4]) {	// BTM2
			int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_VISION_BTM2);
			if (nCase >= 10 && nCase < 13) { gData.bReload[4] = TRUE; g_objSequenceMain.Set_MainRunCase(AUTO_VISION_BTM2, 15); }
		}

		int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_SORT_PICKER1);
		if (nCase == 6) {
			for (int i = 4; i >= 0; i--) { Set_InspectResult(4, gData.nPNoSortPick[0], gData.nTNoSortPick[0][i], gData.nCNoSortPick[0][i]); }
		}

		nCase = g_objSequenceMain.Get_MainRunCase(AUTO_SORT_PICKER2);
		if (nCase == 6) {
			for (int i = 4; i >= 0; i--) { Set_InspectResult(4, gData.nPNoSortPick[1], gData.nTNoSortPick[1][i], gData.nCNoSortPick[1][i]); }
		}
	}
}

void CInspector::Get_ScanComplete(int nInspector, CString sGbn, CString sLotId, CString sPortNo, CString sTrayNo, CString sCmNo)
{
	int	nTNo = atoi(sTrayNo) - 1;
	int	nCNo = atoi(sCmNo) - 1;
	if (nTNo < 0 || nTNo > 39 || nCNo < 0 || nCNo > 44) { g_objCommon.Show_Error(6101); return; }

	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	// 0:Angle, 1:Btm1, 2:Top1, 3:Top2, 4:Btm2
	if (sGbn == "AG") {
		int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_VISION_ANGLE);
		if (nCase != 15) { Exception_Log("Scan Complete", sGbn, nCase); return; }
		gData.bScanDone[0] = TRUE;
		int nY = nCNo / gData.nTrayX;
		int nX = nCNo % gData.nTrayX;
		if (gData.InfoAnglePortTray[nY][nX] == 9) gData.InfoAnglePortTray[nY][nX] = 1;	//Scan Done
		g_objSequenceMain.Set_MainRunCase(AUTO_VISION_ANGLE, 19);

	} else if (sGbn == "B1SP") {
		int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_VISION_BTM1);
		if (nCase != 10) { Exception_Log("Scan Complete", sGbn, nCase); return; }
		m_nB1ScanCnt++;
		if (m_nB1ScanCnt < m_nB1ScanReq) return;
		gData.bScanDone[1] = TRUE;
		g_objSequenceMain.Set_MainRunCase(AUTO_VISION_BTM1, 15);

	} else if (sGbn == "T1") {
		int nCase = 0;
		int nCase1 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION1);
		int nCase2 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION2);
		int nCase3 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION3);
		
		if (nCase1 == 10) nCase = 1;
		if (nCase2 == 10) nCase = 2;
		if (nCase3 == 10) nCase = 3;
		if (nCase == 0) { Exception_Log("Scan Complete", sGbn, nCase); return; }
		m_nT1ScanCnt++;
		if (m_nT1ScanCnt < m_nT1ScanReq) return;
		gData.bScanDone[2] = TRUE;

		if (nCase == 1) g_objSequenceMain.Set_MainRunCase(AUTO_INSPECTION1, 15);
		if (nCase == 2) g_objSequenceMain.Set_MainRunCase(AUTO_INSPECTION2, 15);
		if (nCase == 3) g_objSequenceMain.Set_MainRunCase(AUTO_INSPECTION3, 15);

 	} else if (sGbn == "T2") {
		int nCase = 0;
		int nCase1 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION1);
		int nCase2 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION2);
		int nCase3 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION3);

		if (nCase1 == 20) nCase = 1;
		if (nCase2 == 20) nCase = 2;
		if (nCase3 == 20) nCase = 3;
		if (nCase == 0) { Exception_Log("Scan Complete", sGbn, nCase); return; }
		m_nT2ScanCnt++;
		if (m_nT2ScanCnt < m_nT2ScanReq) return;
		gData.bScanDone[3] = TRUE;

		if (nCase == 1) g_objSequenceMain.Set_MainRunCase(AUTO_INSPECTION1, 25);
		if (nCase == 2) g_objSequenceMain.Set_MainRunCase(AUTO_INSPECTION2, 25);
		if (nCase == 3) g_objSequenceMain.Set_MainRunCase(AUTO_INSPECTION3, 25);

	} else if (sGbn == "B2") {
		int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_VISION_BTM2);
		if (nCase != 10) { Exception_Log("Scan Complete", sGbn, nCase); return; }
		m_nB2ScanCnt++;
		if (m_nB2ScanCnt < m_nB2ScanReq) return;
		gData.bScanDone[4] = TRUE;
		g_objSequenceMain.Set_MainRunCase(AUTO_VISION_BTM2, 15);

	} else {
		g_objCommon.Show_Error(6102);
	}
}

void CInspector::Get_InspectComplete(int nInspector, CString sGbn, CString sLotId, CString sPortNo, CString sTrayNo, CString sCmNo, CString sJudge, CString sNGCode)
{
	int nPx = atoi(sPortNo) - 1;
	int	nTx = atoi(sTrayNo) - 1;
	int	nCx = atoi(sCmNo) - 1;
	if (nPx < 0 || nPx > 1 || nTx < 0 || nTx > 29 || nCx < 0 || nCx > 39) { g_objCommon.Show_Error(6101); return; }

	// byInspectDone[40][45] : 검사완료 (5bit 사용 ==> 0:Angle, 1:Btm1, 2:Top1, 3:Top2, 4:Btm2)
	int nV = (sGbn == "AG" ? 0 : (sGbn == "B1SP" ? 1 : (sGbn == "T1" ? 2 : (sGbn == "T2" ? 3 : (sGbn == "B2" ? 4 : -1)))));
	if (nV == -1) { g_objCommon.Show_Error(6102); return; }

	if(nPx==0){
		gData.cJudgeCode1[nTx][nCx][nV] = *(LPSTR)(LPCTSTR)sJudge;
	}
	if(nPx==1){
		gData.cJudgeCode2[nTx][nCx][nV] = *(LPSTR)(LPCTSTR)sJudge;
	}


	if (sJudge != "G" && sNGCode.GetLength() < 2) sNGCode = "NON";	// Good 일때 NG Code는 Space(" ")

	if (sJudge == "N1" || sJudge == "N2" || sJudge == "N3" || sJudge == "N4") {
		gMes.sJudge[nPx][nTx][nCx] = sJudge;
		gMes.sNGCode[nPx][nTx][nCx] = gData.sNGData[nPx][nTx][nCx][nV] = sNGCode;
		
	}

	int nMode = theApp.Get_MainMode();
	int nPreInfo = gData.nInspectInfo[nPx][nTx][nCx];

	// 우선순위 : N4(8) -> B(7) -> N3(6) -> N2(5) -> N1(4)
	if		(sJudge == "N4") { if (nPreInfo < 8 || nPreInfo > 8) gData.nInspectInfo[nPx][nTx][nCx] = 8; }	// N4
	else if	(sJudge == "B")  { if (nPreInfo < 7 || nPreInfo > 8) gData.nInspectInfo[nPx][nTx][nCx] = 7; }	// BS
	else if	(sJudge == "N3") { if (nPreInfo < 6 || nPreInfo > 8) gData.nInspectInfo[nPx][nTx][nCx] = 6; }	// N3
	else if	(sJudge == "N2") { if (nPreInfo < 5 || nPreInfo > 8) gData.nInspectInfo[nPx][nTx][nCx] = 5; }	// N2
	else if (sJudge == "N1") { if (nPreInfo < 4 || nPreInfo > 8) gData.nInspectInfo[nPx][nTx][nCx] = 4; }	// N1
	else if (sJudge == "G")	 { if (nPreInfo < 2 || nPreInfo > 8) gData.nInspectInfo[nPx][nTx][nCx] = 9; }	// Good
	else if (sJudge != "G")  { if (nPreInfo < 2 || nPreInfo > 8) gData.nInspectInfo[nPx][nTx][nCx] = 2; }	// Normal (20180831 유출 때문에 수정.)
		
	gData.byInspectDone[nPx][nTx][nCx] |= (1 << nV);

	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	
	if (!pEquipData->bUseDispatcher) return;

	if (pEquipData->bUseInspectAngle && ((gData.byInspectDone[nPx][nTx][nCx] >> 0) & 1) == 0) return;	// Angle
	if (pEquipData->bUseInspectBtm1  && ((gData.byInspectDone[nPx][nTx][nCx] >> 1) & 1) == 0) return;	// Btm1_Specular
	if (pEquipData->bUseInspectTop1  && ((gData.byInspectDone[nPx][nTx][nCx] >> 2) & 1) == 0) return;	// Top1
	if (pEquipData->bUseInspectTop2  && ((gData.byInspectDone[nPx][nTx][nCx] >> 3) & 1) == 0) return;	// Top2
	if (pEquipData->bUseInspectBtm2  && ((gData.byInspectDone[nPx][nTx][nCx] >> 4) & 1) == 0) return;	// Btm2
}

void CInspector::Get_AMoveRequest(int nInspector, CString sGbn, CString sZ)
{
	double dZ = atof(sZ);
	CString strLog = "";

	int nMode = theApp.Get_MainMode();
	if (sGbn == "B1SP") {
		if (nMode == MODE_WORK || nMode == MODE_OPERATOR) {	// AutoRun
			int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_VISION_BTM1);
			if (nCase != 10) { Exception_Log("AMove Request", sGbn, nCase); return; }
			g_objSequenceMain.m_dBtm1Z = dZ;
			g_objSequenceMain.Set_MainRunCase(AUTO_VISION_BTM1, 11);

		} else if (nMode == MODE_MANUAL) {
			if (!g_objAJinAXL.Is_Home(AX_BTM1_PICKER_Z) || dZ < 0.0) return;
			g_objAJinAXL.Move_Absolute(AX_BTM1_PICKER_Z, dZ);
			DWORD dwStart = GetTickCount();
			while (!g_objAJinAXL.Is_MoveDone(AX_BTM1_PICKER_Z, dZ)) {
				if (GetTickCount() - dwStart > 10000) return;
				DoEvents();
			}
			Set_MoveComplete(nInspector, sGbn);
		}

	} else if (sGbn == "T1") {
		if (nMode == MODE_WORK || nMode == MODE_OPERATOR) {	// AutoRun
			int nCase1 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION1);
			int nCase2 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION2);
			int nCase3 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION3);
			if (nCase1 != 10 && nCase2 != 10 && nCase3 != 10) { Exception_Log("AMove Request", sGbn, nCase1); return; }
			g_objSequenceMain.m_dTop1Z = dZ;
			int nAutoRun = (nCase3 == 10 ? AUTO_INSPECTION3 : (nCase2 == 10 ? AUTO_INSPECTION2 : AUTO_INSPECTION1));
			g_objSequenceMain.Set_MainRunCase(nAutoRun, 11);

		} else if (nMode == MODE_MANUAL) {
			if (!g_objAJinAXL.Is_Home(AX_VISION_TOP1_Z) || dZ < 0.0) return;
			g_objAJinAXL.Move_Absolute(AX_VISION_TOP1_Z, dZ);
			DWORD dwStart = GetTickCount();
			while (!g_objAJinAXL.Is_MoveDone(AX_VISION_TOP1_Z, dZ)) {
				if (GetTickCount() - dwStart > 10000) return;
				DoEvents();
			}
			Set_MoveComplete(nInspector, sGbn);
		}

	} else if (sGbn == "T2" || sGbn == "T3") {
		if (nMode == MODE_WORK || nMode == MODE_OPERATOR) {	// AutoRun
			int nCase1 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION1);
			int nCase2 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION2);
			int nCase3 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION3);
			if (nCase1 != 20 && nCase2 != 20 && nCase3 != 20) { Exception_Log("AMove Request", sGbn, nCase1); return; }
			g_objSequenceMain.m_dTop2Z = dZ;
			int nAutoRun = (nCase3 == 20 ? AUTO_INSPECTION3 : (nCase2 == 20 ? AUTO_INSPECTION2 : AUTO_INSPECTION1));
			g_objSequenceMain.Set_MainRunCase(nAutoRun, 21);

		} else if (nMode == MODE_MANUAL) {
			if (!g_objAJinAXL.Is_Home(AX_VISION_TOP2_Z) || dZ < 0.0) return;
			g_objAJinAXL.Move_Absolute(AX_VISION_TOP2_Z, dZ);
			DWORD dwStart = GetTickCount();
			while (!g_objAJinAXL.Is_MoveDone(AX_VISION_TOP2_Z, dZ)) {
				if (GetTickCount() - dwStart > 10000) return;
				DoEvents();
			}
			Set_MoveComplete(nInspector, sGbn);
		}

	} else if (sGbn == "B2" || sGbn == "B3") {
		if (nMode == MODE_WORK || nMode == MODE_OPERATOR) {	// AutoRun
			int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_VISION_BTM2);
			if (nCase != 10) { Exception_Log("AMove Request", sGbn, nCase); return; }
			g_objSequenceMain.m_dBtm2Z = dZ;
			g_objSequenceMain.Set_MainRunCase(AUTO_VISION_BTM2, 11);

		} else if (nMode == MODE_MANUAL) {
			if (!g_objAJinAXL.Is_Home(AX_BTM2_PICKER_Z) || dZ < 0.0) return;
			g_objAJinAXL.Move_Absolute(AX_BTM2_PICKER_Z, dZ);
			DWORD dwStart = GetTickCount();
			while (!g_objAJinAXL.Is_MoveDone(AX_BTM2_PICKER_Z, dZ)) {
				if (GetTickCount() - dwStart > 10000) return;
				DoEvents();
			}
			Set_MoveComplete(nInspector, sGbn);
		}
	} else {
		g_objCommon.Show_Error(6102);
	}
}

void CInspector::Get_PositionRequest(int nInspector, CString sGbn)
{
	double dZ = 0.0;

	if (sGbn=="AG") { AXIS_STATUS *pStatusZ = g_objAJinAXL.Get_pStatus(AX_VISION_ANGLE_Z); dZ = pStatusZ->dPos; }
	if (sGbn=="B1SP") { AXIS_STATUS *pStatusZ = g_objAJinAXL.Get_pStatus(AX_BTM1_PICKER_Z); dZ = pStatusZ->dPos; }
	if (sGbn=="T1") { AXIS_STATUS *pStatusZ = g_objAJinAXL.Get_pStatus(AX_VISION_TOP1_Z); dZ = pStatusZ->dPos; }
	if (sGbn=="T2" || sGbn=="T3") { AXIS_STATUS *pStatusZ = g_objAJinAXL.Get_pStatus(AX_VISION_TOP2_Z); dZ = pStatusZ->dPos; }
	if (sGbn=="B2" || sGbn=="B3") { AXIS_STATUS *pStatusZ = g_objAJinAXL.Get_pStatus(AX_BTM2_PICKER_Z); dZ = pStatusZ->dPos; }

	Set_PositionReply(nInspector, sGbn, dZ);
}

void CInspector::Get_AlignComplete(int nInspector, CString sLotId, CString sPortNo, CString sTrayNo, CString sSeqNo, CString sCmCnt, CString sResult)
{
	int	nTrayNo = atoi(sTrayNo);
	int nCmCount = atoi(sCmCnt);
	if (nTrayNo < 1 || nTrayNo > 40 || nCmCount < 0 || nCmCount > 32) { g_objCommon.Show_Error(6101); return; }

	CString strTemp;
	char chSep = ',';
	for (int i = 0; i < nCmCount; i++) {
		AfxExtractSubString(strTemp, sResult, i * 3 + 0, chSep); m_dAlignX[i] = atof(strTemp);
		AfxExtractSubString(strTemp, sResult, i * 3 + 1, chSep); m_dAlignY[i] = atof(strTemp);
		AfxExtractSubString(strTemp, sResult, i * 3 + 2, chSep); m_dAlignQ[i] = atof(strTemp);
	}

	int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_VISION_ANGLE);
	if (nCase != 5) { Exception_Log("Align Complete", "AL", nCase); return; }
	g_objSequenceMain.Set_MainRunCase(AUTO_VISION_ANGLE, 8);
}

void CInspector::Get_ShiftRequest(int nInspector, CString sGbn)
{
	CString strLog = "";

	int nMode = theApp.Get_MainMode();

	if (sGbn == "B1SP") {				// Btm1 Shift Y축
		if (nMode == MODE_WORK || nMode == MODE_OPERATOR) {	// AutoRun
			int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_VISION_BTM1);
			if (nCase != 10) { Exception_Log("AMove Request", sGbn, nCase); return; }
			g_objSequenceMain.Set_MainRunCase(AUTO_VISION_BTM1, 13);

		} 

	} else if (sGbn == "T1") {		// Top1 Mirror Z축
		if (nMode == MODE_WORK || nMode == MODE_OPERATOR) {	// AutoRun
			int nCase1 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION1);
			int nCase2 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION2);
			int nCase3 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION3);
			if (nCase1 != 10 && nCase2 != 10 && nCase3 != 10) { Exception_Log("AMove Request", sGbn, nCase1); return; }
			int nAutoRun = (nCase3 == 10 ? AUTO_INSPECTION3 : (nCase2 == 10 ? AUTO_INSPECTION2 : AUTO_INSPECTION1));
			g_objSequenceMain.Set_MainRunCase(nAutoRun, 13);

		} else if (nMode == MODE_MANUAL) {
			if (!g_objAJinAXL.Is_Home(AX_TOP1_MIRROR_Z)) return;
			g_objCommon.Move_Position(AX_TOP1_MIRROR_Z, 1);
			DWORD dwStart = GetTickCount();
			while (!g_objCommon.Check_Position(AX_TOP1_MIRROR_Z, 1)) {
				if (GetTickCount() - dwStart > 10000) return;
				DoEvents();
			}
			Set_ShiftComplete(nInspector, sGbn);
		}

	} else if (sGbn == "T2") {		// Top2 Shift Y축
		if (nMode == MODE_WORK || nMode == MODE_OPERATOR) {	// AutoRun
			int nCase1 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION1);
			int nCase2 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION2);
			int nCase3 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION3);
			if (nCase1 != 20 && nCase2 != 20 && nCase3 != 20) { Exception_Log("AMove Request", sGbn, nCase1); return; }
			int nAutoRun = (nCase3 == 20 ? AUTO_INSPECTION3 : (nCase2 == 20 ? AUTO_INSPECTION2 : AUTO_INSPECTION1));
			g_objSequenceMain.Set_MainRunCase(nAutoRun, 23);

		} else if (nMode == MODE_MANUAL) {
			if (!g_objAJinAXL.Is_Home(AX_TOP2_SHIFT_Y)) return;
			g_objCommon.Move_Position(AX_TOP2_SHIFT_Y, 1);
			DWORD dwStart = GetTickCount();
			while (!g_objCommon.Check_Position(AX_TOP2_SHIFT_Y, 1)) {
				if (GetTickCount() - dwStart > 10000) return;
				DoEvents();
			}
			Set_ShiftComplete(nInspector, sGbn);
		}

	} else {
		g_objCommon.Show_Error(6102);
	}
}

void CInspector::Get_SMoveRequest(int nInspector, CString sGbn, CString sZ)
{
	double dZ = atof(sZ);
	CString strLog = "";

	int nMode = theApp.Get_MainMode();
	if (sGbn == "B1SP") {
		if (nMode == MODE_WORK || nMode == MODE_OPERATOR) {	// AutoRun
			int nCase = g_objSequenceMain.Get_MainRunCase(AUTO_VISION_BTM1);
			if (nCase != 10) { Exception_Log("SMove Request", sGbn, nCase); return; }
			g_objSequenceMain.m_dBtm1Z = dZ;
			g_objSequenceMain.Set_MainRunCase(AUTO_VISION_BTM1, 13);

		} else if (nMode == MODE_MANUAL) {
			
		}

	} else if (sGbn == "T1") {
		if (nMode == MODE_WORK || nMode == MODE_OPERATOR) {	// AutoRun
			int nCase1 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION1);
			int nCase2 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION2);
			int nCase3 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION3);
			if (nCase1 != 10 && nCase2 != 10 && nCase3 != 10) { Exception_Log("SMove Request", sGbn, nCase1); return; }
			g_objSequenceMain.m_dTop1Z = dZ;
			int nAutoRun = (nCase3 == 10 ? AUTO_INSPECTION3 : (nCase2 == 10 ? AUTO_INSPECTION2 : AUTO_INSPECTION1));
			g_objSequenceMain.Set_MainRunCase(nAutoRun, 13);

		} else if (nMode == MODE_MANUAL) {
			if (!g_objAJinAXL.Is_Home(AX_TOP1_MIRROR_Z)) return;
			if (!g_objAJinAXL.Is_Home(AX_VISION_TOP1_Z) || dZ < 0.0) return;

			g_objCommon.Move_Position(AX_TOP1_MIRROR_Z, 1);
			g_objAJinAXL.Move_Absolute(AX_VISION_TOP1_Z, dZ);
			DWORD dwStart = GetTickCount();
			while (!g_objAJinAXL.Is_MoveDone(AX_VISION_TOP1_Z, dZ) || !g_objCommon.Check_Position(AX_TOP1_MIRROR_Z, 1)) {
				if (GetTickCount() - dwStart > 10000) return;
				DoEvents();
			}
			Set_SMoveComplete(nInspector, sGbn);
		}

	} else if (sGbn == "T2" || sGbn == "T3") {
		if (nMode == MODE_WORK || nMode == MODE_OPERATOR) {	// AutoRun
			int nCase1 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION1);
			int nCase2 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION2);
			int nCase3 = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION3);
			if (nCase1 != 20 && nCase2 != 20 && nCase3 != 20) { Exception_Log("SMove Request", sGbn, nCase1); return; }
			g_objSequenceMain.m_dTop2Z = dZ;
			int nAutoRun = (nCase3 == 20 ? AUTO_INSPECTION3 : (nCase2 == 20 ? AUTO_INSPECTION2 : AUTO_INSPECTION1));
			g_objSequenceMain.Set_MainRunCase(nAutoRun, 23);

		} else if (nMode == MODE_MANUAL) {
			if (!g_objAJinAXL.Is_Home(AX_TOP2_SHIFT_Y)) return;
			if (!g_objAJinAXL.Is_Home(AX_VISION_TOP2_Z) || dZ < 0.0) return;

			g_objCommon.Move_Position(AX_TOP2_SHIFT_Y, 1);
			g_objAJinAXL.Move_Absolute(AX_VISION_TOP2_Z, dZ);
			DWORD dwStart = GetTickCount();
			while (!g_objAJinAXL.Is_MoveDone(AX_VISION_TOP2_Z, dZ) || !g_objCommon.Check_Position(AX_TOP2_SHIFT_Y, 1)) {
				if (GetTickCount() - dwStart > 10000) return;
				DoEvents();
			}
			Set_SMoveComplete(nInspector, sGbn);
		}

	} else {
		g_objCommon.Show_Error(6102);
	}
}

void CInspector::Get_BarcodeResult(int nInspector, CString sLotId, CString sPortNo, CString sTrayNo, CString sCmNo, CString sBarcode)
{
	int nPNo = atoi(sPortNo) - 1;
	int	nTNo = atoi(sTrayNo) - 1;
	int	nCNo = atoi(sCmNo) - 1;
	if (nTNo < 0 || nTNo > 39 || nCNo < 0 || nCNo > 44) { g_objCommon.Show_Error(6101); return; }

	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	gMes.sBarID[nPNo][nTNo][nCNo] = sBarcode;
	Set_BarcodeUpdate(INSPECTOR_PC2, sLotId, nPNo+1, nTNo + 1, nCNo + 1, sBarcode);
	Set_BarcodeUpdate(INSPECTOR_PC3, sLotId, nPNo+1, nTNo + 1, nCNo + 1, sBarcode);
	Set_BarcodeUpdate(INSPECTOR_PC4, sLotId, nPNo+1, nTNo + 1, nCNo + 1, sBarcode);
//	g_objMesAgent.Set_CmRequest(nTNo + 1, nCNo + 1, gLot.sLotID[nPNo], sBarcode);

	if (sBarcode == "NOREAD"/* && atoi(sMainNo) != 0 */) {
		gData.nInspectInfo[nPNo][nTNo][nCNo] = 4;
		CString strLog;
		strLog.Format("[Barcode NoRead] LotId(%s), PortNo(%d), TrayNo(%d), CmNo(%d)", gLot.sLotID[nPNo], nPNo, nTNo, nCNo);
		g_objLogFile.Save_HandlerLog(strLog);
	}
}

void CInspector::Get_ErrorRequest(int nInspector, CString sGbn, CString sLotId, CString sPortNo, CString sTrayNo, CString sCmNo, CString sErrNo)
{
	int	nTNo = atoi(sTrayNo) - 1;
	int	nCNo = atoi(sCmNo) - 1;
	int nPNo = atoi(sPortNo) - 1;
	if (nTNo < 0 || nTNo > 39 || nCNo < 0 || nCNo > 44 || nPNo < 0 || nPNo > 2) { g_objCommon.Show_Error(6101); return; }

	int nErrNo = atoi(sErrNo);	// 6120 : Mirror Z Axis Error, 6130: Btm2 Picker Module Error

	CString strErrMsg;
	strErrMsg.Format(" ==> Vision:%s. LotID:%s. PortNo:%d. Tray:%d. CM:%d", sGbn, sLotId, nPNo+1, nTNo+1, nCNo+1);

	g_objCommon.Set_ErrorSubMessage(strErrMsg);

	int nMode = theApp.Get_MainMode();
	if (nErrNo != 6120 && nErrNo != 6121 && (nMode == MODE_WORK || nMode == MODE_OPERATOR)) g_objCommon.Show_Error(nErrNo);

	if (nErrNo == 6120 || nErrNo == 6121) {	// Mirror Z Axis Error
		gData.nTop1MirrorErrNo = nErrNo;
		int nInsp1Case = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION1);
		int nInsp2Case = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION2);
		int nInsp3Case = g_objSequenceMain.Get_MainRunCase(AUTO_INSPECTION3);
		if (nInsp1Case == 10) { gData.bTop1MirrorErr[0] = TRUE; g_objSequenceMain.Set_MainRunCase(AUTO_INSPECTION1, 8); }
		if (nInsp2Case == 10) { gData.bTop1MirrorErr[1] = TRUE; g_objSequenceMain.Set_MainRunCase(AUTO_INSPECTION2, 8);	}
		if (nInsp3Case == 10) { gData.bTop1MirrorErr[2] = TRUE; g_objSequenceMain.Set_MainRunCase(AUTO_INSPECTION3, 8);	}
	}
}

void CInspector::Get_TimeUpdate(int nInspector, CString sTime)
{
	// sTime Format : "2000-01-01 12:30:30 000"
	CString strTemp;
	SYSTEMTIME time;

	strTemp = sTime.Mid(0, 4);  time.wYear = atoi(strTemp);			// Year
	strTemp = sTime.Mid(5, 2);  time.wMonth = atoi(strTemp);		// Month
	strTemp = sTime.Mid(8, 2);  time.wDay = atoi(strTemp);			// Day
	strTemp = sTime.Mid(11, 2); time.wHour = atoi(strTemp);			// Hour
	strTemp = sTime.Mid(14, 2); time.wMinute = atoi(strTemp);		// Minute
	strTemp = sTime.Mid(17, 2); time.wSecond = atoi(strTemp);		// Second
	strTemp = sTime.Mid(20, 3); time.wMilliseconds = atoi(strTemp);	// MilliSecond

	SetLocalTime(&time);
}

void CInspector::Get_HddReply(int nInspector, CString sHddStatus)
{
	int nStatus = atoi(sHddStatus);
	m_bHddVision[nInspector - 1][0] = TRUE;
	m_bHddVision[nInspector - 1][1] = (nStatus == 0 ? TRUE : FALSE);
}

void CInspector::Get_HeartBeat(int nInspector)
{
	KillTimer(nInspector);
	if (nInspector == INSPECTOR_PC1) m_nStatusPC1 = 1;
	if (nInspector == INSPECTOR_PC2) m_nStatusPC2 = 1;
	if (nInspector == INSPECTOR_PC3) m_nStatusPC3 = 1;
	if (nInspector == INSPECTOR_PC4) m_nStatusPC4 = 1;
	SetTimer(nInspector, 15000, NULL);
}

void CInspector::Exception_Log(CString sFunc, CString sGbn, int nCase)
{
	CString strLog;
	strLog.Format("%s Exception ==> Vision:%s, Case:%d", sFunc, sGbn, nCase);
	g_objLogFile.Save_InspectorLog(strLog);
}

/////////////////////////////////////////////////////////////////////////////
// Set Command

void CInspector::Set_ConnectRequest(int nInspector)
{
	CString	strSendCmd;
	strSendCmd.Format("CONNECT,REQUEST");
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_ConnectReply(int nInspector)
{
	CString	strSendCmd;
	strSendCmd.Format("CONNECT,REPLY");
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_ConnectEnd(int nInspector)
{
	CString	strSendCmd;
	strSendCmd.Format("CONNECT,END");
	Send_Command(nInspector, strSendCmd);

	if (nInspector == INSPECTOR_PC1) m_bConnectPC1 = FALSE;
	if (nInspector == INSPECTOR_PC2) m_bConnectPC2 = FALSE;
	if (nInspector == INSPECTOR_PC3) m_bConnectPC3 = FALSE;
	if (nInspector == INSPECTOR_PC4) m_bConnectPC4 = FALSE;
}

void CInspector::Set_StatusRequest(int nInspector)
{
	CString	strSendCmd;
	strSendCmd.Format("STATUS,REQUEST");
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_StatusReply(int nInspector, int nStatus)
{
	CString	strSendCmd;
	strSendCmd.Format("STATUS,REPLY,%d", nStatus);
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_StatusUpdate(int nInspector, int nStatus)
{
	CString	strSendCmd;
	strSendCmd.Format("STATUS,UPDATE,%d", nStatus);
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_LotStart(int nInspector, CString sLotId, int nPortNo, CString sProgVer, CString sParaVer)
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	m_bLotReady1 = m_bLotReady2 = m_bLotReady3 = m_bLotReady4 = FALSE;
	CString	strSendCmd;
	strSendCmd.Format("LOT,START,%s,%d,%d,%d,%s,%s,%s", sLotId, nPortNo, gData.nTrayUseCount[nPortNo-1], gData.nCmUseCount[nPortNo-1], gData.sRecipeName, sProgVer, sParaVer);
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_LotEnd(int nInspector, CString sLotId, int nPortNo)
{
	CString	strSendCmd;
	strSendCmd.Format("LOT,END,%s,%d", sLotId, nPortNo);
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_LoadComplete(int nInspector, CString sGbn, CString sLotId, int nPortNo, int nTNo1, int nTNo2, int nTNo3, int nTNo4, int nCNo1, int nCNo2, int nCNo3, int nCNo4)
{
	CString	strSendCmd, strTemp;

	if (sGbn == "B1SP" || sGbn == "T1" || sGbn == "T2" || sGbn == "B2") {
		int nScanReq = 0;
		if (nCNo1 > 0) nScanReq++;
		if (nCNo2 > 0) nScanReq++;
		if (nCNo3 > 0) nScanReq++;
		if (nCNo4 > 0) nScanReq++;

		if (nCNo1 == 0) { nCNo1 = -1;	nTNo1 = -1; }
		if (nCNo2 == 0) { nCNo2 = -1;	nTNo2 = -1; }
		if (nCNo3 == 0) { nCNo3 = -1;	nTNo3 = -1; }
		if (nCNo4 == 0) { nCNo4 = -1;	nTNo4 = -1; }

		if (nTNo1 == 0) nTNo1 = -1;
		if (nTNo2 == 0) nTNo2 = -1;
		if (nTNo3 == 0) nTNo3 = -1;
		if (nTNo4 == 0) nTNo4 = -1;

		if (sGbn == "B1SP") { m_nB1ScanReq = nScanReq; m_nB1ScanCnt = 0; gData.bScanDone[1] = FALSE; }
		if (sGbn == "T1") { m_nT1ScanReq = nScanReq; m_nT1ScanCnt = 0; gData.bScanDone[2] = FALSE; }
		if (sGbn == "T2") { m_nT2ScanReq = nScanReq; m_nT2ScanCnt = 0; gData.bScanDone[3] = FALSE; }
		if (sGbn == "B2") { m_nB2ScanReq = nScanReq; m_nB2ScanCnt = 0; gData.bScanDone[4] = FALSE; }

	}
	strSendCmd.Format("LOAD,COMPLETE,%s,%s,%d,%d,%d,%d,%d,%d,%d,%d,%d", sGbn, sLotId, nPortNo, nTNo1, nTNo2, nTNo3, nTNo4, nCNo1, nCNo2, nCNo3, nCNo4);
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_MoveComplete(int nInspector, CString sGbn)
{
	CString	strSendCmd;
	strSendCmd.Format("MOVE,COMPLETE,%s", sGbn);
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_PositionReply(int nInspector, CString sGbn, double dZ)
{
	CString	strSendCmd;
	strSendCmd.Format("POSITION,REPLY,%s,%0.3lf", sGbn, dZ);
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_AlignRequest(int nInspector, CString sLotId, int nPortNo, int nTrayNo, int nSeqNo)
{
	CString	strSendCmd;
	strSendCmd.Format("ALIGN,REQUEST,%s,%d,%d,%d", sLotId, nPortNo, nTrayNo, nSeqNo);
	Send_Command(nInspector, strSendCmd);

	memset(m_dAlignX, 0x00, sizeof(double) * 32);
	memset(m_dAlignY, 0x00, sizeof(double) * 32);
	memset(m_dAlignQ, 0x00, sizeof(double) * 32);
}

void CInspector::Set_ShiftComplete(int nInspector, CString sGbn)
{
	CString	strSendCmd;
	strSendCmd.Format("SHIFT,COMPLETE,%s", sGbn);
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_SMoveComplete(int nInspector, CString sGbn)
{
	CString	strSendCmd;
	strSendCmd.Format("SMOVE,COMPLETE,%s", sGbn);
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_BarcodeUpdate(int nInspector, CString sLotId, int nPortNo, int nTrayNo, int nCmNo, CString sBarcode)
{
	CString	strSendCmd;
	strSendCmd.Format("BARCODE,UPDATE,%s,%d,%d,%d,%s", sLotId, nPortNo, nTrayNo, nCmNo, sBarcode);
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_InitialRequest(int nInspector)
{
	CString	strSendCmd;
	strSendCmd.Format("INITIAL,REQUEST");
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_ReloadComplete(int nInspector, CString sPc)
{
	CString	strSendCmd;
	strSendCmd.Format("RELOAD,COMPLETE,%s", sPc);
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_CycleStop(int nInspector)
{
	CString	strSendCmd;
	strSendCmd.Format("CYCLE,STOP");
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_TimeUpdate(int nInspector)
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strTime;
	strTime.Format("%04d-%02d-%02d %02d:%02d:%02d %03d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

	CString	strSendCmd;
	strSendCmd.Format("TIME,UPDATE,%s", strTime);
	Send_Command(nInspector, strSendCmd);
}

void CInspector::Set_HddRequest(int nInspector)
{
	CString strSendCmd;
	strSendCmd.Format("HDD,REQUEST");
	Send_Command(nInspector, strSendCmd);
}
///////////////////////////////////////////////////////////////////////////////

int CInspector::Get_VisionStatus(int nInspector)
{
	switch (nInspector) {
	case INSPECTOR_PC1:	return m_nStatusPC1;
	case INSPECTOR_PC2:	return m_nStatusPC2;
	case INSPECTOR_PC3:	return m_nStatusPC3;
	case INSPECTOR_PC4:	return m_nStatusPC4;
	default:			return 0;
	}
}

BOOL CInspector::Check_LotReady()
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
#ifdef DRY_RUN_TEST
	m_bLotReady1 = TRUE;
	m_bLotReady2 = TRUE;
	m_bLotReady3 = TRUE;
	m_bLotReady4 = TRUE;
#else
	if (!pEquipData->bUseVisionAlign && !pEquipData->bUseInspectAngle && !pEquipData->bUseInspectBtm1) m_bLotReady1 = TRUE;
	if (!pEquipData->bUseInspectTop1) m_bLotReady2 = TRUE;
	if (!pEquipData->bUseInspectTop2) m_bLotReady3 = TRUE;
	if (!pEquipData->bUseInspectBtm2) m_bLotReady4 = TRUE;
#endif

	return (m_bLotReady1 && m_bLotReady2 && m_bLotReady3 && m_bLotReady4);
}

BOOL CInspector::Check_HddReply()
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (!pEquipData->bUseVisionAlign && !pEquipData->bUseInspectAngle && !pEquipData->bUseInspectBtm1) m_bHddVision[0][0] = TRUE;
	if (!pEquipData->bUseInspectTop1) m_bHddVision[1][0] = TRUE;
	if (!pEquipData->bUseInspectTop2) m_bHddVision[2][0] = TRUE;
	if (!pEquipData->bUseInspectBtm2) m_bHddVision[3][0] = TRUE;

	return (m_bHddVision[0][0] && m_bHddVision[1][0] && m_bHddVision[2][0] && m_bHddVision[3][0]);
}

BOOL CInspector::Check_HddStatus()
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (!pEquipData->bUseVisionAlign && !pEquipData->bUseInspectAngle && !pEquipData->bUseInspectBtm1) m_bHddVision[0][1] = TRUE;
	if (!pEquipData->bUseInspectTop1) m_bHddVision[1][1] = TRUE;
	if (!pEquipData->bUseInspectTop2) m_bHddVision[2][1] = TRUE;
	if (!pEquipData->bUseInspectBtm2) m_bHddVision[3][1] = TRUE;
	return (m_bHddVision[0][1] && m_bHddVision[1][1] && m_bHddVision[2][1] && m_bHddVision[3][1]);
}

void CInspector::Set_InspectResult(int nPC, int nPortNo, int nTrayNo, int nCmNo)
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	// byInspectDone[2][40][45] : 검사완료 (5bit 사용 ==> 0:Angle, 1:Btm1, 2:top1, 3:Top2, 4:Btm2)
	CString strErrMsg = "";

	if	(nPC == 1 && pEquipData->bUseInspectAngle && ((gData.byInspectDone[nPortNo-1][nTrayNo-1][nCmNo-1] >> 0) & 1) == 0) { gData.byInspectDone[nPortNo-1][nTrayNo-1][nCmNo-1] |= (1 << 0); gData.nInspectInfo[nPortNo-1][nTrayNo-1][nCmNo-1] = 4;}	// Angle
	if	(nPC == 1 && pEquipData->bUseInspectBtm1  && ((gData.byInspectDone[nPortNo-1][nTrayNo-1][nCmNo-1] >> 1) & 1) == 0) { gData.byInspectDone[nPortNo-1][nTrayNo-1][nCmNo-1] |= (1 << 1); gData.nInspectInfo[nPortNo-1][nTrayNo-1][nCmNo-1] = 4;}	// Btm1
	if	(nPC == 2 && pEquipData->bUseInspectTop1  && ((gData.byInspectDone[nPortNo-1][nTrayNo-1][nCmNo-1] >> 2) & 1) == 0) { gData.byInspectDone[nPortNo-1][nTrayNo-1][nCmNo-1] |= (1 << 2); gData.nInspectInfo[nPortNo-1][nTrayNo-1][nCmNo-1] = 4;}	// Top1
	if	(nPC == 3 && pEquipData->bUseInspectTop2  && ((gData.byInspectDone[nPortNo-1][nTrayNo-1][nCmNo-1] >> 3) & 1) == 0) { gData.byInspectDone[nPortNo-1][nTrayNo-1][nCmNo-1] |= (1 << 3); gData.nInspectInfo[nPortNo-1][nTrayNo-1][nCmNo-1] = 4;}	// Top2
	if	(nPC == 4 && pEquipData->bUseInspectBtm2  && ((gData.byInspectDone[nPortNo-1][nTrayNo-1][nCmNo-1] >> 4) & 1) == 0) { gData.byInspectDone[nPortNo-1][nTrayNo-1][nCmNo-1] |= (1 << 4); gData.nInspectInfo[nPortNo-1][nTrayNo-1][nCmNo-1] = 4;}	// Btm2

}
/////////////////////////////////////////////////////////////////////////////
// UDP Socket Send Message

void CInspector::Send_Command(int nInspector, CString strSend)
{
	// Inspector Log //////////////////////////////////////
	CString strLog;
	strLog.Format("[H->V%d] : %s", nInspector, strSend);
	g_objLogFile.Save_InspectorLog(strLog);
	///////////////////////////////////////////////////////

	g_csInspector.Lock();	// Critical Section

	CString strSendSocket;
	strSendSocket.Format("@%s\n", strSend);

	char chSend[1024] = { 0 };
	int nLength = strSendSocket.GetLength();
	memcpy(chSend, (LPSTR)(LPCSTR)strSendSocket, nLength);

	if (nInspector == INSPECTOR_ALL || nInspector == INSPECTOR_PC1) m_UdpVisionPC1.Write_Socket((BYTE*)chSend, nLength);
	if (nInspector == INSPECTOR_ALL || nInspector == INSPECTOR_PC2) m_UdpVisionPC2.Write_Socket((BYTE*)chSend, nLength);
	if (nInspector == INSPECTOR_ALL || nInspector == INSPECTOR_PC3) m_UdpVisionPC3.Write_Socket((BYTE*)chSend, nLength);
	if (nInspector == INSPECTOR_ALL || nInspector == INSPECTOR_PC4) m_UdpVisionPC4.Write_Socket((BYTE*)chSend, nLength);

	g_csInspector.Unlock();	// Critical Section
}
/////////////////////////////////////////////////////////////////////////////

void CInspector::DoEvents(int nSleep)
{
	MSG msg;
	if (PeekMessage(&msg, NULL, NULL, NULL, PM_REMOVE)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	if (nSleep > 0) Sleep(nSleep);
}

/////////////////////////////////////////////////////////////////////////////

void CInspector::Test_Command()
{
}
