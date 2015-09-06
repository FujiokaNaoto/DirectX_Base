//**プログラムヘッダ***************************************************************
//	プログラム概要	:	DirectX Graphics9 クラス
//*********************************************************************************

//==include部======================================================================
#include <windows.h>
#include "../System/common.h"
#include "Graphics.h"
//=================================================================================


//==定数・列挙型宣言部=============================================================
// フォント名
const TCHAR* FONTNAME[FONT_MAX] = 
{
	_T("Terminal"),
	_T("HG丸ｺﾞｼｯｸM-PRO"),
	_T("HGP創英角ﾎﾟｯﾌﾟ体"),
};

// フォントサイズ
const int	FONTSIZE[FONT_MAX] = 
{
	18,
	30,
	40,
};

namespace eGraphics
{
	const LPCTSTR	SEMAFO_NAME = _T("DEV_SEMAFO_NAME");		// セマフォ名
	const int		SEMAFO_START_CNT = 1;						// セマフォ初期カウント
	const int		SEMAFO_MAX_CNT = 1;							// セマフォ最大カウント

};
//=================================================================================


//==静的メンバ変数部===============================================================
LPDIRECT3DDEVICE9	CGraphics::m_pDevice = NULL;		// Direct3DDevice9 オブジェクト
D3DXMATRIX			CGraphics::m_MatProj;

HANDLE				CGraphics::m_SemafObjHnd = 0;	
HANDLE				CGraphics::m_SemafHnd = 0;		
//=================================================================================


//==マクロ定義部===================================================================

//=================================================================================


//**関数***************************************************************************
//	概要	:	グラフィック環境 の初期化
//*********************************************************************************
bool CGraphics::Initialize(HWND hWnd, int nWidth, int nHeight, bool bWindow)
{
	// デバイス情報テーブル
	struct TCreateDevice {
		D3DDEVTYPE	type;			// デバイスの種類
		DWORD		behavior;		// デバイスの動作
	};
	const int c_nMaxDevice = 3;
	const TCreateDevice device[c_nMaxDevice] = {
		{ D3DDEVTYPE_HAL, D3DCREATE_HARDWARE_VERTEXPROCESSING },	// ハードウェアシェーダを使う
		{ D3DDEVTYPE_HAL, D3DCREATE_SOFTWARE_VERTEXPROCESSING },	// HAL
		{ D3DDEVTYPE_REF, D3DCREATE_SOFTWARE_VERTEXPROCESSING },	// REF
	};
	int		nDev;			// デバイス種別カウンタ

	// Direct3Dオブジェクトを生成
	if ((m_pD3D = Direct3DCreate9(D3D_SDK_VERSION)) == NULL) {
		MessageBox(hWnd, _T("Direct3Dオブジェクトの作成に失敗しました"), _T("error"), MB_OK | MB_ICONERROR);
		return false;
	}
	// Direct3D 初期化パラメータの設定
	m_nWidth = nWidth;
	m_nHeight = nHeight;
	ZeroMemory(&m_D3DPP, sizeof(D3DPRESENT_PARAMETERS));
	m_D3DPP.BackBufferWidth        = (UINT)nWidth;				// ゲーム画面サイズ
	m_D3DPP.BackBufferHeight       = (UINT)nHeight;
	m_D3DPP.Windowed               = bWindow;					// ウィンドウモードか?
	m_D3DPP.SwapEffect             = D3DSWAPEFFECT_DISCARD;
	m_D3DPP.EnableAutoDepthStencil = true;						// Ｚバッファ有効
	m_D3DPP.AutoDepthStencilFormat = D3DFMT_D24S8;				// Ｚバッファフォーマット
	if (bWindow) 
	{
		// ウィンドウモード
		m_D3DPP.BackBufferFormat           = D3DFMT_UNKNOWN;				// バックバッファ
		m_D3DPP.FullScreen_RefreshRateInHz = 0;								// リフレッシュレート
		m_D3DPP.PresentationInterval       = D3DPRESENT_INTERVAL_IMMEDIATE;	// インターバル
	} else {
		// フルスクリーンモード
		m_D3DPP.BackBufferFormat           = D3DFMT_X8R8G8B8;				// バックバッファ
		m_D3DPP.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;		// リフレッシュレート
		m_D3DPP.PresentationInterval       = D3DPRESENT_INTERVAL_DEFAULT;	// インターバル
	}

	// デバイスオブジェクトを作成
	for (nDev = 0; nDev < c_nMaxDevice; nDev++) {
		if (SUCCEEDED(m_pD3D->CreateDevice(D3DADAPTER_DEFAULT, device[nDev].type, hWnd,	// デバイスを作成
					  device[nDev].behavior, &m_D3DPP, &m_pDevice))) break;
	}
	if (nDev >= c_nMaxDevice) {
		MessageBox(hWnd, _T("デバイスの作成に失敗しました"), _T("error"), MB_OK | MB_ICONERROR);
		return false;
	}

	// フォントを設定
	FontCreate(FONT_DEBUG);
	FontCreate(FONT_MESSAGE);
	FontCreate(FONT_SCORE);

	//-------- ここから３Ｄ関連
	// ビューマトリックス設定をシーンで設定
	
	// プロジェクションマトリックス設定
	D3DXMatrixPerspectiveFovLH(&m_MatProj, D3DXToRadian(FOVY),
		(float)m_nWidth / m_nHeight, NEAR_CLIP, FAR_CLIP);
	m_pDevice->SetTransform(D3DTS_PROJECTION, &m_MatProj);

	// マテリアルの設定
	D3DMATERIAL9	mtrl;				// マテリアルオブジェクト
	ZeroMemory(&mtrl, sizeof(mtrl));	// いったんゼロでクリア
	mtrl.Diffuse.r = mtrl.Diffuse.g = mtrl.Diffuse.b = mtrl.Diffuse.a = 1.0f;
	mtrl.Ambient = mtrl.Diffuse;
	m_pDevice->SetMaterial(&mtrl);

	// ライトの設定
	D3DXVECTOR3	vecDir;					// 方向を指定するベクトル
	
	ZeroMemory(&m_Light, sizeof(m_Light));
	m_Light.Type = D3DLIGHT_DIRECTIONAL;	// 平行光源
	m_Light.Diffuse.r = m_Light.Diffuse.g = m_Light.Diffuse.b = m_Light.Diffuse.a = 1.0f;
	m_Light.Ambient.r = m_Light.Ambient.g = m_Light.Ambient.b = m_Light.Ambient.a = 0.2f;
	m_Light.Specular = m_Light.Diffuse;
	vecDir = D3DXVECTOR3(1.0f, -1.0f, 1.0f);					// 光の向きを決める
	D3DXVec3Normalize((D3DXVECTOR3*)&m_Light.Direction, &vecDir);	// 正規化したベクトルを返す
	m_pDevice->SetLight(0, &m_Light);
	m_pDevice->LightEnable(0, TRUE);	// ライト0を有効

	m_pDevice->SetRenderState(D3DRS_AMBIENTMATERIALSOURCE, D3DMCS_MATERIAL);
	m_pDevice->SetRenderState(D3DRS_DIFFUSEMATERIALSOURCE, D3DMCS_MATERIAL);
	m_pDevice->SetRenderState(D3DRS_SPECULARMATERIALSOURCE, D3DMCS_MATERIAL);
	m_pDevice->SetRenderState(D3DRS_EMISSIVEMATERIALSOURCE, D3DMCS_MATERIAL);
	m_pDevice->SetSamplerState(0 , D3DSAMP_MAGFILTER , D3DTEXF_LINEAR);
	m_pDevice->SetSamplerState(0 , D3DSAMP_MINFILTER , D3DTEXF_LINEAR);

	// アルファ値はマテリアル×テクスチャに設定
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_DIFFUSE);
	m_pDevice->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_TEXTURE);


	// --セマフォ生成--
	m_SemafObjHnd = 0;
	m_SemafObjHnd = 0;
	m_SemafObjHnd = CreateSemaphore(NULL , eGraphics::SEMAFO_START_CNT , 
		eGraphics::SEMAFO_MAX_CNT , eGraphics::SEMAFO_NAME);

	return true;
}


//**関数***************************************************************************
//	概要	:	グラフィック環境のクリーンアップ
//*********************************************************************************
void CGraphics::Finalize()
{
	for(int i = 0 ; i < FONT_MAX ; i ++)
		if(m_pFont[i]) SAFE_RELEASE(m_pFont[i]);	// D3DXFont オブジェクトを解放

	SAFE_RELEASE(m_pDevice);						// Direct3DDevice オブジェクトを解放
	SAFE_RELEASE(m_pD3D);							// Direct3D オブジェクトを解放
}


//**関数***************************************************************************
//	概要	:	描画クラス生成
//*********************************************************************************
CGraphics* CGraphics::Create(HWND hWnd, int nWidth, int nHeight, bool bWindow)
{
	CGraphics* pGraph = new CGraphics();
	if (pGraph) {
		if (!pGraph->Initialize(hWnd, nWidth, nHeight, bWindow)) {
			SAFE_DELETE(pGraph);
		}
	}
	return pGraph;
}


//**関数***************************************************************************
//	概要	:	描画クラス解放
//*********************************************************************************
void CGraphics::Release()
{
	Finalize();
	delete this;
}


//**関数***************************************************************************
//	概要	:	レンダリング開始
//*********************************************************************************
bool CGraphics::Begin()
{
	// ビューポートをクリア
	m_pDevice->Clear(0, NULL, D3DCLEAR_TARGET | D3DCLEAR_ZBUFFER | D3DCLEAR_STENCIL,
					 D3DCOLOR_XRGB(128, 128, 255), 1.0f, 0);

	return SUCCEEDED(m_pDevice->BeginScene());	// シーン開始
}


//**関数***************************************************************************
//	概要	:	レンダリング終了
//*********************************************************************************
void CGraphics::End()
{
	m_pDevice->EndScene();						// シーン終了
}


//**関数***************************************************************************
//	概要	:	バックバッファ入替
//*********************************************************************************
void CGraphics::SwapBuffer()
{
	// バックバッファに描画した内容を表示
	if (FAILED(m_pDevice->Present(NULL, NULL, NULL, NULL))) {
		m_pDevice->Reset(&m_D3DPP);
	}
}


//**関数***************************************************************************
//	概要	:	文字列描画(フォント色白)
//*********************************************************************************
void CGraphics::DrawText(int nX, int nY, LPCTSTR psz , int nFontType)
{
	RECT rcStr = {nX, nY, m_nWidth, m_nHeight};
	m_pFont[nFontType]->DrawText(NULL, psz, -1, &rcStr, DT_LEFT, D3DCOLOR_RGBA(0, 0, 0, 255));
}


//**関数***************************************************************************
//	概要	:	文字列描画	(フォント色指定)
//*********************************************************************************
void CGraphics::DrawText(int nX, int nY, LPCTSTR psz , int nFontType ,
						int nRed , int nGreen , int nBlue , int nAlpha)
{
	RECT rcStr = {nX, nY, m_nWidth, m_nHeight};
	m_pFont[nFontType]->DrawText(NULL, psz, -1, &rcStr, DT_LEFT, D3DCOLOR_RGBA(nRed , nGreen , nBlue , nAlpha));
}


//**関数***************************************************************************
//	概要	:	セマフォロック
//*********************************************************************************
void CGraphics::SemafoLock()
{
	m_SemafHnd = OpenSemaphore(SEMAPHORE_ALL_ACCESS , FALSE , eGraphics::SEMAFO_NAME);

	WaitForSingleObject(m_SemafHnd , INFINITE);
}


//**関数***************************************************************************
//	概要	:	セマフォ使用終了
//*********************************************************************************
void CGraphics::SemafoUnlock()
{
	ReleaseSemaphore(m_SemafHnd , 1 , NULL);
}


//**関数***************************************************************************
//	概要	:	文字列描画	(フォント色指定)
//*********************************************************************************
void CGraphics::FontCreate(int nFontType)
{
	D3DXCreateFont(m_pDevice, FONTSIZE[nFontType] , 0, 0, 0,\
	FALSE, SHIFTJIS_CHARSET, OUT_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH, FONTNAME[nFontType] , &m_pFont[nFontType]);
}
//=======================================================================================
//	End of File
//=======================================================================================