//**プログラムヘッダ***************************************************************
//	プログラム概要	:	シーン管理クラス実装
//*********************************************************************************


//==include部======================================================================
#include "../System/GameManager.h"
#include "../Window/GameWnd.h"

#include <tchar.h>

#include "../System/common.h"
#include "SceneManage.h"

// --各シーン--
#include "Title.h"
#include "GameMain.h"
#include "GameClear.h"
#include "GameOver.h"
#include "ResLoadScene.h"
//=================================================================================


//==定数・列挙型部=================================================================
//const int	START_SCENE = eScene_ID::RESLOAD;
const int	START_SCENE = eScene_ID::TITLE;
//=================================================================================


//==マクロ定義部===================================================================

//=================================================================================


//==クラス・構造体宣言部===========================================================

//=================================================================================


//==静的メンバ変数部===============================================================
SCENE_LIST		CSceneManage::m_SceneList;
//=================================================================================


//==プロトタイプ宣言===============================================================

//=================================================================================


//**関数***************************************************************************
//	概要	:	コンストラクタ
//*********************************************************************************
CSceneManage::CSceneManage():
m_pCurrentScene(NULL),
m_nNextScene(START_SCENE),
m_bChangeFlg(false),
m_pChanging(NULL)
{

}


//**関数***************************************************************************
//	概要	:	デストラクタ
//*********************************************************************************
CSceneManage::~CSceneManage()
{
}


//**関数***************************************************************************
//	概要	:	生成
//*********************************************************************************
CSceneManage* CSceneManage::Create()
{
	CSceneManage*	pManage = new CSceneManage;

	if(! pManage->Init())
	{
		::MessageBox(NULL , _T("SceneManage生成エラー") , _T("error") , MB_ICONERROR | MB_OK);
		return NULL;
	}

	return pManage;
}


//**関数***************************************************************************
//	概要	:	初期化
//*********************************************************************************
bool CSceneManage::Init()
{
	m_pCurrentScene = Entry(START_SCENE);							// 最初のシーン生成
	if(!m_pCurrentScene)
		return false;												// 生成失敗

	m_pCurrentScene->Init();										// 最初のシーン初期化

	return true;
}


//**関数***************************************************************************
//	概要	:	シーン切替
//			:	成功か失敗のフラグを返す
//*********************************************************************************
bool CSceneManage::SetNextScene(int nSceneCode , int nChangeType)
{
	// 遷移中は受け付けない
	if(m_bChangeFlg)
		return false;

	// シーン変更クラス起動
	m_pChanging = (CChanging*)Entry(eScene_ID::CHANGE);				// 変更用シーンを呼び出し
	m_pChanging->SetNextScene(Entry(nSceneCode) , m_pCurrentScene , nChangeType);
	m_pChanging->Init();

	m_pCurrentScene = m_pChanging;

	m_bChangeFlg = true;
	m_nNextScene = nSceneCode;
	
	return true;
}


//**関数***************************************************************************
//	概要	:	シーン切替中の処理
//*********************************************************************************
void CSceneManage::Changeing()
{
	// チェンジ終了確認
	if(! m_pChanging->GetChangeFlg())
	{
		m_bChangeFlg = false;
		m_pChanging->Uninit();

		m_pCurrentScene = m_pChanging->GetAfterScene();			// 更新対象のシーンを差し替え
	}
}


//**関数***************************************************************************
//	概要	:	入力処理
//*********************************************************************************
void CSceneManage::Input()
{
	// シーン切替処理中は入力を受け付けない
	if(m_bChangeFlg)
		return; 

	// シーン更新
	if(m_pCurrentScene)
		m_pCurrentScene->Input();
}


//**関数***************************************************************************
//	概要	:	更新
//*********************************************************************************
void CSceneManage::Update()
{
	// シーン切替処理
	if(m_bChangeFlg)
		Changeing();

	// シーン更新
	if(m_pCurrentScene)
		m_pCurrentScene->Update();
}


//**関数***************************************************************************
//	概要	:	描画
//*********************************************************************************
void CSceneManage::Draw()
{
	// シーン描画
	if(m_pCurrentScene)
		m_pCurrentScene->Draw();
}


//**関数***************************************************************************
//	概要	:	リスト内検索
//			:	指定した名前のシーンへのポインタを返す
//*********************************************************************************
CScene* CSceneManage::Search(int nSceneID)
{
	SCENE_LIST_ITE	begin = m_SceneList.begin();
	SCENE_LIST_ITE	end = m_SceneList.end();

	while(begin != end)
	{
		// 一致しない間リストを動かす
		if((*begin)->GetSceneID() != nSceneID)
			begin ++;
			
		// 一致したらそのシーンへのポインタを返す
		return (*begin);
	}

	// 見つからなかったのでNULLを返す
	return NULL;
}

//**関数***************************************************************************
//	概要	:	配列へ登録
//			:	指定したシーンへのポインタを返す
//			:	ロード失敗したときはNULLを返す
//*********************************************************************************
CScene* CSceneManage::Entry(int nSceneCode)
{
	CScene*	pScene = NULL;
	
	// すでに指定したファイルがあるかを検索
	pScene = Search(nSceneCode);

	// あればそれを返す
	if(pScene)
		return pScene;


	// なかった場合は新規で生成
	if(! ( pScene = SceneCreate(nSceneCode) ))
	{
		// 失敗したときはNULLを返す
		::MessageBox(NULL , _T("シーン生成エラー") , _T("error") , MB_ICONERROR | MB_OK);
		return NULL;
	}

	return pScene;
}


//**関数***************************************************************************
//	概要	:	各シーン生成
//*********************************************************************************
CScene* CSceneManage::SceneCreate(int nSceneCode)
{
	CScene*	pScene = NULL;

	switch(nSceneCode)
	{
	// チェンジ中シーン
	case eScene_ID::CHANGE:
		pScene = CChanging::Create(nSceneCode);
		break;

	// タイトルシーン
	case eScene_ID::TITLE:
		pScene = CTitle::Create(nSceneCode);
		break;

	// メインシーン
	case eScene_ID::MAIN:
		pScene = CGameMain::Create(nSceneCode);
		break;

	// クリアシーン
	case eScene_ID::CLEAR:
		pScene = CGameClear::Create(nSceneCode);
		break;

	// オーバーシーン
	case eScene_ID::OVER:
		pScene = CGameOver::Create(nSceneCode);
		break;

	// リソースロードシーン
	case eScene_ID::RESLOAD:
		pScene = CResLoadScene::Create(nSceneCode);
		break;

	// 該当しない場合NULLを返す
	default:
		return NULL;
		break;
	}

	return pScene;
}


//**関数***************************************************************************
//	概要	:	配列を完全クリア
//*********************************************************************************
void CSceneManage::ListClear()
{
	SCENE_LIST_ITE begin = m_SceneList.begin();
	SCENE_LIST_ITE end = m_SceneList.end();

	// リスト全解放
	while(begin != end)
	{	
		(*begin)->Release();
		begin = m_SceneList.erase(begin);
	}

	m_SceneList.clear();
}


//**関数***************************************************************************
//	概要	:	マネージャ解放
//*********************************************************************************
void CSceneManage::Release()
{
	ListClear();

	delete this;
}


//**関数***************************************************************************
//	概要	:	個別解放(ファイル名指定)
//			:	解放失敗時はfalseを返す
//*********************************************************************************
bool CSceneManage::ClearOnce(int nScene)
{
	SCENE_LIST_ITE begin = m_SceneList.begin();
	SCENE_LIST_ITE end = m_SceneList.end();

	while(begin != end)
	{
		if((*begin)->GetSceneID() != nScene)
			continue;
		
		// 一致したら削除
		(*begin)->Release();
		m_SceneList.erase(begin);

		return true;
	}

	return false;
}

//=================================================================================
//	End of File
//=================================================================================