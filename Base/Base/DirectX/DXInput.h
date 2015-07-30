//プログラムヘッダ*****************************************************************
//	プログラム概要	:	DirectX Inputクラス宣言
//*********************************************************************************
#ifndef __DXINPUT_H__

#define __DXINPUT_H__


//==include部======================================================================
#include <Windows.h>
#include <tchar.h>

#include <d3d9.h>
#include <d3dx9.h>

#define	DIRECTINPUT_VERSION					(0x0800)							//dinput.hの警告防止
#include <dinput.h>
#pragma comment (lib , "dinput8")

#include "../System/common.h"
#include "../CalcData/HitData.h"
//=================================================================================


//==定数部=========================================================================
//=================================================================================


//==列挙型宣言部===================================================================
//=================================================================================


//==マクロ定義部===================================================================
// ジョイパッド数
enum JOY_NO
{
	JOY_1,
	MAX_JOY
};

namespace eJoyButton //ボタン要素番号
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


//==クラス宣言部===================================================================
class CDXInput
{
public:
	CDXInput();
	~CDXInput();

	static CDXInput* Create(HINSTANCE hInst , HWND hWnd);			// 生成
	void	Update(void);											// 更新
	void	Release();
	bool ResetJoy(HINSTANCE hInst , HWND hWnd); //ジョイパッドのリセット
	//--GetSet--
	// キーデータ
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

	// マウス
	DIMOUSESTATE2*	GetMouseState(){return &m_MouseState;}
	
	bool			GetMousePress(int nButton)						// ボタンのプレス
	{
		if(m_MouseState.rgbButtons[nButton] & 0x80) return true;
		else	return false;
	}

	bool			GetMouseTrigger(int nButton)					// ボタンのトリガー
	{
		if(m_MouseTrigger.rgbButtons[nButton] & 0x80) return true;
		else	return false;
	}

	bool		GetMouseRelease(int nButton)						// ボタンのリリース
	{
		if(m_MouseRelease.rgbButtons[nButton] & 0x80) return true;
		else	return false;
	}

	static POINT&	GetMousePoint(){return m_MousePos;}
	static CHitRect	GetMouseHitRect();
	static void		SetMousePoint(POINT pos){m_MousePos = pos;}

	// ゲームパッド 使う数に合わせて関数若干修正
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
	bool	Initialize(HINSTANCE hInst , HWND hWnd);				// 初期処理
	bool	Finalize(void);											// 後始末
	// インタフェースポインタ
	LPDIRECTINPUT8	m_pDInput;										//IDirectInput8インタフェースへのポインタ
	LPDIRECTINPUTDEVICE8	m_pDIDevKeyboard;						//IDirectInput8インタフェースへのポインタ(キーボード)
	LPDIRECTINPUTDEVICE8	m_pDIDevMouse;							//IDirectInput8インタフェースへのポインタ(マウス)
	LPDIRECTINPUTDEVICE8	m_pDIDevJoypad[MAX_JOY];				//IDirectInput8インタフェースへのポインタ(ゲームパッド)

	// キーバッファ
	BYTE	m_KeyState[256];
	BYTE	m_KeyTrigger[256];
	BYTE	m_KeyRelease[256];
	// マウスバッファ
	static POINT	m_MousePos;
	static CHitRect	m_MouseRect;									// マウスのあたり判定矩形

	DIMOUSESTATE2	m_MouseState;
	
	DIMOUSESTATE2	m_MouseTrigger;
	DIMOUSESTATE2	m_MouseRelease;
	// ゲームパッドバッファ
	DIJOYSTATE	m_JoyState[MAX_JOY];
	D3DXVECTOR2 m_JoyDirKeyState[MAX_JOY];
	D3DXVECTOR2 m_JoyTDirKey[MAX_JOY]; //ジョイスティックの状態
	D3DXVECTOR2	m_JoyRDirKey[MAX_JOY];

	//D3DXVECTOR2 m_JoyArrowKeyState[MAX_JOY];
	//D3DXVECTOR2 m_JoyTArrowKey[MAX_JOY]; //ジョイスティックの状態
	//D3DXVECTOR2	m_JoyRArrowKey[MAX_JOY];


	DIJOYSTATE		m_JoyTrigger[MAX_JOY];
	DIJOYSTATE		m_JoyRelease[MAX_JOY];
	unsigned short	m_JoyNum;
	bool m_bJoyInit;


	
};

//=================================================================================


//==プロトタイプ宣言===============================================================
//=================================================================================

//==外部宣言部=====================================================================
//=================================================================================

#endif
//=================================================================================
//	End of File
//=================================================================================