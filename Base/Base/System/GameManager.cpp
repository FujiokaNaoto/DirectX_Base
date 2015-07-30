//**�v���O�����w�b�_***************************************************************
//	�v���O�����T�v	:	�Q�[���Ǘ��N���X����
//*********************************************************************************


//==include��======================================================================
#include "GameManager.h"
#include "../Window/GameWnd.h"
#include "common.h"
#include "Register.h"
//=================================================================================


//==�萔�E�񋓌^��=================================================================

//=================================================================================


//==�}�N����`��===================================================================

//=================================================================================


//==�N���X�E�\���̐錾��===========================================================

//=================================================================================


//==�ÓI�����o�ϐ���===============================================================
TCHAR			CGameManager::m_szDebug[];
CGameManager*	CGameManager::m_pInstance = NULL;
//=================================================================================


//**�֐�***************************************************************************
//	�T�v	:	�R���X�g���N�^
//*********************************************************************************
CGameManager::CGameManager():
	m_pGraph(NULL),
	m_pInput(NULL),
	m_pTexManage(NULL),
	m_pMeshManage(NULL),
	m_pSceneManage(NULL),
	m_pCamera(NULL),
	
	m_hWnd(NULL) , 
	m_hInst(NULL)
{
	m_szDebug[0] = _T('\0');	// �f�o�b�O�����񏉊���
}


//**�֐�***************************************************************************
//	�T�v	:	�f�X�g���N�^
//*********************************************************************************
CGameManager::~CGameManager()
{
}


//**�֐�***************************************************************************
//	�T�v	:	�}�l�[�W���[����
//*********************************************************************************
CGameManager* CGameManager::Create(HWND hWnd , HINSTANCE hInst , bool bWindow)
{
	// ���̂�����Ƃ��͂����Ԃ�
	if(m_pInstance)
		return m_pInstance;

	m_pInstance = new CGameManager;


	// --�������̏�����--
	// �E�B���h�E�̏���o�^���Ă���
	m_pInstance->m_hWnd = hWnd;
	m_pInstance->m_hInst = hInst;

	// �O���t�B�b�N�N���X����
	if(! m_pInstance->m_pGraph) m_pInstance->m_pGraph = CGraphics::Create(m_pInstance->m_hWnd , 
		SCREEN_WIDTH , SCREEN_HEIGHT , bWindow);
	if(!m_pInstance->m_pGraph)
	{
		SAFE_RELEASE(m_pInstance);
		return NULL;												// �������s�̎��̓v���O�����I��
	}

	// ���̓N���X����
	if(! m_pInstance->m_pInput) m_pInstance->m_pInput = CDXInput::Create(m_pInstance->m_hInst , 
		m_pInstance->m_hWnd);
	if(!m_pInstance->m_pInput)
	{
		SAFE_RELEASE(m_pInstance);
		return NULL;												// �������s�̎��̓v���O�����I��
	}

	// --�}�l�[�W������--
	// �e�N�X�`��
	if(!m_pInstance->m_pTexManage) m_pInstance->m_pTexManage = CTextureManage::Create();
	if(!m_pInstance->m_pTexManage)
	{
		SAFE_RELEASE(m_pInstance);
		return NULL;												// �������s�̎��̓v���O�����I��
	}

	// ���b�V��
	if(!m_pInstance->m_pMeshManage) m_pInstance->m_pMeshManage = CMeshManage::Create();
	if(!m_pInstance->m_pMeshManage) 
	{
		SAFE_RELEASE(m_pInstance);
		return NULL;												// �������s�̎��̓v���O�����I��
	}

	// �J����
	if(!m_pInstance->m_pCamera) m_pInstance->m_pCamera = CCamera::Create(eCameraState::POS_FIX ,
		eCameraState::LOOK_FIX , eCameraState::UP_FIX , 
		eCameraState::FIXED , NULL);								// �J��������
	if(!m_pInstance->m_pCamera)	
	{
		SAFE_RELEASE(m_pInstance);
		return NULL;												// �������s�̎��̓v���O�����I��
	}

	// Audio Device
	if(!m_pInstance->m_pAudioDevice) m_pInstance->m_pAudioDevice = OpenDevice();
	if(!m_pInstance->m_pAudioDevice) 
	{
		SAFE_RELEASE(m_pInstance);
		return NULL;												// �������s�̎��̓v���O�����I��
	}

	// �V�[��
	if(!m_pInstance->m_pSceneManage) m_pInstance->m_pSceneManage = CSceneManage::Create();
	if(!m_pInstance->m_pSceneManage) 
	{
		SAFE_RELEASE(m_pInstance);
		return NULL;												// �������s�̎��̓v���O�����I��
	}


	return m_pInstance;
}


//**�֐�***************************************************************************
//	�T�v	:	�X�V
//*********************************************************************************
void CGameManager::Update()
{
	// �폜�ΏۃI�u�W�F�N�g���폜
	REGISTER->ReleaseDeleteFlg();

	// ���͍X�V
	if(m_pInput)m_pInput->Update();						// DirectX�̓��͏����X�V
	m_pSceneManage->Input();							// �V�[�����Ƃ̓��͏������Ă�

	m_pCamera->Update();

	REGISTER->UpdateAll();								// ���X�g�����S�I�u�W�F�N�g���X�V
	m_pSceneManage->Update();							// �V�[���X�V

	REGISTER->AffineAll();								// ���X�g�����S�I�u�W�F�N�g���A�t�B���ϊ�
	REGISTER->CollisionAll();							// ���X�g�����I�u�W�F�N�g�̑S�����蔻��`�F�b�N

}
	

//**�֐�***************************************************************************
//	�T�v	:	�`�揀��
//*********************************************************************************
void CGameManager::Render()
{
	if (m_pGraph->Begin()) 
	{	
		Draw();						// �`�揈��

		m_pGraph->End();			// �`��I��
	}

	m_pGraph->SwapBuffer();			// �o�b�N�o�b�t�@����

	m_szDebug[0] = _T('\0');		// �f�o�b�O�����񏉊���
}


//**�֐�***************************************************************************
//	�T�v	:	�`��
//*********************************************************************************
void CGameManager::Draw()
{
	// �`��J�n
	m_pCamera->Draw();									// �J�����`��

	m_pSceneManage->Draw();								// �V�[���`�揈��
	
	REGISTER->DrawAll();								// �I�u�W�F�N�g�`��

#ifdef _DEBUG
	// �f�o�b�N�e�L�X�g
	TCHAR	str[256];

	static int t = 1;
	static int cnt = 1;

	if(m_nFPS > 0)
	{
		t += m_nFPS;
		cnt ++;
	}

	_stprintf_s(str, _T("FPS = %d\n"), (int)(t/cnt));
	AddDebugStr(str);
	_stprintf_s(str, _T("CameraPos{%.1f , %.1f , %.1f}\n") , m_pCamera->GetPos().x , m_pCamera->GetPos().y , m_pCamera->GetPos().z);
	AddDebugStr(str);
	_stprintf_s(str, _T("CameraLook{%.1f , %.1f , %.1f}\n") , m_pCamera->GetLook().x , m_pCamera->GetLook().y , m_pCamera->GetLook().z);
	AddDebugStr(str);

	// �f�o�b�O������`��
	m_pGraph->DrawText(0, 0, m_szDebug , FONT_DEBUG , 255 , 255 , 255 , 255);
#endif
}


//**�֐�***************************************************************************
//	�T�v	:	�������
//*********************************************************************************
void CGameManager::Release()
{
	// �e�I�u�W�F�N�g���
	if(m_pGraph) SAFE_RELEASE(m_pGraph);				// �O���t�B�b�N�N���X���
	if(m_pInput) SAFE_RELEASE(m_pInput);				// ���̓N���X���
		
	// �}�l�[�W��
	if(m_pTexManage) SAFE_RELEASE(m_pTexManage);		// �e�N�X�`���}�l�[�W�����
	if(m_pMeshManage) SAFE_RELEASE(m_pMeshManage);		// ���b�V���}�l�[�W�����
	if(m_pSceneManage) SAFE_RELEASE(m_pSceneManage);	// �V�[���}�l�[�W�����

	if(m_pCamera)	SAFE_RELEASE(m_pCamera);			// �J�������


	REGISTER->Cleanup();								// �I�u�W�F�Ǘ����X�g�����

	SAFE_DELETE(m_pInstance);							// �������폜
}


//**�֐�***************************************************************************
//	�T�v	:	���b�Z�[�W�{�b�N�X�\��
//*********************************************************************************
void CGameManager::MsgBox(LPCTSTR psz)
{
	MessageBox(m_hWnd , psz , _T("message") , MB_OK);
}


//**�֐�***************************************************************************
//	�T�v	:	�E�B���h�E�v���V�[�W�����̓���
//*********************************************************************************
LRESULT CGameManager::OnWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	//int nMessage;

	switch (uMsg)
	{
	// �L�[����
	case WM_KEYDOWN:
		//switch(wParam)
		//{

		//// ���̑��̓���
		//default:
		//	break;
		//}
		return 0;

	case WM_COMMAND:
		break;

	case WM_DEVICECHANGE:
		m_pInput->ResetJoy(m_hInst, m_hWnd);
		break;


	// �}�E�X�ړ�
	case WM_MOUSEMOVE:
		//�}�E�X���W
		if(m_pInstance && m_pInput)
		{
			POINT	Point;
			Point.x = LOWORD(lParam);
			Point.y = HIWORD(lParam);
			m_pInput->SetMousePoint(Point);									// �}�E�X���W�X�V
		}
		return 0;


	case WM_MOUSEWHEEL:
		return 0;

	default:
		break;
	}

	return 0;
}
//=================================================================================
//	End of File
//=================================================================================