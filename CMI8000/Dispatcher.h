// Dispatcher.h : 헤더 파일
//
#pragma once

// CDispatcher

class CDispatcher : public CWnd
{
	DECLARE_DYNAMIC(CDispatcher)

public:
	CDispatcher();
	virtual ~CDispatcher();

protected:
	DECLARE_MESSAGE_MAP()
	afx_msg LRESULT OnUdpReceive(WPARAM wLocalPort, LPARAM lParam);

private:
	CUdpSocketCS	m_UdpDispatcher;

	CString		m_strRecvCmd;

	BOOL	m_bOpened;
	BOOL	m_bConnected;
	int		m_nStatusDispatcher;
	
	BOOL	m_bJudgeReq[2][40][45];	// 판정 요청
	DWORD	m_dwReqStart[2][40][45];	// 요청 시작
	BOOL	m_bJudgeDone[2][40][45];	// 판정 완료

	void Get_ConnectRequest();
	void Get_ConnectReply();
	void Get_ConnectEnd();
	void Get_StatusRequest();
	void Get_StatusReply(CString sStatus);
	void Get_StatusUpdate(CString sStatus);
	void Get_JudgeReply(CString sLotId, CString sPortNo, CString sTrayNo, CString sCmNo, CString sModel, CString sIp, CString sPort, CString sJudge);
	void Get_TimeUpdate(CString sTime);

	void Send_Command(CString strSend);

public:
	BOOL Initialize();
	void Terminate();
	void Reset();
	void PortReset(int PNo);

	void Set_ConnectRequest();
	void Set_ConnectReply();
	void Set_ConnectEnd();
	void Set_StatusRequest();
	void Set_StatusReply(int nStatus);
	void Set_StatusUpdate(int nStatus);
	void Set_LotStart(int nPortNo);
	void Set_LotEnd(int nPortNo);
	void Set_JudgeRequest(int nPortNo, int nTrayNo, int nCmNo, CString sJudge);
	void Set_InspectRos(CString sCmd);
	void Set_TimeUpdate();

	BOOL Is_Opened() { return m_bOpened; }
	BOOL Is_JudgeDone(int nPort, int nTrayNo, int nCmNo);

	void Test_Command(int nNo);
};

extern CDispatcher g_objDispatcher;

///////////////////////////////////////////////////////////////////////////////
