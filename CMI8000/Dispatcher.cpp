// Dispatcher.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CMI8000.h"
#include "Dispatcher.h"

#include "LogFile.h"
#include "Common.h"
#include "SequenceMain.h"

#define UDP_DISPATCHER_LPORT	8020
#define UDP_DISPATCHER_HPORT	8020

IMPLEMENT_DYNAMIC(CDispatcher, CWnd)

CDispatcher g_objDispatcher;

CCriticalSection g_csDispatcher;	// Send_Command 문제 해결하기 위함

// CDispatcher

CDispatcher::CDispatcher()
{
	m_strRecvCmd = "";
	m_bOpened = FALSE;
	m_bConnected = FALSE;
	m_nStatusDispatcher = 0;	// 상태 (0:Not Ready, 1:Ready)
	Reset();
}

CDispatcher::~CDispatcher()
{
}

BEGIN_MESSAGE_MAP(CDispatcher, CWnd)
	ON_MESSAGE(UM_UDP_RECEIVE, OnUdpReceive)
END_MESSAGE_MAP()

// CDispatcherer 메시지 처리기입니다.

BOOL CDispatcher::Initialize()
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (!pEquipData->bUseDispatcher) return FALSE;
	if (m_bOpened) return TRUE;

	ROS_DATA *pRosData = g_objDataManager.Get_pRosData();
	m_bOpened = m_UdpDispatcher.Open_Socket(UDP_DISPATCHER_LPORT, UDP_DISPATCHER_HPORT, pRosData->sDispatcherIp, this);
	if (!m_bOpened) return FALSE;
		
	Set_ConnectRequest();
	Reset();	// Clear

	CString strSpecialNg = (pRosData->bJudgeSpecialNg ? "TRUE" : "FALSE");
	CString strLog;
	strLog.Format("Initialize : IP(%s), TimeOver(%d msec), SpecialNg(%s)", pRosData->sDispatcherIp, pRosData->nJudgeTimeOver, strSpecialNg);
	g_objLogFile.Save_DispatcherLog(strLog);

	return TRUE;
}

void CDispatcher::Terminate()
{
	Set_ConnectEnd();
	m_bOpened = FALSE;
	m_UdpDispatcher.Close_Socket();
	g_objLogFile.Save_DispatcherLog("Terminate");
}

void CDispatcher::Reset()
{
	memset(m_bJudgeReq, 0x00, sizeof(BOOL) * 2 * 40 * 45);
	memset(m_dwReqStart, 0x00, sizeof(DWORD) * 2 * 40 * 45);
	memset(m_bJudgeDone, 0x00, sizeof(BOOL) * 2 * 40 * 45);
}

void CDispatcher::PortReset(int nPNo)
{
	memset(m_bJudgeReq[nPNo], 0x00, sizeof(BOOL) * 40 * 45);
	memset(m_dwReqStart[nPNo], 0x00, sizeof(DWORD) * 40 * 45);
	memset(m_bJudgeDone[nPNo], 0x00, sizeof(BOOL) * 40 * 45);
}

///////////////////////////////////////////////////////////////////////////////
// UDP Socket Message

LRESULT CDispatcher::OnUdpReceive(WPARAM wLocalPort, LPARAM lParam)
{
	UINT nPort = (UINT)wLocalPort;
	BYTE byRecv[1024] = { 0 };
	CString strLog;

	int nLen = m_UdpDispatcher.Read_Socket(byRecv);

	if (nLen < 1) {
		strLog.Format("[H<-J] : Local Port (%d) Mismatch or Receive Data Zero (%d)", nPort, nLen);
		g_objLogFile.Save_DispatcherLog(strLog);
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
			strLog.Format("[H<-J] : <<Error>> %s : Start(%d), End(%d)", m_strRecvCmd, nStart, nEnd);
			g_objLogFile.Save_DispatcherLog(strLog);
			m_strRecvCmd.Delete(0, nEnd + 1);	// 쓰레기값이 채워져 있어서...
			continue;
		}

		CString strRecv = m_strRecvCmd.Mid(nStart + 1, nEnd - nStart - 1);
		m_strRecvCmd.Delete(0, nEnd + 1);

		EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
		if (!pEquipData->bUseDispatcher) return 0;

		char chSep = ',';
		CString strCmd, strOp;

		AfxExtractSubString(strCmd, strRecv, 0, chSep);
		AfxExtractSubString(strOp, strRecv, 1, chSep);

		// Dispatcher Log /////////////////////////////////////////////////////
		strLog.Format("[H<-J] : %s", strRecv);
		g_objLogFile.Save_DispatcherLog(strLog);
		///////////////////////////////////////////////////////////////////////

		CString strArg[8];
		for (int i = 0; i < 8; i++) AfxExtractSubString(strArg[i], strRecv, i + 2, chSep);

		if (strCmd == "CONNECT") {
			if (strOp == "REQUEST")	Get_ConnectRequest();
			else if (strOp == "REPLY") Get_ConnectReply();
			else if (strOp == "END") Get_ConnectEnd();

		} else if (strCmd == "STATUS") {
			if (strOp == "REQUEST")	Get_StatusRequest();
			else if (strOp == "REPLY") Get_StatusReply(strArg[0]);
			else if (strOp == "UPDATE") Get_StatusUpdate(strArg[0]);

		} else if (strCmd == "JUDGE") {
			if (strOp == "REPLY") Get_JudgeReply(strArg[0], strArg[1], strArg[2], strArg[3], strArg[4], strArg[5], strArg[6], strArg[7]);

		} else if (strCmd == "TIME") {
			if (strOp == "UPDATE") Get_TimeUpdate(strArg[0]);
		}
	}

	return 1;
}

///////////////////////////////////////////////////////////////////////////////
// Get Command

void CDispatcher::Get_ConnectRequest()
{
	m_bConnected = TRUE;
	Set_ConnectReply();
}

void CDispatcher::Get_ConnectReply()
{
	m_bConnected = TRUE;
}

void CDispatcher::Get_ConnectEnd()
{
	m_bConnected = FALSE;
}

void CDispatcher::Get_StatusRequest()
{
	BOOL bStatus = g_objSequenceMain.Is_MainThreadRun();
	if (bStatus) Set_StatusReply(1);
	else Set_StatusReply(0);
}

void CDispatcher::Get_StatusReply(CString sStatus)
{
	m_nStatusDispatcher = atoi(sStatus);
}

void CDispatcher::Get_StatusUpdate(CString sStatus)
{
	m_nStatusDispatcher = atoi(sStatus);
}

void CDispatcher::Get_JudgeReply(CString sLotId, CString sPortNo, CString sTrayNo, CString sCmNo, CString sModel, CString sIp, CString sPort, CString sJudge)
{
	CString strLog;
	int	nTNo = atoi(sTrayNo) - 1;
	int	nCNo = atoi(sCmNo) - 1;
	int nPNo = atoi(sPortNo) - 1;

	if (sLotId != gData.sLotID[nPNo] || !m_bJudgeReq[nPNo][nTNo][nCNo] || nTNo < 0 || nTNo > 39 || nCNo < 0 || nCNo > 44) {
		strLog.Format("[Get_JudgeReply] Receive Data Error ==> LotId(%s),JudgeReq(%d),TrayNo(%d),CmNo(%d),Judge(%s)", sLotId, (int)m_bJudgeReq, nTNo+1, nCNo+1, sJudge);
		g_objLogFile.Save_DispatcherLog(strLog);
		return;
	}
	
	if (((gData.byInspectDone[nPNo][nTNo][nCNo] >> 7) & 1) == 1) {
		strLog.Format("[Get_JudgeReply] Already been judged,LotId(%s),TrayNo(%d),CmNo(%d),Judge(%s)", sLotId, nTNo+1, nCNo+1, sJudge);
		g_objLogFile.Save_DispatcherLog(strLog);
		return;
	}

	// 전에 처리했던 데이터 삭제
	if (m_bJudgeDone[nPNo][nTNo][nCNo]) {
		if (gData.nInspectInfo[nPNo][nTNo][nCNo] == 1) gLot.nRosGood[nPNo]--;		// Good
		if (gData.nInspectInfo[nPNo][nTNo][nCNo] == 2) gLot.nRosNg[nPNo]--;			// ROS NG
		if (gData.nInspectInfo[nPNo][nTNo][nCNo] == 4 || gData.nInspectInfo[nPNo][nTNo][nCNo] == 5) gLot.nRosRepair[nPNo]--;	// Normal NG
	}

	if (sJudge == "G1" || sJudge == "G2") { gData.nInspectInfo[nPNo][nTNo][nCNo] = 1; gLot.nRosGood[nPNo]++; }		// Good
	if (sJudge == "N1" || sJudge == "N2") { gData.nInspectInfo[nPNo][nTNo][nCNo] = 2; gLot.nRosNg[nPNo]++; }		// ROS NG
	if (sJudge == "R1")					  { gData.nInspectInfo[nPNo][nTNo][nCNo] = 4; gLot.nRosRepair[nPNo]++; }	// Normal NG
	if (sJudge == "R2")					  { gData.nInspectInfo[nPNo][nTNo][nCNo] = 5; gLot.nRosRepair[nPNo]++; }	// Normal NG

	m_bJudgeDone[nPNo][nTNo][nCNo] = TRUE;
}

void CDispatcher::Get_TimeUpdate(CString sTime)
{
	// sTime Format : "2000-01-01 12:30:30 000"
	CString strTemp;
	SYSTEMTIME time;

	strTemp = sTime.Mid(0, 4);  time.wYear = atoi(strTemp);			// Year
	strTemp = sTime.Mid(5, 2);  time.wMonth = atoi(strTemp);		// Month
	strTemp = sTime.Mid(8, 2);  time.wDay = atoi(strTemp);			// Day
	strTemp = sTime.Mid(11, 2); time.wHour = atoi(strTemp);			// Hour
	strTemp = sTime.Mid(14, 2); time.wMinute = atoi(strTemp);		// Minite
	strTemp = sTime.Mid(17, 2); time.wSecond = atoi(strTemp);		// Second
	strTemp = sTime.Mid(20, 3); time.wMilliseconds = atoi(strTemp);	// MilliSecond

	SetLocalTime(&time);
}

/////////////////////////////////////////////////////////////////////////////
// Set Command

void CDispatcher::Set_ConnectRequest()
{
	CString	strSendCmd;
	ROS_DATA *pRosData = g_objDataManager.Get_pRosData();

	strSendCmd.Format("CONNECT,REQUEST,%s,%d", pRosData->sHandlerIp, UDP_DISPATCHER_LPORT);
	Send_Command(strSendCmd);
}

void CDispatcher::Set_ConnectReply()
{
	CString	strSendCmd;
	ROS_DATA *pRosData = g_objDataManager.Get_pRosData();

	strSendCmd.Format("CONNECT,REPLY,%s,%d", pRosData->sHandlerIp, UDP_DISPATCHER_LPORT);
	Send_Command(strSendCmd);
}

void CDispatcher::Set_ConnectEnd()
{
	CString	strSendCmd;
	ROS_DATA *pRosData = g_objDataManager.Get_pRosData();

	strSendCmd.Format("CONNECT,END,%s,%d", pRosData->sHandlerIp, UDP_DISPATCHER_LPORT);
	Send_Command(strSendCmd);
	m_bConnected = FALSE;
}

void CDispatcher::Set_StatusRequest()
{
	m_nStatusDispatcher = 0;
	CString	strSendCmd;
	ROS_DATA *pRosData = g_objDataManager.Get_pRosData();

	strSendCmd.Format("STATUS,REQUEST,%s,%d", pRosData->sHandlerIp, UDP_DISPATCHER_LPORT);
	Send_Command(strSendCmd);
}

void CDispatcher::Set_StatusReply(int nStatus)
{
	CString	strSendCmd;
	ROS_DATA *pRosData = g_objDataManager.Get_pRosData();

	strSendCmd.Format("STATUS,REPLY,%d,%s,%d", nStatus, pRosData->sHandlerIp, UDP_DISPATCHER_LPORT);
	Send_Command(strSendCmd);
}

void CDispatcher::Set_StatusUpdate(int nStatus)
{
	CString	strSendCmd;
	ROS_DATA *pRosData = g_objDataManager.Get_pRosData();

	strSendCmd.Format("STATUS,UPDATE,%d,%s,%d", nStatus, pRosData->sHandlerIp, UDP_DISPATCHER_LPORT);
	Send_Command(strSendCmd);
}

void CDispatcher::Set_LotStart(int nPortNo)
{
	CString	strSendCmd;
	ROS_DATA *pRosData = g_objDataManager.Get_pRosData();

	CString strLotId = gData.sLotID[nPortNo-1];
	int nTrayUseCnt = gData.nTrayUseCount[nPortNo-1];
	int nCmUseCnt = gData.nCmUseCount[nPortNo-1];

	strSendCmd.Format("LOT,START,%s,%d,%d,%s,%d,%d,%s", strLotId, nTrayUseCnt, nCmUseCnt, pRosData->sHandlerIp, UDP_DISPATCHER_LPORT, nPortNo, gData.sRecipeName);
	Send_Command(strSendCmd);
}

void CDispatcher::Set_LotEnd(int nPortNo)
{
	CString	strSendCmd;
	ROS_DATA *pRosData = g_objDataManager.Get_pRosData();

	CString strLotId = gData.sLotID[nPortNo-1];

	strSendCmd.Format("LOT,END,%s,%s,%d,%d,%s", strLotId, pRosData->sHandlerIp, UDP_DISPATCHER_LPORT, nPortNo, gData.sRecipeName);
	Send_Command(strSendCmd);
}

void CDispatcher::Set_JudgeRequest(int nPortNo, int nTrayNo, int nCmNo, CString sJudge)
{
	CString	strSendCmd;
	ROS_DATA *pRosData = g_objDataManager.Get_pRosData();
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	CString strLotId = gData.sLotID[nPortNo-1];
	CString strBarcode = gMes.sBarID[nPortNo-1][nTrayNo-1][nCmNo-1];
	CString strNgCode = gMes.sNGCode[nPortNo-1][nTrayNo-1][nCmNo-1];

	//theApp.uSleep(100);		//이헌준SE요청사항 ==> 제거(KCS,20201012)

	strSendCmd.Format("JUDGE,REQUEST,%s,%d,%d,%d,%s,%s,%s,%d,%s,%s",
		strLotId, nPortNo, nTrayNo, nCmNo, sJudge, gData.sRecipeName, pRosData->sHandlerIp, UDP_DISPATCHER_LPORT, strBarcode, strNgCode);
	Send_Command(strSendCmd);
	
	gLot.nRosRequest[nPortNo-1]++;
	m_bJudgeReq[nPortNo-1][nTrayNo-1][nCmNo-1] = TRUE;
	m_dwReqStart[nPortNo-1][nTrayNo-1][nCmNo-1] = GetTickCount();
}

void CDispatcher::Set_InspectRos(CString sCmd)
{
	CString	strSendCmd;
	ROS_DATA *pRosData = g_objDataManager.Get_pRosData();

	strSendCmd.Format("%s,%s,%s,%d", sCmd, gData.sRecipeName, pRosData->sHandlerIp, UDP_DISPATCHER_LPORT);
	Send_Command(strSendCmd);
}

void CDispatcher::Set_TimeUpdate()
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strTime;
	strTime.Format("%04d-%02d-%02d %02d:%02d:%02d %03d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

	CString	strSendCmd;
	ROS_DATA *pRosData = g_objDataManager.Get_pRosData();

	strSendCmd.Format("TIME,UPDATE,%s,%s,%d", strTime, pRosData->sHandlerIp, UDP_DISPATCHER_LPORT);
	Send_Command(strSendCmd);
}

/////////////////////////////////////////////////////////////////////////////
// UDP Socket Send Message

void CDispatcher::Send_Command(CString strSend)
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	if (!pEquipData->bUseDispatcher) return;

	CString strLog;
	strLog.Format("[H->J] : %s", strSend);
	g_objLogFile.Save_DispatcherLog(strLog);

	CString strSendSocket;
	strSendSocket.Format("@%s\n", strSend);

	char chSend[1024] = { 0 };
	int nLength = strSendSocket.GetLength();
	memcpy(chSend, (LPSTR)(LPCSTR)strSendSocket, nLength);

	g_csDispatcher.Lock();	// Critical Section

	m_UdpDispatcher.Write_Socket((BYTE*)chSend, nLength);

	g_csDispatcher.Unlock();	// Critical Section
}

///////////////////////////////////////////////////////////////////////////////

BOOL CDispatcher::Is_JudgeDone(int nPortNo, int nTrayNo, int nCmNo)
{
	if (!m_bJudgeReq[nPortNo-1][nTrayNo-1][nCmNo-1]) return TRUE;
	if (m_bJudgeDone[nPortNo-1][nTrayNo-1][nCmNo-1]) return TRUE;

	ROS_DATA *pRosData = g_objDataManager.Get_pRosData();
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	int nTerm = (int)(GetTickCount() - m_dwReqStart[nPortNo-1][nTrayNo-1][nCmNo-1]);

	if (nTerm > pRosData->nJudgeTimeOver) {		// Time Over
		CString strLog;
		strLog.Format("Judge Time Over (%d msec), PortNo (%d),TrayNo (%d), CmNo (%d)", pRosData->nJudgeTimeOver, nPortNo, nTrayNo, nCmNo);
		g_objLogFile.Save_DispatcherLog(strLog);
		m_bJudgeDone[nPortNo-1][nTrayNo-1][nCmNo-1] = TRUE;
		gLot.nRosTimeOut[nPortNo-1]++;
		return TRUE;

	} else if (pEquipData->bUseRosSkip) {	// Sort Picker 에서 확인할때 ROS 에서 판정이 안왔으면 스킵한다.
		CString strLog;
		strLog.Format("Judge Time Over Sort Picker (%d msec), PortNo (%d),TrayNo (%d), CmNo (%d)", pRosData->nJudgeTimeOver, nPortNo, nTrayNo, nCmNo);
		g_objLogFile.Save_DispatcherLog(strLog);
		m_bJudgeDone[nPortNo-1][nTrayNo-1][nCmNo-1] = TRUE;
		gLot.nRosTimeOut[nPortNo-1]++;
		return TRUE;
	}
	return FALSE;
}

/////////////////////////////////////////////////////////////////////////////

void CDispatcher::Test_Command(int nNo)
{
}
