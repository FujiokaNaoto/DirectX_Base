//�v���O�����w�b�_*****************************************************************
//	�v���O�����T�v	:	�V�[���ύX���N���X��`
//*********************************************************************************
#pragma once

//==include��======================================================================
#include "../System/common.h"
#include "..\Scene\Scene.h"

//=================================================================================


//==�萔�E�񋓌^�錾��=============================================================
namespace eChanging
{
	enum
	{
		CHANGE_NONE,					// �����ؑ�

		CHANGE_WHITEFEAD,				// ���t�F�[�h
		CHANGE_BLACKFEAD,				// ���t�F�[�h

		CHANGE_MAX
	};
};

// �v���C�I���e�B

//=================================================================================


//==�}�N����`��===================================================================
//=================================================================================


//==�N���X�錾��===================================================================
class CChanging : public CScene
{
private:
	//-------- �Q�[���p�I�u�W�F�N�g
	bool	m_bChange;								// �`�F���W���t���O
	bool	m_bBefore;								// �O�V�[���ύX���t���O

	int		m_nChangeType;
	CScene*	m_pBefore;								// �O�V�[��
	CScene*	m_pAfter;								// ���V�[��

	S_HANDLE	m_hChangeObj;						// �ύX�������s���I�u�W�F�N�g

public:
	CChanging();
	virtual ~CChanging();

	static CChanging* Create(int nSceneID);			// ����
	virtual void Release();							// ���

	virtual bool Init();							// ���[�v��������
	virtual void Uninit();							// ���[�v���I������

	virtual void Input();							// ���͎�t
	virtual void Update();							// �X�V
	
	virtual void SceneChange();						// �V�[���ύX
	virtual	void ChangeEnd();						// �V�[���`�F���W�I��

	virtual void Draw();							// �`��
	

	// --�Q�b�g�E�Z�b�g--
	virtual	int		GetMaxPriority();

	bool	SetNextScene(CScene* pNext , CScene* pCurrent , int nChangeType);

	bool	GetChangeFlg(){return m_bChange;}		// �`�F���W���t���O�擾
	CScene*	GetAfterScene(){return m_pAfter;}		// �ύX��V�[���擾

private:
};
//=================================================================================


//==�v���g�^�C�v�錾===============================================================
//=================================================================================

//=================================================================================
//	End of File
//=================================================================================