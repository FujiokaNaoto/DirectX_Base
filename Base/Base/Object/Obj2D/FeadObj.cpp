//**プログラムヘッダ***************************************************************
//	プログラム概要	:	フェードオブジェ
//*********************************************************************************


//==include部======================================================================
#include "FeadObj.h"
#include <tchar.h>
#include "../../System/GameManager.h"
//=================================================================================


//==定数・列挙型部=================================================================
namespace eFeadObjConst
{
	LPCTSTR	FEAD_TEX = _T("../resource_data/img/Feadout.png");

	const int	IN_TIME = 20;
	const int	MIDDLE_TIME = 15;
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
CFeadObj::CFeadObj():
m_bMiddleStop(false),
m_nFeadType(eFeadObjConst::BLACKFEAD),
m_nInTime(eFeadObjConst::IN_TIME), 
m_nMiddleTime (eFeadObjConst::MIDDLE_TIME), 
m_nOutTime(eFeadObjConst::OUT_TIME),
m_nFeadCnt(0)
{
	m_nClassID = eClassCode::OBJ_FEAD;
}


//**関数***************************************************************************
//	概要	:	デストラクタ
//*********************************************************************************
CFeadObj::~CFeadObj()
{
}


//**関数***************************************************************************
//	概要	:	生成
//*********************************************************************************
CFeadObj* CFeadObj::Create(D3DXVECTOR3 pos , int nWidth , int nHeight ,
	int nSceneID , int nPriority , bool bMiddleStop)
{
	CFeadObj* pObj = new CFeadObj();

	if(pObj->Init(pos , nWidth , nHeight , nSceneID , nPriority ,bMiddleStop))
		return pObj;

	else SAFE_RELEASE(pObj);
	return NULL;
}


//**関数***************************************************************************
//	概要	:	初期化
//*********************************************************************************
bool CFeadObj::Init(D3DXVECTOR3 pos , int nWidth , int nHeight ,
	int nSceneID , int nPriority , bool bMiddleStop)
{
	if(! CObj2D::Init(eFeadObjConst::FEAD_TEX , pos , nWidth , nHeight , nSceneID , nPriority))
		return false;

	m_bMiddleStop = bMiddleStop;
	m_bUseFlg = false;

	if(m_nFeadType == eFeadObjConst::WHITEFEAD)
		SetDiffuse(255 , 255 , 255 , 0);
	else
		SetDiffuse(0 , 0 , 0 , 0);

	return true;
}


//**関数***************************************************************************
//	概要	:	更新
//*********************************************************************************
void CFeadObj::Update()
{
	m_nFeadCnt ++;
	
	int	nAlpha = 0;

	switch(m_nStatus)
	{
	// フェードイン
	case eFeadObjConst::FEAD_IN:
		nAlpha = (int)((float)(255.0f * ((float)m_nFeadCnt / (float)m_nInTime)));

		if(m_nFeadType == eChanging::CHANGE_BLACKFEAD)
			SetDiffuse(0 , 0 , 0 , nAlpha);
		else
			SetDiffuse(255 , 255 , 255 , nAlpha);


		// フェード停止状態へ移行
		if(m_nFeadCnt >= m_nInTime)
		{
			m_nFeadCnt = 0;
			m_nStatus = eFeadObjConst::FEAD_MIDDLE;

			if(m_nFeadType == eChanging::CHANGE_BLACKFEAD)
				SetDiffuse(0 , 0 , 0 , 255);
			else
				SetDiffuse(255 , 255 , 255 , 255);
		}
		break;

	// 中間
	case eFeadObjConst::FEAD_MIDDLE:
		if(m_nFeadCnt >= m_nMiddleTime)
		{
			m_nFeadCnt = 0;

			if(m_bMiddleStop)
				m_nStatus = eFeadObjConst::FEAD_MIDDLE_STOP;
			else
				m_nStatus = eFeadObjConst::FEAD_OUT;

		}
		break;

	// 中間停止
	case eFeadObjConst::FEAD_MIDDLE_STOP:

		break;

	case eFeadObjConst::FEAD_OUT:
		nAlpha = (int)((255.0f - (255.0f * ((float)m_nFeadCnt / (float)m_nOutTime))));

		if(m_nFeadType == eChanging::CHANGE_BLACKFEAD)
			SetDiffuse(0 , 0 , 0 , nAlpha);
		else
			SetDiffuse(255 , 255 , 255 , nAlpha);

		if(m_nFeadCnt >= m_nOutTime)
		{
			m_bUseFlg = false;
			m_nStatus = eFeadObjConst::STOP;
		}
		break;

	default:
		break;
	}
}


//**関数***************************************************************************
//	概要	:	フェード開始
//*********************************************************************************
bool CFeadObj::FStart(int nType)
{
	// すでに動作中なら失敗
	if(m_bUseFlg)
		return false;

	m_nFeadType = nType;
	m_nStatus = eFeadObjConst::FEAD_IN;
	m_nFeadCnt = 0;

	if(m_nFeadType == eFeadObjConst::WHITEFEAD)
		SetDiffuse(255 , 255 , 255 , 0);
	else
		SetDiffuse(0 , 0 , 0 , 0);

	m_bUseFlg = true;

	return true;
}


//**関数***************************************************************************
//	概要	:	フェード中間から開始
//*********************************************************************************
bool CFeadObj::FStartMiddle(int nType)
{
	// すでに動作中なら失敗
	if(m_bUseFlg)
		return false;

	m_nFeadType = nType;
	m_nStatus = eFeadObjConst::FEAD_MIDDLE;
	m_nFeadCnt = 0;

	if(m_nFeadType == eFeadObjConst::WHITEFEAD)
		SetDiffuse(255 , 255 , 255 , 255);
	else
		SetDiffuse(0 , 0 , 0 , 255);

	m_bUseFlg = true;

	return true;
}


//**関数***************************************************************************
//	概要	:	中間停止中フェード動作開始
//*********************************************************************************
bool CFeadObj::MiddleStart()
{
	// 中間停止モードでない時またはフェードが中間停止に来ていない時は失敗
	if(!m_bMiddleStop || m_nStatus != eFeadObjConst::FEAD_MIDDLE_STOP)
		return false;

	m_nStatus = eFeadObjConst::FEAD_OUT;
	m_nFeadCnt = 0;

	return true;
}


//**関数***************************************************************************
//	概要	:	フェード時間設定
//*********************************************************************************
bool CFeadObj::SetFeadTime(int nIn , int nMiddle , int nOut)
{
	// 動作中は変更を受け付けない
	if(m_bUseFlg)
		return false;

	m_nInTime = nIn;
	m_nMiddleTime = nMiddle;
	m_nOutTime = nOut;
	return true;
}

//=================================================================================
//	End of File
//=================================================================================