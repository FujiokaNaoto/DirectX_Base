//�v���O�����w�b�_*****************************************************************
//	�v���O�����T�v	:	�e�N�X�`���Ǘ��N���X
//*********************************************************************************
#pragma once

//==include��======================================================================
#include <d3d9.h>
#include <d3dx9.h>

#include <vector>
#include <tchar.h>

//=================================================================================


//==�萔�E�񋓌^�錾��=============================================================

//=================================================================================


//==�}�N����`��===================================================================
#define TEX_VECT			std::vector<LPDIRECT3DTEXTURE9*>			// �e�N�X�`���z��
#define TEX_VECT_ITE		std::vector<LPDIRECT3DTEXTURE9*>::iterator	// �e�N�X�`���z��ւ̃C�e���[�^

#define TEXNAME_VECT		std::vector<LPCTSTR>						// �e�N�X�`�����z��
#define TEXNAME_VECT_ITE	std::vector<LPCTSTR>::iterator				// �e�N�X�`�����z��ւ̃C�e���[�^
//=================================================================================


//==�N���X�E�\���̐錾��===========================================================
class CTextureManage
{
private:
	static TEX_VECT		m_TextureVect;									// ���[�h���ꂽ�e�N�X�`����ێ����郊�X�g
	static TEXNAME_VECT	m_TextureNameVect;								// �e�N�X�`���̖��O���i�[���Ă������X�g

	LPDIRECT3DDEVICE9	m_pDevice;

public:
	// �R���X�g���N�^�E�f�X�g���N�^
	CTextureManage();
	virtual ~CTextureManage();

	static CTextureManage*	Create();									// ����
	bool	Initialize();												// ������

	LPDIRECT3DTEXTURE9*	Search(LPCTSTR TexName);							// ���łɃe�N�X�`�����i�[����Ă��邩������
	LPDIRECT3DTEXTURE9*	Entry(LPCTSTR TexName);							// �e�N�X�`�������X�g���ɓo�^�@
	void	VectorClear();												// �i�[�z������S�N���A
	bool	ClearOnce(LPCTSTR TexName);									// �w�胊�\�[�X�̉��
	
	void	Release();													// �}�l�[�W�����
};
//=================================================================================


//=================================================================================
//	End of File
//=================================================================================