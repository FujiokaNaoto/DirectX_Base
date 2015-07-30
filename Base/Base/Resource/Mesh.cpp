//=======================================================================================
//
//	  メッシュ クラス
//
//=======================================================================================
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS
#include <stdio.h>
#include "../System/common.h"

#include "Mesh.h"
#include "../DirectX/Graphics.h"

//**関数***************************************************************************
// コンストラクタ
//*********************************************************************************
CMesh::CMesh()
{
	m_pD3DMesh = NULL;
	m_dwNumMaterial = 0;
	m_pMaterial = NULL;
	m_ppTexture = NULL;

	m_pAttr = NULL;
	m_dwAttr = 0;
	m_dwVtx = 0;
	m_pVtx = NULL;
	m_dwIdx = 0;
	m_pIdx = NULL;

	m_vHalf = D3DXVECTOR3(0, 0, 0);
	m_vCenter = D3DXVECTOR3(0, 0, 0);
	m_fRadius = 0;
	m_pSphere = NULL;
	m_pSphere = NULL;


	m_pPiece = NULL;
	m_pPieceVtx = NULL;
}

//**関数***************************************************************************
// デストラクタ
//*********************************************************************************
CMesh::~CMesh()
{
	Finalize();
}

//**関数***************************************************************************
// メッシュ初期化
//*********************************************************************************
bool CMesh::Initialize(LPCTSTR pszFName)
{
	TCHAR			szMsg[MAX_PATH + 32];
	TCHAR			szDir[_MAX_DIR];
	TCHAR			szCurrentDir[_MAX_PATH];

	LPD3DXBUFFER	pD3DXMtrlBuffer = NULL;

	// フォルダ名を抽出
	_tsplitpath(pszFName, NULL, szDir, NULL, NULL);

	// Ｘファイルからメッシュデータを読み込む
	HRESULT hr = D3DXLoadMeshFromX(pszFName, D3DXMESH_SYSTEMMEM, CGraphics::GetDevice(),
		NULL, &pD3DXMtrlBuffer, NULL, &m_dwNumMaterial, &m_pD3DMesh);
	if (FAILED(hr)) {
		_stprintf(szMsg, _T("Xファイル(%s)の読み込みに失敗しました。"), pszFName);
		MessageBox(NULL, szMsg, NULL, MB_OK);
		return false;
	}

	// FVF形式を補正(頂点フォーマットを変換)
	LPD3DXMESH pMeshTmp;
	DWORD dwFVF = m_pD3DMesh->GetFVF();
	if (dwFVF != FVF_BVERTEX) {
		hr = m_pD3DMesh->CloneMeshFVF(m_pD3DMesh->GetOptions(), FVF_BVERTEX,
			CGraphics::GetDevice(), &pMeshTmp);
		SAFE_RELEASE(m_pD3DMesh);
		if (FAILED(hr)) {
			SAFE_RELEASE(pD3DXMtrlBuffer);
			return false;
		}
		// 法線が無い場合は強制的に追加
		if ((dwFVF & D3DFVF_NORMAL) == 0) {
			D3DXComputeNormals(pMeshTmp, NULL);
		}
		m_pD3DMesh = pMeshTmp;
	}

	// 属性テーブルを生成するための最適化
	hr = m_pD3DMesh->Optimize(D3DXMESHOPT_ATTRSORT, NULL, NULL, NULL, NULL, &pMeshTmp);
	if (SUCCEEDED(hr)) {
		m_pD3DMesh->Release();
		m_pD3DMesh = pMeshTmp;
	} else {
		SAFE_RELEASE(pD3DXMtrlBuffer);
		return false;
	}
	// 属性テーブル取得
	hr = m_pD3DMesh->GetAttributeTable(NULL, &m_dwAttr);
	if (FAILED(hr)) {
		SAFE_RELEASE(pD3DXMtrlBuffer);
		SAFE_RELEASE(m_pD3DMesh);
		return false;
	}
	m_pAttr = new D3DXATTRIBUTERANGE[m_dwAttr];
	hr = m_pD3DMesh->GetAttributeTable(m_pAttr, &m_dwAttr);
	// 頂点バッファ/インデックスバッファ固定
	LPVOID pVtx;
	m_pD3DMesh->LockVertexBuffer(D3DLOCK_READONLY, &pVtx);
	LPVOID pIdx;
	m_pD3DMesh->LockIndexBuffer(D3DLOCK_READONLY, &pIdx);
	// 抽出場所の確保
	m_dwVtx = m_pD3DMesh->GetNumVertices();
	m_pVtx = new BVERTEX[m_dwVtx];
	m_dwFace = m_pD3DMesh->GetNumFaces();
	m_dwIdx = m_dwFace * 3;
	m_pIdx = new WORD[m_dwIdx];
	m_pPiece = new PARTICLE[m_dwFace];
	m_pPieceVtx = new BVERTEX[m_dwIdx];
	// コピー
	CopyMemory(m_pVtx, pVtx, sizeof(BVERTEX) * m_dwVtx);
	CopyMemory(m_pIdx, pIdx, sizeof(WORD) * m_dwIdx);
	// 頂点バッファ/インデックスバッファ解放
	m_pD3DMesh->UnlockVertexBuffer();
	m_pD3DMesh->UnlockIndexBuffer();

	// カレントディレクトリを変更
	if (szDir[0]) {
		GetCurrentDirectory(_MAX_PATH, szCurrentDir);
		SetCurrentDirectory(szDir);
	}

	// マテリアル読み込み
	D3DXMATERIAL* pD3DMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	m_pMaterial = new D3DMATERIAL9[m_dwNumMaterial];
	m_ppTexture = new LPDIRECT3DTEXTURE9[m_dwNumMaterial];
	for (DWORD i = 0; i < m_dwNumMaterial; i++) { 
		m_pMaterial[i] = pD3DMaterials[i].MatD3D;
		m_pMaterial[i].Ambient = m_pMaterial[i].Diffuse;
		m_ppTexture[i] = NULL;
		if (pD3DMaterials[i].pTextureFilename && pD3DMaterials[i].pTextureFilename[0]) {
			// テクスチャファイルを読み込む
			if (FAILED(D3DXCreateTextureFromFileA(CGraphics::GetDevice(),
				pD3DMaterials[i].pTextureFilename, &m_ppTexture[i]))) {
				_stprintf(szMsg, _T("テクスチャ(%hs)の読み込みに失敗しました。"),
					pD3DMaterials[i].pTextureFilename);
				MessageBox(NULL, szMsg, NULL, MB_OK);
			}
		}
	}

	// カレントディレクトリを元に戻す
	if (szDir[0])
		SetCurrentDirectory(szCurrentDir);

	pD3DXMtrlBuffer->Release();

	// 境界ボックス生成
	D3DXVECTOR3 vMin = m_pVtx[0].pos;
	D3DXVECTOR3 vMax = vMin;
	BVERTEX* pBVtx = m_pVtx + 1;
	for (DWORD i = 1; i < m_dwVtx; i++, pBVtx++) {
		if (vMin.x > pBVtx->pos.x)
			vMin.x = pBVtx->pos.x;
		if (vMin.y > pBVtx->pos.y)
			vMin.y = pBVtx->pos.y;
		if (vMin.z > pBVtx->pos.z)
			vMin.z = pBVtx->pos.z;
		if (vMax.x < pBVtx->pos.x)
			vMax.x = pBVtx->pos.x;
		if (vMax.y < pBVtx->pos.y)
			vMax.y = pBVtx->pos.y;
		if (vMax.z < pBVtx->pos.z)
			vMax.z = pBVtx->pos.z;
	}
	m_vHalf = (vMax - vMin) / 2.0f;
	m_vCenter = (vMax + vMin) / 2.0f;

	// 境界球の生成
	m_fRadius = 0.0f;
	float fR;
	for (DWORD i = 0; i < m_dwVtx; i++) {
		fR = D3DXVec3Length(&(m_pVtx[i].pos - m_vCenter));
		if (m_fRadius < fR)
			m_fRadius = fR;
	}

	// 境界球イメージの生成
	D3DXCreateSphere(CGraphics::GetDevice(),
		m_fRadius, 32, 16, &m_pSphere, NULL);

	// OBB生成
	float fWidth = vMax.x - vMin.x;
	float fHeight = vMax.y - vMin.y;
	float fDepth = vMax.z - vMin.z;

	m_vOBBHalf.x = fWidth / 2.0f;
	m_vOBBHalf.y = fHeight / 2.0f;
	m_vOBBHalf.z = fDepth / 2.0f;
	// OBBイメージの生成
	D3DXCreateBox(CGraphics::GetDevice() , 
		fWidth , fHeight , fDepth , &m_pBox , NULL);

	return true;
}

//**関数***************************************************************************
// メッシュ解放
//*********************************************************************************
void CMesh::Finalize()
{
	// 境界球イメージの解放
	SAFE_RELEASE(m_pSphere);

	SAFE_DELETE_ARRAY(m_pPiece);
	SAFE_DELETE_ARRAY(m_pPieceVtx);

	SAFE_DELETE_ARRAY(m_pVtx);
	SAFE_DELETE_ARRAY(m_pIdx);

	SAFE_DELETE_ARRAY(m_pAttr);

	// テクスチャ オブジェクトを解放
	if (m_ppTexture) {
		for (DWORD i = 0; i < m_dwNumMaterial; i++) {
			SAFE_RELEASE(m_ppTexture[i]);
		}
		SAFE_DELETE_ARRAY(m_ppTexture);
	}
	SAFE_DELETE_ARRAY(m_pMaterial);

	SAFE_RELEASE(m_pD3DMesh);	// メッシュ オブジェクトを解放
}

//**関数***************************************************************************
//	概要	:	メッシュ描画
//*********************************************************************************
void CMesh::Draw(D3DXMATRIX& world)
{
	// ワールド マトリックス設定
    LPDIRECT3DDEVICE9 pDevice = CGraphics::GetDevice();
    pDevice->SetTransform(D3DTS_WORLD, &world);

	for (DWORD i = 0; i < m_dwAttr; i++) {
		DWORD id = m_pAttr[i].AttribId;
        // アルファ値をチェック
        D3DMATERIAL9 mtrl = m_pMaterial[id];
		if (mtrl.Diffuse.a < 1.0f)
			continue;
		pDevice->SetMaterial(&mtrl);
		pDevice->SetTexture(0, m_ppTexture[id]);	// テクスチャを設定
//		m_pD3DMesh->DrawSubset(id);								// 描画を実行
		pDevice->SetFVF(FVF_BVERTEX);
		pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0,
			m_dwVtx, m_pAttr[i].FaceCount, &m_pIdx[m_pAttr[i].FaceStart * 3],
			D3DFMT_INDEX16, m_pVtx, sizeof(BVERTEX));
	}

	// アルファ ブレンド有効化
    pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	for (DWORD i = 0; i < m_dwAttr; i++) {
		DWORD id = m_pAttr[i].AttribId;
        // アルファ値をチェック
        D3DMATERIAL9 mtrl = m_pMaterial[id];
		if (mtrl.Diffuse.a >= 1.0f)
			continue;
		pDevice->SetMaterial(&mtrl);
		pDevice->SetTexture(0, m_ppTexture[id]);	// テクスチャを設定
//		m_pD3DMesh->DrawSubset(id);								// 描画を実行
		pDevice->SetFVF(FVF_BVERTEX);
		pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0,
			m_dwVtx, m_pAttr[i].FaceCount, &m_pIdx[m_pAttr[i].FaceStart * 3],
			D3DFMT_INDEX16, m_pVtx, sizeof(BVERTEX));
	}

    // アルファ ブレンド無効化
    pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

//**関数***************************************************************************
//	概要	:	半透明描画
//*********************************************************************************
void CMesh::Draw(D3DXMATRIX& world, float fAlpha)
{
    // ワールド マトリックス設定
    LPDIRECT3DDEVICE9 pDevice = CGraphics::GetDevice();
    pDevice->SetTransform(D3DTS_WORLD, &world);

    // アルファ ブレンド有効化
    pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    for (DWORD i = 0; i < m_dwAttr; i++) {
        DWORD id = m_pAttr[i].AttribId;

        // アルファ値を変更
        D3DMATERIAL9 mtrl = m_pMaterial[id];
        mtrl.Diffuse.a *= fAlpha;
        pDevice->SetMaterial(&mtrl);

        pDevice->SetTexture(0, m_ppTexture[id]);
        pDevice->SetFVF(FVF_BVERTEX);

		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
			m_pAttr[i].FaceCount,
			&m_pPieceVtx[m_pAttr[i].FaceStart * 3],
			sizeof(BVERTEX));
    }

    // アルファ ブレンド無効化
    pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}


//**関数***************************************************************************
//	概要	:	断片の初期化
//*********************************************************************************
void CMesh::InitParticle()
{
	// 頂点座標、テクスチャ座標をコピー
	for (DWORD i = 0; i < m_dwIdx; ++i) {
		m_pPieceVtx[i].pos = m_pVtx[m_pIdx[i]].pos;
		m_pPieceVtx[i].tex = m_pVtx[m_pIdx[i]].tex;
	}
	D3DXVECTOR3 v0, v1, n;
	for (DWORD i = 0; i < m_dwFace; ++i) {
		// 法線ベクトルを生成
		v0 = m_pPieceVtx[i * 3 + 1].pos
			- m_pPieceVtx[i * 3 + 0].pos;
		v1 = m_pPieceVtx[i * 3 + 2].pos
			- m_pPieceVtx[i * 3 + 1].pos;
		D3DXVec3Cross(&n, &v0, &v1);
		D3DXVec3Normalize(&n, &n);
		m_pPieceVtx[i * 3 + 0].nor = n;
		m_pPieceVtx[i * 3 + 1].nor = n;
		m_pPieceVtx[i * 3 + 2].nor = n;
		// 速度、加速度の初期化
		m_pPiece[i].spd = n * INIT_SPD;
		m_pPiece[i].acl =
			D3DXVECTOR3(0.0f, -GRAVITY, 0.0f);
	}
}


//**関数***************************************************************************
//	概要	:	断片の移動
//*********************************************************************************
void CMesh::UpdateParticle()
{
	for (DWORD i = 0; i < m_dwFace; ++i) {
		m_pPieceVtx[i * 3 + 0].pos += m_pPiece[i].spd;
		m_pPieceVtx[i * 3 + 1].pos += m_pPiece[i].spd;
		m_pPieceVtx[i * 3 + 2].pos += m_pPiece[i].spd;
		m_pPiece[i].spd += m_pPiece[i].acl;
	}
}


//**関数***************************************************************************
//	概要	:	境界球の描画
//*********************************************************************************
void CMesh::DrawSphere(D3DXMATRIX& world, D3DCOLORVALUE color)
{
    LPDIRECT3DDEVICE9 pDevice = CGraphics::GetDevice();

	// ワールド マトリックス設定
	D3DXMATRIX m;
	D3DXMatrixTranslation(&m, m_vCenter.x, m_vCenter.y, m_vCenter.z);
	m *= world;
    pDevice->SetTransform(D3DTS_WORLD, &m);

    // アルファ ブレンド有効化
	//pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    //pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    //pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// メッシュ描画
	D3DMATERIAL9 mtrl = {
		color,//Diffuse
		color,//Ambient
		{1.0f, 1.0f, 1.0f, 1.0f},//Specular
		{0.0f, 0.0f, 0.0f, 1.0f},//Emissive
		1.0f//Power
	};
	pDevice->SetMaterial(&mtrl);
	pDevice->SetTexture(0, NULL);
	pDevice->SetFVF(D3DFVF_XYZ |
		D3DFVF_NORMAL | D3DFVF_TEX1);
	m_pSphere->DrawSubset(0);

    // アルファ ブレンド無効化
    //pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}


//**関数***************************************************************************
//	概要	:	OBBの描画
//*********************************************************************************
void CMesh::DrawBox(D3DXMATRIX& world, D3DCOLORVALUE color)
{
    LPDIRECT3DDEVICE9 pDevice = CGraphics::GetDevice();

	// ワールド マトリックス設定
	D3DXMATRIX m;
	D3DXMatrixTranslation(&m, m_vCenter.x, m_vCenter.y, m_vCenter.z);
	m *= world;
    pDevice->SetTransform(D3DTS_WORLD, &m);

    // アルファ ブレンド有効化
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// メッシュ描画
	D3DMATERIAL9 mtrl = {
		color,//Diffuse
		color,//Ambient
		{1.0f, 1.0f, 1.0f, 1.0f},//Specular
		{0.0f, 0.0f, 0.0f, 1.0f},//Emissive
		1.0f//Power
	};

	pDevice->SetMaterial(&mtrl);
	pDevice->SetTexture(0, NULL);
	pDevice->SetFVF(D3DFVF_XYZ |
		D3DFVF_NORMAL | D3DFVF_TEX1);
	m_pBox->DrawSubset(0);

    // アルファ ブレンド無効化
    pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}


//**関数***************************************************************************
//	概要	:	メッシュ不透明部分のみ描画
//*********************************************************************************
void CMesh::DrawNoAlpha(D3DXMATRIX& world)
{
	// ワールド マトリックス設定
    LPDIRECT3DDEVICE9 pDevice = CGraphics::GetDevice();
    pDevice->SetTransform(D3DTS_WORLD, &world);

	for (DWORD i = 0; i < m_dwAttr; i++) {
		DWORD id = m_pAttr[i].AttribId;
        // アルファ値をチェック
        D3DMATERIAL9 mtrl = m_pMaterial[id];
		if (mtrl.Diffuse.a < 1.0f)
			continue;
		pDevice->SetMaterial(&mtrl);
		pDevice->SetTexture(0, m_ppTexture[id]);	// テクスチャを設定
		m_pD3DMesh->DrawSubset(id);								// 描画を実行
	}
}


//**関数***************************************************************************
//	概要	:	メッシュ半透明部分のみ描画 (アルファ有効化/無効化なし)
//*********************************************************************************
void CMesh::DrawAlpha(D3DXMATRIX& world)
{
	// ワールド マトリックス設定
    LPDIRECT3DDEVICE9 pDevice = CGraphics::GetDevice();
    pDevice->SetTransform(D3DTS_WORLD, &world);

	for (DWORD i = 0; i < m_dwAttr; i++) {
		DWORD id = m_pAttr[i].AttribId;
        // アルファ値をチェック
        D3DMATERIAL9 mtrl = m_pMaterial[id];
		if (mtrl.Diffuse.a >= 1.0f)
			continue;
		pDevice->SetMaterial(&mtrl);
		pDevice->SetTexture(0, m_ppTexture[id]);	// テクスチャを設定
		m_pD3DMesh->DrawSubset(id);					// 描画を実行
	}
}

//=======================================================================================
//	End of File
//=======================================================================================