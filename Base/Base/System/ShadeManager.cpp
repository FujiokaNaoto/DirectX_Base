//**�v���O�����w�b�_***************************************************************
//	�v���O�����T�v	:	�V�F�[�_�}�l�[�W��
//*********************************************************************************


//==include��======================================================================
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


//==�萔�E�񋓌^��=================================================================
namespace eShadeManager
{
	LPCTSTR	FX_NAME = _T("../resource_data/shader/Sharder.fx");
	LPCTSTR FX_BUMP = _T("../resource_data/shader/ParallaxMap.fx");

	// �e�V�F�[�_�̃e�N�j�b�N�ԍ�
	UINT	TECH_PHONG = 0;
	UINT	TECH_TOON = 1;
	UINT	TECH_BLINN = 2;
	UINT	TECH_COOKTRRANCE = 3;
	UINT	TECH_PARAOFF = 2;
};
//=================================================================================


//==�}�N����`��===================================================================

//=================================================================================


//==�N���X�E�\���̐錾��===========================================================

//=================================================================================


//==�ÓI�����o�ϐ���===============================================================
CShadeManager*	CShadeManager::m_pInstance = NULL;
//=================================================================================


//==�v���g�^�C�v�錾===============================================================

//=================================================================================


//**�֐�***************************************************************************
//	�T�v	:	�R���X�g���N�^
//*********************************************************************************
CShadeManager::CShadeManager()
{
}


//**�֐�***************************************************************************
//	�T�v	:	�f�X�g���N�^
//*********************************************************************************
CShadeManager::~CShadeManager()
{
}


//**�֐�***************************************************************************
//	�T�v	:	����
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


//**�֐�***************************************************************************
//	�T�v	:	�e��V�F�[�_�[�t�@�C���𐶐�
//*********************************************************************************
bool CShadeManager::Init()
{
	// �e�V�F�[�_�t�@�C������
	CShader*	pShade = NULL;

	// �t�H��
	pShade = CPhongShader::Create(eShadeManager::FX_NAME , eShadeType::PHONG , eShadeManager::TECH_PHONG);
	if(pShade) m_hList.push_back(pShade->GetHandle());

	// �u����
	pShade = CBlinnShader::Create(eShadeManager::FX_NAME , eShadeType::BLINN , eShadeManager::TECH_BLINN);
	if(pShade) m_hList.push_back(pShade->GetHandle());

	// �N�b�N�g�����X
	pShade = CCookTrranceShader::Create(eShadeManager::FX_NAME , eShadeType::COOKTRRANCE , eShadeManager::TECH_COOKTRRANCE);
	if(pShade) m_hList.push_back(pShade->GetHandle());

	// �g�D�[��
	pShade = CToonShader::Create(eShadeManager::FX_NAME , eShadeType::TOON , eShadeManager::TECH_TOON);
	if(pShade) m_hList.push_back(pShade->GetHandle());

	// �����Օ��}�b�s���O
	pShade = CParaOff::Create(eShadeManager::FX_BUMP , eShadeType::PARAOFF , eShadeManager::TECH_PARAOFF);
	if(pShade) m_hList.push_back(pShade->GetHandle());

	return true;
}


//**�֐�***************************************************************************
//	�T�v	:	���
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


//**�֐�***************************************************************************
//	�T�v	:	�w�肵���V�F�[�_�[�I�u�W�F�̃n���h����Ԃ�
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