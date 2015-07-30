//�v���O�����w�b�_*****************************************************************
//	�v���O�����T�v	:	�v���O�����G���g���|�C���g
//*********************************************************************************
//==include��======================================================================
#include "GameWnd.h"
#include <windows.h>
#include <tchar.h>
#include <stdlib.h>

//#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

//-------- ���C�u�����̃����N
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")
//=================================================================================


//==�萔��=========================================================================

//=================================================================================


//**�֐�***************************************************************************
//	�T�v	:	�G���g���|�C���g
//*********************************************************************************
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpCmdLine, int iCmdShow)
{
	// ���������[�N���o
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// �E�B���h�E���쐬
	int nResult = EXIT_FAILURE;
	CGameWindow* pWnd = new CGameWindow();
	if (pWnd->CreateFrame(CAPTION_NAME,				// �E�B���h�E �L���v�V����
		WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION ,0,	// �E�B���h�E �X�^�C�� �C�g���X�^�C��
		CW_USEDEFAULT, CW_USEDEFAULT,				// �E�B���h�E�ʒu
		SCREEN_WIDTH, SCREEN_HEIGHT))				// �N���C�A���g�̈�T�C�Y
	{
		pWnd->Show(iCmdShow);						// �E�B���h�E��\��
		pWnd->Update();								// �E�B���h�E�\�����X�V
		nResult = pWnd->Run();						// ���b�Z�[�W ���[�v
	}

	delete pWnd;
	return nResult;
}

//=======================================================================================
//	End of File
//=======================================================================================