//プログラムヘッダ*****************************************************************
//	プログラム概要	:	2Dオブジェクトベースクラス
//*********************************************************************************
#pragma once

//==include部======================================================================
#include "../Obj2D/Obj2D.h"
//=================================================================================

#define MAX_PARTITION (100)


//==定数・列挙型宣言部=============================================================
#define	FVF_VERTEX_2D			(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
//=================================================================================

namespace POLYLINE_TYPE
{
	enum POLYLINE
	{
		HERMITE,
		BEZIER,
	};
};


//==クラス・構造体宣言部===========================================================
class CObjLine : public CObjBase
{
public:
	// コンストラクタ・デストラクタ
	CObjLine();
	virtual ~CObjLine();

	// --生成--
	static CObjLine* Create(LPCTSTR texName , D3DXVECTOR3 Spos ,  D3DXVECTOR3 Epos ,
		 int nWidth ,int nPartition ,int nCurvType, int nSceneID , int nPriority);	// 生成

	static CObjLine*	Create(LPCTSTR texName , D3DXVECTOR3 Spos , D3DXVECTOR3 Epos,

		D3DXVECTOR3 Svec ,D3DXVECTOR3 Evec , int nWidth ,int nPartition,int nCurvType, int nSceneID , int nPriority);				// 生成
	
	virtual void	Init(LPCTSTR texName , D3DXVECTOR3 Spos ,  D3DXVECTOR3 Epos ,
		 int nWidth ,int nPartition ,int nCurvType, int nSceneID , int nPriority);
	virtual void	Init(LPCTSTR texName , D3DXVECTOR3 Spos , D3DXVECTOR3 Epos,
		D3DXVECTOR3 Svec ,D3DXVECTOR3 Evec , int nWidth ,int nPartition,int nCurvType, int nSceneID , int nPriority);


	virtual void	Affine();										// アフィン変換
	// 描画命令、DrawLateを指定すると、3Dオブジェクトより後に描画される
	virtual void	Update();										// アップデート
	virtual void	Draw();		
	virtual void	DrawLate();
	
	virtual void	Release();										// 解放

	// --ゲットセット--
	virtual void	SetLinePos(D3DXVECTOR3 Spos, D3DXVECTOR3 Epos);	//始点、終点のセット
	virtual void	SetVector(D3DXVECTOR3 Svec, D3DXVECTOR3 Evec);	//ベクトルor頂点のセット
	virtual void	AnimeSet(int Speed, int WaitTime);				// アニメーション情報セット
	virtual void	AnimeStop();									// アニメーション停止
	virtual void	SetTex();										//テクスチャデータセット
	virtual void	SetVtx();										//頂点設定
	virtual void	SetLineVtx();									//曲線頂点設定
	virtual void	SetAll();										//設定の全設定(任意の呼び出し不可)
	virtual void	AddEffect(float t, float* Width, D3DCOLOR* Diffuse);//追加効果設定関数

	virtual D3DXVECTOR3	GetSpos(){return m_Spos;}
	virtual D3DXVECTOR3	GetEpos(){return m_Epos;}
	virtual D3DXVECTOR3	GetSvec(){return m_Svec;}
	virtual D3DXVECTOR3	GetEvec(){return m_Evec;}

	virtual void	SetDiffuse(int r , int g , int b , int a);		//色設定
	
	CHitRect		GetHitRect();									// あたり判定矩形取得

protected:
	// --変数--
	D3DXVECTOR3		m_Spos;
	D3DXVECTOR3		m_Epos;
	D3DXVECTOR3		m_Svec;
	D3DXVECTOR3		m_Evec;

	int				m_nPartition;									//分割数
	int				m_nCurvType;									//曲線タイプ
	bool			m_bAnimeFlg;									//アニメーションフラグ
	float			m_fAnimeTime;									//アニメーション時間
	int				m_nAnimeSpeed;									//アニメーションスピード（1Fあたりの分割数の進行度）
	int				m_nWaitTime;									//アニメーションの間隔
	int				m_nWaitCnt;										//アニメーションの間隔待機カウンタ

	CHitRect		m_HitRect;										// あたり判定データ
	LPDIRECT3DTEXTURE9*	m_pTexture;									// テクスチャポインタ
	VERTEX_2D			m_vVtx[MAX_PARTITION][3];									// 座標データ

	// --関数--

private:
	VERTEX_2D			m_vVtxBase[4];								// 座標元データ
};
//=================================================================================


//=================================================================================
//	End of File
//=================================================================================