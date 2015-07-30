//�v���O�����w�b�_*****************************************************************
//	�v���O�����T�v	:	�t�F�[�h�I�u�W�F
//*********************************************************************************
#pragma once

//==include��======================================================================
#include "Obj2D.h"
//=================================================================================


//==�萔�E�񋓌^�錾��=============================================================
namespace eFeadObjConst
{
	enum 
	{
		STOP,
		FEAD_IN,
		FEAD_MIDDLE,
		FEAD_MIDDLE_STOP,
		FEAD_OUT,
	};

	enum
	{
		NONE,
		WHITEFEAD,
		BLACKFEAD,

	};
}
//=================================================================================


//==�}�N����`��===================================================================

//=================================================================================


//==�N���X�E�\���̐錾��===========================================================
// �t�F�[�h����������I�u�W�F
class CFeadObj : public CObj2D
{
private:

protected:
	int		m_nInTime , m_nMiddleTime , m_nOutTime;

	// �t�F�[�h���ԂɂȂ������Ɉ�x�~�߂邩�ǂ����̃t���O
	bool	m_bMiddleStop;
	int		m_nFeadType;							// �t�F�[�h�^�C�v�ݒ�
	int		m_nFeadCnt;

public:
	CFeadObj();
	virtual ~CFeadObj();

	static CFeadObj*	Create(D3DXVECTOR3 pos , int nWidth , int nHeight ,
		int nSceneID , int nPriority , bool bMiddleStop);

	virtual bool	Init(D3DXVECTOR3 pos , int nWidth , int nHeight ,
		int nSceneID , int nPriority , bool bMiddleStop);

	virtual void	Update();

	virtual bool	FStart(int nType);
	virtual bool	FStartMiddle(int nType);
	virtual bool	MiddleStart();					// ���ԂŎ~�߂Ă��鎞�ɁA�������n�߂郁�b�Z�[�W

	virtual void	SetMiddleStopFlg(bool bFlg){m_bMiddleStop = bFlg;}
	virtual bool	SetFeadTime(int nIn , int nMiddle , int nOut);
};
//=================================================================================


//=================================================================================
//	End of File
//=================================================================================