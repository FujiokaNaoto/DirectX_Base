//プログラムヘッダ*****************************************************************
//	プログラム概要	:	シーン管理クラス
//*********************************************************************************
#pragma once

//==include部======================================================================
#include "../System/common.h"

#include <list>

#include "..\Scene\Scene.h"
#include "Changing.h"
//=================================================================================


//==定数・列挙型宣言部=============================================================
namespace eScene_ID
{
	enum ESCENE_ID
	{
		NONE,					// 所属シーンなし

		// 特殊シーン
		CHANGE,
		PAUSE,					// ポーズ開始

		// シーン
		TITLE,					// タイトル
		MAIN,					// ゲーム本体

		CLEAR,
		OVER,

		RESLOAD,

		MAX
	};
};
//=================================================================================


//==マクロ定義部===================================================================
#define SCENE_LIST			std::list<CScene*>							// シーンリスト
#define SCENE_LIST_ITE		std::list<CScene*>::iterator				// シーンリストへのイテレータ
//=================================================================================


//==クラス・構造体宣言部===========================================================
class CSceneManage
{
private:
	static	SCENE_LIST		m_SceneList;								// ロードされたテクスチャを保持するリスト

	bool	m_bChangeFlg;												// シーン変更中フラグ

	CChanging*	m_pChanging;											// 変更中シーン
	CScene*	m_pCurrentScene;											// 今のシーンのポインタ
	int		m_nNextScene;												// 次のシーンコード

public:
	// コンストラクタ・デストラクタ
	CSceneManage();
	virtual	~CSceneManage();

	static	CSceneManage*	Create();									// 生成
	bool	Init();														// 初期化
	void	Release();													// マネージャ解放

	void	Input();													// 入力受付
	void	Update();													// 更新
	void	Draw();														// 描画

	void	Changeing();												// シーン変更中処理

	CScene*	Search(int nSceneID);										// すでにシーンが格納されているかを検索
	CScene*	Entry(int nSceneID);										// シーンをリスト内に登録
	CScene*	SceneCreate(int nSceneCode);								// 各シーン生成

	void	ListClear();												// 格納リストを完全クリア
	bool	ClearOnce(int nSceneID);									// 指定シーンの解放

	// --ゲットセット--
	bool	SetNextScene(int nSceneID , int nChangeType);				// シーン変更命令
	bool	GetChangeFlg(){return m_bChangeFlg;}						// シーン変更中フラグ取得

	CScene&	GetCurrentScene(){return *m_pCurrentScene;}					// 現在のシーン取得
};
//=================================================================================


//=================================================================================
//	End of File
//=================================================================================