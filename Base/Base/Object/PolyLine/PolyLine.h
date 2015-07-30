//�v���O�����w�b�_*****************************************************************
//	�v���O�����T�v	:	2D�I�u�W�F�N�g�x�[�X�N���X
//*********************************************************************************
#pragma once

//==include��======================================================================
#include "../Obj2D/Obj2D.h"
//=================================================================================

#define MAX_PARTITION (100)


//==�萔�E�񋓌^�錾��=============================================================
#define	FVF_VERTEX_2D			(D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)
//=================================================================================

namespace POLYLINE_TYPE
{
	enum POLYLINE
	{
		HERMITE,
		BEZIER,
	};
};


//==�N���X�E�\���̐錾��===========================================================
class CObjLine : public CObjBase
{
public:
	// �R���X�g���N�^�E�f�X�g���N�^
	CObjLine();
	virtual ~CObjLine();

	// --����--
	static CObjLine* Create(LPCTSTR texName , D3DXVECTOR3 Spos ,  D3DXVECTOR3 Epos ,
		 int nWidth ,int nPartition ,int nCurvType, int nSceneID , int nPriority);	// ����

	static CObjLine*	Create(LPCTSTR texName , D3DXVECTOR3 Spos , D3DXVECTOR3 Epos,

		D3DXVECTOR3 Svec ,D3DXVECTOR3 Evec , int nWidth ,int nPartition,int nCurvType, int nSceneID , int nPriority);				// ����
	
	virtual void	Init(LPCTSTR texName , D3DXVECTOR3 Spos ,  D3DXVECTOR3 Epos ,
		 int nWidth ,int nPartition ,int nCurvType, int nSceneID , int nPriority);
	virtual void	Init(LPCTSTR texName , D3DXVECTOR3 Spos , D3DXVECTOR3 Epos,
		D3DXVECTOR3 Svec ,D3DXVECTOR3 Evec , int nWidth ,int nPartition,int nCurvType, int nSceneID , int nPriority);


	virtual void	Affine();										// �A�t�B���ϊ�
	// �`�施�߁ADrawLate���w�肷��ƁA3D�I�u�W�F�N�g����ɕ`�悳���
	virtual void	Update();										// �A�b�v�f�[�g
	virtual void	Draw();		
	virtual void	DrawLate();
	
	virtual void	Release();										// ���

	// --�Q�b�g�Z�b�g--
	virtual void	SetLinePos(D3DXVECTOR3 Spos, D3DXVECTOR3 Epos);	//�n�_�A�I�_�̃Z�b�g
	virtual void	SetVector(D3DXVECTOR3 Svec, D3DXVECTOR3 Evec);	//�x�N�g��or���_�̃Z�b�g
	virtual void	AnimeSet(int Speed, int WaitTime);				// �A�j���[�V�������Z�b�g
	virtual void	AnimeStop();									// �A�j���[�V������~
	virtual void	SetTex();										//�e�N�X�`���f�[�^�Z�b�g
	virtual void	SetVtx();										//���_�ݒ�
	virtual void	SetLineVtx();									//�Ȑ����_�ݒ�
	virtual void	SetAll();										//�ݒ�̑S�ݒ�(�C�ӂ̌Ăяo���s��)
	virtual void	AddEffect(float t, float* Width, D3DCOLOR* Diffuse);//�ǉ����ʐݒ�֐�

	virtual D3DXVECTOR3	GetSpos(){return m_Spos;}
	virtual D3DXVECTOR3	GetEpos(){return m_Epos;}
	virtual D3DXVECTOR3	GetSvec(){return m_Svec;}
	virtual D3DXVECTOR3	GetEvec(){return m_Evec;}

	virtual void	SetDiffuse(int r , int g , int b , int a);		//�F�ݒ�
	
	CHitRect		GetHitRect();									// �����蔻���`�擾

protected:
	// --�ϐ�--
	D3DXVECTOR3		m_Spos;
	D3DXVECTOR3		m_Epos;
	D3DXVECTOR3		m_Svec;
	D3DXVECTOR3		m_Evec;

	int				m_nPartition;									//������
	int				m_nCurvType;									//�Ȑ��^�C�v
	bool			m_bAnimeFlg;									//�A�j���[�V�����t���O
	float			m_fAnimeTime;									//�A�j���[�V��������
	int				m_nAnimeSpeed;									//�A�j���[�V�����X�s�[�h�i1F������̕������̐i�s�x�j
	int				m_nWaitTime;									//�A�j���[�V�����̊Ԋu
	int				m_nWaitCnt;										//�A�j���[�V�����̊Ԋu�ҋ@�J�E���^

	CHitRect		m_HitRect;										// �����蔻��f�[�^
	LPDIRECT3DTEXTURE9*	m_pTexture;									// �e�N�X�`���|�C���^
	VERTEX_2D			m_vVtx[MAX_PARTITION][3];									// ���W�f�[�^

	// --�֐�--

private:
	VERTEX_2D			m_vVtxBase[4];								// ���W���f�[�^
};
//=================================================================================


//=================================================================================
//	End of File
//=================================================================================