//**�v���O�����w�b�_***************************************************************
//	�v���O�����T�v	:	�V�[���Ǘ��N���X����
//*********************************************************************************


//==include��======================================================================
#include "../System/GameManager.h"
#include "../Window/GameWnd.h"

#include <tchar.h>

#include "../System/common.h"
#include "SceneManage.h"

// --�e�V�[��--
#include "Title.h"
#include "GameMain.h"
#include "GameClear.h"
#include "GameOver.h"
#include "ResLoadScene.h"
//=================================================================================


//==�萔�E�񋓌^��=================================================================
//const int	START_SCENE = eScene_ID::RESLOAD;
const int	START_SCENE = eScene_ID::TITLE;
//=================================================================================


//==�}�N����`��===================================================================

//=================================================================================


//==�N���X�E�\���̐錾��===========================================================

//=================================================================================


//==�ÓI�����o�ϐ���===============================================================
SCENE_LIST		CSceneManage::m_SceneList;
//=================================================================================


//==�v���g�^�C�v�錾===============================================================

//=================================================================================


//**�֐�***************************************************************************
//	�T�v	:	�R���X�g���N�^
//*********************************************************************************
CSceneManage::CSceneManage():
m_pCurrentScene(NULL),
m_nNextScene(START_SCENE),
m_bChangeFlg(false),
m_pChanging(NULL)
{

}


//**�֐�***************************************************************************
//	�T�v	:	�f�X�g���N�^
//*********************************************************************************
CSceneManage::~CSceneManage()
{
}


//**�֐�***************************************************************************
//	�T�v	:	����
//*********************************************************************************
CSceneManage* CSceneManage::Create()
{
	CSceneManage*	pManage = new CSceneManage;

	if(! pManage->Init())
	{
		::MessageBox(NULL , _T("SceneManage�����G���[") , _T("error") , MB_ICONERROR | MB_OK);
		return NULL;
	}

	return pManage;
}


//**�֐�***************************************************************************
//	�T�v	:	������
//*********************************************************************************
bool CSceneManage::Init()
{
	m_pCurrentScene = Entry(START_SCENE);							// �ŏ��̃V�[������
	if(!m_pCurrentScene)
		return false;												// �������s

	m_pCurrentScene->Init();										// �ŏ��̃V�[��������

	return true;
}


//**�֐�***************************************************************************
//	�T�v	:	�V�[���ؑ�
//			:	���������s�̃t���O��Ԃ�
//*********************************************************************************
bool CSceneManage::SetNextScene(int nSceneCode , int nChangeType)
{
	// �J�ڒ��͎󂯕t���Ȃ�
	if(m_bChangeFlg)
		return false;

	// �V�[���ύX�N���X�N��
	m_pChanging = (CChanging*)Entry(eScene_ID::CHANGE);				// �ύX�p�V�[�����Ăяo��
	m_pChanging->SetNextScene(Entry(nSceneCode) , m_pCurrentScene , nChangeType);
	m_pChanging->Init();

	m_pCurrentScene = m_pChanging;

	m_bChangeFlg = true;
	m_nNextScene = nSceneCode;
	
	return true;
}


//**�֐�***************************************************************************
//	�T�v	:	�V�[���ؑ֒��̏���
//*********************************************************************************
void CSceneManage::Changeing()
{
	// �`�F���W�I���m�F
	if(! m_pChanging->GetChangeFlg())
	{
		m_bChangeFlg = false;
		m_pChanging->Uninit();

		m_pCurrentScene = m_pChanging->GetAfterScene();			// �X�V�Ώۂ̃V�[���������ւ�
	}
}


//**�֐�***************************************************************************
//	�T�v	:	���͏���
//*********************************************************************************
void CSceneManage::Input()
{
	// �V�[���ؑ֏������͓��͂��󂯕t���Ȃ�
	if(m_bChangeFlg)
		return; 

	// �V�[���X�V
	if(m_pCurrentScene)
		m_pCurrentScene->Input();
}


//**�֐�***************************************************************************
//	�T�v	:	�X�V
//*********************************************************************************
void CSceneManage::Update()
{
	// �V�[���ؑ֏���
	if(m_bChangeFlg)
		Changeing();

	// �V�[���X�V
	if(m_pCurrentScene)
		m_pCurrentScene->Update();
}


//**�֐�***************************************************************************
//	�T�v	:	�`��
//*********************************************************************************
void CSceneManage::Draw()
{
	// �V�[���`��
	if(m_pCurrentScene)
		m_pCurrentScene->Draw();
}


//**�֐�***************************************************************************
//	�T�v	:	���X�g������
//			:	�w�肵�����O�̃V�[���ւ̃|�C���^��Ԃ�
//*********************************************************************************
CScene* CSceneManage::Search(int nSceneID)
{
	SCENE_LIST_ITE	begin = m_SceneList.begin();
	SCENE_LIST_ITE	end = m_SceneList.end();

	while(begin != end)
	{
		// ��v���Ȃ��ԃ��X�g�𓮂���
		if((*begin)->GetSceneID() != nSceneID)
			begin ++;
			
		// ��v�����炻�̃V�[���ւ̃|�C���^��Ԃ�
		return (*begin);
	}

	// ������Ȃ������̂�NULL��Ԃ�
	return NULL;
}

//**�֐�***************************************************************************
//	�T�v	:	�z��֓o�^
//			:	�w�肵���V�[���ւ̃|�C���^��Ԃ�
//			:	���[�h���s�����Ƃ���NULL��Ԃ�
//*********************************************************************************
CScene* CSceneManage::Entry(int nSceneCode)
{
	CScene*	pScene = NULL;
	
	// ���łɎw�肵���t�@�C�������邩������
	pScene = Search(nSceneCode);

	// ����΂����Ԃ�
	if(pScene)
		return pScene;


	// �Ȃ������ꍇ�͐V�K�Ő���
	if(! ( pScene = SceneCreate(nSceneCode) ))
	{
		// ���s�����Ƃ���NULL��Ԃ�
		::MessageBox(NULL , _T("�V�[�������G���[") , _T("error") , MB_ICONERROR | MB_OK);
		return NULL;
	}

	return pScene;
}


//**�֐�***************************************************************************
//	�T�v	:	�e�V�[������
//*********************************************************************************
CScene* CSceneManage::SceneCreate(int nSceneCode)
{
	CScene*	pScene = NULL;

	switch(nSceneCode)
	{
	// �`�F���W���V�[��
	case eScene_ID::CHANGE:
		pScene = CChanging::Create(nSceneCode);
		break;

	// �^�C�g���V�[��
	case eScene_ID::TITLE:
		pScene = CTitle::Create(nSceneCode);
		break;

	// ���C���V�[��
	case eScene_ID::MAIN:
		pScene = CGameMain::Create(nSceneCode);
		break;

	// �N���A�V�[��
	case eScene_ID::CLEAR:
		pScene = CGameClear::Create(nSceneCode);
		break;

	// �I�[�o�[�V�[��
	case eScene_ID::OVER:
		pScene = CGameOver::Create(nSceneCode);
		break;

	// ���\�[�X���[�h�V�[��
	case eScene_ID::RESLOAD:
		pScene = CResLoadScene::Create(nSceneCode);
		break;

	// �Y�����Ȃ��ꍇNULL��Ԃ�
	default:
		return NULL;
		break;
	}

	return pScene;
}


//**�֐�***************************************************************************
//	�T�v	:	�z������S�N���A
//*********************************************************************************
void CSceneManage::ListClear()
{
	SCENE_LIST_ITE begin = m_SceneList.begin();
	SCENE_LIST_ITE end = m_SceneList.end();

	// ���X�g�S���
	while(begin != end)
	{	
		(*begin)->Release();
		begin = m_SceneList.erase(begin);
	}

	m_SceneList.clear();
}


//**�֐�***************************************************************************
//	�T�v	:	�}�l�[�W�����
//*********************************************************************************
void CSceneManage::Release()
{
	ListClear();

	delete this;
}


//**�֐�***************************************************************************
//	�T�v	:	�ʉ��(�t�@�C�����w��)
//			:	������s����false��Ԃ�
//*********************************************************************************
bool CSceneManage::ClearOnce(int nScene)
{
	SCENE_LIST_ITE begin = m_SceneList.begin();
	SCENE_LIST_ITE end = m_SceneList.end();

	while(begin != end)
	{
		if((*begin)->GetSceneID() != nScene)
			continue;
		
		// ��v������폜
		(*begin)->Release();
		m_SceneList.erase(begin);

		return true;
	}

	return false;
}

//=================================================================================
//	End of File
//=================================================================================