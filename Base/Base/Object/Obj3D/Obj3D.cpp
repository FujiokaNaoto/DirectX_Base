//**プログラムヘッダ***************************************************************
//	プログラム概要	:	3Dオブジェクトベース
//*********************************************************************************


//==include部======================================================================
#include "../../System/GameManager.h"
#include "Obj3D.h"
#include "../../System/ShadeManager.h"

#include "../../System/Register.h"
#include "../../Resource/Shader/PhongShader.h"
#include "../../Resource/Shader/BlinnShader.h"
#include "../../Resource/Shader/CookTrranceShader.h"
#include "../../Resource/Shader/ToonShader.h"
#include "../../Resource/Shader/ParaOff.h"
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
CObj3D::CObj3D():
m_pMesh(NULL)
{
	m_nClassID = eClassCode::OBJ_3D;
	m_nRSType = eRSType::OBJ_3D;
	m_nRSType = eRSType::OBJ_3D_LATE;
}


//**関数***************************************************************************
//	概要	:	デストラクタ
//*********************************************************************************
CObj3D::~CObj3D()
{
}


//**関数***************************************************************************
//	概要	:	生成
//*********************************************************************************
CObj3D* CObj3D::Create(LPCTSTR meshName , D3DXVECTOR3 pos , 
	 int nSceneID , int nPriority)
{
	CObj3D*	pObj = new CObj3D;

	if(pObj->Init(meshName , pos , nSceneID , nPriority))
		return pObj;

	delete pObj;
	return NULL;
}


//**関数***************************************************************************
//	概要	:	生成
//*********************************************************************************
CObj3D* CObj3D::Create(LPCTSTR meshName , D3DXVECTOR3 pos , 
		D3DXVECTOR3 rot , D3DXVECTOR3 scall , int nSceneID , int nPriority)
{
	CObj3D*	pObj = new CObj3D;

	if(pObj->Init(meshName , pos , rot , scall , nSceneID , nPriority))
		return pObj;

	delete pObj;
	return pObj;
}


//**関数***************************************************************************
//	概要	:	初期化
//*********************************************************************************
bool CObj3D::Init(LPCTSTR meshName , D3DXVECTOR3 pos , 
		 int nSceneID , int nPriority)
{
	return Init(meshName , pos , DEFAULT_ROT , DEFAULT_SCALL , nSceneID , nPriority);
}


//**関数***************************************************************************
//	概要	:	初期化
//*********************************************************************************
bool CObj3D::Init(LPCTSTR meshName , D3DXVECTOR3 pos , 
		D3DXVECTOR3 rot , D3DXVECTOR3 scall , int nSceneID
		 , int nPriority)
{
	// --初期値格納--
	m_pMesh = MANAGER.GetMeshManage()->Entry(meshName);	// メッシュロードと登録
	if(! m_pMesh)	return false;			// 登録失敗時はNULLを返す

	m_Pos = pos;
	m_Rot = rot;
	m_Scall = scall;
	m_nSceneID = nSceneID;
	m_nPriority = nPriority;				// プライオリティ

	Affine();

	return true;
}


//**関数***************************************************************************
//	概要	:	解放
//*********************************************************************************
void CObj3D::Release()
{
	Uninit();
	delete this;
}


//**関数***************************************************************************
//	概要	:	レンダーステート設定
//*********************************************************************************
void CObj3D::SetRS()
{
	CGraphics::SemafoLock();

	LPDIRECT3DDEVICE9 pDevice = MANAGER.GetGraph()->GetDevice();
	// 描画設定
	pDevice->SetRenderState(D3DRS_ZENABLE , TRUE);						// Zバッファオン
	pDevice->SetRenderState(D3DRS_LIGHTING , TRUE);						// ライトオン

	pDevice->SetRenderState(D3DRS_CULLMODE , D3DCULL_CCW);

	CGraphics::SemafoUnlock();
}


//**関数***************************************************************************
//	概要	:	後描画用レンダーステート設定
//*********************************************************************************
void CObj3D::SetRSLate()
{
	CGraphics::SemafoLock();

	// グラフィックデバイス取得
	LPDIRECT3DDEVICE9 pDevice = MANAGER.GetGraph()->GetDevice();

	// 描画設定
	pDevice->SetRenderState(D3DRS_ZENABLE , TRUE);						// Zバッファオン
	pDevice->SetRenderState(D3DRS_LIGHTING , TRUE);						// ライトオン
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// アルファブレンドあり

	pDevice->SetRenderState(D3DRS_CULLMODE , D3DCULL_CCW);

	CGraphics::SemafoUnlock();
}


//**関数***************************************************************************
//	概要	:	先描画
//			:	3Dにおいてはフラグの判定を行わないで透明と不透明両方描画する
//			:	不透明描画
//*********************************************************************************
void CObj3D::Draw()
{
	// 描画フラグがオフかメッシュが格納されていないなら描画しない
	if(!m_bUseFlg || !m_pMesh) return;			

	CGraphics::SemafoLock();

	LPDIRECT3DDEVICE9 pDevice = MANAGER.GetGraph()->GetDevice();
	CCamera* pCamera = MANAGER.GetCamera();

	// 指定シェーダタイプで描画
	if(m_nShadeType == eShadeType::NONE)
	{
		//pDevice->SetTransform(D3DTS_WORLD, &m_Matrix);
		m_pMesh->DrawNoAlpha(m_Matrix , 0);
	}
	else if(m_nShadeType & eShadeType::PHONG && MANAGER.GetShadeManage()->GetShadeHandle(eShadeType::PHONG))
	{
		CPhongShader* pShade = REGISTER_H_P(MANAGER.GetShadeManage()->GetShadeHandle(eShadeType::PHONG) , CPhongShader*);
		
		pShade->SetCamera(&pCamera->GetPos() , &pCamera->GetMatrixView() , &MANAGER.GetGraph()->GetMatProj());
		pShade->SetLight( (D3DXVECTOR3*)&MANAGER.GetGraph()->GetLight()->Direction);

		UINT uPass = pShade->BeginShader();
		for(UINT uNo = 0; uNo  < uPass ; uNo ++)
		{
			pShade->BeginPass(uNo);
			//pDevice->SetTransform(D3DTS_WORLD, &m_Matrix);
			pShade->SetWorldMatrix(&m_Matrix);
			m_pMesh->DrawNoAlpha(m_Matrix , pShade->GetHandle());
			pShade->EndPass();
		}
		pShade->EndShader();
	}
	else if(m_nShadeType & eShadeType::BLINN && MANAGER.GetShadeManage()->GetShadeHandle(eShadeType::BLINN))
	{
		CBlinnShader* pShade = REGISTER_H_P(MANAGER.GetShadeManage()->GetShadeHandle(eShadeType::BLINN) , CBlinnShader*);

		pShade->SetCamera(&pCamera->GetPos() , &pCamera->GetMatrixView() , &MANAGER.GetGraph()->GetMatProj());
		pShade->SetLight( (D3DXVECTOR3*)&MANAGER.GetGraph()->GetLight()->Direction);

		UINT uPass = pShade->BeginShader();
		for(UINT uNo = 0; uNo  < uPass ; uNo ++)
		{
			pShade->BeginPass(uNo);
			pDevice->SetTransform(D3DTS_WORLD, &m_Matrix);
			m_pMesh->DrawNoAlpha(m_Matrix , pShade->GetHandle());
			pShade->EndPass();
		}
		pShade->EndShader();
	}

	else if(m_nShadeType & eShadeType::COOKTRRANCE && MANAGER.GetShadeManage()->GetShadeHandle(eShadeType::COOKTRRANCE))
	{
		CCookTrranceShader* pShade = REGISTER_H_P(MANAGER.GetShadeManage()->GetShadeHandle(eShadeType::COOKTRRANCE) , CCookTrranceShader*);

		pShade->SetCamera(&pCamera->GetPos() , &pCamera->GetMatrixView() , &MANAGER.GetGraph()->GetMatProj());
		pShade->SetLight( (D3DXVECTOR3*)&MANAGER.GetGraph()->GetLight()->Direction);

		UINT uPass = pShade->BeginShader();
		for(UINT uNo = 0; uNo  < uPass ; uNo ++)
		{
			pShade->BeginPass(uNo);
			pDevice->SetTransform(D3DTS_WORLD, &m_Matrix);
			m_pMesh->DrawNoAlpha(m_Matrix , pShade->GetHandle());
			pShade->EndPass();
		}
		pShade->EndShader();
	}
	else if(m_nShadeType & eShadeType::TOON && MANAGER.GetShadeManage()->GetShadeHandle(eShadeType::TOON))
	{
		CToonShader* pShade = REGISTER_H_P(MANAGER.GetShadeManage()->GetShadeHandle(eShadeType::TOON) , CToonShader*);

		pShade->SetCamera(&pCamera->GetPos() , &pCamera->GetMatrixView() , &MANAGER.GetGraph()->GetMatProj());
		pShade->SetLight( (D3DXVECTOR3*)&MANAGER.GetGraph()->GetLight()->Direction);
		pShade->SetMeshRadius(m_pMesh->GetRadius());

		UINT uPass = pShade->BeginShader();
		for(UINT uNo = 0; uNo  < uPass ; uNo ++)
		{
			pShade->BeginPass(uNo);
			pDevice->SetTransform(D3DTS_WORLD, &m_Matrix);
			m_pMesh->DrawNoAlpha(m_Matrix , pShade->GetHandle());
			pShade->EndPass();
		}
		pShade->EndShader();
	}
	else if(m_nShadeType & eShadeType::PARAOFF && MANAGER.GetShadeManage()->GetShadeHandle(eShadeType::PARAOFF))
	{
		CParaOff* pShade = REGISTER_H_P(MANAGER.GetShadeManage()->GetShadeHandle(eShadeType::PARAOFF) , CParaOff*);

		
		pShade->SetCamera(&pCamera->GetPos() , &pCamera->GetMatrixView() , &MANAGER.GetGraph()->GetMatProj());
		pShade->SetLight( (D3DXVECTOR3*)&MANAGER.GetGraph()->GetLight()->Direction);
		
		UINT uPass = pShade->BeginShader();
		for(UINT uNo = 0; uNo  < uPass ; uNo ++)
		{
			pShade->BeginPass(uNo);
			pDevice->SetTransform(D3DTS_WORLD, &m_Matrix);
			m_pMesh->DrawNoAlpha(m_Matrix , pShade->GetHandle());
			pShade->EndPass();
		}
		pShade->EndShader();
	}
	// シェーダを使う予定だったがシェーダが読み込めていなかった時はハードウェア描画を行う
	else
	{
		//pDevice->SetTransform(D3DTS_WORLD, &m_Matrix);
		m_pMesh->DrawNoAlpha(m_Matrix , 0);
	}

	

	CGraphics::SemafoUnlock();
}


//**関数***************************************************************************
//	概要	:	後描画
//			:	3Dにおいてはフラグの判定を行わないで透明と不透明両方描画する
//			:	半透明描画
//*********************************************************************************
void CObj3D::DrawLate()
{
	// 描画フラグがオフかメッシュが格納されていないなら描画しない
	if(!m_bUseFlg || !m_pMesh) return;		

	CGraphics::SemafoLock();
	CCamera* pCamera = MANAGER.GetCamera();

	// グラフィックデバイス取得
	LPDIRECT3DDEVICE9 pDevice = MANAGER.GetGraph()->GetDevice();

	if(m_nShadeType == eShadeType::NONE)
	{
		//pDevice->SetTransform(D3DTS_WORLD, &m_Matrix);
		m_pMesh->DrawNoAlpha(m_Matrix , 0);
	}
	else if(m_nShadeType & eShadeType::PHONG && MANAGER.GetShadeManage()->GetShadeHandle(eShadeType::PHONG))
	{
		CPhongShader* pShade = REGISTER_H_P(MANAGER.GetShadeManage()->GetShadeHandle(eShadeType::PHONG) , CPhongShader*);
		
		pShade->SetCamera(&pCamera->GetPos() , &pCamera->GetMatrixView() , &MANAGER.GetGraph()->GetMatProj());
		pShade->SetLight( (D3DXVECTOR3*)&MANAGER.GetGraph()->GetLight()->Direction);

		UINT uPass = pShade->BeginShader();
		for(UINT uNo = 0; uNo  < uPass ; uNo ++)
		{
			pShade->BeginPass(uNo);
			pDevice->SetTransform(D3DTS_WORLD, &m_Matrix);
			m_pMesh->DrawNoAlpha(m_Matrix , pShade->GetHandle());
			pShade->EndPass();
		}
		pShade->EndShader();
	}
	else if(m_nShadeType & eShadeType::BLINN && MANAGER.GetShadeManage()->GetShadeHandle(eShadeType::BLINN))
	{
		CBlinnShader* pShade = REGISTER_H_P(MANAGER.GetShadeManage()->GetShadeHandle(eShadeType::BLINN) , CBlinnShader*);

		pShade->SetCamera(&pCamera->GetPos() , &pCamera->GetMatrixView() , &MANAGER.GetGraph()->GetMatProj());
		pShade->SetLight( (D3DXVECTOR3*)&MANAGER.GetGraph()->GetLight()->Direction);

		UINT uPass = pShade->BeginShader();
		for(UINT uNo = 0; uNo  < uPass ; uNo ++)
		{
			pShade->BeginPass(uNo);
			pDevice->SetTransform(D3DTS_WORLD, &m_Matrix);
			m_pMesh->DrawNoAlpha(m_Matrix , pShade->GetHandle());
			pShade->EndPass();
		}
		pShade->EndShader();
	}

	else if(m_nShadeType & eShadeType::COOKTRRANCE && MANAGER.GetShadeManage()->GetShadeHandle(eShadeType::COOKTRRANCE))
	{
		CCookTrranceShader* pShade = REGISTER_H_P(MANAGER.GetShadeManage()->GetShadeHandle(eShadeType::COOKTRRANCE) , CCookTrranceShader*);

		pShade->SetCamera(&pCamera->GetPos() , &pCamera->GetMatrixView() , &MANAGER.GetGraph()->GetMatProj());
		pShade->SetLight( (D3DXVECTOR3*)&MANAGER.GetGraph()->GetLight()->Direction);

		UINT uPass = pShade->BeginShader();
		for(UINT uNo = 0; uNo  < uPass ; uNo ++)
		{
			pShade->BeginPass(uNo);
			pDevice->SetTransform(D3DTS_WORLD, &m_Matrix);
			m_pMesh->DrawNoAlpha(m_Matrix , pShade->GetHandle());
			pShade->EndPass();
		}
		pShade->EndShader();
	}
	else if(m_nShadeType & eShadeType::TOON && MANAGER.GetShadeManage()->GetShadeHandle(eShadeType::TOON))
	{
		CToonShader* pShade = REGISTER_H_P(MANAGER.GetShadeManage()->GetShadeHandle(eShadeType::TOON) , CToonShader*);

		pShade->SetCamera(&pCamera->GetPos() , &pCamera->GetMatrixView() , &MANAGER.GetGraph()->GetMatProj());
		pShade->SetLight( (D3DXVECTOR3*)&MANAGER.GetGraph()->GetLight()->Direction);
		pShade->SetMeshRadius(m_pMesh->GetRadius());

		UINT uPass = pShade->BeginShader();
		for(UINT uNo = 0; uNo  < uPass ; uNo ++)
		{
			pShade->BeginPass(uNo);
			pDevice->SetTransform(D3DTS_WORLD, &m_Matrix);
			m_pMesh->DrawNoAlpha(m_Matrix , pShade->GetHandle());
			pShade->EndPass();
		}
		pShade->EndShader();
	}
	else if(m_nShadeType & eShadeType::PARAOFF && MANAGER.GetShadeManage()->GetShadeHandle(eShadeType::PARAOFF))
	{
		CParaOff* pShade = REGISTER_H_P(MANAGER.GetShadeManage()->GetShadeHandle(eShadeType::PARAOFF) , CParaOff*);
		
		pShade->SetCamera(&pCamera->GetPos() , &pCamera->GetMatrixView() , &MANAGER.GetGraph()->GetMatProj());
		pShade->SetLight( (D3DXVECTOR3*)&MANAGER.GetGraph()->GetLight()->Direction);

		UINT uPass = pShade->BeginShader();
		for(UINT uNo = 0; uNo  < uPass ; uNo ++)
		{
			pShade->BeginPass(uNo);
			pDevice->SetTransform(D3DTS_WORLD, &m_Matrix);
			m_pMesh->DrawNoAlpha(m_Matrix , pShade->GetHandle());
			pShade->EndPass();
		}
		pShade->EndShader();
	}

	// シェーダを使う予定だったがシェーダが読み込めていなかった時はハードウェア描画を行う
	else
	{
		//pDevice->SetTransform(D3DTS_WORLD, &m_Matrix);
		m_pMesh->DrawNoAlpha(m_Matrix , 0);
	}

	CGraphics::SemafoUnlock();
}
//=================================================================================
//	End of File
//=================================================================================