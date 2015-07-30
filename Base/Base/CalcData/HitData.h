//プログラムヘッダ*****************************************************************
//	プログラム概要	:	あたり判定情報クラス定義
//*********************************************************************************
#pragma once

//==include部======================================================================
#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")
//=================================================================================


//==定数・列挙型宣言部=============================================================
namespace eHitRect
{
	enum
	{
		CENTER,							// 中央
		DOWN_CENTER,					// 中央左下
		UP_LEFT_2D,						// 2D用_左上
		DOWN_RIGHT_2D,					// 2D用_右下
	};
};
//=================================================================================


//==マクロ定義部===================================================================

//=================================================================================


//==クラス・構造体宣言部===========================================================
// あたり判定用矩形データ
class CHitRect
{
public:
	float		m_fWidth , m_fHeight , m_fDepth;				// 横幅 , 縦幅 , 奥行
	int			m_nPointType;									// 制御点のタイプ
	D3DXVECTOR3	m_Point;										// 制御点

	CHitRect();
	CHitRect(float fwidth , float fHeight , D3DXVECTOR3 point ,
			 int nPoint);										// 2D用初期値
	CHitRect(float fwidth , float fHeight , float fDepth , 
			D3DXVECTOR3 point , int nPoint);					// 3D用初期値
	virtual ~CHitRect();
};

class CHitCircle
{
public:
	float		m_fRadius;										// 半径
	D3DXVECTOR3	m_Point;										// 制御点

	CHitCircle();
	CHitCircle(float fRadius , D3DXVECTOR3 point);				// 引数付きコンストラクタ
	virtual ~CHitCircle();
};


class CHitDelta
{
public:
	D3DXVECTOR3	m_Points[3];

	CHitDelta();
	CHitDelta(D3DXVECTOR3 point1 , D3DXVECTOR3 point2 , D3DXVECTOR3 point3);
	virtual ~CHitDelta();
};


class CHitLine
{
public:
	D3DXVECTOR3	m_SPoint , m_EPoint;							// 開始点、終了店
	
	CHitLine();
	CHitLine(D3DXVECTOR3 startPoint , D3DXVECTOR3 endPoint);
	virtual ~CHitLine();
};


class CHitCapsul
{
public:
	D3DXVECTOR3	m_SPoint , m_EPoint;							// 開始点、終了店
	
	CHitCapsul();
	CHitCapsul(D3DXVECTOR3 startPoint , D3DXVECTOR3 endPoint , float fRad);
	virtual ~CHitCapsul();
};
//=================================================================================


//=================================================================================
//	End of File
//=================================================================================