//�v���O�����w�b�_*****************************************************************
//	�v���O�����T�v	:	�����蔻����N���X��`
//*********************************************************************************
#pragma once

//==include��======================================================================
#include <d3d9.h>
#include <d3dx9.h>

#pragma comment(lib, "d3d9.lib")
#pragma comment(lib, "d3dx9.lib")
#pragma comment(lib, "dxguid.lib")
//=================================================================================


//==�萔�E�񋓌^�錾��=============================================================
namespace eHitRect
{
	enum
	{
		CENTER,							// ����
		DOWN_CENTER,					// ��������
		UP_LEFT_2D,						// 2D�p_����
		DOWN_RIGHT_2D,					// 2D�p_�E��
	};
};
//=================================================================================


//==�}�N����`��===================================================================

//=================================================================================


//==�N���X�E�\���̐錾��===========================================================
// �����蔻��p��`�f�[�^
class CHitRect
{
public:
	float		m_fWidth , m_fHeight , m_fDepth;				// ���� , �c�� , ���s
	int			m_nPointType;									// ����_�̃^�C�v
	D3DXVECTOR3	m_Point;										// ����_

	CHitRect();
	CHitRect(float fwidth , float fHeight , D3DXVECTOR3 point ,
			 int nPoint);										// 2D�p�����l
	CHitRect(float fwidth , float fHeight , float fDepth , 
			D3DXVECTOR3 point , int nPoint);					// 3D�p�����l
	virtual ~CHitRect();
};

class CHitCircle
{
public:
	float		m_fRadius;										// ���a
	D3DXVECTOR3	m_Point;										// ����_

	CHitCircle();
	CHitCircle(float fRadius , D3DXVECTOR3 point);				// �����t���R���X�g���N�^
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
	D3DXVECTOR3	m_SPoint , m_EPoint;							// �J�n�_�A�I���X
	
	CHitLine();
	CHitLine(D3DXVECTOR3 startPoint , D3DXVECTOR3 endPoint);
	virtual ~CHitLine();
};


class CHitCapsul
{
public:
	D3DXVECTOR3	m_SPoint , m_EPoint;							// �J�n�_�A�I���X
	
	CHitCapsul();
	CHitCapsul(D3DXVECTOR3 startPoint , D3DXVECTOR3 endPoint , float fRad);
	virtual ~CHitCapsul();
};
//=================================================================================


//=================================================================================
//	End of File
//=================================================================================