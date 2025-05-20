// SetupEquipDlg.h : 헤더 파일
//
#pragma once

// CSetupEquipDlg 대화 상자입니다.

class CSetupEquipDlg : public CDialogEx
{
	DECLARE_DYNAMIC(CSetupEquipDlg)

public:
	CSetupEquipDlg(CWnd* pParent = NULL);   // 표준 생성자입니다.
	virtual ~CSetupEquipDlg();

// 대화 상자 데이터입니다.
	enum { IDD = IDD_SETUP_EQUIP_DLG };
	CGroupCS	m_Group[8];
	CLabelCS	m_Label[34];
	CStaticCS	m_stcEquipName;
	CComboCS	m_cboLotBarcodePort;
	CCheckCS	m_chkJobListTaktLog;
	CStaticCS	m_stcScreenOff;
	CStaticCS	m_stcNoWorkTime;
	CCheckCS	m_chkManualTaktTest;
	CLabelCS	m_lblDoorLock;
	CRadioCS	m_rdoDoorLock[2];
	CRadioCS	m_rdoModel[3];
	CCheckCS	m_chkUseVisionAlign;
	CCheckCS	m_chkUseInspectAngle;
	CCheckCS	m_chkUseInspectBtm1;
	CCheckCS	m_chkUseInspectTop1;
	CCheckCS	m_chkUseInspectTop2;
	CCheckCS	m_chkUseInspectBtm2;
	CCheckCS	m_chkUseDispatcher;
	CCheckCS	m_chkUseMES;
	CCheckCS	m_chkUseMesNg;
	CCheckCS	m_chkUseSNgMix;
	CCheckCS	m_chkUseRosSkip;
	CCheckCS	m_chkUseInspectSkip;
	CStaticCS	m_stcTrayData[4];
	CStaticCS	m_stcPickerData[4];
	CStaticCS	m_stcInspectData[8];
	CStaticCS	m_stcNgBufferData[1];
	CStaticCS	m_stcVacOffDelay[4];
	CStaticCS	m_stcVacOffRepeat[4];
	CStaticCS	m_stcDelayAdd[4];
	CGroupCS	m_grpRos;
	CLabelCS	m_lblRos[3];
	CIPAddressCtrl	m_ipaDispatcherIp;
	CIPAddressCtrl	m_ipaHandlerIp;
	CButtonCS	m_btnRosConnect;
	CButtonCS	m_btnRosDisconnect;
	CStaticCS	m_stcJudgeTimeOver;
	CCheckCS	m_chkJudgeSpecialNg;
	CGroupCS	m_grpTower;
	CLabelCS	m_lblTower[10];
	CCheckCS	m_chkTower[6][4];
	CGroupCS	m_grpBuzzer;
	CLabelCS	m_lblBuzzer[8];
	CCheckCS	m_chkBuzzer[2][6];
	CGroupCS	m_grpHidden;
	CLabelCS	m_lblPasswordMt;
	CStaticCS	m_stcPasswordMt;
	CLabelCS	m_lblPasswordSi;
	CEditCS		m_edtPasswordSi;
	CGroupCS	m_grpResultTest;
	CCheckCS	m_chkResultTestUse;
	CLabelCS	m_lblResultTest[4];
	CEditCS		m_edtResultTest[4];
	CStaticCS	m_stcVisProgVer;
	CStaticCS	m_stcVisParaVer;
	CCheckCS	m_chkUseFocusLog;
	CStaticCS	m_stcTmrFocusLog;
	CStaticCS	m_stcMotionCheck;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

	DECLARE_MESSAGE_MAP()
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnStnClickedStcEquipName();
	afx_msg void OnStnClickedStcScreenOff();
	afx_msg void OnStcTrayDataClick(UINT nID);
	afx_msg void OnStcPickerDataClick(UINT nID);
	afx_msg void OnStcInspectDataClick(UINT nID);
	afx_msg void OnStcNgBufferDataClick(UINT nID);
	afx_msg void OnStcVacOffDelayClick(UINT nID);
	afx_msg void OnStcVacOffRepeatClick(UINT nID);
	afx_msg void OnStcDelayAddClick(UINT nID);
	afx_msg void OnRdoModelClick(UINT nID);
	afx_msg void OnBnClickedBtnRosConnect();
	afx_msg void OnBnClickedBtnRosDisconnect();
	afx_msg void OnStnClickedStcJudgeTimeOver();
	afx_msg void OnStnClickedStcShowHidden();
	afx_msg void OnStnClickedStcPasswordMt();
	afx_msg void OnStnClickedStcNoworkTime();

private:
	CString m_strLog;
	int		m_nPreModel;
	void Initial_Controls();
	void Display_EquipData();

public:
	void Save_EquipData();
	void Cancel_EquipData();
	afx_msg void OnStnClickedStcVisProgVer();
	afx_msg void OnStnClickedStcVisParaVer();
	afx_msg void OnStnClickedStcTmrFocusLog();
	afx_msg void OnStnClickedStcMotionCheck();
};

///////////////////////////////////////////////////////////////////////////////
