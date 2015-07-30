//**プログラムヘッダ***************************************************************
//	プログラム概要	:	ゲームオーバークラス実装
//*********************************************************************************


//==include部======================================================================
#include "GameOver.h"
#include "../System/GameManager.h"
#include "../Window/GameWnd.h"

#include "../System/Register.h"
#include "../Object/Obj2D/Obj2D.h"
//=================================================================================


//==定数・列挙型定義部=============================================================
namespace eGameOverConst
{
	TCHAR*	BG_TEX = _T("../img/Bg.png");
	//const	char*		BGM_NAME = "../sound/bgm/Silky_Night.mp3";
	//const	char*		KEY_SE_NAME = "../sound/se/button01.mp3";
};
//=================================================================================


//==マクロ定義部===================================================================
//	なし
//=================================================================================


//==クラス宣言部===================================================================
//	なし
//=================================================================================


//==グローバル変数部===============================================================
//	なし
//=================================================================================


//==静的メンバ変数部===============================================================
//=================================================================================


//==プロトタイプ宣言===============================================================
//=================================================================================


//**関数***************************************************************************
//	概要	:	コンストラクタ
//*********************************************************************************
CGameOver::CGameOver(void):
m_pKey(NULL),
m_pBgm(NULL)
{
}


//**関数***************************************************************************
//	概要	:	デストラクタ
//*********************************************************************************
CGameOver::~CGameOver(void)
{
}


//**関数***************************************************************************
//	概要	:	生成
//*********************************************************************************
CGameOver* CGameOver::Create(int nSceneID)
{
	CGameOver* pGameOver = new CGameOver();
	if (!pGameOver) 
	{
		return NULL;
	}

	pGameOver->m_nSceneID = nSceneID;

	return pGameOver;
}


//**関数***************************************************************************
//	概要	:	解放
//*********************************************************************************
void CGameOver::Release()
{
	Uninit();
	delete this;
}


//関数ヘッダ***********************************************************************	
//	概要	:	初期化、ゲームループ内
//*********************************************************************************
bool CGameOver::Init(void)
{
	// オブジェクト生成用バッファ
	CObjBase*	pBuf = NULL;
	
	// SE
	//m_pKey = OpenSoundEffect(MANAGER.GetAudioDevice()->get() , eGameOverConst::KEY_SE_NAME , MULTIPLE);

	//m_pBgm = OpenSound(MANAGER.GetAudioDevice()->get() , eGameOverConst::BGM_NAME , true);
	//m_pBgm->setRepeat(true);
	//m_pBgm->play();


	return true;
}


//関数ヘッダ***********************************************************************	
//	概要	:	終了処理、ゲームループ内
//*********************************************************************************
void CGameOver::Uninit(void)
{
	//m_pBgm->stop();

	// --結果所属のオブジェのみ解放--
	REGISTER->ReleaseScene(eScene_ID::OVER);
}


//**関数***************************************************************************
//	概要	:	入力受付
//*********************************************************************************
void CGameOver::Input()
{
	CDXInput* pInput = MANAGER.GetInput();

	if(pInput->GetKeyTrigger(DIK_RETURN))
	{
		//m_pKey->play();
		MANAGER.GetSceneManage()->SetNextScene(eScene_ID::TITLE , eChanging::CHANGE_WHITEFEAD);
	}
}


//**関数***************************************************************************
//	概要	:	更新
//*********************************************************************************
void CGameOver::Update()
{
}


//**関数***************************************************************************
//	概要	:	描画
//*********************************************************************************
void CGameOver::Draw()
{
	CGraphics*	pGraph = MANAGER.GetGraph();


#ifdef _DEBUG
	TCHAR	str[256];						// デバック用バッファ
	// デバック文字
	_stprintf_s(str , _T("ゲームオーバーシーン\n"));
	MANAGER.AddDebugStr(str);
#endif
}


//**関数***************************************************************************
//	概要	:	プライオリティの最大数取得
//*********************************************************************************
int CGameOver::GetMaxPriority()
{
	return eGameOverPri::MAX;
}
//=================================================================================
//	End of File
//=================================================================================