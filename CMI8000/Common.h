// Common.h : 헤더 파일
//
#pragma once

#define AUTO_COUNT			20

#define AUTO_LOAD_TRAY		0
#define AUTO_LOAD_PICKER	1
#define AUTO_VISION_ANGLE	2
#define AUTO_ANGLE_TRAY1	3
#define AUTO_ANGLE_TRAY2	4
#define AUTO_VISION_BTM1	5
#define AUTO_INSPECTION1	6
#define AUTO_INSPECTION2	7
#define AUTO_INSPECTION3	8
#define AUTO_VISION_BTM2	9
#define AUTO_BUFFER1		10
#define AUTO_BUFFER2		11
#define AUTO_SORT_PICKER1	12
#define AUTO_SORT_PICKER2	13
#define AUTO_GOOD_TRAY1		14
#define AUTO_GOOD_TRAY2		15
#define AUTO_NG_TRAY		16
#define AUTO_EMPTY_TRAY_X	17
#define AUTO_EMPTY_TRAY_EL	18
#define AUTO_EMPTY_TRAY_Y	19

#define INFO_STAGE		0
#define INFO_PICK		1

#define R04A	0
#define R04B	1
#define R04C	2
#define R13A	3
#define R13B_4	4
#define R13B_5	5

const double GOOD_TRAY_WIDTH = 360.0;	// Good Tray 1/2 간섭없는 간격

// CCommon

class CCommon : public CWnd
{
	DECLARE_DYNAMIC(CCommon)

public:
	CCommon();
	virtual ~CCommon();

protected:
	DECLARE_MESSAGE_MAP()

private:
	LONGLONG	m_nFreq;		// Frequence

public:
	void DoEvents();
	void uSleep(int msec);
	void Save_MotionPos();
	int	 Check_MotionPos();

	BOOL Check_Position(int nAxis, int nMoveIdx, double dRange = 0.1);
	void Move_Position(int nAxis, int nMoveIdx);

	void Show_Error(int nErrNo);
	void Set_ErrorSubMessage(CString sErrSubMsg);
	void Show_Alarm(CString sMsg, BOOL bOn=TRUE);
	int Show_MsgBox(int nBtnCnt, CString sMsg);
	int Show_NumPad(CString sOld, CString &sNew, int nAxis = -1);
	int Show_KeyPad(CString &sKey);
	int Show_Password(int &nUser);

	BOOL Check_MainEmgAir();
	BOOL Check_MainDoor(BOOL bAuto=FALSE);
	BOOL Check_PortArea(BOOL bAuto=FALSE);
	BOOL Check_TrayFull();

	BOOL Check_ServoOn();
	BOOL Check_DirveAlarm();
	BOOL Check_EndLimit();
	BOOL Check_HomeDone();

	void Locking_MainDoor(BOOL bLock, BOOL bAuto=FALSE);
	void Locking_PortSlide(BOOL bLock, int nPart=0);

	///////////////////////////////////////////////////////////////////////////

	int Get_Random(int nStart, int nEnd);
	void Backup_File(CString strPath, CString strName);	// System Folder

	///////////////////////////////////////////////////////////////////////////
	// 1. Load Port I/O 함수
	void Set_LoadPortSupportIn(int nNo);
	BOOL Get_LoadPortSupportIn(int nNo);
	void Set_LoadPortSupportOut(int nNo);
	BOOL Get_LoadPortSupportOut(int nNo);

	// 2. Load Tray Picker I/O 함수
	void Set_LoadPickerUp();
	BOOL Get_LoadPickerUp();
	void Set_LoadPickerDown();
	BOOL Get_LoadPickerDown();
	BOOL Get_LoadPickerMasterSlaveIn();
	BOOL Get_LoadPickerMasterSlaveOut();

	// 3. Angle Port I/O 함수
	void Set_AnglePortSupportIn();
	BOOL Get_AnglePortSupportIn();
	void Set_AnglePortSupportOut();
	BOOL Get_AnglePortSupportOut();
	BOOL Get_AnglePortMasterSlaveIn();
	BOOL Get_AnglePortMasterSlaveOut();

	// 4. Btm1 Picker I/O 함수
// 	void Set_Btm1PickerUp(int nSub);
// 	BOOL Get_Btm1PickerUp(int nSub);
 	void Set_Btm1PickerDown();		// ALL Down
 	BOOL Get_Btm1PickerDown(int nSub);
	void Set_Btm1PickerUp24();		// 2번, 4번
	BOOL Get_Btm1PickerUp24();
	void Set_Btm1PickerUp79();		// 7번, 9번
	BOOL Get_Btm1PickerUp79();
	void Set_Btm1PickerVacOff(int nSub);
	BOOL Get_Btm1PickerVacOff(int nSub);
	void Set_Btm1PickerAirOff(int nSub);
	BOOL Get_Btm1PickerNormal();
	BOOL Get_Btm1PickerRotate();

	// 5. Inspection I/O 함수
	void Set_InspectStage1VacOff(int nSub);
	BOOL Get_InspectStage1VacOff(int nSub);
	void Set_InspectStage2VacOff(int nSub);
	BOOL Get_InspectStage2VacOff(int nSub);
	void Set_InspectStage3VacOff(int nSub);
	BOOL Get_InspectStage3VacOff(int nSub);

	void Set_InspectVacOn(int nNo, int nSub);
	BOOL Get_InspectVacOn(int nNo, int nSub);
	void Set_InspectVacOff(int nNo, int nSub);
	BOOL Get_InspectVacOff(int nNo, int nSub);

	// 6. Btm2 Picker I/O 함수
 	void Set_Btm2PickerUp(int nSub);
 	BOOL Get_Btm2PickerUp(int nSub);
	void Set_Btm2PickerDown();		// ALL Down
 	BOOL Get_Btm2PickerDown(int nSub);
	void Set_Btm2PickerUp24();		// 2번, 4번
	BOOL Get_Btm2PickerUp24();
	void Set_Btm2PickerUp79();		// 7번, 9번
	BOOL Get_Btm2PickerUp79();
	void Set_Btm2PickerVacOff(int nSub);
	BOOL Get_Btm2PickerVacOff(int nSub);
	void Set_Btm2PickerAirOff(int nSub);
	BOOL Get_Btm2PickerNormal();
	BOOL Get_Btm2PickerRotate();

// 	void Set_Btm2PickerDownMulti(int nSub, int nCnt, int nRow=0);
// 	BOOL Get_Btm2PickerDownMulti(int nSub, int nCnt, int nRow=0);
	void Set_Btm2PickerVacOffMulti(int nSub, int nCnt, int nRow=0);
	BOOL Get_Btm2PickerVacOffMulti(int nSub, int nCnt, int nRow=0);
	void Set_Btm2PickerAirOffMulti(int nSub, int nCnt, int nRow=0);

	// 7. Sort Picker1 I/O 함수
	void Set_SortPicker1Up(int nSub);
	BOOL Get_SortPicker1Up(int nSub);
	void Set_SortPicker1Down(int nSub);
	BOOL Get_SortPicker1Down(int nSub);
	void Set_SortPicker1VacOff(int nSub);
	BOOL Get_SortPicker1VacOff(int nSub);
	void Set_SortPicker1AirOff(int nSub);

	void Set_SortPicker1DownMulti(int nSub, int nCnt);
	BOOL Get_SortPicker1DownMulti(int nSub, int nCnt);
	void Set_SortPicker1VacOnMulti(int nSub, int nCnt);
	BOOL Get_SortPicker1VacOnMulti(int nSub, int nCnt);
	void Set_SortPicker1VacOffMulti(int nSub, int nCnt);
	BOOL Get_SortPicker1VacOffMulti(int nSub, int nCnt);
	void Set_SortPicker1AirOffMulti(int nSub, int nCnt);

	// 8. Sort Picker2 I/O 함수
	void Set_SortPicker2Up(int nSub);
	BOOL Get_SortPicker2Up(int nSub);
	void Set_SortPicker2Down(int nSub);
	BOOL Get_SortPicker2Down(int nSub);
	void Set_SortPicker2VacOff(int nSub);
	BOOL Get_SortPicker2VacOff(int nSub);
	void Set_SortPicker2AirOff(int nSub);

	void Set_SortPicker2DownMulti(int nSub, int nCnt);
	BOOL Get_SortPicker2DownMulti(int nSub, int nCnt);
	void Set_SortPicker2VacOnMulti(int nSub, int nCnt);
	BOOL Get_SortPicker2VacOnMulti(int nSub, int nCnt);
	void Set_SortPicker2VacOffMulti(int nSub, int nCnt);
	BOOL Get_SortPicker2VacOffMulti(int nSub, int nCnt);
	void Set_SortPicker2AirOffMulti(int nSub, int nCnt);

	// 9. Empty Tray X I/O 함수
	void Set_EmptyTrayXUp();
	BOOL Get_EmptyTrayXUp();
	void Set_EmptyTrayXDown();
	BOOL Get_EmptyTrayXDown();
	BOOL Get_EmptyTrayXMasterSlaveIn();
	BOOL Get_EmptyTrayXMasterSlaveOut();

	// 10. Empty Tray Y I/O 함수
	void Set_EmptyTrayYUp();
	BOOL Get_EmptyTrayYUp();
	void Set_EmptyTrayYDown();
	BOOL Get_EmptyTrayYDown();
	BOOL Get_EmptyTrayYMasterSlaveIn();
	BOOL Get_EmptyTrayYMasterSlaveOut();

	// 11. Tray I/O 함수
	BOOL Get_LoadTrayMasterSlaveIn();
	BOOL Get_LoadTrayMasterSlaveOut();
	BOOL Get_GoodTray1MasterSlaveIn();
	BOOL Get_GoodTray1MasterSlaveOut();
	BOOL Get_GoodTray2MasterSlaveIn();
	BOOL Get_GoodTray2MasterSlaveOut();
	
	///////////////////////////////////////////////////////////////////////////
	// 1. Info Btm1 Picker I/O 함수
	void Set_InfoBtm1PickerDown(int nPos, int nRow=2, int nNo=0, int nLine=0);
	BOOL Get_InfoBtm1PickerDown(int nPos, int nRow=2, int nNo=0, int nLine=0);
	void Set_InfoBtm1PickerVacOn(int nPos, int nRow=2, int nNo=0, int nLine=0);
	BOOL Get_InfoBtm1PickerVacOn();

	// 2. Info Inspection I/O 함수
	void Set_InfoInspectVacOn(int nNo, int nPos=0);
	BOOL Get_InfoInspectVacOn(int nNo, int nPos=0);

	// 3. Info Btm2 Picker I/O 함수
	void Set_InfoBtm2PickerDown(int nRow);
	BOOL Get_InfoBtm2PickerDown(int nRow);
	void Set_InfoBtm2VacOn(int nNo);	// 확인
	BOOL Get_InfoBtm2VacOn(int nNo);

	// 4. Info Sort Picker1 I/O 함수
	void Set_InfoSortPicker1Down(int nPos=0);
	BOOL Get_InfoSortPicker1Down(int nPos=0);
	void Set_InfoSortPicker1VacOn(int nPos=0);
	BOOL Get_InfoSortPicker1VacOn();

	// 5. Info Sort Picker2 I/O 함수
	void Set_InfoSortPicker2Down(int nPos=0);
	BOOL Get_InfoSortPicker2Down(int nPos=0);
	void Set_InfoSortPicker2VacOn(int nPos=0);
	BOOL Get_InfoSortPicker2VacOn();

	///////////////////////////////////////////////////////////////////////////
	//Ng Buffer I/O 함수
	void Set_NgBufferVacOff(int nNo);
	void Set_InfoNgBufferVacOn(int nNo);
	BOOL Get_InfoNgBufferVacOn(int nNo);

	///////////////////////////////////////////////////////////////////////////
	// Load Tray Info
	int *Get_AngleTrayInfo(int nSNo, int nLine);
	int Get_AngleTrayLine();

	///////////////////////////////////////////////////////////////////////////
	CString Get_Btm1PickerVacErr();
	CString Get_Btm2PickerVacErr();
	CString Get_Sort1PickerVacErr();
	CString Get_Sort2PickerVacErr();
};

extern CCommon g_objCommon;

///////////////////////////////////////////////////////////////////////////////
