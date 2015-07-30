//プログラムヘッダ*****************************************************************
//	プログラム概要	:	エフェクトパーティクルクラス定義
//*********************************************************************************
#pragma once

//==include部======================================================================
#include "Obj2D.h"
//=================================================================================


//==定数・列挙型宣言部=============================================================
namespace eParticleConst2D
{
	enum
	{
		STATE_IN,
		STATE_UPDATE,
		STATE_OUT,
	};
};
//=================================================================================


//==マクロ定義部===================================================================

//=================================================================================


//==クラス・構造体宣言部===========================================================
struct ParticleParam2D
{
	D3DXVECTOR3	ParInitPos;											// 初期位置
	D3DXVECTOR3	ParInitSpd , ParInitRot , ParInitScall;				// 初期速度 , 初期回転角 , 初期拡大
	D3DXVECTOR3	ParAccel , ParRotSpd , ParScallSpd;					// 加速度 , 回転速度 , 拡縮速度
	int			nParLife;											// 生存時間
	int			nInFrame , nOutFrame;								// 登場時フェードイン , 消去時フェードアウト
	unsigned int	nRed , nGreen , nBlue , nAlpha;					// カラーコード

	ParticleParam2D(void)
	{
		ParInitPos = D3DXVECTOR3(0.0f , 0.0f , 0.0f);
		ParInitSpd = ParInitRot = D3DXVECTOR3(0.0f , 0.0f , 0.0f);
		ParInitScall = D3DXVECTOR3(1.0f , 1.0f , 1.0f);
		ParAccel = ParRotSpd = ParScallSpd = D3DXVECTOR3(0.0f , 0.0f , 0.0f);
		nParLife = 20;
		nInFrame = 3;
		nOutFrame = 3;
		nRed = nGreen = nBlue = 255;
		nAlpha = 0;
	}
};


class CParticle2D : public CObj2D
{
private:

protected:
	D3DXVECTOR3	m_Accel , m_RotSpd , m_ScallSpd;
	
	int	m_nLifeCnt;
	int	m_nLife , m_nInFrame , m_nOutFrame;

	unsigned int	m_nRed , m_nGreen , m_nBlue , m_nAlpha;

	void ParamSet(ParticleParam2D param);
public:
	CParticle2D();
	virtual ~CParticle2D();

	static CParticle2D* Create(LPCTSTR texName , ParticleParam2D param , int nWidth , int nHeight , int nSceneID , int nPri);
	
	virtual bool	Init(LPCTSTR texName , ParticleParam2D param , int nWidth , int nHeight , int nSceneID , int nPri);

	// テクスチャパターンの切り替え

	virtual void	SetRS();
	virtual	void	Update();


	virtual	void	SetRGBA(int R , int G , int B , int A);
};
//=================================================================================


//=================================================================================
//	End of File
//=================================================================================