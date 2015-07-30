//�v���O�����w�b�_*****************************************************************
//	�v���O�����T�v	:	�G�~�b�^�[�N���X��`
//*********************************************************************************
#pragma once

//==include��======================================================================
#include "../Base/ObjBase.h"
#include "..\Obj2D\Particle2D.h"
//=================================================================================


//==�萔�E�񋓌^�錾��=============================================================
namespace eEmitterConst
{
	const int	NONE_LIFE = -1;										// �������Ԑ����Ȃ�
};
//=================================================================================


//==�}�N����`��===================================================================

//=================================================================================


//==�N���X�E�\���̐錾��===========================================================
class CEmitter2D : public CObjBase
{
private:

protected:
	LPCTSTR		m_TexName;											// �����e�N�X�`����
	CObjBase*	m_pParent;											// �e
	bool		m_bActive;											// �g�p��Ԕ���

	// ---���g�̏��---
	int			m_nMyLife;
	int			m_nGenerateLenge , m_nGenerateNum;					// �����Ԋu , ������
	int			m_nGenerateCnt;

	// ---��������p�[�e�B�N���ɗ^������---
	ParticleParam2D	m_SendParam;									// ���ۂɑ��M����f�[�^
	ParticleParam2D	m_KeyParam;										// ���M����f�[�^�̊�ɂȂ�f�[�^
	int				m_nLifeRandLenge;
	float			m_fWidth , m_fHeight;

	// �����f�[�^�̃����_���l
	D3DXVECTOR3		m_ParRandPosLenge , m_ParRandRotLenge , m_ParRandScallLenge;
	D3DXVECTOR3		m_ParRandSpdLenge , m_ParRandRotSpdLenge , m_ParRandScallSpdLenge;	
	D3DXVECTOR3		m_ParRandAccelLenge;	
	float			m_fKeyWidth , m_fKeyHeight;
	float			m_fWidthLenge , m_fHeightLenge;

public:
	CEmitter2D();
	virtual ~CEmitter2D(){};

	static	CEmitter2D*	Create(LPCTSTR texName , CObjBase* parent , int nSceneID , int nPri , int nGenerateLenge ,
		int nGenerateNum , int nParLife);

	static	CEmitter2D*	Create(LPCTSTR texName , CObjBase* parent , int nSceneID , int nPri , int nGenerateLenge ,
		int nGenerateNum , int nParLife , float fWidth , float fHeight);
	
	virtual bool	Init(LPCTSTR texName , CObjBase* parent , int nSceneID , int nPri , int nGenerateLenge ,
		int nGenerateNum , int nParLife);
	virtual bool	Init(LPCTSTR texName , CObjBase* parent , int nSceneID , int nPri , int nGenerateLenge ,
		int nGenerateNum , int nParLife , float fWidth , float fHeight);

	virtual void	Update();										// �X�V
	virtual	void	Affine();										// �A�t�B���ϊ�

	virtual void	Draw();
	virtual void	DrawLate();

	virtual void	EmitterStart(){m_bActive = true;}
	virtual void	EmitterStop(){m_bActive = false;}


	virtual void	ParticleGenerate();
	virtual void	SendParamSet();

	// --�Q�b�g�Z�b�g--
	virtual void	SetKeyParam(ParticleParam2D param){m_KeyParam = param;}
	virtual void	SetRandPosLenge(D3DXVECTOR3 vec){m_ParRandPosLenge = vec;}
	virtual void	SetRandRotLenge(D3DXVECTOR3 vec){m_ParRandRotLenge = vec;}
	virtual void	SetRandScallLenge(D3DXVECTOR3 vec){m_ParRandScallLenge = vec;}
	virtual void	SetRandSpdLenge(D3DXVECTOR3 vec){m_ParRandSpdLenge = vec;}
	virtual void	SetRandRotSpdLenge(D3DXVECTOR3 vec){m_ParRandRotSpdLenge = vec;}
	virtual void	SetRandScallSpdLenge(D3DXVECTOR3 vec){m_ParRandScallSpdLenge = vec;}
	virtual void	SetRandAccelLenge(D3DXVECTOR3 vec){m_ParRandAccelLenge = vec;}

	virtual void	SetKeyWidth(float fWidth){m_fKeyWidth = fWidth;}
	virtual void	SetRandWidth(float fWidth){m_fKeyWidth = fWidth;}
	virtual void	SetKeyHeight(float fHeight){m_fKeyHeight = fHeight;}
	virtual void	SetRandHeight(float fHeight){m_fKeyHeight = fHeight;}
};
//=================================================================================


//=================================================================================
//	End of File
//=================================================================================