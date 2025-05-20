// LogFile.cpp: implementation of the CLogFile class.
//
#include "stdafx.h"
#include "LogFile.h"
#include "AJinDefine.h"

// CLogFile
CLogFile g_objLogFile;

CCriticalSection g_csAlarmLog;
CCriticalSection g_csAlarmResetLog;
CCriticalSection g_csHandlerLog;
CCriticalSection g_csRunTimeLog;
CCriticalSection g_csInspectorLog;
CCriticalSection g_csJobListLog;
CCriticalSection g_csAlignerLog;
CCriticalSection g_csMesAgentLog;
CCriticalSection g_csDispatcherLog;
CCriticalSection g_csDailyLotLog;
CCriticalSection g_csRosWaitLog;
CCriticalSection g_csInspectWaitLog;
CCriticalSection g_csECMLog;

CLogFile::CLogFile()
{
}

CLogFile::~CLogFile()
{
}

///////////////////////////////////////////////////////////////////////////////

void CLogFile::Create_Folder(CString sPath)
{
	if (sPath == _T("")) return;
	if (sPath.Right(1) == _T("\\")) sPath = sPath.Left(sPath.GetLength() - 1);
	if (GetFileAttributes(sPath) != -1) return;	// Directory Exist!!!

	int nFound = sPath.ReverseFind('\\');
	Create_Folder(sPath.Left(nFound));

	CreateDirectory(sPath, NULL);
}

void CLogFile::MakeFolder(CString sPath)	//"D:\\Run\\Log\\LotData\\YYYY\\MM\\DD\\";
{
	CString sTemp;
	int nIdx = 0;

	while(true) {
		nIdx = sPath.Find("\\");
		sTemp += sPath.Mid(0, nIdx) + "\\";
		CreateDirectory(sTemp,FALSE);
		sPath = sPath.Mid(nIdx+1, sPath.GetLength());
		if (nIdx<0) break;
	}
}

///////////////////////////////////////////////////////////////////////////////

void CLogFile::Save_AlarmLog(CString sLog)
{
	g_csAlarmLog.Lock();

	CString strPath = gsCurrentDir + "\\LOG\\Alarm";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, strSave;
	strFile.Format("%s\\%04d%02d%02d.txt", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			strSave.Format("%02d:%02d:%02d %03d,%s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);	// , 추가

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csAlarmLog.Unlock();
}

void CLogFile::Save_AlarmResetLog(CString sLog)
{
	g_csAlarmResetLog.Lock();

	CString strPath = gsCurrentDir + "\\LOG\\AlarmReset";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, strSave;
	strFile.Format("%s\\%04d%02d%02d.txt", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			strSave.Format("%02d:%02d:%02d %03d,%s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);	// , 추가

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csAlarmResetLog.Unlock();
}

void CLogFile::Save_HandlerLog(CString sLog)
{
	g_csHandlerLog.Lock();

	CString strPath = gsCurrentDir + "\\LOG\\Handler";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, strSave;
	strFile.Format("%s\\%04d%02d%02d.txt", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			strSave.Format("[%02d:%02d:%02d.%03d], %s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csHandlerLog.Unlock();

	Save_ECMLog(4, sLog);
}

void CLogFile::Save_SaveRunTimeLog(CString sLog)
{
	g_csRunTimeLog.Lock();

	CString strPath = gsCurrentDir + "\\LOG\\RunTime";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, strSave;
	strFile.Format("%s\\%04d%02d%02d.txt", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			strSave.Format("[%02d:%02d:%02d.%03d], %s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csRunTimeLog.Unlock();
}

void CLogFile::Save_InspectorLog(CString sLog)
{
	g_csInspectorLog.Lock();

	CString strPath = gsCurrentDir + "\\LOG\\Inspector";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, strSave;
	strFile.Format("%s\\%04d%02d%02d.txt", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			strSave.Format("[%02d:%02d:%02d.%03d],%s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csInspectorLog.Unlock();
}

void CLogFile::Save_AlignerLog(CString sLog)
{
	g_csAlignerLog.Lock();

	CString strPath = gsCurrentDir + "\\LOG\\Aligner";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, strSave;
	strFile.Format("%s\\%04d%02d%02d.txt", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			strSave.Format("[%02d:%02d:%02d %03d] %s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csAlignerLog.Unlock();
}

void CLogFile::Save_JobListLog(CString sLog, BOOL bMode)
{
	g_csJobListLog.Lock();

	CString strPath = gsCurrentDir + "\\LOG\\JobList";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString sTitle, strFile, strSave;
	sTitle.Format("Time,lotNum,Start_Time,End_Time,Run_Time,Tray_Count,CM_Count,Tack,Good_Count,NG_Count,NG1_Count,NG2_Count,MESNG_Count\r\n");
	strFile.Format("%s\\%04d%02d%02d.txt", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();
			if (file.GetLength() < 1) file.Write(sTitle, sTitle.GetLength());

			strSave.Format("[%02d:%02d:%02d.%03d], %s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csJobListLog.Unlock();

	if (bMode == TRUE) Save_ECMLog(2, sLog);
}

void CLogFile::Save_LotResult(int nPNo, CString sLog)
{
	CString strPath1 = "D:\\EVMS\\TP\\Log";
	CString strPath2 = "D:\\EVMS\\TP\\Backup";
	CString strPath3;

	Create_Folder(strPath1);
	Create_Folder(strPath2);

	CTime tNow = CTime::GetCurrentTime();
	CTime tLog = tNow - CTimeSpan(7 * 60 * 60);

	strPath3.Format("%s\\LOG\\SPC\\%04d\\%02d\\%02d\\", gsCurrentDir, tLog.GetYear(), tLog.GetMonth(), tLog.GetDay());
	MakeFolder(strPath3);

	CString strFile1, strFile2, strFile3, strTitle, strDate, strSave;
	strFile1.Format("%s\\%s_%04d%02d%02d07_Lot_Result.csv", strPath1, gData.sLotID[nPNo], tLog.GetYear(), tLog.GetMonth(), tLog.GetDay());
	strFile2.Format("%s\\%s_%04d%02d%02d07_Lot_Result.csv", strPath2, gData.sLotID[nPNo], tLog.GetYear(), tLog.GetMonth(), tLog.GetDay());
	strFile3.Format("%s%s_Lot_Result.csv", strPath3, gData.sLotID[nPNo]);

	CFile file;
	if (!file.Open(strFile1, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) return;

	strTitle.Format("날짜,Lot Start,Lot End,Term,호기,Lot ID,투입수,양품수,불량수,불량률,1차외관(N1),2차외관(N2),MES(M)\r\n");

	try {
		file.SeekToEnd();

		if (file.GetLength() < 1) file.Write(strTitle, strTitle.GetLength());

		strDate.Format("%02d/%02d", tNow.GetMonth(), tNow.GetDay());

		strSave.Format("%s,%s\r\n", strDate, sLog);

		file.Write(strSave, strSave.GetLength());
		file.Close();

		CopyFile(strFile1, strFile2, FALSE);	// Backup
		CopyFile(strFile1, strFile3, FALSE);	// SPC

	} catch (CFileException *pEx) {
		pEx->Delete();
	}
}

void CLogFile::Save_MesAgentLog(CString sLog)
{
	g_csMesAgentLog.Lock();

	CString strPath = gsCurrentDir + "\\LOG\\MESAgent";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, strSave;
	strFile.Format("%s\\%04d%02d%02d.txt", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			strSave.Format("[%02d:%02d:%02d %03d], %s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csMesAgentLog.Unlock();
}

void CLogFile::Save_DispatcherLog(CString sLog)
{
	g_csDispatcherLog.Lock();

	CString strPath = gsCurrentDir + "\\LOG\\Dispatcher";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, strSave;
	strFile.Format("%s\\%04d%02d%02d.txt", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			strSave.Format("[%02d:%02d:%02d %03d] %s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csDispatcherLog.Unlock();
}

void CLogFile::Save_LotTime(int nPNo, CString sLog)
{
	CString strPath1 = "D:\\EVMS\\TP\\Log";
	CString strPath2 = "D:\\EVMS\\TP\\Backup";
	CString strPath3;

	Create_Folder(strPath1);
	Create_Folder(strPath2);

	SYSTEMTIME time;
	GetLocalTime(&time);

	strPath3.Format("%s\\LOG\\SPC\\%04d\\%02d\\%02d\\", gsCurrentDir, time.wYear, time.wMonth, time.wDay);
	MakeFolder(strPath3);

	CString strFile1, strFile2, strFile3, strTitle, strTime, strPcName, strSave;
	strFile1.Format("%s\\%s_%04d%02d%02d%02d_LOT_TIME.csv", strPath1, gData.sLotID[nPNo-1], time.wYear, time.wMonth, time.wDay, time.wHour);
	strFile2.Format("%s\\%s_%04d%02d%02d%02d_LOT_TIME.csv", strPath2, gData.sLotID[nPNo-1], time.wYear, time.wMonth, time.wDay, time.wHour);
	strFile3.Format("%s%s_LOT_TIME.csv", strPath3, gData.sLotID[nPNo-1]);

	CFile file;
	if (!file.Open(strFile1, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) return;

	//m_pEquipData->sEquipName, MAIN_VERSION, gData.sLotID[nPNo-1], nTrayNo, nCmNo, gMes.sBarID[nPNo-1][nTrayNo-1][nCmNo-1], strCode, strResult
	strTitle.Format("Time,Station,Machine,Version,LotID,TrayNo,CmNo,Barcode,AG,B1,T1,T2,B2,Result\r\n");

	try {
		file.SeekToEnd();

		if (file.GetLength() < 1) file.Write(strTitle, strTitle.GetLength());

		strTime.Format("%04d-%02d-%02d %02d:%02d:%02d:%03d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

		char szPcName[MAX_COMPUTERNAME_LENGTH + 1];
		DWORD dwNameSize = MAX_COMPUTERNAME_LENGTH + 1;
		GetComputerName(szPcName, &dwNameSize);

		strSave.Format("%s,%s,%s\r\n", strTime, szPcName, sLog);

		file.Write(strSave, strSave.GetLength());
		file.Close();

		CopyFile(strFile1, strFile2, FALSE);	// Backup
		CopyFile(strFile1, strFile3, FALSE);	// SPC

	} catch (CFileException *pEx) {
		pEx->Delete();
	}
}

void CLogFile::Save_LotError(CString sLog, int nPNo)
{
	CString strPath1 = "D:\\EVMS\\TP\\Log";
	CString strPath2 = "D:\\EVMS\\TP\\Backup";
	CString strPath3;

	Create_Folder(strPath1);
	Create_Folder(strPath2);

	SYSTEMTIME time;
	GetLocalTime(&time);

	strPath3.Format("%s\\LOG\\SPC\\%04d\\%02d\\%02d\\", gsCurrentDir, time.wYear, time.wMonth, time.wDay);
	MakeFolder(strPath3);

	int nNo = nPNo-1;
	if (nNo < 0) {
		nNo = gData.nULPNo-1;
		if (nNo < 0) nNo = gData.nLPNo-1;
		if (nNo < 0) nNo = 0;
	}

	CString strFile1, strFile2, strFile3, strTitle, strDateTime, strPcName, strSave;
	strFile1.Format("%s\\%s_%04d%02d%02d%02d_AVIError.csv", strPath1, gData.sLotID[nNo], time.wYear, time.wMonth, time.wDay, time.wHour);
	strFile2.Format("%s\\%s_%04d%02d%02d%02d_AVIError.csv", strPath2, gData.sLotID[nNo], time.wYear, time.wMonth, time.wDay, time.wHour);
	strFile3.Format("%s%s_AVIError.csv", strPath3, gData.sLotID[nNo]);

	CFile file;
	if (!file.Open(strFile1, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) return;

	strTitle.Format("Time,Station,Model,Version,LotNum,Event,Error Code,event,ActionTime\r\n");

	try {
		file.SeekToEnd();

		if (file.GetLength() < 1) file.Write(strTitle, strTitle.GetLength());

		strDateTime.Format("%04d-%02d-%02d %02d:%02d:%02d:%03d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond, time.wMilliseconds);

		char szPcName[MAX_COMPUTERNAME_LENGTH + 1];
		DWORD dwNameSize = MAX_COMPUTERNAME_LENGTH + 1;
		GetComputerName(szPcName, &dwNameSize);

		strSave.Format("%s,%s,%s\r\n", strDateTime, szPcName, sLog);

		file.Write(strSave, strSave.GetLength());
		file.Close();

		CopyFile(strFile1, strFile2, FALSE);	// Backup
		CopyFile(strFile1, strFile3, FALSE);	// SPC

	} catch (CFileException *pEx) {
		pEx->Delete();
	}
}

void CLogFile::Save_ECMLog(int nType, CString strLog)	//nType:1[Alarm], 2[Joblist] 3[Inspect]
{
	g_csECMLog.Lock();

	CString strFile, sTitle, strTime, strSave;

	CString strPath = "D:\\EVMS\\TP\\Log\\";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);
	strTime.Format("%04d-%02d-%02d %02d:%02d:%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);

	int nNo = gData.nULPNo-1;
	if (nNo < 0) nNo = gData.nLPNo-1;
	if (nNo < 0) nNo = 0;

	if (nType == 1) sTitle.Format("Time,Station,Type,lotNum,Error Code,Error,Start_Time,End_Time,Lead_Time\r\n");
	if (nType == 2) sTitle.Format("Time,Station,Type,lotNum,Start_Time,End_Time,Tack_Time,Tray_Count,CM_Count,Tack,Good_Count,NG_Count,N1_Count,N2_Count,MESNG_Count\r\n");
	if (nType == 3) sTitle.Format("Time,Station,Type,lotNum,Load_Pick,Inspect,Barcode,NG_Pick,Good_Pick,Trans_Pick\r\n");
	if (nType == 4) sTitle.Format("Time,Station,Type\r\n");

	if (nType == 1) strFile.Format("%s%s_%04d%02d%02d%02d_Alarm.csv", strPath, gAlm.sLotID, time.wYear, time.wMonth, time.wDay, time.wHour);
	if (nType == 2) strFile.Format("%s%s_%04d%02d%02d%02d_JobList.csv", strPath, gLot.sLotID[nNo], time.wYear, time.wMonth, time.wDay, time.wHour);
	if (nType == 3) strFile.Format("%s%s_%04d%02d%02d%02d_Inspector.csv", strPath, gLot.sLotID[nNo], time.wYear, time.wMonth, time.wDay, time.wHour);
	if (nType == 4) strFile.Format("%s%s_%04d%02d%02d%02d_Handler.csv", strPath, gLot.sLotID[nNo], time.wYear, time.wMonth, time.wDay, time.wHour);


	CFile file;
	if (!file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) return;

	try {
		file.SeekToEnd();

		if (file.GetLength() < 1) file.Write(sTitle, sTitle.GetLength());

		strSave.Format("%s,%s,%s,%s\r\n", strTime, gData.sComName, gData.sRecipeName, strLog);

		file.Write(strSave, strSave.GetLength());
		file.Close();

	} catch (CFileException *pEx) {
		pEx->Delete();
	}

	g_csECMLog.Unlock();
}

void CLogFile::Save_MesNgLog(CString sLog)
{
	CString strPath1 = "D:\\EVMS\\TP\\Log";
	CString strPath2 = "D:\\EVMS\\TP\\Backup";
	CString strPath3 = gsCurrentDir + "\\LOG\\MES_NG";

	Create_Folder(strPath1);
	Create_Folder(strPath2);
	Create_Folder(strPath3);

	SYSTEMTIME time;
	GetLocalTime(&time);

	int nNo = gData.nLPNo-1;
	if (nNo < 0) nNo = 0;
	CString strFile1, strFile2, strFile3, strTitle, strSave;
	strFile1.Format("%s\\%s_%04d%02d%02d%02d_MESNG.csv", strPath1, gData.sLotID[nNo], time.wYear, time.wMonth, time.wDay, time.wHour);
	strFile2.Format("%s\\%s_%04d%02d%02d%02d_MESNG.csv", strPath2, gData.sLotID[nNo], time.wYear, time.wMonth, time.wDay, time.wHour);
	strFile3.Format("%s\\%04d%02d%02d.csv", strPath3, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (!file.Open(strFile1, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) return;

	strTitle.Format("Date,Lot ID,Module ID,MES Judge,NG Code,NG Text\r\n");

	try {
		file.SeekToEnd();

		if (file.GetLength() < 1) file.Write(strTitle, strTitle.GetLength());

		strSave.Format("%02d:%02d:%02d %03d,%s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

		file.Write(strSave, strSave.GetLength());
		file.Close();

		CopyFile(strFile1, strFile2, FALSE);	// Backup

	} catch (CFileException *pEx) {
		pEx->Delete();
	}

	if (!file.Open(strFile3, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) return;

	try{
		file.SeekToEnd();

		if (file.GetLength() < 1) file.Write(strTitle, strTitle.GetLength());

		strSave.Format("%02d:%02d:%02d %03d,%s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

		file.Write(strSave, strSave.GetLength());
		file.Close();
	}
	catch (CFileException *pEx) {
		pEx->Delete();
	}
}

void CLogFile::Save_DailyLot(CString sLog)
{
	g_csDailyLotLog.Lock();

	CString strPath = gsCurrentDir + "\\LOG\\DailyLot";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, strSave, strTitle;
	strFile.Format("%s\\%04d%02d%02d.txt", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			if (file.GetLength() < 1) {
				strTitle.Format("Time \tLot_ID \tTotal \tGood \tNG \tRosRequest \tRosGood \tRosNg \tRosRepair \tRosTimeOut \tFiltering1 \tFiltering2 \tRateAvi \tFinal \tTakt\r\n");
				file.Write(strTitle, strTitle.GetLength());
			}

			strSave.Format("[%02d:%02d:%02d %03d] %s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csDailyLotLog.Unlock();
}


///////////////////////////////////////////////////////////////////////////////

void CLogFile::Save_TestLog(CString sLog)
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strPath = gsCurrentDir + "\\LOG\\TEST";

	Create_Folder(strPath);

	CString strFile, strSave;
	strFile.Format("%s\\%04d%02d%02d.txt", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			strSave.Format("[%02d:%02d:%02d %03d] %s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
}

void CLogFile::Save_LotLog(int nPNo)
{
	CString sFileName, strFile, strCreDate, sCreFile, sTemp, sData, sInsResult, sBarResult;
	SYSTEMTIME time;
	GetLocalTime(&time);
	int nNo = nPNo-1;
	if (nNo < 0) nNo = 0;

	if (gLot.sLotID[nNo] == "") gLot.sLotID[nNo] = "LOT_ID_DEFAULT";
	strCreDate.Format("%04d%02d%02d_%02d%02d%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
	strFile.Format("LOG/LotData/%04d/%02d/%02d/%s_%s.txt", time.wYear, time.wMonth, time.wDay, gLot.sLotID[nNo], strCreDate);
	sFileName = strFile;

	sCreFile.Format("%s\\LOG\\LotData\\%04d\\%02d\\%02d\\", gsCurrentDir, time.wYear, time.wMonth, time.wDay);
	MakeFolder(sCreFile);

	CFile file;
	if (!file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) return;

	sTemp.Format("LotID,%s,Start_Time,%s,End_Time,%s,Tray_Count,%02d,CM_Count,%04d,Tack,%0.7lf,\r\n\r\n", gLot.sLotID[nNo], gLot.sStartTime[nNo], gLot.sEndTime[nNo], gLot.nTrayCount[nNo], gLot.nCmCount[nNo], gLot.dTackTime);
	sData.Format("%sTray_No,Pocket_No,ID,Inspection Result,ID Result,\r\n", sTemp);
	file.Write(sData, sData.GetLength());

	try {
		int	nCmCount = 0;
		for (int i = 0; i < 30; i++) {
			if (nCmCount > gLot.nCmCount[nNo]) break;

			for (int j = 0; j < 40; j++) {
				nCmCount++;
				if (nCmCount > gLot.nCmCount[nNo]) break;

				file.SeekToEnd();

				sData.Format("%d,%d,%s,%s,%s,\r\n", i+1, j+1, gMes.sBarID[nNo][i][j], gMes.sJudge[nNo][i][j], gMes.sNGCode[nNo][i][j]);
				file.Write(sData, sData.GetLength());
			}
		}

		file.Close();

	} catch (CFileException *pEx) {
		pEx->Delete();
	}
}

void CLogFile::Save_PrintDownLog()
{
// 	CString sFileName, strFile, strCreDate, sCreFile, sTemp, sData, sInsResult, sBarResult, strText;
// 	SYSTEMTIME time;
// 	GetLocalTime(&time);
// 	int nNo = gData.nLPNo-1;
// 	if (nNo < 0) nNo = 0;
// 
// 	strCreDate.Format("%04d%02d%02d_%02d%02d%02d", time.wYear, time.wMonth, time.wDay, time.wHour, time.wMinute, time.wSecond);
// 	strFile.Format("LOG/Print/%04d/%02d/%02d/%s_%s.txt", time.wYear, time.wMonth, time.wDay, gLot.sLotID[nNo], strCreDate);
// 	sFileName = strFile;
// 
// 	sCreFile.Format("%s\\LOG\\Print\\%04d\\%02d\\%02d\\", gsCurrentDir, time.wYear, time.wMonth, time.wDay);
// 	MakeFolder(sCreFile);
// 
// 	CFile file;
// 	if (!file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) return;
// 
// 	sData.Format("%s\r\n", gLot.sLotID[nNo]);
// 	file.Write(sData, sData.GetLength());
// 
// 	try {
// 		int nX, nY;
// 		int	nCmCount = 0;
// 		for (int p = 0; p < 2; p++) {
// 			for (int i = 0; i < 40; i++) {
// 				if (nCmCount > gLot.nCmCount[nNo]) break;
// 
// 				for (int j = 0; j < 60; j++) {
// 					nCmCount++;
// 					if (nCmCount > gLot.nCmCount[nNo]) break;
// 
// 					strText = Get_NGData(p, i, j);
// 					if (strText.GetLength() > 1) {
// 						file.SeekToEnd();
// 
// 						nX = (j / 6) + 1;
// 						nY = (j % 6) + 1;
// 						sData.Format("%02d %dx%d %s %s\r\n", i+1, nX, nY, gMes.sBarID[i][j], strText);
// 						file.Write(sData, sData.GetLength());
// 
// 					}
// 				}
// 			}
// 		}
// 		file.Close();
// 
// 	} catch (CFileException *pEx) {
// 		pEx->Delete();
// 	}
}

void CLogFile::Save_Print(int PickerNo, int LoadTrayNo, int LoadCmNo, int NGTrayNo, int NGCmX, int NGCmY)
{
// 	if (PickerNo == 1 || PickerNo == 2) {
// 		if (LoadTrayNo < 1 || LoadTrayNo > 40 || LoadCmNo < 1 || LoadCmNo > 45) return;
// 	}
// 
// 	SYSTEMTIME time;
// 	GetLocalTime(&time);
// 	int nNo = gData.nLPNo-1;
// 	if (nNo < 0) nNo = 0;
// 
// 	if (PickerNo == 0) {	// Lot Start
// 		CString sCreateFile;
// 		sCreateFile.Format("%s\\LOG\\Print\\%04d\\%02d\\%02d\\", gsCurrentDir, time.wYear, time.wMonth, time.wDay);
// 		MakeFolder(sCreateFile);
// 
// 		gData.sNGFileName.Format("%s%s.txt", sCreateFile, gLot.sLotID[nNo]);
// 		DeleteFile(gData.sNGFileName);
// 	}
// 
// 	CFile file;
// 	if (!file.Open(gData.sNGFileName, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) return;
// 
// 	try {
// 		CString strSave, strText, sBarID;
// 
// 		if (PickerNo == 0) {		//Lot Start
// 			strSave.Format("%s\r\n", gLot.sLotID[nNo]);
// 		} else if (PickerNo == 9) {	//Lot End
// 			strSave.Format("Total:%d  Good:%d  NG:%d\r\n", (gLot.nGoodCount[nNo] + gLot.nNgCount[nNo]), gLot.nGoodCount[nNo], gLot.nNgCount[nNo]);
// 		} else {					//NG Down
// 			strText = Get_NGData(nNo, LoadTrayNo-1, LoadCmNo-1);
// 			sBarID = gMes.sBarID[LoadTrayNo-1][LoadCmNo-1];
// 
// 			strSave.Format("%02d %dx%d %s %s\r\n", NGTrayNo, NGCmX, NGCmY, sBarID, strText);
// 		}
// 
// 		file.SeekToEnd();
// 		file.Write(strSave, strSave.GetLength());
// 		file.Close();
// 		
// 	} catch (CFileException *pEx) {
// 		pEx->Delete();
// 	}
}

CString CLogFile::Get_NGData(int nPort, int nTray, int nCm)
{
	CString sData = "";
	for (int i = 0; i < 5; i++) {
		if (gData.sNGData[nPort][nTray][nCm][i].GetLength() > 1) sData = sData + gData.sNGData[nPort][nTray][nCm][i] + " ";
	}
	return sData;
}

void CLogFile::Save_OutTray(CString strOut, int nTrayCount, int nPosX, int nPosY, int nPortNo, int nTrayNo, int nCmNo)
{
	if (nTrayNo < 1 || nTrayNo > 40 || nCmNo < 1 || nCmNo > 45) return;

	SYSTEMTIME time;
	GetLocalTime(&time);

	int nNo = 0;
	nNo = gData.nPNoSortPick[0];
	if (nNo == 0) nNo = gData.nPNoSortPick[1];

	CString strPath, strFile, strTitle, strSave, strJudge;
	strPath.Format("%s\\LOG\\OutTray\\%04d-%02d-%02d", gsCurrentDir, time.wYear, time.wMonth, time.wDay);
	Create_Folder(strPath);
	if (strOut == "NG") {
		switch(nTrayCount) {
			case 0 : strOut = "1st"; break;
			case 1 : strOut = "2nd"; break;
			case 2 : strOut = "1st"; break;
			case 3 : strOut = "2nd"; break;
// 			case 4 : strOut = "1st"; break;
// 			case 5 : strOut = "2nd"; break;
			default : break;
		}
	}

	strFile.Format("%s\\%s_%s.csv", strPath, gLot.sLotID[nNo-1], strOut);	// NG, N1, N2 / Good

	CFile file;
	if (!file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) return;

	strTitle.Format("Count,Position,Barcode,LoadTray,LoadPos,Judge\r\n");

	try {
		file.SeekToEnd();

		if (file.GetLength() < 1) file.Write(strTitle, strTitle.GetLength());

		int nPos = nPosY * gData.nTrayX + nPosX + 1; 

		//검사결과 (0:Empty, 1:Good, 2:Normal, 3:Mes, 4:Barcode, 5:HREN, 6:Spider, 7:HREP, 8:Chipping, 9:Not NG, 10:Ros)
		int nJudge = gData.nInspectInfo[nPortNo-1][nTrayNo-1][nCmNo-1];
		strJudge = (nJudge == 1 ? "G" : (nJudge == 2 ? "N" : (nJudge == 4 ? "N1" : (nJudge == 5 ? "N2" : " "))));
		strSave.Format("%d,%d,%s,%d,%d,%s\r\n", nTrayCount, nPos,gMes.sBarID[nPortNo-1][nTrayNo-1][nCmNo-1], nTrayNo, nCmNo, strJudge);

		file.Write(strSave, strSave.GetLength());
		file.Close();

	} catch (CFileException *pEx) {
		pEx->Delete();
	}
	Save_ECMOutTray(strSave, nTrayCount, nPosX, nPosY, nPortNo, nTrayNo, nCmNo);
}

void CLogFile::Save_ECMOutTray(CString sLog, int nTrayCount, int nPosX, int nPosY, int nPortNo, int nTrayNo, int nCmNo)
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strPath, strFile, strTitle, strSave, strJudge;
	strPath = "D:\\EVMS\\TP\\Log";
	Create_Folder(strPath);

	if (gLot.sLotID[nPortNo-1] == "") gLot.sLotID[nPortNo-1] = "LOT_ID";
	strFile.Format("%s\\%s_%04d%02d%02d%02d_OutTray.csv", strPath, gLot.sLotID[nPortNo-1], time.wYear, time.wMonth, time.wDay, time.wHour);

	int nPos = nPosY * gData.nTrayX + nPosX + 1; 

	//검사결과 (0:Empty, 1:Good, 2:Normal, 3:Mes, 4:N1, 5:N2)
	int nJudge = gData.nInspectInfo[nPortNo-1][nTrayNo-1][nCmNo-1];
	strJudge = (nJudge == 1 ? "G" : (nJudge == 2 ? "N" : (nJudge == 4 ? "N1" : (nJudge == 5 ? "N2" : " "))));
	strSave.Format("%d,%d,%s,%d,%d,%s\r\n", nTrayCount, nPos,gMes.sBarID[nPortNo-1][nTrayNo-1][nCmNo-1], nTrayNo, nCmNo, strJudge);
	strTitle.Format("Count,Position,Barcode,LoadTray,LoadPos,Judge\r\n");

	CFile file;
	if (!file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) return;

	try {
		file.SeekToEnd();

		if (file.GetLength() < 1) file.Write(strTitle, strTitle.GetLength());

		file.Write(strSave, strSave.GetLength());
		file.Close();

	} catch (CFileException *pEx) {
		pEx->Delete();
	}
}

void CLogFile::Save_OperatingRatio(CString sLog)	// 가동률 작업 중
{
	CString strPath = gsCurrentDir + "\\LOG\\OperatingRatio";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, strSave, strTitle;
	strFile.Format("%s\\%04d%02d%02d.csv", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			if (file.GetLength() < 1) {
				strTitle.Format("Time,호기,Lot ID,Lot Start,Lot End,Cycle Time,CM(EA),Run Time,Stop Time,Error Time,Error Count,수율,UPH,MTB\r\n");
				file.Write(strTitle, strTitle.GetLength());
			}

			char szPcName[MAX_COMPUTERNAME_LENGTH + 1];
			DWORD dwNameSize = MAX_COMPUTERNAME_LENGTH + 1;
			GetComputerName(szPcName, &dwNameSize);

			strSave.Format("[%02d:%02d:%02d %03d],%s,%s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, szPcName, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
}

void CLogFile::Save_FocusAxisLog(int Axis, CString sLog)
{
	CString strPath = gsCurrentDir + "\\LOG\\FocusAxis";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, strTitle, strSave;
	if (Axis == AX_VISION_ANGLE_Z) { strFile.Format("%s\\Angle_%04d%02d%02d.csv", strPath, time.wYear, time.wMonth, time.wDay); } 
	if (Axis == AX_BTM1_PICKER_Z)  { strFile.Format("%s\\Bottom1_%04d%02d%02d.csv", strPath, time.wYear, time.wMonth, time.wDay); } 
	if (Axis == AX_VISION_TOP1_Z)  { strFile.Format("%s\\Top1_%04d%02d%02d.csv", strPath, time.wYear, time.wMonth, time.wDay); } 
	if (Axis == AX_VISION_TOP2_Z)  { strFile.Format("%s\\Top2_%04d%02d%02d.csv", strPath, time.wYear, time.wMonth, time.wDay); } 
	if (Axis == AX_BTM2_PICKER_Z)  { strFile.Format("%s\\Bottom2_%04d%02d%02d.csv", strPath, time.wYear, time.wMonth, time.wDay); } 
//	strFile.Format("%s\\%04d-%02d-%02d.csv", strPath, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			if (file.GetLength() < 1) {
				strTitle.Format("Time,Teach,Encoder\r\n");
				file.Write(strTitle, strTitle.GetLength());
			}

			strSave.Format("%02d:%02d:%02d %03d,%s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}

// 	CFile file;
// 	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
// 		try {
// 			file.SeekToEnd();
// 
// 			strSave.Format("%02d:%02d:%02d %03d,%s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);
// 
// 			file.Write(strSave, strSave.GetLength());
// 			file.Close();
// 
// 		} catch (CFileException *pEx) {
// 			pEx->Delete();
// 		}
// 	}
}

void CLogFile::Save_RosWaitLog(int nPNo, CString sLog)
{
	g_csRosWaitLog.Lock();

	CString strPath = gsCurrentDir + "\\LOG\\ResultWait";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, strTitle, strSave, strLotID;
	strLotID = gLot.sLotID[nPNo-1];
	if (strLotID == "") strLotID = "LOT_ID";

	strFile.Format("%s\\%s_%04d%02d%02d_ROS.csv", strPath, strLotID, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			if (file.GetLength() < 1) {
				strTitle.Format("Time,Port_No,Tray_No,CM_No,Start_Time,Wait_Time\r\n");
				file.Write(strTitle, strTitle.GetLength());
			}

			strSave.Format("%02d:%02d:%02d %03d,%s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);	// , 추가

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csRosWaitLog.Unlock();
}

void CLogFile::Save_InspectWaitLog(int nPNo, CString sLog)
{
	g_csInspectWaitLog.Lock();

	CString strPath = gsCurrentDir + "\\LOG\\ResultWait";

	Create_Folder(strPath);

	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strFile, strTitle, strSave, strLotID;
	strLotID = gLot.sLotID[nPNo-1];
	if (strLotID == "") strLotID = "LOT_ID";

	strFile.Format("%s\\%s_%04d%02d%02d_Inspect.csv", strPath, strLotID, time.wYear, time.wMonth, time.wDay);

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			if (file.GetLength() < 1) {
				strTitle.Format("Time,Port_No,Tray_No,CM_No,Start_Time,Wait_Time\r\n");
				file.Write(strTitle, strTitle.GetLength());
			}

			strSave.Format("%02d:%02d:%02d %03d,%s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);	// , 추가

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
	g_csInspectWaitLog.Unlock();
}

///////////////////////////////////////////////////////////////////////////////



// v1.0.1.1 Test
void CLogFile::Save_JobListExcel(CString sLog)
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strPath, strFile, strTitle, strSave, strJudge;
	strPath.Format("%s\\LOG\\LotJobList\\%04d-%02d-%02d", gsCurrentDir, time.wYear, time.wMonth, time.wDay);
	Create_Folder(strPath);
	int nNo = gData.nLPNo-1;
	if (nNo < 0) nNo = 0;


	if (gLot.sLotID[nNo] == "") gLot.sLotID[nNo] = "LOT_ID";
	strFile.Format("%s\\%s.csv", strPath, gLot.sLotID[nNo]);	

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();
// 			if (file.GetLength() < 1) {
// 				strTitle.Format("Time,호기,Lot ID,Lot Start,Lot End,Cycle Time,LM(EA),Run Time,Stop Time,Error Time,Error Count,수율,UPH,ROS Time\r\n");
// 				file.Write(strTitle, strTitle.GetLength());
// 			}
			strSave.Format("%02d:%02d:%02d %03d,%s\r\n", time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, sLog);

			file.Write(strSave, strSave.GetLength());
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}
}

void CLogFile::Save_AverageCycle(int nPNo)
{
	SYSTEMTIME time;
	GetLocalTime(&time);

	CString strPath, strFile, strLog, strMsg = "";
	strPath.Format("%s\\LOG\\LotJobList\\%04d-%02d-%02d", gsCurrentDir, time.wYear, time.wMonth, time.wDay);
	Create_Folder(strPath);

	strFile.Format("%s\\%s_Average.csv", strPath, gLot.sLotID[nPNo]);	

	CFile file;
	if (file.Open(strFile, CFile::modeCreate | CFile::modeNoTruncate | CFile::modeWrite)) {
		try {
			file.SeekToEnd();

			for (int i = 1; i <= 28; i++) {
				for (int j = 1; j <= 14; j++) {
					strMsg = Job_Msg(i, j);
					if (strMsg != "") {
						double dTemp = gLot.dAverageCycle[i-1][1][j-1] == 0 ? 0.0 : gLot.dAverageCycle[i-1][1][j-1] / gLot.dAverageCycle[i-1][0][j-1];
						strLog.Format("%02d:%02d:%02d %03d,%s,%0.3lf\r\n",time.wHour, time.wMinute, time.wSecond, time.wMilliseconds, strMsg, dTemp);
						file.Write(strLog, strLog.GetLength());
					} else 
						break;
				}
			}
			file.Close();

		} catch (CFileException *pEx) {
			pEx->Delete();
		}
	}

}

CString CLogFile::Job_Msg(int nFun, int nId)
{
	CString strFun, strLog, strMsg = "";
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
	if (strMsg == "") return "";
	CString strTemp;
	strTemp.Format("%s,%d,%s", strFun, nId, strMsg);
	return strTemp;
}