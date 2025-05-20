// LogFile.h : 헤더 파일
//
#pragma once

class CLogFile  
{
public:
	CLogFile();
	virtual ~CLogFile();

public:
	void Create_Folder(CString sPath);
	void MakeFolder(CString sPath);

	void Save_AlarmLog(CString sLog);
	void Save_AlarmResetLog(CString sLog);
	void Save_HandlerLog(CString sLog);
	void Save_SaveRunTimeLog(CString sLog);
	void Save_InspectorLog(CString sLog);
	void Save_AlignerLog(CString sLog);
	void Save_JobListLog(CString sLog, BOOL bMode=FALSE);
	void Save_LotResult(int nPNo, CString sLog);
	void Save_LotLog(int nPNo);
	void Save_MesAgentLog(CString sLog);
	void Save_DispatcherLog(CString sLog);
	void Save_LotTime(int nPNo, CString sLog);
	void Save_LotError(CString sLog, int nPNo=0);
	void Save_ECMLog(int nType, CString strLog);	//nType:1[Alarm], 2[Joblist] 3[Inspect]
	void Save_MesNgLog(CString sLog);
	void Save_TestLog(CString sLog);
	void Save_OperatingRatio(CString sLog);
	void Save_FocusAxisLog(int Axis, CString sLog);
	void Save_RosWaitLog(int nPNo, CString sLog);
	void Save_InspectWaitLog(int nPNo, CString sLog);

	void Save_JobListExcel(CString sLog);
	void Save_AverageCycle(int nPNo);
	CString Job_Msg(int nFun, int nId);

	void Save_DailyLot(CString sLog);

	void Save_Print(int PickerNo, int LoadTrayNo, int LoadCmNo, int NGTrayNo, int NGCmX, int NGCmY);
	void Save_PrintDownLog();
	CString Get_NGData(int nPort, int nTray, int nCm);
	void Save_OutTray(CString strOut, int nTrayCount, int nPosX, int nPosY, int nPortNo, int nTrayNo, int nCmNo);
	void Save_ECMOutTray(CString sLog, int nTrayCount, int nPosX, int nPosY, int nPortNo, int nTrayNo, int nCmNo);
};

extern CLogFile g_objLogFile;

///////////////////////////////////////////////////////////////////////////////
