//**�v���O�����w�b�_***************************************************************
//	�v���O�����T�v	:	�G�t�F�N�g�p�[�e�B�N���N���X����
//*********************************************************************************


//==include��======================================================================
#include "../../System/GameManager.h"
#include "Particle2D.h"
//=================================================================================


//==�萔�E�񋓌^��=================================================================
//=================================================================================


//==�}�N����`��===================================================================

//=================================================================================


//==�N���X�E�\���̐錾��===========================================================

//=================================================================================


//==�ÓI�����o�ϐ���===============================================================

//=================================================================================


//==�v���g�^�C�v�錾===============================================================

//=================================================================================


//**�֐�***************************************************************************
//	�T�v	:	�R���X�g���N�^
//*********************************************************************************
CParticle2D::CParticle2D():
m_nLifeCnt(0),
m_nLife(0),
m_nInFrame(2),
m_nOutFrame(2),
m_nRed(255),
m_nBlue(255),
m_nGreen(255),
m_nAlpha(1)
{
	m_nStatus = eParticleConst2D::STATE_IN;				// �ŏ��̓t�F�[�h�C�����[�h

	m_nRSType = eRSType::PARTICLE_2D;
	m_nRSTypeLate = eRSType::PARTICLE_LATE_2D;
}


//**�֐�***************************************************************************
//	�T�v	:	�f�X�g���N�^
//*********************************************************************************
CParticle2D::~CParticle2D()
{
}


//**�֐�***************************************************************************
//	�T�v	:	����
//*********************************************************************************
CParticle2D* CParticle2D::Create(LPCTSTR texName , ParticleParam2D param , int nWidth , int nHeight , int nSceneID , int nPri)
{
	CParticle2D* pObj = new CParticle2D();

	if(pObj->Init(texName , param , nWidth , nHeight , nSceneID , nPri))
		return pObj;

	SAFE_DELETE(pObj);
	return NULL;
}


//**�֐�***************************************************************************
//	�T�v	:	������
//*********************************************************************************
bool CParticle2D::Init(LPCTSTR texName , ParticleParam2D param , int nWidth , int nHeight , int nSceneID , int nPri)
{
	
	if(! CObj2D::Init(texName , param.ParInitPos , nWidth , nHeight , nSceneID , nPri))
		return false;

	ParamSet(param);								// �p�[�e�B�N���p�����[�^�Z�b�g
	SetDiffuse(m_nRed , m_nGreen , m_nBlue , 0);
	Affine();

	return true;
}


//**�֐�***************************************************************************
//	�T�v	:	�p�[�e�B�N���p�����[�^�Z�b�g
//*********************************************************************************
void CParticle2D::ParamSet(ParticleParam2D param)
{
	m_Pos = param.ParInitPos;
	m_Spd = param.ParInitSpd;
	m_Scall = param.ParInitScall;
	m_Rot = param.ParInitRot;

	m_Accel = param.ParAccel;
	m_RotSpd = param.ParRotSpd;
	m_ScallSpd = param.ParScallSpd;

	m_nLife = param.nParLife;
	m_nInFrame = param.nInFrame;
	m_nOutFrame = param.nOutFrame;

	m_nRed = param.nRed;
	m_nGreen = param.nGreen;
	m_nBlue = param.nBlue;

	if(m_nAlpha > 0)
		m_nAlpha = param.nAlpha;
	else
		m_nAlpha = 1;

	SetDiffuse(m_nRed , m_nGreen , m_nBlue , 0);

}


//**�֐�***************************************************************************
//	�T�v	:	�X�V
//*********************************************************************************
void	CParticle2D::Update()
{
	m_Spd += m_Accel;
	
	m_Pos += m_Spd;
	m_Rot += m_RotSpd;
	m_Scall += m_ScallSpd;

	// �����m�F
	m_nLifeCnt ++;
	if(m_nLifeCnt >= m_nLife)
	{
		m_bDeleteFlg = true;
		return;
	}

	// �����x�ύX
	int nAlpha = 255;
	switch(m_nStatus)
	{
	case eParticleConst2D::STATE_IN:
		nAlpha = (int)(((float)m_nLifeCnt / (float)m_nInFrame) * (float)m_nAlpha);
		SetDiffuse(m_nRed , m_nGreen , m_nBlue , nAlpha);

		if(m_nLifeCnt >= m_nInFrame)
		{
			SetDiffuse(m_nRed , m_nGreen , m_nBlue , m_nAlpha);
			m_nStatus = eParticleConst2D::STATE_UPDATE;
		}
		break;

	case eParticleConst2D::STATE_UPDATE:
		if(m_nLifeCnt >= m_nLife - m_nOutFrame)
			m_nStatus = eParticleConst2D::STATE_OUT;
		break;

	case eParticleConst2D::STATE_OUT:
		nAlpha = (int)(m_nAlpha - ((float)m_nAlpha * ((float)m_nLife - (float)m_nLifeCnt) / (float)m_nOutFrame));
		SetDiffuse(m_nRed , m_nGreen , m_nBlue , nAlpha);
		break;
	}

	AnimUpdate();
}


//**�֐�***************************************************************************
//	�T�v	:	�����_�[�X�e�[�g�ύX
//*********************************************************************************
void CParticle2D::SetRS()
{
	CGraphics::SemafoLock();
	LPDIRECT3DDEVICE9 pDevice = CGraphics::GetDevice();

	// ���C�g�I�t
	pDevice->SetRenderState(D3DRS_LIGHTING, FALSE);

	// �A���t�@ �u�����h�L����
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
	
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);

	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_ONE);

	// Z�o�b�t�@�I�t
	pDevice->SetRenderState(D3DRS_ZENABLE , FALSE);
	pDevice->SetFVF(FVF_VERTEX_2D);

	pDevice->SetRenderState(D3DRS_CULLMODE , D3DCULL_CCW);

	CGraphics::SemafoUnlock();
}


//**�֐�***************************************************************************
//	�T�v	:	�F�R�[�h�Z�b�g
//*********************************************************************************
void CParticle2D::SetRGBA(int R , int G , int B , int A)
{
	m_nRed = R;
	m_nGreen = G;
	m_nBlue = B;

	if(A > 0) m_nAlpha = A;
	else m_nAlpha = 1;
}
//=================================================================================
//	End of File
//=================================================================================