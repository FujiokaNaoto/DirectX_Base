//プログラムヘッダ*****************************************************************
//	プログラム概要	:	エミッタークラス定義
//*********************************************************************************
#pragma once

//==include部======================================================================
#include "../Base/ObjBase.h"
#include "..\ObjBoard\Particle.h"
//=================================================================================


//==定数・列挙型宣言部=============================================================
namespace eEmitterConst
{
	const int	NONE_LIFE = -1;										// 生存時間制限なし
}
//=================================================================================


//==マクロ定義部===================================================================

//=================================================================================


//==クラス・構造体宣言部===========================================================
class CEmitter : public CObjBase
{
private:

protected:
	LPCTSTR		m_TexName;											// 生成テクスチャ名
	CObjBase*	m_pParent;											// 親
	bool		m_bActive;											// 使用状態判定

	// ---自身の情報---
	int			m_nMyLife;
	int			m_nGenerateLenge , m_nGenerateNum;					// 生成間隔 , 生成個数
	int			m_nGenerateCnt;

	// ---生成するパーティクルに与える情報---
	ParticleParam	m_SendParam;									// 実際に送信するデータ
	ParticleParam	m_KeyParam;										// 送信するデータの基準になるデータ
	int				m_nLifeRandLenge;

	// 生成データのランダム値
	D3DXVECTOR3		m_ParRandPosLenge , m_ParRandRotLenge , m_ParRandScallLenge;
	D3DXVECTOR3		m_ParRandSpdLenge , m_ParRandRotSpdLenge , m_ParRandScallSpdLenge;	
	D3DXVECTOR3		m_ParRandAccelLenge;	

public:
	CEmitter();
	virtual ~CEmitter(){};

	static	CEmitter*	Create(LPCTSTR texName , CObjBase* parent , int nSceneID , int nPri , int nGenerateLenge ,
		int nGenerateNum , int nParLife);
	
	virtual bool	Init(LPCTSTR texName , CObjBase* parent , int nSceneID , int nPri , int nGenerateLenge ,
		int nGenerateNum , int nParLife);

	virtual void	Update();										// 更新
	virtual	void	Affine();										// アフィン変換

	virtual void	Draw();
	virtual void	DrawLate();

	virtual void	EmitterStart(){m_bActive = true;}
	virtual void	EmitterStop(){m_bActive = false;}


	virtual void	ParticleGenerate();
	virtual void	SendParamSet();

	// --ゲットセット--
	virtual void	SetKeyParam(ParticleParam param){m_KeyParam = param;}
	virtual void	SetRandPosLenge(D3DXVECTOR3 vec){m_ParRandPosLenge = vec;}
	virtual void	SetRandRotLenge(D3DXVECTOR3 vec){m_ParRandRotLenge = vec;}
	virtual void	SetRandScallLenge(D3DXVECTOR3 vec){m_ParRandScallLenge = vec;}
	virtual void	SetRandSpdLenge(D3DXVECTOR3 vec){m_ParRandSpdLenge = vec;}
	virtual void	SetRandRotSpdLenge(D3DXVECTOR3 vec){m_ParRandRotSpdLenge = vec;}
	virtual void	SetRandScallSpdLenge(D3DXVECTOR3 vec){m_ParRandScallSpdLenge = vec;}
	virtual void	SetRandAccelLenge(D3DXVECTOR3 vec){m_ParRandAccelLenge = vec;}
};
//=================================================================================


//=================================================================================
//	End of File
//=================================================================================