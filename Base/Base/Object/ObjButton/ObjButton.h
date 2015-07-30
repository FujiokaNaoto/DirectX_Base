//プログラムヘッダ*****************************************************************
//	プログラム概要	:	2Dボタンクラス定義
//*********************************************************************************
#pragma once

//==include部======================================================================
#include "../Obj2D/Obj2D.h"
//=================================================================================


//==定数・列挙型宣言部=============================================================
namespace eButtonMoveType
{
	enum
	{
		NONE = 0,
		SIN_MOVE = 1,				// sin波上下移動
		SCALL = 2,					// 拡大縮小
	};
};

namespace eButtonState
{
	enum
	{
		DEFAULT,					// 通常状態
		HOVER,						// カーソルが上に載っているとき
		ACTIVE,						// アクティブ状態
		INACTIVE,					// 停止状態
	};
};
//=================================================================================


//==マクロ定義部===================================================================
//=================================================================================


//==クラス・構造体宣言部===========================================================
class CObjButton : public CObj2D
{
public:
	// コンストラクタ・デストラクタ
	CObjButton();
	virtual ~CObjButton();

	static CObjButton*	Create(LPCTSTR texName , D3DXVECTOR3 pos , 
		 int nWidth , int nHeight , int nSceneID , int nPriority);	// 生成

	static CObjButton*	Create(LPCTSTR texName , D3DXVECTOR3 pos , 
		D3DXVECTOR3 rot , D3DXVECTOR3 scall , int nWidth , 
		int nHeight , int nSceneID , int nPriority);				// 生成
	
	
	virtual bool	Init(LPCTSTR texName , D3DXVECTOR3 pos , 
		 int nWidth , int nHeight , int nSceneID , int nPriority);
	virtual bool	Init(LPCTSTR texName , D3DXVECTOR3 pos , 
		D3DXVECTOR3 rot , D3DXVECTOR3 scall , int nWidth , 
		int nHeight , int nSceneID , int nPriority);
	

	virtual	void	Update();										// 更新
	virtual void	CheckCollision();								// あたり判定調査


	// --Get Set--
	// 状態
	virtual void	SetState(int nState);

	// 挙動
	void	SetMoveType(int nType){m_nMoveType = nType;}			// 挙動セット
	void	ChangeMoveType(int nType){m_nMoveType ^= nType;}		// 挙動変更
	int		GetMoveType(){return m_nMoveType;}						// 挙動取得

	// 上下
	void	SetRadSpd(float fSpd){m_fRadSpd = fSpd;}
	void	SetRadLenge(float fLenge){m_fRadLenge = fLenge;}		// 移動領域セット
	
	// 拡縮
	void	SetScallLenge(float	fLenge){m_fScallLenge = fLenge;}	// 拡縮上限セット
	void	SetScallSpd(float fSpd){m_fScallSpd = fSpd;}			// 拡縮速度セット


protected:
	// --変数--
	int				m_nMoveType;
	D3DXVECTOR3		m_KeyPos;										// 移動のキーになる座標

	// 上下移動
	float			m_fRadLenge;									// ボタン移動領域変数
	float			m_fRadian;										// ボタン移動用変数
	float			m_fRadSpd;										// ボタン移動速度

	// スケール移動
	float			m_fScallLenge;									// スケールの上限
	float			m_fScallSpd;									// スケールの
	bool			m_bExtendFlg;									// 拡大フラグ

	bool			m_bTriggerFlg;									// トリガー判定フラグ

	// --関数--
	virtual void	OnTrigger();
	virtual void	OnRelease();

	virtual	void	UpdateMove();									// 挙動についての更新
	virtual void	UpdateState();									// 状態ごとの更新

private:

};
//=================================================================================


//=================================================================================
//	End of File
//=================================================================================