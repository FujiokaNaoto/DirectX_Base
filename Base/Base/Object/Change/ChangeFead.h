//プログラムヘッダ*****************************************************************
//	プログラム概要	:	フェードクラス
//*********************************************************************************
#pragma once

//==include部======================================================================
#include "../Base/ChangeObj.h"
//=================================================================================


//==定数・列挙型宣言部=============================================================
//=================================================================================


//==マクロ定義部===================================================================
//=================================================================================


//==クラス・構造体宣言部===========================================================
class CChangeFead : public CChangeObj
{
public:
	// コンストラクタ・デストラクタ
	CChangeFead();
	virtual ~CChangeFead();

	// --生成--
	static CChangeFead*	Create(CChanging* pChange , int nFeadType , int nSceneID , 
		int nPriority);													// 生成

	virtual bool	Init(CChanging* pChange , int nFeadType , int nSceneID , 
		int nPriority);

	virtual void	Uninit();

	virtual void	Update();
	virtual	void	Draw();
	virtual	void	DrawLate();
	
	virtual void	ChangeStart();										// 変更開始
	virtual void	ChangeEnd();										// 変更終了


protected:
	// --変数--
	S_HANDLE		m_hFead;
	// --関数--


private:

};
//=================================================================================


//=================================================================================
//	End of File
//=================================================================================