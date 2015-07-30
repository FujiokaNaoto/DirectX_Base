//**プログラムヘッダ***************************************************************
//	プログラム概要	:	ゲーム管理クラス実装
//*********************************************************************************


//==include部======================================================================
#include "GameManager.h"
#include "../Window/GameWnd.h"
#include "common.h"
#include "Register.h"
//=================================================================================


//==定数・列挙型部=================================================================

//=================================================================================


//==マクロ定義部===================================================================

//=================================================================================


//==クラス・構造体宣言部===========================================================

//=================================================================================


//==静的メンバ変数部===============================================================
TCHAR			CGameManager::m_szDebug[];
CGameManager*	CGameManager::m_pInstance = NULL;
//=================================================================================


//**関数***************************************************************************
//	概要	:	コンストラクタ
//*********************************************************************************
CGameManager::CGameManager():
	m_pGraph(NULL),
	m_pInput(NULL),
	m_pTexManage(NULL),
	m_pMeshManage(NULL),
	m_pSceneManage(NULL),
	m_pCamera(NULL),
	
	m_hWnd(NULL) , 
	m_hInst(NULL)
{
	m_szDebug[0] = _T('\0');	// デバッグ文字列初期化
}


//**関数***************************************************************************
//	概要	:	デストラクタ
//*********************************************************************************
CGameManager::~CGameManager()
{
}


//**関数***************************************************************************
//	概要	:	マネージャー生成
//*********************************************************************************
CGameManager* CGameManager::Create(HWND hWnd , HINSTANCE hInst , bool bWindow)
{
	// 実体があるときはそれを返す
	if(m_pInstance)
		return m_pInstance;

	m_pInstance = new CGameManager;


	// --生成時の初期化--
	// ウィンドウの情報を登録しておく
	m_pInstance->m_hWnd = hWnd;
	m_pInstance->m_hInst = hInst;

	// グラフィッククラス生成
	if(! m_pInstance->m_pGraph) m_pInstance->m_pGraph = CGraphics::Create(m_pInstance->m_hWnd , 
		SCREEN_WIDTH , SCREEN_HEIGHT , bWindow);
	if(!m_pInstance->m_pGraph)
	{
		SAFE_RELEASE(m_pInstance);
		return NULL;												// 生成失敗の時はプログラム終了
	}

	// 入力クラス生成
	if(! m_pInstance->m_pInput) m_pInstance->m_pInput = CDXInput::Create(m_pInstance->m_hInst , 
		m_pInstance->m_hWnd);
	if(!m_pInstance->m_pInput)
	{
		SAFE_RELEASE(m_pInstance);
		return NULL;												// 生成失敗の時はプログラム終了
	}

	// --マネージャ生成--
	// テクスチャ
	if(!m_pInstance->m_pTexManage) m_pInstance->m_pTexManage = CTextureManage::Create();
	if(!m_pInstance->m_pTexManage)
	{
		SAFE_RELEASE(m_pInstance);
		return NULL;												// 生成失敗の時はプログラム終了
	}

	// メッシュ
	if(!m_pInstance->m_pMeshManage) m_pInstance->m_pMeshManage = CMeshManage::Create();
	if(!m_pInstance->m_pMeshManage) 
	{
		SAFE_RELEASE(m_pInstance);
		return NULL;												// 生成失敗の時はプログラム終了
	}

	// カメラ
	if(!m_pInstance->m_pCamera) m_pInstance->m_pCamera = CCamera::Create(eCameraState::POS_FIX ,
		eCameraState::LOOK_FIX , eCameraState::UP_FIX , 
		eCameraState::FIXED , NULL);								// カメラ生成
	if(!m_pInstance->m_pCamera)	
	{
		SAFE_RELEASE(m_pInstance);
		return NULL;												// 生成失敗の時はプログラム終了
	}

	// Audio Device
	if(!m_pInstance->m_pAudioDevice) m_pInstance->m_pAudioDevice = OpenDevice();
	if(!m_pInstance->m_pAudioDevice) 
	{
		SAFE_RELEASE(m_pInstance);
		return NULL;												// 生成失敗の時はプログラム終了
	}

	// シーン
	if(!m_pInstance->m_pSceneManage) m_pInstance->m_pSceneManage = CSceneManage::Create();
	if(!m_pInstance->m_pSceneManage) 
	{
		SAFE_RELEASE(m_pInstance);
		return NULL;												// 生成失敗の時はプログラム終了
	}


	return m_pInstance;
}


//**関数***************************************************************************
//	概要	:	更新
//*********************************************************************************
void CGameManager::Update()
{
	// 削除対象オブジェクトを削除
	REGISTER->ReleaseDeleteFlg();

	// 入力更新
	if(m_pInput)m_pInput->Update();						// DirectXの入力処理更新
	m_pSceneManage->Input();							// シーンごとの入力処理を呼ぶ

	m_pCamera->Update();

	REGISTER->UpdateAll();								// リスト所属全オブジェクトを更新
	m_pSceneManage->Update();							// シーン更新

	REGISTER->AffineAll();								// リスト所属全オブジェクトをアフィン変換
	REGISTER->CollisionAll();							// リスト所属オブジェクトの全あたり判定チェック

}
	

//**関数***************************************************************************
//	概要	:	描画準備
//*********************************************************************************
void CGameManager::Render()
{
	if (m_pGraph->Begin()) 
	{	
		Draw();						// 描画処理

		m_pGraph->End();			// 描画終了
	}

	m_pGraph->SwapBuffer();			// バックバッファ入替

	m_szDebug[0] = _T('\0');		// デバッグ文字列初期化
}


//**関数***************************************************************************
//	概要	:	描画
//*********************************************************************************
void CGameManager::Draw()
{
	// 描画開始
	m_pCamera->Draw();									// カメラ描画

	m_pSceneManage->Draw();								// シーン描画処理
	
	REGISTER->DrawAll();								// オブジェクト描画

#ifdef _DEBUG
	// デバックテキスト
	TCHAR	str[256];

	static int t = 1;
	static int cnt = 1;

	if(m_nFPS > 0)
	{
		t += m_nFPS;
		cnt ++;
	}

	_stprintf_s(str, _T("FPS = %d\n"), (int)(t/cnt));
	AddDebugStr(str);
	_stprintf_s(str, _T("CameraPos{%.1f , %.1f , %.1f}\n") , m_pCamera->GetPos().x , m_pCamera->GetPos().y , m_pCamera->GetPos().z);
	AddDebugStr(str);
	_stprintf_s(str, _T("CameraLook{%.1f , %.1f , %.1f}\n") , m_pCamera->GetLook().x , m_pCamera->GetLook().y , m_pCamera->GetLook().z);
	AddDebugStr(str);

	// デバッグ文字列描画
	m_pGraph->DrawText(0, 0, m_szDebug , FONT_DEBUG , 255 , 255 , 255 , 255);
#endif
}


//**関数***************************************************************************
//	概要	:	解放処理
//*********************************************************************************
void CGameManager::Release()
{
	// 各オブジェクト解放
	if(m_pGraph) SAFE_RELEASE(m_pGraph);				// グラフィッククラス解放
	if(m_pInput) SAFE_RELEASE(m_pInput);				// 入力クラス解放
		
	// マネージャ
	if(m_pTexManage) SAFE_RELEASE(m_pTexManage);		// テクスチャマネージャ解放
	if(m_pMeshManage) SAFE_RELEASE(m_pMeshManage);		// メッシュマネージャ解放
	if(m_pSceneManage) SAFE_RELEASE(m_pSceneManage);	// シーンマネージャ解放

	if(m_pCamera)	SAFE_RELEASE(m_pCamera);			// カメラ解放


	REGISTER->Cleanup();								// オブジェ管理リストを解放

	SAFE_DELETE(m_pInstance);							// 自分を削除
}


//**関数***************************************************************************
//	概要	:	メッセージボックス表示
//*********************************************************************************
void CGameManager::MsgBox(LPCTSTR psz)
{
	MessageBox(m_hWnd , psz , _T("message") , MB_OK);
}


//**関数***************************************************************************
//	概要	:	ウィンドウプロシージャ内の動作
//*********************************************************************************
LRESULT CGameManager::OnWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//int nMessage;

	switch (uMsg)
	{
	// キー入力
	case WM_KEYDOWN:
		//switch(wParam)
		//{

		//// その他の入力
		//default:
		//	break;
		//}
		return 0;

	case WM_COMMAND:
		break;

	case WM_DEVICECHANGE:
		m_pInput->ResetJoy(m_hInst, m_hWnd);
		break;


	// マウス移動
	case WM_MOUSEMOVE:
		//マウス座標
		if(m_pInstance && m_pInput)
		{
			POINT	Point;
			Point.x = LOWORD(lParam);
			Point.y = HIWORD(lParam);
			m_pInput->SetMousePoint(Point);									// マウス座標更新
		}
		return 0;


	case WM_MOUSEWHEEL:
		return 0;

	default:
		break;
	}

	return 0;
}
//=================================================================================
//	End of File
//=================================================================================