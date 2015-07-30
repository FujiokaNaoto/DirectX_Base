//�v���O�����w�b�_*****************************************************************
//	�v���O�����T�v	:	���[���h��|���S���x�[�X�N���X
//*********************************************************************************
#pragma once

//==include��======================================================================
#include "../Base/ObjBase.h"
//=================================================================================


//==�萔�E�񋓌^�錾��=============================================================
#define	FVF_VERTEX_BOARD			(D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define	FVF_VERTEX_BOARD_TEXNONE	(D3DFVF_XYZ | D3DFVF_DIFFUSE)
//=================================================================================


//==�}�N����`��===================================================================
struct VERTEX_BOARD	
{
	D3DXVECTOR3		vtx;								//���_���W
	D3DCOLOR		diffuse;							//���ˌ�
	D3DXVECTOR2		tex;								//�e�N�X�`�����W
};

struct VERTEX_BOARD_TEXNONE
{
	D3DXVECTOR3		vtx;								//���_���W
	D3DCOLOR		diffuse;							//���ˌ�
};
//=================================================================================


//==�N���X�E�\���̐錾��===========================================================
class CObjBoard : public CObjBase
{
public:
	// �R���X�g���N�^�E�f�X�g���N�^
	CObjBoard();
	virtual ~CObjBoard();

	static CObjBoard*	Create(LPCTSTR texName , D3DXVECTOR3 pos , 
		D3DXVECTOR3 rot , D3DXVECTOR3 scall , int nWidth , 
		int nHeight , int nSceneID , int nPriority);				// ����

	static CObjBoard*	Create(LPCTSTR texName , D3DXVECTOR3 pos , 
		int nWidth , int nHeight , int nSceneID , int nPriority);	// ����
	
	virtual bool	Init(LPCTSTR texName , D3DXVECTOR3 pos , 
		int nWidth , int nHeight , int nSceneID , int nPriority);
	
	virtual bool	Init(LPCTSTR texName , D3DXVECTOR3 pos , 
		D3DXVECTOR3 rot , D3DXVECTOR3 scall , int nWidth , 
		int nHeight , int nSceneID , int nPriority);

	virtual void	Affine();										// �A�t�B���ϊ�
	virtual void	SetRS();
	virtual void	SetRSLate();
	virtual void	Draw();											// ��`��
	virtual void	DrawLate();										// ��`��
	
	virtual void	Release();										// ���

	// --�Q�b�g�Z�b�g--
	virtual void	SetTex();										//�e�N�X�`���f�[�^�Z�b�g
	virtual void	SetVtx();										//���_�ݒ�
	virtual void	SetVtx(D3DXVECTOR3* vtx);						//���_�ݒ�
	virtual void	SetDiffuse(int r , int g , int b , int a);		//�F�ݒ�

	virtual void	SetAnimParam(int nAnimFrame , int nTexPatternNum , 
		int nUPatternNum , int nVPatternNum);

	// �r���{�[�h�t���O
	bool			GetBillFlg(){return m_bBillFlg;}				// �r���{�[�h�t���O�擾
	void			SetBillFlg(bool	bFlg){m_bBillFlg = bFlg;}		// �r���{�[�h�t���O�Z�b�g

protected:
	// --�ϐ�--
	int				m_nWidth , m_nHeight;							// �����A�c��
	bool			m_bBillFlg;										// �r���{�[�h�t���O

	LPDIRECT3DTEXTURE9*		m_pTexture;									// �e�N�X�`���|�C���^
	VERTEX_BOARD			m_vVtx[4];									// ���W�f�[�^
	VERTEX_BOARD_TEXNONE	m_vVtx_NTex[4];								// �e�N�X�`���Ȃ����W�f�[�^

	int		m_nAnimCnt;
	int		m_nAnimFrame;															// �A�j���[�V�����؂�ւ��t���[��
	int		m_nTexPatternNum;														// �e�N�X�`���p�^�[����
	
	int		m_nPatternNumU , m_nPatternNumV;										// �e�N�X�`����UV�����̃p�^�[����
	int		m_nCurPatternU , m_nCurPatternV;										// ����UV�e�N�X�`���p�^�[��

	virtual void	AnimUpdate();													// �A�j���[�V�����X�V

private:
	
};
//=================================================================================


//=================================================================================
//	End of File
//=================================================================================