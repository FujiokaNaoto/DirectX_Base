//プログラムヘッダ*****************************************************************
//	プログラム概要	:	フェードオブジェ
//*********************************************************************************
#pragma once

//==include部======================================================================
#include "Obj2D.h"
//=================================================================================


//==定数・列挙型宣言部=============================================================
namespace eFeadObjConst
{
	enum 
	{
		STOP,
		FEAD_IN,
		FEAD_MIDDLE,
		FEAD_MIDDLE_STOP,
		FEAD_OUT,
	};

	enum
	{
		NONE,
		WHITEFEAD,
		BLACKFEAD,

	};
}
//=================================================================================


//==マクロ定義部===================================================================

//=================================================================================


//==クラス・構造体宣言部===========================================================
// フェードだけをするオブジェ
class CFeadObj : public CObj2D
{
private:

protected:
	int		m_nInTime , m_nMiddleTime , m_nOutTime;

	// フェード昼間になった時に一度止めるかどうかのフラグ
	bool	m_bMiddleStop;
	int		m_nFeadType;							// フェードタイプ設定
	int		m_nFeadCnt;

public:
	CFeadObj();
	virtual ~CFeadObj();

	static CFeadObj*	Create(D3DXVECTOR3 pos , int nWidth , int nHeight ,
		int nSceneID , int nPriority , bool bMiddleStop);

	virtual bool	Init(D3DXVECTOR3 pos , int nWidth , int nHeight ,
		int nSceneID , int nPriority , bool bMiddleStop);

	virtual void	Update();

	virtual bool	FStart(int nType);
	virtual bool	FStartMiddle(int nType);
	virtual bool	MiddleStart();					// 中間で止めている時に、動かし始めるメッセージ

	virtual void	SetMiddleStopFlg(bool bFlg){m_bMiddleStop = bFlg;}
	virtual bool	SetFeadTime(int nIn , int nMiddle , int nOut);
};
//=================================================================================


//=================================================================================
//	End of File
//=================================================================================