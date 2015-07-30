//**プログラムヘッダ***************************************************************
//	プログラム概要	:	2Dオブジェクトベース
//*********************************************************************************


//==include部======================================================================
#include "../../System/GameManager.h"

#include "PolyLine.h"
#include "../../DirectX/Graphics.h"
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
CObjLine::CObjLine():
m_pTexture(NULL)
{
	m_nClassID = eClassCode::OBJ_2D;

	m_HitRect.m_fWidth = 0.0f;
	m_HitRect.m_fHeight = 0.0f;
	m_HitRect.m_fDepth = 0.0f;
	m_HitRect.m_nPointType = eHitRect::CENTER;
	m_fAnimeTime = 0.0f;
	m_nAnimeSpeed = 0;
	m_bAnimeFlg = false;
	m_nWaitTime = 0;
	m_nWaitCnt = 0;
}


//**関数***************************************************************************
//	概要	:	デストラクタ
//*********************************************************************************
CObjLine::~CObjLine()
{

}


//**関数***************************************************************************
//	概要	:	生成
//*********************************************************************************
CObjLine* CObjLine::Create(LPCTSTR texName , D3DXVECTOR3 Spos ,  D3DXVECTOR3 Epos,
		 int nWidth,int nPartition , int nCurvType, int nSceneID , int nPriority)
{

	D3DXVECTOR3 vec;
	D3DXVECTOR3 vec2;

	switch(nCurvType)
	{
	case POLYLINE_TYPE::HERMITE:
		vec.x = 0;
		vec.y = 0;
		return Create(texName , Spos ,Epos , vec , vec, nWidth, nPartition, nCurvType, nSceneID , nPriority);
		break;

	case POLYLINE_TYPE::BEZIER:
		vec = Spos;
		vec2 = Epos;
		return Create(texName , Spos ,Epos , vec , vec2, nWidth, nPartition, nCurvType, nSceneID , nPriority);
		break;
	}
	return Create(texName , Spos ,Epos , vec , vec2, nWidth, nPartition, nCurvType, nSceneID , nPriority);
}


//**関数***************************************************************************
//	概要	:	生成
//*********************************************************************************
CObjLine* CObjLine::Create(LPCTSTR texName ,  D3DXVECTOR3 Spos ,D3DXVECTOR3 Epos , 
	D3DXVECTOR3 Svec, D3DXVECTOR3 Evec , int nWidth ,int nPartition, int nCurvType, int nSceneID , int nPriority)
{
	CObjLine*		pObj = new CObjLine;

	// --初期値格納--
	pObj->m_pTexture = MANAGER.GetTexManage()->Entry(texName);
	if(! pObj->m_pTexture)	return NULL;			// 登録失敗時はNULLを返す
	
	pObj->m_Spos = Spos;
	pObj->m_Epos = Epos;
	pObj->m_Svec = Svec;
	pObj->m_Evec = Evec;

	pObj->m_HitRect.m_fWidth = (float)nWidth;		// 幅
	pObj->m_nPartition = nPartition;				// 分割数
	pObj->m_nCurvType = nCurvType;					// 曲線タイプ
	pObj->m_nSceneID = nSceneID;					// シーンID
	pObj->m_nPriority = nPriority;					// プライオリティ
	
	return pObj;
}

//**関数***************************************************************************
//	概要	:	初期化
//*********************************************************************************
void CObjLine::Init(LPCTSTR texName , D3DXVECTOR3 Spos ,  D3DXVECTOR3 Epos ,
		 int nWidth ,int nPartition ,int nCurvType, int nSceneID , int nPriority)
{
	D3DXVECTOR3 vec;
	D3DXVECTOR3 vec2;

	switch(nCurvType)
	{
	case POLYLINE_TYPE::HERMITE:
		vec.x = 0;
		vec.y = 0;
		return Init(texName , Spos ,Epos , vec , vec, nWidth, nPartition, nCurvType, nSceneID , nPriority);
		break;

	case POLYLINE_TYPE::BEZIER:
		vec = Spos;
		vec2 = Epos;
		return Init(texName , Spos ,Epos , vec , vec2, nWidth, nPartition, nCurvType, nSceneID , nPriority);
		break;
	}
	return Init(texName , Spos ,Epos , vec , vec2, nWidth, nPartition, nCurvType, nSceneID , nPriority);
}

//**関数***************************************************************************
//	概要	:	初期化
//*********************************************************************************
void CObjLine::Init(LPCTSTR texName , D3DXVECTOR3 Spos , D3DXVECTOR3 Epos,
			D3DXVECTOR3 Svec ,D3DXVECTOR3 Evec , int nWidth ,int nPartition,int nCurvType, int nSceneID , int nPriority)
{
	// --初期値格納--
	m_pTexture = MANAGER.GetTexManage()->Entry(texName);
	
	m_Spos = Spos;
	m_Epos = Epos;
	m_Svec = Svec;
	m_Evec = Evec;

	m_HitRect.m_fWidth = (float)nWidth;		// 幅
	m_nPartition = nPartition;				// 分割数
	m_nCurvType = nCurvType;					// 曲線タイプ
	m_nSceneID = nSceneID;					// シーンID
	m_nPriority = nPriority;					// プライオリティ
}



//**関数***************************************************************************
//	概要	:	解放
//*********************************************************************************
void CObjLine::Release()
{
	Uninit();
	delete this;
}


//**関数***************************************************************************
//	概要	:	アフィン変換
//*********************************************************************************
void CObjLine::Affine()
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

	/*
	for(int n = 0 ; n < 4 ; n ++)
		D3DXVec3TransformCoord((D3DXVECTOR3*)&m_vVtx[n].vtx , (D3DXVECTOR3*) &m_vVtxBase[n].vtx , &m_Matrix);
		*/
}

//**関数***************************************************************************
//	概要	:	更新
//*********************************************************************************
void CObjLine::Update()
{
	if(m_bAnimeFlg && m_nWaitCnt ? (m_nWaitCnt-- ? false:false) :true)
	{
		m_fAnimeTime += (1.0f / (float)m_nPartition) * (float)m_nAnimeSpeed;

		while(m_fAnimeTime > 1)
		{
			m_fAnimeTime = 0.0f;
			m_nWaitCnt = m_nWaitTime;
		}

	}
}

//**関数***************************************************************************
//	概要	:	描画
//*********************************************************************************
void CObjLine::Draw()
{
	// 使用フラグがオフまたは後描画フラグがオンなら描画しない
	if(!m_bUseFlg || m_bDrawLate) return;			
	
	// --2D用設定に変更--
	// ライトオフ
	CGraphics::GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
	// アルファ ブレンド有効化
	CGraphics::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    CGraphics::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    CGraphics::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	// Zバッファオフ
	CGraphics::GetDevice()->SetRenderState(D3DRS_ZENABLE , FALSE);

	// テクスチャセット
	if(m_pTexture) CGraphics::GetDevice()->SetTexture(0 , *m_pTexture);

	CGraphics::GetDevice()->SetFVF(FVF_VERTEX_2D);
	CGraphics::GetDevice()->SetTransform(D3DTS_WORLD , &m_Matrix);

	//頂点に曲線上の点を代入
	SetLineVtx();
	/*
	//曲線上の点を参考にポリゴン描画用頂点をセット
	SetVtx();
	//テクスチャセット
	SetTex();
	//ディフューズセット
	//SetDiffuse(255,255,255,255);
	*/
	SetAll();

	for(int i=0; i < m_nPartition -1; i++)
	{

		VERTEX_2D VTX[4];

		VTX[0] = m_vVtx[i+1][2];
		VTX[1] = m_vVtx[i+1][1];
		VTX[2] = m_vVtx[i][2];
		VTX[3] = m_vVtx[i][1];

		if(!(VTX[0].tex.y > VTX[2].tex.y))
			CGraphics::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP , 2 , VTX , sizeof(VERTEX_2D));
	}	
	// --2D用設定を戻す--
	// Zバッファオン
	CGraphics::GetDevice()->SetRenderState(D3DRS_ZENABLE , TRUE);
	 // アルファ ブレンド無効化
    CGraphics::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	// ライトオン
	CGraphics::GetDevice()->SetRenderState(D3DRS_LIGHTING, TRUE);
}


//**関数***************************************************************************
//	概要	:	描画
//*********************************************************************************
void CObjLine::DrawLate()
{
	// 使用フラグがオフまたは後描画フラグがオフなら描画しない
	if(!m_bUseFlg || ! m_bDrawLate) return;
	
	// --2D用設定に変更--
	// ライトオフ
	CGraphics::GetDevice()->SetRenderState(D3DRS_LIGHTING, FALSE);
	// アルファ ブレンド有効化
	CGraphics::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    CGraphics::GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    CGraphics::GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// Zバッファオフ
	CGraphics::GetDevice()->SetRenderState(D3DRS_ZENABLE , FALSE);

	// テクスチャセット
	if(m_pTexture) CGraphics::GetDevice()->SetTexture(0 , *m_pTexture);

	CGraphics::GetDevice()->SetFVF(FVF_VERTEX_2D);
	CGraphics::GetDevice()->SetTransform(D3DTS_WORLD , &m_Matrix);
	CGraphics::GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP , 2 , m_vVtx , sizeof(VERTEX_2D));
	
	// --2D用設定を戻す--
	// Zバッファオン
	CGraphics::GetDevice()->SetRenderState(D3DRS_ZENABLE , TRUE);
	 // アルファ ブレンド無効化
    CGraphics::GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
	CGraphics::GetDevice()->SetRenderState(D3DRS_ZENABLE , TRUE);

	// ライトオン
	CGraphics::GetDevice()->SetRenderState(D3DRS_LIGHTING, TRUE);
}


//**関数***************************************************************************
//	概要	:	始点、終点のセット
//*********************************************************************************
void CObjLine::SetLinePos(D3DXVECTOR3 Spos, D3DXVECTOR3 Epos)	
{
	m_Spos = Spos;
	m_Epos = Epos;
}


//**関数***************************************************************************
//	概要	:	ベクトルor頂点のセット
//*********************************************************************************
void CObjLine::SetVector(D3DXVECTOR3 Svec, D3DXVECTOR3 Evec)	
{
	m_Svec = Svec;
	m_Evec = Evec;
}


//**関数***************************************************************************
//	概要	:	アニメーション情報セット
//*********************************************************************************
void CObjLine::AnimeSet(int Speed, int WaitTime)
{
	m_bAnimeFlg = true;
	m_nAnimeSpeed = Speed;
	m_nWaitTime = WaitTime;
}

//**関数***************************************************************************
//	概要	:	アニメーション停止
//*********************************************************************************
void CObjLine::AnimeStop()
{
	m_bAnimeFlg = false;
}

//**関数***************************************************************************
//	概要	:	テクスチャセット
//*********************************************************************************
void CObjLine::SetTex()
{

	for(int i = 0; i < m_nPartition; i++)
	{

		m_vVtx[i][2].tex = D3DXVECTOR2(0.0f ,(m_fAnimeTime + 1.0f - (1.0f/(float)m_nPartition)*i)<=1 ? 
			(m_fAnimeTime + 1.0f - (1.0f/(float)m_nPartition)*i) : 
			(m_fAnimeTime - (1.0f/(float)m_nPartition)*i));

		m_vVtx[i][1].tex = D3DXVECTOR2(1.0f ,(m_fAnimeTime + 1.0f - (1.0f/(float)m_nPartition)*i)<=1 ? 
			(m_fAnimeTime + 1.0f - (1.0f/(float)m_nPartition)*i) : 
		(m_fAnimeTime - (1.0f/(float)m_nPartition)*i));

	}
}


//**関数***************************************************************************
//	概要	:	頂点情報セット
//*********************************************************************************
void CObjLine::SetVtx()
{
	//ポリゴン描画用頂点情報セット
	for(int i = 0; i < m_nPartition; i++)
	{
		D3DXVECTOR3 vec;
		D3DXVECTOR3 Lvec;
		D3DXVECTOR3 Rvec;

		vec.x = m_vVtx[i+1][0].vtx.x - m_vVtx[i][0].vtx.x;
		vec.y = m_vVtx[i+1][0].vtx.y - m_vVtx[i][0].vtx.y;
		vec.z = 0;

		D3DXVec3Normalize(&vec,&vec);
		vec.x = vec.x* (float)m_HitRect.m_fWidth*0.5f;
		vec.y = vec.y* (float)m_HitRect.m_fWidth*0.5f;

		Lvec.x = -vec.y;
		Lvec.y = vec.x;
		Rvec.x = vec.y;
		Rvec.y = -vec.x;

		m_vVtx[i][1].vtx.x = m_vVtx[i][0].vtx.x + Lvec.x;
		m_vVtx[i][1].vtx.y = m_vVtx[i][0].vtx.y + Lvec.y;
		m_vVtx[i][2].vtx.x = m_vVtx[i][0].vtx.x + Rvec.x;
		m_vVtx[i][2].vtx.y = m_vVtx[i][0].vtx.y + Rvec.y;

		if(i == m_nPartition - 2)
		{
			i++;
			m_vVtx[i][1].vtx.x = m_vVtx[i][0].vtx.x + Lvec.x;
			m_vVtx[i][1].vtx.y = m_vVtx[i][0].vtx.y + Lvec.y;
			m_vVtx[i][2].vtx.x = m_vVtx[i][0].vtx.x + Rvec.x;
			m_vVtx[i][2].vtx.y = m_vVtx[i][0].vtx.y + Rvec.y;
		}
	}
}


//**関数***************************************************************************
//	概要	:	曲線上の頂点情報セット
//*********************************************************************************
void CObjLine::SetLineVtx()
{
	float t = 0;
	switch(m_nCurvType)
		{
		case POLYLINE_TYPE::HERMITE:
				for(int i = 0; t <= 1 ; t += (float)((float)1/(float)m_nPartition),i++)
				{
					D3DXVECTOR3 Out;
					D3DXVec3Hermite(&Out,
									&m_Spos,
									&m_Svec,
									&m_Epos,
									&m_Evec,
									t);

					m_vVtx[i][0].vtx.x = Out.x;
					m_vVtx[i][0].vtx.y = Out.y;
				}
				break;


		case  POLYLINE_TYPE::BEZIER:
				for(int i = 0; t <= 1 ; t += (float)((float)1/(float)m_nPartition),i++)
				{
					D3DXVECTOR3 Out;
					Out.x = (1-t)*(1-t)*(1-t)*m_Spos.x + 3*(1-t)*(1-t)*t*m_Svec.x + 3*(1-t)*t*t*m_Evec.x + t*t*t*m_Epos.x;
					Out.y = (1-t)*(1-t)*(1-t)*m_Spos.y + 3*(1-t)*(1-t)*t*m_Svec.y + 3*(1-t)*t*t*m_Evec.y + t*t*t*m_Epos.y;

					m_vVtx[i][0].vtx.x = Out.x;
					m_vVtx[i][0].vtx.y = Out.y;
				}
				break;

		}
}


//**関数***************************************************************************
//	概要	:	色設定
//*********************************************************************************
void CObjLine::SetDiffuse(int r , int g , int b , int a)
{
	//宣言部
	int 	nLoopCnt;
	for(nLoopCnt = 0 ; nLoopCnt < m_nPartition ; nLoopCnt ++)
	{
		m_vVtx[nLoopCnt][1].diffuse = D3DCOLOR_RGBA(r , g , b , a);		//色指定
		m_vVtx[nLoopCnt][2].diffuse = D3DCOLOR_RGBA(r , g , b , a);		//色指定
	}
}

//**関数***************************************************************************
//	概要	:	頂点情報全設定
//********************************************************************************* 
void CObjLine::SetAll()
{
	float t = 0.0f;
	for(int i = 0; i < m_nPartition; i++,t += 1.0f/(float)m_nPartition)
	{
		//ポリゴン描画用頂点情報セット
		D3DXVECTOR3 vec;
		D3DXVECTOR3 Lvec;
		D3DXVECTOR3 Rvec;

		float fWidth = m_HitRect.m_fWidth;
		D3DCOLOR fDiffuse = D3DCOLOR_RGBA(255,255,255,255);

		AddEffect(t,&fWidth,&fDiffuse);

		vec.x = m_vVtx[i+1][0].vtx.x - m_vVtx[i][0].vtx.x;
		vec.y = m_vVtx[i+1][0].vtx.y - m_vVtx[i][0].vtx.y;
		vec.z = 0;

		D3DXVec3Normalize(&vec,&vec);
		vec.x = vec.x* (float)fWidth*0.5f;
		vec.y = vec.y* (float)fWidth*0.5f;

		Lvec.x = -vec.y;
		Lvec.y = vec.x;
		Rvec.x = vec.y;
		Rvec.y = -vec.x;

		m_vVtx[i][1].vtx.x = m_vVtx[i][0].vtx.x + Lvec.x;
		m_vVtx[i][1].vtx.y = m_vVtx[i][0].vtx.y + Lvec.y;
		m_vVtx[i][2].vtx.x = m_vVtx[i][0].vtx.x + Rvec.x;
		m_vVtx[i][2].vtx.y = m_vVtx[i][0].vtx.y + Rvec.y;

		//テクスチャ設定
		m_vVtx[i][2].tex = D3DXVECTOR2(0.0f ,(m_fAnimeTime + 1.0f - (1.0f/(float)m_nPartition)*i)<=1 ? 
		(m_fAnimeTime + 1.0f - (1.0f/(float)m_nPartition)*i) : 
		(m_fAnimeTime - (1.0f/(float)m_nPartition)*i));

		m_vVtx[i][1].tex = D3DXVECTOR2(1.0f ,(m_fAnimeTime + 1.0f - (1.0f/(float)m_nPartition)*i)<=1 ? 
		(m_fAnimeTime + 1.0f - (1.0f/(float)m_nPartition)*i) : 
		(m_fAnimeTime - (1.0f/(float)m_nPartition)*i));

		m_vVtx[i][1].diffuse = fDiffuse;		//色指定
		m_vVtx[i][2].diffuse = fDiffuse;		//色指定


		if(i == m_nPartition - 2)
		{
			i++;
			t = 1.0f;
			fWidth = m_HitRect.m_fWidth;
			fDiffuse = D3DCOLOR_RGBA(255,255,255,255);

			AddEffect(t,&fWidth,&fDiffuse);
			m_vVtx[i][1].vtx.x = m_vVtx[i][0].vtx.x + Lvec.x;
			m_vVtx[i][1].vtx.y = m_vVtx[i][0].vtx.y + Lvec.y;
			m_vVtx[i][2].vtx.x = m_vVtx[i][0].vtx.x + Rvec.x;
			m_vVtx[i][2].vtx.y = m_vVtx[i][0].vtx.y + Rvec.y;
			//テクスチャ設定
			m_vVtx[i][2].tex = D3DXVECTOR2(0.0f ,(m_fAnimeTime + 1.0f - (1.0f/(float)m_nPartition)*i)<=1 ? 
			(m_fAnimeTime + 1.0f - (1.0f/(float)m_nPartition)*i) : 
			(m_fAnimeTime - (1.0f/(float)m_nPartition)*i));

			m_vVtx[i][1].tex = D3DXVECTOR2(1.0f ,(m_fAnimeTime + 1.0f - (1.0f/(float)m_nPartition)*i)<=1 ? 
			(m_fAnimeTime + 1.0f - (1.0f/(float)m_nPartition)*i) : 
			(m_fAnimeTime - (1.0f/(float)m_nPartition)*i));

			m_vVtx[i][1].diffuse = fDiffuse;		//色指定
			m_vVtx[i][2].diffuse = fDiffuse;		//色指定
		}


	}

}

//**関数***************************************************************************
//	概要	:	追加効果記述用関数(通常は空白)
//*********************************************************************************
void CObjLine::AddEffect(float t, float* Width, D3DCOLOR* Diffuse)
{
	//テスト用記述
	if(t < 0.1f)
		*Diffuse = D3DCOLOR_RGBA(255,255,255,(int)((float)255*t*(float)10));

	if(t > 0.9f)
		*Diffuse = D3DCOLOR_RGBA(255,255,255,(int)((float)255*(1.0f-t)*(float)10));

	//*Width = ((abs(t-0.5f)*-1.0f)+1.0f)* (*Width*2);
	*Width = t/2**Width*2 + *Width/2;
}


//**関数***************************************************************************
//	概要	:	あたり判定矩形取得
//*********************************************************************************
CHitRect CObjLine::GetHitRect()
{
	m_HitRect.m_Point = m_Pos;
	return m_HitRect;
}
//=================================================================================
//	End of File
//=================================================================================