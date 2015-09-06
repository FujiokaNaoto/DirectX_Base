//プログラムヘッダ*****************************************************************
//	プログラム概要	:	ゲーム管理クラス定義
//*********************************************************************************
#pragma once

//==include部======================================================================
#include <Windows.h>

#include "../DirectX/Graphics.h"	// draw
#include "../DirectX/DXInput.h"	// input
#include "audiere.h"			// sound
using namespace audiere;

// シーン
#include "../Resource/TextureManage.h"
#include "../Resource/MeshManage.h"
#include "../Scene/SceneManage.h"

// オブジェクト
#include "Camera.h"
#include "../CalcData/Calc.h"
#include "ShadeManager.h"
//=================================================================================


//==定数・列挙型宣言部=============================================================
// 管理するオブジェクトのクラスコードの基準
namespace eClassCode
{
	enum
	{
		SCENE_NONE,

		TEXMANAGE,					// テクスチャマネージャ
		MESHMANAGE,					// メッシュマネージャ
		SCENEMANAGE,				// シーンマネージャ

		// シーン
		SCENE = SCENEMANAGE + 100,	// シーン
		SCENEMAX,


		// 2Dタイプオブジェ	
		OBJ_2D = SCENE + 100,		// 2Dオブジェ
		OBJ_FEAD,
		OBJ_BUTTON,					// 2D上下移動ボタン

		OBJ_2DMAX,


		// 3Dタイプオブジェ
		OBJ_3D,						// 3Dオブジェクト
		OBJ_3DMAX,					// 3Dオブジェクト


		// ビルボードタイプオブジェ
		OBJ_BOARD,					// ビルボード
		OBJ_BOARDMAX,

		MAX
	};
};

// レンダーステートタイプ
namespace eRSType
{
	enum
	{
		NONE,						// 設定なし

		// 2D
		OBJ_2D,

		// 3D
		OBJ_3D,
		OBJ_3D_LATE,

		// スカイドーム
		SKYDOME,

		// ビルボード
		BILLBOARD,
		
		PARTICLE_2D,
		PARTICLE_LATE_2D,

		PARTICLE,
		PARTICLE_LATE,
	};
};
//=================================================================================


//==クラス・構造体宣言部===========================================================
class	CGameManager
{
private:
	static	CGameManager*	m_pInstance;

	HWND		m_hWnd;					
	HINSTANCE	m_hInst;				
	int			m_nFPS;
	static	TCHAR		m_szDebug[4096];

	// --DirectX--
	CGraphics*	m_pGraph;							// 描画関係
	CDXInput*	m_pInput;							// 入力管理

	// --マネージャ--
	CTextureManage*	m_pTexManage;					// テクスチャリソース管理
	CMeshManage*	m_pMeshManage;					// 3Dオブジェクトリソース管理
	CSceneManage*	m_pSceneManage;					// シーン切替管理
	CShadeManager*	m_pShadeManage;					// シェーダ管理

	// --ゲーム内オブジェクト--
	CCamera*		m_pCamera;
	AudioDevicePtr	m_pAudioDevice;
	

public:
	// コンストラクタ・デストラクタ
	CGameManager();
	virtual ~CGameManager();

	static CGameManager* Create(HWND hWnd , HINSTANCE hInst , bool bWindow);

	void	Update();				// 更新処理
	
	void	Render();				// 描画処理準備
	void	Draw();					// 描画処理
	
	void	Release();				// 解放処理


	void	GameEnd(){PostQuitMessage(0);}						// ゲーム終了依頼

	// デバッグ用
	void	SetFPS(int nFPS) {m_nFPS = nFPS;}
	void	AddDebugStr(LPCTSTR psz) {lstrcat(m_szDebug, psz);}
	void	MsgBox(LPCTSTR psz);


	LRESULT	OnWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);	// プロシージャ実装部分

	// --ゲットセット--
	static	CGameManager&	GetInstance(){return *m_pInstance;}	// 実体取得


	HWND			GetHWND(){return m_hWnd;}					// ウィンドウハンドル
	HINSTANCE		GetHINST(){return m_hInst;}					// インスタンスハンドル
	CGraphics*		GetGraph(){return m_pGraph;}				// グラフィックデバイス取得
	CDXInput*		GetInput(){return m_pInput;}				// 入力処理初期化

	// マネージャ
	CTextureManage*	GetTexManage(){return m_pTexManage;}		// テクスチャマネージャ取得
	CMeshManage*	GetMeshManage(){return m_pMeshManage;}		// メッシュマネージャ取得
	CSceneManage*	GetSceneManage(){return m_pSceneManage;}	// シーンマネージャ取得
	CShadeManager*	GetShadeManage(){return m_pShadeManage;}	// シェードマネージャ取得

	CCamera*		GetCamera(){return m_pCamera;}				// カメラ取得

	AudioDevicePtr*	GetAudioDevice(){return &m_pAudioDevice;}
};


//==マクロ定義部===================================================================
#define MANAGER		CGameManager::GetInstance()
//=================================================================================


//=================================================================================


//=================================================================================
//	End of File
//=================================================================================