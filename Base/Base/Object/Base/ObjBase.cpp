//**プログラムヘッダ***************************************************************
//	プログラム概要	:	オブジェクトクラスベース
//*********************************************************************************


//==include部======================================================================
#include "../../System/GameManager.h"
#include "../../System/Register.h"
#include "ObjBase.h"
//=================================================================================


//==定数・列挙型部=================================================================

//=================================================================================


//==マクロ定義部===================================================================

//=================================================================================


//==クラス・構造体宣言部===========================================================

//=================================================================================


//==静的メンバ変数部===============================================================
UINT CObjBase::m_nUniqeID = 0;
//=================================================================================


//==プロトタイプ宣言===============================================================

//=================================================================================


//**関数***************************************************************************
//	概要	:	コンストラクタ
//*********************************************************************************
CObjBase::CObjBase():
m_Pos (D3DXVECTOR3(0.0f , 0.0f , 0.0f)) ,
m_Rot (DEFAULT_ROT) , 
m_Scall (DEFAULT_SCALL),

m_Spd (DEFAULT_SPD),
m_Accel (DEFAULT_ACCEL),

m_fRadius(0.0f),

m_bUseFlg (true),
m_bDrawLate(false),
m_bDeleteFlg(false),

m_nClassID (ID_NONE),
m_nSceneID(eScene_ID::NONE),
m_nPriority (0),
m_nParent(NULL),

m_nRSType(eRSType::NONE),
m_nRSTypeLate(eRSType::NONE),

m_nStatus(-1)
{
	m_nHandle = REGISTER->handle.CreateHandle(this);

	D3DXMatrixIdentity(&m_Matrix);				// 行列初期化
	m_nObjID = m_nUniqeID;						// ユニークIDをオブジェクトに登録
	m_nUniqeID ++;
}


//**関数***************************************************************************
//	概要	:	デストラクタ
//*********************************************************************************
CObjBase::~CObjBase()
{
	REGISTER->handle.ReleaseHandle(m_nHandle);
}


//**関数***************************************************************************
//	概要	:	生成
//*********************************************************************************
CObjBase* CObjBase::Create(int nSceneID ,int nPriority)
{
	CObjBase*	pObj = new CObjBase();

	if(pObj->Init(nSceneID , nPriority))
		return pObj;

	SAFE_DELETE(pObj);
	return NULL;
}


//**関数***************************************************************************
//	概要	:	生成
//*********************************************************************************
CObjBase* CObjBase::Create(D3DXVECTOR3 pos , int nSceneID , int nPriority)
{
	CObjBase*	pObj = new CObjBase();

	if(pObj->Init(pos , nSceneID , nPriority))
		return pObj;
	
	SAFE_DELETE(pObj);
	return NULL;
}


//**関数***************************************************************************
//	概要	:	初期化
//*********************************************************************************
bool CObjBase::Init(int nSceneID ,int nPriority)
{
	m_Pos = DEFAULT_POS;								// 位置
	m_Rot = DEFAULT_ROT;								// 回転角
	m_Scall = DEFAULT_SCALL;							// 拡大率

	m_nSceneID = nSceneID;								// シーンID
	m_nPriority = nPriority;							// プライオリティ

	return true;
}


//**関数***************************************************************************
//	概要	:	初期化
//*********************************************************************************
bool CObjBase::Init(D3DXVECTOR3 pos , int nSceneID , int nPriority)
{
	m_Pos = pos;								// 位置
	m_Rot = DEFAULT_ROT;								// 回転角
	m_Scall = DEFAULT_SCALL;							// 拡大率

	m_nSceneID = nSceneID;					// シーンID
	m_nPriority = nPriority;					// プライオリティ

	return true;
}


//**関数***************************************************************************
//	概要	:	更新
//*********************************************************************************
void CObjBase::Update()
{
	if(!m_bUseFlg)				// 描画しないときは処理なし
		return;

	m_Spd += m_Accel;
	m_Pos += m_Spd;
}


//**関数***************************************************************************
//	概要	:	アフィン
//*********************************************************************************
void CObjBase::Affine()
{
	if(!m_bUseFlg)				// 描画しないときは処理なし
		return;

	D3DXMATRIX	matTrans , matRot , matScall;

	// 初期化
	D3DXMatrixIdentity(&matTrans);
	D3DXMatrixIdentity(&matRot);
	D3DXMatrixIdentity(&matScall);

	// アフィン
	D3DXMatrixTranslation(&matTrans , m_Pos.x , m_Pos.y , m_Pos.z);			// 移動
	D3DXMatrixRotationYawPitchRoll(&matRot , m_Rot.y , m_Rot.x , m_Rot.z);	// 回転
	D3DXMatrixScaling(&matScall , m_Scall.x , m_Scall.y , m_Scall.z);		// 拡縮

	m_Matrix = matRot * matScall * matTrans;
}


//**関数***************************************************************************
//	概要	:	解放
//*********************************************************************************
void CObjBase::Release()
{
	delete this;
}


bool CObjBase::asc( const CObjBase* left, const CObjBase* right ) 
{
	//プライオリティー値 + 描画ステージx1000 がソート値
		return left->m_nPriority + left->m_nDrawID * STGAE_ENTRY_MAX < right->m_nPriority + right->m_nDrawID * STGAE_ENTRY_MAX;
}

//=================================================================================
//	End of File
//=================================================================================