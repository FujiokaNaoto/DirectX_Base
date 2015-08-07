//�v���O�����w�b�_*****************************************************************
//	�v���O�����T�v	:	�Q�[���Ǘ��N���X��`
//*********************************************************************************
#pragma once

//==include��======================================================================
#include <Windows.h>

#include "../DirectX/Graphics.h"	// draw
#include "../DirectX/DXInput.h"	// input
#include "audiere.h"			// sound
using namespace audiere;

// �V�[��
#include "../Resource/TextureManage.h"
#include "../Resource/MeshManage.h"
#include "../Scene/SceneManage.h"

// �I�u�W�F�N�g
#include "Camera.h"
#include "../CalcData/Calc.h"
#include "ShadeManager.h"
//=================================================================================


//==�萔�E�񋓌^�錾��=============================================================
// �Ǘ�����I�u�W�F�N�g�̃N���X�R�[�h�̊
namespace eClassCode
{
	enum
	{
		SCENE_NONE,

		TEXMANAGE,					// �e�N�X�`���}�l�[�W��
		MESHMANAGE,					// ���b�V���}�l�[�W��
		SCENEMANAGE,				// �V�[���}�l�[�W��

		// �V�[��
		SCENE = SCENEMANAGE + 100,	// �V�[��
		SCENEMAX,


		// 2D�^�C�v�I�u�W�F	
		OBJ_2D = SCENE + 100,		// 2D�I�u�W�F
		OBJ_FEAD,
		OBJ_BUTTON,					// 2D�㉺�ړ��{�^��

		OBJ_2DMAX,


		// 3D�^�C�v�I�u�W�F
		OBJ_3D,						// 3D�I�u�W�F�N�g
		OBJ_3DMAX,					// 3D�I�u�W�F�N�g


		// �r���{�[�h�^�C�v�I�u�W�F
		OBJ_BOARD,					// �r���{�[�h
		OBJ_BOARDMAX,

		MAX
	};
};

// �����_�[�X�e�[�g�^�C�v
namespace eRSType
{
	enum
	{
		NONE,						// �ݒ�Ȃ�

		// 2D
		OBJ_2D,

		// 3D
		OBJ_3D,
		OBJ_3D_LATE,

		// �X�J�C�h�[��
		SKYDOME,

		// �r���{�[�h
		BILLBOARD,
		
		PARTICLE_2D,
		PARTICLE_LATE_2D,

		PARTICLE,
		PARTICLE_LATE,
	};
};
//=================================================================================


//==�N���X�E�\���̐錾��===========================================================
class	CGameManager
{
private:
	static	CGameManager*	m_pInstance;

	HWND		m_hWnd;					
	HINSTANCE	m_hInst;				
	int			m_nFPS;
	static	TCHAR		m_szDebug[4096];

	// --DirectX--
	CGraphics*	m_pGraph;							// �`��֌W
	CDXInput*	m_pInput;							// ���͊Ǘ�

	// --�}�l�[�W��--
	CTextureManage*	m_pTexManage;					// �e�N�X�`�����\�[�X�Ǘ�
	CMeshManage*	m_pMeshManage;					// 3D�I�u�W�F�N�g���\�[�X�Ǘ�
	CSceneManage*	m_pSceneManage;					// �V�[���ؑ֊Ǘ�
	CShadeManager*	m_pShadeManage;					// �V�F�[�_�Ǘ�

	// --�Q�[�����I�u�W�F�N�g--
	CCamera*		m_pCamera;
	AudioDevicePtr	m_pAudioDevice;
	

public:
	// �R���X�g���N�^�E�f�X�g���N�^
	CGameManager();
	virtual ~CGameManager();

	static CGameManager* Create(HWND hWnd , HINSTANCE hInst , bool bWindow);

	void	Update();				// �X�V����
	
	void	Render();				// �`�揈������
	void	Draw();					// �`�揈��
	
	void	Release();				// �������


	void	GameEnd(){PostQuitMessage(0);}						// �Q�[���I���˗�

	// �f�o�b�O�p
	void	SetFPS(int nFPS) {m_nFPS = nFPS;}
	void	AddDebugStr(LPCTSTR psz) {lstrcat(m_szDebug, psz);}
	void	MsgBox(LPCTSTR psz);


	LRESULT	OnWndProc(UINT uMsg, WPARAM wParam, LPARAM lParam);	// �v���V�[�W����������

	// --�Q�b�g�Z�b�g--
	static	CGameManager&	GetInstance(){return *m_pInstance;}	// ���̎擾


	HWND			GetHWND(){return m_hWnd;}					// �E�B���h�E�n���h��
	HINSTANCE		GetHINST(){return m_hInst;}					// �C���X�^���X�n���h��
	CGraphics*		GetGraph(){return m_pGraph;}				// �O���t�B�b�N�f�o�C�X�擾
	CDXInput*		GetInput(){return m_pInput;}				// ���͏���������

	// �}�l�[�W��
	CTextureManage*	GetTexManage(){return m_pTexManage;}		// �e�N�X�`���}�l�[�W���擾
	CMeshManage*	GetMeshManage(){return m_pMeshManage;}		// ���b�V���}�l�[�W���擾
	CSceneManage*	GetSceneManage(){return m_pSceneManage;}	// �V�[���}�l�[�W���擾
	CShadeManager*	GetShadeManage(){return m_pShadeManage;}	// �V�F�[�h�}�l�[�W���擾

	CCamera*		GetCamera(){return m_pCamera;}				// �J�����擾

	AudioDevicePtr*	GetAudioDevice(){return &m_pAudioDevice;}
};


//==�}�N����`��===================================================================
#define MANAGER		CGameManager::GetInstance()
//=================================================================================


//=================================================================================


//=================================================================================
//	End of File
//=================================================================================