//**プログラムヘッダ***************************************************************
//	プログラム概要	:	カメラ管理
//*********************************************************************************


//==include部======================================================================
#include "GameManager.h"
#include "Register.h"
#include "Camera.h"
//=================================================================================


//==定数部=========================================================================
//=================================================================================


//==列挙型定義部===================================================================
//=================================================================================


//==マクロ定義部===================================================================
//	なし
//=================================================================================


//==クラス宣言部===================================================================
//	なし
//=================================================================================


//==静的メンバ変数部===============================================================
//	なし
//=================================================================================


//==プロトタイプ宣言===============================================================
//=================================================================================


//**関数***************************************************************************
//	概要	:	コンストラクタ
//*********************************************************************************
CCamera::CCamera(void):
m_hKeyObj		(0),
m_vPos			(D3DXVECTOR3(0.0f , 0.0f , 0.0f)),
m_vKeyPos		(D3DXVECTOR3(0.0f , 0.0f , 0.0f)),	
m_vChangePos	(D3DXVECTOR3(0.0f , 0.0f , 0.0f)),

m_vLook			(D3DXVECTOR3(0.0f , 1.0f , 0.0f)),
m_vKeyLook		(D3DXVECTOR3(0.0f , 1.0f , 0.0f)),	
m_vChangeLook	(D3DXVECTOR3(0.0f , 0.0f , 0.0f)),

m_vUp			(D3DXVECTOR3(0.0f , 1.0f , 0.0f)),
m_vKeyUp		(D3DXVECTOR3(0.0f , 1.0f , 0.0f)),	

m_vLenge		(eCameraState::LENGE_MONOSPACE),	

m_nMode			(eCameraState::FIXED),	
m_nChangeCnt	(0)
{
	D3DXMatrixIdentity(&m_matrixView);
}


//**関数***************************************************************************
//	概要	:	デストラクタ
//*********************************************************************************
CCamera::~CCamera(void)
{
}


//**関数***************************************************************************
//	概要	:	生成
//*********************************************************************************
CCamera* CCamera::Create(D3DXVECTOR3 pos , D3DXVECTOR3 Look , D3DXVECTOR3 UpVec , int nCameraMode , S_HANDLE hObj)
{
	CCamera* pCamera = new CCamera();
	if (pCamera) 
	{
		if (!pCamera->Initialize(pos , Look , UpVec , nCameraMode , hObj))
		{
			SAFE_DELETE(pCamera);
		}
	}
	return pCamera;
}


//**関数***************************************************************************
//	概要	:	生成時初期化
//*********************************************************************************
bool CCamera::Initialize(D3DXVECTOR3 pos , D3DXVECTOR3 Look , D3DXVECTOR3 UpVec , int nCameraMode , S_HANDLE hObj)
{
	// 代入
	m_vPos	= m_vKeyPos = pos;
	m_vLook	= m_vKeyLook = Look;
	m_vUp	= m_vKeyUp = UpVec;

	// 基準マトリックスがあれば格納
	if(hObj)
		m_hKeyObj = hObj;
	else if(!hObj && (nCameraMode == eCameraState::FIRSTPERSON || nCameraMode == eCameraState::THIRDPERSON))
		return false;

	// カメラ
	m_nChangeCnt = 0;								// 変更時のカウンタ
	m_nMode = nCameraMode;							// デフォルト定点

	// ビューマトリックス初期化
	D3DXMatrixIdentity(&m_matrixView);

	// ビューマトリックス設定
	D3DXMatrixLookAtLH(&m_matrixView,
						&m_vPos,					// 視点座標
						&m_vLook,					// 注視点座標
						&m_vUp);					// アップベクトル
	MANAGER.GetGraph()->GetDevice()->SetTransform(D3DTS_VIEW, &m_matrixView);

	return true;
}


//**関数***************************************************************************
//	概要	:	解放
//*********************************************************************************
void CCamera::Release()
{
	Finalize();
	delete this;
}


//**関数***************************************************************************
//	概要	:	終了処理
//*********************************************************************************
void CCamera::Finalize()
{
	
}


//**関数***************************************************************************
//	概要	:	入力受付
//*********************************************************************************
void CCamera::Input()
{
	CDXInput* pInput = MANAGER.GetInput();

}


//**関数***************************************************************************
//	概要	:	更新
//*********************************************************************************
void CCamera::Update()
{
	switch(m_nMode)
	{
	// 1人称
	// 3人称
	case eCameraState::FIRSTPERSON:
	case eCameraState::THIRDPERSON:
		if(m_nChangeCnt)
		{
			D3DXVec3TransformCoord(&m_vPos , &(m_vKeyPos - m_vChangePos * (float)m_nChangeCnt) , &REGISTER_H_P(m_hKeyObj , CObjBase*)->GetMatrix());
			D3DXVec3TransformCoord(&m_vLook , &(m_vKeyLook + m_vChangeLook * (float)m_nChangeCnt) , &REGISTER_H_P(m_hKeyObj , CObjBase*)->GetMatrix());
			m_nChangeCnt --;
		}
		else
		{
			D3DXVec3TransformCoord(&m_vPos , &m_vKeyPos , &REGISTER_H_P(m_hKeyObj , CObjBase*)->GetMatrix());
			D3DXVec3TransformCoord(&m_vLook , &m_vKeyLook , &REGISTER_H_P(m_hKeyObj , CObjBase*)->GetMatrix());
		}
		m_vUp.x = REGISTER_H_P(m_hKeyObj , CObjBase*)->GetMatrix()._21;
		m_vUp.y = REGISTER_H_P(m_hKeyObj , CObjBase*)->GetMatrix()._22;
		m_vUp.z = REGISTER_H_P(m_hKeyObj , CObjBase*)->GetMatrix()._23;
		break;


	// 注視点から一定間隔の位置を維持
	case eCameraState::MONOSPACE:
		if(m_nChangeCnt)
		{
			D3DXVec3TransformCoord(&m_vPos , &(m_vKeyPos - m_vChangePos * (float)m_nChangeCnt) , &REGISTER_H_P(m_hKeyObj , CObjBase*)->GetMatrix());
			D3DXVec3TransformCoord(&m_vLook , &(m_vKeyLook + m_vChangeLook * (float)m_nChangeCnt) , &REGISTER_H_P(m_hKeyObj , CObjBase*)->GetMatrix());

			m_nChangeCnt --;
		}
		else
		{
			m_vLook = REGISTER_H_P(m_hKeyObj , CObjBase*)->GetPos();
			m_vPos = m_vLook + m_vLenge;
		}
		break;

	default:			// 定点カメラ
		// 注視点移動
		if(m_nChangeCnt)
		{
			m_vPos = m_vPos - m_vChangePos * (float)m_nChangeCnt;
			m_nChangeCnt --;
		}
		else
		{
			m_vPos = m_vKeyPos;
			m_vLook = m_vKeyLook;
			m_vUp = m_vKeyUp;
		}
		break;
	}
}


//**関数***************************************************************************
//	概要	:	ビューマトリックス更新
//*********************************************************************************
bool CCamera::Draw(void)
{
	// ビューマトリックス設定
	D3DXMatrixLookAtLH(&m_matrixView,
						&m_vPos,					// 視点座標
						&m_vLook,					// 注視点座標
						&m_vUp);					// アップベクトル
	MANAGER.GetGraph()->GetDevice()->SetTransform(D3DTS_VIEW, &m_matrixView);

	return true;
}


//**関数***************************************************************************
//	概要	:	カメラモード変更
//*********************************************************************************
bool CCamera::SetCameraMode(int nMode , S_HANDLE hObj , D3DXVECTOR3 vPos , 
	D3DXVECTOR3 vLook , D3DXVECTOR3 vUp , int nChangeCnt)
{
	// 値変更
	/*if(m_nMode == nMode)
		return false;*/

	// 移動基準マトリックス
	m_hKeyObj = NULL;							// マトリックスクリア
	if(hObj)
		m_hKeyObj = hObj;


	D3DXVECTOR3	vNextPos , vNextLook;

	switch(nMode)
	{
	// 1人称
	// 3人称
	case eCameraState::FIRSTPERSON:
	case eCameraState::THIRDPERSON:
		if(!m_hKeyObj)
			return false;

		D3DXVec3TransformCoord(&vNextPos , &vPos , &REGISTER_H_P(m_hKeyObj , CObjBase*)->GetMatrix());
		D3DXVec3TransformCoord(&vNextLook , &vLook , &REGISTER_H_P(m_hKeyObj , CObjBase*)->GetMatrix());

		m_vKeyPos = vPos;
		m_vKeyLook = vLook;
		m_vKeyUp = vUp;
		break;


	// 等幅追従
	case eCameraState::MONOSPACE:
		if(!m_hKeyObj)
			return false;

		m_vKeyLook = vNextLook = REGISTER_H_P(m_hKeyObj , CObjBase*)->GetPos();
		m_vKeyPos = vNextPos = m_vKeyLook + m_vLenge;
		m_vKeyUp = eCameraState::UP_FIX;
		break;

	// 定点
	default:
		vNextPos = vPos;
		vNextLook = vLook;
		m_vKeyUp = vUp;
		break;
	}

	m_nMode = nMode;								// 次のカメラのモード設定
	
	// カメラ変更速度設定
	if(nChangeCnt > 0)
	{
		m_nChangeCnt = nChangeCnt;					// 変更にかけるフレーム
		m_vChangePos = (vNextPos - m_vPos) / (float)m_nChangeCnt;
		m_vChangeLook = (vNextLook - m_vLook) / (float)m_nChangeCnt;
	}
	// 変更に時間はかけない
	else
	{
		m_nChangeCnt = 0;
		m_vChangePos = m_vPos = vPos;
		m_vChangeLook = m_vLook = vLook;
		m_vUp = vUp;
	}

	return true;
}
//=================================================================================
//	End of File
//=================================================================================