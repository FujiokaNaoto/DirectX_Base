//�v���O�����w�b�_*****************************************************************
//	�v���O�����T�v	:	DirectX Graphics9 �N���X��`
//*********************************************************************************
//#pragma once

#ifndef __GRAPHICS_H__
#define __GRAPHICS_H__

//==include��======================================================================
#include <d3d9.h>
#include <d3dx9.h>
#include <tchar.h>
//=================================================================================


//==�萔�E�񋓌^�錾��=============================================================
#define FOVY			45.0f				// ����p
#define NEAR_CLIP		0.1f				// �j�A�N���b�v���s������
#define FAR_CLIP		10000.0f			// �t�@�[�N���b�v���s������

// �o�^�t�H���g
enum FONT_TYPE
{
	FONT_DEBUG,								// �f�o�b�O�����t�H���g
	FONT_MESSAGE,							// �Q�[�������b�Z�[�W�t�H���g
	FONT_SCORE,								// �X�R�A�t�H���g

	FONT_MAX
};
//=================================================================================


//==�}�N����`��===================================================================

//=================================================================================


//==�N���X�錾��===================================================================
class CGraphics
{
private:
	LPDIRECT3D9					m_pD3D;					// Direct3D9 �I�u�W�F�N�g
	static LPDIRECT3DDEVICE9	m_pDevice;				// Direct3DDevice9 �I�u�W�F�N�g
	D3DPRESENT_PARAMETERS		m_D3DPP;				// PRESENT PARAMETERS

	LPD3DXFONT					m_pFont[FONT_MAX];		// D3DXFont �I�u�W�F�N�g

	static D3DXMATRIX			m_MatProj;				// �ˉe�}�g���b�N�X

	HWND						m_hWnd;					// �E�B���h�E �n���h��
	int							m_nWidth;				// �\���̈敝
	int							m_nHeight;				// �\���̈捂��


	// �Z�}�t�H
	static HANDLE				m_SemafObjHnd;			// �Z�}�t�H�I�u�W�F�n���h��
	static HANDLE				m_SemafHnd;				// �Z�}�t�H�n���h��

public:
	static void				 SemafoLock();
	static void				 SemafoUnlock();

	static LPDIRECT3DDEVICE9 GetDevice(){return m_pDevice;};
	static D3DXMATRIX		 GetMatProj() {return m_MatProj;}

	static CGraphics* Create(HWND hWnd, int nWidth, int nHeight, bool bWindow);
	void Release();
	bool Begin();
	void End();
	void SwapBuffer();

	void DrawText(int nX, int nY, LPCTSTR psz , int nFontType);			// �t�H���g�`��
	void DrawText(int nX, int nY, LPCTSTR psz , int nFontType ,
					int nRed , int nGreen , int nBlue , int nAlpha);	// �F��񍞂݃t�H���g�`��
	
private:
	bool Initialize(HWND hWnd, int nWidth, int nHeight, bool bWindow);
	void Finalize();

	void FontCreate(int FontType);
};
//=================================================================================

#endif
//=================================================================================
//	End of File
//=================================================================================