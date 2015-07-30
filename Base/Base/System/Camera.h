//�v���O�����w�b�_*****************************************************************
//	�t�@�C����		:	Camera.h
//---------------------------------------------------------------------------------
//	�v���O�����T�v	:	�J�����Ǘ��w�b�_�[
//---------------------------------------------------------------------------------
//	�쐬��			:	�������l	
//---------------------------------------------------------------------------------
//	����			:	
//*********************************************************************************
#ifndef __CAMERA_H__

#define __CAMERA_H__

//==include��======================================================================
#include <d3d9.h>
#include <d3dx9.h>

#include "common.h"
//=================================================================================


//==�萔��=========================================================================
//=================================================================================


//==�񋓌^�錾��===================================================================
//  �J�����̍X�V���[�h
namespace eCameraState
{
	enum 
	{
		FIRSTPERSON = 1,	// 1�l��
		THIRDPERSON,		// 3�l��
		FIXED,				// �Œ�

		MONOSPACE,			// �����Ǐ]�A�����_����˂Ɉ��ʈ���������

		MAX_CT
	};

	// ��_
	const D3DXVECTOR3	POS_FIX		= D3DXVECTOR3(0.0f , 200.0f , - 100.0f );
	const D3DXVECTOR3	LOOK_FIX	= D3DXVECTOR3(0.0f , 10.0f , 1000.0f );
	const D3DXVECTOR3	UP_FIX		= D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	// 3�l��
	const D3DXVECTOR3	POS_THIRD	= D3DXVECTOR3(0.0f , 80.0f , -200.0f);
	const D3DXVECTOR3	LOOK_THIRD	= D3DXVECTOR3(0.0f , 0.0f , 1000.0f);
	// 1�l��
	const D3DXVECTOR3	POS_FIRST	= D3DXVECTOR3(0.0f , 60.0f , 36.0f );
	const D3DXVECTOR3	LOOK_FIRST	= D3DXVECTOR3(0.0f , 30.0f , 1000.0f );

	// �f�t�H���g�A�J�������������W
	const D3DXVECTOR3	LENGE_MONOSPACE	= D3DXVECTOR3(0.0f , 250.0f , -300.0f );

	const int			MODECHANGE_FRAME = 20;				// �l�ύX�t���[��
};
//=================================================================================


//==�}�N����`��===================================================================
//=================================================================================


//==�N���X�錾��===================================================================
class CCamera
{
private:
	// ��ɂ���I�u�W�F�N�g�̃|�C���^��ێ�
	S_HANDLE	m_hKeyObj;

	D3DXMATRIX	m_matrixView;											// �r���[�}�g���b�N�X
	
	// ���_
	D3DXVECTOR3	m_vPos;													// �J�����ʒu
	D3DXVECTOR3	m_vKeyPos;												// ��J�����ʒu
	D3DXVECTOR3	m_vChangePos;											// �J�����ʒu�ύX�p�̕ψ�

	// �����_
	D3DXVECTOR3	m_vLook;												// �J���������_
	D3DXVECTOR3	m_vKeyLook;												// ��J���������_
	D3DXVECTOR3	m_vChangeLook;											// �J���������_�ύX�p�̕ψ�

	// �A�b�v�x�N�g��
	D3DXVECTOR3	m_vUp;													// �J�����A�b�v�x�N�g��
	D3DXVECTOR3	m_vKeyUp;												// ��J�����A�b�v�x�N�g��

	// �����Ǐ]�����W
	D3DXVECTOR3 m_vLenge;												// �����Ǐ]�����W�̕�

	int			m_nMode;												// �J�����̎��_���[�h
	int			m_nChangeCnt;											// �J�����̎��_���[�h�̕␳�J�E���^

public:

	CCamera();
	~CCamera();

	static CCamera* Create(D3DXVECTOR3 pos , D3DXVECTOR3 Look , 
		D3DXVECTOR3 UpVec , int nCameraMode , S_HANDLE hObj);			// ����
	bool Initialize(D3DXVECTOR3 pos , D3DXVECTOR3 Look ,
		D3DXVECTOR3 UpVec , int nCameraMode , S_HANDLE hObj);			// ������������

	void Finalize();

	void Release();
	void Update();
	void Input();
	bool Draw(void);

	
	// --�J�����̏�ԕύX--
	bool SetCameraMode(int nMode , S_HANDLE hObj , 
		D3DXVECTOR3 vPos , D3DXVECTOR3 vLook , 
		D3DXVECTOR3 vUp , int nChangeCnt);								// �ڍוύX
	
	// �x�N�g���i�[
	bool SetPos(D3DXVECTOR3 Pos){m_vPos = Pos; return true;}
	bool SetLook(D3DXVECTOR3 Look){m_vLook = Look; return true;}
	bool SetUp(D3DXVECTOR3 Vec){m_vUp; return true;}
	// �x�N�g���擾
	D3DXVECTOR3	GetPos(void){return m_vPos;}
	D3DXVECTOR3	GetLook(void){return m_vLook;}
	D3DXVECTOR3	GetUp(void){return m_vUp;}

	// --�Q�b�g�E�Z�b�g--
	int	GetMode(void){return m_nMode;}
	D3DXMATRIX	GetMatrixView(void){return m_matrixView;}

	void SetMonospaceLenge(D3DXVECTOR3 vLenge){m_vLenge = vLenge;}		// �����Ǐ]�����ێ�


private:
};
//=================================================================================


//==�O���錾��=====================================================================
//=================================================================================


#endif