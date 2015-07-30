//�v���O�����w�b�_*****************************************************************
//	�v���O�����T�v	:	�V�[���Ǘ��N���X
//*********************************************************************************
#pragma once

//==include��======================================================================
#include "../System/common.h"

#include <list>

#include "..\Scene\Scene.h"
#include "Changing.h"
//=================================================================================


//==�萔�E�񋓌^�錾��=============================================================
namespace eScene_ID
{
	enum ESCENE_ID
	{
		NONE,					// �����V�[���Ȃ�

		// ����V�[��
		CHANGE,
		PAUSE,					// �|�[�Y�J�n

		// �V�[��
		TITLE,					// �^�C�g��
		MAIN,					// �Q�[���{��

		CLEAR,
		OVER,

		RESLOAD,

		MAX
	};
};
//=================================================================================


//==�}�N����`��===================================================================
#define SCENE_LIST			std::list<CScene*>							// �V�[�����X�g
#define SCENE_LIST_ITE		std::list<CScene*>::iterator				// �V�[�����X�g�ւ̃C�e���[�^
//=================================================================================


//==�N���X�E�\���̐錾��===========================================================
class CSceneManage
{
private:
	static	SCENE_LIST		m_SceneList;								// ���[�h���ꂽ�e�N�X�`����ێ����郊�X�g

	bool	m_bChangeFlg;												// �V�[���ύX���t���O

	CChanging*	m_pChanging;											// �ύX���V�[��
	CScene*	m_pCurrentScene;											// ���̃V�[���̃|�C���^
	int		m_nNextScene;												// ���̃V�[���R�[�h

public:
	// �R���X�g���N�^�E�f�X�g���N�^
	CSceneManage();
	virtual	~CSceneManage();

	static	CSceneManage*	Create();									// ����
	bool	Init();														// ������
	void	Release();													// �}�l�[�W�����

	void	Input();													// ���͎�t
	void	Update();													// �X�V
	void	Draw();														// �`��

	void	Changeing();												// �V�[���ύX������

	CScene*	Search(int nSceneID);										// ���łɃV�[�����i�[����Ă��邩������
	CScene*	Entry(int nSceneID);										// �V�[�������X�g���ɓo�^
	CScene*	SceneCreate(int nSceneCode);								// �e�V�[������

	void	ListClear();												// �i�[���X�g�����S�N���A
	bool	ClearOnce(int nSceneID);									// �w��V�[���̉��

	// --�Q�b�g�Z�b�g--
	bool	SetNextScene(int nSceneID , int nChangeType);				// �V�[���ύX����
	bool	GetChangeFlg(){return m_bChangeFlg;}						// �V�[���ύX���t���O�擾

	CScene&	GetCurrentScene(){return *m_pCurrentScene;}					// ���݂̃V�[���擾
};
//=================================================================================


//=================================================================================
//	End of File
//=================================================================================