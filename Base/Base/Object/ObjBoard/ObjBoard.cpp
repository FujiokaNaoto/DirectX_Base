//**プログラムヘッダ***************************************************************
//	プログラム概要	:	ワールド上板ポリゴンベース
//*********************************************************************************


//==include部======================================================================
#include "../../System/GameManager.h"

#include "ObjBoard.h"
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
CObjBoard::CObjBoard():
m_pTexture(NULL) , 
m_nWidth(0),
m_nHeight(0),
m_bBillFlg(true),

m_nAnimCnt(0),
m_nAnimFrame(1),
m_nTexPatternNum(1),
m_nPatternNumU(1),
m_nPatternNumV(1),
m_nCurPatternU(0),
m_nCurPatternV(0)
{
	m_nClassID = eClassCode::OBJ_BOARD;
	m_nRSType = m_nRSTypeLate = eRSType::BILLBOARD;
}


//**関数***************************************************************************
//	概要	:	デストラクタ
//*********************************************************************************
CObjBoard::~CObjBoard()
{

}


//**関数***************************************************************************
//	概要	:	生成
//*********************************************************************************
CObjBoard* CObjBoard::Create(LPCTSTR texName , D3DXVECTOR3 pos , 
		D3DXVECTOR3 rot , D3DXVECTOR3 scall , int nWidth , 
		int nHeight , int nSceneID , int nPriority)
{
	CObjBoard*		pObj = new CObjBoard;

	if(pObj->Init(texName , pos , rot , scall , nWidth , nHeight ,
		nSceneID , nPriority))
		return pObj;

	SAFE_DELETE(pObj);
	return NULL;
}


//**関数***************************************************************************
//	概要	:	生成
//*********************************************************************************
CObjBoard* CObjBoard::Create(LPCTSTR texName , D3DXVECTOR3 pos , 
	 int nWidth , int nHeight , int nSceneID , int nPriority)
{
	CObjBoard*		pObj = new CObjBoard;

	if(pObj->Init(texName , pos , nWidth , nHeight , nSceneID , nPriority))
		return pObj;

	SAFE_DELETE(pObj);
	return NULL;
}


//**関数***************************************************************************
//	概要	:	初期化
//*********************************************************************************
bool CObjBoard::Init(LPCTSTR texName , D3DXVECTOR3 pos , 
		int nWidth , int nHeight , int nSceneID , int nPriority)
{
	return Init(texName , pos , DEFAULT_ROT , DEFAULT_SCALL , nWidth , nHeight ,
		nSceneID , nPriority);
}


//**関数***************************************************************************
//	概要	:	初期化
//*********************************************************************************
bool CObjBoard::Init(LPCTSTR texName , D3DXVECTOR3 pos , 
	D3DXVECTOR3 rot , D3DXVECTOR3 scall , int nWidth , 
	int nHeight , int nSceneID , int nPriority)
{
	m_pTexture = MANAGER.GetTexManage()->Entry(texName);

	if(! m_pTexture)	return false;			// 登録失敗時はNULLを返す
	m_Pos = pos;
	m_Rot = rot;
	m_Scall = scall;
	m_nWidth = nWidth;
	m_nHeight = nHeight;

	m_nSceneID = nSceneID;
	m_nPriority = nPriority;

	// --当たり用半径--
	if(m_nWidth > m_nHeight)
		m_fRadius = nWidth / 2.0f;
	else
		m_fRadius = nHeight / 2.0f;

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
void CObjBoard::Release()
{
	Uninit();
	delete this;
}


//**関数***************************************************************************
//	概要	:	アフィン変換
//*********************************************************************************
void CObjBoard::Affine()
{
	D3DXMATRIX	matTrans , matRot , matScall , matBill;

	// 初期化
	D3DXMatrixIdentity(&matTrans);
	D3DXMatrixIdentity(&matRot);
	D3DXMatrixIdentity(&matScall);		

	// ビルボードをする
	if(m_bBillFlg)
	{
		D3DXMatrixIdentity(&matBill);

		// カメラ情報を取得
		CCamera* pCamera = MANAGER.GetCamera();
		D3DXMATRIX	matCamera;
		D3DXMatrixIdentity(&matCamera);
		D3DXMatrixLookAtLH(&matCamera , &pCamera->GetPos() , &pCamera->GetLook() , &pCamera->GetUp());

		// 転置行列を作る
		matBill._11 = matCamera._11;
		matBill._12 = matCamera._21;
		matBill._13 = matCamera._31;

		matBill._21 = matCamera._12;
		matBill._22 = matCamera._22;
		matBill._23 = matCamera._32;

		matBill._31 = matCamera._13;
		matBill._32 = matCamera._23;
		matBill._33 = matCamera._33;

		// 移動情報を解除
		matBill._41 = 0.0f;
		matBill._42 = 0.0f;
		matBill._43 = 0.0f;

		// 拡大
		D3DXMatrixScaling(&matScall , m_Scall.x , m_Scall.y , m_Scall.z);
		// 移動
		D3DXMatrixTranslation(&matTrans , m_Pos.x , m_Pos.y , m_Pos.z);

		// ワールドマトリクスにセット
		m_Matrix = matScall * matBill * matTrans;
	}

	// ビルボードをしない
	else
	{
		// 回転
		D3DXMatrixRotationYawPitchRoll(&matRot , 
			m_Rot.y , m_Rot.x , m_Rot.z);
		// 拡大
		D3DXMatrixScaling(&matScall , m_Scall.x , m_Scall.y , m_Scall.z);
		// 移動
		D3DXMatrixTranslation(&matTrans , m_Pos.x , m_Pos.y , m_Pos.z);

		// ワールドマトリクスにセット
		m_Matrix = matScall * matRot * matTrans;
	}
}


//**関数***************************************************************************
//	概要	:	レンダーステート設定
//*********************************************************************************
void CObjBoard::SetRS()
{
	CGraphics::SemafoLock();

	LPDIRECT3DDEVICE9 pDevice = MANAGER.GetGraph()->GetDevice();

	pDevice->SetFVF(FVF_VERTEX_BOARD);

	pDevice->SetRenderState(D3DRS_ZENABLE , TRUE);						// Zバッファオン
	pDevice->SetRenderState(D3DRS_LIGHTING , FALSE);					// ライトオフ
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// アルファブレンドあり
	
	// 合成パターン
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	//pDevice->SetRenderState(D3DRS_CULLMODE , D3DCULL_CCW);

	CGraphics::SemafoUnlock();
}


//**関数***************************************************************************
//	概要	:	レンダーステート設定
//*********************************************************************************
void CObjBoard::SetRSLate()
{
	CGraphics::SemafoLock();

	// グラフィックデバイス取得
	LPDIRECT3DDEVICE9 pDevice = MANAGER.GetGraph()->GetDevice();

	// 頂点ストリーム設定
	pDevice->SetFVF(FVF_VERTEX_BOARD);

	// レンダーステート設定
	pDevice->SetRenderState(D3DRS_ZENABLE , TRUE);						// Zバッファオン
	pDevice->SetRenderState(D3DRS_LIGHTING , FALSE);					// ライトオフ
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// アルファブレンドあり
	
	// 合成パターン
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	
	//pDevice->SetRenderState(D3DRS_CULLMODE , D3DCULL_CCW);

	CGraphics::SemafoUnlock();
}


//**関数***************************************************************************
//	概要	:	描画
//*********************************************************************************
void CObjBoard::Draw()
{
	// 描画フラグがオフまたは後描画フラグがオンなら描画しない
	if(!m_bUseFlg || m_bDrawLate) return;

	CGraphics::SemafoLock();

	LPDIRECT3DDEVICE9 pDevice = MANAGER.GetGraph()->GetDevice();

	pDevice->SetTransform(D3DTS_WORLD, &m_Matrix);

	if(m_pTexture)
	{
		pDevice->SetFVF(FVF_VERTEX_BOARD);
		pDevice->SetTexture(0 , *m_pTexture);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP , 2 , m_vVtx , sizeof(VERTEX_BOARD));
	}
	else
	{
		pDevice->SetFVF(FVF_VERTEX_BOARD_TEXNONE);
		pDevice->SetTexture(0 , *m_pTexture);
		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP , 2 , m_vVtx_NTex , sizeof(VERTEX_BOARD_TEXNONE));
	}


	CGraphics::SemafoUnlock();
}


//**関数***************************************************************************
//	概要	:	後描画
//*********************************************************************************
void CObjBoard::DrawLate()
{
	// 描画フラグがオフまたは後描画フラグがオフなら描画しない
	if(!m_bUseFlg || ! m_bDrawLate) return;		

	CGraphics::SemafoLock();

	// グラフィックデバイス取得
	LPDIRECT3DDEVICE9 pDevice = MANAGER.GetGraph()->GetDevice();

	pDevice->SetTexture(0 , *m_pTexture);								// テクスチャセット
	pDevice->SetTransform(D3DTS_WORLD, &m_Matrix);						// アフィン
	pDevice->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP , 2 , m_vVtx , sizeof(VERTEX_BOARD));

	CGraphics::SemafoUnlock();
}


//**関数***************************************************************************
//	概要	:	テクスチャセット
//*********************************************************************************
void CObjBoard::SetTex()
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
void CObjBoard::SetVtx()
{
	//頂点座標
	m_vVtx[0].vtx = D3DXVECTOR3(- (float)m_nWidth / 2 ,   (float)m_nHeight / 2 , 0.0);
	m_vVtx[1].vtx = D3DXVECTOR3(  (float)m_nWidth / 2 ,   (float)m_nHeight / 2 , 0.0);
	m_vVtx[2].vtx = D3DXVECTOR3(- (float)m_nWidth / 2 , - (float)m_nHeight / 2 , 0.0);
	m_vVtx[3].vtx = D3DXVECTOR3(  (float)m_nWidth / 2 , - (float)m_nHeight / 2 , 0.0);

	m_vVtx_NTex[0].vtx = D3DXVECTOR3(- (float)m_nWidth / 2 ,   (float)m_nHeight / 2 , 0.0);
	m_vVtx_NTex[1].vtx = D3DXVECTOR3(  (float)m_nWidth / 2 ,   (float)m_nHeight / 2 , 0.0);
	m_vVtx_NTex[2].vtx = D3DXVECTOR3(- (float)m_nWidth / 2 , - (float)m_nHeight / 2 , 0.0);
	m_vVtx_NTex[3].vtx = D3DXVECTOR3(  (float)m_nWidth / 2 , - (float)m_nHeight / 2 , 0.0);
}


//**関数***************************************************************************
//	概要	:	頂点情報セット
//*********************************************************************************
void CObjBoard::SetVtx(D3DXVECTOR3* vtx)
{
	//頂点座標
	m_vVtx[0].vtx = vtx[0];
	m_vVtx[1].vtx = vtx[1];
	m_vVtx[2].vtx = vtx[2];
	m_vVtx[3].vtx = vtx[3];

	m_vVtx_NTex[0].vtx = vtx[0];
	m_vVtx_NTex[1].vtx = vtx[1];
	m_vVtx_NTex[2].vtx = vtx[2];
	m_vVtx_NTex[3].vtx = vtx[3];
}


//**関数***************************************************************************
//	概要	:	色設定
//*********************************************************************************
void CObjBoard::SetDiffuse(int r , int g , int b , int a)
{
	//宣言部
	int 	nLoopCnt;

	for(nLoopCnt = 0 ; nLoopCnt < 4 ; nLoopCnt ++)
	{
		m_vVtx[nLoopCnt].diffuse = D3DCOLOR_RGBA(r , g , b , a);		//色指定
		m_vVtx_NTex[nLoopCnt].diffuse = D3DCOLOR_RGBA(r , g , b , a);		//色指定
	}

}


//**関数***************************************************************************
//	概要	:	アニメーション情報セット
//*********************************************************************************
void CObjBoard::SetAnimParam(int nAnimFrame , int nTexPatternNum , 
		int nUPatternNum , int nVPatternNum)
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
//	概要	:	アニメーション更新
//*********************************************************************************
void CObjBoard::AnimUpdate()
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
//=================================================================================
//	End of File
//=================================================================================