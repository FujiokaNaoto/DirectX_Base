//�v���O�����w�b�_*****************************************************************
//	�v���O�����T�v	:	�I�u�W�F�N�g�h�����x�[�X�N���X
//*********************************************************************************
#pragma once

//==include��======================================================================
#include "../../System/common.h"
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib , "dxguid")	
#pragma comment(lib , "d3d9")
#pragma comment(lib , "d3dx9")
//=================================================================================

//==�萔�E�񋓌^�錾��=============================================================
const UINT ID_NONE = 0;

const D3DXVECTOR3	DEFAULT_POS = D3DXVECTOR3(0.0f , 0.0f , 0.0f);
const D3DXVECTOR3	DEFAULT_ROT = D3DXVECTOR3(0.0f , 0.0f , 0.0f);
const D3DXVECTOR3	DEFAULT_SCALL = D3DXVECTOR3(1.0f , 1.0f , 1.0f);

const D3DXVECTOR3	DEFAULT_SPD = D3DXVECTOR3(0.0f , 0.0f , 0.0f);
const D3DXVECTOR3	DEFAULT_ACCEL = D3DXVECTOR3(0.0f , 0.0f , 0.0f);
//=================================================================================


//==�}�N����`��===================================================================

//=================================================================================


//==�N���X�E�\���̐錾��===========================================================
class CRegister;
class CObjBase
{
public:
	// �R���X�g���N�^�E�f�X�g���N�^
	CObjBase();
	virtual ~CObjBase();

	static CObjBase*	Create(int nSceneID ,int nPriority);
	static CObjBase*	Create(D3DXVECTOR3 pos , int nSceneID ,
						int nPriority);

	virtual bool	Init(int nSceneID ,int nPriority);				// �Q�[�����[�v��������
	virtual bool	Init(D3DXVECTOR3 pos , int nSceneID ,
						int nPriority);								// �Q�[�����[�v��������

	virtual void	Uninit(){}										// �Q�[�����[�v���I������
	virtual void	Update();										// �X�V
	virtual void	OnInput(){};									// ���͎�t


	virtual void	SetRS(){}										// �����_�[�X�e�[�g�ݒ�
	virtual void	Draw(){}										// ��ɕ`��(3D�I�u�W�F�N�g�����2D�w�i��\��������A�s�����I�u�W�F��\��������)
	virtual void	SetRSLate(){}									// ��`��p�����_�[�X�e�[�g�ݒ�
	virtual void	DrawLate(){}									// ��ɕ`��(2D���j���[��UI�A�������I�u�W�F�̕\��)

	virtual void	Release();										// �������


	// �����蔻��
	virtual	void	CheckCollision(){}								// �����蔻�薽�߂��Ăяo��
	virtual bool	Collision(void* pObj , int ObjType){return false;}	// �����蔻��	nObjType�ő��肪�ǂ�ȃI�u�W�F�N�g���𔻒f����

	virtual	void	Affine();										// �A�t�B���ϊ�

	// ��r�֐����`
	static bool asc( const CObjBase* left, const CObjBase* right );

	// --�Q�b�g�Z�b�g--
	// �g�p����t���O
	virtual bool	GetUseFlg(){return m_bUseFlg;}
	virtual void	SetUpdateFlg(bool bFlg){m_bUseFlg = bFlg;}		
	// �`��^�C�~���O�t���O
	virtual bool	GetDrawLateFlg(){return m_bDrawLate;}			
	virtual void	SetDrawLateFlg(bool bFlg){m_bDrawLate = bFlg;}	

	// �폜�t���O�Atrue�ɂ���ƃ��W�X�^����폜�����
	virtual bool	GetDeleteFlg(){return m_bDeleteFlg;}			
	virtual void	SetDeleteFlg(bool bFlg){m_bDeleteFlg = bFlg;}	
		
	virtual UINT	GetObjID(){return m_nObjID;}					

	// �����N���X��ID�A��������Ƀ��W�X�^���猟������������
	virtual UINT	GetClassID(){return m_nClassID;}				
	virtual void	SetClassID(UINT nID){m_nClassID = nID;}			

	// �����_�[�X�e�[�g
	virtual UINT	GetRS(){return m_nRSType;}						
	virtual UINT	GetRSLate(){return m_nRSTypeLate;}				

	// �����V�[��������ID�A�V�[���̐؂�ւ�莞�ɊY���V�[���̃I�u�W�F�N�g�͍폜�����
	virtual int		GetSceneID(){return m_nSceneID;}				
	virtual void	SetSceneID(int nID){m_nSceneID = nID;}

	// �����X�e�[�W�A�X�V���X�g�̃X�e�[�W�����p
	virtual int		GetStageID(){return m_nStageID;}
	virtual void	SetStageID(int nID){m_nStageID = nID;}	

	// �`��X�e�[�W�A�X�V���ƕ`�揇�𕪂��邽�߂�ID
	virtual int		GetDrawID(){return m_nDrawID;}
	virtual void	SetDrawID(int nID){m_nDrawID = nID;}

	// �X�e�[�W���̃v���C�I���e�B���擾
	virtual UINT	GetPriority(){return m_nPriority;}			
	virtual void	SetPriority(UINT nPri){m_nPriority = nPri;}	

	virtual int		GetState(){return m_nStatus;}
	virtual void	SetState(int nState){m_nStatus = nState;}

	// �n���h��
	virtual S_HANDLE	GetHandle(){return m_nHandle;}
	virtual S_HANDLE	GetParent(){return m_nParent;}
	virtual void		SetParent(S_HANDLE handle){m_nParent = handle;}

	// �V�F�[�_�^�C�v
	virtual int		GetShadeType(){return m_nShadeType;}
	virtual void	SetShadeType(int nType){m_nShadeType = nType;}

	// �A�t�B���n
	virtual D3DXVECTOR3	GetPos(){return m_Pos;}						// ���W�擾
	virtual void	SetPos(D3DXVECTOR3 pos){m_Pos = pos;}			// ���W�Z�b�g
	virtual D3DXVECTOR3	GetRot(){return m_Rot;}						// ��]�擾
	virtual void	SetRot(D3DXVECTOR3 rot){m_Rot = rot;}			// ��]�Z�b�g
	virtual D3DXVECTOR3	GetScall(){return m_Scall;}					// �g��擾
	virtual void	SetScall(D3DXVECTOR3 scall){m_Scall = scall;}	// �g��Z�b�g

	// ���x�n
	virtual D3DXVECTOR3	GetSpd(){return m_Spd;}						// ���x�擾
	virtual void	SetSpd(D3DXVECTOR3 Spd){m_Spd = Spd;}			// ���x�Z�b�g
	virtual D3DXVECTOR3	GetAccel(){return m_Accel;}					// �����x�擾
	virtual void	SetAccel(D3DXVECTOR3 Accel){m_Accel = Accel;}	// �����x�Z�b�g

	virtual D3DXMATRIX	GetMatrix(){return m_Matrix;}				// �s��擾
	virtual void	SetMatrix(D3DXMATRIX matrix){m_Matrix = matrix;}// �s��Z�b�g
	
	virtual float	GetRadius(){return m_fRadius;}					// ���a�擾



protected:
	// --�ϐ�--
	static UINT	m_nUniqeID;											// �I�u�W�F�N�g�̃��j�[�NID�n���h��(����𐶐����ɖ߂��āA�n���h���ɂ���)

	UINT	m_nObjID;												// �I�u�W�F�N�gID
	UINT	m_nClassID;												// �N���X������ID

	UINT	m_nRSType;												// �����_�[�X�e�[�g�^�C�v
	UINT	m_nRSTypeLate;											// ��`��p�����_�[�X�e�[�g�^�C�v

	int		m_nShadeType;											// �d�l�V�F�[�_���r�b�g�ł܂Ƃ߂ĉ��H

	int		m_nSceneID;												// �V�[��������ID
	int		m_nStageID;												// �����X�e�[�W������ID
	int		m_nDrawID;												// �����`��X�e�[�W������ID
	
	S_HANDLE m_nHandle;												// ���g�̃n���h��
	S_HANDLE m_nParent;												// �e�̃n���h��


	UINT	m_nPriority;											// �D�揇��
	int		m_nStatus;												// ���

	D3DXVECTOR3	m_Pos;												// �ʒu
	D3DXVECTOR3	m_Rot;												// ��]�p�x
	D3DXVECTOR3	m_Scall;											// �g�嗦

	D3DXVECTOR3	m_Spd;												// ���x
	D3DXVECTOR3	m_Accel;											// �����x

	D3DXMATRIX	m_Matrix;											// ���[���h�}�g���N�X
	float		m_fRadius;											// ���a
	
	bool	m_bUseFlg;												// �g�p����̃t���O
	bool	m_bDrawLate;											// �`��^�C�~���O�A��ɕ`�悷��Ƃ��͂��̃t���O�𗧂Ă�

	bool	m_bDeleteFlg;											// �폜�t���O
};
//=================================================================================
//=================================================================================
//	End of File
//=================================================================================