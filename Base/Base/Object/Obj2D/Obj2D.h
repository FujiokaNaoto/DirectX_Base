//�v���O�����w�b�_*****************************************************************
//	�v���O�����T�v	:	2D�I�u�W�F�N�g�x�[�X�N���X
//*********************************************************************************
#pragma once

//==include��======================================================================
#include "../Base/ObjBase.h"
#include "../../CalcData/HitData.h"
//=================================================================================


//==�萔�E�񋓌^�錾��=============================================================
#define	FVF_VERTEX_2D			(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
#define	FVF_VERTEX_2D_TEXNONE	(D3DFVF_XYZRHW | D3DFVF_DIFFUSE)
//=================================================================================


//==�}�N����`��===================================================================
struct VERTEX_2D
{
	D3DXVECTOR4		vtx;								//���_���W
	D3DCOLOR		diffuse;							//���ˌ�
	D3DXVECTOR2		tex;								//�e�N�X�`�����W
};

struct VERTEX_2D_TEXNONE
{
	D3DXVECTOR4		vtx;								//���_���W
	D3DCOLOR		diffuse;							//���ˌ�
};
//=================================================================================


//==�N���X�E�\���̐錾��===========================================================
class CObj2D : public CObjBase
{
public:
	// �R���X�g���N�^�E�f�X�g���N�^
	CObj2D();
	virtual ~CObj2D();

	// --����--
	static CObj2D*	Create(LPCTSTR texName , D3DXVECTOR3 pos , 
		int nWidth , int nHeight , int nSceneID , int nPriority);	// ����

	static CObj2D*	Create(LPCTSTR texName , D3DXVECTOR3 pos , 
		D3DXVECTOR3 rot , D3DXVECTOR3 scall , int nWidth , 
		int nHeight , int nSceneID , int nPriority);				// ����

	
	virtual bool	Init(LPCTSTR texName , D3DXVECTOR3 pos , 
		int nWidth , int nHeight , int nSceneID , int nPriority);	// ������

	virtual bool	Init(LPCTSTR texName , D3DXVECTOR3 pos , 
		D3DXVECTOR3 rot , D3DXVECTOR3 scall , int nWidth , 
		int nHeight , int nSceneID , int nPriority);				// ������


	virtual void	Update();										// �X�V
	virtual void	Affine();										// �A�t�B���ϊ�
	// �`�施�߁ADrawLate���w�肷��ƁA3D�I�u�W�F�N�g����ɕ`�悳���
	virtual void	SetRS();
	virtual	void	SetRSLate();
	virtual void	Draw();
	virtual void	DrawLate();
	
	virtual void	Release();										// ���

	// --�Q�b�g�Z�b�g--
	virtual void	SetTex();										//�e�N�X�`���f�[�^�Z�b�g
	virtual void	SetVtx();										//���_�ݒ�
	virtual void	SetVtx(D3DXVECTOR4* vtx);						//���_�ݒ�

	virtual void	SetAnimParam(int nAnimFrame , int nTexPatternNum , 
		int nUPatternNum , int nVPatternNum);						// �A�j���[�V�������Z�b�g

	virtual void	SetDiffuse(int r , int g , int b , int a);		//�F�ݒ�
	
	CHitRect		GetHitRect();									// �����蔻���`�擾
	void			SetHitRect(CHitRect rect);						// �����蔻���`�Z�b�g

	void			SetResTexture(TCHAR* texName);
	void			SetResTexture(LPDIRECT3DTEXTURE9* tex);
	LPDIRECT3DTEXTURE9*	GetResTexture(){return m_pTexture;}

protected:
	// --�ϐ�--
	CHitRect		m_HitRect;										// �����蔻��f�[�^

	LPDIRECT3DTEXTURE9*	m_pTexture;									// �e�N�X�`���|�C���^
	VERTEX_2D			m_vVtx[4];									// ���W�f�[�^
	VERTEX_2D_TEXNONE	m_vVtxNTex[4];								// �e�N�X�`���Ȃ����W�f�[�^

	int		m_nAnimCnt;
	int		m_nAnimFrame;											// �A�j���[�V�����؂�ւ��t���[��
	int		m_nTexPatternNum;										// �e�N�X�`���p�^�[����
	
	int		m_nPatternNumU , m_nPatternNumV;						// �e�N�X�`����UV�����̃p�^�[����
	int		m_nCurPatternU , m_nCurPatternV;						// ����UV�e�N�X�`���p�^�[��

	// --�֐�--
	virtual void	AnimUpdate();									// �A�j���[�V�����X�V

private:
	VERTEX_2D			m_vVtxBase[4];								// ���W���f�[�^
	VERTEX_2D_TEXNONE	m_vVtxNTexBase[4];							// �e�N�X�`���Ȃ����W���f�[�^
};
//=================================================================================


//=================================================================================
//	End of File
//=================================================================================