//**プログラムヘッダ***************************************************************
//	プログラム概要	:	2Dオブジェクトベース
//*********************************************************************************


//==include部======================================================================
#include "../../System/GameManager.h"
#include "Obj2D.h"
//=================================================================================


//==定数・列挙型部=================================================================

//=================================================================================


//==マクロ定義部===================================================================

//=================================================================================


//==クラス・構造体宣言部===========================================================

//=================================================================================


//==静的メンバ変数部===============================================================

//=================================================================================


//==プロトタイプ宣言===============================================================

//=================================================================================


//**関数***************************************************************************
//	概要	:	コンストラクタ
//*********************************************************************************
CObj2D::CObj2D():
m_pTexture(NULL),
m_nAnimCnt(0),
m_nAnimFrame(1),
m_nTexPatternNum(1),
m_nPatternNumU(1),
m_nPatternNumV(1),
m_nCurPatternU(0),
m_nCurPatternV(0)
{
	m_nClassID = eClassCode::OBJ_2D;
	m_nRSType = m_nRSTypeLate = eRSType::OBJ_2D;				// レンダーステートタイプ設定

	m_HitRect.m_fWidth = 0.0f;
	m_HitRect.m_fHeight = 0.0f;
	m_HitRect.m_fDepth = 0.0f;
	m_HitRect.m_nPointType = eHitRect::CENTER;
}


//**関数***************************************************************************
//	概要	:	デストラクタ
//*********************************************************************************
CObj2D::~CObj2D()
{

}


//**関数***************************************************************************
//	概要	:	生成
//*********************************************************************************
CObj2D* CObj2D::Create(LPCTSTR texName , D3DXVECTOR3 pos , 
		 int nWidth , int nHeight , int nSceneID , int nPriority)
{
	CObj2D*		pObj = new CObj2D;

	if(pObj->Init(texName , pos , nWidth , nHeight , nSceneID , nPriority))
		return pObj;

	SAFE_DELETE(pObj);
	return NULL;
}


//**関数***************************************************************************
//	概要	:	生成
//*********************************************************************************
CObj2D* CObj2D::Create(LPCTSTR texName , D3DXVECTOR3 pos , 
		D3DXVECTOR3 rot , D3DXVECTOR3 scall , int nWidth , 
		int nHeight , int nSceneID , int nPriority)
{
	CObj2D*		pObj = new CObj2D;

	if(pObj->Init(texName , pos , rot , scall , nWidth , nHeight , nSceneID , nPriority))
		return pObj;

	SAFE_DELETE(pObj);
	return NULL;
}


//**関数***************************************************************************
//	概要	:	初期化
//*********************************************************************************
bool CObj2D::Init(LPCTSTR texName , D3DXVECTOR3 pos , 
		int nWidth , int nHeight , int nSceneID , int nPriority)
{
	return Init(texName , pos , DEFAULT_ROT , DEFAULT_SCALL , nWidth , 
		nHeight , nSceneID , nPriority);
}


//**関数***************************************************************************
//	概要	:	初期化
//*********************************************************************************
bool CObj2D::Init(LPCTSTR texName , D3DXVECTOR3 pos , 
		D3DXVECTOR3 rot , D3DXVECTOR3 scall , int nWidth , 
		int nHeight , int nSceneID , int nPriority)
{
	// --初期値格納--
	if(texName != NULL)
	{
		m_pTexture = MANAGER.GetTexManage()->Entry(texName);
	}

	m_Pos = pos;								// 位置
	m_Rot = rot;								// 回転角
	m_Scall = scall;							// 拡大率
	
	m_HitRect.m_fWidth = (float)nWidth;			// 幅
	m_HitRect.m_fHeight = (float)nHeight;		// 高さ

	m_nSceneID = nSceneID;						// シーンID
	m_nPriority = nPriority;					// プライオリティ

	// --当たり用半径--
	if(m_HitRect.m_fWidth > m_HitRect.m_fHeight)
		m_fRadius = m_HitRect.m_fWidth / 2.0f;
	else
		m_fRadius = m_HitRect.m_fHeight / 2.0f;

	// --ポリゴン初期化--
	SetTex();
	SetVtx();
	SetDiffuse(255 , 255 , 255 , 255);

	Affine();

	return true;
}


//**関数***************************************************************************
//	概要	:	解放
//*********************************************************************************
void CObj2D::Release()
{
	Uninit();
	delete this;
}


//**関数***************************************************************************
//	概要	:	更新
//*********************************************************************************
void CObj2D::Update()
{
	AnimUpdate();
}


//**関数***************************************************************************
//	概要	:	アニメーション更新
//*********************************************************************************
void CObj2D::AnimUpdate()
{
	m_nAnimCnt ++;

	if(m_nAnimFrame <= m_nAnimCnt)
	{
		m_nCurPatternU ++;
		if(m_nCurPatternU >= m_nPatternNumU)
		{
			m_nCurPatternU = 0;
			m_nCurPatternV ++;			

			if(m_nCurPatternV >= m_nPatternNumV)
				m_nCurPatternV = 0;
		}

		if(m_nTexPatternNum <= m_nCurPatternU + m_nCurPatternV * m_nPatternNumU)
				m_nCurPatternU = m_nCurPatternV = 0;

		m_nAnimCnt = 0;

		SetTex();									// テクスチャ位置変更
	}
}


//**関数***************************************************************************
//	概要	:	アフィン変換
//*********************************************************************************
void CObj2D::Affine()
{
	// 使っていないときは処理なし
	if(!m_bUseFlg) return;

	D3DXMATRIX	matTrans , matRot , matScall;

	// 初期化
	D3DXMatrixIdentity(&matTrans);
	D3DXMatrixIdentity(&matRot);
	D3DXMatrixIdentity(&matScall);

	// アフィン
	D3DXMatrixTranslation(&matTrans , m_Pos.x , m_Pos.y , m_Pos.z);			// 移動
	D3DXMatrixRotationYawPitchRoll(&matRot , m_Rot.y , m_Rot.x , m_Rot.z);	// 回転
	D3DXMatrixScaling(&matScall , m_Scall.x , m_Scall.y , m_Scall.z);		// 拡縮

	m_Matrix = matRot * matScall * matTrans;


	for(int n = 0 ; n < 4 ; n ++)
	{
		D3DXVec3TransformCoord((D3DXVECTOR3*)&m_vVtx[n].vtx , (D3DXVECTOR3*) &m_vVtxBase[n].vtx , &m_Matrix);
		D3DXVec3TransformCoord((D3DXVECTOR3*)&m_vVtxNTex[n].vtx , (D3DXVECTOR3*) &m_vVtxNTexBase[n].vtx , &m_Matrix);
	}
}


//**関数***************************************************************************
//	概要	:	レンダーステート設定
//*********************************************************************************
void CObj2D::SetRS()
{
	CGraphics::SemafoLock();
	LPDIRECT3DDEVICE9 pDevice = CGraphics::GetDevice();
	
	// --2D用設定に変更--
	// ライトオフ
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// アルファ ブレンド有効化
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// Zバッファオフ
	pDevice->SetRenderState(D3DRS_ZENABLE , FALSE);
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetRenderState(D3DRS_CULLMODE , D3DCULL_CCW);

	CGraphics::SemafoUnlock();
}


//**関数***************************************************************************
//	概要	:	後描画用レンダーステート設定
//*********************************************************************************
void CObj2D::SetRSLate()
{
	CGraphics::SemafoLock();

	LPDIRECT3DDEVICE9 pDevice = CGraphics::GetDevice();

	// --2D用設定に変更--
	// ライトオフ
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);
	// アルファ ブレンド有効化
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// Zバッファオフ
	pDevice->SetRenderState(D3DRS_ZENABLE , FALSE);

	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetRenderState(D3DRS_CULLMODE , D3DCULL_CCW);

	CGraphics::SemafoUnlock();
}


//**関数***************************************************************************
//	概要	:	描画
//*********************************************************************************
void CObj2D::Draw()
{
	// 使用フラグがオフまたは後描画フラグがオンなら描画しない
	if(!m_bUseFlg || m_bDrawLate) return;

	CGraphics::SemafoLock();
	LPDIRECT3DDEVICE9 pDevice = CGraphics::GetDevice();

	pDevice->SetTransform(D3DTS_WORLD , &m_Matrix);
	
	if(m_pTexture) 
	{
		pDevice->SetFVF(FVF_VERTEX_2D);
		pDevice->SetTexture(0 , *m_pTexture);				// テクスチャセット
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP , 2 , m_vVtx , sizeof(VERTEX_2D));
	}
	else
	{
		pDevice->SetFVF(FVF_VERTEX_2D_TEXNONE);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP , 2 , m_vVtxNTex , sizeof(VERTEX_2D_TEXNONE));
	}
	
	CGraphics::SemafoUnlock();
}


//**関数***************************************************************************
//	概要	:	描画
//*********************************************************************************
void CObj2D::DrawLate()
{
	// 使用フラグがオフまたは後描画フラグがオフなら描画しない
	if(!m_bUseFlg || ! m_bDrawLate) return;
	
	CGraphics::SemafoLock();

	LPDIRECT3DDEVICE9 pDevice = CGraphics::GetDevice();
	pDevice->SetTransform(D3DTS_WORLD , &m_Matrix);

	if(m_pTexture) 
	{
		pDevice->SetFVF(FVF_VERTEX_2D);
		pDevice->SetTexture(0 , *m_pTexture);			// テクスチャセット
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP , 2 , m_vVtx , sizeof(VERTEX_2D));
	}
	else
	{
		pDevice->SetFVF(FVF_VERTEX_2D_TEXNONE);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP , 2 , m_vVtxNTex , sizeof(VERTEX_2D_TEXNONE));
	}
	
	CGraphics::SemafoUnlock();
}


//**関数***************************************************************************
//	概要	:	テクスチャセット
//*********************************************************************************
void CObj2D::SetTex()
{
	float fPerU = 1.0f / m_nPatternNumU;
	float fPerV = 1.0f / m_nPatternNumV;

	//テクスチャ貼り付け
	m_vVtx[0].tex = D3DXVECTOR2(0.0f  + m_nCurPatternU * fPerU , 0.0f  + m_nCurPatternV * fPerV);
	m_vVtx[1].tex = D3DXVECTOR2(fPerU + m_nCurPatternU * fPerU , 0.0f  + m_nCurPatternV * fPerV);
	m_vVtx[2].tex = D3DXVECTOR2(0.0f  + m_nCurPatternU * fPerU , fPerV + m_nCurPatternV * fPerV);
	m_vVtx[3].tex = D3DXVECTOR2(fPerU + m_nCurPatternU * fPerU , fPerV + m_nCurPatternV * fPerV);
}


//**関数***************************************************************************
//	概要	:	頂点情報セット
//*********************************************************************************
void CObj2D::SetVtx()
{
	//頂点座標
	m_vVtx[0].vtx = m_vVtxBase[0].vtx = D3DXVECTOR4(- m_HitRect.m_fWidth / 2 , - m_HitRect.m_fHeight / 2 , 0.0 , 1.0);
	m_vVtx[1].vtx = m_vVtxBase[1].vtx = D3DXVECTOR4(  m_HitRect.m_fWidth / 2 , - m_HitRect.m_fHeight / 2 , 0.0 , 1.0);
	m_vVtx[2].vtx = m_vVtxBase[2].vtx = D3DXVECTOR4(- m_HitRect.m_fWidth / 2 ,   m_HitRect.m_fHeight / 2 , 0.0 , 1.0);
	m_vVtx[3].vtx = m_vVtxBase[3].vtx = D3DXVECTOR4(  m_HitRect.m_fWidth / 2 ,   m_HitRect.m_fHeight / 2 , 0.0 , 1.0);

	
	m_vVtxNTex[0].vtx = m_vVtxNTexBase[0].vtx = D3DXVECTOR4(- m_HitRect.m_fWidth / 2 , - m_HitRect.m_fHeight / 2 , 0.0 , 1.0);
	m_vVtxNTex[1].vtx = m_vVtxNTexBase[1].vtx = D3DXVECTOR4(  m_HitRect.m_fWidth / 2 , - m_HitRect.m_fHeight / 2 , 0.0 , 1.0);
	m_vVtxNTex[2].vtx = m_vVtxNTexBase[2].vtx = D3DXVECTOR4(- m_HitRect.m_fWidth / 2 ,   m_HitRect.m_fHeight / 2 , 0.0 , 1.0);
	m_vVtxNTex[3].vtx = m_vVtxNTexBase[3].vtx = D3DXVECTOR4(  m_HitRect.m_fWidth / 2 ,   m_HitRect.m_fHeight / 2 , 0.0 , 1.0);
}


//**関数***************************************************************************
//	概要	:	頂点情報セット
//*********************************************************************************
void CObj2D::SetVtx(D3DXVECTOR4* vtx)
{
	//頂点座標
	m_vVtx[0].vtx = m_vVtxBase[0].vtx = vtx[0];
	m_vVtx[1].vtx = m_vVtxBase[1].vtx = vtx[1];
	m_vVtx[2].vtx = m_vVtxBase[2].vtx = vtx[2];
	m_vVtx[3].vtx = m_vVtxBase[3].vtx = vtx[3];

	m_vVtxNTex[0].vtx = m_vVtxNTexBase[0].vtx = vtx[0];
	m_vVtxNTex[1].vtx = m_vVtxNTexBase[1].vtx = vtx[1];
	m_vVtxNTex[2].vtx = m_vVtxNTexBase[2].vtx = vtx[2];
	m_vVtxNTex[3].vtx = m_vVtxNTexBase[3].vtx = vtx[3];
}


//**関数***************************************************************************
//	概要	:	色設定
//*********************************************************************************
void CObj2D::SetDiffuse(int r , int g , int b , int a)
{
	//宣言部
	int 	nLoopCnt;

	for(nLoopCnt = 0 ; nLoopCnt < 4 ; nLoopCnt ++)
	{
		m_vVtx[nLoopCnt].diffuse = D3DCOLOR_RGBA(r , g , b , a);		//色指定
		m_vVtxNTex[nLoopCnt].diffuse = D3DCOLOR_RGBA(r , g , b , a);		//色指定
	}
}


//**関数***************************************************************************
//	概要	:	アニメーション情報セット
//*********************************************************************************
void CObj2D::SetAnimParam(int nAnimFrame , int nTexPatternNum , int nUPatternNum , int nVPatternNum)
{
	// アニメーション情報初期化
	m_nAnimCnt = 0;
	m_nCurPatternU = 0;
	m_nCurPatternV = 0;

	// アニメーション情報セット
	m_nAnimFrame = nAnimFrame;
	m_nTexPatternNum = nTexPatternNum;
	m_nPatternNumU = nUPatternNum;
	m_nPatternNumV = nVPatternNum;

	// アニメーション対応テクスチャ情報に書き換え
	SetTex();
}


//**関数***************************************************************************
//	概要	:	あたり判定矩形取得
//*********************************************************************************
CHitRect CObj2D::GetHitRect()
{
	m_HitRect.m_Point = m_Pos;
	return m_HitRect;
}


//**関数***************************************************************************
//	概要	:	あたり判定矩形セット
//*********************************************************************************
void CObj2D::SetHitRect(CHitRect rect)
{
	m_HitRect = rect;

	SetVtx();
}


//**関数***************************************************************************
//	概要	:	リソーステクスチャセット
//*********************************************************************************
void CObj2D::SetResTexture(TCHAR* texName)
{
	m_pTexture = MANAGER.GetTexManage()->Entry(texName);
}


//**関数***************************************************************************
//	概要	:	リソーステクスチャセット
//*********************************************************************************
void CObj2D::SetResTexture(LPDIRECT3DTEXTURE9* tex)
{
	m_pTexture = tex;
}

//=================================================================================
//	End of File
//=================================================================================