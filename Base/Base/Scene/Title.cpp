//**プログラムヘッダ***************************************************************
//	プログラム概要	:	タイトルクラス実装
//*********************************************************************************


//==include部======================================================================
#include "Title.h"
#include "../System/GameManager.h"
#include "../Window/GameWnd.h"

#include "../System/Register.h"
#include "../Object/ObjButton/ObjButton.h"

#include "../Object/Obj3D/Skydome.h"

//=================================================================================


//==定数・列挙型定義部=============================================================
namespace eTitleConst
{
	const LPCTSTR	BG = _T("../resource_data/img/Bg.png");
	const LPCTSTR	MESH = _T("../resource_data/mesh/nanase_pose.x");
	const LPCTSTR	SKY = _T("../resource_data/mesh/sky.x");
};
//=================================================================================


//==マクロ定義部===================================================================
//	なし
//=================================================================================


//==クラス宣言部===================================================================
//	なし
//=================================================================================


//==グローバル変数部===============================================================
//=================================================================================


//==静的メンバ変数部===============================================================
//=================================================================================


//==プロトタイプ宣言===============================================================
//=================================================================================


//**関数***************************************************************************
//	概要	:	コンストラクタ
//*********************************************************************************
CTitle::CTitle(void):
m_pBgm(NULL),
m_pKey(NULL),
m_bChangeFlg(false)
{
}


//**関数***************************************************************************
//	概要	:	デストラクタ
//*********************************************************************************
CTitle::~CTitle(void)
{
}


//**関数***************************************************************************
//	概要	:	生成
//*********************************************************************************
CTitle* CTitle::Create(int nSceneID)
{
	CTitle* pTitle = new CTitle();	
	if(!pTitle)
	{
		return NULL;
	}
	
	pTitle->m_nSceneID = nSceneID;								// シーンIDセット
	return pTitle;
}


//**関数***************************************************************************
//	概要	:	解放
//*********************************************************************************
void CTitle::Release()
{
	Uninit();
	delete this;
}


//関数ヘッダ***********************************************************************	
//	概要	:	初期化、ゲームループ内
//*********************************************************************************
bool CTitle::Init(void)
{
	CObj2D*	pBuf = NULL;

	// SEロード
	/*
	m_pKey = OpenSoundEffect(MANAGER.GetAudioDevice()->get() , eTitleConst::KEY_SE_NAME , MULTIPLE);
	m_pCursor = OpenSoundEffect(MANAGER.GetAudioDevice()->get() , eTitleConst::SEL_SE_NAME , MULTIPLE);

	// BGMロード
	m_pBgm = OpenSound(MANAGER.GetAudioDevice()->get() , eTitleConst::BGM_NAME , true);
	m_pBgm->setRepeat(true);
	m_pBgm->play();					// 再生
	*/


	D3DXVECTOR3 pos;/* = D3DXVECTOR3(SCREEN_WIDTH / 2.0f , SCREEN_HEIGHT / 2.0f , 0.0f);
	pBuf = CObj2D::Create(eTitleConst::BG , pos , SCREEN_WIDTH , SCREEN_HEIGHT , m_nSceneID , 0);
	REGISTER->Entry(pBuf , eOBJSTAGE::BG);
	*/



	pos = D3DXVECTOR3(0.0f , 198.0f , -92.0f);
	CObj3D* pObj3D = CObj3D::Create(eTitleConst::MESH , pos , m_nSceneID , 0);
	REGISTER->Entry(pObj3D , eOBJSTAGE::OBJ);
	m_hObj = pObj3D->GetHandle();

	
	pObj3D = CSkydome::Create(eTitleConst::SKY , m_nSceneID , 0);
	REGISTER->Entry(pObj3D , eOBJSTAGE::BG);
	

	return true;
}


//関数ヘッダ***********************************************************************	
//	概要	:	終了処理、ゲームループ内
//*********************************************************************************
void CTitle::Uninit(void)
{
	//m_pBgm->stop();

	// タイトル所属のオブジェのみ解放
	REGISTER->ReleaseScene(eScene_ID::TITLE);
}


//**関数***************************************************************************
//	概要	:	入力受付
//*********************************************************************************
void CTitle::Input()
{
	static int nType;

	CDXInput* pInput = MANAGER.GetInput();	
	// Enterキートリガーで次シーンへ
	if(pInput->GetKeyTrigger(DIK_RETURN))
	{
		//m_pKey->play();
		// 今のボタンに応じたシーンに移行
		MANAGER.GetSceneManage()->SetNextScene(eScene_ID::MAIN , eChanging::CHANGE_BLACKFEAD);
	}

	CObj3D* pObj = REGISTER_H_P(m_hObj , CObj3D*);
	D3DXVECTOR3 pos = pObj->GetPos();
	if(pInput->GetKeyState(DIK_UP)) pos.y += 0.2f;
	if(pInput->GetKeyState(DIK_DOWN)) pos.y -= 0.2f;
	if(pInput->GetKeyState(DIK_RIGHT)) pos.x += 0.2f;
	if(pInput->GetKeyState(DIK_LEFT)) pos.x -= 0.2f;
	pObj->SetPos(pos);

	if(pInput->GetKeyTrigger(DIK_SPACE))
	{
		nType == 0 ? nType = eShadeType::PHONG : nType *= 2;
		nType > eShadeType::TOON ? nType = 0 : false;
		pObj->SetShadeType(nType);
	}
}


//**関数***************************************************************************
//	概要	:	更新
//*********************************************************************************
void CTitle::Update()
{
}


//**関数***************************************************************************
//	概要	:	描画
//*********************************************************************************
void CTitle::Draw()
{

#ifdef _DEBUG
	TCHAR	str[256];						// デバック用バッファ
	// デバック文字
	_stprintf_s(str , _T("タイトルクラス\n"));
	MANAGER.AddDebugStr(str);
#endif
}


//**関数***************************************************************************
//	概要	:	プライオリティの最大数取得
//*********************************************************************************
int CTitle::GetMaxPriority()
{
	return eTitlePri::MAX;
}

//=================================================================================
//	End of File
//=================================================================================