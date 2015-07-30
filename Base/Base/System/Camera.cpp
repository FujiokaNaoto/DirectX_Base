//**�v���O�����w�b�_***************************************************************
//	�v���O�����T�v	:	�J�����Ǘ�
//*********************************************************************************


//==include��======================================================================
#include "GameManager.h"
#include "Register.h"
#include "Camera.h"
//=================================================================================


//==�萔��=========================================================================
//=================================================================================


//==�񋓌^��`��===================================================================
//=================================================================================


//==�}�N����`��===================================================================
//	�Ȃ�
//=================================================================================


//==�N���X�錾��===================================================================
//	�Ȃ�
//=================================================================================


//==�ÓI�����o�ϐ���===============================================================
//	�Ȃ�
//=================================================================================


//==�v���g�^�C�v�錾===============================================================
//=================================================================================


//**�֐�***************************************************************************
//	�T�v	:	�R���X�g���N�^
//*********************************************************************************
CCamera::CCamera(void):
m_hKeyObj		(0),
m_vPos			(D3DXVECTOR3(0.0f , 0.0f , 0.0f)),
m_vKeyPos		(D3DXVECTOR3(0.0f , 0.0f , 0.0f)),	
m_vChangePos	(D3DXVECTOR3(0.0f , 0.0f , 0.0f)),

m_vLook			(D3DXVECTOR3(0.0f , 1.0f , 0.0f)),
m_vKeyLook		(D3DXVECTOR3(0.0f , 1.0f , 0.0f)),	
m_vChangeLook	(D3DXVECTOR3(0.0f , 0.0f , 0.0f)),

m_vUp			(D3DXVECTOR3(0.0f , 1.0f , 0.0f)),
m_vKeyUp		(D3DXVECTOR3(0.0f , 1.0f , 0.0f)),	

m_vLenge		(eCameraState::LENGE_MONOSPACE),	

m_nMode			(eCameraState::FIXED),	
m_nChangeCnt	(0)
{
	D3DXMatrixIdentity(&m_matrixView);
}


//**�֐�***************************************************************************
//	�T�v	:	�f�X�g���N�^
//*********************************************************************************
CCamera::~CCamera(void)
{
}


//**�֐�***************************************************************************
//	�T�v	:	����
//*********************************************************************************
CCamera* CCamera::Create(D3DXVECTOR3 pos , D3DXVECTOR3 Look , D3DXVECTOR3 UpVec , int nCameraMode , S_HANDLE hObj)
{
	CCamera* pCamera = new CCamera();
	if (pCamera) 
	{
		if (!pCamera->Initialize(pos , Look , UpVec , nCameraMode , hObj))
		{
			SAFE_DELETE(pCamera);
		}
	}
	return pCamera;
}


//**�֐�***************************************************************************
//	�T�v	:	������������
//*********************************************************************************
bool CCamera::Initialize(D3DXVECTOR3 pos , D3DXVECTOR3 Look , D3DXVECTOR3 UpVec , int nCameraMode , S_HANDLE hObj)
{
	// ���
	m_vPos	= m_vKeyPos = pos;
	m_vLook	= m_vKeyLook = Look;
	m_vUp	= m_vKeyUp = UpVec;

	// ��}�g���b�N�X������Ίi�[
	if(hObj)
		m_hKeyObj = hObj;
	else if(!hObj && (nCameraMode == eCameraState::FIRSTPERSON || nCameraMode == eCameraState::THIRDPERSON))
		return false;

	// �J����
	m_nChangeCnt = 0;								// �ύX���̃J�E���^
	m_nMode = nCameraMode;							// �f�t�H���g��_

	// �r���[�}�g���b�N�X������
	D3DXMatrixIdentity(&m_matrixView);

	// �r���[�}�g���b�N�X�ݒ�
	D3DXMatrixLookAtLH(&m_matrixView,
						&m_vPos,					// ���_���W
						&m_vLook,					// �����_���W
						&m_vUp);					// �A�b�v�x�N�g��
	MANAGER.GetGraph()->GetDevice()->SetTransform(D3DTS_VIEW, &m_matrixView);

	return true;
}


//**�֐�***************************************************************************
//	�T�v	:	���
//*********************************************************************************
void CCamera::Release()
{
	Finalize();
	delete this;
}


//**�֐�***************************************************************************
//	�T�v	:	�I������
//*********************************************************************************
void CCamera::Finalize()
{
	
}


//**�֐�***************************************************************************
//	�T�v	:	���͎�t
//*********************************************************************************
void CCamera::Input()
{
	CDXInput* pInput = MANAGER.GetInput();

}


//**�֐�***************************************************************************
//	�T�v	:	�X�V
//*********************************************************************************
void CCamera::Update()
{
	switch(m_nMode)
	{
	// 1�l��
	// 3�l��
	case eCameraState::FIRSTPERSON:
	case eCameraState::THIRDPERSON:
		if(m_nChangeCnt)
		{
			D3DXVec3TransformCoord(&m_vPos , &(m_vKeyPos - m_vChangePos * (float)m_nChangeCnt) , &REGISTER_H_P(m_hKeyObj , CObjBase*)->GetMatrix());
			D3DXVec3TransformCoord(&m_vLook , &(m_vKeyLook + m_vChangeLook * (float)m_nChangeCnt) , &REGISTER_H_P(m_hKeyObj , CObjBase*)->GetMatrix());
			m_nChangeCnt --;
		}
		else
		{
			D3DXVec3TransformCoord(&m_vPos , &m_vKeyPos , &REGISTER_H_P(m_hKeyObj , CObjBase*)->GetMatrix());
			D3DXVec3TransformCoord(&m_vLook , &m_vKeyLook , &REGISTER_H_P(m_hKeyObj , CObjBase*)->GetMatrix());
		}
		m_vUp.x = REGISTER_H_P(m_hKeyObj , CObjBase*)->GetMatrix()._21;
		m_vUp.y = REGISTER_H_P(m_hKeyObj , CObjBase*)->GetMatrix()._22;
		m_vUp.z = REGISTER_H_P(m_hKeyObj , CObjBase*)->GetMatrix()._23;
		break;


	// �����_������Ԋu�̈ʒu���ێ�
	case eCameraState::MONOSPACE:
		if(m_nChangeCnt)
		{
			D3DXVec3TransformCoord(&m_vPos , &(m_vKeyPos - m_vChangePos * (float)m_nChangeCnt) , &REGISTER_H_P(m_hKeyObj , CObjBase*)->GetMatrix());
			D3DXVec3TransformCoord(&m_vLook , &(m_vKeyLook + m_vChangeLook * (float)m_nChangeCnt) , &REGISTER_H_P(m_hKeyObj , CObjBase*)->GetMatrix());

			m_nChangeCnt --;
		}
		else
		{
			m_vLook = REGISTER_H_P(m_hKeyObj , CObjBase*)->GetPos();
			m_vPos = m_vLook + m_vLenge;
		}
		break;

	default:			// ��_�J����
		// �����_�ړ�
		if(m_nChangeCnt)
		{
			m_vPos = m_vPos - m_vChangePos * (float)m_nChangeCnt;
			m_nChangeCnt --;
		}
		else
		{
			m_vPos = m_vKeyPos;
			m_vLook = m_vKeyLook;
			m_vUp = m_vKeyUp;
		}
		break;
	}
}


//**�֐�***************************************************************************
//	�T�v	:	�r���[�}�g���b�N�X�X�V
//*********************************************************************************
bool CCamera::Draw(void)
{
	// �r���[�}�g���b�N�X�ݒ�
	D3DXMatrixLookAtLH(&m_matrixView,
						&m_vPos,					// ���_���W
						&m_vLook,					// �����_���W
						&m_vUp);					// �A�b�v�x�N�g��
	MANAGER.GetGraph()->GetDevice()->SetTransform(D3DTS_VIEW, &m_matrixView);

	return true;
}


//**�֐�***************************************************************************
//	�T�v	:	�J�������[�h�ύX
//*********************************************************************************
bool CCamera::SetCameraMode(int nMode , S_HANDLE hObj , D3DXVECTOR3 vPos , 
	D3DXVECTOR3 vLook , D3DXVECTOR3 vUp , int nChangeCnt)
{
	// �l�ύX
	/*if(m_nMode == nMode)
		return false;*/

	// �ړ���}�g���b�N�X
	m_hKeyObj = NULL;							// �}�g���b�N�X�N���A
	if(hObj)
		m_hKeyObj = hObj;


	D3DXVECTOR3	vNextPos , vNextLook;

	switch(nMode)
	{
	// 1�l��
	// 3�l��
	case eCameraState::FIRSTPERSON:
	case eCameraState::THIRDPERSON:
		if(!m_hKeyObj)
			return false;

		D3DXVec3TransformCoord(&vNextPos , &vPos , &REGISTER_H_P(m_hKeyObj , CObjBase*)->GetMatrix());
		D3DXVec3TransformCoord(&vNextLook , &vLook , &REGISTER_H_P(m_hKeyObj , CObjBase*)->GetMatrix());

		m_vKeyPos = vPos;
		m_vKeyLook = vLook;
		m_vKeyUp = vUp;
		break;


	// �����Ǐ]
	case eCameraState::MONOSPACE:
		if(!m_hKeyObj)
			return false;

		m_vKeyLook = vNextLook = REGISTER_H_P(m_hKeyObj , CObjBase*)->GetPos();
		m_vKeyPos = vNextPos = m_vKeyLook + m_vLenge;
		m_vKeyUp = eCameraState::UP_FIX;
		break;

	// ��_
	default:
		vNextPos = vPos;
		vNextLook = vLook;
		m_vKeyUp = vUp;
		break;
	}

	m_nMode = nMode;								// ���̃J�����̃��[�h�ݒ�
	
	// �J�����ύX���x�ݒ�
	if(nChangeCnt > 0)
	{
		m_nChangeCnt = nChangeCnt;					// �ύX�ɂ�����t���[��
		m_vChangePos = (vNextPos - m_vPos) / (float)m_nChangeCnt;
		m_vChangeLook = (vNextLook - m_vLook) / (float)m_nChangeCnt;
	}
	// �ύX�Ɏ��Ԃ͂����Ȃ�
	else
	{
		m_nChangeCnt = 0;
		m_vChangePos = m_vPos = vPos;
		m_vChangeLook = m_vLook = vLook;
		m_vUp = vUp;
	}

	return true;
}
//=================================================================================
//	End of File
//=================================================================================