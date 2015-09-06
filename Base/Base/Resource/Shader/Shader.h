//�v���O�����w�b�_*****************************************************************
//	�v���O�����T�v	:	�V�F�[�_�[�N���X�w�b�_
//*********************************************************************************
#pragma once

//==include��======================================================================
#include <tchar.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "../../Object/Base/CHANDLE.h"
//=================================================================================


//==�萔�E�񋓌^�錾��=============================================================

//=================================================================================


//==�}�N����`��===================================================================

//=================================================================================


//==�N���X�E�\���̐錾��===========================================================
class CShader
{
protected:
	int				m_nType;							// �V�F�[�_�^�C�v�i�[�ϐ�
	S_HANDLE		m_unHandle;							// ���g�̃n���h��

	UINT			m_unPassNum;						// �p�X�̔ԍ�
	D3DXHANDLE		m_hTech;							// �e�N�j�b�N�̃n���h��
	LPD3DXEFFECT	m_pFx;								// �G�t�F�N�g�I�u�W�F�N�g
	

public:
	CShader();
	virtual	~CShader();
	
	virtual	void		Release();						// ���

	virtual	UINT		BeginShader();					// �V�F�[�_�J�n(�p�X�̐���Ԃ�)
	virtual	void		BeginPass(UINT nPassNo = 0);	// �p�X�̔ԍ����w�肵�A�p�X�J�n

	virtual void		UpdateShader();					// �V�F�[�_�X�V
	virtual void		EndPass();
	virtual void		EndShader();					// �V�F�[�_�I��
	
	virtual S_HANDLE	GetHandle(){return m_unHandle;}	// �n���h���擾
	virtual	int			GetType(){return m_nType;}		// �^�C�v�擾

	virtual void		SetWorldMatrix(LPD3DXMATRIX pWorld){}
	virtual void		SetMaterial(D3DMATERIAL9* pMaterial , LPDIRECT3DTEXTURE9 pTexture){}
};	
//=================================================================================


//=================================================================================
//	End of File
//=================================================================================