//プログラムヘッダ*****************************************************************
//	プログラム概要	:	テクスチャ管理クラス
//*********************************************************************************
#pragma once

//==include部======================================================================
#include <d3d9.h>
#include <d3dx9.h>

#include <vector>
#include <tchar.h>

//=================================================================================


//==定数・列挙型宣言部=============================================================

//=================================================================================


//==マクロ定義部===================================================================
#define TEX_VECT			std::vector<LPDIRECT3DTEXTURE9*>			// テクスチャ配列
#define TEX_VECT_ITE		std::vector<LPDIRECT3DTEXTURE9*>::iterator	// テクスチャ配列へのイテレータ

#define TEXNAME_VECT		std::vector<LPCTSTR>						// テクスチャ名配列
#define TEXNAME_VECT_ITE	std::vector<LPCTSTR>::iterator				// テクスチャ名配列へのイテレータ
//=================================================================================


//==クラス・構造体宣言部===========================================================
class CTextureManage
{
private:
	static TEX_VECT		m_TextureVect;									// ロードされたテクスチャを保持するリスト
	static TEXNAME_VECT	m_TextureNameVect;								// テクスチャの名前を格納しておくリスト

	LPDIRECT3DDEVICE9	m_pDevice;

public:
	// コンストラクタ・デストラクタ
	CTextureManage();
	virtual ~CTextureManage();

	static CTextureManage*	Create();									// 生成
	bool	Initialize();												// 初期化

	LPDIRECT3DTEXTURE9*	Search(LPCTSTR TexName);							// すでにテクスチャが格納されているかを検索
	LPDIRECT3DTEXTURE9*	Entry(LPCTSTR TexName);							// テクスチャをリスト内に登録　
	void	VectorClear();												// 格納配列を完全クリア
	bool	ClearOnce(LPCTSTR TexName);									// 指定リソースの解放
	
	void	Release();													// マネージャ解放
};
//=================================================================================


//=================================================================================
//	End of File
//=================================================================================