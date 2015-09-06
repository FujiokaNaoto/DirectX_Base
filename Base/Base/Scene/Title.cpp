//**�v���O�����w�b�_***************************************************************
//	�v���O�����T�v	:	�^�C�g���N���X����
//*********************************************************************************


//==include��======================================================================
#include "Title.h"
#include "../System/GameManager.h"
#include "../Window/GameWnd.h"

#include "../System/Register.h"
#include "../Object/ObjButton/ObjButton.h"

#include "../Object/Obj3D/Skydome.h"

//=================================================================================


//==�萔�E�񋓌^��`��=============================================================
namespace eTitleConst
{
	const LPCTSTR	BG = _T("../resource_data/img/Bg.png");
	const LPCTSTR	MESH = _T("../resource_data/mesh/nanase_pose.x");
	const LPCTSTR	SKY = _T("../resource_data/mesh/sky.x");
};
//=================================================================================


//==�}�N����`��===================================================================
//	�Ȃ�
//=================================================================================


//==�N���X�錾��===================================================================
//	�Ȃ�
//=================================================================================


//==�O���[�o���ϐ���===============================================================
//=================================================================================


//==�ÓI�����o�ϐ���===============================================================
//=================================================================================


//==�v���g�^�C�v�錾===============================================================
//=================================================================================


//**�֐�***************************************************************************
//	�T�v	:	�R���X�g���N�^
//*********************************************************************************
CTitle::CTitle(void):
m_pBgm(NULL),
m_pKey(NULL),
m_bChangeFlg(false)
{
}


//**�֐�***************************************************************************
//	�T�v	:	�f�X�g���N�^
//*********************************************************************************
CTitle::~CTitle(void)
{
}


//**�֐�***************************************************************************
//	�T�v	:	����
//*********************************************************************************
CTitle* CTitle::Create(int nSceneID)
{
	CTitle* pTitle = new CTitle();	
	if(!pTitle)
	{
		return NULL;
	}
	
	pTitle->m_nSceneID = nSceneID;								// �V�[��ID�Z�b�g
	return pTitle;
}


//**�֐�***************************************************************************
//	�T�v	:	���
//*********************************************************************************
void CTitle::Release()
{
	Uninit();
	delete this;
}


//�֐��w�b�_***********************************************************************	
//	�T�v	:	�������A�Q�[�����[�v��
//*********************************************************************************
bool CTitle::Init(void)
{
	CObj2D*	pBuf = NULL;

	// SE���[�h
	/*
	m_pKey = OpenSoundEffect(MANAGER.GetAudioDevice()->get() , eTitleConst::KEY_SE_NAME , MULTIPLE);
	m_pCursor = OpenSoundEffect(MANAGER.GetAudioDevice()->get() , eTitleConst::SEL_SE_NAME , MULTIPLE);

	// BGM���[�h
	m_pBgm = OpenSound(MANAGER.GetAudioDevice()->get() , eTitleConst::BGM_NAME , true);
	m_pBgm->setRepeat(true);
	m_pBgm->play();					// �Đ�
	*/


	D3DXVECTOR3 pos;/* = D3DXVECTOR3(SCREEN_WIDTH / 2.0f , SCREEN_HEIGHT / 2.0f , 0.0f);
	pBuf = CObj2D::Create(eTitleConst::BG , pos , SCREEN_WIDTH , SCREEN_HEIGHT , m_nSceneID , 0);
	REGISTER->Entry(pBuf , eOBJSTAGE::BG);
	*/



	pos = D3DXVECTOR3(0.0f , 198.0f , -92.0f);
	CObj3D* pObj3D = CObj3D::Create(eTitleConst::MESH , pos , m_nSceneID , 0);
	REGISTER->Entry(pObj3D , eOBJSTAGE::OBJ);
	m_hObj = pObj3D->GetHandle();

	
	pObj3D = CSkydome::Create(eTitleConst::SKY , m_nSceneID , 0);
	REGISTER->Entry(pObj3D , eOBJSTAGE::BG);
	

	return true;
}


//�֐��w�b�_***********************************************************************	
//	�T�v	:	�I�������A�Q�[�����[�v��
//*********************************************************************************
void CTitle::Uninit(void)
{
	//m_pBgm->stop();

	// �^�C�g�������̃I�u�W�F�̂݉��
	REGISTER->ReleaseScene(eScene_ID::TITLE);
}


//**�֐�***************************************************************************
//	�T�v	:	���͎�t
//*********************************************************************************
void CTitle::Input()
{
	static int nType;

	CDXInput* pInput = MANAGER.GetInput();	
	// Enter�L�[�g���K�[�Ŏ��V�[����
	if(pInput->GetKeyTrigger(DIK_RETURN))
	{
		//m_pKey->play();
		// ���̃{�^���ɉ������V�[���Ɉڍs
		MANAGER.GetSceneManage()->SetNextScene(eScene_ID::MAIN , eChanging::CHANGE_BLACKFEAD);
	}

	CObj3D* pObj = REGISTER_H_P(m_hObj , CObj3D*);
	D3DXVECTOR3 pos = pObj->GetPos();
	if(pInput->GetKeyState(DIK_UP)) pos.y += 0.2f;
	if(pInput->GetKeyState(DIK_DOWN)) pos.y -= 0.2f;
	if(pInput->GetKeyState(DIK_RIGHT)) pos.x += 0.2f;
	if(pInput->GetKeyState(DIK_LEFT)) pos.x -= 0.2f;
	pObj->SetPos(pos);

	if(pInput->GetKeyTrigger(DIK_SPACE))
	{
		nType == 0 ? nType = eShadeType::PHONG : nType *= 2;
		nType > eShadeType::TOON ? nType = 0 : false;
		pObj->SetShadeType(nType);
	}
}


//**�֐�***************************************************************************
//	�T�v	:	�X�V
//*********************************************************************************
void CTitle::Update()
{
}


//**�֐�***************************************************************************
//	�T�v	:	�`��
//*********************************************************************************
void CTitle::Draw()
{

#ifdef _DEBUG
	TCHAR	str[256];						// �f�o�b�N�p�o�b�t�@
	// �f�o�b�N����
	_stprintf_s(str , _T("�^�C�g���N���X\n"));
	MANAGER.AddDebugStr(str);
#endif
}


//**�֐�***************************************************************************
//	�T�v	:	�v���C�I���e�B�̍ő吔�擾
//*********************************************************************************
int CTitle::GetMaxPriority()
{
	return eTitlePri::MAX;
}

//=================================================================================
//	End of File
//=================================================================================