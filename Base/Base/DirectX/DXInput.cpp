//**�v���O�����w�b�_***************************************************************
//	�v���O�����T�v	:	DirectXInput�N���X����
//*********************************************************************************


//==include��======================================================================
#include "DXInput.h"
//=================================================================================


//==�萔��=========================================================================
//	�Ȃ�
//=================================================================================
#define NEUTRAL  65535 / 2// ���͖���
#define ASOBI    20000    // �V�ђl
#define MAX_JOYBUTTON 32  // �ő�{�^���� DIJOYSTATE2����128

//==�񋓌^��`��===================================================================
//	�Ȃ�
//=================================================================================


//==�}�N����`��===================================================================
//	�Ȃ�
//=================================================================================
#define Arrow_LEFT_r  4294940295
#define Arrow_UP_r    4294967295
#define Arrow_RIGHT_r 4294958295
#define Arrow_DOWN_r  4294949295

//==�N���X�錾��===================================================================
//	�Ȃ�
//=================================================================================


//==�O���[�o���ϐ���===============================================================
int		g_nNumCallback;					// �W���C�p�b�h�R�[���o�b�N�p�J�E���^
//=================================================================================


//==�ÓI�����o�ϐ���===============================================================
POINT		CDXInput::m_MousePos;
CHitRect	CDXInput::m_MouseRect = CHitRect(1.0f , 1.0f , D3DXVECTOR3(0.0f , 0.0f , 0.0f) , eHitRect::CENTER);
//=================================================================================


//==�v���g�^�C�v�錾===============================================================
//=================================================================================


//**�֐�***************************************************************************
//	�T�v	:	�R���X�g���N�^
//*********************************************************************************
CDXInput::CDXInput():
m_pDInput(NULL),
	m_pDIDevKeyboard(NULL),
	m_pDIDevMouse(NULL)
{	
	for(int nNum = 0 ; nNum < MAX_JOY ; nNum ++)
		m_pDIDevJoypad[nNum] = NULL;

	m_MousePos.x = 0;
	m_MousePos.y = 0;
	m_bJoyInit = false;
	return;
}


//�֐��w�b�_***********************************************************************	
//	�T�v	:	�f�X�g���N�^
//*********************************************************************************
CDXInput::~CDXInput()
{
	return;
}


//�֐��w�b�_***********************************************************************	
//	�T�v	:	��������
//*********************************************************************************
CDXInput* CDXInput::Create(HINSTANCE hInst , HWND hWnd)
{
	//--�錾��--
	CDXInput* pInput;

	pInput = new CDXInput;

	if(pInput)
	{
		// ���������A���s������f�o�C�X�폜
		if(! pInput->Initialize(hInst , hWnd))
			SAFE_DELETE(pInput);
	}

	m_MouseRect.m_fWidth = 1.0f;
	m_MouseRect.m_fHeight = 1.0f;

	return pInput;
}


//�֐��w�b�_***********************************************************************	
//	�T�v	:	��������
//*********************************************************************************
bool CDXInput::Initialize(HINSTANCE hInst , HWND hWnd)
{
	//---DirectInput�I�u�W�F�N�g�̍쐬---
	if(FAILED(DirectInput8Create(hInst , DIRECTINPUT_VERSION , IID_IDirectInput8 , (void**)&m_pDInput , NULL)))
	{
		MessageBox(hWnd , _T("DirectInput�I�u�W�F�N�g�̐������s") , _T("error") , MB_OK); 
		return false;
	}


	//---�L�[�{�[�h�̏�����---
	//�f�o�C�X�I�u�W�F�N�g���쐬
	if(FAILED(m_pDInput->CreateDevice(GUID_SysKeyboard , &m_pDIDevKeyboard , NULL)) || m_pDIDevKeyboard == NULL)
	{
		MessageBox(hWnd , _T("�L�[�{�[�h�̃f�o�C�X�I�u�W�F�N�g�̍쐬���s") , _T("error") , MB_OK); 
		return false;
	}
	//�f�[�^�t�H�[�}�b�g�ݒ�
	if(FAILED(m_pDIDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		MessageBox(hWnd , _T("�L�[�{�[�h�̃f�[�^�t�H�[�}�b�g�ݒ莸�s") , _T("error") , MB_OK); 
		return false;
	}

	//�������[�h��ݒ�(�t�H�A�O���E���h&��r�����[�h)
	if(FAILED(m_pDIDevKeyboard->SetCooperativeLevel(hWnd , (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		MessageBox(hWnd , _T("�L�[�{�[�h�������[�h�̐ݒ莸�s") , _T("error") , MB_OK); 
		return false;
	}

	//�L�[�{�[�h�ւ̃A�N�Z�X�����l��(���͐���J�n)
	m_pDIDevKeyboard->Acquire();



	//---�}�E�X�̏�����---
	//�f�o�C�X�I�u�W�F�N�g�쐬
	if(FAILED(m_pDInput->CreateDevice(GUID_SysMouse , &m_pDIDevMouse , NULL)) || m_pDIDevMouse == NULL)
	{
		MessageBox(hWnd , _T("�}�E�X�̃f�o�C�X�I�u�W�F�N�g�������s") , _T("error") , MB_OK); 
		return false;
	}

	//�f�[�^�t�H�[�}�b�g�ݒ�
	if(FAILED(m_pDIDevMouse->SetDataFormat(&c_dfDIMouse2)))
	{
		MessageBox(hWnd , _T("�}�E�X�̃f�[�^�t�H�[�}�b�g�ݒ莸�s") , _T("error") , MB_OK); 
		return false;
	}

	//�������[�h�ݒ�
	if(FAILED(m_pDIDevMouse->SetCooperativeLevel(hWnd , (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		MessageBox(hWnd , _T("�}�E�X�̋������[�h�ݒ莸�s") , _T("error") , MB_OK); 
		return false;
	}
	//�}�E�X�ւ̃A�N�Z�X�����l��
	m_pDIDevMouse->Acquire();

	//---�Q�[���p�b�h---
	int		nLoop;
	for(nLoop = 0 ; nLoop < MAX_JOY ; nLoop ++)
		m_pDIDevJoypad[nLoop] = NULL;

	//�f�o�C�X�I�u�W�F�N�g�̍쐬�i�쐬����Ă���W���C�p�b�h��񋓂���j
	if(FAILED(m_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL , EnumJoyCallback , this , DIEDFL_ATTACHEDONLY )))
	{
		MessageBox(hWnd , _T("�Q�[���p�b�h�̃f�o�C�X�I�u�W�F�N�g�̍쐬���s") , _T("error") , MB_OK); 
		return false;
	}

	// �ڑ������擾
	for(m_JoyNum = 0; m_JoyNum < MAX_JOY && *(m_pDIDevJoypad + m_JoyNum) != NULL; m_JoyNum++){/*�L�q�Ȃ�*/}

	//�W���C�p�b�h�̕���������
	for(nLoop = 0 ; nLoop < MAX_JOY ; nLoop ++)
	{
		//�W���C�p�b�h���������̓X�L�b�v
		if(m_pDIDevJoypad[nLoop] == NULL)
			continue;

		//�f�[�^�t�H�[�}�b�g�̐ݒ�
		if(FAILED(m_pDIDevJoypad[nLoop]->SetDataFormat(&c_dfDIJoystick)))
		{
			MessageBox(hWnd , _T("�Q�[���p�b�h�̃f�[�^�t�H�[�}�b�g�̐ݒ莸�s") , _T("error") , MB_OK); 
			return false;
		}

		//�������x���̐ݒ�
		if(FAILED(m_pDIDevJoypad[nLoop]->SetCooperativeLevel(hWnd , DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
		{
			MessageBox(hWnd , _T("�Q�[���p�b�h�̋������x���̐ݒ莸�s") , _T("error") , MB_OK); 
			return false;
		}

		//�f�o�C�X�ւ̓��͐���J�n
		m_pDIDevJoypad[nLoop]->Acquire();


		m_bJoyInit = true;
	}

	return true;
}


bool CDXInput::ResetJoy(HINSTANCE hInst , HWND hWnd)
{
	if(m_bJoyInit == false) // ���������Ă��Ȃ����̂݋N��
	{
		if(Initialize(hInst , hWnd)) //�t���O���Ă� ��Ԃ� poll�ł݂�
		{
			m_bJoyInit = true;
			return true;			
		}else{
			return false;
		}
	}else
	{
		return true;
	}	

}


//�֐��w�b�_******************************************************************************************
//	�T�v	:	�W���C�X�e�B�b�N�₢���킹�p�R�[���o�b�N�֐�
//****************************************************************************************************
BOOL CALLBACK CDXInput::EnumJoyCallback(const DIDEVICEINSTANCE* lpddi , void* pvRef)
{
	//�錾��
	DIDEVCAPS	diDevCaps;																			//�f�o�C�X���
	CDXInput*	pDI = (CDXInput*)pvRef;

	//�W���C�X�e�B�b�N�p�f�o�C�X�I�u�W�F�N�g�쐬
	if(FAILED(pDI->m_pDInput->CreateDevice(lpddi->guidInstance , &pDI->m_pDIDevJoypad[g_nNumCallback] , NULL)))
		return DIENUM_CONTINUE;																		//�񋓂𑱂���

	//�W���C�X�e�B�b�N�̔\�͂𒲂ׂ�
	diDevCaps.dwSize = sizeof(DIDEVCAPS);
	if(FAILED(pDI->m_pDIDevJoypad[g_nNumCallback]->GetCapabilities(&diDevCaps)))
	{
		if(pDI->m_pDIDevJoypad[g_nNumCallback])
			pDI->m_pDIDevJoypad[g_nNumCallback]->Release();
		pDI->m_pDIDevJoypad[g_nNumCallback] = NULL;
		return DIENUM_CONTINUE;																		//�񋓂𑱂���
	}

	//�K�萔�ɒB������I��
	g_nNumCallback ++;
	if(g_nNumCallback >= MAX_JOY)
		return DIENUM_STOP;																			//�񋓏I��
	else
		return DIENUM_CONTINUE;																		//�񋓂𑱂���
}


//�֐��w�b�_***********************************************************************	
//	�T�v	:	�L�[���X�V
//*********************************************************************************
void CDXInput::Update(void)
{
	//�錾��
	int	nLoop;
	short nButtonCnt;

	//---�L�[�{�[�h�X�V---
	BYTE ReadKeyState[256];
	//�f�o�C�X����f�[�^�擾�B���s������L�[�{�[�h�ւ̃A�N�Z�X�����擾
	if(FAILED(m_pDIDevKeyboard->GetDeviceState(sizeof(ReadKeyState) , ReadKeyState)))
		m_pDIDevKeyboard->Acquire();

	else
	{
		//�g���K�[�E�����[�X�擾
		for(nLoop = 0 ; nLoop < 256 ; nLoop ++)
		{
			m_KeyTrigger[nLoop] = (ReadKeyState[nLoop] ^ m_KeyState[nLoop]) & ReadKeyState[nLoop];	//�g���K�[
			m_KeyRelease[nLoop] = (ReadKeyState[nLoop] ^ m_KeyState[nLoop]) & m_KeyState[nLoop];	//�����[�X
		}


		//�������
		memcpy(m_KeyState , ReadKeyState , sizeof(m_KeyState));
	}

	//---�}�E�X�̍X�V---
	DIMOUSESTATE2	ReadmouseState;

	//�f�o�C�X����f�[�^�擾�B���s������}�E�X�̃A�N�Z�X�����擾
	if(FAILED(m_pDIDevMouse->GetDeviceState(sizeof(ReadmouseState) , &ReadmouseState)))
		m_pDIDevMouse->Acquire();

	else
	{
		//DIMOUSESTATE2�ɂ���{�^��8�������g���K�[�����[�X�擾
		for(nLoop = 0 ; nLoop < 8 ; nLoop ++)
		{
			//�g���K�[�L�[��񒊏o	���N���b�N�E�E�N���b�N
			m_MouseTrigger.rgbButtons[nLoop] = (m_MouseState.rgbButtons[nLoop] ^ ReadmouseState.rgbButtons[nLoop]) & ReadmouseState.rgbButtons[nLoop];
			//�����[�X�L�[��񒊏o
			m_MouseRelease.rgbButtons[nLoop] = (m_MouseState.rgbButtons[nLoop] ^ ReadmouseState.rgbButtons[nLoop]) & m_MouseState.rgbButtons[nLoop];
		}

		//�}�E�X�ʏ퉟�����ۑ�
		memcpy(&m_MouseState , &ReadmouseState , sizeof(m_MouseState));									//�}�E�X�������
	}


	//---�Q�[���p�b�h�̍X�V---
	DIJOYSTATE	ReadJoyState[MAX_JOY];

	for(nLoop = 0 ; nLoop < MAX_JOY ; nLoop ++)
	{
		//�f�o�C�X�������
		if(m_pDIDevJoypad[nLoop])
		{
			//�f�o�C�X����f�[�^�擾
			//HRESULT hr = m_pDIDevJoypad[nLoop]->GetDeviceState(sizeof(ReadJoyState[nLoop]) , &ReadJoyState[nLoop]);
			if(FAILED(m_pDIDevJoypad[nLoop]->GetDeviceState(sizeof(ReadJoyState[nLoop]) , &ReadJoyState[nLoop])))
				m_pDIDevJoypad[nLoop]->Acquire();

			else
			{
				D3DXVECTOR2 Read = GetJoyDirKey(nLoop);
				//�����R���g���[�� �g���K�[�B�����[�X
				for(nButtonCnt = 0 ; nButtonCnt < 4 ; nButtonCnt ++)
				{
					m_JoyTDirKey[nLoop].x = (float)(((int)Read.x ^ (int)m_JoyDirKeyState[nLoop].x) & (int)Read.x);
					m_JoyTDirKey[nLoop].y = (float)(((int)Read.y ^ (int)m_JoyDirKeyState[nLoop].y) & (int)Read.y);
					m_JoyRDirKey[nLoop].x = (float)(((int)Read.x ^ (int)m_JoyDirKeyState[nLoop].x) & (int)m_JoyDirKeyState[nLoop].x);
					m_JoyRDirKey[nLoop].y = (float)(((int)Read.y ^ (int)m_JoyDirKeyState[nLoop].y) & (int)m_JoyDirKeyState[nLoop].y);
				}

				//�{�^���@�g���K�[�E�����[�X
				for(nButtonCnt = 0 ; nButtonCnt < 32 ; nButtonCnt ++)
				{
					m_JoyTrigger[nLoop].rgbButtons[nButtonCnt] = (ReadJoyState[nLoop].rgbButtons[nButtonCnt] ^ m_JoyState[nLoop].rgbButtons[nButtonCnt]) & ReadJoyState[nLoop].rgbButtons[nButtonCnt];	//�g���K�[
					m_JoyRelease[nLoop].rgbButtons[nButtonCnt] = (ReadJoyState[nLoop].rgbButtons[nButtonCnt] ^ m_JoyState[nLoop].rgbButtons[nButtonCnt]) & m_JoyState[nLoop].rgbButtons[nButtonCnt];	//�����[�X				
				}

				//�������擾
				m_JoyDirKeyState[nLoop] = Read;			
				memcpy(&m_JoyState[nLoop] , &ReadJoyState[nLoop] , sizeof(m_JoyState[nLoop]));


			}
		}
	}

	return;
}


//�֐��w�b�_***********************************************************************	
//	�T�v	:	�I������
//*********************************************************************************
void CDXInput::Release(void)
{
	Finalize();
	delete this;
}


//�֐��w�b�_***********************************************************************	
//	�T�v	:	�I������
//*********************************************************************************
bool CDXInput::Finalize(void)
{
	//�L�[�{�[�h�̏I������
	if(m_pDIDevKeyboard)
		SAFE_RELEASE(m_pDIDevKeyboard);

	//�}�E�X�̏I������
	if(m_pDIDevMouse)
		SAFE_RELEASE(m_pDIDevMouse);

	//�Q�[���p�b�h�̏I������
	for(int nNum = 0 ; nNum < MAX_JOY && m_pDIDevJoypad[nNum] ; nNum ++)
	{
		if(m_pDIDevJoypad[nNum]) 
			SAFE_RELEASE(m_pDIDevJoypad[nNum]);
	}

	//�I�u�W�F�N�g�̌�n��
	SAFE_RELEASE(m_pDInput);
	return true;
}



//�֐��w�b�_***********************************************************************	
//	�T�v	:	�W���C�p�b�h�f�[�^
//*********************************************************************************
DIJOYSTATE*	CDXInput::GetJoyState(int nNo)
{
	switch(nNo)
	{
	case JOY_1:
		return &m_JoyState[nNo];
		break;

	default:
		return NULL;
		break;
	}
}

//�֐��w�b�_***********************************************************************	
//	�T�v	:�@
//*********************************************************************************
D3DXVECTOR2	CDXInput::GetJoyDirKey(unsigned int nNo)
{
	D3DXVECTOR2 Input = D3DXVECTOR2(0,0);
	int MAX = NEUTRAL + ASOBI;
	int Min = NEUTRAL - ASOBI;

	if(m_JoyNum > MAX_JOY){
		return Input;
	}

	if((m_JoyState[nNo].lX) > MAX){
		Input.x = 1;
	}else if((m_JoyState[nNo].lX) < Min ){
			Input.x = -1;
		}if((m_JoyState[nNo].lY) > MAX){
			Input.y = -1;
		}else if( (m_JoyState[nNo].lY) < Min ){
			Input.y = 1;
		}	
			return Input;

}

//�֐��w�b�_***********************************************************************	
//	�T�v	:	�W���C�p�b�h�̃g���K�[
//*********************************************************************************
bool CDXInput::GetJoyTrigger(int nNo,int nCord)
{
	if(nNo > MAX_JOY || nCord > MAX_JOYBUTTON || m_pDIDevJoypad[nNo] == NULL){
		return false;
	}
	return (m_JoyTrigger[nNo].rgbButtons[nCord] != 0);
}

//�֐��w�b�_***********************************************************************	
//	�T�v	:	�W���C�p�b�h�̃����[�X
//*********************************************************************************
bool CDXInput::GetJoyRelease(int nNo, int nCord)
{
	if(nNo > MAX_JOY || nCord > MAX_JOYBUTTON || m_pDIDevJoypad[nNo] == NULL){ // �G���[����
		return false; 
	}
	return (m_JoyTrigger[nNo].rgbButtons[nCord] != 0);
}

//�֐��w�b�_***********************************************************************	
//	�T�v	:	�}�E�X�̃|�C���g�擾
//*********************************************************************************
CHitRect CDXInput::GetMouseHitRect()
{
	// ���̃}�E�X�ʒu���擾
	m_MouseRect.m_Point.x = (float)m_MousePos.x;
	m_MouseRect.m_Point.y = (float)m_MousePos.y;

	return m_MouseRect;
}
//=================================================================================
//	End of File
//=================================================================================