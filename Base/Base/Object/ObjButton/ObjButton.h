//�v���O�����w�b�_*****************************************************************
//	�v���O�����T�v	:	2D�{�^���N���X��`
//*********************************************************************************
#pragma once

//==include��======================================================================
#include "../Obj2D/Obj2D.h"
//=================================================================================


//==�萔�E�񋓌^�錾��=============================================================
namespace eButtonMoveType
{
	enum
	{
		NONE = 0,
		SIN_MOVE = 1,				// sin�g�㉺�ړ�
		SCALL = 2,					// �g��k��
	};
};

namespace eButtonState
{
	enum
	{
		DEFAULT,					// �ʏ���
		HOVER,						// �J�[�\������ɍڂ��Ă���Ƃ�
		ACTIVE,						// �A�N�e�B�u���
		INACTIVE,					// ��~���
	};
};
//=================================================================================


//==�}�N����`��===================================================================
//=================================================================================


//==�N���X�E�\���̐錾��===========================================================
class CObjButton : public CObj2D
{
public:
	// �R���X�g���N�^�E�f�X�g���N�^
	CObjButton();
	virtual ~CObjButton();

	static CObjButton*	Create(LPCTSTR texName , D3DXVECTOR3 pos , 
		 int nWidth , int nHeight , int nSceneID , int nPriority);	// ����

	static CObjButton*	Create(LPCTSTR texName , D3DXVECTOR3 pos , 
		D3DXVECTOR3 rot , D3DXVECTOR3 scall , int nWidth , 
		int nHeight , int nSceneID , int nPriority);				// ����
	
	
	virtual bool	Init(LPCTSTR texName , D3DXVECTOR3 pos , 
		 int nWidth , int nHeight , int nSceneID , int nPriority);
	virtual bool	Init(LPCTSTR texName , D3DXVECTOR3 pos , 
		D3DXVECTOR3 rot , D3DXVECTOR3 scall , int nWidth , 
		int nHeight , int nSceneID , int nPriority);
	

	virtual	void	Update();										// �X�V
	virtual void	CheckCollision();								// �����蔻�蒲��


	// --Get Set--
	// ���
	virtual void	SetState(int nState);

	// ����
	void	SetMoveType(int nType){m_nMoveType = nType;}			// �����Z�b�g
	void	ChangeMoveType(int nType){m_nMoveType ^= nType;}		// �����ύX
	int		GetMoveType(){return m_nMoveType;}						// �����擾

	// �㉺
	void	SetRadSpd(float fSpd){m_fRadSpd = fSpd;}
	void	SetRadLenge(float fLenge){m_fRadLenge = fLenge;}		// �ړ��̈�Z�b�g
	
	// �g�k
	void	SetScallLenge(float	fLenge){m_fScallLenge = fLenge;}	// �g�k����Z�b�g
	void	SetScallSpd(float fSpd){m_fScallSpd = fSpd;}			// �g�k���x�Z�b�g


protected:
	// --�ϐ�--
	int				m_nMoveType;
	D3DXVECTOR3		m_KeyPos;										// �ړ��̃L�[�ɂȂ���W

	// �㉺�ړ�
	float			m_fRadLenge;									// �{�^���ړ��̈�ϐ�
	float			m_fRadian;										// �{�^���ړ��p�ϐ�
	float			m_fRadSpd;										// �{�^���ړ����x

	// �X�P�[���ړ�
	float			m_fScallLenge;									// �X�P�[���̏��
	float			m_fScallSpd;									// �X�P�[����
	bool			m_bExtendFlg;									// �g��t���O

	bool			m_bTriggerFlg;									// �g���K�[����t���O

	// --�֐�--
	virtual void	OnTrigger();
	virtual void	OnRelease();

	virtual	void	UpdateMove();									// �����ɂ��Ă̍X�V
	virtual void	UpdateState();									// ��Ԃ��Ƃ̍X�V

private:

};
//=================================================================================


//=================================================================================
//	End of File
//=================================================================================