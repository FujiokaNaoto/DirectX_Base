//**�v���O�����w�b�_***************************************************************
//	�v���O�����T�v	:	2D�{�^���N���X����
//*********************************************************************************


//==include��======================================================================
#include "../../System/GameManager.h"

#include "ObjButton.h"
//=================================================================================


//==�萔�E�񋓌^��=================================================================
namespace eButtonConst
{
	// �g��֌W�̒萔
	const float	SCALL_SPD = 0.002f;
	const float	SCALL_LENGE = 0.1f;
	
	// �㉺�ړ��֌W�̒萔
	const float	SIN_SPD = 0.0625f;
	const float	SIN_LENGE = 10.0f;
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
CObjButton::CObjButton() :
m_nMoveType(eButtonMoveType::NONE),

m_fRadLenge(eButtonConst::SIN_LENGE),
m_fRadian(0.0f),
m_fRadSpd(eButtonConst::SIN_SPD),
m_KeyPos(DEFAULT_POS),

m_fScallLenge(eButtonConst::SCALL_LENGE),
m_fScallSpd(eButtonConst::SCALL_SPD),
m_bExtendFlg(true),
m_bTriggerFlg(false)
{
	m_nClassID = eClassCode::OBJ_BUTTON;

	m_nStatus = eButtonState::DEFAULT;					// �{�^���������
}


//**�֐�***************************************************************************
//	�T�v	:	�f�X�g���N�^
//*********************************************************************************
CObjButton::~CObjButton()
{

}


//**�֐�***************************************************************************
//	�T�v	:	����
//*********************************************************************************
CObjButton* CObjButton::Create(LPCTSTR texName , D3DXVECTOR3 pos , 
	 int nWidth , int nHeight , int nSceneID , int nPriority)
{
	CObjButton*		pObj = new CObjButton;

	if(pObj->Init(texName , pos , DEFAULT_ROT , DEFAULT_SCALL , nWidth , nHeight , nSceneID ,
		nPriority))
		return pObj;
	
	SAFE_DELETE(pObj);
	return NULL;
}


//**�֐�***************************************************************************
//	�T�v	:	����
//*********************************************************************************
CObjButton* CObjButton::Create(LPCTSTR texName , D3DXVECTOR3 pos , 
		D3DXVECTOR3 rot , D3DXVECTOR3 scall , int nWidth , 
		int nHeight , int nSceneID , int nPriority)
{
	CObjButton*		pObj = new CObjButton;

	if(pObj->Init(texName , pos , rot , scall , nWidth , nHeight , nSceneID ,
		nPriority))
		return pObj;
	
	SAFE_DELETE(pObj);
	return NULL;
}


//**�֐�***************************************************************************
//	�T�v	:	������
//*********************************************************************************
bool CObjButton::Init(LPCTSTR texName , D3DXVECTOR3 pos , 
		 int nWidth , int nHeight , int nSceneID , int nPriority)
{
	return Init(texName , pos , DEFAULT_ROT , DEFAULT_SCALL , nWidth , nHeight ,
		nSceneID , nPriority);
}


//**�֐�***************************************************************************
//	�T�v	:	������
//*********************************************************************************
bool CObjButton::Init(LPCTSTR texName , D3DXVECTOR3 pos , 
		D3DXVECTOR3 rot , D3DXVECTOR3 scall , int nWidth , 
		int nHeight , int nSceneID , int nPriority)
{
	if(! CObj2D::Init(texName , pos , rot , scall , nWidth , nHeight ,
		nSceneID , nPriority))
		return false;
	
	m_KeyPos =  pos;
	return true;
}


//**�֐�***************************************************************************
//	�T�v	:	�X�V
//*********************************************************************************
void CObjButton::Update()
{
	UpdateMove();				// �����X�V
 	UpdateState();				// ��ԍX�V
}


//**�֐�***************************************************************************
//	�T�v	:	�����X�V
//*********************************************************************************
void CObjButton::UpdateMove()
{
	// �s�������͓���Ȃ�
	if(m_nStatus == eButtonState::INACTIVE)
		return;

	// �㉺�ړ�
	if(m_nMoveType & eButtonMoveType::SIN_MOVE)
	{
		m_fRadian += m_fRadSpd;
		if(m_fRadian > 2 * D3DX_PI) 
			m_fRadian -= 2 * D3DX_PI;
		m_Pos.y = m_KeyPos.y + sin(m_fRadian) * m_fRadLenge;
	}

	// �g�k
	if(m_nMoveType & eButtonMoveType::SCALL)
	{
		if(m_bExtendFlg)
		{
			m_Scall.x += m_fScallSpd;
			m_Scall.y += m_fScallSpd;

			if(m_Scall.x >= 1.0f + m_fScallLenge)
				m_bExtendFlg = false;
		}
		else
		{
			m_Scall.x -= m_fScallSpd;
			m_Scall.y -= m_fScallSpd;

			if(m_Scall.x <= 1.0f)
				m_bExtendFlg = true;
		}
	}
}


//**�֐�***************************************************************************
//	�T�v	:	��ԍX�V
//*********************************************************************************
void CObjButton::UpdateState()
{
	switch(m_nStatus)
	{
	case eButtonState::DEFAULT:
		if(m_bTriggerFlg)
			m_bTriggerFlg = false;
		break;

	case eButtonState::HOVER:
		break;

	case eButtonState::ACTIVE:
		break;

	case eButtonState::INACTIVE:
		break;

	default:
		// �G���[
		break;
	}
}


//**�֐�***************************************************************************
//	�T�v	:	�����蔻��
//*********************************************************************************
void CObjButton::CheckCollision()
{
	// ���g�p���͏����Ȃ�
	if(m_nStatus == eButtonState::INACTIVE) 
		return;

	CDXInput* pInput = MANAGER.GetInput();

	switch(m_nStatus)
	{
	case eButtonState::DEFAULT:
		if(CCalc::IntersectBox2D(this->GetHitRect() , pInput->GetMouseHitRect()))
			SetState(eButtonState::HOVER);
		break;

	case eButtonState::HOVER:
		if(! (CCalc::IntersectBox2D(this->GetHitRect() , pInput->GetMouseHitRect() )) )
			SetState(eButtonState::DEFAULT);

		if(pInput->GetMouseTrigger(0))
			OnTrigger();
		else if(pInput->GetMouseRelease(0))
			OnRelease();
		break;

	case eButtonState::ACTIVE:

		if(pInput->GetMouseTrigger(0))
			OnTrigger();
		else if(pInput->GetMouseRelease(0))
			OnRelease();
		break;

	default:
		// �G���[
		break;
	}

	return;
}


//**�֐�***************************************************************************
//	�T�v	:	��ԕύX
//*********************************************************************************
void CObjButton::OnTrigger()
{
	m_bTriggerFlg = true;
}


//**�֐�***************************************************************************
//	�T�v	:	��ԕύX
//*********************************************************************************
void CObjButton::OnRelease()
{
	switch(m_nStatus)
	{
	case eButtonState::HOVER:
		if(m_bTriggerFlg)
			SetState(eButtonState::ACTIVE);
		break;

	case eButtonState::ACTIVE:
		if(m_bTriggerFlg)
			SetState(eButtonState::HOVER);
		break;

	default:
		// �G���[
		break;
	}
}

//**�֐�***************************************************************************
//	�T�v	:	��ԕύX
//*********************************************************************************
void CObjButton::SetState(int nState)
{
	m_nStatus = nState;

	// ��Ԃɍ��킹�ă{�^���̗l�q��ύX
	switch(m_nStatus)
	{
	case eButtonState::DEFAULT:
		SetDiffuse(220 , 220 , 220 , 255);
		break;

	case eButtonState::HOVER:
		SetDiffuse(255 , 255 , 255 , 255);
		break;

	case eButtonState::ACTIVE:
		SetDiffuse(255 , 255 , 255 , 255);
		break;

	case eButtonState::INACTIVE:
		SetDiffuse(55 , 55 , 55 , 255);
		break;

	default:
		// �G���[
		break;
	}
	
}

//=================================================================================
//	End of File
//=================================================================================