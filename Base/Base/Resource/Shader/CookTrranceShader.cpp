//**プログラムヘッダ***************************************************************
//	プログラム概要	:	クックトランスシェーダークラス
//*********************************************************************************


//==include部======================================================================
#include "../../System/common.h"
#include "CookTrranceShader.h"
#include "../../System/GameManager.h"
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
CCookTrranceShader::CCookTrranceShader():
m_hMatWVP(0),
m_hMatW(0),
m_hCamera(0),
m_hLight(0),
m_hDiffuse(0),
m_hSpecular(0),
m_hPower(0),
m_hAmbient(0),
m_hTexture(0),
m_hTexEnable(0),

m_vLight(D3DXVECTOR3(0,0,0)),
m_vCamera(D3DXVECTOR3(0,0,0))
{
	memset(&m_mView , 0 , sizeof(m_mView));
	memset(&m_mProj , 0 , sizeof(m_mProj));
}


//**関数***************************************************************************
//	概要	:	デストラクタ
//*********************************************************************************
CCookTrranceShader::~CCookTrranceShader()
{
}


//**関数***************************************************************************
//	概要	:	生成
//*********************************************************************************
CCookTrranceShader* CCookTrranceShader::Create(LPCTSTR pFxName , int nType , UINT unTech)
{
	CCookTrranceShader* pShade = new CCookTrranceShader();

	if(pShade->Init(pFxName , nType , unTech))
		return pShade;

	SAFE_RELEASE(pShade);
	return NULL;
}


//**関数***************************************************************************
//	概要	:	初期化
//*********************************************************************************
bool CCookTrranceShader::Init(LPCTSTR pFxName , int nType , UINT unTech)
{
	HRESULT	hr;
	hr = D3DXCreateEffectFromFile(MANAGER.GetGraph()->GetDevice(), pFxName , 
		NULL , NULL , 0 , NULL , &m_pFx , NULL);

	if(FAILED(hr))
	{
#ifdef _DEBUG
		TCHAR szMsg[512];
		_stprintf_s(szMsg , _T("シェーダ(%s)読み込みエラー"), pFxName);
		MANAGER.MsgBox(szMsg);
#endif
		return false;
	}

	struct _param {
		LPCSTR		pszName;
		D3DXHANDLE*	pHandle;
	} param[] = {
		{"WORLDVIEWPROJECTION",	&m_hMatWVP},
		{"WORLD",				&m_hMatW},
		{"CAMERAPOSITION",		&m_hCamera},
		{"MATERIALDIFFUSE",		&m_hDiffuse},
		{"MATERIALSPECULAR",	&m_hSpecular},
		{"MATERIALPOWER",		&m_hPower},
		{"MATERIALAMBIENT",		&m_hAmbient},
		{"MATERIALTEXTURE",		&m_hTexture},
		{"MATERIALTEXTUREVALID", &m_hTexEnable},
	};
	for (int i = 0; i < _countof(param); i++) {
		*param[i].pHandle =
			m_pFx->GetParameterBySemantic(NULL,
									param[i].pszName);
	}

	m_hLight = m_pFx->GetParameterByName(NULL, "light");
	m_hTech = m_pFx->GetTechnique(unTech);
	m_nType = nType;
	return true;
}


//**関数***************************************************************************
//	概要	:	シェーダ開始
//*********************************************************************************
UINT CCookTrranceShader::BeginShader()
{
	// テクニック設定
	m_pFx->SetTechnique(m_hTech);
	m_pFx->Begin(&m_unPassNum , 0);

	return m_unPassNum;
}


//**関数***************************************************************************
//	概要	:	カメラセット
//*********************************************************************************
void CCookTrranceShader::SetCamera(LPD3DXVECTOR3 pCamera , LPD3DXMATRIX pView , LPD3DXMATRIX pProj)
{
	m_vCamera = *pCamera;
	m_mView = *pView;
	m_mProj = *pProj;
}


//**関数***************************************************************************
//	概要	:	光源セット
//*********************************************************************************
void CCookTrranceShader::SetLight(LPD3DXVECTOR3 pLight)
{
	m_vLight = *pLight;
}


//**関数***************************************************************************
//	概要	:	ワールド座標セット
//*********************************************************************************
void CCookTrranceShader::SetWorldMatrix(LPD3DXMATRIX pWorld)
{
	m_pFx->SetMatrix(m_hMatWVP, &(*pWorld * m_mView * m_mProj));
	m_pFx->SetMatrix(m_hMatW, pWorld);
	m_pFx->SetFloatArray(m_hLight, &m_vLight.x, 3);
	m_pFx->SetFloatArray(m_hCamera, &m_vCamera.x, 3);
}


//**関数***************************************************************************
//	概要	:	マテリアルセット
//*********************************************************************************
void CCookTrranceShader::SetMaterial(D3DMATERIAL9* pMaterial , LPDIRECT3DTEXTURE9 pTexture)
{
	m_pFx->SetVector(m_hDiffuse, (LPD3DXVECTOR4)&pMaterial->Diffuse);

	m_pFx->SetVector(m_hSpecular, (LPD3DXVECTOR4)&pMaterial->Specular);
	m_pFx->SetFloat(m_hPower, pMaterial->Power);
	m_pFx->SetVector(m_hAmbient , (LPD3DXVECTOR4)&pMaterial->Ambient);

	if (pTexture) 
	{
		m_pFx->SetTexture(m_hTexture, pTexture);
		m_pFx->SetBool(m_hTexEnable, TRUE);
	} else 
	{
		m_pFx->SetBool(m_hTexEnable, FALSE);
	}
	m_pFx->CommitChanges();
}

//=================================================================================
//	End of File
//=================================================================================