//**�v���O�����w�b�_***************************************************************
//	�v���O�����T�v	:	�t�F�[�h�I�u�W�F
//*********************************************************************************


//==include��======================================================================
#include "FeadObj.h"
#include <tchar.h>
#include "../../System/GameManager.h"
//=================================================================================


//==�萔�E�񋓌^��=================================================================
namespace eFeadObjConst
{
	LPCTSTR	FEAD_TEX = _T("../resource_data/img/Feadout.png");

	const int	IN_TIME = 20;
	const int	MIDDLE_TIME = 15;
	const int	OUT_TIME = 20;
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
CFeadObj::CFeadObj():
m_bMiddleStop(false),
m_nFeadType(eFeadObjConst::BLACKFEAD),
m_nInTime(eFeadObjConst::IN_TIME), 
m_nMiddleTime (eFeadObjConst::MIDDLE_TIME), 
m_nOutTime(eFeadObjConst::OUT_TIME),
m_nFeadCnt(0)
{
	m_nClassID = eClassCode::OBJ_FEAD;
}


//**�֐�***************************************************************************
//	�T�v	:	�f�X�g���N�^
//*********************************************************************************
CFeadObj::~CFeadObj()
{
}


//**�֐�***************************************************************************
//	�T�v	:	����
//*********************************************************************************
CFeadObj* CFeadObj::Create(D3DXVECTOR3 pos , int nWidth , int nHeight ,
	int nSceneID , int nPriority , bool bMiddleStop)
{
	CFeadObj* pObj = new CFeadObj();

	if(pObj->Init(pos , nWidth , nHeight , nSceneID , nPriority ,bMiddleStop))
		return pObj;

	else SAFE_RELEASE(pObj);
	return NULL;
}


//**�֐�***************************************************************************
//	�T�v	:	������
//*********************************************************************************
bool CFeadObj::Init(D3DXVECTOR3 pos , int nWidth , int nHeight ,
	int nSceneID , int nPriority , bool bMiddleStop)
{
	if(! CObj2D::Init(eFeadObjConst::FEAD_TEX , pos , nWidth , nHeight , nSceneID , nPriority))
		return false;

	m_bMiddleStop = bMiddleStop;
	m_bUseFlg = false;

	if(m_nFeadType == eFeadObjConst::WHITEFEAD)
		SetDiffuse(255 , 255 , 255 , 0);
	else
		SetDiffuse(0 , 0 , 0 , 0);

	return true;
}


//**�֐�***************************************************************************
//	�T�v	:	�X�V
//*********************************************************************************
void CFeadObj::Update()
{
	m_nFeadCnt ++;
	
	int	nAlpha = 0;

	switch(m_nStatus)
	{
	// �t�F�[�h�C��
	case eFeadObjConst::FEAD_IN:
		nAlpha = (int)((float)(255.0f * ((float)m_nFeadCnt / (float)m_nInTime)));

		if(m_nFeadType == eChanging::CHANGE_BLACKFEAD)
			SetDiffuse(0 , 0 , 0 , nAlpha);
		else
			SetDiffuse(255 , 255 , 255 , nAlpha);


		// �t�F�[�h��~��Ԃֈڍs
		if(m_nFeadCnt >= m_nInTime)
		{
			m_nFeadCnt = 0;
			m_nStatus = eFeadObjConst::FEAD_MIDDLE;

			if(m_nFeadType == eChanging::CHANGE_BLACKFEAD)
				SetDiffuse(0 , 0 , 0 , 255);
			else
				SetDiffuse(255 , 255 , 255 , 255);
		}
		break;

	// ����
	case eFeadObjConst::FEAD_MIDDLE:
		if(m_nFeadCnt >= m_nMiddleTime)
		{
			m_nFeadCnt = 0;

			if(m_bMiddleStop)
				m_nStatus = eFeadObjConst::FEAD_MIDDLE_STOP;
			else
				m_nStatus = eFeadObjConst::FEAD_OUT;

		}
		break;

	// ���Ԓ�~
	case eFeadObjConst::FEAD_MIDDLE_STOP:

		break;

	case eFeadObjConst::FEAD_OUT:
		nAlpha = (int)((255.0f - (255.0f * ((float)m_nFeadCnt / (float)m_nOutTime))));

		if(m_nFeadType == eChanging::CHANGE_BLACKFEAD)
			SetDiffuse(0 , 0 , 0 , nAlpha);
		else
			SetDiffuse(255 , 255 , 255 , nAlpha);

		if(m_nFeadCnt >= m_nOutTime)
		{
			m_bUseFlg = false;
			m_nStatus = eFeadObjConst::STOP;
		}
		break;

	default:
		break;
	}
}


//**�֐�***************************************************************************
//	�T�v	:	�t�F�[�h�J�n
//*********************************************************************************
bool CFeadObj::FStart(int nType)
{
	// ���łɓ��쒆�Ȃ玸�s
	if(m_bUseFlg)
		return false;

	m_nFeadType = nType;
	m_nStatus = eFeadObjConst::FEAD_IN;
	m_nFeadCnt = 0;

	if(m_nFeadType == eFeadObjConst::WHITEFEAD)
		SetDiffuse(255 , 255 , 255 , 0);
	else
		SetDiffuse(0 , 0 , 0 , 0);

	m_bUseFlg = true;

	return true;
}


//**�֐�***************************************************************************
//	�T�v	:	�t�F�[�h���Ԃ���J�n
//*********************************************************************************
bool CFeadObj::FStartMiddle(int nType)
{
	// ���łɓ��쒆�Ȃ玸�s
	if(m_bUseFlg)
		return false;

	m_nFeadType = nType;
	m_nStatus = eFeadObjConst::FEAD_MIDDLE;
	m_nFeadCnt = 0;

	if(m_nFeadType == eFeadObjConst::WHITEFEAD)
		SetDiffuse(255 , 255 , 255 , 255);
	else
		SetDiffuse(0 , 0 , 0 , 255);

	m_bUseFlg = true;

	return true;
}


//**�֐�***************************************************************************
//	�T�v	:	���Ԓ�~���t�F�[�h����J�n
//*********************************************************************************
bool CFeadObj::MiddleStart()
{
	// ���Ԓ�~���[�h�łȂ����܂��̓t�F�[�h�����Ԓ�~�ɗ��Ă��Ȃ����͎��s
	if(!m_bMiddleStop || m_nStatus != eFeadObjConst::FEAD_MIDDLE_STOP)
		return false;

	m_nStatus = eFeadObjConst::FEAD_OUT;
	m_nFeadCnt = 0;

	return true;
}


//**�֐�***************************************************************************
//	�T�v	:	�t�F�[�h���Ԑݒ�
//*********************************************************************************
bool CFeadObj::SetFeadTime(int nIn , int nMiddle , int nOut)
{
	// ���쒆�͕ύX���󂯕t���Ȃ�
	if(m_bUseFlg)
		return false;

	m_nInTime = nIn;
	m_nMiddleTime = nMiddle;
	m_nOutTime = nOut;
	return true;
}

//=================================================================================
//	End of File
//=================================================================================