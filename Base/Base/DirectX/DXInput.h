//�v���O�����w�b�_*****************************************************************
//	�v���O�����T�v	:	DirectX Input�N���X�錾
//*********************************************************************************
#ifndef __DXINPUT_H__

#define __DXINPUT_H__


//==include��======================================================================
#include <Windows.h>
#include <tchar.h>

#include <d3d9.h>
#include <d3dx9.h>

#define	DIRECTINPUT_VERSION					(0x0800)							//dinput.h�̌x���h�~
#include <dinput.h>
#pragma comment (lib , "dinput8")

#include "../System/common.h"
#include "../CalcData/HitData.h"
//=================================================================================


//==�萔��=========================================================================
//=================================================================================


//==�񋓌^�錾��===================================================================
//=================================================================================


//==�}�N����`��===================================================================
// �W���C�p�b�h��
enum JOY_NO
{
	JOY_1,
	MAX_JOY
};

namespace eJoyButton //�{�^���v�f�ԍ�
{	
	enum {	
		Button_1,	
		Button_2,
		Button_3,
		Button_4,
		Button_5,
		Button_6,
		Button_7,
		Button_8,
		Button_9,
		Button_10,
		Button_11,  
		Button_12, 
	};
	const D3DXVECTOR2 DIR_UP = D3DXVECTOR2(0,1);
	const D3DXVECTOR2 DIR_DOWN = D3DXVECTOR2(0,-1);
	const D3DXVECTOR2 DIR_RIGHT = D3DXVECTOR2(1,0);
	const D3DXVECTOR2 DIR_LEFT = D3DXVECTOR2(-1,0);

}
//=================================================================================


//==�N���X�錾��===================================================================
class CDXInput
{
public:
	CDXInput();
	~CDXInput();

	static CDXInput* Create(HINSTANCE hInst , HWND hWnd);			// ����
	void	Update(void);											// �X�V
	void	Release();
	bool ResetJoy(HINSTANCE hInst , HWND hWnd); //�W���C�p�b�h�̃��Z�b�g
	//--GetSet--
	// �L�[�f�[�^
	bool	GetKeyState(BYTE code)
	{
		if(m_KeyState[code] & 0x80) return true;
		else return false;
	}
	bool	GetKeyTrigger(BYTE code)
	{
		if(m_KeyTrigger[code] & 0x80) return true;
		else return false;
	}
	bool	GetKeyRelease(BYTE code)
	{
		if(m_KeyRelease[code] & 0x80) return true;
		else return false;
	}

	// �}�E�X
	DIMOUSESTATE2*	GetMouseState(){return &m_MouseState;}
	
	bool			GetMousePress(int nButton)						// �{�^���̃v���X
	{
		if(m_MouseState.rgbButtons[nButton] & 0x80) return true;
		else	return false;
	}

	bool			GetMouseTrigger(int nButton)					// �{�^���̃g���K�[
	{
		if(m_MouseTrigger.rgbButtons[nButton] & 0x80) return true;
		else	return false;
	}

	bool		GetMouseRelease(int nButton)						// �{�^���̃����[�X
	{
		if(m_MouseRelease.rgbButtons[nButton] & 0x80) return true;
		else	return false;
	}

	static POINT&	GetMousePoint(){return m_MousePos;}
	static CHitRect	GetMouseHitRect();
	static void		SetMousePoint(POINT pos){m_MousePos = pos;}

	// �Q�[���p�b�h �g�����ɍ��킹�Ċ֐��኱�C��
	DIJOYSTATE*		GetJoyState(int nNo);
	
	bool			GetJoyTrigger(int nNo,int nButton);
	bool			GetJoyRelease(int nNo,int nButton);		
	inline unsigned short GetJoyNum(void){ return m_JoyNum; }

	D3DXVECTOR2	GetJoyDirKey(unsigned int nNo);
	inline D3DXVECTOR2	CDXInput::GetJoyRDirKey(unsigned int No)
	{
		if(m_JoyNum > MAX_JOY){
			return D3DXVECTOR2(0.0f,0.0f);
		}	
		return m_JoyRDirKey[No];
	}
	inline D3DXVECTOR2	CDXInput::GetJoyTDirKey(unsigned int No)
	{
		if(m_JoyNum > MAX_JOY){
			return D3DXVECTOR2(0.0f,0.0f);
		}
		return m_JoyTDirKey[No];
	}
private:
	static BOOL CALLBACK EnumJoyCallback(const DIDEVICEINSTANCE* lpddi , void* pvRef);
	bool	Initialize(HINSTANCE hInst , HWND hWnd);				// ��������
	bool	Finalize(void);											// ��n��
	// �C���^�t�F�[�X�|�C���^
	LPDIRECTINPUT8	m_pDInput;										//IDirectInput8�C���^�t�F�[�X�ւ̃|�C���^
	LPDIRECTINPUTDEVICE8	m_pDIDevKeyboard;						//IDirectInput8�C���^�t�F�[�X�ւ̃|�C���^(�L�[�{�[�h)
	LPDIRECTINPUTDEVICE8	m_pDIDevMouse;							//IDirectInput8�C���^�t�F�[�X�ւ̃|�C���^(�}�E�X)
	LPDIRECTINPUTDEVICE8	m_pDIDevJoypad[MAX_JOY];				//IDirectInput8�C���^�t�F�[�X�ւ̃|�C���^(�Q�[���p�b�h)

	// �L�[�o�b�t�@
	BYTE	m_KeyState[256];
	BYTE	m_KeyTrigger[256];
	BYTE	m_KeyRelease[256];
	// �}�E�X�o�b�t�@
	static POINT	m_MousePos;
	static CHitRect	m_MouseRect;									// �}�E�X�̂����蔻���`

	DIMOUSESTATE2	m_MouseState;
	
	DIMOUSESTATE2	m_MouseTrigger;
	DIMOUSESTATE2	m_MouseRelease;
	// �Q�[���p�b�h�o�b�t�@
	DIJOYSTATE	m_JoyState[MAX_JOY];
	D3DXVECTOR2 m_JoyDirKeyState[MAX_JOY];
	D3DXVECTOR2 m_JoyTDirKey[MAX_JOY]; //�W���C�X�e�B�b�N�̏��
	D3DXVECTOR2	m_JoyRDirKey[MAX_JOY];

	//D3DXVECTOR2 m_JoyArrowKeyState[MAX_JOY];
	//D3DXVECTOR2 m_JoyTArrowKey[MAX_JOY]; //�W���C�X�e�B�b�N�̏��
	//D3DXVECTOR2	m_JoyRArrowKey[MAX_JOY];


	DIJOYSTATE		m_JoyTrigger[MAX_JOY];
	DIJOYSTATE		m_JoyRelease[MAX_JOY];
	unsigned short	m_JoyNum;
	bool m_bJoyInit;


	
};

//=================================================================================


//==�v���g�^�C�v�錾===============================================================
//=================================================================================

//==�O���錾��=====================================================================
//=================================================================================

#endif
//=================================================================================
//	End of File
//=================================================================================