//プログラムヘッダ*****************************************************************
//	プログラム概要	:	ワールド上板ポリゴンベースクラス
//*********************************************************************************
#pragma once

//==include部======================================================================
#include "../Base/ObjBase.h"
//=================================================================================


//==定数・列挙型宣言部=============================================================
#define	FVF_VERTEX_BOARD			(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define	FVF_VERTEX_BOARD_TEXNONE	(D3DFVF_XYZ | D3DFVF_DIFFUSE)
//=================================================================================


//==マクロ定義部===================================================================
struct VERTEX_BOARD	
{
	D3DXVECTOR3		vtx;								//頂点座標
	D3DCOLOR		diffuse;							//反射光
	D3DXVECTOR2		tex;								//テクスチャ座標
};

struct VERTEX_BOARD_TEXNONE
{
	D3DXVECTOR3		vtx;								//頂点座標
	D3DCOLOR		diffuse;							//反射光
};
//=================================================================================


//==クラス・構造体宣言部===========================================================
class CObjBoard : public CObjBase
{
public:
	// コンストラクタ・デストラクタ
	CObjBoard();
	virtual ~CObjBoard();

	static CObjBoard*	Create(LPCTSTR texName , D3DXVECTOR3 pos , 
		D3DXVECTOR3 rot , D3DXVECTOR3 scall , int nWidth , 
		int nHeight , int nSceneID , int nPriority);				// 生成

	static CObjBoard*	Create(LPCTSTR texName , D3DXVECTOR3 pos , 
		int nWidth , int nHeight , int nSceneID , int nPriority);	// 生成
	
	virtual bool	Init(LPCTSTR texName , D3DXVECTOR3 pos , 
		int nWidth , int nHeight , int nSceneID , int nPriority);
	
	virtual bool	Init(LPCTSTR texName , D3DXVECTOR3 pos , 
		D3DXVECTOR3 rot , D3DXVECTOR3 scall , int nWidth , 
		int nHeight , int nSceneID , int nPriority);

	virtual void	Affine();										// アフィン変換
	virtual void	SetRS();
	virtual void	SetRSLate();
	virtual void	Draw();											// 先描画
	virtual void	DrawLate();										// 後描画
	
	virtual void	Release();										// 解放

	// --ゲットセット--
	virtual void	SetTex();										//テクスチャデータセット
	virtual void	SetVtx();										//頂点設定
	virtual void	SetVtx(D3DXVECTOR3* vtx);						//頂点設定
	virtual void	SetDiffuse(int r , int g , int b , int a);		//色設定

	virtual void	SetAnimParam(int nAnimFrame , int nTexPatternNum , 
		int nUPatternNum , int nVPatternNum);

	// ビルボードフラグ
	bool			GetBillFlg(){return m_bBillFlg;}				// ビルボードフラグ取得
	void			SetBillFlg(bool	bFlg){m_bBillFlg = bFlg;}		// ビルボードフラグセット

protected:
	// --変数--
	int				m_nWidth , m_nHeight;							// 横幅、縦幅
	bool			m_bBillFlg;										// ビルボードフラグ

	LPDIRECT3DTEXTURE9*		m_pTexture;									// テクスチャポインタ
	VERTEX_BOARD			m_vVtx[4];									// 座標データ
	VERTEX_BOARD_TEXNONE	m_vVtx_NTex[4];								// テクスチャなし座標データ

	int		m_nAnimCnt;
	int		m_nAnimFrame;															// アニメーション切り替えフレーム
	int		m_nTexPatternNum;														// テクスチャパターン数
	
	int		m_nPatternNumU , m_nPatternNumV;										// テクスチャのUV方向のパターン数
	int		m_nCurPatternU , m_nCurPatternV;										// 今のUVテクスチャパターン

	virtual void	AnimUpdate();													// アニメーション更新

private:
	
};
//=================================================================================


//=================================================================================
//	End of File
//=================================================================================