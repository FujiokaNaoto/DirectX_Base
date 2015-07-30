//**プログラムヘッダ***************************************************************
//	プログラム概要	:	エミッタークラス実装
//*********************************************************************************


//==include部======================================================================
#include <stdlib.h>
#include <tchar.h>

#include "../../System/Register.h"
#include "Emitter.h"
#include "..\ObjBoard\Particle.h"
//=================================================================================


//==定数・列挙型部=================================================================
namespace eEmitterConst
{
	const int	DEFAULT_SIZE = 5;

	const int	FEADIN_FARME = 5;
	const int	FEADOUT_FRAME = 5;
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
CEmitter::CEmitter():
m_pParent(NULL),
m_TexName(NULL),
m_nMyLife(eEmitterConst::NONE_LIFE),
m_bActive(true),
m_nGenerateNum(0),
m_nGenerateLenge(0),
m_nGenerateCnt(0),
m_nLifeRandLenge(1),
m_ParRandPosLenge(D3DXVECTOR3(0.0f , 0.0f , 0.0f)),
m_ParRandSpdLenge(D3DXVECTOR3(0.0f , 0.0f , 0.0f)),
m_ParRandRotLenge(D3DXVECTOR3(0.0f , 0.0f , 0.0f)),
m_ParRandScallLenge(D3DXVECTOR3(0.0f , 0.0f , 0.0f)),
m_ParRandRotSpdLenge(D3DXVECTOR3(0.0f , 0.0f , 0.0f)), 
m_ParRandScallSpdLenge(D3DXVECTOR3(0.0f , 0.0f , 0.0f)),
m_ParRandAccelLenge(D3DXVECTOR3(0.0f , 0.0f , 0.0f))
{
	memset(&m_SendParam , 0 , sizeof(m_SendParam));
	m_SendParam.ParInitScall = DEFAULT_SCALL;
	m_SendParam.nInFrame = eEmitterConst::FEADIN_FARME;
	m_SendParam.nOutFrame = eEmitterConst::FEADOUT_FRAME;

	m_SendParam.nRed = m_SendParam.nGreen = m_SendParam.nBlue = m_SendParam.nAlpha = 255;
}


//**関数***************************************************************************
//	概要	:	生成
//*********************************************************************************
CEmitter* CEmitter::Create(LPCTSTR texName , CObjBase* parent , int nSceneID , int nPri , int nGenerateLenge , 
	int nGenerateNum , int nParLife)
{
	CEmitter*	pObj = new CEmitter();

	if(pObj->Init(texName , parent , nSceneID , nPri , nGenerateLenge ,nGenerateNum , nParLife))
		return pObj;

	SAFE_DELETE(pObj);
	return NULL;
}


//**関数***************************************************************************
//	概要	:	初期化
//*********************************************************************************
bool CEmitter::Init(LPCTSTR texName , CObjBase* parent , int nSceneID , int nPri , int nGenerateLenge ,
		int nGenerateNum , int nParLife)
{
	if(parent)
		m_pParent = parent;

	m_TexName = texName;
	m_nSceneID = nSceneID;
	m_nPriority = nPri;
	m_nGenerateLenge = nGenerateLenge;
	m_nGenerateNum = nGenerateNum;

	m_KeyParam.nParLife = nParLife;

	return true;
}

//**関数***************************************************************************
//	概要	:	更新
//*********************************************************************************
void CEmitter::Update()
{
	if(!m_bActive)
		return;			// 動作中以外は更新なし

	if(m_nMyLife != eEmitterConst::NONE_LIFE)
	{
		m_nMyLife --;
		if(m_nMyLife == 0)
		{
			m_bDeleteFlg = true;
			m_bActive = false;
		}
	}

	m_nGenerateCnt ++;

	if(m_nGenerateCnt >= m_nGenerateLenge)
	{
		m_nGenerateCnt -= m_nGenerateLenge;
		ParticleGenerate();
	}
}


//**関数***************************************************************************
//	概要	:	パーティクル生成
//*********************************************************************************
void CEmitter::ParticleGenerate()
{
	CParticle*	pBuf;

	// 生成数分、クリエイト
	for(int i = 0 ; i < m_nGenerateNum ; i ++)
	{
		SendParamSet();
		pBuf = CParticle::Create(m_TexName , m_SendParam , eEmitterConst::DEFAULT_SIZE ,
			eEmitterConst::DEFAULT_SIZE , m_nSceneID , m_nPriority);

		REGISTER->Entry(pBuf , eOBJSTAGE::EFFECT);
	}
}


//**関数***************************************************************************
//	概要	:	パーティクルに送るデータのセット
//*********************************************************************************
void CEmitter::SendParamSet()
{
	if(m_pParent)
		m_SendParam.ParInitPos = m_pParent->GetPos() + m_Pos + m_KeyParam.ParInitPos + RANDF * m_ParRandPosLenge;
	else
		m_SendParam.ParInitPos = m_Pos + m_KeyParam.ParInitPos + RANDF * m_ParRandPosLenge;

	m_SendParam.ParInitSpd = m_KeyParam.ParInitSpd + RANDF * m_ParRandSpdLenge;
	m_SendParam.ParInitScall = m_KeyParam.ParInitScall + RANDF * m_ParRandScallLenge;
	m_SendParam.ParInitRot = m_KeyParam.ParInitRot + RANDF * m_ParRandRotLenge;

	m_SendParam.ParAccel = m_KeyParam.ParAccel + RANDF * m_ParRandAccelLenge;
	m_SendParam.ParRotSpd = m_KeyParam.ParRotSpd + RANDF * m_ParRandRotSpdLenge;
	m_SendParam.ParScallSpd = m_KeyParam.ParScallSpd + RANDF * m_ParRandScallSpdLenge;

	m_SendParam.nParLife = m_KeyParam.nParLife + (int)(RANDF * m_nLifeRandLenge);

	m_SendParam.nInFrame = m_KeyParam.nInFrame;
	m_SendParam.nOutFrame = m_KeyParam.nOutFrame;

	m_SendParam.nRed = m_KeyParam.nRed;
	m_SendParam.nGreen = m_KeyParam.nGreen;
	m_SendParam.nBlue = m_KeyParam.nBlue;
}


//**関数***************************************************************************
//	概要	:	アフィン変換なし
//*********************************************************************************
void CEmitter::Affine()
{
}


//**関数***************************************************************************
//	概要	:	前描画処理なし
//*********************************************************************************
void CEmitter::Draw()
{
}


//**関数***************************************************************************
//	概要	:	後描画処理なし
//*********************************************************************************
void CEmitter::DrawLate()
{
}
//=================================================================================
//	End of File
//=================================================================================