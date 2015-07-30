//**�v���O�����w�b�_***************************************************************
//	�v���O�����T�v	:	
//*********************************************************************************


//==include��======================================================================
#include "TimeAnalizer.h"
#include "../../System/GameManager.h"
//=================================================================================


//==�萔�E�񋓌^��=================================================================

//=================================================================================


//==�}�N����`��===================================================================

//=================================================================================


//==�N���X�E�\���̐錾��===========================================================

//=================================================================================


//==�ÓI�����o�ϐ���===============================================================

//=================================================================================


//==�v���g�^�C�v�錾===============================================================

//=================================================================================


//**�֐�***************************************************************************
//	�T�v	:	�R���X�g���N�^
//*********************************************************************************
CTimeAnalizer::CTimeAnalizer()
{
	_tcscpy_s(m_szMsg , _T("\0"));
	m_nSTime.HighPart = 0;
	m_nSTime.LowPart = 0;
	m_nETime.HighPart = 0;
	m_nETime.LowPart = 0;
}


//**�֐�***************************************************************************
//	�T�v	:	�f�X�g���N�^
//*********************************************************************************
CTimeAnalizer::~CTimeAnalizer()
{
}


//**�֐�***************************************************************************
//	�T�v	:	����
//*********************************************************************************
CTimeAnalizer* CTimeAnalizer::Create(TCHAR* szStr , int nSceneID)
{
	CTimeAnalizer* pObj = new CTimeAnalizer();

	// �Z�b�g
	if(pObj->Init(szStr , nSceneID))
		return pObj;

	SAFE_RELEASE(pObj);
	return NULL;
}


//**�֐�***************************************************************************
//	�T�v	:	
//*********************************************************************************
bool CTimeAnalizer::Init(TCHAR* szStr , int nSceneID)
{
	_tcscpy_s(m_szMsg , szStr);
	m_nSTime.HighPart = 0;
	m_nSTime.LowPart = 0;
	m_nETime.HighPart = 0;
	m_nETime.LowPart = 0;

	m_nSceneID = nSceneID;

	m_bUseFlg = true;

	// �p�t�H�[�}���X�J�E���^�̎��g���擾
	LARGE_INTEGER freq;
	QueryPerformanceFrequency(&freq);
	if(freq.QuadPart == 0)
	{
		MessageBox(MANAGER.GetHWND() , _T("�p�t�H�[�}���X�J�E���^�͎g�p�ł��܂���B\n") , _T("error") , MB_OK);
		m_bUseFlg = false;

		return false;
	}

	return true;
}


//**�֐�***************************************************************************
//	�T�v	:	�`��
//*********************************************************************************
void CTimeAnalizer::Draw()
{
	if(!m_bUseFlg)
		return;

	TCHAR	Str[768];

	_stprintf_s(Str , _T("%s : %8d\n") , m_szMsg , (int)(m_nETime.QuadPart - m_nSTime.QuadPart));
	MANAGER.AddDebugStr(Str);
}


//**�֐�***************************************************************************
//	�T�v	:	��`��(�����Ȃ�)
//*********************************************************************************
void CTimeAnalizer::DrawLate()
{
}


//**�֐�***************************************************************************
//	�T�v	:	�v���J�n�_�w��
//*********************************************************************************
void CTimeAnalizer::StartPoint()
{
	if(!m_bUseFlg)
		return;

	QueryPerformanceCounter(&m_nSTime);
}


//**�֐�***************************************************************************
//	�T�v	:	�v���I���_�w��
//*********************************************************************************
void CTimeAnalizer::EndPoint()
{
	if(!m_bUseFlg)
		return;

	QueryPerformanceCounter(&m_nETime);
}


//**�֐�***************************************************************************
//	�T�v	:	�v����
//*********************************************************************************
void CTimeAnalizer::Activate(bool bFlg)
{
	m_bUseFlg = bFlg;
}


//**�֐�***************************************************************************
//	�T�v	:	���b�Z�[�W�Z�b�g
//*********************************************************************************
void CTimeAnalizer::SetMsg(TCHAR* str)
{
	_tcscpy_s(m_szMsg , str);
}
//=================================================================================
//	End of File
//=================================================================================