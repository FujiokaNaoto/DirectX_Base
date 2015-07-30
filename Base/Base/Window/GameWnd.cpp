//**�v���O�����w�b�_***************************************************************
//	�v���O�����T�v	:	�Q�[���p�E�C���h�E�N���X��`
//*********************************************************************************

//==include��======================================================================
#include "../Window/GameWnd.h"
#include "../System/common.h"
#include <stdlib.h>
#include <time.h>
#include "../System/GameManager.h"
//=================================================================================


//==�ÓI�����o�ϐ���===============================================================
//=================================================================================


//**�֐�***************************************************************************
//	�T�v	:	�E�B���h�E������
//*********************************************************************************
bool CGameWindow::InitInstance()
{
	if (!CWindow::InitInstance()) return false;

	m_bWindow = true;
	/*if (IDYES == MessageBox(_T("�E�B���h�E���[�h�Ŏ��s���܂����H"), _T("��ʃ��[�h"), MB_YESNO))
		m_bWindow = true;*/

	
	srand((unsigned) time(NULL));						// �����V�[�h�ݒ�

	::timeBeginPeriod(1);								// �^�C�}�̕���\���ŏ��ɃZ�b�g

	// �}�l�[�W���[������
	m_pGameManager = CGameManager::Create(m_hWnd , m_hInstance , m_bWindow);
	if(! m_pGameManager) return false;


	m_dwFPSLastTime = ::timeGetTime();
	m_dwExecLastTime = m_dwFPSLastTime - FRAME_RATE;	// �K���ŏ��� m_pScene->Update() �����s
	m_dwFrameCount = 0;

	return true;
}

//**�֐�***************************************************************************
//	�T�v	:	�E�B���h�E���
//*********************************************************************************
int CGameWindow::ExitInstance()
{
	// �}�l�[�W���[���
	SAFE_RELEASE(m_pGameManager);	
	
	::timeEndPeriod(1);				// �^�C�}�̕���\�����ɖ߂�

	return CWindow::ExitInstance();	// ���s�t�@�C���̖߂�l
}


//**�֐�***************************************************************************
//	�T�v	:	�A�C�h��������
//*********************************************************************************
bool CGameWindow::OnIdle(long lCount)
{
	// �t���[�����J�E���g
	DWORD dwCurrentTime = ::timeGetTime();				// ���݂̃^�C�}�l���擾
	if (dwCurrentTime - m_dwFPSLastTime >= 500) 
	{	// 0.5 �b���ƂɌv��
		// �t���[�������v�Z
		if (m_pGameManager) 
		{
			m_pGameManager->SetFPS(m_dwFrameCount * 1000 / (dwCurrentTime - m_dwFPSLastTime));
		}
		m_dwFPSLastTime = dwCurrentTime;				// �^�C�}�l���X�V
		m_dwFrameCount = 0;								// �t���[���J�E���^�����Z�b�g
	}


	// ���ԊǗ�
	while (dwCurrentTime - m_dwExecLastTime >= FRAME_RATE) 
	{	// ��莞�Ԃ��o�߂�����c
		m_dwExecLastTime += FRAME_RATE;					// �^�C�}�l���X�V
			
		if (m_pGameManager) m_pGameManager->Update();	// �Q�[�� ���C������
	}


	// �`��
	if (m_pGameManager) 
	{
		m_pGameManager->Render();						// �����_�����O����
	}

	m_dwFrameCount++;									// �t���[���J�E���g�{�P
	return true;
}


//**�֐�***************************************************************************
//	�T�v	:	�E�B���h�E�v���V�[�W����M������
//*********************************************************************************
LRESULT CGameWindow::WndProc(UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	int nMessage;

	switch (uMsg)
	{
	// �L�[����
	case WM_KEYDOWN:
		switch(wParam)
		{
		//[Esc]���͎��̓Q�[���I���m�F
		case VK_ESCAPE:
			nMessage = MessageBox(_T("�Q�[�����I�����܂����H") ,
									_T("�I���m�F") , MB_YESNO);	//�I���m�F

			if(nMessage == IDYES)
				PostQuitMessage(0);								//�I���w�����o��
			break;

		// ���̑��̓���
		default:
			// �Q�[���}�l�[�W���[�Ɉϑ�
			m_pGameManager->OnWndProc(uMsg , wParam , lParam);
			break;
		}
		return 0;

	// �~�{�^���N���b�N
	case WM_CLOSE:
		nMessage = MessageBox(_T("�Q�[�����I�����܂����H") , 
								_T("�I���m�F") , MB_YESNO);		//�I���m�F

		if(nMessage == IDYES)
			PostQuitMessage(0);
		return 0;

	// �E�B���h�E��������
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		// �Q�[���}�l�[�W���[�Ɉϑ�
		m_pGameManager->OnWndProc(uMsg , wParam , lParam);
		break;
	}
	return DefWindowProc(uMsg, wParam, lParam);
}
//=======================================================================================
//	End of File
//=======================================================================================