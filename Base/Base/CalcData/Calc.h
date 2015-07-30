//�v���O�����w�b�_*****************************************************************
//	�v���O�����T�v	:	�v�Z�N���X��`
//*********************************************************************************
#pragma once

//==include��======================================================================
#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")

#include "BoundData.h"
#include "../Object/Base/CHANDLE.h"
#include "HitData.h"
//=================================================================================


//==�萔�E�񋓌^�錾��=============================================================

//=================================================================================


//==�}�N����`��===================================================================
//=================================================================================


//==�N���X�E�\���̐錾��===========================================================
class CCalc
{
private:

protected:

public:

	// --�����蔻��--
	// --2D--
	// ��`������
	static bool IntersectBox2D(CHitRect rectA , CHitRect rectB);
	static bool IntersectBox2DScale(CHitRect rectA , D3DMATRIX matA , CHitRect rectB , D3DMATRIX matB);		// �g�卞�ݓ�����

	// 2D�~������
	static bool IntersectCircle(CHitCircle circleA , CHitCircle circleB);
	// �g�卞�݁A2D�~������
	static bool IntersectCircleScale(CHitCircle circleA , D3DMATRIX matA , CHitCircle circleB , D3DXMATRIX matB);		// �g�卞�ݓ�����

	// �~�Ƌ�`�̓�����
	static bool	IntersectBoxAndCircle(CHitRect rect , CHitCircle circle);
	static bool	IntersectBoxAndCircleScale(CHitRect rect , D3DMATRIX matRect , CHitCircle circle , D3DMATRIX matCircle);


	// --3D--
	// 3D��`
	static bool IntersectBox3D(CHitRect rectA , CHitRect rectB);
	static bool IntersectBox3DScale(CHitRect rectA , D3DMATRIX matA , CHitRect rectB , D3DMATRIX matB);		// �g�卞�ݓ�����

	// 3D��������
	static bool IntersectSphere(CHitCircle circleA , CHitCircle circleB);
	static bool IntersectSphereScale(CHitCircle circleA , D3DMATRIX matA , CHitCircle circleB , D3DXMATRIX matB);		// �g�卞�ݓ�����

	// 3DOBB������
	static bool IntersectOBB(S_HANDLE pObjA , S_HANDLE pObjB);
	static bool IntersectOBB(D3DXMATRIX matA , D3DXVECTOR3 ObbHalfA , D3DXVECTOR3 CenterA , D3DXMATRIX matB , D3DXVECTOR3 ObbHalfB , D3DXVECTOR3 CenterB);

	// �O�p�`�Ɛ����̂����蔻��
	static bool	IntersectDeltaAndLine(CHitDelta delta , CHitLine line);

	// --�Փ˔���--
	static void	Bound2D(CBoundData* boundA , CBoundData* boundB);
	static void	Bound3D(CBoundData* boundA , CBoundData* boundB);
	
	static void	WallBound(CBoundData* wall , D3DXVECTOR3 wallVec , CBoundData* boundB);
	
	//�Ȑ��v�Z
	static D3DXVECTOR3	CurvHermite(float t ,D3DXVECTOR3 Spos,D3DXVECTOR3 Epos,D3DXVECTOR3 vec1,D3DXVECTOR3 vec2);
	static D3DXVECTOR3	CurvBezier(float t ,D3DXVECTOR3 Spos,D3DXVECTOR3 Epos,D3DXVECTOR3 vec1,D3DXVECTOR3 vec2);

	//2�_�̋����v�Z
	static float Range(D3DXVECTOR3 pos1,D3DXVECTOR3 pos2);
};
//=================================================================================


//=================================================================================
//	End of File
//=================================================================================