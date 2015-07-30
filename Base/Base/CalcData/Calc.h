//プログラムヘッダ*****************************************************************
//	プログラム概要	:	計算クラス定義
//*********************************************************************************
#pragma once

//==include部======================================================================
#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")

#include "BoundData.h"
#include "../Object/Base/CHANDLE.h"
#include "HitData.h"
//=================================================================================


//==定数・列挙型宣言部=============================================================

//=================================================================================


//==マクロ定義部===================================================================
//=================================================================================


//==クラス・構造体宣言部===========================================================
class CCalc
{
private:

protected:

public:

	// --あたり判定--
	// --2D--
	// 矩形当たり
	static bool IntersectBox2D(CHitRect rectA , CHitRect rectB);
	static bool IntersectBox2DScale(CHitRect rectA , D3DMATRIX matA , CHitRect rectB , D3DMATRIX matB);		// 拡大込み当たり

	// 2D円当たり
	static bool IntersectCircle(CHitCircle circleA , CHitCircle circleB);
	// 拡大込み、2D円当たり
	static bool IntersectCircleScale(CHitCircle circleA , D3DMATRIX matA , CHitCircle circleB , D3DXMATRIX matB);		// 拡大込み当たり

	// 円と矩形の当たり
	static bool	IntersectBoxAndCircle(CHitRect rect , CHitCircle circle);
	static bool	IntersectBoxAndCircleScale(CHitRect rect , D3DMATRIX matRect , CHitCircle circle , D3DMATRIX matCircle);


	// --3D--
	// 3D矩形
	static bool IntersectBox3D(CHitRect rectA , CHitRect rectB);
	static bool IntersectBox3DScale(CHitRect rectA , D3DMATRIX matA , CHitRect rectB , D3DMATRIX matB);		// 拡大込み当たり

	// 3D球当たり
	static bool IntersectSphere(CHitCircle circleA , CHitCircle circleB);
	static bool IntersectSphereScale(CHitCircle circleA , D3DMATRIX matA , CHitCircle circleB , D3DXMATRIX matB);		// 拡大込み当たり

	// 3DOBB当たり
	static bool IntersectOBB(S_HANDLE pObjA , S_HANDLE pObjB);
	static bool IntersectOBB(D3DXMATRIX matA , D3DXVECTOR3 ObbHalfA , D3DXVECTOR3 CenterA , D3DXMATRIX matB , D3DXVECTOR3 ObbHalfB , D3DXVECTOR3 CenterB);

	// 三角形と線分のあたり判定
	static bool	IntersectDeltaAndLine(CHitDelta delta , CHitLine line);

	// --衝突判定--
	static void	Bound2D(CBoundData* boundA , CBoundData* boundB);
	static void	Bound3D(CBoundData* boundA , CBoundData* boundB);
	
	static void	WallBound(CBoundData* wall , D3DXVECTOR3 wallVec , CBoundData* boundB);
	
	//曲線計算
	static D3DXVECTOR3	CurvHermite(float t ,D3DXVECTOR3 Spos,D3DXVECTOR3 Epos,D3DXVECTOR3 vec1,D3DXVECTOR3 vec2);
	static D3DXVECTOR3	CurvBezier(float t ,D3DXVECTOR3 Spos,D3DXVECTOR3 Epos,D3DXVECTOR3 vec1,D3DXVECTOR3 vec2);

	//2点の距離計算
	static float Range(D3DXVECTOR3 pos1,D3DXVECTOR3 pos2);
};
//=================================================================================


//=================================================================================
//	End of File
//=================================================================================