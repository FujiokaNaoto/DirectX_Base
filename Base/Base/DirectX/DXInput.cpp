//**プログラムヘッダ***************************************************************
//	プログラム概要	:	DirectXInputクラス実装
//*********************************************************************************


//==include部======================================================================
#include "DXInput.h"
//=================================================================================


//==定数部=========================================================================
//	なし
//=================================================================================
#define NEUTRAL  65535 / 2// 入力無し
#define ASOBI    20000    // 遊び値
#define MAX_JOYBUTTON 32  // 最大ボタン数 DIJOYSTATE2時は128

//==列挙型定義部===================================================================
//	なし
//=================================================================================


//==マクロ定義部===================================================================
//	なし
//=================================================================================
#define Arrow_LEFT_r  4294940295
#define Arrow_UP_r    4294967295
#define Arrow_RIGHT_r 4294958295
#define Arrow_DOWN_r  4294949295

//==クラス宣言部===================================================================
//	なし
//=================================================================================


//==グローバル変数部===============================================================
int		g_nNumCallback;					// ジョイパッドコールバック用カウンタ
//=================================================================================


//==静的メンバ変数部===============================================================
POINT		CDXInput::m_MousePos;
CHitRect	CDXInput::m_MouseRect = CHitRect(1.0f , 1.0f , D3DXVECTOR3(0.0f , 0.0f , 0.0f) , eHitRect::CENTER);
//=================================================================================


//==プロトタイプ宣言===============================================================
//=================================================================================


//**関数***************************************************************************
//	概要	:	コンストラクタ
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


//関数ヘッダ***********************************************************************	
//	概要	:	デストラクタ
//*********************************************************************************
CDXInput::~CDXInput()
{
	return;
}


//関数ヘッダ***********************************************************************	
//	概要	:	初期処理
//*********************************************************************************
CDXInput* CDXInput::Create(HINSTANCE hInst , HWND hWnd)
{
	//--宣言部--
	CDXInput* pInput;

	pInput = new CDXInput;

	if(pInput)
	{
		// 初期処理、失敗したらデバイス削除
		if(! pInput->Initialize(hInst , hWnd))
			SAFE_DELETE(pInput);
	}

	m_MouseRect.m_fWidth = 1.0f;
	m_MouseRect.m_fHeight = 1.0f;

	return pInput;
}


//関数ヘッダ***********************************************************************	
//	概要	:	初期処理
//*********************************************************************************
bool CDXInput::Initialize(HINSTANCE hInst , HWND hWnd)
{
	//---DirectInputオブジェクトの作成---
	if(FAILED(DirectInput8Create(hInst , DIRECTINPUT_VERSION , IID_IDirectInput8 , (void**)&m_pDInput , NULL)))
	{
		MessageBox(hWnd , _T("DirectInputオブジェクトの生成失敗") , _T("error") , MB_OK); 
		return false;
	}


	//---キーボードの初期化---
	//デバイスオブジェクトを作成
	if(FAILED(m_pDInput->CreateDevice(GUID_SysKeyboard , &m_pDIDevKeyboard , NULL)) || m_pDIDevKeyboard == NULL)
	{
		MessageBox(hWnd , _T("キーボードのデバイスオブジェクトの作成失敗") , _T("error") , MB_OK); 
		return false;
	}
	//データフォーマット設定
	if(FAILED(m_pDIDevKeyboard->SetDataFormat(&c_dfDIKeyboard)))
	{
		MessageBox(hWnd , _T("キーボードのデータフォーマット設定失敗") , _T("error") , MB_OK); 
		return false;
	}

	//協調モードを設定(フォアグラウンド&非排他モード)
	if(FAILED(m_pDIDevKeyboard->SetCooperativeLevel(hWnd , (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		MessageBox(hWnd , _T("キーボード協調モードの設定失敗") , _T("error") , MB_OK); 
		return false;
	}

	//キーボードへのアクセス権を獲得(入力制御開始)
	m_pDIDevKeyboard->Acquire();



	//---マウスの初期化---
	//デバイスオブジェクト作成
	if(FAILED(m_pDInput->CreateDevice(GUID_SysMouse , &m_pDIDevMouse , NULL)) || m_pDIDevMouse == NULL)
	{
		MessageBox(hWnd , _T("マウスのデバイスオブジェクト生成失敗") , _T("error") , MB_OK); 
		return false;
	}

	//データフォーマット設定
	if(FAILED(m_pDIDevMouse->SetDataFormat(&c_dfDIMouse2)))
	{
		MessageBox(hWnd , _T("マウスのデータフォーマット設定失敗") , _T("error") , MB_OK); 
		return false;
	}

	//協調モード設定
	if(FAILED(m_pDIDevMouse->SetCooperativeLevel(hWnd , (DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))))
	{
		MessageBox(hWnd , _T("マウスの強調モード設定失敗") , _T("error") , MB_OK); 
		return false;
	}
	//マウスへのアクセス権を獲得
	m_pDIDevMouse->Acquire();

	//---ゲームパッド---
	int		nLoop;
	for(nLoop = 0 ; nLoop < MAX_JOY ; nLoop ++)
		m_pDIDevJoypad[nLoop] = NULL;

	//デバイスオブジェクトの作成（作成されているジョイパッドを列挙する）
	if(FAILED(m_pDInput->EnumDevices(DI8DEVCLASS_GAMECTRL , EnumJoyCallback , this , DIEDFL_ATTACHEDONLY )))
	{
		MessageBox(hWnd , _T("ゲームパッドのデバイスオブジェクトの作成失敗") , _T("error") , MB_OK); 
		return false;
	}

	// 接続数を取得
	for(m_JoyNum = 0; m_JoyNum < MAX_JOY && *(m_pDIDevJoypad + m_JoyNum) != NULL; m_JoyNum++){/*記述なし*/}

	//ジョイパッドの分だけ処理
	for(nLoop = 0 ; nLoop < MAX_JOY ; nLoop ++)
	{
		//ジョイパッドが無い時はスキップ
		if(m_pDIDevJoypad[nLoop] == NULL)
			continue;

		//データフォーマットの設定
		if(FAILED(m_pDIDevJoypad[nLoop]->SetDataFormat(&c_dfDIJoystick)))
		{
			MessageBox(hWnd , _T("ゲームパッドのデータフォーマットの設定失敗") , _T("error") , MB_OK); 
			return false;
		}

		//協調レベルの設定
		if(FAILED(m_pDIDevJoypad[nLoop]->SetCooperativeLevel(hWnd , DISCL_NONEXCLUSIVE | DISCL_FOREGROUND)))
		{
			MessageBox(hWnd , _T("ゲームパッドの協調レベルの設定失敗") , _T("error") , MB_OK); 
			return false;
		}

		//デバイスへの入力制御開始
		m_pDIDevJoypad[nLoop]->Acquire();


		m_bJoyInit = true;
	}

	return true;
}


bool CDXInput::ResetJoy(HINSTANCE hInst , HWND hWnd)
{
	if(m_bJoyInit == false) // 初期化していない時のみ起動
	{
		if(Initialize(hInst , hWnd)) //フラグ立てて 状態は pollでみる
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


//関数ヘッダ******************************************************************************************
//	概要	:	ジョイスティック問い合わせ用コールバック関数
//****************************************************************************************************
BOOL CALLBACK CDXInput::EnumJoyCallback(const DIDEVICEINSTANCE* lpddi , void* pvRef)
{
	//宣言部
	DIDEVCAPS	diDevCaps;																			//デバイス情報
	CDXInput*	pDI = (CDXInput*)pvRef;

	//ジョイスティック用デバイスオブジェクト作成
	if(FAILED(pDI->m_pDInput->CreateDevice(lpddi->guidInstance , &pDI->m_pDIDevJoypad[g_nNumCallback] , NULL)))
		return DIENUM_CONTINUE;																		//列挙を続ける

	//ジョイスティックの能力を調べる
	diDevCaps.dwSize = sizeof(DIDEVCAPS);
	if(FAILED(pDI->m_pDIDevJoypad[g_nNumCallback]->GetCapabilities(&diDevCaps)))
	{
		if(pDI->m_pDIDevJoypad[g_nNumCallback])
			pDI->m_pDIDevJoypad[g_nNumCallback]->Release();
		pDI->m_pDIDevJoypad[g_nNumCallback] = NULL;
		return DIENUM_CONTINUE;																		//列挙を続ける
	}

	//規定数に達したら終了
	g_nNumCallback ++;
	if(g_nNumCallback >= MAX_JOY)
		return DIENUM_STOP;																			//列挙終了
	else
		return DIENUM_CONTINUE;																		//列挙を続ける
}


//関数ヘッダ***********************************************************************	
//	概要	:	キー情報更新
//*********************************************************************************
void CDXInput::Update(void)
{
	//宣言部
	int	nLoop;
	short nButtonCnt;

	//---キーボード更新---
	BYTE ReadKeyState[256];
	//デバイスからデータ取得。失敗したらキーボードへのアクセス権を取得
	if(FAILED(m_pDIDevKeyboard->GetDeviceState(sizeof(ReadKeyState) , ReadKeyState)))
		m_pDIDevKeyboard->Acquire();

	else
	{
		//トリガー・リリース取得
		for(nLoop = 0 ; nLoop < 256 ; nLoop ++)
		{
			m_KeyTrigger[nLoop] = (ReadKeyState[nLoop] ^ m_KeyState[nLoop]) & ReadKeyState[nLoop];	//トリガー
			m_KeyRelease[nLoop] = (ReadKeyState[nLoop] ^ m_KeyState[nLoop]) & m_KeyState[nLoop];	//リリース
		}


		//押下情報
		memcpy(m_KeyState , ReadKeyState , sizeof(m_KeyState));
	}

	//---マウスの更新---
	DIMOUSESTATE2	ReadmouseState;

	//デバイスからデータ取得。失敗したらマウスのアクセス権を取得
	if(FAILED(m_pDIDevMouse->GetDeviceState(sizeof(ReadmouseState) , &ReadmouseState)))
		m_pDIDevMouse->Acquire();

	else
	{
		//DIMOUSESTATE2にあるボタン8つ分だけトリガーリリース取得
		for(nLoop = 0 ; nLoop < 8 ; nLoop ++)
		{
			//トリガーキー情報抽出	左クリック・右クリック
			m_MouseTrigger.rgbButtons[nLoop] = (m_MouseState.rgbButtons[nLoop] ^ ReadmouseState.rgbButtons[nLoop]) & ReadmouseState.rgbButtons[nLoop];
			//リリースキー情報抽出
			m_MouseRelease.rgbButtons[nLoop] = (m_MouseState.rgbButtons[nLoop] ^ ReadmouseState.rgbButtons[nLoop]) & m_MouseState.rgbButtons[nLoop];
		}

		//マウス通常押下情報保存
		memcpy(&m_MouseState , &ReadmouseState , sizeof(m_MouseState));									//マウス押下情報
	}


	//---ゲームパッドの更新---
	DIJOYSTATE	ReadJoyState[MAX_JOY];

	for(nLoop = 0 ; nLoop < MAX_JOY ; nLoop ++)
	{
		//デバイスがあれば
		if(m_pDIDevJoypad[nLoop])
		{
			//デバイスからデータ取得
			//HRESULT hr = m_pDIDevJoypad[nLoop]->GetDeviceState(sizeof(ReadJoyState[nLoop]) , &ReadJoyState[nLoop]);
			if(FAILED(m_pDIDevJoypad[nLoop]->GetDeviceState(sizeof(ReadJoyState[nLoop]) , &ReadJoyState[nLoop])))
				m_pDIDevJoypad[nLoop]->Acquire();

			else
			{
				D3DXVECTOR2 Read = GetJoyDirKey(nLoop);
				//方向コントローラ トリガー。リリース
				for(nButtonCnt = 0 ; nButtonCnt < 4 ; nButtonCnt ++)
				{
					m_JoyTDirKey[nLoop].x = (float)(((int)Read.x ^ (int)m_JoyDirKeyState[nLoop].x) & (int)Read.x);
					m_JoyTDirKey[nLoop].y = (float)(((int)Read.y ^ (int)m_JoyDirKeyState[nLoop].y) & (int)Read.y);
					m_JoyRDirKey[nLoop].x = (float)(((int)Read.x ^ (int)m_JoyDirKeyState[nLoop].x) & (int)m_JoyDirKeyState[nLoop].x);
					m_JoyRDirKey[nLoop].y = (float)(((int)Read.y ^ (int)m_JoyDirKeyState[nLoop].y) & (int)m_JoyDirKeyState[nLoop].y);
				}

				//ボタン　トリガー・リリース
				for(nButtonCnt = 0 ; nButtonCnt < 32 ; nButtonCnt ++)
				{
					m_JoyTrigger[nLoop].rgbButtons[nButtonCnt] = (ReadJoyState[nLoop].rgbButtons[nButtonCnt] ^ m_JoyState[nLoop].rgbButtons[nButtonCnt]) & ReadJoyState[nLoop].rgbButtons[nButtonCnt];	//トリガー
					m_JoyRelease[nLoop].rgbButtons[nButtonCnt] = (ReadJoyState[nLoop].rgbButtons[nButtonCnt] ^ m_JoyState[nLoop].rgbButtons[nButtonCnt]) & m_JoyState[nLoop].rgbButtons[nButtonCnt];	//リリース				
				}

				//押下情報取得
				m_JoyDirKeyState[nLoop] = Read;			
				memcpy(&m_JoyState[nLoop] , &ReadJoyState[nLoop] , sizeof(m_JoyState[nLoop]));


			}
		}
	}

	return;
}


//関数ヘッダ***********************************************************************	
//	概要	:	終了処理
//*********************************************************************************
void CDXInput::Release(void)
{
	Finalize();
	delete this;
}


//関数ヘッダ***********************************************************************	
//	概要	:	終了処理
//*********************************************************************************
bool CDXInput::Finalize(void)
{
	//キーボードの終了処理
	if(m_pDIDevKeyboard)
		SAFE_RELEASE(m_pDIDevKeyboard);

	//マウスの終了処理
	if(m_pDIDevMouse)
		SAFE_RELEASE(m_pDIDevMouse);

	//ゲームパッドの終了処理
	for(int nNum = 0 ; nNum < MAX_JOY && m_pDIDevJoypad[nNum] ; nNum ++)
	{
		if(m_pDIDevJoypad[nNum]) 
			SAFE_RELEASE(m_pDIDevJoypad[nNum]);
	}

	//オブジェクトの後始末
	SAFE_RELEASE(m_pDInput);
	return true;
}



//関数ヘッダ***********************************************************************	
//	概要	:	ジョイパッドデータ
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

//関数ヘッダ***********************************************************************	
//	概要	:　
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

//関数ヘッダ***********************************************************************	
//	概要	:	ジョイパッドのトリガー
//*********************************************************************************
bool CDXInput::GetJoyTrigger(int nNo,int nCord)
{
	if(nNo > MAX_JOY || nCord > MAX_JOYBUTTON || m_pDIDevJoypad[nNo] == NULL){
		return false;
	}
	return (m_JoyTrigger[nNo].rgbButtons[nCord] != 0);
}

//関数ヘッダ***********************************************************************	
//	概要	:	ジョイパッドのリリース
//*********************************************************************************
bool CDXInput::GetJoyRelease(int nNo, int nCord)
{
	if(nNo > MAX_JOY || nCord > MAX_JOYBUTTON || m_pDIDevJoypad[nNo] == NULL){ // エラー判定
		return false; 
	}
	return (m_JoyTrigger[nNo].rgbButtons[nCord] != 0);
}

//関数ヘッダ***********************************************************************	
//	概要	:	マウスのポイント取得
//*********************************************************************************
CHitRect CDXInput::GetMouseHitRect()
{
	// 今のマウス位置を取得
	m_MouseRect.m_Point.x = (float)m_MousePos.x;
	m_MouseRect.m_Point.y = (float)m_MousePos.y;

	return m_MouseRect;
}
//=================================================================================
//	End of File
//=================================================================================