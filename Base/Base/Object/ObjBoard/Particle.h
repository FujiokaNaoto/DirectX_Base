//�v���O�����w�b�_*****************************************************************
//	�v���O�����T�v	:	�G�t�F�N�g�p�[�e�B�N���N���X��`
//*********************************************************************************
#pragma once

//==include��======================================================================
#include "ObjBoard.h"
//=================================================================================


//==�萔�E�񋓌^�錾��=============================================================
namespace eParticleConst
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
struct ParticleParam
{
	D3DXVECTOR3	ParInitPos;											// �����ʒu
	D3DXVECTOR3	ParInitSpd , ParInitRot , ParInitScall;				// �������x , ������]�p , �����g��
	D3DXVECTOR3	ParAccel , ParRotSpd , ParScallSpd;					// �����x , ��]���x , �g�k���x
	int			nParLife;											// ��������
	int			nInFrame , nOutFrame;								// �o�ꎞ�t�F�[�h�C�� , �������t�F�[�h�A�E�g
	unsigned int	nRed , nGreen , nBlue , nAlpha;					// �J���[�R�[�h

	ParticleParam(void)
	{
		ParInitPos = D3DXVECTOR3(0.0f , 0.0f , 0.0f);
		ParInitSpd = ParInitRot = D3DXVECTOR3(0.0f , 0.0f , 0.0f);
		ParInitScall = D3DXVECTOR3(1.0f , 1.0f , 1.0f);
		ParAccel = ParRotSpd = ParScallSpd = D3DXVECTOR3(0.0f , 0.0f , 0.0f);
		nParLife = 3;
		nInFrame = 10;
		nOutFrame = 3;
		nRed = nGreen = nBlue = 255;
		nAlpha = 0;
	}
};


class CParticle : public CObjBoard
{
private:

protected:
	D3DXVECTOR3	m_Accel , m_RotSpd , m_ScallSpd;
	
	int	m_nLifeCnt;
	int	m_nLife , m_nInFrame , m_nOutFrame;

	unsigned int	m_nRed , m_nGreen , m_nBlue , m_nAlpha;

	void ParamSet(ParticleParam param);
public:
	CParticle();
	virtual ~CParticle();

	static CParticle* Create(LPCTSTR texName , ParticleParam param , int nWidth , int nHeight , int nSceneID , int nPri);
	virtual bool	Init(LPCTSTR texName , ParticleParam param , int nWidth , int nHeight , int nSceneID , int nPri);

	// �e�N�X�`���p�^�[���̐؂�ւ�
	virtual	void	Update();

	virtual void	SetRS();
	virtual void	SetRSLate();
	
	virtual	void	SetRGBA(int R , int G , int B , int A);
};
//=================================================================================


//=================================================================================
//	End of File
//=================================================================================