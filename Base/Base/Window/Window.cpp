//**�v���O�����w�b�_***************************************************************
//	�v���O�����T�v	:	�E�B���h�E �N���X
//*********************************************************************************
//==include��======================================================================
#include "Window.h"
//#include "DXInput.h"
//=================================================================================


//==�萔��=========================================================================
static const TCHAR g_szProc[]	= _T("PropCWindowProc");	// �v���p�e�B�����ʂ��镶����
static const TCHAR g_szClass[]	= _T("CWindow");			// �E�B���h�E�N���X��
//=================================================================================


//**�֐�***************************************************************************
//	�T�v	:	�R���X�g���N�^
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

	// �V�K���b�Z�[�W�o�^
}


//**�֐�***************************************************************************
//	�T�v	:	�f�X�g���N�^
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


//**�֐�***************************************************************************
//	�T�v	:	[�ÓI�����o�֐�] �U�蕪���p�̃E�B���h�E�v���V�[�W��
//*********************************************************************************
LRESULT CALLBACK CWindow::WndMapProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	CWindow* pWnd = (CWindow*)::GetProp(hWnd, g_szProc);
	if (pWnd)	// �C���X�^���X���̃v���V�[�W������
		return pWnd->WndProc(uMsg, wParam, lParam);

	// �_�C�A���O�̏ꍇ�AFALSE��Ԃ�
	if (::GetWindowLong(hWnd, DWL_DLGPROC))
		return FALSE;
	// �f�t�H���g�E�B���h�E�v���V�[�W�����Ăяo��
	return ::DefWindowProc(hWnd, uMsg, wParam, lParam);
}


//**�֐�***************************************************************************
//	�T�v	:	�I�u�W�F�N�g���̃E�B���h�E�v���V�[�W��
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


//**�֐�***************************************************************************
//	�T�v	:	�f�t�H���g �E�B���h�E �v���V�[�W��
//*********************************************************************************
LRESULT CWindow::DefWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	if (m_lpfnOldWndProc)
		return ::CallWindowProc(m_lpfnOldWndProc, m_hWnd, uMsg, wParam, lParam);
	return ::DefWindowProc(m_hWnd, uMsg, wParam, lParam);
}


//**�֐�***************************************************************************
//	�T�v	:	�E�B���h�E�N���X�o�^
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
		wcex.lpszClassName	= m_pszClassName;						// �N���X��
		wcex.lpfnWndProc	= WndMapProc;							// �E�B���h�E�v���V�[�W��
		wcex.style			= CS_VREDRAW | CS_HREDRAW;				// ��L�w��
		wcex.hInstance		= m_hInstance;							// �C���X�^���X
		wcex.hCursor		= ::LoadCursor(NULL, IDC_ARROW);		// �f�t�H���g �J�[�\��
		wcex.hIcon			= ::LoadIcon(NULL, IDI_APPLICATION);	// �f�t�H���g �A�C�R��
		wcex.hbrBackground	= (HBRUSH)::GetStockObject(WHITE_BRUSH);// �w�i�F
		wcex.hIconSm		= ::LoadIcon(NULL, IDI_APPLICATION);	// �f�t�H���g �A�C�R��
		m_Atom = ::RegisterClassEx(&wcex);
		if (!m_Atom)
			return false;
	}

	return true;
}


//**�֐�***************************************************************************
//	�T�v	:	�E�B���h�E����
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


//**�֐�***************************************************************************
//	�T�v	:	�E�B���h�E����
//*********************************************************************************
bool CWindow::Create(LPCTSTR pszWindowName, DWORD dwStyle, DWORD dwExStyle,
	const RECT& rc, CWindow* pParentWnd, UINT nID, LPVOID pParam)
{
	return Create(pszWindowName, dwStyle, dwExStyle,
		rc.left, rc.top, rc.right - rc.left, rc.bottom - rc.top,
		pParentWnd->m_hWnd, (HMENU)nID, pParam);
}


//**�֐�***************************************************************************
//	�T�v	:	�N���C�A���g �T�C�Y���w�肵�ăE�B���h�E����
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


//**�֐�***************************************************************************
//	�T�v	:	�E�B���h�E �n���h���� CWindow �I�u�W�F�N�g�����ѕt����
//*********************************************************************************
bool CWindow::Attach(HWND hWnd)
{
	// �E�B���h�E �n���h�� hWnd �� m_hWnd �ɕۑ�
	if (!hWnd)
		return false;
	m_hWnd = hWnd;
	// �_�C�A���O���E�B���h�E���𔻒肷��
	m_bDialog = (::GetWindowLong(hWnd, DWL_DLGPROC) != 0);
	int nIndex = m_bDialog ? DWL_DLGPROC : GWL_WNDPROC;

	// �E�B���h�E�n���h���� CWindow �I�u�W�F�N�g�����т���
	::SetProp(m_hWnd, g_szProc, (HANDLE)this);

	// �����̃E�B���h�E���T�u�N���X������ꍇ�́A�E�B���h�E(�_�C�A���O)
	// �v���V�[�W����WndMapProc�ɒu��������
	if (::GetWindowLong(m_hWnd, nIndex) != (LONG)WndMapProc)
		m_lpfnOldWndProc = (WNDPROC)::SetWindowLong(m_hWnd, nIndex, (LONG)WndMapProc);

	return true;
}


//**�֐�***************************************************************************
//	�T�v	:	�E�B���h�E�n���h���� CWindow �I�u�W�F�N�g����؂藣��
//*********************************************************************************
HWND CWindow::Detach()
{
	if (!m_hWnd)
		return NULL;

	// �E�B���h�E���T�u�N���X������Ă���ꍇ�́A
	// �E�B���h�E(�_�C�A���O)�v���V�[�W�������ɖ߂��B
	if (m_lpfnOldWndProc)
	{
		::SetWindowLong(m_hWnd, (m_bDialog ? DWL_DLGPROC : GWL_WNDPROC), (LONG)m_lpfnOldWndProc);
		m_lpfnOldWndProc = NULL;
	}

	// �E�B���h�E �n���h���� CWindow �I�u�W�F�N�g����؂藣��
	::RemoveProp(m_hWnd, g_szProc);

	HWND hWnd = m_hWnd;
	m_hWnd = NULL;
	return hWnd;
}


//**�֐�***************************************************************************
//	�T�v	:	�f�t�H���g�̃��b�Z�[�W ���[�v
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
	return -1;	// �����ɂ͗��Ȃ��͂��B
}


//**�֐�***************************************************************************
//	�T�v	:	���b�Z�[�W���f�B�X�p�b�`���邩�ۂ��̔���
//*********************************************************************************
bool CWindow::PreTranslateMessage(MSG* pMsg)
{
	return false;
}


//**�֐�***************************************************************************
//	�T�v	:	���b�Z�[�W�̎擾�ƃf�B�X�p�b�`
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


//**�֐�***************************************************************************
//	�T�v	:	���͑҂����̏���
//*********************************************************************************
bool CWindow::OnIdle(long lCount)
{
	return lCount < 0;
}


//**�֐�***************************************************************************
//	�T�v	:	OnIdle �Ăяo���ۂ̔���
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
	// WM_PAINT �� WM_SYSTIMER(�L�����b�g�_��)
	return pMsg->message != WM_PAINT && pMsg->message != 0x0118;
}


//**�֐�***************************************************************************
//	�T�v	:	�A�v���P�[�V�����J�n���̏���
//*********************************************************************************
bool CWindow::InitInstance()
{
	return true;
}


//**�֐�***************************************************************************
//	�T�v	:	�A�v���P�[�V�����I�����̏���
//*********************************************************************************
int CWindow::ExitInstance()
{
	return m_msgCur.wParam;
}


//**�֐�***************************************************************************
//	�T�v	:	WM_MOUSEWHEEL ���b�Z�[�W �n���h��
//*********************************************************************************
int CWindow::MessageBox(LPCTSTR pszText, LPCTSTR pszCaption, UINT nType)
{
	return ::MessageBox(m_hWnd, pszText, pszCaption, nType);
}

//=======================================================================================
//	End of File
//=======================================================================================