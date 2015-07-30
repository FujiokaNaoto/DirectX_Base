//**�v���O�����w�b�_***************************************************************
//	�v���O�����T�v	:	�e�N�X�`���Ǘ��N���X����
//*********************************************************************************


//==include��======================================================================
#include "../System/GameManager.h"

#include "../System/common.h"
#include "TextureManage.h"
//=================================================================================


//==�萔�E�񋓌^��=================================================================

//=================================================================================


//==�}�N����`��===================================================================

//=================================================================================


//==�N���X�E�\���̐錾��===========================================================

//=================================================================================


//==�ÓI�����o�ϐ���===============================================================
TEX_VECT		CTextureManage::m_TextureVect(0);
TEXNAME_VECT	CTextureManage::m_TextureNameVect(0);
//=================================================================================


//==�v���g�^�C�v�錾===============================================================

//=================================================================================


//**�֐�***************************************************************************
//	�T�v	:	�R���X�g���N�^
//*********************************************************************************
CTextureManage::CTextureManage()
{

}


//**�֐�***************************************************************************
//	�T�v	:	�f�X�g���N�^
//*********************************************************************************
CTextureManage::~CTextureManage()
{

}


//**�֐�***************************************************************************
//	�T�v	:	����
//*********************************************************************************
CTextureManage* CTextureManage::Create()
{
	CTextureManage*	pManage = new CTextureManage;

	if(! pManage->Initialize())
	{
		::MessageBox(NULL , _T("TextureManage�����G���[") , _T("error") , MB_ICONERROR | MB_OK);
		return NULL;
	}

	return pManage;
}


//**�֐�***************************************************************************
//	�T�v	:	������
//*********************************************************************************
bool CTextureManage::Initialize()
{
	m_pDevice = CGraphics::GetDevice();

	return true;
}


//**�֐�***************************************************************************
//	�T�v	:	�z�������
//			:	�w�肵�����O�̃e�N�X�`���ւ̃|�C���^��Ԃ�
//*********************************************************************************
LPDIRECT3DTEXTURE9* CTextureManage::Search(LPCTSTR TexName)
{
	for(UINT i = 0 ; i < m_TextureNameVect.size() ; i ++)
	{
		if(_tcscmp(m_TextureNameVect[i] , TexName) != 0)
			continue;
			
		// ��v�����炻�̃e�N�X�`���ւ̃|�C���^��Ԃ�
		return (LPDIRECT3DTEXTURE9*)(m_TextureVect[i]);
	}

	// ������Ȃ������̂�NULL��Ԃ�
	return NULL;
}

//**�֐�***************************************************************************
//	�T�v	:	�z��֓o�^
//			:	�w�肵���e�N�X�`���ւ̃|�C���^��Ԃ�
//			:	���[�h���s�����Ƃ���NULL��Ԃ�
//*********************************************************************************
LPDIRECT3DTEXTURE9* CTextureManage::Entry(LPCTSTR TexName)
{
	LPDIRECT3DTEXTURE9*	pTex = NULL;
	
	// ���łɎw�肵���t�@�C�������邩������
	pTex = Search(TexName);

	// ����΂����Ԃ�
	if(pTex)
		return pTex;


	// �Ȃ������ꍇ�͐V�K�Ń��[�h
	pTex = new LPDIRECT3DTEXTURE9;

	// �Z�}�t�H���b�N
	CGraphics::SemafoLock();
	if(FAILED(D3DXCreateTextureFromFile(m_pDevice , TexName , pTex)))
	{
		// ���s�����Ƃ���NULL��Ԃ�
		TCHAR dbg[256];
		_stprintf_s(dbg , _T("�e�N�X�`���ǂݍ��݃G���[%s\n") , TexName);
		::MessageBox(NULL , dbg , _T("error") , MB_ICONERROR | MB_OK);
		return NULL;
	}
	CGraphics::SemafoUnlock();

	// �e�N�X�`���ƃe�N�X�`�������i�[
	m_TextureVect.push_back(pTex);
	m_TextureNameVect.push_back(TexName);

	return pTex;
}


//**�֐�***************************************************************************
//	�T�v	:	�z������S�N���A
//*********************************************************************************
void CTextureManage::VectorClear()
{
	// ���X�g�S���
	for(UINT i = 0 ; i < m_TextureVect.size() ; i ++)
	{	
		(*m_TextureVect[i])->Release();
		m_TextureNameVect[i] = NULL;
	}

	m_TextureVect.clear();
	m_TextureNameVect.clear();
}


//**�֐�***************************************************************************
//	�T�v	:	�}�l�[�W�����
//*********************************************************************************
void CTextureManage::Release()
{
	VectorClear();

	delete this;
}


//**�֐�***************************************************************************
//	�T�v	:	�ʉ��(�t�@�C�����w��)
//			:	������s����false��Ԃ�
//*********************************************************************************
bool CTextureManage::ClearOnce(LPCTSTR TexName)
{
	TEXNAME_VECT_ITE iteName = m_TextureNameVect.begin();
	TEX_VECT_ITE iteTex = m_TextureVect.begin();

	for(UINT i = 0 ; i < m_TextureNameVect.size() ; i ++ , iteName ++ , iteTex ++)
	{
		if(_tcscmp(m_TextureNameVect[i] , TexName) != 0)
			continue;
		
		// ��v������폜
		(*m_TextureVect[i])->Release();
		m_TextureNameVect[i] = NULL;
		m_TextureNameVect.erase(iteName);
		m_TextureVect.erase(iteTex);

		return true;
	}

	return false;
}


//=================================================================================
//	End of File
//=================================================================================