//=======================================================================================
//
//	  ���b�V�� �N���X
//
//=======================================================================================
#define _CRT_SECURE_NO_WARNINGS
#define _CRT_NON_CONFORMING_SWPRINTFS
#include <stdio.h>
#include "../System/common.h"

#include "Mesh.h"
#include "../DirectX/Graphics.h"

//**�֐�***************************************************************************
// �R���X�g���N�^
//*********************************************************************************
CMesh::CMesh()
{
	m_pD3DMesh = NULL;
	m_dwNumMaterial = 0;
	m_pMaterial = NULL;
	m_ppTexture = NULL;

	m_pAttr = NULL;
	m_dwAttr = 0;
	m_dwVtx = 0;
	m_pVtx = NULL;
	m_dwIdx = 0;
	m_pIdx = NULL;

	m_vHalf = D3DXVECTOR3(0, 0, 0);
	m_vCenter = D3DXVECTOR3(0, 0, 0);
	m_fRadius = 0;
	m_pSphere = NULL;
	m_pSphere = NULL;


	m_pPiece = NULL;
	m_pPieceVtx = NULL;
}

//**�֐�***************************************************************************
// �f�X�g���N�^
//*********************************************************************************
CMesh::~CMesh()
{
	Finalize();
}

//**�֐�***************************************************************************
// ���b�V��������
//*********************************************************************************
bool CMesh::Initialize(LPCTSTR pszFName)
{
	TCHAR			szMsg[MAX_PATH + 32];
	TCHAR			szDir[_MAX_DIR];
	TCHAR			szCurrentDir[_MAX_PATH];

	LPD3DXBUFFER	pD3DXMtrlBuffer = NULL;

	// �t�H���_���𒊏o
	_tsplitpath(pszFName, NULL, szDir, NULL, NULL);

	// �w�t�@�C�����烁�b�V���f�[�^��ǂݍ���
	HRESULT hr = D3DXLoadMeshFromX(pszFName, D3DXMESH_SYSTEMMEM, CGraphics::GetDevice(),
		NULL, &pD3DXMtrlBuffer, NULL, &m_dwNumMaterial, &m_pD3DMesh);
	if (FAILED(hr)) {
		_stprintf(szMsg, _T("X�t�@�C��(%s)�̓ǂݍ��݂Ɏ��s���܂����B"), pszFName);
		MessageBox(NULL, szMsg, NULL, MB_OK);
		return false;
	}

	// FVF�`����␳(���_�t�H�[�}�b�g��ϊ�)
	LPD3DXMESH pMeshTmp;
	DWORD dwFVF = m_pD3DMesh->GetFVF();
	if (dwFVF != FVF_BVERTEX) {
		hr = m_pD3DMesh->CloneMeshFVF(m_pD3DMesh->GetOptions(), FVF_BVERTEX,
			CGraphics::GetDevice(), &pMeshTmp);
		SAFE_RELEASE(m_pD3DMesh);
		if (FAILED(hr)) {
			SAFE_RELEASE(pD3DXMtrlBuffer);
			return false;
		}
		// �@���������ꍇ�͋����I�ɒǉ�
		if ((dwFVF & D3DFVF_NORMAL) == 0) {
			D3DXComputeNormals(pMeshTmp, NULL);
		}
		m_pD3DMesh = pMeshTmp;
	}

	// �����e�[�u���𐶐����邽�߂̍œK��
	hr = m_pD3DMesh->Optimize(D3DXMESHOPT_ATTRSORT, NULL, NULL, NULL, NULL, &pMeshTmp);
	if (SUCCEEDED(hr)) {
		m_pD3DMesh->Release();
		m_pD3DMesh = pMeshTmp;
	} else {
		SAFE_RELEASE(pD3DXMtrlBuffer);
		return false;
	}
	// �����e�[�u���擾
	hr = m_pD3DMesh->GetAttributeTable(NULL, &m_dwAttr);
	if (FAILED(hr)) {
		SAFE_RELEASE(pD3DXMtrlBuffer);
		SAFE_RELEASE(m_pD3DMesh);
		return false;
	}
	m_pAttr = new D3DXATTRIBUTERANGE[m_dwAttr];
	hr = m_pD3DMesh->GetAttributeTable(m_pAttr, &m_dwAttr);
	// ���_�o�b�t�@/�C���f�b�N�X�o�b�t�@�Œ�
	LPVOID pVtx;
	m_pD3DMesh->LockVertexBuffer(D3DLOCK_READONLY, &pVtx);
	LPVOID pIdx;
	m_pD3DMesh->LockIndexBuffer(D3DLOCK_READONLY, &pIdx);
	// ���o�ꏊ�̊m��
	m_dwVtx = m_pD3DMesh->GetNumVertices();
	m_pVtx = new BVERTEX[m_dwVtx];
	m_dwFace = m_pD3DMesh->GetNumFaces();
	m_dwIdx = m_dwFace * 3;
	m_pIdx = new WORD[m_dwIdx];
	m_pPiece = new PARTICLE[m_dwFace];
	m_pPieceVtx = new BVERTEX[m_dwIdx];
	// �R�s�[
	CopyMemory(m_pVtx, pVtx, sizeof(BVERTEX) * m_dwVtx);
	CopyMemory(m_pIdx, pIdx, sizeof(WORD) * m_dwIdx);
	// ���_�o�b�t�@/�C���f�b�N�X�o�b�t�@���
	m_pD3DMesh->UnlockVertexBuffer();
	m_pD3DMesh->UnlockIndexBuffer();

	// �J�����g�f�B���N�g����ύX
	if (szDir[0]) {
		GetCurrentDirectory(_MAX_PATH, szCurrentDir);
		SetCurrentDirectory(szDir);
	}

	// �}�e���A���ǂݍ���
	D3DXMATERIAL* pD3DMaterials = (D3DXMATERIAL*)pD3DXMtrlBuffer->GetBufferPointer();
	m_pMaterial = new D3DMATERIAL9[m_dwNumMaterial];
	m_ppTexture = new LPDIRECT3DTEXTURE9[m_dwNumMaterial];
	for (DWORD i = 0; i < m_dwNumMaterial; i++) { 
		m_pMaterial[i] = pD3DMaterials[i].MatD3D;
		m_pMaterial[i].Ambient = m_pMaterial[i].Diffuse;
		m_ppTexture[i] = NULL;
		if (pD3DMaterials[i].pTextureFilename && pD3DMaterials[i].pTextureFilename[0]) {
			// �e�N�X�`���t�@�C����ǂݍ���
			if (FAILED(D3DXCreateTextureFromFileA(CGraphics::GetDevice(),
				pD3DMaterials[i].pTextureFilename, &m_ppTexture[i]))) {
				_stprintf(szMsg, _T("�e�N�X�`��(%hs)�̓ǂݍ��݂Ɏ��s���܂����B"),
					pD3DMaterials[i].pTextureFilename);
				MessageBox(NULL, szMsg, NULL, MB_OK);
			}
		}
	}

	// �J�����g�f�B���N�g�������ɖ߂�
	if (szDir[0])
		SetCurrentDirectory(szCurrentDir);

	pD3DXMtrlBuffer->Release();

	// ���E�{�b�N�X����
	D3DXVECTOR3 vMin = m_pVtx[0].pos;
	D3DXVECTOR3 vMax = vMin;
	BVERTEX* pBVtx = m_pVtx + 1;
	for (DWORD i = 1; i < m_dwVtx; i++, pBVtx++) {
		if (vMin.x > pBVtx->pos.x)
			vMin.x = pBVtx->pos.x;
		if (vMin.y > pBVtx->pos.y)
			vMin.y = pBVtx->pos.y;
		if (vMin.z > pBVtx->pos.z)
			vMin.z = pBVtx->pos.z;
		if (vMax.x < pBVtx->pos.x)
			vMax.x = pBVtx->pos.x;
		if (vMax.y < pBVtx->pos.y)
			vMax.y = pBVtx->pos.y;
		if (vMax.z < pBVtx->pos.z)
			vMax.z = pBVtx->pos.z;
	}
	m_vHalf = (vMax - vMin) / 2.0f;
	m_vCenter = (vMax + vMin) / 2.0f;

	// ���E���̐���
	m_fRadius = 0.0f;
	float fR;
	for (DWORD i = 0; i < m_dwVtx; i++) {
		fR = D3DXVec3Length(&(m_pVtx[i].pos - m_vCenter));
		if (m_fRadius < fR)
			m_fRadius = fR;
	}

	// ���E���C���[�W�̐���
	D3DXCreateSphere(CGraphics::GetDevice(),
		m_fRadius, 32, 16, &m_pSphere, NULL);

	// OBB����
	float fWidth = vMax.x - vMin.x;
	float fHeight = vMax.y - vMin.y;
	float fDepth = vMax.z - vMin.z;

	m_vOBBHalf.x = fWidth / 2.0f;
	m_vOBBHalf.y = fHeight / 2.0f;
	m_vOBBHalf.z = fDepth / 2.0f;
	// OBB�C���[�W�̐���
	D3DXCreateBox(CGraphics::GetDevice() , 
		fWidth , fHeight , fDepth , &m_pBox , NULL);

	return true;
}

//**�֐�***************************************************************************
// ���b�V�����
//*********************************************************************************
void CMesh::Finalize()
{
	// ���E���C���[�W�̉��
	SAFE_RELEASE(m_pSphere);

	SAFE_DELETE_ARRAY(m_pPiece);
	SAFE_DELETE_ARRAY(m_pPieceVtx);

	SAFE_DELETE_ARRAY(m_pVtx);
	SAFE_DELETE_ARRAY(m_pIdx);

	SAFE_DELETE_ARRAY(m_pAttr);

	// �e�N�X�`�� �I�u�W�F�N�g�����
	if (m_ppTexture) {
		for (DWORD i = 0; i < m_dwNumMaterial; i++) {
			SAFE_RELEASE(m_ppTexture[i]);
		}
		SAFE_DELETE_ARRAY(m_ppTexture);
	}
	SAFE_DELETE_ARRAY(m_pMaterial);

	SAFE_RELEASE(m_pD3DMesh);	// ���b�V�� �I�u�W�F�N�g�����
}

//**�֐�***************************************************************************
//	�T�v	:	���b�V���`��
//*********************************************************************************
void CMesh::Draw(D3DXMATRIX& world)
{
	// ���[���h �}�g���b�N�X�ݒ�
    LPDIRECT3DDEVICE9 pDevice = CGraphics::GetDevice();
    pDevice->SetTransform(D3DTS_WORLD, &world);

	for (DWORD i = 0; i < m_dwAttr; i++) {
		DWORD id = m_pAttr[i].AttribId;
        // �A���t�@�l���`�F�b�N
        D3DMATERIAL9 mtrl = m_pMaterial[id];
		if (mtrl.Diffuse.a < 1.0f)
			continue;
		pDevice->SetMaterial(&mtrl);
		pDevice->SetTexture(0, m_ppTexture[id]);	// �e�N�X�`����ݒ�
//		m_pD3DMesh->DrawSubset(id);								// �`������s
		pDevice->SetFVF(FVF_BVERTEX);
		pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0,
			m_dwVtx, m_pAttr[i].FaceCount, &m_pIdx[m_pAttr[i].FaceStart * 3],
			D3DFMT_INDEX16, m_pVtx, sizeof(BVERTEX));
	}

	// �A���t�@ �u�����h�L����
    pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	for (DWORD i = 0; i < m_dwAttr; i++) {
		DWORD id = m_pAttr[i].AttribId;
        // �A���t�@�l���`�F�b�N
        D3DMATERIAL9 mtrl = m_pMaterial[id];
		if (mtrl.Diffuse.a >= 1.0f)
			continue;
		pDevice->SetMaterial(&mtrl);
		pDevice->SetTexture(0, m_ppTexture[id]);	// �e�N�X�`����ݒ�
//		m_pD3DMesh->DrawSubset(id);								// �`������s
		pDevice->SetFVF(FVF_BVERTEX);
		pDevice->DrawIndexedPrimitiveUP(D3DPT_TRIANGLELIST, 0,
			m_dwVtx, m_pAttr[i].FaceCount, &m_pIdx[m_pAttr[i].FaceStart * 3],
			D3DFMT_INDEX16, m_pVtx, sizeof(BVERTEX));
	}

    // �A���t�@ �u�����h������
    pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}

//**�֐�***************************************************************************
//	�T�v	:	�������`��
//*********************************************************************************
void CMesh::Draw(D3DXMATRIX& world, float fAlpha)
{
    // ���[���h �}�g���b�N�X�ݒ�
    LPDIRECT3DDEVICE9 pDevice = CGraphics::GetDevice();
    pDevice->SetTransform(D3DTS_WORLD, &world);

    // �A���t�@ �u�����h�L����
    pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

    for (DWORD i = 0; i < m_dwAttr; i++) {
        DWORD id = m_pAttr[i].AttribId;

        // �A���t�@�l��ύX
        D3DMATERIAL9 mtrl = m_pMaterial[id];
        mtrl.Diffuse.a *= fAlpha;
        pDevice->SetMaterial(&mtrl);

        pDevice->SetTexture(0, m_ppTexture[id]);
        pDevice->SetFVF(FVF_BVERTEX);

		pDevice->DrawPrimitiveUP(D3DPT_TRIANGLELIST,
			m_pAttr[i].FaceCount,
			&m_pPieceVtx[m_pAttr[i].FaceStart * 3],
			sizeof(BVERTEX));
    }

    // �A���t�@ �u�����h������
    pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}


//**�֐�***************************************************************************
//	�T�v	:	�f�Ђ̏�����
//*********************************************************************************
void CMesh::InitParticle()
{
	// ���_���W�A�e�N�X�`�����W���R�s�[
	for (DWORD i = 0; i < m_dwIdx; ++i) {
		m_pPieceVtx[i].pos = m_pVtx[m_pIdx[i]].pos;
		m_pPieceVtx[i].tex = m_pVtx[m_pIdx[i]].tex;
	}
	D3DXVECTOR3 v0, v1, n;
	for (DWORD i = 0; i < m_dwFace; ++i) {
		// �@���x�N�g���𐶐�
		v0 = m_pPieceVtx[i * 3 + 1].pos
			- m_pPieceVtx[i * 3 + 0].pos;
		v1 = m_pPieceVtx[i * 3 + 2].pos
			- m_pPieceVtx[i * 3 + 1].pos;
		D3DXVec3Cross(&n, &v0, &v1);
		D3DXVec3Normalize(&n, &n);
		m_pPieceVtx[i * 3 + 0].nor = n;
		m_pPieceVtx[i * 3 + 1].nor = n;
		m_pPieceVtx[i * 3 + 2].nor = n;
		// ���x�A�����x�̏�����
		m_pPiece[i].spd = n * INIT_SPD;
		m_pPiece[i].acl =
			D3DXVECTOR3(0.0f, -GRAVITY, 0.0f);
	}
}


//**�֐�***************************************************************************
//	�T�v	:	�f�Ђ̈ړ�
//*********************************************************************************
void CMesh::UpdateParticle()
{
	for (DWORD i = 0; i < m_dwFace; ++i) {
		m_pPieceVtx[i * 3 + 0].pos += m_pPiece[i].spd;
		m_pPieceVtx[i * 3 + 1].pos += m_pPiece[i].spd;
		m_pPieceVtx[i * 3 + 2].pos += m_pPiece[i].spd;
		m_pPiece[i].spd += m_pPiece[i].acl;
	}
}


//**�֐�***************************************************************************
//	�T�v	:	���E���̕`��
//*********************************************************************************
void CMesh::DrawSphere(D3DXMATRIX& world, D3DCOLORVALUE color)
{
    LPDIRECT3DDEVICE9 pDevice = CGraphics::GetDevice();

	// ���[���h �}�g���b�N�X�ݒ�
	D3DXMATRIX m;
	D3DXMatrixTranslation(&m, m_vCenter.x, m_vCenter.y, m_vCenter.z);
	m *= world;
    pDevice->SetTransform(D3DTS_WORLD, &m);

    // �A���t�@ �u�����h�L����
	//pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    //pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    //pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// ���b�V���`��
	D3DMATERIAL9 mtrl = {
		color,//Diffuse
		color,//Ambient
		{1.0f, 1.0f, 1.0f, 1.0f},//Specular
		{0.0f, 0.0f, 0.0f, 1.0f},//Emissive
		1.0f//Power
	};
	pDevice->SetMaterial(&mtrl);
	pDevice->SetTexture(0, NULL);
	pDevice->SetFVF(D3DFVF_XYZ |
		D3DFVF_NORMAL | D3DFVF_TEX1);
	m_pSphere->DrawSubset(0);

    // �A���t�@ �u�����h������
    //pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}


//**�֐�***************************************************************************
//	�T�v	:	OBB�̕`��
//*********************************************************************************
void CMesh::DrawBox(D3DXMATRIX& world, D3DCOLORVALUE color)
{
    LPDIRECT3DDEVICE9 pDevice = CGraphics::GetDevice();

	// ���[���h �}�g���b�N�X�ݒ�
	D3DXMATRIX m;
	D3DXMatrixTranslation(&m, m_vCenter.x, m_vCenter.y, m_vCenter.z);
	m *= world;
    pDevice->SetTransform(D3DTS_WORLD, &m);

    // �A���t�@ �u�����h�L����
	pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    pDevice->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    pDevice->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);

	// ���b�V���`��
	D3DMATERIAL9 mtrl = {
		color,//Diffuse
		color,//Ambient
		{1.0f, 1.0f, 1.0f, 1.0f},//Specular
		{0.0f, 0.0f, 0.0f, 1.0f},//Emissive
		1.0f//Power
	};

	pDevice->SetMaterial(&mtrl);
	pDevice->SetTexture(0, NULL);
	pDevice->SetFVF(D3DFVF_XYZ |
		D3DFVF_NORMAL | D3DFVF_TEX1);
	m_pBox->DrawSubset(0);

    // �A���t�@ �u�����h������
    pDevice->SetRenderState(D3DRS_ALPHABLENDENABLE, FALSE);
}


//**�֐�***************************************************************************
//	�T�v	:	���b�V���s���������̂ݕ`��
//*********************************************************************************
void CMesh::DrawNoAlpha(D3DXMATRIX& world)
{
	// ���[���h �}�g���b�N�X�ݒ�
    LPDIRECT3DDEVICE9 pDevice = CGraphics::GetDevice();
    pDevice->SetTransform(D3DTS_WORLD, &world);

	for (DWORD i = 0; i < m_dwAttr; i++) {
		DWORD id = m_pAttr[i].AttribId;
        // �A���t�@�l���`�F�b�N
        D3DMATERIAL9 mtrl = m_pMaterial[id];
		if (mtrl.Diffuse.a < 1.0f)
			continue;
		pDevice->SetMaterial(&mtrl);
		pDevice->SetTexture(0, m_ppTexture[id]);	// �e�N�X�`����ݒ�
		m_pD3DMesh->DrawSubset(id);								// �`������s
	}
}


//**�֐�***************************************************************************
//	�T�v	:	���b�V�������������̂ݕ`�� (�A���t�@�L����/�������Ȃ�)
//*********************************************************************************
void CMesh::DrawAlpha(D3DXMATRIX& world)
{
	// ���[���h �}�g���b�N�X�ݒ�
    LPDIRECT3DDEVICE9 pDevice = CGraphics::GetDevice();
    pDevice->SetTransform(D3DTS_WORLD, &world);

	for (DWORD i = 0; i < m_dwAttr; i++) {
		DWORD id = m_pAttr[i].AttribId;
        // �A���t�@�l���`�F�b�N
        D3DMATERIAL9 mtrl = m_pMaterial[id];
		if (mtrl.Diffuse.a >= 1.0f)
			continue;
		pDevice->SetMaterial(&mtrl);
		pDevice->SetTexture(0, m_ppTexture[id]);	// �e�N�X�`����ݒ�
		m_pD3DMesh->DrawSubset(id);					// �`������s
	}
}

//=======================================================================================
//	End of File
//=======================================================================================