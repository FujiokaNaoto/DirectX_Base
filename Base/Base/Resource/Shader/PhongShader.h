//�v���O�����w�b�_*****************************************************************
//	�v���O�����T�v	:	�t�H���V�F�[�_�[�N���X�w�b�_
//*********************************************************************************
#pragma once

//==include��======================================================================
#include "Shader.h"
//=================================================================================


//==�萔�E�񋓌^�錾��=============================================================

//=================================================================================


//==�}�N����`��===================================================================

//=================================================================================


//==�N���X�E�\���̐錾��===========================================================
class CPhongShader : public CShader
{
protected:
	D3DXHANDLE	m_hMatWVP;		// ���[���h�~�r���[�~�ˉe
	D3DXHANDLE	m_hMatW;		// ���[���h �}�g���b�N�X
	D3DXHANDLE	m_hCamera;		// ���_���W
	D3DXHANDLE	m_hLight;		// �����x�N�g��
	D3DXHANDLE	m_hDiffuse;		// (�}�e���A��)�g�U�F
	D3DXHANDLE	m_hSpecular;	// (�}�e���A��)���ʔ��ːF
	D3DXHANDLE	m_hPower;		// (�}�e���A��)���ʔ��ˎw��
	D3DXHANDLE	m_hAmbient;		// (�}�e���A��)���F
	D3DXHANDLE	m_hTexture;		// �e�N�X�`��
	D3DXHANDLE	m_hTexEnable;	// �e�N�X�`���L��

	D3DXVECTOR3	m_vLight;
	D3DXVECTOR3	m_vCamera;
	D3DXMATRIX	m_mView;
	D3DXMATRIX	m_mProj;

public:
	CPhongShader();
	virtual	~CPhongShader();
	
	static CPhongShader*	Create(LPCTSTR pFxName , int nType , UINT unTech);
	virtual	bool		Init(LPCTSTR pFxName , int nType , UINT unTech);

	virtual	UINT		BeginShader();					// �V�F�[�_�J�n(�p�X�̐���Ԃ�)
	virtual void		SetCamera(LPD3DXVECTOR3 pCamera , LPD3DXMATRIX pView , LPD3DXMATRIX pProj);
	virtual void		SetLight(LPD3DXVECTOR3 pLight);
	virtual void		SetWorldMatrix(LPD3DXMATRIX pWorld);
	virtual void		SetMaterial(D3DMATERIAL9* pMaterial , LPDIRECT3DTEXTURE9 pTexture);
};
//=================================================================================


//=================================================================================
//	End of File
//=================================================================================