// ManualBtm1Dlg.cpp : 구현 파일입니다.
//
#include "stdafx.h"
#include "CMI8000.h"
#include "ManualBtm1Dlg.h"
#include "afxdialogex.h"
#include "math.h"

#include "LogFile.h"
#include "AJinAXL.h"
#include "DataManager.h"
#include "Common.h"
#include "SequenceMain.h"

// CManualBtm1Dlg 대화 상자입니다.

IMPLEMENT_DYNAMIC(CManualBtm1Dlg, CDialogEx)

CManualBtm1Dlg::CManualBtm1Dlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CManualBtm1Dlg::IDD, pParent)
{
}

CManualBtm1Dlg::~CManualBtm1Dlg()
{
}

void CManualBtm1Dlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	for (int i = 0; i < 19; i++) {if(i==5)continue; DDX_Control(pDX, IDC_GROUP_0 + i, m_Group[i]);}
	for (int i = 0; i < 14; i++) {if(i==4)continue; DDX_Control(pDX, IDC_LABEL_0 + i, m_Label[i]);}
	for (int i = 0; i < 14; i++) {if(i==4)continue; DDX_Control(pDX, IDC_STC_AXIS_POS_0 + i, m_stcAxisPos[i]);}
	for (int i = 0; i <  8; i++) DDX_Control(pDX, IDC_BTN_BTM1_PICKER_X_0 + i, m_btnBtm1PickerX[i]);
	for (int i = 0; i <  4; i++) DDX_Control(pDX, IDC_BTN_BTM1_PICKER_Z_0 + i, m_btnBtm1PickerZ[i]);
	for (int i = 0; i <  3; i++) DDX_Control(pDX, IDC_BTN_BTM1_PICKER_P1_0 + i, m_btnBtm1PickerP1[i]);
	for (int i = 0; i <  3; i++) DDX_Control(pDX, IDC_BTN_BTM1_PICKER_P2_0 + i, m_btnBtm1PickerP2[i]);
	for (int i = 0; i < 34; i++) DDX_Control(pDX, IDC_BTN_BTM1_PICKER_IO_0 + i, m_btnBtm1PickerIo[i]);
	for (int i = 0; i < 30; i++) DDX_Control(pDX, IDC_LED_BTM1_PICKER_IO_0 + i, m_ledBtm1PickerIo[i]);
	
	for (int i = 0; i < 10; i++) DDX_Control(pDX, IDC_BTN_INSPECT_STAGE1_X_0 + i, m_btnInspectStage1X[i]);
	for (int i = 0; i < 10; i++) DDX_Control(pDX, IDC_CHK_INSPECT_STAGE1_IO_0 + i, m_chkInspectStage1Io[i]);
	for (int i = 0; i <  4; i++) DDX_Control(pDX, IDC_BTN_INSPECT_STAGE1_IO_0 + i, m_btnInspectStage1Io[i]);
	for (int i = 0; i < 12; i++) DDX_Control(pDX, IDC_LED_INSPECT_STAGE1_IO_0 + i, m_ledInspectStage1Io[i]);
	for (int i = 0; i < 10; i++) DDX_Control(pDX, IDC_BTN_INSPECT_STAGE2_X_0 + i, m_btnInspectStage2X[i]);
	for (int i = 0; i < 10; i++) DDX_Control(pDX, IDC_CHK_INSPECT_STAGE2_IO_0 + i, m_chkInspectStage2Io[i]);
	for (int i = 0; i <  6; i++) DDX_Control(pDX, IDC_BTN_INSPECT_STAGE2_IO_0 + i, m_btnInspectStage2Io[i]);
	for (int i = 0; i < 14; i++) DDX_Control(pDX, IDC_LED_INSPECT_STAGE2_IO_0 + i, m_ledInspectStage2Io[i]);
	for (int i = 0; i < 10; i++) DDX_Control(pDX, IDC_BTN_INSPECT_STAGE3_X_0 + i, m_btnInspectStage3X[i]);
	for (int i = 0; i < 10; i++) DDX_Control(pDX, IDC_CHK_INSPECT_STAGE3_IO_0 + i, m_chkInspectStage3Io[i]);
	for (int i = 0; i <  6; i++) DDX_Control(pDX, IDC_BTN_INSPECT_STAGE3_IO_0 + i, m_btnInspectStage3Io[i]);
	for (int i = 0; i < 14; i++) DDX_Control(pDX, IDC_LED_INSPECT_STAGE3_IO_0 + i, m_ledInspectStage3Io[i]);
	for (int i = 0; i <  3; i++) DDX_Control(pDX, IDC_BTN_MODULE_ALIGN_Y_0 + i, m_btnModuleAlignY[i]);
	for (int i = 0; i <  2; i++) DDX_Control(pDX, IDC_BTN_MODULE_ALIGN_Z_0 + i, m_btnModuleAlignZ[i]);
	for (int i = 0; i <  1; i++) DDX_Control(pDX, IDC_BTN_VISION_TOP1_Z_0 + i, m_btnVisionTop1Z[i]);
	for (int i = 0; i <  2; i++) DDX_Control(pDX, IDC_BTN_TOP1_MIRROR_Z_0 + i, m_btnTop1MirrorZ[i]);
	for (int i = 0; i <  2; i++) DDX_Control(pDX, IDC_BTN_TOP1_MIRROR_IO_0 + i, m_btnTop1MirrorIo[i]);
	for (int i = 0; i <  4; i++) DDX_Control(pDX, IDC_LED_TOP1_MIRROR_IO_0 + i, m_ledTop1MirrorIo[i]);
	for (int i = 0; i <  1; i++) DDX_Control(pDX, IDC_BTN_VISION_TOP2_Z_0 + i, m_btnVisionTop2Z[i]);
}

BEGIN_MESSAGE_MAP(CManualBtm1Dlg, CDialogEx)
	ON_WM_SHOWWINDOW()
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_BTM1_PICKER_X_0, IDC_BTN_BTM1_PICKER_X_7, OnBtnBtm1PickerXClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_BTM1_PICKER_Z_0, IDC_BTN_BTM1_PICKER_Z_3, OnBtnBtm1PickerZClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_BTM1_PICKER_P1_0, IDC_BTN_BTM1_PICKER_P1_2, OnBtnBtm1PickerP1Click)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_BTM1_PICKER_P2_0, IDC_BTN_BTM1_PICKER_P2_2, OnBtnBtm1PickerP2Click)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_BTM1_PICKER_IO_0, IDC_BTN_BTM1_PICKER_IO_33, OnBtnBtm1PickerIoClick)
	
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_INSPECT_STAGE1_X_0, IDC_BTN_INSPECT_STAGE1_X_9, OnBtnInspectStage1XClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHK_INSPECT_STAGE1_IO_0, IDC_CHK_INSPECT_STAGE1_IO_9, OnChkInspectStage1IoClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_INSPECT_STAGE1_IO_0, IDC_BTN_INSPECT_STAGE1_IO_3, OnBtnInspectStage1IoClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_INSPECT_STAGE2_X_0, IDC_BTN_INSPECT_STAGE2_X_9, OnBtnInspectStage2XClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHK_INSPECT_STAGE2_IO_0, IDC_CHK_INSPECT_STAGE2_IO_9, OnChkInspectStage2IoClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_INSPECT_STAGE2_IO_0, IDC_BTN_INSPECT_STAGE2_IO_5, OnBtnInspectStage2IoClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_INSPECT_STAGE3_X_0, IDC_BTN_INSPECT_STAGE3_X_9, OnBtnInspectStage3XClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_CHK_INSPECT_STAGE3_IO_0, IDC_CHK_INSPECT_STAGE3_IO_9, OnChkInspectStage3IoClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_INSPECT_STAGE3_IO_0, IDC_BTN_INSPECT_STAGE3_IO_5, OnBtnInspectStage3IoClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_MODULE_ALIGN_Y_0, IDC_BTN_MODULE_ALIGN_Y_2, OnBtnModuleAlignYClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_MODULE_ALIGN_Z_0, IDC_BTN_MODULE_ALIGN_Z_1, OnBtnModuleAlignZClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_VISION_TOP1_Z_0, IDC_BTN_VISION_TOP1_Z_0, OnBtnVisionTop1ZClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_TOP1_MIRROR_Z_0, IDC_BTN_TOP1_MIRROR_Z_1, OnBtnTop1MirrorZClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_TOP1_MIRROR_IO_0, IDC_BTN_TOP1_MIRROR_IO_1, OnBtnTop1MirrorIoClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_TOP2_SHIFT_Y_0, IDC_BTN_TOP2_SHIFT_Y_1, OnBtnTop2ShiftYClick)
	ON_CONTROL_RANGE(BN_CLICKED, IDC_BTN_VISION_TOP2_Z_0, IDC_BTN_VISION_TOP2_Z_0, OnBtnVisionTop2ZClick)
END_MESSAGE_MAP()

// CManualBtm1Dlg 메시지 처리기입니다.
BOOL CManualBtm1Dlg::OnInitDialog() 
{
	CDialogEx::OnInitDialog();

	// TODO:  여기에 추가 초기화 작업을 추가합니다.
	SetWindowPos(this, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOZORDER);

	Initial_Controls();

	m_btnTop1MirrorIo[0].EnableWindow(FALSE);
	m_btnTop1MirrorIo[1].EnableWindow(FALSE);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}

BOOL CManualBtm1Dlg::PreTranslateMessage(MSG* pMsg) 
{
	if (pMsg->message == WM_KEYDOWN && (pMsg->wParam == VK_RETURN || pMsg->wParam == VK_ESCAPE))
		return TRUE;

	return CDialogEx::PreTranslateMessage(pMsg);
}

void CManualBtm1Dlg::OnShowWindow(BOOL bShow, UINT nStatus) 
{
	CDialogEx::OnShowWindow(bShow, nStatus);

	if (!bShow) return;

	Display_Status();

	m_btnBtm1PickerIo[8].EnableWindow(gData.bR04A);	// Normal
	m_btnBtm1PickerIo[9].EnableWindow(gData.bR04A);	// Rotate

#ifndef DRY_RUN_TEST
	for (int i = 0; i < 8; i++) m_btnBtm1PickerIo[i].EnableWindow(FALSE);	// Up/Down
	m_btnBtm1PickerIo[30].EnableWindow(FALSE);	// All Up
	m_btnBtm1PickerIo[31].EnableWindow(FALSE);	// All Down
#endif

	DY_DATA_05 *pDY05 = g_objAJinAXL.Get_pDY05();
	DY_DATA_06 *pDY06 = g_objAJinAXL.Get_pDY06();

	m_chkInspectStage1Io[0].SetCheck(pDY05->oInspectStage1Vac01);
	m_chkInspectStage1Io[1].SetCheck(pDY05->oInspectStage1Vac02);
	m_chkInspectStage1Io[2].SetCheck(pDY05->oInspectStage1Vac03);
	m_chkInspectStage1Io[3].SetCheck(pDY05->oInspectStage1Vac04);
	m_chkInspectStage1Io[4].SetCheck(pDY05->oInspectStage1Vac05);
	m_chkInspectStage1Io[5].SetCheck(pDY05->oInspectStage1Vac06);
	m_chkInspectStage1Io[6].SetCheck(pDY05->oInspectStage1Vac07);
	m_chkInspectStage1Io[7].SetCheck(pDY05->oInspectStage1Vac08);
	m_chkInspectStage1Io[8].SetCheck(pDY05->oInspectStage1Vac09);
	m_chkInspectStage1Io[9].SetCheck(pDY05->oInspectStage1Vac10);

	m_chkInspectStage2Io[0].SetCheck(pDY06->oInspectStage2Vac01);
	m_chkInspectStage2Io[1].SetCheck(pDY06->oInspectStage2Vac02);
	m_chkInspectStage2Io[2].SetCheck(pDY06->oInspectStage2Vac03);
	m_chkInspectStage2Io[3].SetCheck(pDY06->oInspectStage2Vac04);
	m_chkInspectStage2Io[4].SetCheck(pDY06->oInspectStage2Vac05);
	m_chkInspectStage2Io[5].SetCheck(pDY06->oInspectStage2Vac06);
	m_chkInspectStage2Io[6].SetCheck(pDY06->oInspectStage2Vac07);
	m_chkInspectStage2Io[7].SetCheck(pDY06->oInspectStage2Vac08);
	m_chkInspectStage2Io[8].SetCheck(pDY06->oInspectStage2Vac09);
	m_chkInspectStage2Io[9].SetCheck(pDY06->oInspectStage2Vac10);

	m_chkInspectStage3Io[0].SetCheck(pDY06->oInspectStage3Vac01);
	m_chkInspectStage3Io[1].SetCheck(pDY06->oInspectStage3Vac02);
	m_chkInspectStage3Io[2].SetCheck(pDY06->oInspectStage3Vac03);
	m_chkInspectStage3Io[3].SetCheck(pDY06->oInspectStage3Vac04);
	m_chkInspectStage3Io[4].SetCheck(pDY06->oInspectStage3Vac05);
	m_chkInspectStage3Io[5].SetCheck(pDY06->oInspectStage3Vac06);
	m_chkInspectStage3Io[6].SetCheck(pDY06->oInspectStage3Vac07);
	m_chkInspectStage3Io[7].SetCheck(pDY06->oInspectStage3Vac08);
	m_chkInspectStage3Io[8].SetCheck(pDY06->oInspectStage3Vac09);
	m_chkInspectStage3Io[9].SetCheck(pDY06->oInspectStage3Vac10);

	m_strLog.Format("[Manual Index] Show Window");
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualBtm1Dlg::Initial_Controls() 
{
	for (int i = 0; i < 5; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0xD0, 0x10, 0x10), COLOR_DEFAULT);
	for (int i = 5; i < 6; i++) {if(i==5)continue;m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0xD0, 0x10, 0xD0), COLOR_DEFAULT);}//btm shift y ui 삭제
	for (int i = 6; i < 12; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0x00, 0x00, 0xFF), COLOR_DEFAULT);
	for (int i = 12; i < 14; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0x00, 0x80, 0x40), COLOR_DEFAULT);
	for (int i = 14; i < 19; i++) m_Group[i].Init_Ctrl("Arial", 11, TRUE, RGB(0xD0, 0x10, 0xD0), COLOR_DEFAULT);
	for (int i = 0; i < 14; i++) {if(i==4)continue; m_Label[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, RGB(0xB0, 0xB0, 0xB0));}//btm shift y ui 삭제
	for (int i = 0; i < 14; i++)  {if(i==4)continue; m_stcAxisPos[i].Init_Ctrl("Arial", 10, TRUE, RGB(0xFF, 0xFF, 0xFF), RGB(0x10, 0x10, 0x60));}//btm shift y ui 삭제
	for (int i = 0; i <  8; i++) m_btnBtm1PickerX[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  4; i++) m_btnBtm1PickerZ[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  3; i++) m_btnBtm1PickerP1[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  3; i++) m_btnBtm1PickerP2[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 34; i++) m_btnBtm1PickerIo[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 30; i++) m_ledBtm1PickerIo[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	
	for (int i = 0; i < 10; i++) m_btnInspectStage1X[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 10; i++) m_chkInspectStage1Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  4; i++) m_btnInspectStage1Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 12; i++) m_ledInspectStage1Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i < 10; i++) m_btnInspectStage2X[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 10; i++) m_chkInspectStage2Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  6; i++) m_btnInspectStage2Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 14; i++) m_ledInspectStage2Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i < 10; i++) m_btnInspectStage3X[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 10; i++) m_chkInspectStage3Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  6; i++) m_btnInspectStage3Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i < 14; i++) m_ledInspectStage3Io[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i <  3; i++) m_btnModuleAlignY[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  2; i++) m_btnModuleAlignZ[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  1; i++) m_btnVisionTop1Z[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  2; i++) m_btnTop1MirrorZ[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  2; i++) m_btnTop1MirrorIo[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
	for (int i = 0; i <  4; i++) m_ledTop1MirrorIo[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, CLedCS::emGreen, CLedCS::em16);
	for (int i = 0; i <  1; i++) m_btnVisionTop2Z[i].Init_Ctrl("Arial", 10, FALSE, COLOR_DEFAULT, COLOR_DEFAULT, 0, 0);
}

void CManualBtm1Dlg::Display_Status()
{
	CString strPos;
	for (int i = 0; i < 14; i++) 
	{
		if(i==4)continue; //btm 1 shift y remove
		double dPos = g_objAJinAXL.Get_Position(AX_BTM1_PICKER_X + i);
		strPos.Format("%0.3lf", dPos);
		m_stcAxisPos[i].SetWindowText(strPos);
	}

	DX_DATA_03 *pDX03 = g_objAJinAXL.Get_pDX03();
	DX_DATA_04 *pDX04 = g_objAJinAXL.Get_pDX04();
	DX_DATA_05 *pDX05 = g_objAJinAXL.Get_pDX05();
	DX_DATA_06 *pDX06 = g_objAJinAXL.Get_pDX06();

	m_ledBtm1PickerIo[0].Set_On(pDX03->iBtm1PickerNor01);
	m_ledBtm1PickerIo[1].Set_On(pDX03->iBtm1PickerNor02);
	m_ledBtm1PickerIo[2].Set_On(pDX03->iBtm1PickerNor03);
	m_ledBtm1PickerIo[3].Set_On(pDX03->iBtm1PickerNor04);
	m_ledBtm1PickerIo[4].Set_On(pDX03->iBtm1PickerNor05);
	m_ledBtm1PickerIo[5].Set_On(pDX03->iBtm1PickerNor06);
	m_ledBtm1PickerIo[6].Set_On(pDX03->iBtm1PickerNor07);
	m_ledBtm1PickerIo[7].Set_On(pDX03->iBtm1PickerNor08);
	m_ledBtm1PickerIo[8].Set_On(pDX03->iBtm1PickerNor09);
	m_ledBtm1PickerIo[9].Set_On(pDX03->iBtm1PickerNor10);

	m_ledBtm1PickerIo[10].Set_On(pDX03->iBtm1PickerRot01);
	m_ledBtm1PickerIo[11].Set_On(pDX03->iBtm1PickerRot02);
	m_ledBtm1PickerIo[12].Set_On(pDX03->iBtm1PickerRot03);
	m_ledBtm1PickerIo[13].Set_On(pDX03->iBtm1PickerRot04);
	m_ledBtm1PickerIo[14].Set_On(pDX03->iBtm1PickerRot05);
	m_ledBtm1PickerIo[15].Set_On(pDX03->iBtm1PickerRot06);
	m_ledBtm1PickerIo[16].Set_On(pDX03->iBtm1PickerRot07);
	m_ledBtm1PickerIo[17].Set_On(pDX03->iBtm1PickerRot08);
	m_ledBtm1PickerIo[18].Set_On(pDX03->iBtm1PickerRot09);
	m_ledBtm1PickerIo[19].Set_On(pDX03->iBtm1PickerRot10);

	m_ledBtm1PickerIo[20].Set_On(pDX04->iBtm1PickerVac01);
	m_ledBtm1PickerIo[21].Set_On(pDX04->iBtm1PickerVac02);
	m_ledBtm1PickerIo[22].Set_On(pDX04->iBtm1PickerVac03);
	m_ledBtm1PickerIo[23].Set_On(pDX04->iBtm1PickerVac04);
	m_ledBtm1PickerIo[24].Set_On(pDX04->iBtm1PickerVac05);
	m_ledBtm1PickerIo[25].Set_On(pDX04->iBtm1PickerVac06);
	m_ledBtm1PickerIo[26].Set_On(pDX04->iBtm1PickerVac07);
	m_ledBtm1PickerIo[27].Set_On(pDX04->iBtm1PickerVac08);
	m_ledBtm1PickerIo[28].Set_On(pDX04->iBtm1PickerVac09);
	m_ledBtm1PickerIo[29].Set_On(pDX04->iBtm1PickerVac10);

	m_ledInspectStage1Io[0].Set_On(pDX05->iInspectStage1Vac01);
	m_ledInspectStage1Io[1].Set_On(pDX05->iInspectStage1Vac02);
	m_ledInspectStage1Io[2].Set_On(pDX05->iInspectStage1Vac03);
	m_ledInspectStage1Io[3].Set_On(pDX05->iInspectStage1Vac04);
	m_ledInspectStage1Io[4].Set_On(pDX05->iInspectStage1Vac05);
	m_ledInspectStage1Io[5].Set_On(pDX05->iInspectStage1Vac06);
	m_ledInspectStage1Io[6].Set_On(pDX05->iInspectStage1Vac07);
	m_ledInspectStage1Io[7].Set_On(pDX05->iInspectStage1Vac08);
	m_ledInspectStage1Io[8].Set_On(pDX05->iInspectStage1Vac09);
	m_ledInspectStage1Io[9].Set_On(pDX05->iInspectStage1Vac10);
	m_ledInspectStage1Io[10].Set_On(pDX05->iInspectStage1Up);
	m_ledInspectStage1Io[11].Set_On(pDX05->iInspectStage1Down);

	m_ledInspectStage2Io[0].Set_On(pDX06->iInspectStage2Vac01);
	m_ledInspectStage2Io[1].Set_On(pDX06->iInspectStage2Vac02);
	m_ledInspectStage2Io[2].Set_On(pDX06->iInspectStage2Vac03);
	m_ledInspectStage2Io[3].Set_On(pDX06->iInspectStage2Vac04);
	m_ledInspectStage2Io[4].Set_On(pDX06->iInspectStage2Vac05);
	m_ledInspectStage2Io[5].Set_On(pDX06->iInspectStage2Vac06);
	m_ledInspectStage2Io[6].Set_On(pDX06->iInspectStage2Vac07);
	m_ledInspectStage2Io[7].Set_On(pDX06->iInspectStage2Vac08);
	m_ledInspectStage2Io[8].Set_On(pDX06->iInspectStage2Vac09);
	m_ledInspectStage2Io[9].Set_On(pDX06->iInspectStage2Vac10);
	m_ledInspectStage2Io[10].Set_On(pDX05->iInspectStage2Up);
	m_ledInspectStage2Io[11].Set_On(pDX05->iInspectStage2Down);
	m_ledInspectStage2Io[12].Set_On(pDX05->iInspectStage2Fwd);
	m_ledInspectStage2Io[13].Set_On(pDX05->iInspectStage2Bwd);

	m_ledInspectStage3Io[0].Set_On(pDX06->iInspectStage3Vac01);
	m_ledInspectStage3Io[1].Set_On(pDX06->iInspectStage3Vac02);
	m_ledInspectStage3Io[2].Set_On(pDX06->iInspectStage3Vac03);
	m_ledInspectStage3Io[3].Set_On(pDX06->iInspectStage3Vac04);
	m_ledInspectStage3Io[4].Set_On(pDX06->iInspectStage3Vac05);
	m_ledInspectStage3Io[5].Set_On(pDX06->iInspectStage3Vac06);
	m_ledInspectStage3Io[6].Set_On(pDX06->iInspectStage3Vac07);
	m_ledInspectStage3Io[7].Set_On(pDX06->iInspectStage3Vac08);
	m_ledInspectStage3Io[8].Set_On(pDX06->iInspectStage3Vac09);
	m_ledInspectStage3Io[9].Set_On(pDX06->iInspectStage3Vac10);
	m_ledInspectStage3Io[10].Set_On(pDX05->iInspectStage3Up);
	m_ledInspectStage3Io[11].Set_On(pDX05->iInspectStage3Down);
	m_ledInspectStage3Io[12].Set_On(pDX05->iInspectStage3Fwd);
	m_ledInspectStage3Io[13].Set_On(pDX05->iInspectStage3Bwd);

	m_ledTop1MirrorIo[0].Set_On(pDX06->iTop1Mirror1Up);
	m_ledTop1MirrorIo[1].Set_On(pDX06->iTop1Mirror1Down);
	m_ledTop1MirrorIo[2].Set_On(pDX06->iTop1Mirror2Up);
	m_ledTop1MirrorIo[3].Set_On(pDX06->iTop1Mirror2Down);
}

///////////////////////////////////////////////////////////////////////////////

void CManualBtm1Dlg::OnBtnBtm1PickerXClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_BTM1_PICKER_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_BTM1_PICKER_Z)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_BTM1_PICKER_X_0;

	if (!g_objCommon.Check_Position(AX_BTM1_PICKER_Z, 0)) { AfxMessageBox("Btm1 Picker Z축 Ready Up 위치 확인 후 진행하세요."); return; }

	if (nIndex == 6 || nIndex == 7) {	// Pitch Move
		EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
		int nDir = (nIndex == 6 ? -1 : 1);
		double dMove = pEquipData->dTrayPitchX * nDir;
		if (gData.bR04A || gData.bR13A) dMove = pEquipData->dTrayPitchY * nDir;
		g_objAJinAXL.Move_Relative(AX_BTM1_PICKER_X, dMove);
	} else {
		g_objCommon.Move_Position(AX_BTM1_PICKER_X, nIndex);
	}

	m_strLog.Format("[Manual Btm1] Btm1 Picker X (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualBtm1Dlg::OnBtnBtm1PickerZClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_BTM1_PICKER_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_BTM1_PICKER_X)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_BTM1_PICKER_Z_0;

	int nPosX = -1;
	for (int i = 0; i < 6; i++) { if (g_objCommon.Check_Position(AX_BTM1_PICKER_X, i)) { nPosX = i; break; } }

	if (nIndex == 1 && nPosX >= 4) { AfxMessageBox("Btm1 Picker X축이 Load 위치인지 확인 후 진행하세요."); return; }
	if (nIndex == 2 && nPosX != 4) { AfxMessageBox("Btm1 Picker X축이 BTM1 위치인지 확인 후 진행하세요."); return; }
	if (nIndex == 3 && nPosX != 5) { AfxMessageBox("Btm1 Picker X축이 Unload 위치인지 확인 후 진행하세요."); return; }

	g_objCommon.Move_Position(AX_BTM1_PICKER_Z, nIndex);

	m_strLog.Format("[Manual Btm1] Btm1 Picker Z (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualBtm1Dlg::OnBtnBtm1PickerP1Click(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_BTM1_PICKER_P1)) return;
	if (!g_objAJinAXL.Is_Home(AX_BTM1_PICKER_Z)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_BTM1_PICKER_P1_0;

	if (!g_objCommon.Check_Position(AX_BTM1_PICKER_Z, 0)) { AfxMessageBox("Btm1 Picker Z축 Ready Up 위치 확인 후 진행하세요."); return; }

	if (nIndex == 2) {
		g_objAJinAXL.Move_Absolute(AX_BTM1_PICKER_P1, -2.5);
	} else {
		if (!gData.bR04A && !g_objCommon.Get_Btm1PickerNormal()) { AfxMessageBox("Btm1 Picker Normal 상태 확인 후 진행하세요."); return; }
		g_objCommon.Move_Position(AX_BTM1_PICKER_P1, nIndex);
	}

	m_strLog.Format("[Manual Btm1] Btm1 Picker P1 (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualBtm1Dlg::OnBtnBtm1PickerP2Click(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_BTM1_PICKER_P2)) return;
	if (!g_objAJinAXL.Is_Home(AX_BTM1_PICKER_Z)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_BTM1_PICKER_P2_0;

	if (!g_objCommon.Check_Position(AX_BTM1_PICKER_Z, 0)) { AfxMessageBox("Btm1 Picker Z축 Ready Up 위치 확인 후 진행하세요."); return; }

	if (nIndex == 2) {
		g_objAJinAXL.Move_Absolute(AX_BTM1_PICKER_P2, -2.5);
	} else {
		if (!gData.bR04A && !g_objCommon.Get_Btm1PickerNormal()) { AfxMessageBox("Btm1 Picker Normal 상태 확인 후 진행하세요."); return; }
		g_objCommon.Move_Position(AX_BTM1_PICKER_P2, nIndex);
	}

	m_strLog.Format("[Manual Btm1] Btm1 Picker P2 (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualBtm1Dlg::OnBtnBtm1PickerIoClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;

	int nIndex = nID - IDC_BTN_BTM1_PICKER_IO_0;

	DY_DATA_03 *pDY03 = g_objAJinAXL.Get_pDY03();
	DY_DATA_04 *pDY04 = g_objAJinAXL.Get_pDY04();

#ifdef EDITION_2ND
	if (nIndex == 0 && pDY03->oBtm1PickerDown05) { AfxMessageBox("05번 Up 확인 후 진행하여 주십시오."); return; }
	if (nIndex == 2 && pDY04->oBtm1PickerDown10) { AfxMessageBox("10번 Up 확인 후 진행하여 주십시오."); return; }
	if (nIndex == 5 && !pDY03->oBtm1PickerDown01) { AfxMessageBox("01번 Down 확인 후 진행하여 주십시오."); return; }
	if (nIndex == 7 && !pDY04->oBtm1PickerDown06) { AfxMessageBox("06번 Down 확인 후 진행하여 주십시오."); return; }

	if (nIndex == 30 || nIndex == 0) pDY03->oBtm1PickerDown01 = pDY03->oBtm1PickerDown02 = pDY03->oBtm1PickerDown03 = pDY03->oBtm1PickerDown04 = FALSE;		// Up
	if (nIndex == 30 || nIndex == 1) pDY03->oBtm1PickerDown05 = FALSE;
	if (nIndex == 30 || nIndex == 2) pDY04->oBtm1PickerDown06 = pDY04->oBtm1PickerDown07 = pDY04->oBtm1PickerDown08 = pDY04->oBtm1PickerDown09 = FALSE;
	if (nIndex == 30 || nIndex == 3) pDY04->oBtm1PickerDown10 = FALSE;

	if (nIndex == 31 || nIndex == 4) pDY03->oBtm1PickerDown01 = pDY03->oBtm1PickerDown02 = pDY03->oBtm1PickerDown03 = pDY03->oBtm1PickerDown04 = TRUE;		// Down
	if (nIndex == 31 || nIndex == 5) pDY03->oBtm1PickerDown05 = TRUE;
	if (nIndex == 31 || nIndex == 6) pDY04->oBtm1PickerDown06 = pDY04->oBtm1PickerDown07 = pDY04->oBtm1PickerDown08 = pDY04->oBtm1PickerDown09 = TRUE;
	if (nIndex == 31 || nIndex == 7) pDY04->oBtm1PickerDown10 = TRUE;
#else
	if (nIndex == 0 && pDY03->oBtm1PickerDown02) { AfxMessageBox("02번, 04번 Up 확인 후 진행하여 주십시오."); return; }
	if (nIndex == 2 && pDY03->oBtm1PickerDown07) { AfxMessageBox("07번, 09번 Up 확인 후 진행하여 주십시오."); return; }
	if (nIndex == 5 && !pDY03->oBtm1PickerDown01) { AfxMessageBox("01번 Down 확인 후 진행하여 주십시오."); return; }
	if (nIndex == 7 && !pDY03->oBtm1PickerDown06) { AfxMessageBox("06번 Down 확인 후 진행하여 주십시오."); return; }

	if (nIndex == 30 || nIndex == 0) pDY03->oBtm1PickerDown01 = FALSE;		// Up
	if (nIndex == 30 || nIndex == 1) pDY03->oBtm1PickerDown02 = FALSE;
	if (nIndex == 30 || nIndex == 2) pDY03->oBtm1PickerDown06 = FALSE;
	if (nIndex == 30 || nIndex == 3) pDY03->oBtm1PickerDown07 = FALSE;

	if (nIndex == 31 || nIndex == 4) pDY03->oBtm1PickerDown01 = TRUE;		// Down
	if (nIndex == 31 || nIndex == 5) pDY03->oBtm1PickerDown02 = TRUE;
	if (nIndex == 31 || nIndex == 6) pDY03->oBtm1PickerDown06 = TRUE;
	if (nIndex == 31 || nIndex == 7) pDY03->oBtm1PickerDown07 = TRUE;
#endif

	if (nIndex == 8 || nIndex == 9) {
		if (!g_objAJinAXL.Is_Home(AX_BTM1_PICKER_P1)) return;
		if (!g_objAJinAXL.Is_Home(AX_BTM1_PICKER_P2)) return;
		if (g_objAJinAXL.Is_MoveDone(AX_BTM1_PICKER_P1, -2.5) && g_objAJinAXL.Is_MoveDone(AX_BTM1_PICKER_P2, -2.5)) {
#ifdef EDITION_2ND
			if (nIndex == 8) {		// Ready
				pDY03->oBtm1PickerReadyL1 = TRUE; pDY03->oBtm1PickerTurnL1 = FALSE;
				pDY03->oBtm1PickerReadyL2 = TRUE; pDY03->oBtm1PickerTurnL2 = FALSE;
				pDY03->oBtm1PickerReadyL3 = TRUE; pDY03->oBtm1PickerTurnL3 = FALSE;
				pDY03->oBtm1PickerReadyL4 = TRUE; pDY03->oBtm1PickerTurnL4 = FALSE;
				pDY03->oBtm1PickerReadyL5 = TRUE; pDY03->oBtm1PickerTurnL5 = FALSE;
				pDY03->oBtm1PickerReadyL6 = TRUE; pDY03->oBtm1PickerTurnL6 = FALSE;
				pDY03->oBtm1PickerReadyL7 = TRUE; pDY03->oBtm1PickerTurnL7 = FALSE;
				pDY03->oBtm1PickerReadyL8 = TRUE; pDY03->oBtm1PickerTurnL8 = FALSE;
				pDY03->oBtm1PickerReadyL9 = TRUE; pDY03->oBtm1PickerTurnL9 = FALSE;
				pDY03->oBtm1PickerReadyL10 = TRUE; pDY03->oBtm1PickerTurnL10 = FALSE;
				
			}
			if (nIndex == 9) {		// Turn
				pDY03->oBtm1PickerReadyL1 = FALSE; pDY03->oBtm1PickerTurnL1 = TRUE;
				pDY03->oBtm1PickerReadyL2 = FALSE; pDY03->oBtm1PickerTurnL2 = TRUE;
				pDY03->oBtm1PickerReadyL3 = FALSE; pDY03->oBtm1PickerTurnL3 = TRUE;
				pDY03->oBtm1PickerReadyL4 = FALSE; pDY03->oBtm1PickerTurnL4 = TRUE;
				pDY03->oBtm1PickerReadyL5 = FALSE; pDY03->oBtm1PickerTurnL5 = TRUE;
				pDY03->oBtm1PickerReadyL6 = FALSE; pDY03->oBtm1PickerTurnL6 = TRUE;
				pDY03->oBtm1PickerReadyL7 = FALSE; pDY03->oBtm1PickerTurnL7 = TRUE;
				pDY03->oBtm1PickerReadyL8 = FALSE; pDY03->oBtm1PickerTurnL8 = TRUE;
				pDY03->oBtm1PickerReadyL9 = FALSE; pDY03->oBtm1PickerTurnL9 = TRUE;
				pDY03->oBtm1PickerReadyL10 = FALSE; pDY03->oBtm1PickerTurnL10 = TRUE;
				
			}
#else
			if (nIndex == 8) {		// Ready
				pDY03->oBtm1PickerTurn01 = FALSE; pDY03->oBtm1PickerTurn06 = FALSE;
				pDY03->oBtm1PickerTurn23 = FALSE; pDY03->oBtm1PickerTurn78 = FALSE;
				pDY03->oBtm1PickerTurn45 = FALSE; pDY03->oBtm1PickerTurn910 = FALSE;
			}
			if (nIndex == 9) {		// Turn
				pDY03->oBtm1PickerTurn01 = TRUE; pDY03->oBtm1PickerTurn06 = TRUE;
				pDY03->oBtm1PickerTurn23 = TRUE; pDY03->oBtm1PickerTurn78 = TRUE;
				pDY03->oBtm1PickerTurn45 = TRUE; pDY03->oBtm1PickerTurn910 = TRUE;
			}
#endif
		} else {
			AfxMessageBox("P1축과 P2축이 Zero 위치인지 확인 후 진행하여 주십시오."); return;
		}
	}

	if (nIndex == 32 || nIndex == 10) { pDY04->oBtm1PickerVac01 = TRUE; pDY04->oBtm1PickerAir01 = FALSE; }	// Vac On
	if (nIndex == 32 || nIndex == 11) { pDY04->oBtm1PickerVac02 = TRUE; pDY04->oBtm1PickerAir02 = FALSE; }
	if (nIndex == 32 || nIndex == 12) { pDY04->oBtm1PickerVac03 = TRUE; pDY04->oBtm1PickerAir03 = FALSE; }
	if (nIndex == 32 || nIndex == 13) { pDY04->oBtm1PickerVac04 = TRUE; pDY04->oBtm1PickerAir04 = FALSE; }
	if (nIndex == 32 || nIndex == 14) { pDY04->oBtm1PickerVac05 = TRUE; pDY04->oBtm1PickerAir05 = FALSE; }
	if (nIndex == 32 || nIndex == 15) { pDY04->oBtm1PickerVac06 = TRUE; pDY04->oBtm1PickerAir06 = FALSE; }
	if (nIndex == 32 || nIndex == 16) { pDY04->oBtm1PickerVac07 = TRUE; pDY04->oBtm1PickerAir07 = FALSE; }
	if (nIndex == 32 || nIndex == 17) { pDY04->oBtm1PickerVac08 = TRUE; pDY04->oBtm1PickerAir08 = FALSE; }
	if (nIndex == 32 || nIndex == 18) { pDY04->oBtm1PickerVac09 = TRUE; pDY04->oBtm1PickerAir09 = FALSE; }
	if (nIndex == 32 || nIndex == 19) { pDY04->oBtm1PickerVac10 = TRUE; pDY04->oBtm1PickerAir10 = FALSE; }

	if (nIndex == 33 || nIndex == 20) { pDY04->oBtm1PickerVac01 = FALSE; pDY04->oBtm1PickerAir01 = TRUE; }	// Vac Off
	if (nIndex == 33 || nIndex == 21) { pDY04->oBtm1PickerVac02 = FALSE; pDY04->oBtm1PickerAir02 = TRUE; }
	if (nIndex == 33 || nIndex == 22) { pDY04->oBtm1PickerVac03 = FALSE; pDY04->oBtm1PickerAir03 = TRUE; }
	if (nIndex == 33 || nIndex == 23) { pDY04->oBtm1PickerVac04 = FALSE; pDY04->oBtm1PickerAir04 = TRUE; }
	if (nIndex == 33 || nIndex == 24) { pDY04->oBtm1PickerVac05 = FALSE; pDY04->oBtm1PickerAir05 = TRUE; }
	if (nIndex == 33 || nIndex == 25) { pDY04->oBtm1PickerVac06 = FALSE; pDY04->oBtm1PickerAir06 = TRUE; }
	if (nIndex == 33 || nIndex == 26) { pDY04->oBtm1PickerVac07 = FALSE; pDY04->oBtm1PickerAir07 = TRUE; }
	if (nIndex == 33 || nIndex == 27) { pDY04->oBtm1PickerVac08 = FALSE; pDY04->oBtm1PickerAir08 = TRUE; }
	if (nIndex == 33 || nIndex == 28) { pDY04->oBtm1PickerVac09 = FALSE; pDY04->oBtm1PickerAir09 = TRUE; }
	if (nIndex == 33 || nIndex == 29) { pDY04->oBtm1PickerVac10 = FALSE; pDY04->oBtm1PickerAir10 = TRUE; }

	if (nIndex == 33 || (nIndex >= 20 && nIndex < 30)) { g_objAJinAXL.Write_Output(4); Sleep(500); }

	if (nIndex == 33 || nIndex == 20) pDY04->oBtm1PickerAir01 = FALSE;		// Air Off
	if (nIndex == 33 || nIndex == 21) pDY04->oBtm1PickerAir02 = FALSE;
	if (nIndex == 33 || nIndex == 22) pDY04->oBtm1PickerAir03 = FALSE;
	if (nIndex == 33 || nIndex == 23) pDY04->oBtm1PickerAir04 = FALSE;
	if (nIndex == 33 || nIndex == 24) pDY04->oBtm1PickerAir05 = FALSE;
	if (nIndex == 33 || nIndex == 25) pDY04->oBtm1PickerAir06 = FALSE;
	if (nIndex == 33 || nIndex == 26) pDY04->oBtm1PickerAir07 = FALSE;
	if (nIndex == 33 || nIndex == 27) pDY04->oBtm1PickerAir08 = FALSE;
	if (nIndex == 33 || nIndex == 28) pDY04->oBtm1PickerAir09 = FALSE;
	if (nIndex == 33 || nIndex == 29) pDY04->oBtm1PickerAir10 = FALSE;

	g_objAJinAXL.Write_Output(3);
	g_objAJinAXL.Write_Output(4);

	m_strLog.Format("[Manual Btm1] Btm1 Picker IO (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}


void CManualBtm1Dlg::OnBtnInspectStage1XClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_INSPECT_STAGE1_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_INSPECT_STAGE2_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_INSPECT_STAGE3_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_BTM1_PICKER_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_BTM2_PICKER_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_MODULE_ALIGN_Y)) return;
	if (!g_objAJinAXL.Is_Home(AX_MODULE_ALIGN_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_TOP1_MIRROR_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_TOP2_SHIFT_Y)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_INSPECT_STAGE1_X_0;

	DX_DATA_05 *pDX05 = g_objAJinAXL.Get_pDX05();
	if (pDX05->iInspectStage1Up && !pDX05->iInspectStage1Down) {
		if (pDX05->iInspectStage2Up || !pDX05->iInspectStage2Down) { AfxMessageBox("Inspect Stage2 Down 상태를 확인 후 진행하세요."); return; }
		if (pDX05->iInspectStage3Up || !pDX05->iInspectStage3Down) { AfxMessageBox("Inspect Stage3 Down 상태를 확인 후 진행하세요."); return; }
		if (!g_objCommon.Check_Position(AX_MODULE_ALIGN_Y, 0)) { AfxMessageBox("Module Align Y축 Center 위치 확인 후 진행하세요."); return; }
		if (!g_objCommon.Check_Position(AX_MODULE_ALIGN_Z, 0)) { AfxMessageBox("Module Align Z축 Ready Up 위치 확인 후 진행하세요."); return; }
		if (!g_objCommon.Check_Position(AX_TOP1_MIRROR_Z, 0)) { AfxMessageBox("Top1 Mirror Z축 Ready Up 위치 확인 후 진행하세요."); return; }
		if (!g_objCommon.Check_Position(AX_TOP2_SHIFT_Y, 0)) { AfxMessageBox("Top2 Shift Y축 Ready 위치 확인 후 진행하세요."); return; }

		if (g_objCommon.Check_Position(AX_BTM1_PICKER_X, 2) && !g_objCommon.Check_Position(AX_BTM1_PICKER_Z, 0)) {
			AfxMessageBox("Btm1 Picker X축 Z축 위치 확인 후 진행하세요."); return;
		}

		if (g_objCommon.Check_Position(AX_BTM2_PICKER_X, 0) && !g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 0)) {
			AfxMessageBox("Btm1 Picker X축 Z축 위치 확인 후 진행하세요."); return;
		}

	} else if (!pDX05->iInspectStage1Up && pDX05->iInspectStage1Down) {
		if (!pDX05->iInspectStage2Up || pDX05->iInspectStage2Down) { AfxMessageBox("Inspect Stage2 Up 상태를 확인 후 진행하세요."); return; }
		if (!pDX05->iInspectStage3Up || pDX05->iInspectStage3Down) { AfxMessageBox("Inspect Stage3 Up 상태를 확인 후 진행하세요."); return; }

	} else {
		AfxMessageBox("Inspect Stage1 Up 또는 Down 상태를 확인 후 진행하세요."); return;
	}

	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	MOVE_DATA *pMoveData = g_objDataManager.Get_pMoveData();

	if (nIndex == 2 || nIndex == 3) {			// AL Fwd, AL Bwd
		double dBack = (nIndex == 3 ? pEquipData->dInspectALBackOffset : 0.0);
		double dMove = pMoveData->dInspectStage1X[1] + pEquipData->dInspectALOffset - dBack;
		g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE1_X, dMove);

	} else if (nIndex == 7 || nIndex == 8) {	// Pitch Move
		int nDir = (nIndex == 7 ? -1 : 1);
		double dMove = pEquipData->dInspectPitchX * nDir;
		g_objAJinAXL.Move_Relative(AX_INSPECT_STAGE1_X, dMove);

	} else if (nIndex == 0 || nIndex == 1 || nIndex == 4 || nIndex == 5 || nIndex == 6) {	// Load, Align, Top1, Top2, Unload
		int nIdx = (nIndex > 1 ? (nIndex - 2) : nIndex);
		g_objCommon.Move_Position(AX_INSPECT_STAGE1_X, nIdx);

	} else if (nIndex == 9) {
		double dMove = pEquipData->dInspectTop2ShiftX;
		g_objAJinAXL.Move_Relative(AX_INSPECT_STAGE1_X, dMove);
	}

	m_strLog.Format("[Manual Btm1] Inspect Stage1 X (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualBtm1Dlg::OnChkInspectStage1IoClick(UINT nID)
{
// 	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_CHK_INSPECT_STAGE1_IO_0;

	BOOL bOn = m_chkInspectStage1Io[nIndex].GetCheck();
	DY_DATA_05 *pDY05 = g_objAJinAXL.Get_pDY05();

	if (nIndex == 0) pDY05->oInspectStage1Vac01 = bOn;		// Vac On/Off
	if (nIndex == 1) pDY05->oInspectStage1Vac02 = bOn;
	if (nIndex == 2) pDY05->oInspectStage1Vac03 = bOn;
	if (nIndex == 3) pDY05->oInspectStage1Vac04 = bOn;
	if (nIndex == 4) pDY05->oInspectStage1Vac05 = bOn;
	if (nIndex == 5) pDY05->oInspectStage1Vac06 = bOn;
	if (nIndex == 6) pDY05->oInspectStage1Vac07 = bOn;
	if (nIndex == 7) pDY05->oInspectStage1Vac08 = bOn;
	if (nIndex == 8) pDY05->oInspectStage1Vac09 = bOn;
	if (nIndex == 9) pDY05->oInspectStage1Vac10 = bOn;

	g_objAJinAXL.Write_Output(5);

	m_strLog.Format("[Manual Btm1] Inspect Stage1 Vac Click (%d, %d)", nIndex, (int)bOn);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualBtm1Dlg::OnBtnInspectStage1IoClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;

	int nIndex = nID - IDC_BTN_INSPECT_STAGE1_IO_0;

	if (nIndex == 2 || nIndex == 3) {	// Up / Down
// 		int nPosX1 = -1, nPosX2 = -1, nPosX3 = -1;
// 		for (int i = 0; i < 5; i++) { if (g_objCommon.Check_Position(AX_INSPECT_STAGE1_X, i)) { nPosX1 = i; break; } }
// 		for (int i = 0; i < 5; i++) { if (g_objCommon.Check_Position(AX_INSPECT_STAGE2_X, i)) { nPosX2 = i; break; } }
// 		for (int i = 0; i < 5; i++) { if (g_objCommon.Check_Position(AX_INSPECT_STAGE3_X, i)) { nPosX3 = i; break; } }
// 		if (nPosX1 == 0 && (nPosX2 == 0 || nPosX3 == 0)) { AfxMessageBox("Inspect Stage 1/2/3 X축 위치 확인 후 진행하세요."); return; }
// 		if (nPosX1 == 4 && (nPosX2 == 4 || nPosX3 == 4)) { AfxMessageBox("Inspect Stage 1/2/3 X축 위치 확인 후 진행하세요."); return; }
		double dX1 = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE1_X);
		double dX2 = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE2_X);
		double dX3 = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE3_X);
		if (fabs(dX1 - dX2) < 300.0) { AfxMessageBox("Inspect Stage 2 X축 위치 확인 후 진행하세요."); return; }
		if (fabs(dX1 - dX3) < 300.0) { AfxMessageBox("Inspect Stage 3 X축 위치 확인 후 진행하세요."); return; }
	}

	DY_DATA_05 *pDY05 = g_objAJinAXL.Get_pDY05();
	if (nIndex == 0) pDY05->nValue |= (0x3FF << 16);	// Vac On All
	if (nIndex == 1) pDY05->nValue &= ~(0x3FF << 16);	// Vac Off All
	if (nIndex == 2) { pDY05->oInspectStage1Up = TRUE;  pDY05->oInspectStage1Down = FALSE; }	// Up
	if (nIndex == 3) { pDY05->oInspectStage1Up = FALSE; pDY05->oInspectStage1Down = TRUE; }		// Down

	if (nIndex == 0) { for (int i = 0; i < 10; i++) m_chkInspectStage1Io[i].SetCheck(TRUE); }
	if (nIndex == 1) { for (int i = 0; i < 10; i++) m_chkInspectStage1Io[i].SetCheck(FALSE); }

	g_objAJinAXL.Write_Output(5);
	m_strLog.Format("[Manual Btm1] Inspect Stage1 Io (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualBtm1Dlg::OnBtnInspectStage2XClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_INSPECT_STAGE2_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_INSPECT_STAGE3_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_INSPECT_STAGE1_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_BTM1_PICKER_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_BTM2_PICKER_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_MODULE_ALIGN_Y)) return;
	if (!g_objAJinAXL.Is_Home(AX_MODULE_ALIGN_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_TOP1_MIRROR_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_TOP2_SHIFT_Y)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_INSPECT_STAGE2_X_0;

	DX_DATA_05 *pDX05 = g_objAJinAXL.Get_pDX05();
	if (pDX05->iInspectStage2Up && !pDX05->iInspectStage2Down) {
		if (pDX05->iInspectStage1Up || !pDX05->iInspectStage1Down) { AfxMessageBox("Inspect Stage1 Down 상태를 확인 후 진행하세요."); return; }
		if (pDX05->iInspectStage3Up || !pDX05->iInspectStage3Down) { AfxMessageBox("Inspect Stage3 Down 상태를 확인 후 진행하세요."); return; }
		if (!g_objCommon.Check_Position(AX_MODULE_ALIGN_Y, 0)) { AfxMessageBox("Module Align Y축 Center 위치 확인 후 진행하세요."); return; }
		if (!g_objCommon.Check_Position(AX_MODULE_ALIGN_Z, 0)) { AfxMessageBox("Module Align Z축 Ready Up 위치 확인 후 진행하세요."); return; }
		if (!g_objCommon.Check_Position(AX_TOP1_MIRROR_Z, 0)) { AfxMessageBox("Top1 Mirror Z축 Ready Up 위치 확인 후 진행하세요."); return; }
		if (!g_objCommon.Check_Position(AX_TOP2_SHIFT_Y, 0)) { AfxMessageBox("Top2 Shift Y축 Ready 위치 확인 후 진행하세요."); return; }

		if (g_objCommon.Check_Position(AX_BTM1_PICKER_X, 2) && !g_objCommon.Check_Position(AX_BTM1_PICKER_Z, 0)) {
			AfxMessageBox("Btm1 Picker X축 Z축 위치 확인 후 진행하세요."); return;
		}

		if (g_objCommon.Check_Position(AX_BTM2_PICKER_X, 0) && !g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 0)) {
			AfxMessageBox("Btm1 Picker X축 Z축 위치 확인 후 진행하세요."); return;
		}

	} else if (!pDX05->iInspectStage2Up && pDX05->iInspectStage2Down) {
		if (!pDX05->iInspectStage1Up || pDX05->iInspectStage1Down) { AfxMessageBox("Inspect Stage1 Up 상태를 확인 후 진행하세요."); return; }
		if (!pDX05->iInspectStage3Up || pDX05->iInspectStage3Down) { AfxMessageBox("Inspect Stage3 Up 상태를 확인 후 진행하세요."); return; }
		if (pDX05->iInspectStage2Fwd || !pDX05->iInspectStage2Bwd) { AfxMessageBox("Inspect Stage2 Bwd 상태를 확인 후 진행하세요."); return; }

	} else {
		AfxMessageBox("Inspect Stage2 Up 또는 Down 상태를 확인 후 진행하세요."); return;
	}

	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	MOVE_DATA *pMoveData = g_objDataManager.Get_pMoveData();

	if (nIndex == 2 || nIndex == 3) {			// AL Fwd, AL Bwd
		double dBack = (nIndex == 3 ? pEquipData->dInspectALBackOffset : 0.0);
		double dMove = pMoveData->dInspectStage2X[1] + pEquipData->dInspectALOffset - dBack;
		g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE2_X, dMove);

	} else if (nIndex == 7 || nIndex == 8) {	// Pitch Move
		int nDir = (nIndex == 7 ? -1 : 1);
		double dMove = pEquipData->dInspectPitchX * nDir;
		g_objAJinAXL.Move_Relative(AX_INSPECT_STAGE2_X, dMove);

	} else if (nIndex == 0 || nIndex == 1 || nIndex == 4 || nIndex == 5 || nIndex == 6) {	// Load, Align, Top1, Top2, Unload
		int nIdx = (nIndex > 1 ? (nIndex - 2) : nIndex);
		g_objCommon.Move_Position(AX_INSPECT_STAGE2_X, nIdx);

	} else if (nIndex == 9) {
		double dMove = pEquipData->dInspectTop2ShiftX;
		g_objAJinAXL.Move_Relative(AX_INSPECT_STAGE2_X, dMove);
	}

	m_strLog.Format("[Manual Btm1] Inspect Stage2 X (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualBtm1Dlg::OnChkInspectStage2IoClick(UINT nID)
{
// 	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_CHK_INSPECT_STAGE2_IO_0;

	BOOL bOn = m_chkInspectStage2Io[nIndex].GetCheck();
	DY_DATA_06 *pDY06 = g_objAJinAXL.Get_pDY06();

	if (nIndex == 0) pDY06->oInspectStage2Vac01 = bOn;		// Vac On/Off
	if (nIndex == 1) pDY06->oInspectStage2Vac02 = bOn;
	if (nIndex == 2) pDY06->oInspectStage2Vac03 = bOn;
	if (nIndex == 3) pDY06->oInspectStage2Vac04 = bOn;
	if (nIndex == 4) pDY06->oInspectStage2Vac05 = bOn;
	if (nIndex == 5) pDY06->oInspectStage2Vac06 = bOn;
	if (nIndex == 6) pDY06->oInspectStage2Vac07 = bOn;
	if (nIndex == 7) pDY06->oInspectStage2Vac08 = bOn;
	if (nIndex == 8) pDY06->oInspectStage2Vac09 = bOn;
	if (nIndex == 9) pDY06->oInspectStage2Vac10 = bOn;

	g_objAJinAXL.Write_Output(6);

	m_strLog.Format("[Manual Btm1] Inspect Stage2 Vac Click (%d, %d)", nIndex, (int)bOn);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualBtm1Dlg::OnBtnInspectStage2IoClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;

	int nIndex = nID - IDC_BTN_INSPECT_STAGE2_IO_0;

	if (nIndex == 2 || nIndex == 3) {	// Up / Down
// 		int nPosX2 = -1, nPosX3 = -1, nPosX1 = -1;
// 		for (int i = 0; i < 5; i++) { if (g_objCommon.Check_Position(AX_INSPECT_STAGE2_X, i)) { nPosX2 = i; break; } }
// 		for (int i = 0; i < 5; i++) { if (g_objCommon.Check_Position(AX_INSPECT_STAGE3_X, i)) { nPosX3 = i; break; } }
// 		for (int i = 0; i < 5; i++) { if (g_objCommon.Check_Position(AX_INSPECT_STAGE1_X, i)) { nPosX1 = i; break; } }
// 		if (nPosX2 == 0 && (nPosX3 == 0 || nPosX1 == 0)) { AfxMessageBox("Inspect Stage 1/2/3 X축 위치 확인 후 진행하세요."); return; }
// 		if (nPosX2 == 4 && (nPosX3 == 4 || nPosX1 == 4)) { AfxMessageBox("Inspect Stage 1/2/3 X축 위치 확인 후 진행하세요."); return; }
		double dX2 = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE2_X);
		double dX3 = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE3_X);
		double dX1 = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE1_X);
		if (fabs(dX2 - dX3) < 300.0) { AfxMessageBox("Inspect Stage 3 X축 위치 확인 후 진행하세요."); return; }
		if (fabs(dX2 - dX1) < 300.0) { AfxMessageBox("Inspect Stage 1 X축 위치 확인 후 진행하세요."); return; }
	}

	DX_DATA_05 *pDX05 = g_objAJinAXL.Get_pDX05();
	if (nIndex == 4 && !pDX05->iInspectStage2Up && pDX05->iInspectStage2Down) {
		AfxMessageBox("Inspect Stage2 Down 상태에서 동작하지 않습니다."); return;
	}

	DY_DATA_05 *pDY05 = g_objAJinAXL.Get_pDY05();
	DY_DATA_06 *pDY06 = g_objAJinAXL.Get_pDY06();
	if (nIndex == 0) pDY06->nValue |= (0x3FF << 0);		// Vac On All
	if (nIndex == 1) pDY06->nValue &= ~(0x3FF << 0);	// Vac Off All
	if (nIndex == 2) { pDY05->oInspectStage2Up = TRUE;   pDY05->oInspectStage2Down = FALSE; }	// Up
	if (nIndex == 3) { pDY05->oInspectStage2Up = FALSE;  pDY05->oInspectStage2Down = TRUE; }	// Down
	if (nIndex == 4) { pDY05->oInspectStage2Fwd = TRUE;  pDY05->oInspectStage2Bwd = FALSE; }	// Fwd
	if (nIndex == 5) { pDY05->oInspectStage2Fwd = FALSE; pDY05->oInspectStage2Bwd = TRUE; }		// Bwd
	g_objAJinAXL.Write_Output(5);
	g_objAJinAXL.Write_Output(6);

	if (nIndex == 0) { for (int i = 0; i < 10; i++) m_chkInspectStage2Io[i].SetCheck(TRUE); }
	if (nIndex == 1) { for (int i = 0; i < 10; i++) m_chkInspectStage2Io[i].SetCheck(FALSE); }

	m_strLog.Format("[Manual Btm1] Inspect Stage2 Io (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualBtm1Dlg::OnBtnInspectStage3XClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_INSPECT_STAGE3_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_INSPECT_STAGE1_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_INSPECT_STAGE2_X)) return;
	if (!g_objAJinAXL.Is_Home(AX_BTM1_PICKER_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_BTM2_PICKER_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_MODULE_ALIGN_Y)) return;
	if (!g_objAJinAXL.Is_Home(AX_MODULE_ALIGN_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_TOP1_MIRROR_Z)) return;
	if (!g_objAJinAXL.Is_Home(AX_TOP2_SHIFT_Y)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_INSPECT_STAGE3_X_0;

	DX_DATA_05 *pDX05 = g_objAJinAXL.Get_pDX05();
	if (pDX05->iInspectStage3Up && !pDX05->iInspectStage3Down) {
		if (pDX05->iInspectStage1Up || !pDX05->iInspectStage1Down) { AfxMessageBox("Inspect Stage1 Down 상태를 확인 후 진행하세요."); return; }
		if (pDX05->iInspectStage2Up || !pDX05->iInspectStage2Down) { AfxMessageBox("Inspect Stage2 Down 상태를 확인 후 진행하세요."); return; }
		if (!g_objCommon.Check_Position(AX_MODULE_ALIGN_Y, 0)) { AfxMessageBox("Module Align Y축 Center 위치 확인 후 진행하세요."); return; }
		if (!g_objCommon.Check_Position(AX_MODULE_ALIGN_Z, 0)) { AfxMessageBox("Module Align Z축 Ready Up 위치 확인 후 진행하세요."); return; }
		if (!g_objCommon.Check_Position(AX_TOP1_MIRROR_Z, 0)) { AfxMessageBox("Top1 Mirror Z축 Ready Up 위치 확인 후 진행하세요."); return; }
		if (!g_objCommon.Check_Position(AX_TOP2_SHIFT_Y, 0)) { AfxMessageBox("Top2 Shift Y축 Ready 위치 확인 후 진행하세요."); return; }

		if (g_objCommon.Check_Position(AX_BTM1_PICKER_X, 2) && !g_objCommon.Check_Position(AX_BTM1_PICKER_Z, 0)) {
			AfxMessageBox("Btm1 Picker X축 Z축 위치 확인 후 진행하세요."); return;
		}

		if (g_objCommon.Check_Position(AX_BTM2_PICKER_X, 0) && !g_objCommon.Check_Position(AX_BTM2_PICKER_Z, 0)) {
			AfxMessageBox("Btm1 Picker X축 Z축 위치 확인 후 진행하세요."); return;
		}

	} else if (!pDX05->iInspectStage3Up && pDX05->iInspectStage3Down) {
		if (!pDX05->iInspectStage1Up || pDX05->iInspectStage1Down) { AfxMessageBox("Inspect Stage1 Up 상태를 확인 후 진행하세요."); return; }
		if (!pDX05->iInspectStage2Up || pDX05->iInspectStage2Down) { AfxMessageBox("Inspect Stage2 Up 상태를 확인 후 진행하세요."); return; }
		if (pDX05->iInspectStage3Fwd || !pDX05->iInspectStage3Bwd) { AfxMessageBox("Inspect Stage3 Bwd 상태를 확인 후 진행하세요."); return; }

	} else {
		AfxMessageBox("Inspect Stage3 Up 또는 Down 상태를 확인 후 진행하세요."); return;
	}

	EQUIP_DATA *pEquipData = g_objDataManager.Get_pEquipData();
	MOVE_DATA *pMoveData = g_objDataManager.Get_pMoveData();

	if (nIndex == 2 || nIndex == 3) {			// AL Fwd, AL Bwd
		double dBack = (nIndex == 3 ? pEquipData->dInspectALBackOffset : 0.0);
		double dMove = pMoveData->dInspectStage3X[1] + pEquipData->dInspectALOffset - dBack;
		g_objAJinAXL.Move_Absolute(AX_INSPECT_STAGE3_X, dMove);

	} else if (nIndex == 7 || nIndex == 8) {	// Pitch Move
		int nDir = (nIndex == 7 ? -1 : 1);
		double dMove = pEquipData->dInspectPitchX * nDir;
		g_objAJinAXL.Move_Relative(AX_INSPECT_STAGE3_X, dMove);

	} else if (nIndex == 0 || nIndex == 1 || nIndex == 4 || nIndex == 5 || nIndex == 6) {	// Load, Align, Top1, Top2, Unload
		int nIdx = (nIndex > 1 ? (nIndex - 2) : nIndex);
		g_objCommon.Move_Position(AX_INSPECT_STAGE3_X, nIdx);

	} else if (nIndex == 9) {
		double dMove = pEquipData->dInspectTop2ShiftX;
		g_objAJinAXL.Move_Relative(AX_INSPECT_STAGE3_X, dMove);
	}

	m_strLog.Format("[Manual Btm1] Inspect Stage3 X (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualBtm1Dlg::OnChkInspectStage3IoClick(UINT nID)
{
// 	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_CHK_INSPECT_STAGE3_IO_0;

	BOOL bOn = m_chkInspectStage3Io[nIndex].GetCheck();
	DY_DATA_06 *pDY06 = g_objAJinAXL.Get_pDY06();

	if (nIndex == 0) pDY06->oInspectStage3Vac01 = bOn;		// Vac On/Off
	if (nIndex == 1) pDY06->oInspectStage3Vac02 = bOn;
	if (nIndex == 2) pDY06->oInspectStage3Vac03 = bOn;
	if (nIndex == 3) pDY06->oInspectStage3Vac04 = bOn;
	if (nIndex == 4) pDY06->oInspectStage3Vac05 = bOn;
	if (nIndex == 5) pDY06->oInspectStage3Vac06 = bOn;
	if (nIndex == 6) pDY06->oInspectStage3Vac07 = bOn;
	if (nIndex == 7) pDY06->oInspectStage3Vac08 = bOn;
	if (nIndex == 8) pDY06->oInspectStage3Vac09 = bOn;
	if (nIndex == 9) pDY06->oInspectStage3Vac10 = bOn;

	g_objAJinAXL.Write_Output(6);

	m_strLog.Format("[Manual Btm1] Inspect Stage3 Vac Click (%d, %d)", nIndex, (int)bOn);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualBtm1Dlg::OnBtnInspectStage3IoClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;

	int nIndex = nID - IDC_BTN_INSPECT_STAGE3_IO_0;

	if (nIndex == 2 || nIndex == 3) {	// Up / Down
// 		int nPosX3 = -1, nPosX1 = -1, nPosX2 = -1;
// 		for (int i = 0; i < 5; i++) { if (g_objCommon.Check_Position(AX_INSPECT_STAGE3_X, i)) { nPosX3 = i; break; } }
// 		for (int i = 0; i < 5; i++) { if (g_objCommon.Check_Position(AX_INSPECT_STAGE1_X, i)) { nPosX1 = i; break; } }
// 		for (int i = 0; i < 5; i++) { if (g_objCommon.Check_Position(AX_INSPECT_STAGE2_X, i)) { nPosX2 = i; break; } }
// 		if (nPosX3 == 0 && (nPosX1 == 0 || nPosX2 == 0)) { AfxMessageBox("Inspect Stage 1/2/3 X축 위치 확인 후 진행하세요."); return; }
// 		if (nPosX3 == 4 && (nPosX1 == 4 || nPosX2 == 4)) { AfxMessageBox("Inspect Stage 1/2/3 X축 위치 확인 후 진행하세요."); return; }
		double dX3 = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE3_X);
		double dX1 = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE1_X);
		double dX2 = g_objAJinAXL.Get_Position(AX_INSPECT_STAGE2_X);
		if (fabs(dX3 - dX1) < 300.0) { AfxMessageBox("Inspect Stage 1 X축 위치 확인 후 진행하세요."); return; }
		if (fabs(dX3 - dX2) < 300.0) { AfxMessageBox("Inspect Stage 2 X축 위치 확인 후 진행하세요."); return; }
	}

	DX_DATA_05 *pDX05 = g_objAJinAXL.Get_pDX05();
	if (nIndex == 4 && !pDX05->iInspectStage3Up && pDX05->iInspectStage3Down) {
		AfxMessageBox("Inspect Stage3 Down 상태에서 동작하지 않습니다."); return;
	}

	DY_DATA_05 *pDY05 = g_objAJinAXL.Get_pDY05();
	DY_DATA_06 *pDY06 = g_objAJinAXL.Get_pDY06();
	if (nIndex == 0) pDY06->nValue |= (0x3FF << 16);	// Vac On All
	if (nIndex == 1) pDY06->nValue &= ~(0x3FF << 16);	// Vac Off All
	if (nIndex == 2) { pDY05->oInspectStage3Up = TRUE;   pDY05->oInspectStage3Down = FALSE; }	// Up
	if (nIndex == 3) { pDY05->oInspectStage3Up = FALSE;  pDY05->oInspectStage3Down = TRUE; }	// Down
	if (nIndex == 4) { pDY05->oInspectStage3Fwd = TRUE;  pDY05->oInspectStage3Bwd = FALSE; }	// Fwd
	if (nIndex == 5) { pDY05->oInspectStage3Fwd = FALSE; pDY05->oInspectStage3Bwd = TRUE; }		// Bwd
	g_objAJinAXL.Write_Output(5);
	g_objAJinAXL.Write_Output(6);

	if (nIndex == 0) { for (int i = 0; i < 10; i++) m_chkInspectStage3Io[i].SetCheck(TRUE); }
	if (nIndex == 1) { for (int i = 0; i < 10; i++) m_chkInspectStage3Io[i].SetCheck(FALSE); }

	m_strLog.Format("[Manual Btm1] Inspect Stage3 Io (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualBtm1Dlg::OnBtnModuleAlignYClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_MODULE_ALIGN_Y)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_MODULE_ALIGN_Y_0;

	g_objCommon.Move_Position(AX_MODULE_ALIGN_Y, nIndex);

	m_strLog.Format("[Manual Btm1] Moudle Align Y (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualBtm1Dlg::OnBtnModuleAlignZClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_MODULE_ALIGN_Z)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_MODULE_ALIGN_Z_0;

	g_objCommon.Move_Position(AX_MODULE_ALIGN_Z, nIndex);

	m_strLog.Format("[Manual Btm1] Moudle Align Z (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualBtm1Dlg::OnBtnVisionTop1ZClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_VISION_TOP1_Z)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_VISION_TOP1_Z_0;

	g_objCommon.Move_Position(AX_VISION_TOP1_Z, nIndex);

	m_strLog.Format("[Manual Btm1] Vision Top1 Z (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualBtm1Dlg::OnBtnTop1MirrorZClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_TOP1_MIRROR_Z)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_TOP1_MIRROR_Z_0;

	g_objCommon.Move_Position(AX_TOP1_MIRROR_Z, nIndex);

	m_strLog.Format("[Manual Btm1] Top1 Mirror Z (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualBtm1Dlg::OnBtnTop1MirrorIoClick(UINT nID)
{
	if (!g_objCommon.Check_MainDoor()) return;

	int nIndex = nID - IDC_BTN_TOP1_MIRROR_Z_0;

	DY_DATA_06 *pDY06 = g_objAJinAXL.Get_pDY06();
	if (nIndex == 0) { pDY06->oTop1Mirror12Up = TRUE;  pDY06->oTop1Mirror12Down = FALSE; }	// Up
	if (nIndex == 1) { pDY06->oTop1Mirror12Up = FALSE; pDY06->oTop1Mirror12Down = TRUE; }	// Down
	g_objAJinAXL.Write_Output(6);

	m_strLog.Format("[Manual Btm1] Top1 Mirror Io (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualBtm1Dlg::OnBtnTop2ShiftYClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_TOP2_SHIFT_Y)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_TOP2_SHIFT_Y_0;

	g_objCommon.Move_Position(AX_TOP2_SHIFT_Y, nIndex);

	m_strLog.Format("[Manual Btm1] Top2 Shift Y (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

void CManualBtm1Dlg::OnBtnVisionTop2ZClick(UINT nID)
{
	if (!g_objAJinAXL.Is_Home(AX_VISION_TOP2_Z)) return;
	if (!g_objCommon.Check_MainDoor()) return;
	int nIndex = nID - IDC_BTN_VISION_TOP2_Z_0;

	g_objCommon.Move_Position(AX_VISION_TOP2_Z, nIndex);

	m_strLog.Format("[Manual Btm1] Vision Top2 Z (%d) Click", nIndex);
	g_objLogFile.Save_HandlerLog(m_strLog);
}

///////////////////////////////////////////////////////////////////////////////
