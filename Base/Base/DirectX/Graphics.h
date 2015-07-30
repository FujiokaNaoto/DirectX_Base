//プログラムヘッダ*****************************************************************
//	プログラム概要	:	DirectX Graphics9 クラス定義
//*********************************************************************************
//#pragma once

#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

//==include部======================================================================
#include <d3d9.h>
#include <d3dx9.h>
#include <tchar.h>
//=================================================================================


//==定数・列挙型宣言部=============================================================
#define FOVY			45.0f				// 視野角
#define NEAR_CLIP		0.1f				// ニアクリップを行う距離
#define FAR_CLIP		10000.0f			// ファークリップを行う距離

// 登録フォント
enum FONT_TYPE
{
	FONT_DEBUG,								// デバッグ文字フォント
	FONT_MESSAGE,							// ゲーム内メッセージフォント
	FONT_SCORE,								// スコアフォント

	FONT_MAX
};
//=================================================================================


//==マクロ定義部===================================================================

//=================================================================================


//==クラス宣言部===================================================================
class CGraphics
{
private:
	LPDIRECT3D9					m_pD3D;					// Direct3D9 オブジェクト
	static LPDIRECT3DDEVICE9	m_pDevice;				// Direct3DDevice9 オブジェクト
	D3DPRESENT_PARAMETERS		m_D3DPP;				// PRESENT PARAMETERS

	LPD3DXFONT					m_pFont[FONT_MAX];		// D3DXFont オブジェクト

	static D3DXMATRIX			m_MatProj;				// 射影マトリックス

	HWND						m_hWnd;					// ウィンドウ ハンドル
	int							m_nWidth;				// 表示領域幅
	int							m_nHeight;				// 表示領域高さ


	// セマフォ
	static HANDLE				m_SemafObjHnd;			// セマフォオブジェハンドル
	static HANDLE				m_SemafHnd;				// セマフォハンドル

public:
	static void				 SemafoLock();
	static void				 SemafoUnlock();

	static LPDIRECT3DDEVICE9 GetDevice(){return m_pDevice;};
	static D3DXMATRIX		 GetMatProj() {return m_MatProj;}

	static CGraphics* Create(HWND hWnd, int nWidth, int nHeight, bool bWindow);
	void Release();
	bool Begin();
	void End();
	void SwapBuffer();

	void DrawText(int nX, int nY, LPCTSTR psz , int nFontType);			// フォント描画
	void DrawText(int nX, int nY, LPCTSTR psz , int nFontType ,
					int nRed , int nGreen , int nBlue , int nAlpha);	// 色情報込みフォント描画
	
private:
	bool Initialize(HWND hWnd, int nWidth, int nHeight, bool bWindow);
	void Finalize();

	void FontCreate(int FontType);
};
//=================================================================================

#endif
//=================================================================================
//	End of File
//=================================================================================