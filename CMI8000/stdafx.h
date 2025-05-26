// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 및 프로젝트 관련 포함 파일이 
// 들어 있는 포함 파일입니다.
#pragma once

#ifndef _SECURE_ATL
#define _SECURE_ATL 1
#endif

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 일부 CString 생성자는 명시적으로 선언됩니다.

// MFC의 공통 부분과 무시 가능한 경고 메시지에 대한 숨기기를 해제합니다.
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 핵심 및 표준 구성 요소입니다.
#include <afxext.h>         // MFC 확장입니다.

#include <afxdisp.h>        // MFC 자동화 클래스입니다.

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // Internet Explorer 4 공용 컨트롤에 대한 MFC 지원입니다.
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // Windows 공용 컨트롤에 대한 MFC 지원입니다.
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // MFC의 리본 및 컨트롤 막대 지원


#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

///////////////////////////////////////////////////////////////////////////////

// Library Add
#include "CSControls.h"
#include "CSIniFile.h" 
#include "CSComPort.h"
#include "CSUdpSocket.h"
#include "CSClientSocket.h"
#include "CSGrid.h"
#ifdef _DEBUG
	#pragma comment(lib, "CSControlsD.lib")
	#pragma comment(lib, "CSIniFileD.lib")
	#pragma comment(lib, "CSComPortD.lib")
	#pragma comment(lib, "CSUdpSocketD.lib")
	#pragma comment(lib, "CSClientSocketD.lib")
	#pragma comment(lib, "CSGridD.lib")
#else
	#pragma comment(lib, "CSControlsR.lib")
	#pragma comment(lib, "CSIniFileR.lib")
	#pragma comment(lib, "CSComPortR.lib")
	#pragma comment(lib, "CSUdpSocketR.lib")
	#pragma comment(lib, "CSClientSocketR.lib")
	#pragma comment(lib, "CSGridR.lib")
#endif

#define MAIN_VERSION	_T("V 1.0.0.4")
#define MES_WAITTIME	5000	//5초

//SIMulation 모드 일떄 AJIN, BARCODE, LOADCELL 등은 주석처리하고 SIMULATION_MODE는 주석지운다 

#define AJIN_BOARD_USE
#define LOT_BARCODE_USE
//#define SIMULATION_MODE

// 2~5호기 옵션
#define EDITION_2ND		// 2~5호기

// 21년 모델
// #define TYPE_R13	// 21년 신규 모델	// 총 3가지 R13A, R13B(4*3), R13B(5*3)

// 테스트 런 옵션
// #define DRY_RUN_TEST		// Dry Run Test 시 사용
// #define TEST_MODE	// 양불 비율 설정한대로 배출한다.

///////////////////////////////////////////////////////////////////////////////
// 사용 안하는 옵션
// #define TYPE_R04A
// #define TYPE_R04B
// #define TYPE_R04C
// #define VISION_REPEAT	// 현재 TEST_MODE로 대체하여 사용중.


#define SIM_WAITTIMES	 1		//
#define SIM_WAITTIMEM	 1		//


#ifdef TYPE_R04A
// const BOOL	bR04A = TRUE;
// const BOOL	bR04B = FALSE;
// const BOOL	bR04C = FALSE;
// const int	TRAY_X = 4;
// const int	TRAY_Y = 5;
// const int	SCAN_CNT = 2;
// const int	BTM_PICK_QT = 5;
// const int	SORT_PICK_QT = 4;
#elif defined (TYPE_R04B)
// const BOOL	bR04A = FALSE;
// const BOOL	bR04B = TRUE;
// const BOOL	bR04C = FALSE;
// const int	TRAY_X = 5;
// const int	TRAY_Y = 3;
// const int	SCAN_CNT = 2;
// const int	BTM_PICK_QT = 5;
// const int	SORT_PICK_QT = 5;
#else
// const BOOL	bR04A = FALSE;
// const BOOL	bR04B = FALSE;
// const BOOL	bR04C = TRUE;
// const int	TRAY_X = 8;
// const int	TRAY_Y = 4;
// const int	SCAN_CNT = 1;
// const int	BTM_PICK_QT = 4;
// const int	SORT_PICK_QT = 4;
#endif
///////////////////////////////////////////////////////////////////////////////


extern CString gsCurrentDir;	// 현재 프로젝트 폴더

typedef struct {
	CString	sLotID[2];
	CString	sOperID;				// Operator
	CString	sRecipeName;			// Recipe Item

	int		nTrayUseCount[2];		// 사용자 입력한 Tray 갯수
	int		nCmUseCount[2];			// 사용자 입력한 CM 갯수
	int		nCmMaxCount;			// 한 트레이의 최대 CM 갯수

	int		nLPNo;					// Load 쪽 Port No
	int		nLoadTrayCount[2];
	int		nEmptyTrayCount;
	int		nGoodTrayCount;
	int		nNgTrayNo;
	int		nULPNo;					// Unload Port No
	int		nErrPNo;

	int		InfoAnglePortTray[5][8];// Angle Port Tray 정보 표시 (0:Not Use, 9:Use)
	int		InfoAngleTray[2][5][8];	// Load Tray 정보 표시 (0:Not Use, 9:Use)
	int		InfoBuffTray[2][5][8];	// Buffer Tray 정보 표시 (0:Not Use, 1:Exist)
	int		InfoGoodTray[5][8];		// Good Tray 정보 표시 (0:Empty, 1:Exist)
	int		InfoNgTray[6][5][8];	// NG Tray 정보 표시 (0:Empty, 2:Exist)
	int		InfoNgBuffer[2][5];		// NG Buffer 정보 표시 (0:Empty, 2:Exist)

	int		InfoBtm1Pick[10];		// 0:좌상단
	int		InfoInspect[3][10];		// 0~2:스테이지번호, 0:좌상단
	int		InfoBtm2Pick[10];		// 0:좌상단
	int		InfoSortPick[2][5];		// 0: Rear 1: Front, 0~5:좌측부터 0 

	int		nPNoTrayPick;			// Tray Picker Port No (1, 2)
	int		nPNoAnglePort[2];		// Angle Port Port No (1, 2)
	int		nPNoAngleTray[2];		// Angle Tray Port No (1, 2)
	int		nPNoBtm1Pick;			// Btm1 Picker Port No (1, 2)
	int		nPNoInspect[3];			// Inspect Stage Port No (1, 2)
	int		nPNoBtm2Pick;			// Btm2 Picker Port No (1, 2)
	int		nPNoBuffTray[2];		// Buffer Tray Port No (1, 2)
	int		nPNoSortPick[2];		// Sort Picker Port No (1, 2)
	int		nPNoGoodTray;			// Good Tray Port No (1, 2)
	int		nPNoNgTray;				// NG Tray Port No (1, 2)
	int		nPNoNgBuffer[2];		// NG Buffer Port No (1, 2)

	int		nTNoTrayPick;			// Tray Picker Tray No (1 ~ 25)
	int		nTNoAnglePort[2];		// Angle Port Tray No (1 ~ 25)
	int		nTNoAngleTray[2];		// Angle Tray Tray No (1 ~ 25)
	int		nTNoBtm1Pick[10];		// Btm1 Picker Tray No (1 ~ 25)
	int		nTNoInspect[3][10];		// Inspect Stage Tray No (1 ~ 25)
	int		nTNoBtm2Pick[10];		// Btm2 Picker Tray No (1 ~ 25)
	int		nTNoBuffTray[2][5][8];	// Buffer Tray Tray No (1 ~ 25)
	int		nTNoSortPick[2][5];		// Main Index Tray (1 ~ 25)
	int		nTNoNgBuffer[2][5];

	int		nCNoBtm1Pick[10];		// Load Picker CM No R04A:(1 ~ 20), R04B:(1~15), R04C:(1~32)
	int		nCNoInspect[3][10];		// Load Buffer CM No R04A:(1 ~ 20), R04B:(1~15), R04C:(1~32)
	int		nCNoBtm2Pick[10];		// Load Index CM No R04A:(1 ~ 20), R04B:(1~15), R04C:(1~32)
	int		nCNoBuffTray[2][5][8];	// Load Index CM No R04A:(1 ~ 20), R04B:(1~15), R04C:(1~32)
	int		nCNoSortPick[2][5];		// Main Index CM R04A:(1 ~ 20), R04B:(1~15), R04C:(1~32)
	int		nCNoNgBuffer[2][5];

	BOOL	bReload[5];				// 0:Angle, 1:Btm1, 2:Top1, 3:Top2, 4:Btm2
	BOOL	bScanDone[5];			// 0:Angle, 1:Btm1, 2:Top1, 3:Top2, 4:Btm2
	// Port 별로 저장해야함.
	BYTE	byInspectDone[2][30][40];	// 검사완료 (5bit 사용 ==> 0:Angle, 1:Btm1, 2:Top1, 3:Top2, 4:Btm2)
	int		nInspectInfo[2][30][40];	// 검사결과 (0:Empty, 1:Good, 2:Normal, 3:Mes, 4:N1, 5:N2)
	CString sNGData[2][30][40][5];		// NG 코드  (0:Angle, 1:Btm1, 2:Top1, 3:Top2, 4:Btm2)
	char	cJudgeCode1[30][40][7];	// 판정코드 (0:Angle, 1:Btm1_SP, 2:Top1, 3:Top2, 4:Btm2 5:Btm1_AG 6:Btm1_3D)
	char    cJudgeCode2[30][40][7];

	CString sSortWaitStartTime[2];
	BOOL	bRosDone[2][30][40];		// ROS 판정 대기 Log 용

	BOOL	bCycleStop;				// Run 중간에 멈출때 (초기화 필요)

	int		nErrPickNo;				// Error 상세 처리 (Picker 1 ~ 10)

	BOOL	bNgTrayEnd;				// Lot 종료 되고 NG Door 먼저 열수 있게 해준다. 
	BOOL	bNGTrayWait;			// NG 모듈 제거하고 다시 투입 할때까지 대기
	BOOL	bGoodTrayLotEnd[2];
	BOOL	bGoodTrayWait;			// Good 모듈 배출 완료까지 대기
	BOOL	bEmptyZWait;			// Empty Z Port Full Tray

	BOOL	bLoadLampOn[2];			// Load Port Lamp 켜지는 시점을 Lot End되는 시점으로 바꿔준다.

	BOOL	bTop1MirrorErr[3];		// Top1 Mirror Down Error : TRUE
	int		nTop1MirrorErrNo;		// Top1 Mirror Error No : TRUE


	BOOL	bR04A;					// Model R04A
	BOOL	bR04B;					// Model R04B
	BOOL	bR04C;					// Model R04C
	BOOL	bR13A;					// Model R13A
	BOOL	bR13B_4;				// Model R13B 4x3
	BOOL	bR13B_5;				// Model R13B 5x3
//	int		nModelType;				// 0:R04A, 1:R04B, 2:R04C
	int		nTrayX;					// Tray X Count (R04A:4, R04B:5, R04C:8)
	int		nTrayY;					// Tray Y Count (R04A:5, R04B:3, R04C:4)
	int		nScanCnt;				// Scan Count (R04A:2, R04B:2, R04C:1)
	int		nBtmPickQt;				// Bottom Picker Qt (R04A:5, R04B:5, R04C:4)
	int		nSortPickQt;			// Sort Picker Qt (R04A:4, R04B:5, R04C:4)

	BOOL	bBuffTrayChange;		//
	BOOL	bContinueLotEnd;

	BOOL	bVisionVerErr;			// Vision 프로그램 버전이나 파라미터 버전이 달라 알람이 나면 다시 LotStart 보내줘야한다.
	BOOL	bFirstLotStart;
	BOOL	bMesFirstLot;
	BOOL	bMesContinueLot;		// MES 사용중 연속랏사용할때.
	BOOL	bMesRegistered[2];		// Port 별로 MES 착공 했는지 안했는지 확인.	

	double	dEmptyPortZUpOffset;	// Empty Port Z축 Up Offset 위치 저장 해준다.
	double	dEmptyPortZDownOffset;	// Empty Port Z축 Down Offset 위치 저장 해준다.
	BOOL	bEmptyFull;

	BOOL	bInspetBWD;

	int		nErrAngleStageNo;
	int		nErrBufferStageNo;

	BOOL	bBuffStageMove;			// Sort Picker 복귀할때 Buffer Y축이 움직였으면 다른 Sort Picker 복귀할때는 Buffer Y축을 움직이면 안된다.
	BOOL	bLotEndBeep;			// NG Full 알림음 때문에 LotEnd 알림음이 Skip 안되게 해준다.

	int		nMesPortNo;				// 연속랏 중 MES 알람 발생하면 다시 시작할수 있게 해준다.

	int		nSp1Timer;
	int		nSp2Timer;

	BOOL	bAlignReply;
	BOOL	bEmptyZTopCheckRepeat;	// Empty Z축 Top Check Sensor 확인을 2번해준다.

	CString sListName;
	CString sYYMMDD;
	CString sNGFileName;

	CString sAlarmTime[3];
	CString sAlarmList[3];

	CString	sComName;
} GLOVAL_DATA;

typedef struct {
	CString	sLotID[2];
	CString sStartTime[2];
	CString sEndTime[2];
	CString sMESStartTime[2];
	DWORD	dwLotStart[2];
	DWORD	dwLotEnd[2];
	int		nTrayCount[2];
	int		nCmCount[2];
	double  dTackTime;
	int		nGoodCount[2];
	int		nNgCount[2];
	int		nSNgCount[2][6];			// Special NG (MES, N1, N2)
	int		nRosRequest[2];
	int		nRosGood[2];
	int		nRosNg[2];
	int		nRosRepair[2];
	int		nRosTimeOut[2];

	int		nErrorCount;
	DWORD	dwRunTime;
	DWORD	dwStopTime;
	DWORD	dwErrorTime;

	BOOL	bLotEndComplete[2];

	double	dAverageCycle[28][2][14];

	int		nNGT;
	int		nNGC;
	int		nGDT;
	int		nG1DC;
} GLOVAL_LOT;

typedef struct {
	BOOL	bBegin;
	CString	sLotID;
	int		nAlmNo;
	CString sAlmMsg;
	CString sStartTime;
	CString sEndTime;
	DWORD	dwStartTime;
	DWORD	dwEndTime;
	DWORD	dwProcTime;

	double  dMotionChkPos;
	double  dMotionPos[46];
} GLOVAL_ALM;

typedef struct {
	double  dTaktTime;
	double	dTakt[24][50];	// 시간당 각 Lot의 Takt (시간당 최대 50개)
	int		nLotCount[24];	// 시간당 Lot 수량 (0~23)
	int		nCmCount[2];	// 주간(0:7~19h), 야간(1:19~7h)
} GLOVAL_UPH;

typedef struct {
	int		nLotStatus;				// 0:Stop, 1:Start Send, 2:Start Receive
	DWORD	dwLotStartTime;			// Lot Start time
	int		nCmResult[30][40];		// 0:Not Receive, 1:Result Done, 2:Result Skip

	// Host 전공정 Data
	CString		sHostLotID;			//Host 수신 Lot ID
	CString		sHostRecipe;		//Host 수신 Recipe ID
	CString		sHostModel;			//Host 수신 Recipe ID
	int			nHostCount;			//Host 수신 CM 총갯수
	CString		sHostCancelCode;	//Host Cancel 수신 code
	CString		sHostCancelText;	//Host Cancel 수신 내용

	// 장비 실적 Data	// LJH Port별로 관리해야할지 정해야한다.
	CString		sBarID[2][30][40];		//Barcode ID(장비) [30]:Tray No, [42]:Pocket No
	CString		sJudge[2][30][40];		//N:NG, H:Special NG, M:성능불량(MES NG)
	CString		sNGCode[2][30][40];		//대표NG Code
} GLOVAL_MES;

extern GLOVAL_DATA	gData;
extern GLOVAL_LOT	gLot;
extern GLOVAL_ALM	gAlm;
extern GLOVAL_UPH	gUph;
extern GLOVAL_MES	gMes;
