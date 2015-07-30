//**プログラムヘッダ***************************************************************
//	プログラム概要	:	フェードクラス
//*********************************************************************************


//==include部======================================================================
#include "../../System/GameManager.h"
#include "../../Window/GameWnd.h"
#include "../../System/Register.h"

#include "ChangeFead.h"
#include "../../DirectX/Graphics.h"
#include "../Obj2D/FeadObj.h"
//=================================================================================


//==定数・列挙型部=================================================================
namespace eChangeFead
{
	enum 
	{
		NONE,
		FEAD_IN,
		FEAD_CHANGE,
		FEAD_STAY,
		FEAD_OUT,
	};

	const int	IN_TIME = 20;
	const int	STAY_TIME = 30;
	const int	OUT_TIME = 20;
};
//=================================================================================


//==マクロ定義部===================================================================

//=================================================================================


//==クラス・構造体宣言部===========================================================

//=================================================================================


//==静的メンバ変数部===============================================================

//=================================================================================


//==プロトタイプ宣言===============================================================

//=================================================================================


//**関数***************************************************************************
//	概要	:	コンストラクタ
//*********************************************************************************
CChangeFead::CChangeFead()
{
	m_nClassID = eClassCode::OBJ_FEAD;
}


//**関数***************************************************************************
//	概要	:	デストラクタ
//*********************************************************************************
CChangeFead::~CChangeFead()
{

}


//**関数***************************************************************************
//	概要	:	生成
//*********************************************************************************
CChangeFead* CChangeFead::Create(CChanging* pChange , int nFeadType , int nSceneID , int nPriority)
{
	CChangeFead*		pObj = new CChangeFead;

	if(pObj->Init(pChange , nFeadType , nSceneID , nPriority))
		return pObj;

	SAFE_DELETE(pObj);
	return NULL;
}


//**関数***************************************************************************
//	概要	:	初期化
//*********************************************************************************
bool CChangeFead::Init(CChanging* pChange , int nFeadType , int nSceneID , 
		int nPriority)
{
	// --初期値格納--
	D3DXVECTOR3 pos(SCREEN_WIDTH / 2.0f , SCREEN_HEIGHT / 2.0f , 0.0f);
	CFeadObj*	pFead = NULL;

	pFead = CFeadObj::Create(pos , SCREEN_WIDTH , SCREEN_HEIGHT , nSceneID , nPriority , true);
	if(!pFead)
		return false;

	pFead->SetDrawLateFlg(true);
	pFead->SetFeadTime(eChangeFead::IN_TIME , eChangeFead::STAY_TIME , eChangeFead::OUT_TIME);

	m_pChange = pChange;
	pFead->FStart(nFeadType);


	if(nFeadType == eChanging::CHANGE_WHITEFEAD)
		pFead->SetDiffuse(255 , 255 , 255 , 0);
	else
		pFead->SetDiffuse(0 , 0 , 0 , 0);

	REGISTER->Entry(pFead , eOBJSTAGE::FADE);

	m_hFead = pFead->GetHandle();
	return true;
}


//**関数***************************************************************************
//	概要	:	更新
//*********************************************************************************
void CChangeFead::Update()
{
	switch(REGISTER_H_P(m_hFead , CFeadObj*)->GetState())
	{
	case eFeadObjConst::FEAD_MIDDLE_STOP:
		if(m_pChange)
		{
			m_pChange->SceneChange();								// シーンのオブジェクト切替
		}
		
		REGISTER_H_P(m_hFead , CFeadObj*)->MiddleStart();
		break;

	case eFeadObjConst::STOP:
		ChangeEnd();
		break;
	}
}


//**関数***************************************************************************
//	概要	:	終了処理
//*********************************************************************************
void CChangeFead::Draw()
{
}


//**関数***************************************************************************
//	概要	:	終了処理
//*********************************************************************************
void CChangeFead::DrawLate()
{
}


//**関数***************************************************************************
//	概要	:	終了処理
//*********************************************************************************
void CChangeFead::Uninit()
{

}


//**関数***************************************************************************
//	概要	:	変更開始命令
//*********************************************************************************
void CChangeFead::ChangeStart()
{
	m_nStatus = eChangeFead::FEAD_IN;
	
	m_nChangeCnt = 0;
}


//**関数***************************************************************************
//	概要	:	変更終了
//*********************************************************************************
void CChangeFead::ChangeEnd()
{
	m_nChangeCnt = 0;
	m_nStatus = eChangeFead::FEAD_STAY;

	// 変更中シーンに変更終了を通知
	if(m_pChange)
		m_pChange->ChangeEnd();

	m_bDeleteFlg = true;
}
//=================================================================================
//	End of File
//=================================================================================