//プログラムヘッダ*****************************************************************
//	プログラム概要	:	プログラムエントリポイント
//*********************************************************************************
//==include部======================================================================
#include "GameWnd.h"
#include <windows.h>
#include <tchar.h>
#include <stdlib.h>

//#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>

//-------- ライブラリのリンク
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")
#pragma comment(lib, "dinput8.lib")
//=================================================================================


//==定数部=========================================================================

//=================================================================================


//**関数***************************************************************************
//	概要	:	エントリポイント
//*********************************************************************************
int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInst, LPTSTR lpCmdLine, int iCmdShow)
{
	// メモリリーク検出
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// ウィンドウを作成
	int nResult = EXIT_FAILURE;
	CGameWindow* pWnd = new CGameWindow();
	if (pWnd->CreateFrame(CAPTION_NAME,				// ウィンドウ キャプション
		WS_OVERLAPPED | WS_SYSMENU | WS_CAPTION ,0,	// ウィンドウ スタイル ，拡張スタイル
		CW_USEDEFAULT, CW_USEDEFAULT,				// ウィンドウ位置
		SCREEN_WIDTH, SCREEN_HEIGHT))				// クライアント領域サイズ
	{
		pWnd->Show(iCmdShow);						// ウィンドウを表示
		pWnd->Update();								// ウィンドウ表示を更新
		nResult = pWnd->Run();						// メッセージ ループ
	}

	delete pWnd;
	return nResult;
}

//=======================================================================================
//	End of File
//=======================================================================================