//プログラムヘッダ*****************************************************************
//	ファイル名		:	Camera.h
//---------------------------------------------------------------------------------
//	プログラム概要	:	カメラ管理ヘッダー
//---------------------------------------------------------------------------------
//	作成者			:	藤岡尚斗	
//---------------------------------------------------------------------------------
//	履歴			:	
//*********************************************************************************
#ifndef __CAMERA_H__

#define __CAMERA_H__

//==include部======================================================================
#include <d3d9.h>
#include <d3dx9.h>

#include "common.h"
//=================================================================================


//==定数部=========================================================================
//=================================================================================


//==列挙型宣言部===================================================================
//  カメラの更新モード
namespace eCameraState
{
	enum 
	{
		FIRSTPERSON = 1,	// 1人称
		THIRDPERSON,		// 3人称
		FIXED,				// 固定

		MONOSPACE,			// 等幅追従、注視点からつねに一定量引き続ける

		MAX_CT
	};

	// 定点
	const D3DXVECTOR3	POS_FIX		= D3DXVECTOR3(0.0f , 200.0f , - 100.0f );
	const D3DXVECTOR3	LOOK_FIX	= D3DXVECTOR3(0.0f , 10.0f , 1000.0f );
	const D3DXVECTOR3	UP_FIX		= D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	// 3人称
	const D3DXVECTOR3	POS_THIRD	= D3DXVECTOR3(0.0f , 80.0f , -200.0f);
	const D3DXVECTOR3	LOOK_THIRD	= D3DXVECTOR3(0.0f , 0.0f , 1000.0f);
	// 1人称
	const D3DXVECTOR3	POS_FIRST	= D3DXVECTOR3(0.0f , 60.0f , 36.0f );
	const D3DXVECTOR3	LOOK_FIRST	= D3DXVECTOR3(0.0f , 30.0f , 1000.0f );

	// デフォルト、カメラ等幅レンジ
	const D3DXVECTOR3	LENGE_MONOSPACE	= D3DXVECTOR3(0.0f , 250.0f , -300.0f );

	const int			MODECHANGE_FRAME = 20;				// 値変更フレーム
};
//=================================================================================


//==マクロ定義部===================================================================
//=================================================================================


//==クラス宣言部===================================================================
class CCamera
{
private:
	// 基準にするオブジェクトのポインタを保持
	S_HANDLE	m_hKeyObj;

	D3DXMATRIX	m_matrixView;											// ビューマトリックス
	
	// 視点
	D3DXVECTOR3	m_vPos;													// カメラ位置
	D3DXVECTOR3	m_vKeyPos;												// 基準カメラ位置
	D3DXVECTOR3	m_vChangePos;											// カメラ位置変更用の変位

	// 注視点
	D3DXVECTOR3	m_vLook;												// カメラ注視点
	D3DXVECTOR3	m_vKeyLook;												// 基準カメラ注視点
	D3DXVECTOR3	m_vChangeLook;											// カメラ注視点変更用の変位

	// アップベクトル
	D3DXVECTOR3	m_vUp;													// カメラアップベクトル
	D3DXVECTOR3	m_vKeyUp;												// 基準カメラアップベクトル

	// 等幅追従レンジ
	D3DXVECTOR3 m_vLenge;												// 等幅追従レンジの幅

	int			m_nMode;												// カメラの視点モード
	int			m_nChangeCnt;											// カメラの視点モードの補正カウンタ

public:

	CCamera();
	~CCamera();

	static CCamera* Create(D3DXVECTOR3 pos , D3DXVECTOR3 Look , 
		D3DXVECTOR3 UpVec , int nCameraMode , S_HANDLE hObj);			// 生成
	bool Initialize(D3DXVECTOR3 pos , D3DXVECTOR3 Look ,
		D3DXVECTOR3 UpVec , int nCameraMode , S_HANDLE hObj);			// 生成時初期化

	void Finalize();

	void Release();
	void Update();
	void Input();
	bool Draw(void);

	
	// --カメラの状態変更--
	bool SetCameraMode(int nMode , S_HANDLE hObj , 
		D3DXVECTOR3 vPos , D3DXVECTOR3 vLook , 
		D3DXVECTOR3 vUp , int nChangeCnt);								// 詳細変更
	
	// ベクトル格納
	bool SetPos(D3DXVECTOR3 Pos){m_vPos = Pos; return true;}
	bool SetLook(D3DXVECTOR3 Look){m_vLook = Look; return true;}
	bool SetUp(D3DXVECTOR3 Vec){m_vUp; return true;}
	// ベクトル取得
	D3DXVECTOR3	GetPos(void){return m_vPos;}
	D3DXVECTOR3	GetLook(void){return m_vLook;}
	D3DXVECTOR3	GetUp(void){return m_vUp;}

	// --ゲット・セット--
	int	GetMode(void){return m_nMode;}
	D3DXMATRIX	GetMatrixView(void){return m_matrixView;}

	void SetMonospaceLenge(D3DXVECTOR3 vLenge){m_vLenge = vLenge;}		// 等幅追従距離保持


private:
};
//=================================================================================


//==外部宣言部=====================================================================
//=================================================================================


#endif