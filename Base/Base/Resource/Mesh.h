//=======================================================================================
//
//	  メッシュ クラス定義
//
//=======================================================================================
#pragma once
#include <windows.h>		// Windowsプログラムにはこれを付ける
#include <tchar.h>			// 汎用テキスト マッピング
#include <d3d9.h>			// DirectX Graphics 関連のヘッダ
#include <d3dx9.h>			// DirectX Graphics 関連のヘッダ
#include <dxerr.h>			// DirectX Graphics 関連のヘッダ
#include "../Object/Base/CHANDLE.h"

struct BVERTEX {
	D3DXVECTOR3	pos;
	D3DXVECTOR3	nor;
	D3DXVECTOR2	tex;
};
#define FVF_BVERTEX		(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)

struct PARTICLE {
	D3DXVECTOR3 acl;	// 加速度
	D3DXVECTOR3 spd;	// 速度
};

#define INIT_SPD	3.0f	// 初速(要調節)
#define GRAVITY		0.098f	// 重力加速度(要調節)

class CMesh
{
private:
	LPD3DXMESH			m_pD3DMesh;			// D3DXMesh オブジェクト
	DWORD				m_dwNumMaterial;	// マテリアル数
	D3DMATERIAL9*		m_pMaterial;		// マテリアル
	LPDIRECT3DTEXTURE9*	m_ppTexture;		// テクスチャ

	D3DXATTRIBUTERANGE*	m_pAttr;			// 属性テーブル
	DWORD				m_dwAttr;			// 属性数
	DWORD				m_dwVtx;			// 頂点数
	BVERTEX*			m_pVtx;				// 頂点配列
	DWORD				m_dwIdx;			// インデックス数
	WORD*				m_pIdx;				// インデックス配列

	D3DXVECTOR3			m_vHalf;			// 境界ボックス ハーフサイズ
	D3DXVECTOR3			m_vCenter;			// 境界ボックス(境界球)中心座標
	float				m_fRadius;			// 境界球半径

	// デバッグ表示
	LPD3DXMESH			m_pSphere;			// 境界球表示用メッシュ
	LPD3DXMESH			m_pBox;				// OBB表示用メッシュ

	D3DXVECTOR3			m_vOBBHalf;			// OBBデータの半径

	PARTICLE*			m_pPiece;			// 断片の制御用
	BVERTEX*			m_pPieceVtx;
	DWORD				m_dwFace;			// 面数

public:
	CMesh();								// コンストラクタ
	virtual ~CMesh();						// デストラクタ
	bool Initialize(LPCTSTR pszFName);		// メッシュ初期化
	void Finalize();						// メッシュ解放
	void Draw(D3DXMATRIX& world);			// メッシュ描画
	void Draw(D3DXMATRIX& world, float fAlpha);
	DWORD GetNumVertex() {return m_dwVtx;}	// 頂点数取得
	BVERTEX* GetVertex() {return m_pVtx;}	// 頂点配列取得
	DWORD GetNumIndex() {return m_dwIdx;}	// インデックス数取得
	WORD* GetIndex() {return m_pIdx;}		// インデックス配列取得
	D3DXVECTOR3& GetHalfSize() {return m_vHalf;}	// ボックスハーフサイズ取得
	D3DXVECTOR3& GetCenter() {return m_vCenter;}	// 境界ボックス/球中心座標取得
	float GetRadius() {return m_fRadius;}	// 境界球半径取得

	D3DXVECTOR3& GetObbHalf(){return m_vOBBHalf;}	// OBB半分のサイズ取得

	void InitParticle();					// 断片の初期化
	void UpdateParticle();					// 断片の移動

	void DrawSphere(D3DXMATRIX& world, D3DCOLORVALUE color);
	void DrawBox(D3DXMATRIX& world, D3DCOLORVALUE color);

	void DrawNoAlpha(D3DXMATRIX& world , S_HANDLE nShadeHndle);
	void DrawAlpha(D3DXMATRIX& world , S_HANDLE nShadeHndle);
};

//=======================================================================================
//	End of File
//=======================================================================================