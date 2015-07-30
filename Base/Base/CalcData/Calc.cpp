//**プログラムヘッダ***************************************************************
//	プログラム概要	:	計算クラス実装
//					:	計算式を一括で格納
//*********************************************************************************


//==include部======================================================================
#include "Calc.h"
#include "../System/common.h"
#include "../System/Register.h"
#include <math.h>

#include "../Object/Obj3D/Obj3D.h"
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
//	概要	:	2D矩形あたり判定
//*********************************************************************************
bool CCalc::IntersectBox2D(CHitRect rectA , CHitRect rectB)
{
	// 左上に位置合わせ
	switch(rectA.m_nPointType)
	{
	case eHitRect::CENTER:
		rectA.m_Point.x -= rectA.m_fWidth / 2.0f;
		rectA.m_Point.y -= rectA.m_fHeight / 2.0f;
		break;

	case eHitRect::UP_LEFT_2D:
		break;

	case eHitRect::DOWN_CENTER:
		rectA.m_Point.x -= rectA.m_fWidth / 2.0f;
		rectA.m_Point.y -= rectA.m_fHeight;
		break;

	case eHitRect::DOWN_RIGHT_2D:
		rectA.m_Point.x -= rectA.m_fWidth;
		rectA.m_Point.y -= rectA.m_fHeight;
		break;

	default:
		// エラー
		return false;
		break;
	}

	switch(rectB.m_nPointType)
	{
	case eHitRect::CENTER:
		rectB.m_Point.x -= rectB.m_fWidth / 2.0f;
		rectB.m_Point.y -= rectB.m_fHeight / 2.0f;
		break;

	case eHitRect::UP_LEFT_2D:
		break;

	case eHitRect::DOWN_CENTER:
		rectB.m_Point.x -= rectB.m_fWidth / 2.0f;
		rectB.m_Point.y -= rectB.m_fHeight;
		break;

	case eHitRect::DOWN_RIGHT_2D:
		rectB.m_Point.x -= rectB.m_fWidth;
		rectB.m_Point.y -= rectB.m_fHeight;
		break;

	default:
		// エラー
		return false;
		break;
	}

	
	// 判定
	if(rectA.m_Point.x < rectB.m_Point.x + rectB.m_fWidth && rectA.m_Point.x + rectA.m_fWidth > rectB.m_Point.x &&
		rectA.m_Point.y < rectB.m_Point.y + rectB.m_fHeight && rectA.m_Point.y + rectA.m_fHeight > rectB.m_Point.y)
		return true;
	else
		return false;
}


//**関数***************************************************************************
//	概要	:	拡大込み2D矩形あたり判定
//*********************************************************************************
bool CCalc::IntersectBox2DScale(CHitRect rectA , D3DMATRIX matA , CHitRect rectB , D3DMATRIX matB)
{
	// 拡大率計算
	D3DXVECTOR3 ScaleA = D3DXVECTOR3(0,0,0) , ScaleB = D3DXVECTOR3(0,0,0);
	ScaleA.x = D3DXVec3Length(& D3DXVECTOR3(matA._11 , matA._12 , matA._13));
	ScaleA.y = D3DXVec3Length(& D3DXVECTOR3(matA._21 , matA._22 , matA._23));
	ScaleB.x = D3DXVec3Length(& D3DXVECTOR3(matB._11 , matB._12 , matB._13));
	ScaleB.y = D3DXVec3Length(& D3DXVECTOR3(matB._21 , matB._22 , matB._23));

	rectA.m_fWidth *= ScaleA.x;
	rectA.m_fHeight *= ScaleA.y;
	rectB.m_fWidth *= ScaleB.x;
	rectB.m_fHeight *= ScaleB.y;

	// 左上に位置合わせ
	switch(rectA.m_nPointType)
	{
	case eHitRect::CENTER:
		rectA.m_Point.x -= rectA.m_fWidth / 2.0f;
		rectA.m_Point.y -= rectA.m_fHeight / 2.0f;
		break;

	case eHitRect::UP_LEFT_2D:
		break;

	case eHitRect::DOWN_CENTER:
		rectA.m_Point.x -= rectA.m_fWidth / 2.0f;
		rectA.m_Point.y -= rectA.m_fHeight;
		break;

	case eHitRect::DOWN_RIGHT_2D:
		rectA.m_Point.x -= rectA.m_fWidth;
		rectA.m_Point.y -= rectA.m_fHeight;
		break;

	default:
		// エラー
		return false;
		break;
	}

	switch(rectB.m_nPointType)
	{
	case eHitRect::CENTER:
		rectB.m_Point.x -= rectB.m_fWidth / 2.0f;
		rectB.m_Point.y -= rectB.m_fHeight / 2.0f;
		break;

	case eHitRect::UP_LEFT_2D:
		break;

	case eHitRect::DOWN_CENTER:
		rectB.m_Point.x -= rectB.m_fWidth / 2.0f;
		rectB.m_Point.y -= rectB.m_fHeight;
		break;

	case eHitRect::DOWN_RIGHT_2D:
		rectB.m_Point.x -= rectB.m_fWidth;
		rectB.m_Point.y -= rectB.m_fHeight;
		break;

	default:
		// エラー
		return false;
		break;
	}
	
	// 判定
	if(rectA.m_Point.x < rectB.m_Point.x + rectB.m_fWidth && rectA.m_Point.x + rectA.m_fWidth > rectB.m_Point.x &&
		rectA.m_Point.y < rectB.m_Point.y + rectB.m_fHeight && rectA.m_Point.y + rectA.m_fHeight > rectB.m_Point.y)
		return true;
	else
		return false;
}


//**関数***************************************************************************
//	概要	:	円判定
//*********************************************************************************
bool CCalc::IntersectCircle(CHitCircle circleA , CHitCircle circleB)
{
	D3DXVECTOR2	pointA = circleA.m_Point;
	D3DXVECTOR2 pointB = circleB.m_Point;

	float fDist = D3DXVec2LengthSq(&(pointA - pointB));
	float fRadi = powf(circleA.m_fRadius + circleB.m_fRadius , 2.0f);

	if(fRadi > fDist)
		return true;
	else
		return false;
}


//**関数***************************************************************************
//	概要	:	拡大適応円判定
//*********************************************************************************
bool CCalc::IntersectCircleScale(CHitCircle circleA , D3DMATRIX matA , CHitCircle circleB , D3DXMATRIX matB)
{
	D3DXVECTOR2	pointA = circleA.m_Point;
	D3DXVECTOR2 pointB = circleB.m_Point;

	// ワールドマトリクスから拡大率取得
	D3DXVECTOR3 ScaleA = D3DXVECTOR3(0,0,0) , ScaleB = D3DXVECTOR3(0,0,0);
	ScaleA.x = D3DXVec3Length(&D3DXVECTOR3(matA._11 , matA._12 , matA._13));
	//ScaleA.y = D3DXVec3Length(&D3DXVECTOR3(matA._21 , matA._22 , matA._23));
	ScaleB.x = D3DXVec3Length(&D3DXVECTOR3(matB._11 , matB._12 , matB._13));
	//ScaleB.y = D3DXVec3Length(&D3DXVECTOR3(matB._21 , matB._22 , matB._23));

	float fDist = D3DXVec2LengthSq(&(pointA - pointB));
	float fRadi = powf(circleA.m_fRadius * ScaleA.x + circleB.m_fRadius * ScaleB.x , 2.0f);

	if(fRadi > fDist)
		return true;
	else
		return false;
}


//**関数***************************************************************************
//	概要	:	2D矩形と円判定
//*********************************************************************************
bool CCalc::IntersectBoxAndCircle(CHitRect rect , CHitCircle circle)
{
	rect.m_fWidth += circle.m_fRadius * 2;
	rect.m_fHeight += circle.m_fRadius * 2;

	// 左上に位置合わせ
	switch(rect.m_nPointType)
	{
	case eHitRect::CENTER:
		rect.m_Point.x -= rect.m_fWidth / 2.0f;
		rect.m_Point.y -= rect.m_fHeight / 2.0f;
		break;

	case eHitRect::UP_LEFT_2D:
		break;

	case eHitRect::DOWN_CENTER:
		rect.m_Point.x -= rect.m_fWidth / 2.0f;
		rect.m_Point.y -= rect.m_fHeight;
		break;

	case eHitRect::DOWN_RIGHT_2D:
		rect.m_Point.x -= rect.m_fWidth;
		rect.m_Point.y -= rect.m_fHeight;
		break;

	default:
		// エラー
		return false;
		break;
	}
	
	// 判定
	if(rect.m_Point.x < circle.m_Point.x && rect.m_Point.x + rect.m_fWidth > circle.m_Point.x &&
		rect.m_Point.y < circle.m_Point.y && rect.m_Point.y + rect.m_fHeight > circle.m_Point.y)
		return true;
	else
		return false;
}


//**関数***************************************************************************
//	概要	:	拡大込み2D矩形と円判定
//*********************************************************************************
bool CCalc::IntersectBoxAndCircleScale(CHitRect rect , D3DMATRIX matRect , CHitCircle circle , D3DMATRIX matCircle)
{
	D3DXVECTOR3 ScaleA = D3DXVECTOR3(0,0,0) , ScaleB = D3DXVECTOR3(0,0,0);
	ScaleA.x = D3DXVec3Length(& D3DXVECTOR3(matRect._11 , matRect._12 , matRect._13));
	ScaleA.y = D3DXVec3Length(& D3DXVECTOR3(matRect._21 , matRect._22 , matRect._23));
	ScaleB.x = D3DXVec3Length(& D3DXVECTOR3(matCircle._11 , matCircle._12 , matCircle._13));
	
	rect.m_fWidth *= ScaleA.x;
	rect.m_fHeight *= ScaleA.y;
	circle.m_fRadius *= ScaleB.x;


	rect.m_fWidth += circle.m_fRadius * 2;
	rect.m_fHeight += circle.m_fRadius * 2;

	// 左上に位置合わせ
	switch(rect.m_nPointType)
	{
	case eHitRect::CENTER:
		rect.m_Point.x -= rect.m_fWidth / 2.0f;
		rect.m_Point.y -= rect.m_fHeight / 2.0f;
		break;

	case eHitRect::UP_LEFT_2D:
		break;

	case eHitRect::DOWN_CENTER:
		rect.m_Point.x -= rect.m_fWidth / 2.0f;
		rect.m_Point.y -= rect.m_fHeight;
		break;

	case eHitRect::DOWN_RIGHT_2D:
		rect.m_Point.x -= rect.m_fWidth;
		rect.m_Point.y -= rect.m_fHeight;
		break;

	default:
		// エラー
		return false;
		break;
	}
	
	// 判定
	if(rect.m_Point.x < circle.m_Point.x && rect.m_Point.x + rect.m_fWidth > circle.m_Point.x &&
		rect.m_Point.y < circle.m_Point.y && rect.m_Point.y + rect.m_fHeight > circle.m_Point.y)
		return true;
	else
		return false;
}


//**関数***************************************************************************
//	概要	:	3D矩形あたり判定
//*********************************************************************************
bool CCalc::IntersectBox3D(CHitRect rectA , CHitRect rectB)
{
	// 左上手前に位置合わせ
	switch(rectA.m_nPointType)
	{
	case eHitRect::CENTER:
		rectA.m_Point.x -= rectA.m_fWidth / 2.0f;
		rectA.m_Point.y += rectA.m_fHeight / 2.0f;
		rectA.m_Point.z -= rectA.m_fDepth / 2.0f;
		break;

	case eHitRect::DOWN_CENTER:
		rectA.m_Point.x -= rectA.m_fWidth / 2.0f;
		rectA.m_Point.y += rectA.m_fHeight;
		rectA.m_Point.z -= rectA.m_fDepth / 2.0f;
		break;

	default:
		// エラー
		return false;
		break;
	}

	switch(rectB.m_nPointType)
	{
	case eHitRect::CENTER:
		rectB.m_Point.x -= rectB.m_fWidth / 2.0f;
		rectB.m_Point.y += rectB.m_fHeight / 2.0f;
		rectB.m_Point.z -= rectB.m_fDepth / 2.0f;
		break;

	case eHitRect::DOWN_CENTER:
		rectB.m_Point.x -= rectB.m_fWidth / 2.0f;
		rectB.m_Point.y += rectB.m_fHeight;
		rectB.m_Point.z -= rectB.m_fDepth / 2.0f;
		break;

	default:
		// エラー
		return false;
		break;
	}

	
	// 判定
	if(rectA.m_Point.x < rectB.m_Point.x + rectB.m_fWidth && rectA.m_Point.x + rectA.m_fWidth > rectB.m_Point.x &&
		rectA.m_Point.y < rectB.m_Point.y - rectB.m_fHeight && rectA.m_Point.y - rectA.m_fHeight > rectB.m_Point.y && 
		rectA.m_Point.z < rectB.m_Point.z + rectB.m_fDepth && rectA.m_Point.z + rectA.m_fDepth > rectB.m_Point.z)
		return true;
	else
		return false;
}


//**関数***************************************************************************
//	概要	:	拡大込み3D矩形あたり判定
//*********************************************************************************
bool CCalc::IntersectBox3DScale(CHitRect rectA , D3DMATRIX matA , CHitRect rectB , D3DMATRIX matB)
{
		// 拡大率計算
	D3DXVECTOR3 ScaleA = D3DXVECTOR3(0,0,0) , ScaleB = D3DXVECTOR3(0,0,0);
	ScaleA.x = D3DXVec3Length(& D3DXVECTOR3(matA._11 , matA._12 , matA._13));
	ScaleA.y = D3DXVec3Length(& D3DXVECTOR3(matA._21 , matA._22 , matA._23));
	ScaleA.z = D3DXVec3Length(& D3DXVECTOR3(matA._31 , matA._32 , matA._33));
	ScaleB.x = D3DXVec3Length(& D3DXVECTOR3(matB._11 , matB._12 , matB._13));
	ScaleB.y = D3DXVec3Length(& D3DXVECTOR3(matB._21 , matB._22 , matB._23));
	ScaleB.z = D3DXVec3Length(& D3DXVECTOR3(matB._31 , matB._32 , matB._33));

	rectA.m_fWidth *= ScaleA.x;
	rectA.m_fHeight *= ScaleA.y;
	rectA.m_fDepth *= ScaleA.z;
	rectB.m_fWidth *= ScaleB.x;
	rectB.m_fHeight *= ScaleB.y;
	rectB.m_fDepth *= ScaleB.z;

	// 左上手前に位置合わせ
	switch(rectA.m_nPointType)
	{
	case eHitRect::CENTER:
		rectA.m_Point.x -= rectA.m_fWidth / 2.0f;
		rectA.m_Point.y += rectA.m_fHeight / 2.0f;
		rectA.m_Point.z -= rectA.m_fDepth / 2.0f;
		break;

	case eHitRect::DOWN_CENTER:
		rectA.m_Point.x -= rectA.m_fWidth / 2.0f;
		rectA.m_Point.y += rectA.m_fHeight;
		rectA.m_Point.z -= rectA.m_fDepth / 2.0f;
		break;

	default:
		// エラー
		return false;
		break;
	}

	switch(rectB.m_nPointType)
	{
	case eHitRect::CENTER:
		rectB.m_Point.x -= rectB.m_fWidth / 2.0f;
		rectB.m_Point.y += rectB.m_fHeight / 2.0f;
		rectB.m_Point.z -= rectB.m_fDepth / 2.0f;
		break;

	case eHitRect::DOWN_CENTER:
		rectB.m_Point.x -= rectB.m_fWidth / 2.0f;
		rectB.m_Point.y += rectB.m_fHeight;
		rectB.m_Point.z -= rectB.m_fDepth / 2.0f;
		break;

	default:
		// エラー
		return false;
		break;
	}
	
	// 判定
	if(rectA.m_Point.x < rectB.m_Point.x + rectB.m_fWidth && rectA.m_Point.x + rectA.m_fWidth > rectB.m_Point.x &&
		rectA.m_Point.y < rectB.m_Point.y - rectB.m_fHeight && rectA.m_Point.y - rectA.m_fHeight > rectB.m_Point.y && 
		rectA.m_Point.z < rectB.m_Point.z + rectB.m_fDepth && rectA.m_Point.z + rectA.m_fDepth > rectB.m_Point.z)
		return true;
	else
		return false;
}


//**関数***************************************************************************
//	概要	:	拡大球判定
//*********************************************************************************
bool CCalc::IntersectSphereScale(CHitCircle circleA , D3DMATRIX matA , CHitCircle circleB , D3DXMATRIX matB)
{
	// ワールドマトリクスから拡大率取得
	D3DXVECTOR3 ScaleA = D3DXVECTOR3(0,0,0) , ScaleB = D3DXVECTOR3(0,0,0);
	ScaleA.x = D3DXVec3Length(&D3DXVECTOR3(matA._11 , matA._12 , matA._13));
	//ScaleA.y = D3DXVec3Length(&D3DXVECTOR3(matA._21 , matA._22 , matA._23));
	ScaleB.x = D3DXVec3Length(&D3DXVECTOR3(matB._11 , matB._12 , matB._13));
	//ScaleB.y = D3DXVec3Length(&D3DXVECTOR3(matB._21 , matB._22 , matB._23));

	float fDist = D3DXVec3LengthSq(&(circleA.m_Point - circleB.m_Point));
	float fRadi = powf(circleA.m_fRadius * ScaleA.x + circleB.m_fRadius * ScaleB.x , 2.0f);

	if(fRadi > fDist)	return true;
	else				return false;
}


//**関数***************************************************************************
//	概要	:	拡大球判定
//*********************************************************************************
bool CCalc::IntersectSphere(CHitCircle circleA , CHitCircle circleB)
{
	float fDist = D3DXVec3LengthSq(&(circleA.m_Point - circleB.m_Point));
	float fRadi = powf(circleA.m_fRadius + circleB.m_fRadius , 2.0f);

	if(fRadi > fDist)	return true;
	else				return false;
}


//**関数***************************************************************************
//	概要	:	OBBあたり判定
//*********************************************************************************
bool CCalc::IntersectOBB(S_HANDLE hObjA , S_HANDLE hObjB)
{
	// 計算用バッファ
	CObj3D*	pObjA , *pObjB;
	pObjA = REGISTER_H_P(hObjA , CObj3D*);
	pObjB = REGISTER_H_P(hObjB , CObj3D*);

	D3DXMATRIX matA = pObjA ->GetMatrix();
	D3DXMATRIX matB = pObjB->GetMatrix();

	// 両オブジェの法線ベクトル取得
	D3DXVECTOR3	vNormal[6];
	// pObjA
	vNormal[0] = D3DXVECTOR3(matA._11 , matA._12 , matA._13);
	vNormal[1] = D3DXVECTOR3(matA._21 , matA._22 , matA._23);
	vNormal[2] = D3DXVECTOR3(matA._31 , matA._32 , matA._33);
	// pObjB
	vNormal[3] = D3DXVECTOR3(matB._11 , matB._12 , matB._13);
	vNormal[4] = D3DXVECTOR3(matB._21 , matB._22 , matB._23);
	vNormal[5] = D3DXVECTOR3(matB._31 , matB._32 , matB._33);


	// 両オブジェOBBの長さを反映
	D3DXVECTOR3	vHalf[6];
	// pObjA
	vHalf[0] = vNormal[0] * pObjA->GetMesh()->GetObbHalf().x;
	vHalf[1] = vNormal[1] * pObjA->GetMesh()->GetObbHalf().y;
	vHalf[2] = vNormal[2] * pObjA->GetMesh()->GetObbHalf().z;
	// pObjB
	vHalf[3] = vNormal[3] * pObjB->GetMesh()->GetObbHalf().x;
	vHalf[4] = vNormal[4] * pObjB->GetMesh()->GetObbHalf().y;
	vHalf[5] = vNormal[5] * pObjB->GetMesh()->GetObbHalf().z;

	// 中心間のベクトルを計算
	D3DXVECTOR3 vACenter , vBCenter , vDiff;
	D3DXVec3TransformCoord(&vACenter , &pObjA->GetMesh()->GetCenter() , &matA);
	D3DXVec3TransformCoord(&vBCenter , &pObjB->GetMesh()->GetCenter() , &matB);
	vDiff = vBCenter - vACenter;

	// それぞれのローカル座標軸の各軸が分離軸候補
	float fDiff , fHalf;
	for(int i = 0 ; i < 6 ; i ++)
	{
		D3DXVECTOR3* pVSep = &vNormal[i];			// 分離軸候補

		// 中心間距離を正の数にして投影
		fDiff = fabs(D3DXVec3Dot(&vDiff , pVSep));

		// 各OBBの半分のサイズの分離軸へ投影
		fHalf = 0.0f;
		for(int j = 0 ; j < 6 ; j ++)
			fHalf += fabs( D3DXVec3Dot(&vHalf[j] , pVSep) );

		if(fDiff > fHalf) return false;
	}

	// 座標軸の組み合わせが分離軸候補
	D3DXVECTOR3	vSep;

	for(int i = 0 ; i < 3 ; i ++)
	{
		for(int j = 3 ; j < 6 ; j ++)
		{
			D3DXVec3Cross(&vSep , &vNormal[i] , &vNormal[j]);
			D3DXVec3Normalize(&vSep , &vSep);	// 分離軸候補

			// fDiff中心間の距離の投影
			fDiff = fabs(D3DXVec3Dot(&vDiff , &vSep));

			// 各OBBの分離軸への投影
			fHalf = 0.0f;

			for(int k = 0 ; k < 6 ; k ++)
				fHalf += fabs(D3DXVec3Dot(&vHalf[k] , &vSep));

			if(fDiff > fHalf)return false;
		}
	}

	return true;
}


//**関数***************************************************************************
//	概要	:	OBBあたり判定
//*********************************************************************************
bool CCalc::IntersectOBB(D3DXMATRIX matA , D3DXVECTOR3 ObbHalfA , D3DXVECTOR3 CenterA , D3DXMATRIX matB , D3DXVECTOR3 ObbHalfB , D3DXVECTOR3 CenterB)
{
	// 両オブジェの法線ベクトル取得
	D3DXVECTOR3	vNormal[6];
	// pObjA
	vNormal[0] = D3DXVECTOR3(matA._11 , matA._12 , matA._13);
	vNormal[1] = D3DXVECTOR3(matA._21 , matA._22 , matA._23);
	vNormal[2] = D3DXVECTOR3(matA._31 , matA._32 , matA._33);
	// pObjB
	vNormal[3] = D3DXVECTOR3(matB._11 , matB._12 , matB._13);
	vNormal[4] = D3DXVECTOR3(matB._21 , matB._22 , matB._23);
	vNormal[5] = D3DXVECTOR3(matB._31 , matB._32 , matB._33);


	// 両オブジェOBBの長さを反映
	D3DXVECTOR3	vHalf[6];
	// pObjA
	vHalf[0] = vNormal[0] * ObbHalfA.x;
	vHalf[1] = vNormal[1] * ObbHalfA.y;
	vHalf[2] = vNormal[2] * ObbHalfA.z;
	// pObjB
	vHalf[3] = vNormal[3] * ObbHalfB.x;
	vHalf[4] = vNormal[4] * ObbHalfB.y;
	vHalf[5] = vNormal[5] * ObbHalfB.z;

	// 中心間のベクトルを計算
	D3DXVECTOR3 vACenter , vBCenter , vDiff;
	D3DXVec3TransformCoord(&vACenter , &CenterA , &matA);
	D3DXVec3TransformCoord(&vBCenter , &CenterB , &matB);
	vDiff = vBCenter - vACenter;

	// それぞれのローカル座標軸の各軸が分離軸候補
	float fDiff , fHalf;
	for(int i = 0 ; i < 6 ; i ++)
	{
		D3DXVECTOR3* pVSep = &vNormal[i];			// 分離軸候補

		// 中心間距離を正の数にして投影
		fDiff = fabs(D3DXVec3Dot(&vDiff , pVSep));

		// 各OBBの半分のサイズの分離軸へ投影
		fHalf = 0.0f;
		for(int j = 0 ; j < 6 ; j ++)
			fHalf += fabs( D3DXVec3Dot(&vHalf[j] , pVSep) );

		if(fDiff > fHalf) return false;
	}

	// 座標軸の組み合わせが分離軸候補
	D3DXVECTOR3	vSep;

	for(int i = 0 ; i < 3 ; i ++)
	{
		for(int j = 3 ; j < 6 ; j ++)
		{
			D3DXVec3Cross(&vSep , &vNormal[i] , &vNormal[j]);
			D3DXVec3Normalize(&vSep , &vSep);	// 分離軸候補

			// fDiff中心間の距離の投影
			fDiff = fabs(D3DXVec3Dot(&vDiff , &vSep));

			// 各OBBの分離軸への投影
			fHalf = 0.0f;

			for(int k = 0 ; k < 6 ; k ++)
				fHalf += fabs(D3DXVec3Dot(&vHalf[k] , &vSep));

			if(fDiff > fHalf)return false;
		}
	}

	return true;
}


//**関数***************************************************************************
//	概要	:	三角形と線分のあたり判定
//*********************************************************************************
bool CCalc::IntersectDeltaAndLine(CHitDelta delta , CHitLine line)
{
	D3DXVECTOR3	n1 , n2[3];
	
	// 法線計算
	D3DXVec3Cross(&n1 , &(delta.m_Points[1] - delta.m_Points[0]) , &(delta.m_Points[2] - delta.m_Points[1]));
	D3DXVec3Normalize(&n1 , &n1);
	for(int i = 0 ; i < 3 ; i ++)
	{
		if(i != 2)
			D3DXVec3Cross(&n2[i] , &(delta.m_Points[i + 1] - delta.m_Points[i]) , &(line.m_EPoint - line.m_SPoint));
		else
			D3DXVec3Cross(&n2[i] , &(delta.m_Points[i] - delta.m_Points[0]) , &(line.m_EPoint - line.m_SPoint));
		D3DXVec3Normalize(&n2[i] , &n2[i]);
	}


	// --Phase1--
	float	fBuf1 , fBuf2[2];

	fBuf1 = D3DXVec3Dot(&n1 , &delta.m_Points[0]);
	
	fBuf2[0] = D3DXVec3Dot(&n1 , &line.m_SPoint);
	fBuf2[1] = D3DXVec3Dot(&n1 , &line.m_EPoint);

	// 分離平面ができていたらfalseを返す
	if(! ((fBuf1 > fBuf2[0] && fBuf1 < fBuf2[1]) || (fBuf1 < fBuf2[0] && fBuf1 > fBuf2[1])))
		return false;


	// --Phase2--
	for(int i = 0 ; i < 3 ; i ++)
	{
		fBuf1 = D3DXVec3Dot(&n2[i] , &line.m_SPoint);

		for(int j = 0 , nCnt = 0; j < 3 ; j ++)
		{
			if(i == j)
				continue;

			fBuf2[nCnt] = D3DXVec3Dot(&n2[i] , &delta.m_Points[j]);
			nCnt ++;
		}

		if(! ((fBuf1 > fBuf2[0] && fBuf1 < fBuf2[1]) || (fBuf1 < fBuf2[0] && fBuf1 > fBuf2[1])))
			return false;
	}

	return true;
}


//**関数***************************************************************************
//	概要	:	2D衝突判定
//*********************************************************************************
void CCalc::Bound2D(CBoundData* boundA , CBoundData* boundB)
{
	D3DXVECTOR2 VelA , VelB;

	VelA = (D3DXVECTOR2)boundA->m_Velocity;
	VelB = (D3DXVECTOR2)boundB->m_Velocity;
	float fWaA = boundA->m_fMass;
	float fWaB = boundB->m_fMass;
	float fBou = boundA->m_fBounds;

	// ベクトル計算
	boundA->m_Velocity = (D3DXVECTOR3) ((fWaA - fBou * fWaB) * VelA + ((1 + fBou) * fWaB * VelB)) / (fWaA + fWaB);
	boundB->m_Velocity = (D3DXVECTOR3) ((fWaB - fBou * fWaA) * VelB + ((1 + fBou) * fWaA * VelA)) / (fWaB + fWaA);
}


//**関数***************************************************************************
//	概要	:	反射計算
//*********************************************************************************
void CCalc::Bound3D(CBoundData* boundA , CBoundData* boundB)
{
	D3DXVECTOR3 VelA , VelB;

	D3DXVECTOR3 VecAtoB;					// オブジェクトの方向
	D3DXVECTOR3 VecBtoA;					// オブジェクトの方向

	D3DXVECTOR3 vecBuf;
	float		fBuf;						// 計算用バッファ

	VelA = boundA->m_Velocity;
	VelB = boundB->m_Velocity;
	float fWaA = boundA->m_fMass;
	float fWaB = boundB->m_fMass;
	float fBou = boundA->m_fBounds;

	// オブジェクトの衝突方向を計算
	VecAtoB = boundB->m_Pos - boundA->m_Pos;	// AからBへのベクトル
	D3DXVec3Normalize(&VecAtoB , &VecAtoB);
	VecBtoA = boundA->m_Pos - boundB->m_Pos;	// BからAへのベクトル
	D3DXVec3Normalize(&VecBtoA , &VecBtoA);

	// Aの速度ベクトルを分解
	fBuf = D3DXVec3Dot(&VelA , &VecAtoB);
	VecAtoB = VecAtoB * fBuf;					// 分解したベクトルを保存
	VelA -= VecAtoB;							// 継続して持つベクトルを逃がす
	
	// Bの速度ベクトル
	fBuf = D3DXVec3Dot(&VelB , &VecBtoA);
	VecBtoA = VecBtoA * fBuf;
	VelB -= VecBtoA;

	// ベクトル計算	
	boundA->m_Velocity = ((fWaA - fBou * fWaB) * VecAtoB + ((1 + fBou) * fWaB * VecBtoA)) / (fWaA + fWaB);
	boundB->m_Velocity = ((fWaB - fBou * fWaA) * VecBtoA + ((1 + fBou) * fWaA * VecAtoB)) / (fWaB + fWaA);
	boundA->m_Velocity += VelA;
	boundB->m_Velocity += VelB;

	// 衝突回避位置に移動
	D3DXVECTOR3 vec = boundB->m_Pos - boundA->m_Pos;
	D3DXVec3Normalize(&vec , &vec);
	boundB->m_Pos.x = boundA->m_Pos.x + vec.x * (boundA->m_fRadius + boundB->m_fRadius + 0.5f);
	boundB->m_Pos.y = boundA->m_Pos.y + vec.y * (boundA->m_fRadius + boundB->m_fRadius + 0.5f);
	boundB->m_Pos.z = boundA->m_Pos.z + vec.z * (boundA->m_fRadius + boundB->m_fRadius + 0.5f);
}


//**関数***************************************************************************
//	概要	:	壁に対する反射計算
//*********************************************************************************
void CCalc::WallBound(CBoundData* wall , D3DXVECTOR3 wallVec , CBoundData* boundB)
{
	D3DXVECTOR3 VelA = D3DXVECTOR3(0,0,0) , VelB = D3DXVECTOR3(0,0,0);

	D3DXVECTOR3 VecAtoB = D3DXVECTOR3(0,0,0);					// オブジェクトの方向
	D3DXVECTOR3 VecBtoA = D3DXVECTOR3(0,0,0);					// オブジェクトの方向

	D3DXVECTOR3 vecBuf = D3DXVECTOR3(0,0,0);
	float		fBuf;						// 計算用バッファ

	VelA = wall->m_Velocity;
	VelB = boundB->m_Velocity;
	float fWaA = wall->m_fMass;
	float fWaB = boundB->m_fMass;
	float fBou = wall->m_fBounds;

	// オブジェクトの衝突方向を計算
	VecAtoB = wallVec;						// 壁の面ベクトル
	D3DXVec3Normalize(&VecAtoB , &VecAtoB);
	VecBtoA = -1 * wallVec;					// 壁の面ベクトルを反転
	D3DXVec3Normalize(&VecBtoA , &VecBtoA);


	// Aの速度ベクトルを分解
	fBuf = D3DXVec3Dot(&VelA , &VecAtoB);
	VecAtoB = VecAtoB * fBuf;				// 分解したベクトルを保存
	VelA -= VecAtoB;						// 継続して持つベクトルを逃がす
	
	// Bの速度ベクトル
	fBuf = D3DXVec3Dot(&VelB , &VecBtoA);
	VecBtoA = VecBtoA * fBuf;
	VelB -= VecBtoA;

	// ベクトル計算	
	wall->m_Velocity = ((fWaA - fBou * fWaB) * VecAtoB + ((1 + fBou) * fWaB * VecBtoA)) / (fWaA + fWaB);
	boundB->m_Velocity = ((fWaB - fBou * fWaA) * VecBtoA + ((1 + fBou) * fWaA * VecAtoB)) / (fWaB + fWaA);
	wall->m_Velocity += VelA;
	boundB->m_Velocity += VelB;

	// 衝突回避位置に移動
	D3DXVECTOR3 vec = wallVec;
	D3DXVec3Normalize(&vec , &vec);

	if(vec.x != 0)
		boundB->m_Pos.x = wall->m_Pos.x + vec.x * (wall->m_fRadius + boundB->m_fRadius + 1.0f);
	if(vec.y != 0)
		boundB->m_Pos.y = wall->m_Pos.y + vec.y * (wall->m_fRadius + boundB->m_fRadius + 1.0f);
	if(vec.z != 0)
		boundB->m_Pos.z = wall->m_Pos.z + vec.z * (wall->m_fRadius + boundB->m_fRadius + 1.0f);
	
}


//**関数***************************************************************************
//	概要	:	曲線計算(エルミート曲線)
//*********************************************************************************
D3DXVECTOR3	CCalc::CurvHermite(float t ,D3DXVECTOR3 Spos,D3DXVECTOR3 Epos,D3DXVECTOR3 vec1,D3DXVECTOR3 vec2)
{
	while(t > 1.0f)
		t -= 1.0f;

	D3DXVECTOR3 Out;
	D3DXVec3Hermite(&Out,
					&Spos,
					&vec1,
					&Epos,
					&vec2,
					t);

	return Out;
}

//**関数***************************************************************************
//	概要	:	曲線計算(ベジエ曲線)
//*********************************************************************************
D3DXVECTOR3	CCalc::CurvBezier(float t ,D3DXVECTOR3 Spos,D3DXVECTOR3 Epos,D3DXVECTOR3 vec1,D3DXVECTOR3 vec2)
{
	while(t > 1.0f)
		t -= 1.0f;
	
	D3DXVECTOR3 Out;
	Out.x = (1-t)*(1-t)*(1-t)*Spos.x + 3*(1-t)*(1-t)*t*vec1.x + 3*(1-t)*t*t*vec2.x + t*t*t*Epos.x;
	Out.y = (1-t)*(1-t)*(1-t)*Spos.y + 3*(1-t)*(1-t)*t*vec1.y + 3*(1-t)*t*t*vec2.y + t*t*t*Epos.y;
	Out.z = (1-t)*(1-t)*(1-t)*Spos.z + 3*(1-t)*(1-t)*t*vec1.z + 3*(1-t)*t*t*vec2.z + t*t*t*Epos.z;

	return Out;
	
}

//**関数***************************************************************************
//	概要	:	距離計算
//*********************************************************************************
float CCalc::Range(D3DXVECTOR3 pos1,D3DXVECTOR3 pos2)
{
	float Out;

	Out = sqrt(pow(pos1.x-pos2.x,2)+pow(pos1.y-pos2.y,2)+pow(pos1.z-pos2.z,2));
	
	return Out;
}





//=================================================================================
//	End of File
//=================================================================================