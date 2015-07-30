//プログラムヘッダ*****************************************************************
//	プログラム概要	:	タイトルクラス定義
//*********************************************************************************
#pragma once

//==include部======================================================================
#include "../System/common.h"
#include "Scene.h"
//=================================================================================


//==定数部=========================================================================
namespace eTitleConst
{
	enum BUTTON
	{
		START,
		CREDIT,
		EXIT,

		BUTTON_MAX,
	};
};
//=================================================================================


//==列挙型宣言部===================================================================
// プライオリティ
namespace eTitlePri
{
	enum 
	{	
		NONE,
		BG,

		OBJ,

		MAX
	};
};

//=================================================================================


//==マクロ定義部===================================================================
//=================================================================================


//==クラス宣言部===================================================================
class CTitle : public CScene
{
private:
	//-------- ゲーム用オブジェクト
	OutputStreamPtr	m_pBgm;
	SoundEffectPtr	m_pKey;
	SoundEffectPtr	m_pCursor;

	bool			m_bChangeFlg;

	S_HANDLE		m_hObj;

public:
	CTitle();
	virtual ~CTitle();

	static CTitle* Create(int nSceneID);			// 生成
	virtual void Release();							// 解放
	
	virtual bool Init();							// ループ内初期化
	virtual void Uninit();							// ループ内終了処理
	
	virtual void Input();							// 入力受付
	virtual void Update();							// 更新
	virtual void Draw();							// 描画


	// --ゲット・セット--
	virtual	int		GetMaxPriority();

private:

};
//=================================================================================


//==プロトタイプ宣言===============================================================
//=================================================================================

//=================================================================================
//	End of File
//=================================================================================