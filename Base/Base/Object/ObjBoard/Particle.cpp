//**�v���O�����w�b�_***************************************************************
//	�v���O�����T�v	:	�G�t�F�N�g�p�[�e�B�N���N���X����
//*********************************************************************************


//==include��======================================================================
#include "../../System/GameManager.h"
#include "Particle.h"
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
CParticle::CParticle():
m_nLifeCnt(0),
m_nLife(0),
m_nInFrame(2),
m_nOutFrame(2),
m_nRed(255),
m_nBlue(255),
m_nGreen(255),
m_nAlpha(1)
{
	m_nStatus = eParticleConst::STATE_IN;				// �ŏ��̓t�F�[�h�C�����[�h
	m_nRSType = eRSType::PARTICLE;
	m_nRSTypeLate = eRSType::PARTICLE_LATE;
}


//**�֐�***************************************************************************
//	�T�v	:	�f�X�g���N�^
//*********************************************************************************
CParticle::~CParticle()
{
}


//**�֐�***************************************************************************
//	�T�v	:	����
//*********************************************************************************
CParticle* CParticle::Create(LPCTSTR texName , ParticleParam param , int nWidth , int nHeight , int nSceneID , int nPri)
{
	CParticle* pObj = new CParticle();

	if(pObj->Init(texName , param , nWidth , nHeight ,nSceneID , nPri))
		return pObj;

	SAFE_DELETE(pObj);
	return NULL;
}


//**�֐�***************************************************************************
//	�T�v	:	������
//*********************************************************************************
bool CParticle::Init(LPCTSTR texName , ParticleParam param , int nWidth , int nHeight , int nSceneID , int nPri)
{
	if(! CObjBoard::Init(texName , DEFAULT_POS , nWidth , nHeight , nSceneID , nPri))
		return false;

	m_bBillFlg = false;
	SetDiffuse(m_nRed , m_nGreen , m_nBlue , 0);

	ParamSet(param);								// �p�[�e�B�N���p�����[�^�Z�b�g

	return true;
}


//**�֐�***************************************************************************
//	�T�v	:	�p�[�e�B�N���p�����[�^�Z�b�g
//*********************************************************************************
void CParticle::ParamSet(ParticleParam param)
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
void	CParticle::Update()
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
	case eParticleConst::STATE_IN:
		nAlpha = (int)(((float)m_nLifeCnt / (float)m_nInFrame) * (float)m_nAlpha);
		SetDiffuse(m_nRed , m_nGreen , m_nBlue , nAlpha);

		if(m_nLifeCnt >= m_nInFrame)
		{
			SetDiffuse(m_nRed , m_nGreen , m_nBlue , m_nAlpha);
			m_nStatus = eParticleConst::STATE_UPDATE;
		}
		break;

	case eParticleConst::STATE_UPDATE:
		if(m_nLifeCnt >= m_nLife - m_nOutFrame)
			m_nStatus = eParticleConst::STATE_OUT;
		break;

	case eParticleConst::STATE_OUT:
		nAlpha = (int)(m_nAlpha - ((float)m_nAlpha * ((float)m_nLife - (float)m_nLifeCnt) / (float)m_nOutFrame));
		SetDiffuse(m_nRed , m_nGreen , m_nBlue , nAlpha);
		break;
	}
}


//**�֐�***************************************************************************
//	�T�v	:	�����_�[�X�e�[�g�Z�b�g
//*********************************************************************************
void CParticle::SetRS()
{
	CGraphics::SemafoLock();

	LPDIRECT3DDEVICE9 pDevice = MANAGER.GetGraph()->GetDevice();

	pDevice->SetFVF(FVF_VERTEX_BOARD);

	pDevice->SetRenderState(D3DRS_ZENABLE , TRUE);
	pDevice->SetRenderState(D3DRS_LIGHTING , FALSE);
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);

	pDevice->SetRenderState(D3DRS_CULLMODE , D3DCULL_NONE);				// �J�����O�I�t

	//pDevice->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_INVSRCALPHA);

	// �����p�^�[��
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_ONE);

	CGraphics::SemafoUnlock();
}


//**�֐�***************************************************************************
//	�T�v	:	��`�惌���_�[�X�e�[�g�Z�b�g
//*********************************************************************************
void CParticle::SetRSLate()
{
	CGraphics::SemafoLock();

	// �O���t�B�b�N�f�o�C�X�擾
	LPDIRECT3DDEVICE9 pDevice = MANAGER.GetGraph()->GetDevice();

	// ���_�X�g���[���ݒ�
	pDevice->SetFVF(FVF_VERTEX_BOARD);

	pDevice->SetRenderState(D3DRS_CULLMODE , D3DCULL_NONE);				// �J�����O�I�t

	// �`��ݒ�
	pDevice->SetRenderState(D3DRS_ZENABLE , TRUE);						// Z�o�b�t�@�I��
	pDevice->SetRenderState(D3DRS_LIGHTING , FALSE);					// ���C�g�I�t
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);				// �A���t�@�u�����h����
	

	// �����p�^�[��
	pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
	//pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
	pDevice->SetRenderState(D3DRS_DESTBLEND , D3DBLEND_ONE);

	CGraphics::SemafoUnlock();
}


//**�֐�***************************************************************************
//	�T�v	:	�F�R�[�h�Z�b�g
//*********************************************************************************
void CParticle::SetRGBA(int R , int G , int B , int A)
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