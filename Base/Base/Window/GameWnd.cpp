//**プログラムヘッダ***************************************************************
//	プログラム概要	:	ゲーム用ウインドウクラス定義
//*********************************************************************************

//==include部======================================================================
#include "../Window/GameWnd.h"
#include "../System/common.h"
#include <stdlib.h>
#include <time.h>
#include "../System/GameManager.h"
//=================================================================================


//==静的メンバ変数部===============================================================
//=================================================================================


//**関数***************************************************************************
//	概要	:	ウィンドウ初期化
//*********************************************************************************
bool CGameWindow::InitInstance()
{
	if (!CWindow::InitInstance()) return false;

	m_bWindow = true;
	/*if (IDYES == MessageBox(_T("ウィンドウモードで実行しますか？"), _T("画面モード"), MB_YESNO))
		m_bWindow = true;*/

	
	srand((unsigned) time(NULL));						// 乱数シード設定

	::timeBeginPeriod(1);								// タイマの分解能を最小にセット

	// マネージャー初期化
	m_pGameManager = CGameManager::Create(m_hWnd , m_hInstance , m_bWindow);
	if(! m_pGameManager) return false;


	m_dwFPSLastTime = ::timeGetTime();
	m_dwExecLastTime = m_dwFPSLastTime - FRAME_RATE;	// 必ず最初に m_pScene->Update() を実行
	m_dwFrameCount = 0;

	return true;
}

//**関数***************************************************************************
//	概要	:	ウィンドウ解放
//*********************************************************************************
int CGameWindow::ExitInstance()
{
	// マネージャー解放
	SAFE_RELEASE(m_pGameManager);	
	
	::timeEndPeriod(1);				// タイマの分解能を元に戻す

	return CWindow::ExitInstance();	// 実行ファイルの戻り値
}


//**関数***************************************************************************
//	概要	:	アイドル時処理
//*********************************************************************************
bool CGameWindow::OnIdle(long lCount)
{
	// フレーム数カウント
	DWORD dwCurrentTime = ::timeGetTime();				// 現在のタイマ値を取得
	if (dwCurrentTime - m_dwFPSLastTime >= 500) 
	{	// 0.5 秒ごとに計測
		// フレーム数を計算
		if (m_pGameManager) 
		{
			m_pGameManager->SetFPS(m_dwFrameCount * 1000 / (dwCurrentTime - m_dwFPSLastTime));
		}
		m_dwFPSLastTime = dwCurrentTime;				// タイマ値を更新
		m_dwFrameCount = 0;								// フレームカウンタをリセット
	}


	// 時間管理
	while (dwCurrentTime - m_dwExecLastTime >= FRAME_RATE) 
	{	// 一定時間が経過したら…
		m_dwExecLastTime += FRAME_RATE;					// タイマ値を更新
			
		if (m_pGameManager) m_pGameManager->Update();	// ゲーム メイン処理
	}


	// 描画
	if (m_pGameManager) 
	{
		m_pGameManager->Render();						// レンダリング処理
	}

	m_dwFrameCount++;									// フレームカウント＋１
	return true;
}


//**関数***************************************************************************
//	概要	:	ウィンドウプロシージャ受信時処理
//*********************************************************************************
LRESULT CGameWindow::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nMessage;

	switch (uMsg)
	{
	// キー入力
	case WM_KEYDOWN:
		switch(wParam)
		{
		//[Esc]入力時はゲーム終了確認
		case VK_ESCAPE:
			nMessage = MessageBox(_T("ゲームを終了しますか？") ,
									_T("終了確認") , MB_YESNO);	//終了確認

			if(nMessage == IDYES)
				PostQuitMessage(0);								//終了指示を出す
			break;

		// その他の入力
		default:
			// ゲームマネージャーに委託
			m_pGameManager->OnWndProc(uMsg , wParam , lParam);
			break;
		}
		return 0;

	// ×ボタンクリック
	case WM_CLOSE:
		nMessage = MessageBox(_T("ゲームを終了しますか？") , 
								_T("終了確認") , MB_YESNO);		//終了確認

		if(nMessage == IDYES)
			PostQuitMessage(0);
		return 0;

	// ウィンドウ消去命令
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		// ゲームマネージャーに委託
		m_pGameManager->OnWndProc(uMsg , wParam , lParam);
		break;
	}
	return DefWindowProc(uMsg, wParam, lParam);
}
//=======================================================================================
//	End of File
//=======================================================================================