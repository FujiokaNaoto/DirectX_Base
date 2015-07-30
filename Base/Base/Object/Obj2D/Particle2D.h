//�v���O�����w�b�_*****************************************************************
//	�v���O�����T�v	:	�G�t�F�N�g�p�[�e�B�N���N���X��`
//*********************************************************************************
#pragma once

//==include��======================================================================
#include "Obj2D.h"
//=================================================================================


//==�萔�E�񋓌^�錾��=============================================================
namespace eParticleConst2D
{
	enum
	{
		STATE_IN,
		STATE_UPDATE,
		STATE_OUT,
	};
};
//=================================================================================


//==�}�N����`��===================================================================

//=================================================================================


//==�N���X�E�\���̐錾��===========================================================
struct ParticleParam2D
{
	D3DXVECTOR3	ParInitPos;											// �����ʒu
	D3DXVECTOR3	ParInitSpd , ParInitRot , ParInitScall;				// �������x , ������]�p , �����g��
	D3DXVECTOR3	ParAccel , ParRotSpd , ParScallSpd;					// �����x , ��]���x , �g�k���x
	int			nParLife;											// ��������
	int			nInFrame , nOutFrame;								// �o�ꎞ�t�F�[�h�C�� , �������t�F�[�h�A�E�g
	unsigned int	nRed , nGreen , nBlue , nAlpha;					// �J���[�R�[�h

	ParticleParam2D(void)
	{
		ParInitPos = D3DXVECTOR3(0.0f , 0.0f , 0.0f);
		ParInitSpd = ParInitRot = D3DXVECTOR3(0.0f , 0.0f , 0.0f);
		ParInitScall = D3DXVECTOR3(1.0f , 1.0f , 1.0f);
		ParAccel = ParRotSpd = ParScallSpd = D3DXVECTOR3(0.0f , 0.0f , 0.0f);
		nParLife = 20;
		nInFrame = 3;
		nOutFrame = 3;
		nRed = nGreen = nBlue = 255;
		nAlpha = 0;
	}
};


class CParticle2D : public CObj2D
{
private:

protected:
	D3DXVECTOR3	m_Accel , m_RotSpd , m_ScallSpd;
	
	int	m_nLifeCnt;
	int	m_nLife , m_nInFrame , m_nOutFrame;

	unsigned int	m_nRed , m_nGreen , m_nBlue , m_nAlpha;

	void ParamSet(ParticleParam2D param);
public:
	CParticle2D();
	virtual ~CParticle2D();

	static CParticle2D* Create(LPCTSTR texName , ParticleParam2D param , int nWidth , int nHeight , int nSceneID , int nPri);
	
	virtual bool	Init(LPCTSTR texName , ParticleParam2D param , int nWidth , int nHeight , int nSceneID , int nPri);

	// �e�N�X�`���p�^�[���̐؂�ւ�

	virtual void	SetRS();
	virtual	void	Update();


	virtual	void	SetRGBA(int R , int G , int B , int A);
};
//=================================================================================


//=================================================================================
//	End of File
//=================================================================================