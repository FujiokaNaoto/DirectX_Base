//**プログラムヘッダ***************************************************************
//	プログラム概要	:	シェーダマネージャ
//*********************************************************************************


//==include部======================================================================
#include "common.h"
#include "ShadeManager.h"
#include "../Resource/Shader/Shader.h"
#include "Register.h"

#include "../Resource/Shader/ToonShader.h"
#include "../Resource/Shader/PhongShader.h"
#include "../Resource/Shader/CookTrranceShader.h"
#include "../Resource/Shader/BlinnShader.h"
#include "../Resource/Shader/ParaOff.h"
//=================================================================================


//==定数・列挙型部=================================================================
namespace eShadeManager
{
	LPCTSTR	FX_NAME = _T("../resource_data/shader/Sharder.fx");
	LPCTSTR FX_BUMP = _T("../resource_data/shader/ParallaxMap.fx");

	// 各シェーダのテクニック番号
	UINT	TECH_PHONG = 0;
	UINT	TECH_TOON = 1;
	UINT	TECH_BLINN = 2;
	UINT	TECH_COOKTRRANCE = 3;
	UINT	TECH_PARAOFF = 2;
};
//=================================================================================


//==マクロ定義部===================================================================

//=================================================================================


//==クラス・構造体宣言部===========================================================

//=================================================================================


//==静的メンバ変数部===============================================================
CShadeManager*	CShadeManager::m_pInstance = NULL;
//=================================================================================


//==プロトタイプ宣言===============================================================

//=================================================================================


//**関数***************************************************************************
//	概要	:	コンストラクタ
//*********************************************************************************
CShadeManager::CShadeManager()
{
}


//**関数***************************************************************************
//	概要	:	デストラクタ
//*********************************************************************************
CShadeManager::~CShadeManager()
{
}


//**関数***************************************************************************
//	概要	:	生成
//*********************************************************************************
CShadeManager* CShadeManager::Create()
{
	if(m_pInstance)
		return m_pInstance;

	m_pInstance = new CShadeManager();

	if(m_pInstance->Init())
		return m_pInstance;

	SAFE_RELEASE(m_pInstance);
	return NULL;
}


//**関数***************************************************************************
//	概要	:	各種シェーダーファイルを生成
//*********************************************************************************
bool CShadeManager::Init()
{
	// 各シェーダファイル生成
	CShader*	pShade = NULL;

	// フォン
	pShade = CPhongShader::Create(eShadeManager::FX_NAME , eShadeType::PHONG , eShadeManager::TECH_PHONG);
	if(pShade) m_hList.push_back(pShade->GetHandle());

	// ブリン
	pShade = CBlinnShader::Create(eShadeManager::FX_NAME , eShadeType::BLINN , eShadeManager::TECH_BLINN);
	if(pShade) m_hList.push_back(pShade->GetHandle());

	// クックトランス
	pShade = CCookTrranceShader::Create(eShadeManager::FX_NAME , eShadeType::COOKTRRANCE , eShadeManager::TECH_COOKTRRANCE);
	if(pShade) m_hList.push_back(pShade->GetHandle());

	// トゥーン
	pShade = CToonShader::Create(eShadeManager::FX_NAME , eShadeType::TOON , eShadeManager::TECH_TOON);
	if(pShade) m_hList.push_back(pShade->GetHandle());

	// 視差遮蔽マッピング
	pShade = CParaOff::Create(eShadeManager::FX_BUMP , eShadeType::PARAOFF , eShadeManager::TECH_PARAOFF);
	if(pShade) m_hList.push_back(pShade->GetHandle());

	return true;
}


//**関数***************************************************************************
//	概要	:	解放
//*********************************************************************************
void CShadeManager::Release()
{
	CShader* pShade = NULL;

	list<S_HANDLE>::iterator	begin = m_hList.begin();
	list<S_HANDLE>::iterator	end = m_hList.end();

	while(begin != end)
	{
		pShade = REGISTER_H_P((*begin) , CShader*);
		if(pShade) pShade->Release();
		begin = m_hList.erase(begin);
	}
}


//**関数***************************************************************************
//	概要	:	指定したシェーダーオブジェのハンドルを返す
//*********************************************************************************
S_HANDLE CShadeManager::GetShadeHandle(int nType)
{
	CShader* pShade = NULL;

	list<S_HANDLE>::iterator	begin = m_hList.begin();
	list<S_HANDLE>::iterator	end = m_hList.end();

	while(begin != end)
	{
		pShade = REGISTER_H_P((*begin) , CShader*);
		if(pShade->GetType() == nType)
			return pShade->GetHandle();

		begin ++;
	}

	return 0;
}

//=================================================================================
//	End of File
//=================================================================================