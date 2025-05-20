// SetupEquipDlg.cpp : ±∏«ˆ ∆ƒ¿œ¿‘¥œ¥Ÿ.
//
#include "stdafx.h"
#include "CMI8000.h"
#include "SetupEquipDlg.h"
#include "afxdialogex.h"

#include "LogFile.h"
#include "DataManager.h"
#include "Common.h"

#include "SetupDlg.h"
#include "Inspector.h"
#include "SequenceMain.h"
#include "SequenceInit.h"
#include "WorkDlg.h"

#include "CMI8000Dlg.h"
#include "Dispatcher.h"

// CSetupEquipDlg ¥Î»≠ ªÛ¿⁄¿‘¥œ¥Ÿ.

IMPLEMENT_DYNAMIC(CSetupEquipDlg, CDialogEx)

CSetupEquipDlg::CSetupEquipDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CSetupEquipDlg::IDD, pParent)
{
}

CSetupEquipDlg::~CSetupEquipDlg()
{
}

void CSetupEquipDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_GROUP_0 + i, m_Group[i]);
	for (int i = 0; i < 34; i++) DDX_Control(pDX, IDC_LABEL_0 + i,  m_Label[i]);
	DDX_Control(pDX, IDC_STC_EQUIP_NAME, m_stcEquipName);
	DDX_Control(pDX, IDC_CBO_LOT_BARCODE_PORT, m_cboLotBarcodePort);
	DDX_Control(pDX, IDC_CHK_JOB_LIST_TAKT_LOG, m_chkJobListTaktLog);
	DDX_Control(pDX, IDC_STC_SCREEN_OFF, m_stcScreenOff);
	DDX_Control(pDX, IDC_STC_NOWORK_TIME, m_stcNoWorkTime);
	DDX_Control(pDX, IDC_CHK_MANUAL_TAKT_TEST, m_chkManualTaktTest);
	DDX_Control(pDX, IDC_LBL_DOOR_LOCK, m_lblDoorLock);
	for (int i = 0; i < 2; i++) DDX_Control(pDX, IDC_RDO_DOOR_LOCK_0 + i, m_rdoDoorLock[i]);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_RDO_MODEL_0 + i, m_rdoModel[i]);
	DDX_Control(pDX, IDC_CHK_USE_VISION_ALIGN, m_chkUseVisionAlign);
	DDX_Control(pDX, IDC_CHK_USE_INSPECT_ANGLE, m_chkUseInspectAngle);
	DDX_Control(pDX, IDC_CHK_USE_INSPECT_BTM_1, m_chkUseInspectBtm1);
	DDX_Control(pDX, IDC_CHK_USE_INSPECT_TOP_1, m_chkUseInspectTop1);
	DDX_Control(pDX, IDC_CHK_USE_INSPECT_TOP_2, m_chkUseInspectTop2);
	DDX_Control(pDX, IDC_CHK_USE_INSPECT_BTM_2, m_chkUseInspectBtm2);
	DDX_Control(pDX, IDC_CHK_USE_DISPATCHER, m_chkUseDispatcher);
	DDX_Control(pDX, IDC_CHK_USE_MES_NG, m_chkUseMesNg);
	DDX_Control(pDX, IDC_CHK_USE_MES, m_chkUseMES);

	DDX_Control(pDX, IDC_CHK_USE_ROS_SKIP, m_chkUseRosSkip);
	DDX_Control(pDX, IDC_CHK_USE_INSP_SKIP, m_chkUseInspectSkip);
	
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_STC_TRAY_DATA_0 + i, m_stcTrayData[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_STC_PICKER_DATA_0 + i, m_stcPickerData[i]);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_STC_INSPECT_DATA_0 + i, m_stcInspectData[i]);
	for (int i = 0; i < 1; i++) DDX_Control(pDX, IDC_STC_NG_BUFFER_DATA_0 + i, m_stcNgBufferData[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_STC_VAC_OFF_DELAY_0 + i, m_stcVacOffDelay[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_STC_VAC_OFF_REPEAT_0 + i, m_stcVacOffRepeat[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_STC_DELAY_ADD_0 + i, m_stcDelayAdd[i]);
	DDX_Control(pDX, IDC_GRP_ROS, m_grpRos);
	for (int i = 0; i < 3; i++) DDX_Control(pDX, IDC_LBL_ROS_0 + i, m_lblRos[i]);
	DDX_Control(pDX, IDC_IPA_DISPATCHER_IP, m_ipaDispatcherIp);
	DDX_Control(pDX, IDC_IPA_HANDLER_IP, m_ipaHandlerIp);
	DDX_Control(pDX, IDC_BTN_ROS_CONNECT, m_btnRosConnect);
	DDX_Control(pDX, IDC_BTN_ROS_DISCONNECT, m_btnRosDisconnect);
	DDX_Control(pDX, IDC_STC_JUDGE_TIME_OVER, m_stcJudgeTimeOver);
	DDX_Control(pDX, IDC_CHK_JUDGE_SPECIA_NG, m_chkJudgeSpecialNg);
	DDX_Control(pDX, IDC_GRP_TOWER, m_grpTower);
	for (int i = 0; i < 10; i++) DDX_Control(pDX, IDC_LBL_TOWER_0 + i,  m_lblTower[i]);
	for (int i = 0; i < 6; i++) for (int j = 0; j < 4; j++) DDX_Control(pDX, IDC_CHK_TOWER_0_0 + i * 4 + j,  m_chkTower[i][j]);
	DDX_Control(pDX, IDC_GRP_BUZZER, m_grpBuzzer);
	for (int i = 0; i < 8; i++) DDX_Control(pDX, IDC_LBL_BUZZER_0 + i,  m_lblBuzzer[i]);
	for (int i = 0; i < 2; i++) for (int j = 0; j < 6; j++) DDX_Control(pDX, IDC_CHK_BUZZER_0_0 + i * 6 + j,  m_chkBuzzer[i][j]);
	DDX_Control(pDX, IDC_GRP_HIDDEN, m_grpHidden);
	DDX_Control(pDX, IDC_LBL_PASSWORD_MT, m_lblPasswordMt);
	DDX_Control(pDX, IDC_STC_PASSWORD_MT, m_stcPasswordMt);
	DDX_Control(pDX, IDC_LBL_PASSWORD_SI, m_lblPasswordSi);
	DDX_Control(pDX, IDC_EDT_PASSWORD_SI, m_edtPasswordSi);
	DDX_Control(pDX, IDC_GRP_RESULT_TEST, m_grpResultTest);
	DDX_Control(pDX, IDC_CHK_RESULT_TEST_USE, m_chkResultTestUse);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_LBL_RESULT_TEST_0 + i, m_lblResultTest[i]);
	for (int i = 0; i < 4; i++) DDX_Control(pDX, IDC_EDT_RESULT_TEST_0 + i, m_edtResultTest[i]);
	DDX_Control(pDX, IDC_STC_VIS_PROG_VER, m_stcVisProgVer);
	DDX_Control(pDX, IDC_STC_VIS_PARA_VER, m_stcVisParaVer);
	DDX_Control(pDX, IDC_CHK_USE_FOCSU_LOG, m_chkUseFocusLog);
	DDX_Control(pDX, IDC_STC_TMR_FOCUS_LOG, m_stcTmrFocusLog);

	DDX_Control(pDX, IDC_STC_MOTION_CHECK, m_stcMotionCheck);
}

BEGIN_MESSAGE_MAP(CSetupEquipDlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_STN_CLICKED(IDC_STC_EQUIP_NAME, &CSetupEquipDlg::OnStnClickedStcEquipName)
	ON_STN_CLICKED(IDC_STC_SCREEN_OFF, &CSetupEquipDlg::OnStnClickedStcScreenOff)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_TRAY_DATA_0, IDC_STC_TRAY_DATA_3, OnStcTrayDataClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_PICKER_DATA_0, IDC_STC_PICKER_DATA_3, OnStcPickerDataClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_INSPECT_DATA_0, IDC_STC_INSPECT_DATA_7, OnStcInspectDataClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_NG_BUFFER_DATA_0, IDC_STC_NG_BUFFER_DATA_0, OnStcNgBufferDataClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_VAC_OFF_DELAY_0, IDC_STC_VAC_OFF_DELAY_3, OnStcVacOffDelayClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_VAC_OFF_REPEAT_0, IDC_STC_VAC_OFF_REPEAT_3, OnStcVacOffRepeatClick)
	ON_CONTROL_RANGE(STN_CLICKED, IDC_STC_DELAY_ADD_0, IDC_STC_DELAY_ADD_3, OnStcDelayAddClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_RDO_MODEL_0, IDC_RDO_MODEL_2, OnRdoModelClick)
	ON_BN_CLICKED(IDC_BTN_ROS_CONNECT, &CSetupEquipDlg::OnBnClickedBtnRosConnect)
	ON_BN_CLICKED(IDC_BTN_ROS_DISCONNECT, &CSetupEquipDlg::OnBnClickedBtnRosDisconnect)
	ON_STN_CLICKED(IDC_STC_JUDGE_TIME_OVER, &CSetupEquipDlg::OnStnClickedStcJudgeTimeOver)
	ON_STN_CLICKED(IDC_STC_SHOW_HIDDEN, &CSetupEquipDlg::OnStnClickedStcShowHidden)
	ON_STN_CLICKED(IDC_STC_PASSWORD_MT, &CSetupEquipDlg::OnStnClickedStcPasswordMt)
	ON_STN_CLICKED(IDC_STC_NOWORK_TIME, &CSetupEquipDlg::OnStnClickedStcNoworkTime)
	ON_STN_CLICKED(IDC_STC_VIS_PROG_VER, &CSetupEquipDlg::OnStnClickedStcVisProgVer)
	ON_STN_CLICKED(IDC_STC_VIS_PARA_VER, &CSetupEquipDlg::OnStnClickedStcVisParaVer)
	ON_STN_CLICKED(IDC_STC_TMR_FOCUS_LOG, &CSetupEquipDlg::OnStnClickedStcTmrFocusLog)


	ON_STN_CLICKED(IDC_STC_MOTION_CHECK, &CSetupEquipDlg::OnStnClickedStcMotionCheck)
END_MESSAGE_MAP()

// CSetupEquipDlg ∏ﬁΩ√¡ˆ √≥∏Æ±‚¿‘¥œ¥Ÿ.

BOOL CSetupEquipDlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  ø©±‚ø° √ﬂ∞° √ ±‚»≠ ¿€æ˜¿ª √ﬂ∞°«’¥œ¥Ÿ.
	SetWindowPos(this, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	m_nPreModel = -1;
	Initial_Controls();

#ifdef TYPE_R13
	m_rdoModel[0].SetWindowText("R13A");
	m_rdoModel[1].SetWindowText("R13B (4X3)");
	m_rdoModel[2].SetWindowText("R13B (5X3)");

	m_rdoModel[1].ShowWindow(FALSE);
#endif

	return TRUE;  // return TRUE unless you set the focus to a control
	// øπø‹: OCX º”º∫ ∆‰¿Ã¡ˆ¥¬ FALSE∏¶ π›»Ø«ÿæﬂ «’¥œ¥Ÿ.
}

BOOL CSetupEquipDlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CSetupEquipDlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (!bShow) {
		if (m_nPreModel != -1) {
			CIniFileCS INI(gsCurrentDir + "\\System\\ModelData.ini");
			if (!INI.Check_File()) { AfxMessageBox("ModelData.ini File Not Found!!!"); return; }

			INI.Set_Integer("MODEL", "MODEL", m_nPreModel);

			g_objDataManager.Read_ModelData();
			g_objDataManager.Read_EquipData();
			g_objDataManager.Read_IdleData();
			g_objDataManager.Read_RosData();

			m_nPreModel = -1;
		}
		return;
	}

	Display_EquipData();

	m_grpHidden.ShowWindow(SW_HIDE);
	m_lblPasswordMt.ShowWindow(SW_HIDE);
	m_stcPasswordMt.ShowWindow(SW_HIDE);
	m_lblPasswordSi.ShowWindow(SW_HIDE);
	m_edtPasswordSi.ShowWindow(SW_HIDE);

#ifdef DRY_RUN_TEST
	m_grpResultTest.ShowWindow(SW_SHOW);
	m_chkResultTestUse.ShowWindow(SW_HIDE);
	for (int i = 0; i < 4; i++) m_lblResultTest[i].ShowWindow(SW_SHOW);
	for (int i = 0; i < 4; i++) m_edtResultTest[i].ShowWindow(SW_SHOW);
#else
	m_grpResultTest.ShowWindow(SW_HIDE);
	m_chkResultTestUse.ShowWindow(SW_HIDE);
	for (int i = 0; i < 4; i++) m_lblResultTest[i].ShowWindow(SW_HIDE);
	for (int i = 0; i < 4; i++) m_edtResultTest[i].ShowWindow(SW_HIDE);
#endif

}

void CSetupEquipDlg::OnStnClickedStcEquipName()
{
	CString strKey;
	if (g_objCommon.Show_KeyPad(strKey) != IDOK) return;

	m_stcEquipName.SetWindowText(strKey);
}

void CSetupEquipDlg::OnStnClickedStcScreenOff()
{
	CString strOld, strNew;
	m_stcScreenOff.GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcScreenOff.SetWindowText(strNew);
}

void CSetupEquipDlg::OnStnClickedStcNoworkTime()
{
	CString strOld, strNew;
	m_stcNoWorkTime.GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcNoWorkTime.SetWindowText(strNew);
}

void CSetupEquipDlg::OnStcTrayDataClick(UINT nID)
{
	int ID = nID - IDC_STC_TRAY_DATA_0;

	CString strOld, strNew;
	m_stcTrayData[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcTrayData[ID].SetWindowText(strNew);
}

void CSetupEquipDlg::OnStcPickerDataClick(UINT nID)
{
	int ID = nID - IDC_STC_PICKER_DATA_0;

	CString strOld, strNew;
	m_stcPickerData[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcPickerData[ID].SetWindowText(strNew);
}

void CSetupEquipDlg::OnStcInspectDataClick(UINT nID)
{
	int ID = nID - IDC_STC_INSPECT_DATA_0;

	CString strOld, strNew;
	m_stcInspectData[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcInspectData[ID].SetWindowText(strNew);
}

void CSetupEquipDlg::OnStcNgBufferDataClick(UINT nID)
{
	int ID = nID - IDC_STC_NG_BUFFER_DATA_0;

	CString strOld, strNew;
	m_stcNgBufferData[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcNgBufferData[ID].SetWindowText(strNew);
}

void CSetupEquipDlg::OnStcVacOffDelayClick(UINT nID)
{
	int ID = nID - IDC_STC_VAC_OFF_DELAY_0;

	CString strOld, strNew;
	m_stcVacOffDelay[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcVacOffDelay[ID].SetWindowText(strNew);
}

void CSetupEquipDlg::OnStcVacOffRepeatClick(UINT nID)
{
	int ID = nID - IDC_STC_VAC_OFF_REPEAT_0;

	CString strOld, strNew;
	m_stcVacOffRepeat[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcVacOffRepeat[ID].SetWindowText(strNew);
}

void CSetupEquipDlg::OnStcDelayAddClick(UINT nID)
{
	int ID = nID - IDC_STC_DELAY_ADD_0;

	CString strOld, strNew;
	m_stcDelayAdd[ID].GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcDelayAdd[ID].SetWindowText(strNew);
}

void CSetupEquipDlg::OnBnClickedBtnRosConnect()
{
	if (!g_objDispatcher.Initialize()) return;
	m_ipaDispatcherIp.EnableWindow(FALSE);
	m_ipaHandlerIp.EnableWindow(FALSE);
	g_objLogFile.Save_DispatcherLog("[Ros Connect] Click");
}

void CSetupEquipDlg::OnBnClickedBtnRosDisconnect()
{
	g_objDispatcher.Terminate();
	m_ipaDispatcherIp.EnableWindow(TRUE);
	m_ipaHandlerIp.EnableWindow(TRUE);
	g_objLogFile.Save_DispatcherLog("[Ros Disconnect] Click");
}

void CSetupEquipDlg::OnStnClickedStcJudgeTimeOver()
{
	CString strOld, strNew;
	m_stcJudgeTimeOver.GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcJudgeTimeOver.SetWindowText(strNew);
}

void CSetupEquipDlg::OnStnClickedStcPasswordMt()
{
	CString strTemp, strData;

	if (g_objCommon.Show_NumPad("", strData) != IDOK) return;

	int nLen = strData.GetLength();
	if (nLen < 1) return;

	for (int i = 0; i < nLen; i++ ) {
		strTemp = strData.Mid(i, 1);
		if (strTemp < "0" || strTemp > "9") { AfxMessageBox("Password Input Error......"); return; }
	}
	m_stcPasswordMt.SetWindowText(strData);
}

void CSetupEquipDlg::OnStnClickedStcShowHidden()
{
	if (m_grpHidden.IsWindowVisible()) {
		m_grpHidden.ShowWindow(SW_HIDE);
		m_lblPasswordMt.ShowWindow(SW_HIDE);
		m_stcPasswordMt.ShowWindow(SW_HIDE);
		if (g_dlgSetup.Get_LoginUser() != 2) return;
		m_lblPasswordSi.ShowWindow(SW_HIDE);
		m_edtPasswordSi.ShowWindow(SW_HIDE);
	} else {
		m_grpHidden.ShowWindow(SW_SHOW);
		m_lblPasswordMt.ShowWindow(SW_SHOW);
		m_stcPasswordMt.ShowWindow(SW_SHOW);
		if (g_dlgSetup.Get_LoginUser() != 2) return;
		m_lblPasswordSi.ShowWindow(SW_SHOW);
		m_edtPasswordSi.ShowWindow(SW_SHOW);
	}

#ifndef DRY_RUN_TEST
	if (m_grpResultTest.IsWindowVisible()) {
		m_grpResultTest.ShowWindow(SW_HIDE);
		m_chkResultTestUse.ShowWindow(SW_HIDE);
		for (int i = 0; i < 4; i++) m_lblResultTest[i].ShowWindow(SW_HIDE);
		for (int i = 0; i < 4; i++) m_edtResultTest[i].ShowWindow(SW_HIDE);
	} else {
		m_grpResultTest.ShowWindow(SW_SHOW);
		m_chkResultTestUse.ShowWindow(SW_SHOW);
		for (int i = 0; i < 4; i++) m_lblResultTest[i].ShowWindow(SW_SHOW);
		for (int i = 0; i < 4; i++) m_edtResultTest[i].ShowWindow(SW_SHOW);
	}
#endif
}

void CSetupEquipDlg::OnRdoModelClick(UINT nID)
{
	int ID = nID - IDC_RDO_MODEL_0;
#ifdef TYPE_R13
	if (m_nPreModel == -1 && gData.bR13A)   m_nPreModel = R13A;
	if (m_nPreModel == -1 && gData.bR13B_4) m_nPreModel = R13B_4;
	if (m_nPreModel == -1 && gData.bR13B_5) m_nPreModel = R13B_5;
	ID = ID + 3;
#else
	if (m_nPreModel == -1 && gData.bR04A) m_nPreModel = R04A;
	if (m_nPreModel == -1 && gData.bR04B) m_nPreModel = R04B;
	if (m_nPreModel == -1 && gData.bR04C) m_nPreModel = R04C;
#endif

	CIniFileCS INI(gsCurrentDir + "\\System\\ModelData.ini");
	if (!INI.Check_File()) { AfxMessageBox("ModelData.ini File Not Found!!!"); return; }

	INI.Set_Integer("MODEL", "MODEL", ID);

	g_objDataManager.Read_ModelData();
	g_objDataManager.Read_EquipData();

	Display_EquipData();
}

void CSetupEquipDlg::OnStnClickedStcVisProgVer()
{
	CString strOld, strNew;
	m_stcVisProgVer.GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;
	
	int nData = atoi(strNew); strNew.Format("%04d", nData);
	m_stcVisProgVer.SetWindowText(strNew);
}

void CSetupEquipDlg::OnStnClickedStcVisParaVer()
{
	CString strOld, strNew;
	m_stcVisParaVer.GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	int nData = atoi(strNew); strNew.Format("%04d", nData);
	m_stcVisParaVer.SetWindowText(strNew);
}

void CSetupEquipDlg::OnStnClickedStcTmrFocusLog()
{
	CString strOld, strNew;
	m_stcTmrFocusLog.GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcTmrFocusLog.SetWindowText(strNew);
}

///////////////////////////////////////////////////////////////////////////////
// User Functions

void CSetupEquipDlg::Initial_Controls() 
{
	CString strText;
	for (int i = 0; i < 8; i++) m_Group[i].Init_Ctrl("πŸ≈¡", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	for (int i = 0; i < 3; i++) m_Label[i].Init_Ctrl("πŸ≈¡", 11, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x10, 0xC0));	// Equip Data
	for (int i = 3; i < 7; i++) m_Label[i].Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x20, 0x50, 0x00));	// Tray Data
	for (int i = 7; i < 11; i++) m_Label[i].Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x40, 0x20, 0x20));	// Picker Data
	for (int i = 11; i < 15; i++) m_Label[i].Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x40, 0x80));	// Inspect Data
	for (int i = 15; i < 17; i++) m_Label[i].Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x60, 0x60, 0x60));	// Vac Label
	for (int i = 17; i < 21; i++) m_Label[i].Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x80, 0x00, 0x00));	// Vacuum Off
	for (int i = 21; i < 24; i++) m_Label[i].Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x40, 0x60, 0x40));	// Delay Add
	m_Label[24].Init_Ctrl("πŸ≈¡", 11, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x10, 0xC0));	// Equip Data
	m_Label[25].Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x80, 0xC0));	// Inspect Offset
	m_Label[26].Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x40, 0x60, 0x40));	// Delay Add
	m_Label[27].Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x80, 0xC0));	// Inspect Offset
	m_Label[28].Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0xB0, 0xFF));	// Inspect Top2 Shift X
	m_Label[29].Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0xB0, 0xFF));	// Inspect Top2 Shift X
	m_Label[30].Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0xB0, 0xFF));	// Inspect Top2 Shift X
	for (int i = 31; i < 34; i++) m_Label[i].Init_Ctrl("πŸ≈¡", 11, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x10, 0xC0));
	m_stcEquipName.Init_Ctrl("πŸ≈¡", 15, TRUE, RGB(0x00, 0x00, 0x80), RGB(0xE0, 0xFF, 0xE0));
	for (int i = 0; i < 4; i++) { strText.Format("COM%d", i + 1); m_cboLotBarcodePort.AddString(strText); }
	m_cboLotBarcodePort.Init_Ctrl("πŸ≈¡", 12, TRUE, RGB(0x00, 0x00, 0x00), RGB(0xF0, 0xE0, 0x00));
	m_chkJobListTaktLog.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xC0, 0xC0, 0xC0), CCheckCS::emRed, 0);
	m_stcScreenOff.Init_Ctrl("πŸ≈¡", 11, TRUE, RGB(0x00, 0x00, 0x80), RGB(0xF0, 0xE0, 0x00));
	m_stcNoWorkTime.Init_Ctrl("πŸ≈¡", 11, TRUE, RGB(0x00, 0x00, 0x80), RGB(0xF0, 0xE0, 0x00));
	m_chkManualTaktTest.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xC0, 0xC0, 0xC0), CCheckCS::emBlue, 0);
	m_lblDoorLock.Init_Ctrl("πŸ≈¡", 11, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x60, 0x60, 0x60));
	for (int i = 0; i < 2; i++) m_rdoDoorLock[i].Init_Ctrl("πŸ≈¡", 11, FALSE, COLOR_DEFAULT, RGB(0xC0, 0xC0, 0xC0), CRadioCS::emRed, 0);
	for (int i = 0; i < 3; i++) m_rdoModel[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xFF, 0xFF), CRadioCS::emRed, 0);
	m_chkUseVisionAlign.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0x60, 0xF0, 0x80), CCheckCS::emRed, 0);
	m_chkUseInspectAngle.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0x60, 0xF0, 0x80), CCheckCS::emRed, 0);
	m_chkUseInspectBtm1.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0x60, 0xF0, 0x80), CCheckCS::emRed, 0);
	m_chkUseInspectTop1.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0x60, 0xF0, 0x80), CCheckCS::emRed, 0);
	m_chkUseInspectTop2.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0x60, 0xF0, 0x80), CCheckCS::emRed, 0);
	m_chkUseInspectBtm2.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0x60, 0xF0, 0x80), CCheckCS::emRed, 0);
	m_chkUseDispatcher.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0x90, 0x90, 0xF0), CCheckCS::emRed, 0);
	m_chkUseMesNg.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0x90, 0x90, 0xF0), CCheckCS::emRed, 0);
	m_chkUseMES.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0x90, 0x90, 0xF0), CCheckCS::emRed, 0);
	m_chkUseRosSkip.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xC0, 0x20), CCheckCS::emRed, 0);
	m_chkUseInspectSkip.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xC0, 0x20), CCheckCS::emRed, 0);

	for (int i = 0; i < 2; i++) m_stcTrayData[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xD0, 0xD0, 0xD0));
	for (int i = 2; i < 4; i++) m_stcTrayData[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xFF, 0xE0));
	for (int i = 0; i < 2; i++) m_stcPickerData[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xD0, 0xD0, 0xD0));
	for (int i = 2; i < 4; i++) m_stcPickerData[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xFF, 0xE0));
	for (int i = 0; i < 2; i++) m_stcInspectData[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xD0, 0xD0, 0xD0));
	for (int i = 2; i < 6; i++) m_stcInspectData[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xFF, 0xE0));
	for (int i = 6; i < 8; i++) m_stcInspectData[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xFF, 0xE0));
	for (int i = 0; i < 1; i++) m_stcNgBufferData[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xFF, 0xE0));
	for (int i = 0; i < 4; i++) m_stcVacOffDelay[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xE0, 0xE0));
	for (int i = 0; i < 4; i++) m_stcVacOffRepeat[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xE0, 0xE0));
	for (int i = 0; i < 4; i++) m_stcDelayAdd[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xFF, 0xFF, 0xE0));
	m_grpRos.Init_Ctrl("πŸ≈¡", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	for (int i = 0; i < 3; i++) m_lblRos[i].Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x60, 0x60, 0x60)); // ROS Data
	m_btnRosConnect.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	m_btnRosDisconnect.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	m_stcJudgeTimeOver.Init_Ctrl("πŸ≈¡", 10, TRUE, RGB(0x00, 0x00, 0xFF), RGB(0xE0, 0xFF, 0xE0));
	m_chkJudgeSpecialNg.Init_Ctrl("πŸ≈¡", 10, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x60, 0x60, 0x60), CCheckCS::emBlue, 0);
	m_grpTower.Init_Ctrl("πŸ≈¡", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	for (int i = 0; i < 10; i++) m_lblTower[i].Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x40, 0x40, 0x40));
	for (int i = 0; i < 6; i++) for (int j = 0; j < 4; j++) m_chkTower[i][j].Init_Ctrl("πŸ≈¡", 11, FALSE, COLOR_DEFAULT, RGB(0x00, 0xF0, 0x00), CCheckCS::emRed, 0);
	m_grpBuzzer.Init_Ctrl("πŸ≈¡", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	for (int i = 0; i < 8; i++) m_lblBuzzer[i].Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x40, 0x40, 0x40));
	for (int i = 0; i < 2; i++) for (int j = 0; j < 6; j++) m_chkBuzzer[i][j].Init_Ctrl("πŸ≈¡", 11, FALSE, COLOR_DEFAULT, RGB(0x00, 0xE0, 0xF0), CCheckCS::emRed, 0);
	m_grpHidden.Init_Ctrl("πŸ≈¡", 10, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	m_lblPasswordMt.Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x60, 0x60, 0x60));
	m_stcPasswordMt.Init_Ctrl("πŸ≈¡", 11, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0xC0, 0xC0, 0xC0));
	m_lblPasswordSi.Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x60, 0x60, 0x60));
	m_edtPasswordSi.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	m_grpResultTest.Init_Ctrl("πŸ≈¡", 12, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	m_chkResultTestUse.Init_Ctrl("πŸ≈¡", 10, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x60, 0x60, 0x60), CCheckCS::emRed, 0);
	for (int i = 0; i < 4; i++) m_lblResultTest[i].Init_Ctrl("πŸ≈¡", 11, FALSE, RGB(0xFF, 0xFF, 0xFF), RGB(0x60, 0x60, 0x60));
	for (int i = 0; i < 4; i++) m_edtResultTest[i].Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, COLOR_DEFAULT);
	m_stcVisProgVer.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xC0, 0xFF, 0xF0));
	m_stcVisParaVer.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xC0, 0xFF, 0xF0));
	m_chkUseFocusLog.Init_Ctrl("πŸ≈¡", 11, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x00, 0x10, 0xC0), CCheckCS::emRed, 0);
	m_stcTmrFocusLog.Init_Ctrl("πŸ≈¡", 11, TRUE, COLOR_DEFAULT, RGB(0xC0, 0xFF, 0xF0));
	m_stcMotionCheck.Init_Ctrl("πŸ≈¡", 11, TRUE, RGB(0x00, 0x00, 0x80), RGB(0xF0, 0xE0, 0x00));
}

void CSetupEquipDlg::Display_EquipData()
{
	CString strData;
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	m_stcEquipName.SetWindowText(pEquipData->sEquipName);
	CCMI8000Dlg *pMainDlg = (CCMI8000Dlg*)AfxGetApp()->GetMainWnd();
	pMainDlg->Display_EquipName();
	m_cboLotBarcodePort.SetCurSel(pEquipData->nLotBarcodePort - 1);
	m_chkJobListTaktLog.SetCheck(pEquipData->bJobListTaktLog);
	strData.Format("%d", pEquipData->nScreenOff); m_stcScreenOff.SetWindowText(strData);
	strData.Format("%d", pEquipData->nNoWorkTime); m_stcNoWorkTime.SetWindowText(strData);
	m_chkManualTaktTest.SetCheck(pEquipData->bManualTaktTest);
	pEquipData->bUseDoorLock ? m_rdoDoorLock[1].SetCheck(TRUE) : m_rdoDoorLock[0].SetCheck(TRUE);
	m_chkUseVisionAlign.SetCheck(pEquipData->bUseVisionAlign);
	m_chkUseInspectAngle.SetCheck(pEquipData->bUseInspectAngle);
	m_chkUseInspectBtm1.SetCheck(pEquipData->bUseInspectBtm1);
	m_chkUseInspectTop1.SetCheck(pEquipData->bUseInspectTop1);
	m_chkUseInspectTop2.SetCheck(pEquipData->bUseInspectTop2);
	m_chkUseInspectBtm2.SetCheck(pEquipData->bUseInspectBtm2);
	m_chkUseDispatcher.SetCheck(pEquipData->bUseDispatcher);
	m_chkUseMesNg.SetCheck(pEquipData->bUseMesNg);
	m_chkUseMES.SetCheck(pEquipData->bUseMES);
	m_chkUseRosSkip.SetCheck(pEquipData->bUseRosSkip);
	m_chkUseInspectSkip.SetCheck(pEquipData->bUseInspectSkip);

	strData.Format("%0.3lf", gAlm.dMotionChkPos);	 m_stcMotionCheck.SetWindowText(strData);

	strData.Format("%s", pEquipData->sVisionProgVer); m_stcVisProgVer.SetWindowText(strData);
	strData.Format("%s", pEquipData->sVisionParaVer); m_stcVisParaVer.SetWindowText(strData);
	m_chkUseFocusLog.SetCheck(pEquipData->bUseFocusLog);
	strData.Format("%d", pEquipData->nTmrFocusLog); m_stcTmrFocusLog.SetWindowText(strData);

	strData.Format("%0.2lf", pEquipData->dTrayPitchX); m_stcTrayData[2].SetWindowText(strData);
	strData.Format("%0.2lf", pEquipData->dTrayPitchY); m_stcTrayData[3].SetWindowText(strData);
	strData.Format("%0.2lf", pEquipData->dPickerPitchX); m_stcPickerData[2].SetWindowText(strData);
	strData.Format("%0.2lf", pEquipData->dPickerPitchY); m_stcPickerData[3].SetWindowText(strData);
	strData.Format("%0.2lf", pEquipData->dInspectPitchX); m_stcInspectData[2].SetWindowText(strData);
	strData.Format("%0.2lf", pEquipData->dInspectPitchY); m_stcInspectData[3].SetWindowText(strData);
	strData.Format("%0.2lf", pEquipData->dInspectALOffset); m_stcInspectData[4].SetWindowText(strData);
	strData.Format("%0.2lf", pEquipData->dInspectALBackOffset); m_stcInspectData[5].SetWindowText(strData);
	strData.Format("%0.2lf", pEquipData->dInspectTop2ShiftX); m_stcInspectData[6].SetWindowText(strData);
	strData.Format("%0.2lf", pEquipData->dInspectBTM1ShiftX); m_stcInspectData[7].SetWindowText(strData);
	strData.Format("%0.2lf", pEquipData->dNgBufferPitchX); m_stcNgBufferData[0].SetWindowText(strData);

	for (int i = 0; i < 4; i++) { strData.Format("%d", pEquipData->nVacOffDelay[i]); m_stcVacOffDelay[i].SetWindowText(strData); }
	for (int i = 0; i < 4; i++) { strData.Format("%d", pEquipData->nVacOffRepeat[i]); m_stcVacOffRepeat[i].SetWindowText(strData); }
	for (int i = 0; i < 4; i++) { strData.Format("%d", pEquipData->nDelayAdd[i]); m_stcDelayAdd[i].SetWindowText(strData); }

	ROS_DATA *pRosData = g_objDataManager.Get_pRosData();
	m_ipaDispatcherIp.SetWindowText(pRosData->sDispatcherIp);
	m_ipaHandlerIp.SetWindowText(pRosData->sHandlerIp);
	m_ipaDispatcherIp.EnableWindow(!g_objDispatcher.Is_Opened());
	m_ipaHandlerIp.EnableWindow(!g_objDispatcher.Is_Opened());
	strData.Format("%d", pRosData->nJudgeTimeOver); m_stcJudgeTimeOver.SetWindowText(strData);
	m_chkJudgeSpecialNg.SetCheck(pRosData->bJudgeSpecialNg);
	for (int i = 0; i < 6; i++) for (int j = 0; j < 4; j++) m_chkTower[i][j].SetCheck(pEquipData->bTower[i][j]);
	for (int i = 0; i < 2; i++) for (int j = 0; j < 6; j++) m_chkBuzzer[i][j].SetCheck(pEquipData->bBuzzer[i][j]);
	m_stcPasswordMt.SetWindowText(pEquipData->sPasswordMt);
	m_edtPasswordSi.SetWindowText(pEquipData->sPasswordSi);
	m_chkResultTestUse.SetCheck(pEquipData->bResultTestUse);
	strData.Format("%d", pEquipData->nResultTestN1); m_edtResultTest[0].SetWindowText(strData);
	strData.Format("%d", pEquipData->nResultTestN2); m_edtResultTest[1].SetWindowText(strData);
	strData.Format("%d", pEquipData->nResultTestN3); m_edtResultTest[2].SetWindowText(strData);
	strData.Format("%d", pEquipData->nResultTestN4); m_edtResultTest[3].SetWindowText(strData);

#ifdef TYPE_R13
	if (gData.bR13A)   { m_rdoModel[0].SetCheck(TRUE); }
	if (gData.bR13B_4) { m_rdoModel[1].SetCheck(TRUE); }
	if (gData.bR13B_5) { m_rdoModel[2].SetCheck(TRUE); }
#else
	if (gData.bR04A) { m_rdoModel[0].SetCheck(TRUE); }
	if (gData.bR04B) { m_rdoModel[1].SetCheck(TRUE); }
	if (gData.bR04C) { m_rdoModel[2].SetCheck(TRUE); }
#endif

	// ∫Ø∞Ê ∫“∞°
	strData.Format("%d", gData.nTrayX); m_stcTrayData[0].SetWindowText(strData);		// Tray Count X
	strData.Format("%d", gData.nTrayY);	m_stcTrayData[1].SetWindowText(strData);		// Tray Count Y

	strData.Format("%d", gData.nBtmPickQt ); m_stcPickerData[0].SetWindowText(strData);	// Picker Count X
	m_stcPickerData[1].SetWindowText("2");												// Picker Count Y

	strData.Format("%d", gData.nBtmPickQt );m_stcInspectData[0].SetWindowText(strData);	// Inspect Count X
	m_stcInspectData[1].SetWindowText("2");												// Inspect Count Y
}

void CSetupEquipDlg::Save_EquipData()
{
	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();

	CString strKey, strData;
	int nData;
	double dData;

	CString strTemp;
	if (gData.bR04A) strTemp = "\\System\\EquipData_A.ini";
	if (gData.bR04B) strTemp = "\\System\\EquipData_B.ini";
	if (gData.bR04C) strTemp = "\\System\\EquipData_C.ini";
	if (gData.bR13A) strTemp = "\\System\\EquipData_R13A.ini";
	if (gData.bR13B_4) strTemp = "\\System\\EquipData_R13B_4.ini";
	if (gData.bR13B_5) strTemp = "\\System\\EquipData_R13B_5.ini";

// 	CIniFileCS INI(gsCurrentDir + "\\System\\EquipData.ini");
	CIniFileCS INI(gsCurrentDir + strTemp);
	if (!INI.Check_File()) { AfxMessageBox("EquipData.ini File Not Found!!!"); return; }

	if (gData.bR04A) strTemp = "EquipData_A";
	if (gData.bR04B) strTemp = "EquipData_B";
	if (gData.bR04C) strTemp = "EquipData_C";
	if (gData.bR13A) strTemp = "EquipData_R13A";
	if (gData.bR13B_4) strTemp = "EquipData_R13B_4";
	if (gData.bR13B_5) strTemp = "EquipData_R13B_5";
	g_objCommon.Backup_File(gsCurrentDir + "\\System", strTemp);

	m_stcEquipName.GetWindowText(strData); INI.Set_String("EQUIPMENT", "NAME", strData);
	nData = m_cboLotBarcodePort.GetCurSel(); INI.Set_Integer("EQUIPMENT", "LOT_BARCODE", nData + 1);
	INI.Set_Bool("EQUIPMENT", "JOB_LIST_TAKT_LOG", m_chkJobListTaktLog.GetCheck());
	m_stcScreenOff.GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("EQUIPMENT", "SCREEN_OFF", nData);
	m_stcNoWorkTime.GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("EQUIPMENT", "NOWORK_TIME", nData);
	INI.Set_Bool("EQUIPMENT", "MANUAL_TAKT_TEST", m_chkManualTaktTest.GetCheck());
	INI.Set_Bool("EQUIPMENT", "DOOR_LOCK", m_rdoDoorLock[1].GetCheck());

	m_stcMotionCheck.GetWindowText(strData); dData = atof(strData); INI.Set_Double("EQUIPMENT", "MOTION_CHECK", dData, "%0.3lf");

#ifndef DRY_RUN_TEST
	INI.Set_Bool("OPTION", "VISION_ALIGN", m_chkUseVisionAlign.GetCheck());
	INI.Set_Bool("OPTION", "INSPECT_ANGLE", m_chkUseInspectAngle.GetCheck());
	INI.Set_Bool("OPTION", "INSPECT_BTM_1", m_chkUseInspectBtm1.GetCheck());
	INI.Set_Bool("OPTION", "INSPECT_TOP_1", m_chkUseInspectTop1.GetCheck());
	INI.Set_Bool("OPTION", "INSPECT_TOP_2", m_chkUseInspectTop2.GetCheck());
	INI.Set_Bool("OPTION", "INSPECT_BTM_2", m_chkUseInspectBtm2.GetCheck());
	INI.Set_Bool("OPTION", "DISPATCHER", m_chkUseDispatcher.GetCheck());
	INI.Set_Bool("OPTION", "MES_NG", m_chkUseMesNg.GetCheck());
	INI.Set_Bool("OPTION", "MES_USE", m_chkUseMES.GetCheck());
#endif

	INI.Set_Bool("OPTION", "ROS_SKIP", m_chkUseRosSkip.GetCheck());
	INI.Set_Bool("OPTION", "INSPECT_SKIP", m_chkUseInspectSkip.GetCheck());

	m_stcVisProgVer.GetWindowText(strData); INI.Set_String("OPTION", "VIS_PROG_VER", strData);
	m_stcVisParaVer.GetWindowText(strData); INI.Set_String("OPTION", "VIS_PARA_VER", strData);
	INI.Set_Bool("OPTION", "USE_FOCUS_LOG", m_chkUseFocusLog.GetCheck());
	m_stcTmrFocusLog.GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("OPTION", "TMR_FOCUS_LOG", nData);

	m_stcTrayData[2].GetWindowText(strData); dData = atof(strData); INI.Set_Double ("TRAY", "PITCH_X", dData, "%0.2lf");
	m_stcTrayData[3].GetWindowText(strData); dData = atof(strData); INI.Set_Double ("TRAY", "PITCH_Y", dData, "%0.2lf");
	m_stcPickerData[2].GetWindowText(strData); dData = atof(strData); INI.Set_Double ("PICKER", "PITCH_X", dData, "%0.2lf");
	m_stcPickerData[3].GetWindowText(strData); dData = atof(strData); INI.Set_Double ("PICKER", "PITCH_Y", dData, "%0.2lf");
	m_stcInspectData[2].GetWindowText(strData); dData = atof(strData); INI.Set_Double ("INSPECT", "PITCH_X", dData, "%0.2lf");
	m_stcInspectData[3].GetWindowText(strData); dData = atof(strData); INI.Set_Double ("INSPECT", "PITCH_Y", dData, "%0.2lf");
	m_stcInspectData[4].GetWindowText(strData); dData = atof(strData); INI.Set_Double ("INSPECT", "AL_OFFSET", dData, "%0.2lf");
	m_stcInspectData[5].GetWindowText(strData); dData = atof(strData); INI.Set_Double ("INSPECT", "AL_BACK_OFFSET", dData, "%0.2lf");
	m_stcInspectData[6].GetWindowText(strData); dData = atof(strData); INI.Set_Double ("INSPECT", "TOP2_SHIFT_X", dData, "%0.2lf");
	m_stcInspectData[7].GetWindowText(strData); dData = atof(strData); INI.Set_Double ("INSPECT", "BTM1_SHIFT_X", dData, "%0.2lf");

	m_stcNgBufferData[0].GetWindowText(strData); dData = atof(strData); INI.Set_Double ("NG_BUFFER", "PITCH_X", dData, "%0.2lf");

	for (int i = 0; i < 4; i++) { strKey.Format("%d", i); m_stcVacOffDelay[i].GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("VAC_OFF_DELAY", strKey, nData); }
	for (int i = 0; i < 4; i++) { strKey.Format("%d", i); m_stcVacOffRepeat[i].GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("VAC_OFF_REPEAT", strKey, nData); }
	for (int i = 0; i < 4; i++) { strKey.Format("%d", i); m_stcDelayAdd[i].GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("DELAY_ADD", strKey, nData); }
	for (int i = 0; i < 6; i++) for (int j = 0; j < 4; j++) { strKey.Format("%d%d", i, j); INI.Set_Bool("TOWER", strKey, m_chkTower[i][j].GetCheck()); }
	for (int i = 0; i < 2; i++) for (int j = 0; j < 6; j++) { strKey.Format("%d%d", i, j); INI.Set_Bool("BUZZER", strKey, m_chkBuzzer[i][j].GetCheck()); }

	m_stcPasswordMt.GetWindowText(strData); INI.Set_String("HIDDEN", "PASSWORD_MT", strData);
	m_edtPasswordSi.GetWindowText(strData); INI.Set_String("HIDDEN", "PASSWORD_SI", strData);

	pEquipData->bResultTestUse = m_chkResultTestUse.GetCheck();
	m_edtResultTest[0].GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("RESULT_TEST", "N1", nData);
	m_edtResultTest[1].GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("RESULT_TEST", "N2", nData);
	m_edtResultTest[2].GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("RESULT_TEST", "N3", nData);
	m_edtResultTest[3].GetWindowText(strData); nData = atoi(strData); INI.Set_Integer("RESULT_TEST", "N4", nData);

	if (m_nPreModel != -1) {
#ifdef TYPE_R13
		if ((m_nPreModel != R13A && gData.bR13A) || (m_nPreModel != R13B_4 && gData.bR13B_4) || (m_nPreModel != R13B_5 && gData.bR13B_5))
#else
		if ((m_nPreModel != R04A && gData.bR04A) || (m_nPreModel != R04B && gData.bR04B) || (m_nPreModel != R04C && gData.bR04C))
#endif
		{
			g_objSequenceInit.Set_InitComplete(FALSE);
			g_objDataManager.Read_MoveData();
			g_dlgWork.PostMessage(UM_MODEL_CHANGE, NULL, NULL);
			gData.bBuffTrayChange = TRUE;
			strData.Format("Model Change µ«æ˙Ω¿¥œ¥Ÿ.\nPicker Jig ±≥√º »ƒ √ ±‚»≠ «ÿ¡÷Ω Ω√ø¿.\n√ ±‚»≠ øœ∑· »ƒ Buffer/NG Tray ±≥√º «ÿ¡÷Ω Ω√ø¿.");
			g_objCommon.Show_MsgBox(1, strData);
		}
		m_nPreModel = -1;
	}

	g_objLogFile.Save_HandlerLog("[Setup Equip] Save Click");

	CIniFileCS INI2(gsCurrentDir + "\\System\\RosData.ini");
	if (!INI2.Check_File()) { AfxMessageBox("RosData.ini File Not Found!!!"); return; }

	m_ipaDispatcherIp.GetWindowText(strData); INI2.Set_String("DISPATCHER", "DISPATCHER_IP", strData);
	m_ipaHandlerIp.GetWindowText(strData); INI2.Set_String("DISPATCHER", "HANDLER_IP", strData);
	m_stcJudgeTimeOver.GetWindowText(strData); nData = atoi(strData); INI2.Set_Integer("DISPATCHER", "TIME_OVER", nData);
	INI2.Set_Bool("DISPATCHER", "SPECIAL_NG", m_chkJudgeSpecialNg.GetCheck());

	Cancel_EquipData();
}

void CSetupEquipDlg::Cancel_EquipData()
{
	if (m_nPreModel != -1) {
		CIniFileCS INI(gsCurrentDir + "\\System\\ModelData.ini");
		if (!INI.Check_File()) { AfxMessageBox("ModelData.ini File Not Found!!!"); return; }

		INI.Set_Integer("MODEL", "MODEL", m_nPreModel);
		m_nPreModel = -1;
	}
	g_objDataManager.Read_ModelData();
	g_objDataManager.Read_EquipData();
	g_objDataManager.Read_IdleData();
	g_objDataManager.Read_RosData();

	Display_EquipData();

//	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
//	CCMI8000Dlg *pMainDlg = (CCMI8000Dlg*)AfxGetApp()->GetMainWnd();
//	pMainDlg->Set_TimerFocusAxis(pEquipData->bUseFocusLog, pEquipData->nTmrFocusLog);

	g_objLogFile.Save_HandlerLog("[Setup Equip] Cancel Click");
}

///////////////////////////////////////////////////////////////////////////////


void CSetupEquipDlg::OnStnClickedStcMotionCheck()
{
	CString strOld, strNew;
	m_stcMotionCheck.GetWindowText(strOld);
	if (g_objCommon.Show_NumPad(strOld, strNew) != IDOK) return;

	m_stcMotionCheck.SetWindowText(strNew);
}
