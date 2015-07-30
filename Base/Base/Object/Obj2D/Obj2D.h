//プログラムヘッダ*****************************************************************
//	プログラム概要	:	2Dオブジェクトベースクラス
//*********************************************************************************
#pragma once

//==include部======================================================================
#include "../Base/ObjBase.h"
#include "../../CalcData/HitData.h"
//=================================================================================


//==定数・列挙型宣言部=============================================================
#define	FVF_VERTEX_2D			(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define	FVF_VERTEX_2D_TEXNONE	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE)
//=================================================================================


//==マクロ定義部===================================================================
struct VERTEX_2D
{
	D3DXVECTOR4		vtx;								//頂点座標
	D3DCOLOR		diffuse;							//反射光
	D3DXVECTOR2		tex;								//テクスチャ座標
};

struct VERTEX_2D_TEXNONE
{
	D3DXVECTOR4		vtx;								//頂点座標
	D3DCOLOR		diffuse;							//反射光
};
//=================================================================================


//==クラス・構造体宣言部===========================================================
class CObj2D : public CObjBase
{
public:
	// コンストラクタ・デストラクタ
	CObj2D();
	virtual ~CObj2D();

	// --生成--
	static CObj2D*	Create(LPCTSTR texName , D3DXVECTOR3 pos , 
		int nWidth , int nHeight , int nSceneID , int nPriority);	// 生成

	static CObj2D*	Create(LPCTSTR texName , D3DXVECTOR3 pos , 
		D3DXVECTOR3 rot , D3DXVECTOR3 scall , int nWidth , 
		int nHeight , int nSceneID , int nPriority);				// 生成

	
	virtual bool	Init(LPCTSTR texName , D3DXVECTOR3 pos , 
		int nWidth , int nHeight , int nSceneID , int nPriority);	// 初期化

	virtual bool	Init(LPCTSTR texName , D3DXVECTOR3 pos , 
		D3DXVECTOR3 rot , D3DXVECTOR3 scall , int nWidth , 
		int nHeight , int nSceneID , int nPriority);				// 初期化


	virtual void	Update();										// 更新
	virtual void	Affine();										// アフィン変換
	// 描画命令、DrawLateを指定すると、3Dオブジェクトより後に描画される
	virtual void	SetRS();
	virtual	void	SetRSLate();
	virtual void	Draw();
	virtual void	DrawLate();
	
	virtual void	Release();										// 解放

	// --ゲットセット--
	virtual void	SetTex();										//テクスチャデータセット
	virtual void	SetVtx();										//頂点設定
	virtual void	SetVtx(D3DXVECTOR4* vtx);						//頂点設定

	virtual void	SetAnimParam(int nAnimFrame , int nTexPatternNum , 
		int nUPatternNum , int nVPatternNum);						// アニメーション情報セット

	virtual void	SetDiffuse(int r , int g , int b , int a);		//色設定
	
	CHitRect		GetHitRect();									// あたり判定矩形取得
	void			SetHitRect(CHitRect rect);						// あたり判定矩形セット

	void			SetResTexture(TCHAR* texName);
	void			SetResTexture(LPDIRECT3DTEXTURE9* tex);
	LPDIRECT3DTEXTURE9*	GetResTexture(){return m_pTexture;}

protected:
	// --変数--
	CHitRect		m_HitRect;										// あたり判定データ

	LPDIRECT3DTEXTURE9*	m_pTexture;									// テクスチャポインタ
	VERTEX_2D			m_vVtx[4];									// 座標データ
	VERTEX_2D_TEXNONE	m_vVtxNTex[4];								// テクスチャなし座標データ

	int		m_nAnimCnt;
	int		m_nAnimFrame;											// アニメーション切り替えフレーム
	int		m_nTexPatternNum;										// テクスチャパターン数
	
	int		m_nPatternNumU , m_nPatternNumV;						// テクスチャのUV方向のパターン数
	int		m_nCurPatternU , m_nCurPatternV;						// 今のUVテクスチャパターン

	// --関数--
	virtual void	AnimUpdate();									// アニメーション更新

private:
	VERTEX_2D			m_vVtxBase[4];								// 座標元データ
	VERTEX_2D_TEXNONE	m_vVtxNTexBase[4];							// テクスチャなし座標元データ
};
//=================================================================================


//=================================================================================
//	End of File
//=================================================================================