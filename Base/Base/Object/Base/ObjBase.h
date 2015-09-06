//プログラムヘッダ*****************************************************************
//	プログラム概要	:	オブジェクト派生元ベースクラス
//*********************************************************************************
#pragma once

//==include部======================================================================
#include "../../System/common.h"
#include <d3d9.h>
#include <d3dx9.h>
#pragma comment(lib , "dxguid")	
#pragma comment(lib , "d3d9")
#pragma comment(lib , "d3dx9")
//=================================================================================

//==定数・列挙型宣言部=============================================================
const UINT ID_NONE = 0;

const D3DXVECTOR3	DEFAULT_POS = D3DXVECTOR3(0.0f , 0.0f , 0.0f);
const D3DXVECTOR3	DEFAULT_ROT = D3DXVECTOR3(0.0f , 0.0f , 0.0f);
const D3DXVECTOR3	DEFAULT_SCALL = D3DXVECTOR3(1.0f , 1.0f , 1.0f);

const D3DXVECTOR3	DEFAULT_SPD = D3DXVECTOR3(0.0f , 0.0f , 0.0f);
const D3DXVECTOR3	DEFAULT_ACCEL = D3DXVECTOR3(0.0f , 0.0f , 0.0f);
//=================================================================================


//==マクロ定義部===================================================================

//=================================================================================


//==クラス・構造体宣言部===========================================================
class CRegister;
class CObjBase
{
public:
	// コンストラクタ・デストラクタ
	CObjBase();
	virtual ~CObjBase();

	static CObjBase*	Create(int nSceneID ,int nPriority);
	static CObjBase*	Create(D3DXVECTOR3 pos , int nSceneID ,
						int nPriority);

	virtual bool	Init(int nSceneID ,int nPriority);				// ゲームループ中初期化
	virtual bool	Init(D3DXVECTOR3 pos , int nSceneID ,
						int nPriority);								// ゲームループ中初期化

	virtual void	Uninit(){}										// ゲームループ中終了処理
	virtual void	Update();										// 更新
	virtual void	OnInput(){};									// 入力受付


	virtual void	SetRS(){}										// レンダーステート設定
	virtual void	Draw(){}										// 先に描画(3Dオブジェクトより先に2D背景を表示したり、不透明オブジェを表示したり)
	virtual void	SetRSLate(){}									// 後描画用レンダーステート設定
	virtual void	DrawLate(){}									// 後に描画(2DメニューやUI、半透明オブジェの表示)

	virtual void	Release();										// 解放処理


	// あたり判定
	virtual	void	CheckCollision(){}								// あたり判定命令を呼び出す
	virtual bool	Collision(void* pObj , int ObjType){return false;}	// あたり判定	nObjTypeで相手がどんなオブジェクトかを判断する

	virtual	void	Affine();										// アフィン変換

	// 比較関数を定義
	static bool asc( const CObjBase* left, const CObjBase* right );

	// --ゲットセット--
	// 使用判定フラグ
	virtual bool	GetUseFlg(){return m_bUseFlg;}
	virtual void	SetUpdateFlg(bool bFlg){m_bUseFlg = bFlg;}		
	// 描画タイミングフラグ
	virtual bool	GetDrawLateFlg(){return m_bDrawLate;}			
	virtual void	SetDrawLateFlg(bool bFlg){m_bDrawLate = bFlg;}	

	// 削除フラグ、trueにするとレジスタから削除される
	virtual bool	GetDeleteFlg(){return m_bDeleteFlg;}			
	virtual void	SetDeleteFlg(bool bFlg){m_bDeleteFlg = bFlg;}	
		
	virtual UINT	GetObjID(){return m_nObjID;}					

	// 所属クラスのID、これを元にレジスタから検索をかけられる
	virtual UINT	GetClassID(){return m_nClassID;}				
	virtual void	SetClassID(UINT nID){m_nClassID = nID;}			

	// レンダーステート
	virtual UINT	GetRS(){return m_nRSType;}						
	virtual UINT	GetRSLate(){return m_nRSTypeLate;}				

	// 所属シーンを示すID、シーンの切り替わり時に該当シーンのオブジェクトは削除される
	virtual int		GetSceneID(){return m_nSceneID;}				
	virtual void	SetSceneID(int nID){m_nSceneID = nID;}

	// 所属ステージ、更新リストのステージ分け用
	virtual int		GetStageID(){return m_nStageID;}
	virtual void	SetStageID(int nID){m_nStageID = nID;}	

	// 描画ステージ、更新順と描画順を分けるためのID
	virtual int		GetDrawID(){return m_nDrawID;}
	virtual void	SetDrawID(int nID){m_nDrawID = nID;}

	// ステージ内のプライオリティを取得
	virtual UINT	GetPriority(){return m_nPriority;}			
	virtual void	SetPriority(UINT nPri){m_nPriority = nPri;}	

	virtual int		GetState(){return m_nStatus;}
	virtual void	SetState(int nState){m_nStatus = nState;}

	// ハンドル
	virtual S_HANDLE	GetHandle(){return m_nHandle;}
	virtual S_HANDLE	GetParent(){return m_nParent;}
	virtual void		SetParent(S_HANDLE handle){m_nParent = handle;}

	// シェーダタイプ
	virtual int		GetShadeType(){return m_nShadeType;}
	virtual void	SetShadeType(int nType){m_nShadeType = nType;}

	// アフィン系
	virtual D3DXVECTOR3	GetPos(){return m_Pos;}						// 座標取得
	virtual void	SetPos(D3DXVECTOR3 pos){m_Pos = pos;}			// 座標セット
	virtual D3DXVECTOR3	GetRot(){return m_Rot;}						// 回転取得
	virtual void	SetRot(D3DXVECTOR3 rot){m_Rot = rot;}			// 回転セット
	virtual D3DXVECTOR3	GetScall(){return m_Scall;}					// 拡大取得
	virtual void	SetScall(D3DXVECTOR3 scall){m_Scall = scall;}	// 拡大セット

	// 速度系
	virtual D3DXVECTOR3	GetSpd(){return m_Spd;}						// 速度取得
	virtual void	SetSpd(D3DXVECTOR3 Spd){m_Spd = Spd;}			// 速度セット
	virtual D3DXVECTOR3	GetAccel(){return m_Accel;}					// 加速度取得
	virtual void	SetAccel(D3DXVECTOR3 Accel){m_Accel = Accel;}	// 加速度セット

	virtual D3DXMATRIX	GetMatrix(){return m_Matrix;}				// 行列取得
	virtual void	SetMatrix(D3DXMATRIX matrix){m_Matrix = matrix;}// 行列セット
	
	virtual float	GetRadius(){return m_fRadius;}					// 半径取得



protected:
	// --変数--
	static UINT	m_nUniqeID;											// オブジェクトのユニークIDハンドル(これを生成時に戻して、ハンドルにする)

	UINT	m_nObjID;												// オブジェクトID
	UINT	m_nClassID;												// クラスを示すID

	UINT	m_nRSType;												// レンダーステートタイプ
	UINT	m_nRSTypeLate;											// 後描画用レンダーステートタイプ

	int		m_nShadeType;											// 仕様シェーダをビットでまとめて加工

	int		m_nSceneID;												// シーンを示すID
	int		m_nStageID;												// 所属ステージを示すID
	int		m_nDrawID;												// 所属描画ステージを示すID
	
	S_HANDLE m_nHandle;												// 自身のハンドル
	S_HANDLE m_nParent;												// 親のハンドル


	UINT	m_nPriority;											// 優先順位
	int		m_nStatus;												// 状態

	D3DXVECTOR3	m_Pos;												// 位置
	D3DXVECTOR3	m_Rot;												// 回転角度
	D3DXVECTOR3	m_Scall;											// 拡大率

	D3DXVECTOR3	m_Spd;												// 速度
	D3DXVECTOR3	m_Accel;											// 加速度

	D3DXMATRIX	m_Matrix;											// ワールドマトリクス
	float		m_fRadius;											// 半径
	
	bool	m_bUseFlg;												// 使用判定のフラグ
	bool	m_bDrawLate;											// 描画タイミング、後に描画するときはこのフラグを立てる

	bool	m_bDeleteFlg;											// 削除フラグ
};
//=================================================================================
//=================================================================================
//	End of File
//=================================================================================