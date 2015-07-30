//**プログラムヘッダ***************************************************************
//	プログラム概要	:	エフェクトパーティクルクラス実装
//*********************************************************************************


//==include部======================================================================
#include "../../System/GameManager.h"
#include "Particle2D.h"
//=================================================================================


//==定数・列挙型部=================================================================
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
CParticle2D::CParticle2D():
m_nLifeCnt(0),
m_nLife(0),
m_nInFrame(2),
m_nOutFrame(2),
m_nRed(255),
m_nBlue(255),
m_nGreen(255),
m_nAlpha(1)
{
	m_nStatus = eParticleConst2D::STATE_IN;				// 最初はフェードインモード

	m_nRSType = eRSType::PARTICLE_2D;
	m_nRSTypeLate = eRSType::PARTICLE_LATE_2D;
}


//**関数***************************************************************************
//	概要	:	デストラクタ
//*********************************************************************************
CParticle2D::~CParticle2D()
{
}


//**関数***************************************************************************
//	概要	:	生成
//*********************************************************************************
CParticle2D* CParticle2D::Create(LPCTSTR texName , ParticleParam2D param , int nWidth , int nHeight , int nSceneID , int nPri)
{
	CParticle2D* pObj = new CParticle2D();

	if(pObj->Init(texName , param , nWidth , nHeight , nSceneID , nPri))
		return pObj;

	SAFE_DELETE(pObj);
	return NULL;
}


//**関数***************************************************************************
//	概要	:	初期化
//*********************************************************************************
bool CParticle2D::Init(LPCTSTR texName , ParticleParam2D param , int nWidth , int nHeight , int nSceneID , int nPri)
{
	
	if(! CObj2D::Init(texName , param.ParInitPos , nWidth , nHeight , nSceneID , nPri))
		return false;

	ParamSet(param);								// パーティクルパラメータセット
	SetDiffuse(m_nRed , m_nGreen , m_nBlue , 0);
	Affine();

	return true;
}


//**関数***************************************************************************
//	概要	:	パーティクルパラメータセット
//*********************************************************************************
void CParticle2D::ParamSet(ParticleParam2D param)
{
	m_Pos = param.ParInitPos;
	m_Spd = param.ParInitSpd;
	m_Scall = param.ParInitScall;
	m_Rot = param.ParInitRot;

	m_Accel = param.ParAccel;
	m_RotSpd = param.ParRotSpd;
	m_ScallSpd = param.ParScallSpd;

	m_nLife = param.nParLife;
	m_nInFrame = param.nInFrame;
	m_nOutFrame = param.nOutFrame;

	m_nRed = param.nRed;
	m_nGreen = param.nGreen;
	m_nBlue = param.nBlue;

	if(m_nAlpha > 0)
		m_nAlpha = param.nAlpha;
	else
		m_nAlpha = 1;

	SetDiffuse(m_nRed , m_nGreen , m_nBlue , 0);

}


//**関数***************************************************************************
//	概要	:	更新
//*********************************************************************************
void	CParticle2D::Update()
{
	m_Spd += m_Accel;
	
	m_Pos += m_Spd;
	m_Rot += m_RotSpd;
	m_Scall += m_ScallSpd;

	// 生存確認
	m_nLifeCnt ++;
	if(m_nLifeCnt >= m_nLife)
	{
		m_bDeleteFlg = true;
		return;
	}

	// 透明度変更
	int nAlpha = 255;
	switch(m_nStatus)
	{
	case eParticleConst2D::STATE_IN:
		nAlpha = (int)(((float)m_nLifeCnt / (float)m_nInFrame) * (float)m_nAlpha);
		SetDiffuse(m_nRed , m_nGreen , m_nBlue , nAlpha);

		if(m_nLifeCnt >= m_nInFrame)
		{
			SetDiffuse(m_nRed , m_nGreen , m_nBlue , m_nAlpha);
			m_nStatus = eParticleConst2D::STATE_UPDATE;
		}
		break;

	case eParticleConst2D::STATE_UPDATE:
		if(m_nLifeCnt >= m_nLife - m_nOutFrame)
			m_nStatus = eParticleConst2D::STATE_OUT;
		break;

	case eParticleConst2D::STATE_OUT:
		nAlpha = (int)(m_nAlpha - ((float)m_nAlpha * ((float)m_nLife - (float)m_nLifeCnt) / (float)m_nOutFrame));
		SetDiffuse(m_nRed , m_nGreen , m_nBlue , nAlpha);
		break;
	}

	AnimUpdate();
}


//**関数***************************************************************************
//	概要	:	レンダーステート変更
//*********************************************************************************
void CParticle2D::SetRS()
{
	CGraphics::SemafoLock();
	LPDIRECT3DDEVICE9 pDevice = CGraphics::GetDevice();

	// ライトオフ
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// アルファ ブレンド有効化
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_ONE);

	// Zバッファオフ
	pDevice->SetRenderState(D3DRS_ZENABLE , FALSE);
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetRenderState(D3DRS_CULLMODE , D3DCULL_CCW);

	CGraphics::SemafoUnlock();
}


//**関数***************************************************************************
//	概要	:	色コードセット
//*********************************************************************************
void CParticle2D::SetRGBA(int R , int G , int B , int A)
{
	m_nRed = R;
	m_nGreen = G;
	m_nBlue = B;

	if(A > 0) m_nAlpha = A;
	else m_nAlpha = 1;
}
//=================================================================================
//	End of File
//=================================================================================