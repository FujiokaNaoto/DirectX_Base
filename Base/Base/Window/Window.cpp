//**プログラムヘッダ***************************************************************
//	プログラム概要	:	ウィンドウ クラス
//*********************************************************************************
//==include部======================================================================
#include "Window.h"
//#include "DXInput.h"
//=================================================================================


//==定数部=========================================================================
static const TCHAR g_szProc[]	= _T("PropCWindowProc");	// プロパティを識別する文字列
static const TCHAR g_szClass[]	= _T("CWindow");			// ウィンドウクラス名
//=================================================================================


//**関数***************************************************************************
//	概要	:	コンストラクタ
//*********************************************************************************
CWindow::CWindow() :
	m_hWnd(NULL),
	m_lpfnOldWndProc(NULL),
	m_bDialog(false),
	m_nMsgLast(0),
	m_pszClassName(NULL),
	m_Atom(NULL)
{
	m_ptCursorLast.x = m_ptCursorLast.y = -1;
	m_hInstance = (HINSTANCE)::GetModuleHandle(NULL);
	::ZeroMemory(&m_msgCur, sizeof(m_msgCur));

	// 新規メッセージ登録
}


//**関数***************************************************************************
//	概要	:	デストラクタ
//*********************************************************************************
CWindow::~CWindow()
{
	HWND hWnd = Detach();
	if (hWnd)
		::DestroyWindow(hWnd);
	if (m_Atom)
	{
		::UnregisterClass(m_pszClassName, m_hInstance);
		m_Atom = NULL;
	}
	if (m_pszClassName)
	{
		delete[] m_pszClassName;
		m_pszClassName = NULL;
	}
}


//**関数***************************************************************************
//	概要	:	[静的メンバ関数] 振り分け用のウィンドウプロシージャ
//*********************************************************************************
LRESULT CALLBACK CWindow::WndMapProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CWindow* pWnd = (CWindow*)::GetProp(hWnd, g_szProc);
	if (pWnd)	// インスタンス毎のプロシージャ処理
		return pWnd->WndProc(uMsg, wParam, lParam);

	// ダイアログの場合、FALSEを返す
	if (::GetWindowLong(hWnd, DWL_DLGPROC))
		return FALSE;
	// デフォルトウィンドウプロシージャを呼び出す
	return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}


//**関数***************************************************************************
//	概要	:	オブジェクト毎のウィンドウプロシージャ
//*********************************************************************************
LRESULT CWindow::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
	case WM_CREATE:
		return 0;

	case WM_KEYDOWN:
		return 0;

	case WM_CLOSE:
		return 0;

	case WM_DESTROY:
		break;

	case WM_COMMAND:
		break;

	case WM_MOUSEMOVE:
		return 0;

	case WM_DROPFILES:
		return 0;

	case WM_MOUSEWHEEL:
		return 0;

	default:
		break;
	}
	return DefWindowProc(uMsg, wParam, lParam);
}


//**関数***************************************************************************
//	概要	:	デフォルト ウィンドウ プロシージャ
//*********************************************************************************
LRESULT CWindow::DefWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (m_lpfnOldWndProc)
		return ::CallWindowProc(m_lpfnOldWndProc, m_hWnd, uMsg, wParam, lParam);
	return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
}


//**関数***************************************************************************
//	概要	:	ウィンドウクラス登録
//*********************************************************************************
bool CWindow::RegisterClass()
{
	if (!m_pszClassName) 
	{
		m_pszClassName = new TCHAR[_tcslen(g_szClass) + 8 + 1];
		::wsprintf(m_pszClassName, _T("%s%08X"), g_szClass, ::GetTickCount());
		if (!m_pszClassName)
			return false;
	}

	if (!m_Atom)
	{
		WNDCLASSEX wcex;
		ZeroMemory(&wcex, sizeof(WNDCLASSEX));
		wcex.cbSize			= sizeof(wcex);
		wcex.lpszClassName	= m_pszClassName;						// クラス名
		wcex.lpfnWndProc	= WndMapProc;							// ウィンドウプロシージャ
		wcex.style			= CS_VREDRAW | CS_HREDRAW;				// 占有指定
		wcex.hInstance		= m_hInstance;							// インスタンス
		wcex.hCursor		= ::LoadCursor(NULL, IDC_ARROW);		// デフォルト カーソル
		wcex.hIcon			= ::LoadIcon(NULL, IDI_APPLICATION);	// デフォルト アイコン
		wcex.hbrBackground	= (HBRUSH)::GetStockObject(WHITE_BRUSH);// 背景色
		wcex.hIconSm		= ::LoadIcon(NULL, IDI_APPLICATION);	// デフォルト アイコン
		m_Atom = ::RegisterClassEx(&wcex);
		if (!m_Atom)
			return false;
	}

	return true;
}


//**関数***************************************************************************
//	概要	:	ウィンドウ生成
//*********************************************************************************
bool CWindow::Create(LPCTSTR pszWindowName, DWORD dwStyle, DWORD dwExStyle,
	int nX, int nY, int nWidth, int nHeight,
	HWND hwndParent, HMENU nIDorHMenu, LPVOID pParam)
{
	if (!RegisterClass())
		return false;

	HWND hWnd = ::CreateWindowEx(dwExStyle, m_pszClassName, pszWindowName, dwStyle,
		nX, nY, nWidth, nHeight, hwndParent, nIDorHMenu, m_hInstance, pParam);
	if (hWnd) {
		if (!Attach(hWnd))
		{
			::DestroyWindow(hWnd);
			m_hWnd = NULL;
		}
	}
	return m_hWnd != NULL;
}


//**関数***************************************************************************
//	概要	:	ウィンドウ生成
//*********************************************************************************
bool CWindow::Create(LPCTSTR pszWindowName, DWORD dwStyle, DWORD dwExStyle,
	const RECT& rc, CWindow* pParentWnd, UINT nID, LPVOID pParam)
{
	return Create(pszWindowName, dwStyle, dwExStyle,
		rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
		pParentWnd->m_hWnd, (HMENU)nID, pParam);
}


//**関数***************************************************************************
//	概要	:	クライアント サイズを指定してウィンドウ生成
//*********************************************************************************
bool CWindow::CreateFrame(LPCTSTR pszWindowName,
	DWORD dwStyle, DWORD dwExStyle,
	int nX, int nY, int nClientWidth, int nClientHeight,
	HWND hwndParent, HMENU nIDorHMenu, LPVOID pParam)
{
	RECT rc = {0, 0, nClientWidth, nClientHeight};
	if (!::AdjustWindowRectEx(&rc, dwStyle,
		nIDorHMenu && !IS_INTRESOURCE(nIDorHMenu), dwExStyle))
		return false;

	return Create(pszWindowName, dwStyle, dwExStyle,
		nX, nY, rc.right - rc.left, rc.bottom - rc.top,
		hwndParent, nIDorHMenu, pParam);
}


//**関数***************************************************************************
//	概要	:	ウィンドウ ハンドルと CWindow オブジェクトを結び付ける
//*********************************************************************************
bool CWindow::Attach(HWND hWnd)
{
	// ウィンドウ ハンドル hWnd を m_hWnd に保存
	if (!hWnd)
		return false;
	m_hWnd = hWnd;
	// ダイアログかウィンドウかを判定する
	m_bDialog = (::GetWindowLong(hWnd, DWL_DLGPROC) != 0);
	int nIndex = m_bDialog ? DWL_DLGPROC : GWL_WNDPROC;

	// ウィンドウハンドルと CWindow オブジェクトを結びつける
	::SetProp(m_hWnd, g_szProc, (HANDLE)this);

	// 既存のウィンドウをサブクラス化する場合は、ウィンドウ(ダイアログ)
	// プロシージャをWndMapProcに置き換える
	if (::GetWindowLong(m_hWnd, nIndex) != (LONG)WndMapProc)
		m_lpfnOldWndProc = (WNDPROC)::SetWindowLong(m_hWnd, nIndex, (LONG)WndMapProc);

	return true;
}


//**関数***************************************************************************
//	概要	:	ウィンドウハンドルを CWindow オブジェクトから切り離す
//*********************************************************************************
HWND CWindow::Detach()
{
	if (!m_hWnd)
		return NULL;

	// ウィンドウがサブクラス化されている場合は、
	// ウィンドウ(ダイアログ)プロシージャを元に戻す。
	if (m_lpfnOldWndProc)
	{
		::SetWindowLong(m_hWnd, (m_bDialog ? DWL_DLGPROC : GWL_WNDPROC), (LONG)m_lpfnOldWndProc);
		m_lpfnOldWndProc = NULL;
	}

	// ウィンドウ ハンドルを CWindow オブジェクトから切り離す
	::RemoveProp(m_hWnd, g_szProc);

	HWND hWnd = m_hWnd;
	m_hWnd = NULL;
	return hWnd;
}


//**関数***************************************************************************
//	概要	:	デフォルトのメッセージ ループ
//*********************************************************************************
int CWindow::Run()
{
	if (!InitInstance())
		return ExitInstance();

	bool bIdle = TRUE;
	long lIdleCount = 0;
	for (;;)
	{
		while (bIdle && !::PeekMessage(&m_msgCur, NULL, NULL, NULL, PM_NOREMOVE)) 
		{
			if (!OnIdle(lIdleCount++))
				bIdle = false;
		}

		do 
		{
			if (!PumpMessage())
				return ExitInstance();
			if (IsIdleMessage(&m_msgCur))
			{
				bIdle = true;
				lIdleCount = 0;
			}

		}
		while (::PeekMessage(&m_msgCur, NULL, NULL, NULL, PM_NOREMOVE));
	}
	return -1;	// ここには来ないはず。
}


//**関数***************************************************************************
//	概要	:	メッセージをディスパッチするか否かの判定
//*********************************************************************************
bool CWindow::PreTranslateMessage(MSG* pMsg)
{
	return false;
}


//**関数***************************************************************************
//	概要	:	メッセージの取得とディスパッチ
//*********************************************************************************
bool CWindow::PumpMessage()
{
	if (!::GetMessage(&m_msgCur, NULL, NULL, NULL))
		return false;

	if (!PreTranslateMessage(&m_msgCur)) 
	{
		::TranslateMessage(&m_msgCur);
		::DispatchMessage(&m_msgCur);
	}
	return true;
}


//**関数***************************************************************************
//	概要	:	入力待ち時の処理
//*********************************************************************************
bool CWindow::OnIdle(long lCount)
{
	return lCount < 0;
}


//**関数***************************************************************************
//	概要	:	OnIdle 呼び出し可否の判定
//*********************************************************************************
bool CWindow::IsIdleMessage(MSG* pMsg)
{
	if (pMsg->message == WM_MOUSEMOVE || pMsg->message == WM_NCMOUSEMOVE) 
	{
		if (m_ptCursorLast.x == pMsg->pt.x && m_ptCursorLast.y == pMsg->pt.y &&
			pMsg->message == m_nMsgLast)
			return false;
		m_ptCursorLast = pMsg->pt;
		m_nMsgLast = pMsg->message;
		return true;
	}
	// WM_PAINT と WM_SYSTIMER(キャレット点滅)
	return pMsg->message != WM_PAINT && pMsg->message != 0x0118;
}


//**関数***************************************************************************
//	概要	:	アプリケーション開始時の処理
//*********************************************************************************
bool CWindow::InitInstance()
{
	return true;
}


//**関数***************************************************************************
//	概要	:	アプリケーション終了時の処理
//*********************************************************************************
int CWindow::ExitInstance()
{
	return m_msgCur.wParam;
}


//**関数***************************************************************************
//	概要	:	WM_MOUSEWHEEL メッセージ ハンドラ
//*********************************************************************************
int CWindow::MessageBox(LPCTSTR pszText, LPCTSTR pszCaption, UINT nType)
{
	return ::MessageBox(m_hWnd, pszText, pszCaption, nType);
}

//=======================================================================================
//	End of File
//=======================================================================================