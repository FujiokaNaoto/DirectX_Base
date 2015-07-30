//�v���O�����w�b�_*****************************************************************
//	�v���O�����T�v	:	���\�[�X���[�h
//*********************************************************************************
#pragma once

//==include��======================================================================
#include "../System/common.h"
#include "Scene.h"
//=================================================================================


//==�萔�E�񋓌^�錾��=============================================================
// �v���C�I���e�B
namespace eResLoadScenePri
{
	enum 
	{	
		NONE,
		BG,

		OBJ,

		FEAD,

		MAX
	};
};
//=================================================================================


//==�}�N����`��===================================================================

//=================================================================================


//==�N���X�E�\���̐錾��===========================================================
class CResLoadScene : public CScene
{
protected:
	//-------- �Q�[���p�I�u�W�F�N�g
	int			m_nWaitCnt;
	S_HANDLE	m_hFead;
	HANDLE		m_hThread;							// �X���b�h�̃n���h��

	static unsigned int WINAPI ResLoading(void* arg);		// ���\�[�X���[�h�֐�
	bool		m_bChangeFlg;

public:
	CResLoadScene();
	virtual ~CResLoadScene();

	static CResLoadScene* Create(int nSceneID);		// ����
	
	virtual bool Init();							// ���[�v��������
	virtual void Uninit();							// ���[�v���I������
	
	virtual void Update();							// �X�V
	

	// --�Q�b�g�E�Z�b�g--
	virtual	int		GetMaxPriority();

private:

};
//=================================================================================


//=================================================================================
//	End of File
//=================================================================================