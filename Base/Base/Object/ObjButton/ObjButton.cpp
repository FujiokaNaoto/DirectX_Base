//**プログラムヘッダ***************************************************************
//	プログラム概要	:	2Dボタンクラス実装
//*********************************************************************************


//==include部======================================================================
#include "../../System/GameManager.h"

#include "ObjButton.h"
//=================================================================================


//==定数・列挙型部=================================================================
namespace eButtonConst
{
	// 拡大関係の定数
	const float	SCALL_SPD = 0.002f;
	const float	SCALL_LENGE = 0.1f;
	
	// 上下移動関係の定数
	const float	SIN_SPD = 0.0625f;
	const float	SIN_LENGE = 10.0f;
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
CObjButton::CObjButton() :
m_nMoveType(eButtonMoveType::NONE),

m_fRadLenge(eButtonConst::SIN_LENGE),
m_fRadian(0.0f),
m_fRadSpd(eButtonConst::SIN_SPD),
m_KeyPos(DEFAULT_POS),

m_fScallLenge(eButtonConst::SCALL_LENGE),
m_fScallSpd(eButtonConst::SCALL_SPD),
m_bExtendFlg(true),
m_bTriggerFlg(false)
{
	m_nClassID = eClassCode::OBJ_BUTTON;

	m_nStatus = eButtonState::DEFAULT;					// ボタン初期状態
}


//**関数***************************************************************************
//	概要	:	デストラクタ
//*********************************************************************************
CObjButton::~CObjButton()
{

}


//**関数***************************************************************************
//	概要	:	生成
//*********************************************************************************
CObjButton* CObjButton::Create(LPCTSTR texName , D3DXVECTOR3 pos , 
	 int nWidth , int nHeight , int nSceneID , int nPriority)
{
	CObjButton*		pObj = new CObjButton;

	if(pObj->Init(texName , pos , DEFAULT_ROT , DEFAULT_SCALL , nWidth , nHeight , nSceneID ,
		nPriority))
		return pObj;
	
	SAFE_DELETE(pObj);
	return NULL;
}


//**関数***************************************************************************
//	概要	:	生成
//*********************************************************************************
CObjButton* CObjButton::Create(LPCTSTR texName , D3DXVECTOR3 pos , 
		D3DXVECTOR3 rot , D3DXVECTOR3 scall , int nWidth , 
		int nHeight , int nSceneID , int nPriority)
{
	CObjButton*		pObj = new CObjButton;

	if(pObj->Init(texName , pos , rot , scall , nWidth , nHeight , nSceneID ,
		nPriority))
		return pObj;
	
	SAFE_DELETE(pObj);
	return NULL;
}


//**関数***************************************************************************
//	概要	:	初期化
//*********************************************************************************
bool CObjButton::Init(LPCTSTR texName , D3DXVECTOR3 pos , 
		 int nWidth , int nHeight , int nSceneID , int nPriority)
{
	return Init(texName , pos , DEFAULT_ROT , DEFAULT_SCALL , nWidth , nHeight ,
		nSceneID , nPriority);
}


//**関数***************************************************************************
//	概要	:	初期化
//*********************************************************************************
bool CObjButton::Init(LPCTSTR texName , D3DXVECTOR3 pos , 
		D3DXVECTOR3 rot , D3DXVECTOR3 scall , int nWidth , 
		int nHeight , int nSceneID , int nPriority)
{
	if(! CObj2D::Init(texName , pos , rot , scall , nWidth , nHeight ,
		nSceneID , nPriority))
		return false;
	
	m_KeyPos =  pos;
	return true;
}


//**関数***************************************************************************
//	概要	:	更新
//*********************************************************************************
void CObjButton::Update()
{
	UpdateMove();				// 挙動更新
 	UpdateState();				// 状態更新
}


//**関数***************************************************************************
//	概要	:	挙動更新
//*********************************************************************************
void CObjButton::UpdateMove()
{
	// 不活性時は動作なし
	if(m_nStatus == eButtonState::INACTIVE)
		return;

	// 上下移動
	if(m_nMoveType & eButtonMoveType::SIN_MOVE)
	{
		m_fRadian += m_fRadSpd;
		if(m_fRadian > 2 * D3DX_PI) 
			m_fRadian -= 2 * D3DX_PI;
		m_Pos.y = m_KeyPos.y + sin(m_fRadian) * m_fRadLenge;
	}

	// 拡縮
	if(m_nMoveType & eButtonMoveType::SCALL)
	{
		if(m_bExtendFlg)
		{
			m_Scall.x += m_fScallSpd;
			m_Scall.y += m_fScallSpd;

			if(m_Scall.x >= 1.0f + m_fScallLenge)
				m_bExtendFlg = false;
		}
		else
		{
			m_Scall.x -= m_fScallSpd;
			m_Scall.y -= m_fScallSpd;

			if(m_Scall.x <= 1.0f)
				m_bExtendFlg = true;
		}
	}
}


//**関数***************************************************************************
//	概要	:	状態更新
//*********************************************************************************
void CObjButton::UpdateState()
{
	switch(m_nStatus)
	{
	case eButtonState::DEFAULT:
		if(m_bTriggerFlg)
			m_bTriggerFlg = false;
		break;

	case eButtonState::HOVER:
		break;

	case eButtonState::ACTIVE:
		break;

	case eButtonState::INACTIVE:
		break;

	default:
		// エラー
		break;
	}
}


//**関数***************************************************************************
//	概要	:	あたり判定
//*********************************************************************************
void CObjButton::CheckCollision()
{
	// 未使用時は処理なし
	if(m_nStatus == eButtonState::INACTIVE) 
		return;

	CDXInput* pInput = MANAGER.GetInput();

	switch(m_nStatus)
	{
	case eButtonState::DEFAULT:
		if(CCalc::IntersectBox2D(this->GetHitRect() , pInput->GetMouseHitRect()))
			SetState(eButtonState::HOVER);
		break;

	case eButtonState::HOVER:
		if(! (CCalc::IntersectBox2D(this->GetHitRect() , pInput->GetMouseHitRect() )) )
			SetState(eButtonState::DEFAULT);

		if(pInput->GetMouseTrigger(0))
			OnTrigger();
		else if(pInput->GetMouseRelease(0))
			OnRelease();
		break;

	case eButtonState::ACTIVE:

		if(pInput->GetMouseTrigger(0))
			OnTrigger();
		else if(pInput->GetMouseRelease(0))
			OnRelease();
		break;

	default:
		// エラー
		break;
	}

	return;
}


//**関数***************************************************************************
//	概要	:	状態変更
//*********************************************************************************
void CObjButton::OnTrigger()
{
	m_bTriggerFlg = true;
}


//**関数***************************************************************************
//	概要	:	状態変更
//*********************************************************************************
void CObjButton::OnRelease()
{
	switch(m_nStatus)
	{
	case eButtonState::HOVER:
		if(m_bTriggerFlg)
			SetState(eButtonState::ACTIVE);
		break;

	case eButtonState::ACTIVE:
		if(m_bTriggerFlg)
			SetState(eButtonState::HOVER);
		break;

	default:
		// エラー
		break;
	}
}

//**関数***************************************************************************
//	概要	:	状態変更
//*********************************************************************************
void CObjButton::SetState(int nState)
{
	m_nStatus = nState;

	// 状態に合わせてボタンの様子を変更
	switch(m_nStatus)
	{
	case eButtonState::DEFAULT:
		SetDiffuse(220 , 220 , 220 , 255);
		break;

	case eButtonState::HOVER:
		SetDiffuse(255 , 255 , 255 , 255);
		break;

	case eButtonState::ACTIVE:
		SetDiffuse(255 , 255 , 255 , 255);
		break;

	case eButtonState::INACTIVE:
		SetDiffuse(55 , 55 , 55 , 255);
		break;

	default:
		// エラー
		break;
	}
	
}

//=================================================================================
//	End of File
//=================================================================================