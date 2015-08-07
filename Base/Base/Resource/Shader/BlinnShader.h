//プログラムヘッダ*****************************************************************
//	プログラム概要	:	ブリンシェーダークラスヘッダ
//*********************************************************************************
#pragma once

//==include部======================================================================
#include "Shader.h"
//=================================================================================


//==定数・列挙型宣言部=============================================================

//=================================================================================


//==マクロ定義部===================================================================

//=================================================================================


//==クラス・構造体宣言部===========================================================
class CBlinnShader : public CShader
{
protected:
	D3DXHANDLE	m_hMatWVP;		// ワールド×ビュー×射影
	D3DXHANDLE	m_hMatW;		// ワールド マトリックス
	D3DXHANDLE	m_hCamera;		// 視点座標
	D3DXHANDLE	m_hLight;		// 光源ベクトル
	D3DXHANDLE	m_hDiffuse;		// (マテリアル)拡散色
	D3DXHANDLE	m_hSpecular;	// (マテリアル)鏡面反射色
	D3DXHANDLE	m_hPower;		// (マテリアル)鏡面反射指数
	D3DXHANDLE	m_hAmbient;		// (マテリアル)環境色
	D3DXHANDLE	m_hTexture;		// テクスチャ
	D3DXHANDLE	m_hTexEnable;	// テクスチャ有無

	D3DXVECTOR3	m_vLight;
	D3DXVECTOR3	m_vCamera;
	D3DXMATRIX	m_mView;
	D3DXMATRIX	m_mProj;

public:
	CBlinnShader();
	virtual	~CBlinnShader();
	
	static CBlinnShader*	Create(LPCTSTR pFxName , int nType , UINT unTech);
	virtual	bool		Init(LPCTSTR pFxName , int nType , UINT unTech);

	virtual	UINT		BeginShader();					// シェーダ開始(パスの数を返す)
	virtual void		SetCamera(LPD3DXVECTOR3 pCamera , LPD3DXMATRIX pView , LPD3DXMATRIX pProj);
	virtual void		SetLight(LPD3DXVECTOR3 pLight);
	virtual void		SetWorldMatrix(LPD3DXMATRIX pWorld);
	virtual void		SetMaterial(D3DMATERIAL9* pMaterial , LPDIRECT3DTEXTURE9 pTexture);
};
//=================================================================================


//=================================================================================
//	End of File
//=================================================================================