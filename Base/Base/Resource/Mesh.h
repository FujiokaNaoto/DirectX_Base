//=======================================================================================
//
//	  ���b�V�� �N���X��`
//
//=======================================================================================
#pragma once
#include <windows.h>		// Windows�v���O�����ɂ͂����t����
#include <tchar.h>			// �ėp�e�L�X�g �}�b�s���O
#include <d3d9.h>			// DirectX Graphics �֘A�̃w�b�_
#include <d3dx9.h>			// DirectX Graphics �֘A�̃w�b�_
#include <dxerr.h>			// DirectX Graphics �֘A�̃w�b�_
#include "../Object/Base/CHANDLE.h"

struct BVERTEX {
	D3DXVECTOR3	pos;
	D3DXVECTOR3	nor;
	D3DXVECTOR2	tex;
};
#define FVF_BVERTEX		(D3DFVF_XYZ|D3DFVF_NORMAL|D3DFVF_TEX1)

struct PARTICLE {
	D3DXVECTOR3 acl;	// �����x
	D3DXVECTOR3 spd;	// ���x
};

#define INIT_SPD	3.0f	// ����(�v����)
#define GRAVITY		0.098f	// �d�͉����x(�v����)

class CMesh
{
private:
	LPD3DXMESH			m_pD3DMesh;			// D3DXMesh �I�u�W�F�N�g
	DWORD				m_dwNumMaterial;	// �}�e���A����
	D3DMATERIAL9*		m_pMaterial;		// �}�e���A��
	LPDIRECT3DTEXTURE9*	m_ppTexture;		// �e�N�X�`��

	D3DXATTRIBUTERANGE*	m_pAttr;			// �����e�[�u��
	DWORD				m_dwAttr;			// ������
	DWORD				m_dwVtx;			// ���_��
	BVERTEX*			m_pVtx;				// ���_�z��
	DWORD				m_dwIdx;			// �C���f�b�N�X��
	WORD*				m_pIdx;				// �C���f�b�N�X�z��

	D3DXVECTOR3			m_vHalf;			// ���E�{�b�N�X �n�[�t�T�C�Y
	D3DXVECTOR3			m_vCenter;			// ���E�{�b�N�X(���E��)���S���W
	float				m_fRadius;			// ���E�����a

	// �f�o�b�O�\��
	LPD3DXMESH			m_pSphere;			// ���E���\���p���b�V��
	LPD3DXMESH			m_pBox;				// OBB�\���p���b�V��

	D3DXVECTOR3			m_vOBBHalf;			// OBB�f�[�^�̔��a

	PARTICLE*			m_pPiece;			// �f�Ђ̐���p
	BVERTEX*			m_pPieceVtx;
	DWORD				m_dwFace;			// �ʐ�

public:
	CMesh();								// �R���X�g���N�^
	virtual ~CMesh();						// �f�X�g���N�^
	bool Initialize(LPCTSTR pszFName);		// ���b�V��������
	void Finalize();						// ���b�V�����
	void Draw(D3DXMATRIX& world);			// ���b�V���`��
	void Draw(D3DXMATRIX& world, float fAlpha);
	DWORD GetNumVertex() {return m_dwVtx;}	// ���_���擾
	BVERTEX* GetVertex() {return m_pVtx;}	// ���_�z��擾
	DWORD GetNumIndex() {return m_dwIdx;}	// �C���f�b�N�X���擾
	WORD* GetIndex() {return m_pIdx;}		// �C���f�b�N�X�z��擾
	D3DXVECTOR3& GetHalfSize() {return m_vHalf;}	// �{�b�N�X�n�[�t�T�C�Y�擾
	D3DXVECTOR3& GetCenter() {return m_vCenter;}	// ���E�{�b�N�X/�����S���W�擾
	float GetRadius() {return m_fRadius;}	// ���E�����a�擾

	D3DXVECTOR3& GetObbHalf(){return m_vOBBHalf;}	// OBB�����̃T�C�Y�擾

	void InitParticle();					// �f�Ђ̏�����
	void UpdateParticle();					// �f�Ђ̈ړ�

	void DrawSphere(D3DXMATRIX& world, D3DCOLORVALUE color);
	void DrawBox(D3DXMATRIX& world, D3DCOLORVALUE color);

	void DrawNoAlpha(D3DXMATRIX& world , S_HANDLE nShadeHndle);
	void DrawAlpha(D3DXMATRIX& world , S_HANDLE nShadeHndle);
};

//=======================================================================================
//	End of File
//=======================================================================================