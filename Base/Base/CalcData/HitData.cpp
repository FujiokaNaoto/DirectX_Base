//**プログラムヘッダ***************************************************************
//	プログラム概要	:	あたり判定情報クラス	
//*********************************************************************************


//==include部======================================================================
#include "HitData.h"
//=================================================================================


//==定数・列挙型部=================================================================

//=================================================================================


//==マクロ定義部===================================================================

//=================================================================================


//==クラス・構造体宣言部===========================================================

//=================================================================================


//==静的メンバ変数部===============================================================

//=================================================================================


//==プロトタイプ宣言===============================================================

//=================================================================================


//**関数***************************************************************************
//	概要	:	コンストラクタ
//*********************************************************************************
CHitRect::CHitRect()
{
	CHitRect(0.0f , 0.0f , 0.0f , D3DXVECTOR3(0.0f , 0.0f , 0.0f) , eHitRect::CENTER);
}


//**関数***************************************************************************
//	概要	:	引数つきコンストラクタ_2D
//*********************************************************************************
CHitRect::CHitRect(float fwidth , float fHeight , D3DXVECTOR3 point , int nPoint = eHitRect::CENTER)
{
	CHitRect(fwidth , fHeight , 0.0f , point , nPoint);
}


//**関数***************************************************************************
//	概要	:	引数つきコンストラクタ_3D
//*********************************************************************************
CHitRect::CHitRect(float fwidth , float fHeight , float fDepth , D3DXVECTOR3 point , int nPoint = eHitRect::CENTER)
{
	m_fWidth = fwidth;
	m_fHeight = fHeight;
	m_fDepth = fDepth;
	m_Point = point;
	m_nPointType = nPoint;
}


//**関数***************************************************************************
//	概要	:	デストラクタ
//*********************************************************************************
CHitRect::~CHitRect()
{
}


//--円------------------------------------------------------------------------------------------


//**関数***************************************************************************
//	概要	:	コンストラクタ
//*********************************************************************************
CHitCircle::CHitCircle()
{
	CHitCircle(0.0f ,  D3DXVECTOR3(0.0f , 0.0f , 0.0f));
}


//**関数***************************************************************************
//	概要	:	引数つきコンストラクタ
//*********************************************************************************
CHitCircle::CHitCircle(float fRadius , D3DXVECTOR3 point)
{
	m_fRadius = fRadius;
	m_Point = point;
}


//**関数***************************************************************************
//	概要	:	デストラクタ
//*********************************************************************************
CHitCircle::~CHitCircle()
{
}


//--三角形----------------------------------------------------------------------------------------

//**関数***************************************************************************
//	概要	:	コンストラクタ
//*********************************************************************************
CHitDelta::CHitDelta()
{
	CHitDelta(D3DXVECTOR3(0.0f , 0.0f , 0.0f) , D3DXVECTOR3(0.0f , 0.0f , 0.0f) , 
		D3DXVECTOR3(0.0f , 0.0f , 0.0f));
}


//**関数***************************************************************************
//	概要	:	引数つきコンストラクタ
//*********************************************************************************
CHitDelta::CHitDelta(D3DXVECTOR3 point1 , D3DXVECTOR3 point2 , D3DXVECTOR3 point3)
{
	m_Points[0] = point1;
	m_Points[1] = point2;
	m_Points[2] = point3;
}


//**関数***************************************************************************
//	概要	:	デストラクタ
//*********************************************************************************
CHitDelta::~CHitDelta()
{
}


//--線分------------------------------------------------------------------------------------------

//**関数***************************************************************************
//	概要	:	コンストラクタ
//*********************************************************************************
CHitLine::CHitLine()
{
	CHitLine(D3DXVECTOR3(0.0f , 0.0f , 0.0f) , D3DXVECTOR3(0.0f , 0.0f , 0.0f));
}


//**関数***************************************************************************
//	概要	:	引数つきコンストラクタ
//*********************************************************************************
CHitLine::CHitLine(D3DXVECTOR3 StartPoint , D3DXVECTOR3 EndPoint)
{
	m_SPoint = StartPoint;
	m_EPoint = EndPoint;
}


//**関数***************************************************************************
//	概要	:	デストラクタ
//*********************************************************************************
CHitLine::~CHitLine()
{
}

//=================================================================================
//	End of File
//=================================================================================