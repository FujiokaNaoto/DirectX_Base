//**�v���O�����w�b�_***************************************************************
//	�v���O�����T�v	:	�G�~�b�^�[�N���X����
//*********************************************************************************


//==include��======================================================================
#include <stdlib.h>
#include <tchar.h>

#include "../../System/Register.h"
#include "Emitter2D.h"
#include "..\Obj2D\Particle2D.h"
//=================================================================================


//==�萔�E�񋓌^��=================================================================
namespace eEmitterConst
{
	const float	DEFAULT_SIZE = 5.0f;

	const int	FEADIN_FARME = 5;
	const int	FEADOUT_FRAME = 5;
};
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
CEmitter2D::CEmitter2D():
m_pParent(NULL),
m_TexName(NULL),
m_nMyLife(eEmitterConst::NONE_LIFE),
m_bActive(true),
m_nGenerateNum(0),
m_nGenerateLenge(0),
m_nGenerateCnt(0),
m_nLifeRandLenge(1),
m_fWidth(0.0f), 
m_fHeight(0.0f),
m_fKeyWidth(0.0f), 
m_fKeyHeight(0.0f),
m_fWidthLenge(0.0f), 
m_fHeightLenge(0.0f),
m_ParRandPosLenge(D3DXVECTOR3(0.0f , 0.0f , 0.0f)),
m_ParRandSpdLenge(D3DXVECTOR3(0.0f , 0.0f , 0.0f)),
m_ParRandRotLenge(D3DXVECTOR3(0.0f , 0.0f , 0.0f)),
m_ParRandScallLenge(D3DXVECTOR3(0.0f , 0.0f , 0.0f)),
m_ParRandRotSpdLenge(D3DXVECTOR3(0.0f , 0.0f , 0.0f)), 
m_ParRandScallSpdLenge(D3DXVECTOR3(0.0f , 0.0f , 0.0f)),
m_ParRandAccelLenge(D3DXVECTOR3(0.0f , 0.0f , 0.0f))
{
	memset(&m_SendParam , 0 , sizeof(m_SendParam));
	m_SendParam.ParInitScall = DEFAULT_SCALL;
	m_SendParam.nInFrame = eEmitterConst::FEADIN_FARME;
	m_SendParam.nOutFrame = eEmitterConst::FEADOUT_FRAME;

	m_SendParam.nRed = m_SendParam.nGreen = m_SendParam.nBlue = m_SendParam.nAlpha = 255;
}


//**�֐�***************************************************************************
//	�T�v	:	����
//*********************************************************************************
CEmitter2D* CEmitter2D::Create(LPCTSTR texName , CObjBase* parent , int nSceneID , int nPri , int nGenerateLenge , 
	int nGenerateNum , int nParLife)
{
	CEmitter2D*	pObj = new CEmitter2D();

	if(pObj->Init(texName , parent , nSceneID , nPri , nGenerateLenge ,
		nGenerateNum , nParLife))
		return pObj;

	SAFE_DELETE(pObj);
	return NULL;
}


//**�֐�***************************************************************************
//	�T�v	:	����
//*********************************************************************************
CEmitter2D* CEmitter2D::Create(LPCTSTR texName , CObjBase* parent , int nSceneID , int nPri , int nGenerateLenge , 
	int nGenerateNum , int nParLife , float fWidth , float fHeight)
{
	CEmitter2D*	pObj = new CEmitter2D();

	if(pObj->Init(texName , parent , nSceneID , nPri , nGenerateLenge ,
		nGenerateNum , nParLife , fWidth , fHeight))
		return pObj;

	SAFE_DELETE(pObj);
	return NULL;
}


//**�֐�***************************************************************************
//	�T�v	:	������
//*********************************************************************************
bool CEmitter2D::Init(LPCTSTR texName , CObjBase* parent , int nSceneID , int nPri , int nGenerateLenge ,
		int nGenerateNum , int nParLife)
{
	return Init(texName , parent , nSceneID , nPri , nGenerateLenge ,
		nGenerateNum , nParLife , eEmitterConst::DEFAULT_SIZE , eEmitterConst::DEFAULT_SIZE);
}


//**�֐�***************************************************************************
//	�T�v	:	������
//*********************************************************************************
bool CEmitter2D::Init(LPCTSTR texName , CObjBase* parent , int nSceneID , int nPri , int nGenerateLenge ,
		int nGenerateNum , int nParLife , float fWidth , float fHeight)
{
	if(parent)
		m_pParent = parent;

	m_TexName = texName;
	m_nSceneID = nSceneID;
	m_nPriority = nPri;
	m_nGenerateLenge = nGenerateLenge;
	m_nGenerateNum = nGenerateNum;

	m_KeyParam.nParLife = nParLife;

	m_fWidth = m_fKeyWidth = fWidth;
	m_fHeight = m_fKeyHeight = fHeight;

	return true;
}


//**�֐�***************************************************************************
//	�T�v	:	�X�V
//*********************************************************************************
void CEmitter2D::Update()
{
	if(!m_bActive)
		return;			// ���쒆�ȊO�͍X�V�Ȃ�

	if(m_nMyLife != eEmitterConst::NONE_LIFE)
	{
		m_nMyLife --;
		if(m_nMyLife == 0)
		{
			m_bDeleteFlg = true;
			m_bActive = false;
		}
	}

	m_nGenerateCnt ++;

	if(m_nGenerateCnt >= m_nGenerateLenge)
	{
		m_nGenerateCnt -= m_nGenerateLenge;
		ParticleGenerate();
	}
}


//**�֐�***************************************************************************
//	�T�v	:	�p�[�e�B�N������
//*********************************************************************************
void CEmitter2D::ParticleGenerate()
{
	CParticle2D*	pBuf;

	// ���������A�N���G�C�g
	for(int i = 0 ; i < m_nGenerateNum ; i ++)
	{
		SendParamSet();
		pBuf = CParticle2D::Create(m_TexName , m_SendParam , (int)m_fWidth ,
			(int)m_fHeight , m_nSceneID , m_nPriority);

		pBuf->SetDrawLateFlg(false);

		REGISTER->Entry(pBuf , eOBJSTAGE::EFFECT);
	}
}


//**�֐�***************************************************************************
//	�T�v	:	�p�[�e�B�N���ɑ���f�[�^�̃Z�b�g
//*********************************************************************************
void CEmitter2D::SendParamSet()
{
	if(m_pParent)
		m_SendParam.ParInitPos = m_pParent->GetPos() + m_Pos + m_KeyParam.ParInitPos + RANDF * m_ParRandPosLenge;
	else
		m_SendParam.ParInitPos = m_Pos + m_KeyParam.ParInitPos + RANDF * m_ParRandPosLenge;

	m_SendParam.ParInitSpd = m_KeyParam.ParInitSpd + RANDF * m_ParRandSpdLenge;
	m_SendParam.ParInitScall = m_KeyParam.ParInitScall + RANDF * m_ParRandScallLenge;
	m_SendParam.ParInitRot = m_KeyParam.ParInitRot + RANDF * m_ParRandRotLenge;

	m_SendParam.ParAccel = m_KeyParam.ParAccel + RANDF * m_ParRandAccelLenge;
	m_SendParam.ParRotSpd = m_KeyParam.ParRotSpd + RANDF * m_ParRandRotSpdLenge;
	m_SendParam.ParScallSpd = m_KeyParam.ParScallSpd + RANDF * m_ParRandScallSpdLenge;

	m_SendParam.nParLife = m_KeyParam.nParLife + (int)(RANDF * m_nLifeRandLenge);

	m_SendParam.nInFrame = m_KeyParam.nInFrame;
	m_SendParam.nOutFrame = m_KeyParam.nOutFrame;

	m_SendParam.nRed = m_KeyParam.nRed;
	m_SendParam.nGreen = m_KeyParam.nGreen;
	m_SendParam.nBlue = m_KeyParam.nBlue;

	m_fWidth = m_fKeyWidth + RANDF * m_fWidthLenge;
	m_fHeight = m_fKeyHeight + RANDF * m_fHeightLenge;
}


//**�֐�***************************************************************************
//	�T�v	:	�A�t�B���ϊ��Ȃ�
//*********************************************************************************
void CEmitter2D::Affine()
{
}


//**�֐�***************************************************************************
//	�T�v	:	�O�`�揈���Ȃ�
//*********************************************************************************
void CEmitter2D::Draw()
{
}


//**�֐�***************************************************************************
//	�T�v	:	��`�揈���Ȃ�
//*********************************************************************************
void CEmitter2D::DrawLate()
{
}
//=================================================================================
//	End of File
//=================================================================================