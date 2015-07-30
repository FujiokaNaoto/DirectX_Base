//�v���O�����w�b�_*****************************************************************
//	�t�@�C����		:	StateMain.h
//---------------------------------------------------------------------------------
//	�v���O�����T�v	:	�E�B���h�E �N���X��`
//---------------------------------------------------------------------------------
//	����			:	
//*********************************************************************************
#pragma once
//==include��======================================================================
#ifndef _WIN32_WINNT
#define _WIN32_WINNT	0x0500
#endif
#include <windows.h>
#include <tchar.h>
//=================================================================================


//==�萔��=========================================================================
//=================================================================================


//==�N���X�錾��===================================================================
class CWindow
{
public:
	// �R���X�g���N�^�^�f�X�g���N�^
	CWindow();
	virtual ~CWindow();

// ����
private:
	ATOM		m_Atom;				// �E�B���h�E �N���X�o�^���� ATOM
	LPTSTR		m_pszClassName;		// �N���X��
	WNDPROC		m_lpfnOldWndProc;
	bool		m_bDialog;		
	MSG			m_msgCur;			// ���݂̃��b�Z�[�W
	POINT		m_ptCursorLast;	 
	UINT		m_nMsgLast;		 
	// ���b�Z�[�W��U�蕪����E�B���h�E(�_�C�A���O)�v���V�[�W��
	static LRESULT CALLBACK WndMapProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

protected:
	HWND		m_hWnd;				// �E�B���h�E �n���h��
	HINSTANCE	m_hInstance;		// �C���X�^���X �n���h��

	// �I�[�o�[���C�h�\�ȃE�B���h�E(�_�C�A���O)�v���V�[�W��
	virtual LRESULT WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);
	virtual LRESULT DefWindowProc(UINT uMsg, WPARAM wParam, LPARAM lParam);


	// �E�B���h�E�N���X�o�^
	bool RegisterClass();

public:

	// �E�B���h�E����
	bool Create(LPCTSTR pszWindowName,
		DWORD dwStyle = WS_OVERLAPPEDWINDOW, DWORD dwExStyle = 0,
		int nX = CW_USEDEFAULT, int nY = CW_USEDEFAULT,
		int nWidth = CW_USEDEFAULT, int nHeight = CW_USEDEFAULT,
		HWND hwndParent = NULL, HMENU nIDorHMenu = NULL, LPVOID pParam = NULL);
	bool Create(LPCTSTR pszWindowName, DWORD dwStyle, DWORD dwExStyle,
		const RECT& rc, CWindow* pParentWnd, UINT nID, LPVOID pParam = NULL);
	bool CreateFrame(LPCTSTR pszWindowName,
		DWORD dwStyle = WS_OVERLAPPEDWINDOW, DWORD dwExStyle = 0,
		int nX = CW_USEDEFAULT, int nY = CW_USEDEFAULT,
		int nWidth = CW_USEDEFAULT, int nHeight = CW_USEDEFAULT,
		HWND hwndParent = NULL, HMENU nIDorHMenu = NULL, LPVOID pParam = NULL);
	
	
	// �E�B���h�E�n���h����CWindow�I�u�W�F�N�g�����ѕt����
	bool Attach(HWND hWnd);
	// �E�B���h�E�n���h����CWindow�I�u�W�F�N�g����؂藣��
	HWND Detach();

	// ������
	virtual bool InitInstance();
	// �I������
	virtual int ExitInstance();

	// �f�t�H���g�̃��b�Z�[�W ���[�v
	virtual int Run();
	virtual bool PreTranslateMessage(MSG* pMsg);
	virtual bool PumpMessage();
	virtual bool OnIdle(long lCount);
	virtual bool IsIdleMessage(MSG* pMsg);


	// �E�B���h�E �n���h����Ԃ�
	HWND GetHwnd() {return (this) ? m_hWnd : NULL;}
	
	// �C���X�^���X�n���h����Ԃ�
	HINSTANCE	GetInstance() const {return (this) ? m_hInstance : NULL;}

	// �E�B���h�E�̕\��
	bool Show(int nCmdShow) {return ::ShowWindow(m_hWnd, nCmdShow) != FALSE;}
	void Update() {::UpdateWindow(m_hWnd);}

	// ���b�Z�[�W �{�b�N�X�̕\��
	int MessageBox(LPCTSTR pszText, LPCTSTR pszCaption = NULL, UINT nType = MB_OK);

	// �E�B���h�E �T�C�Y
	bool GetClientRect(LPRECT pRect) {return ::GetClientRect(m_hWnd, pRect) != FALSE;}
};
//=================================================================================


//=======================================================================================
//	End of File
//=======================================================================================